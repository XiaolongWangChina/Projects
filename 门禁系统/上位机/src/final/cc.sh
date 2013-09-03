#!/bin/sh
export QTDIR=/home/qt3_arm/qt-embedded-free-3.3.8
export QMAKESPEC=$QTDIR/mkspecs/qws/linux-arm-g++
make clean
make
