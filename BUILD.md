# Following packages must be installed

## Linux
at least 
* qt5.2 development packages
* libasound2-dev
* libclalsadrv-dev
* libusb-1.0-0-dev

Surely some more...

## preconditions
you need to checkout separate controls/widgets from https://codeberg.org/dehnhardt/PKControls.git
They must be found in the same root directory as iconnconfig
|
|- iconnconfig
|- PKControls
This way, the additional sources should be included without any changes

## compile
* change to this directory
* qmake (be shure that QMake uses at least QT 5.2. e.g. with the switch: qmake -qt=5)
* make

There might be errors because of missing additional development packages. As I do not have a 'clean' system around, I can not test it. Please create an issue for such errors, so I can add dependencies.
