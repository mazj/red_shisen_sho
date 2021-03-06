kdsli version for shishen_sho

This project was created to teach my son how to program and maintain my professional skills, and also because of the small amount of free time.

It is written without viewing the source code of the KShisen program from KDE and uses only pure QT and graphic images of this project, including tiles and background image.
The current version uses Ben Gillbanks designer icons (https://iconbird.com/designer/BenGillbanks)

The goal of the project was to find the optimal tiles removal algorithms.

In version 0.3, the program is rewritten using QGraphicsView and QGraphicsScene.

In version 0.4, the game control logic is transferred to the QStateMachine state machine.

In addition, the program has additional features in relation to the original game: 

- when a program reaches a deadlock, it demonstrates that the correct way to remove the tiles is possible.
- you can set your own background images by placing them in the UserBackhround directory.
- the program has a training mode, in which the hint, undo and redo functions are available, but the results do not fit 
  into the high score table. When the training mode is off, these functions are not available, but the results are recorded 
  in the table of records.

The source code of the program is successfully tested:

cppcheck -q --enable=all --inconclusive --std=c++14

The main language of the program and comments is Russian, but there is an interface translation file into English.

Build under Windows posted here. But no work was done on installing the program on Linux, since the program was 
not planned as public. Nevertheless, it is quite workable.

The program can be assembled according to the following algorithm:

- git clone https://github.com/kdsli/red_shisen_sho
- mkdir build
- cd build
- cmake ..
- make
- make install

The copy obtained in the output_Release directory is fully functional.

From the unfinished I can specify:

- no sounds are output

In the near future I plan to fix it.

Respectfully, Dmitry kdsli@kdsli.ru

![screenshot1](screenshots/screenshot1_small.png)
![screenshot2](screenshots/screenshot2_small.png)
![screenshot3](screenshots/screenshot3_small.png)
![screenshot4](screenshots/screenshot4_small.png)
![screenshot5](screenshots/screenshot5.png)
![screenshot6](screenshots/screenshot6.png)
![screenshot7](screenshots/screenshot7.png)
