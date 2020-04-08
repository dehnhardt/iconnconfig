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
They must be found in the same root as iconnconfig  
```
- root 
 |- iconnconfig  
 |- PKControls  
```
 
This way, the additional sources should be included without any changes

There is a bash script in the root directory which will fetch the additional project.
* If called without any parameter `./build.sh` this script will
 * fetch the additional sources in a new directory (or update the sources) in the same root folder like iconnconfig (see above)
 * create a makefile
 * build the application
* if called with the parameter build_deb `./build.sh build_deb` this script will
 * fetch the additional sources in a new directory (or update the sources) in the same root folder like iconnconfig (see above)
 * create a makefile in the root directory of iconnconfig which specifies the install dir `debian/iconnconfig/usr/bin`

## compile
* change to this directory
* qmake (be shure that QMake uses at least QT 5.2. e.g. with the switch: qmake -qt=5)
* make

There might be errors because of missing additional development packages. As I do not have a 'clean' system around, I can not test it. Please create an issue for such errors, so I can add dependencies.
