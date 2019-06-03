#include "cboard.h"

#include "csettings.h"
#include "cbackgroundmanager.h"
#include "ctilesmanager.h"
#include "crecordsmanager.h"

#ifdef DEFINED_OPENGL
#include <QtOpenGL/QGLWidget>
#endif

#include <QResizeEvent>

#include <QDebug>

CBoard::CBoard(QWidget *parent) : QGraphicsView(parent),
    m_field(new CField(this)),
    m_scene(nullptr)
{
    setFrameStyle(QFrame::NoFrame);
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setOptimizationFlag(QGraphicsView::DontAdjustForAntialiasing);

    setResizeAnchor(QGraphicsView::AnchorViewCenter);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Заполним массив типов полей
    m_field_types.insert(fz14x6, {14, 6, 4});
    m_field_types.insert(fz16x9, {16, 9, 4});
    m_field_types.insert(fz18x8, {18, 8, 4});
    m_field_types.insert(fz24x12, {24, 12, 8});
    m_field_types.insert(fz26x14, {26, 14, 8});
    m_field_types.insert(fz30x16, {30, 16, 12});

#ifdef DEFINED_OPENGL
    setViewport(new QGLWidget(QGLFormat(QGL::DoubleBuffer)));
#endif
}

// ------------------------------------------------------------------------------------------------
void CBoard::createScene()
{
    m_scene = new CScene(m_field, this);
    m_scene->setBackground(bg_manager->currentFile());
    setScene(m_scene);

    connect(m_scene, &CScene::signalPaintPath, this, &CBoard::slotRepaintPath);
    connect(m_scene, &CScene::signalVariantStatus, this, &CBoard::slotVariantStatus);

    slotNewGame();
}

// ------------------------------------------------------------------------------------------------
QString CBoard::gameInfo()
{
    QTime t(0, 0, 0);
    t = t.addSecs(m_seconds);

    return tr("Ваше время: ") + t.toString("hh:m:ss") + tr("  Удалено: ")
            + QString::number(m_field->m_tiles_count - m_field->m_remaining) + "/"
            + QString::number(m_field->m_tiles_count);
}

// ------------------------------------------------------------------------------------------------
void CBoard::slotNewGame()
{
    m_field->newGame(m_field_types[settings->currentGameType()].x,
            m_field_types[settings->currentGameType()].y,
            m_field_types[settings->currentGameType()].count);

    doNewGame();
}

// ------------------------------------------------------------------------------------------------
void CBoard::slotRepeatGame()
{
    m_field->restoreField();
    doNewGame();
}

// ------------------------------------------------------------------------------------------------
// Новая игра
void CBoard::doNewGame()
{
    m_game_state = gsNormal;

    // Заполним сцену новыми значениями
    m_scene->newGame();

    recalcView();

    // Запускаем таймер игры
    m_seconds = 0;
    m_game_timer = startTimer(1000);

    emit signalUpdateInfo();

//    startDemonstration();
}

// ------------------------------------------------------------------------------------------------
void CBoard::slotHint()
{
    // Скажем сцене поместить в путь
    m_scene->showHint();
}

// ------------------------------------------------------------------------------------------------
void CBoard::slotPause()
{
    if (m_game_state != gsPause) {
        m_scene->showMessage(tr("Игра приостановлена") + "\n" + tr("Щелкните для продолжения"), true);
        m_prev_pause_state = m_game_state;
        m_game_state = gsPause;
    } else {
        m_scene->hideMessage(true);
        m_game_state = m_prev_pause_state;
    }
}

// ------------------------------------------------------------------------------------------------
// Изменился тип игры (из окна настроек)
void CBoard::slotNewTypeGame()
{
    slotNewGame();
}

// ------------------------------------------------------------------------------------------------
// Изменился тип костяшек
void CBoard::slotSetTileset()
{
    tiles_manager->initCurrentFile();
}

// ------------------------------------------------------------------------------------------------
// Установить новый фон
void CBoard::slotSetBackground()
{
    m_scene->setBackground(bg_manager->currentFile());
}

// ------------------------------------------------------------------------------------------------
void CBoard::resizeEvent(QResizeEvent *)
{
    recalcView();
}

// ------------------------------------------------------------------------------------------------
// Нажатие на кнопки мыши
void CBoard::mousePressEvent(QMouseEvent *event)
{
     if (event->buttons().testFlag(Qt::LeftButton))
         clickLeftButton(event);
     if (event->buttons().testFlag(Qt::RightButton) && settings->isTraining())
         clickRightButton(event);
}

// ------------------------------------------------------------------------------------------------
void CBoard::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_game_timer) {
        if (m_game_state == gsNormal) {
            ++m_seconds;
            emit signalUpdateInfo();
        }
    }
}

// ------------------------------------------------------------------------------------------------
// Пересчитать view (вызвается при инициализации и каждом изменении размера)
void CBoard::recalcView()
{
    fitInView(m_scene->m_field_rect, Qt::KeepAspectRatio);
}

// ------------------------------------------------------------------------------------------------
void CBoard::clickLeftButton(QMouseEvent *event)
{
    switch (m_game_state) {
    case gsNormal:
        m_scene->mouseLeft(mapToScene(event->pos()));
        break;
    case gsPause:
        slotPause();
        break;
    case gsDemostration:
        m_scene->closeDemonstration();
        break;
    case gsNotVariants:
        startDemonstration();
        break;
    case gsVictory:
        checkResult();
        break;
    case gsEmpty:
        break;
    }
}

// ------------------------------------------------------------------------------------------------
void CBoard::clickRightButton(QMouseEvent *event)
{
    // Отдадим нажатие сцене
    if (settings->isTraining()) m_scene->mouseRight(mapToScene(event->pos()));
}

// ------------------------------------------------------------------------------------------------
// Отобразить путь
void CBoard::slotRepaintPath()
{
    if (m_field->m_path.isEmpty()) return;

    QRegion region;

    auto prev_coord = mapFromScene(m_field->m_path[0]);
    for (int i = 1; i < m_field->m_path.size(); ++i) {
        auto coord = mapFromScene(m_field->m_path[i]);
        auto rect = QRectF(prev_coord, coord).normalized();
        rect.adjust(-1, -1, 1, 1);
        region += rect.toRect();
        prev_coord = coord;
    }
    viewport()->repaint(region);
}

// ------------------------------------------------------------------------------------------------
// Поле показало, какие у него варианты
// Получаются только vsVictory и vsNotVariant
void CBoard::slotVariantStatus(VariantStatus status)
{
    if (status == vsVictory)
        doVictory();
    if (status == vsNotVariant)
        doNotVariant();
}

// ------------------------------------------------------------------------------------------------
// Победа!
void CBoard::doVictory()
{
    m_game_state = gsVictory;
    stopGameTimer();
    m_scene->showMessage(tr("Вы выиграли!"));
}

// ------------------------------------------------------------------------------------------------
// Поражение :(
void CBoard::doNotVariant()
{
    m_game_state = gsNotVariants;
    stopGameTimer();

    QString s = tr("Игра зашла в тупик.") + "\n";
    if (settings->isDecision())
        s += tr("А вариант был - посмотрите демонстрацию.") + "\n";
    m_scene->showMessage(s + tr("Щелкните для продолжения..."));
}

// ------------------------------------------------------------------------------------------------
void CBoard::checkResult()
{
    m_game_state = gsEmpty;
    m_scene->hideMessage(false);

    auto result = records_manager->checkRecord(m_seconds);

    if (result != -1)
        emit signalShowResult(result);
}

// ------------------------------------------------------------------------------------------------
// Начать демонстрацию
void CBoard::startDemonstration()
{
    m_scene->hideMessage(false);

    // Вернуть на место поля и сцену
    m_field->restoreField();
    m_scene->fillScene();

    if (!settings->isDecision()) return;

    m_game_state = gsDemostration;

    m_scene->startDemonstration();
}

// ------------------------------------------------------------------------------------------------
void CBoard::stopGameTimer()
{
    Q_ASSERT(m_game_timer != -1);
    killTimer(m_game_timer);
    m_game_timer = -1;
}

