# red_shisen_sho
kdsli's version for shishen_sho

Этот проект был создан для обучения моего сына программированию и поддержки моих профессиональных навыков, 
а также из-за наличия определенного количества свободного времени.

Проект написан без просмотра исходного кода программы KShisen из KDE. В нем используются только графические 
изображения этого проекта: плитки и фоновое изображение.
Текущая версия использует иконки дизайнера Ben Gillbanks (https://iconbird.com/designer/BenGillbanks)

Целью проекта было найти оптимальные алгоритмы удаления костяшек.

Кроме того, программа имеет дополнительные функции по отношению к оригинальной игре:

- когда программа заходит в тупик, она демонстрирует возможный способ удаления костяшек.
- вы можете установить свои собственные фоновые изображения, поместив их в каталог UserBackhround.
- в программе есть режим тренировки, в котором доступны функции подсказки, отмены и повтора, но результаты 
  не записываются в таблицу рекордов. Когда режим обучения выключен, эти функции недоступны, но результаты 
  в таблицу рекордов заносятся.

Исходный текст программы успешно проходит проверку:

cppcheck -q --enable=all --inconclusive --std=c++14

Основной язык программы и комментариев - русский, но есть файл перевода интерфейса на английский.

Сборки под Windows выложены тут. Но не было сделано никаких работ по установке программы в среду Linux, так как
программа не планировалась как публичная, Тем не менее, она вполне работоспособная. 

Программа может быть собрана по следующему алгоритму:

  git clone https://github.com/kdsli/red_shisen_sho
  mkdir build
  cd build
  cmake ..
  make
  make install

Копия, полученная в каталоге output_Release, является полностью функциональной.

Из несделанного могу указать:

- таблица рекордеров отключена
- звуки не выводятся

В ближайшее время планирую это исправить.

С уважением, Дмитрий, kdsli@kdsli.ru
