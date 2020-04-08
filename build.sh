#!/bin/bash

MYDIR=$(pwd)
RELEASE=$(grep "QCoreApplication::setApplicationVersion(\"" src/main.cpp |grep -o "\([0-9.]\)\+")

echo "Building version $RELEASE"

if [ -d ../PKControls ]
then
	cd ../PKControls
	git pull 
	cd $MYDIR
else
	git clone https://codeberg.org/dehnhardt/PKControls.git ../PKControls
fi

if [ "$1" == "build_deb" ]
then
	cd .. 
	qmake -o Makefile PREFIX=debian/iconnconfig/usr CONFIG+=build_deb iconnconfig/iConnConfig.pro
	cd $MYDIR
else
	qmake PREFIX=/usr
	make
fi

