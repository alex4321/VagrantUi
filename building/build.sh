#!/bin/sh
cd ..
git clone https://github.com/qterminal/qtermwidget
mkdir qtermwidget/build
cd qtermwidget/build
cmake .. -DUSE_QT5=true -DBUILD_DESIGNER_PLUGIN=0
make
make install
cd ../..
qmake
make
cd building

mkdir install/usr/lib
mkdir install/usr/bin
cp /usr/lib/libqtermwidget5.so install/usr/lib
cp /usr/lib/libqtermwidget5.so.0 install/usr/lib
cp /usr/lib/libqtermwidget5.so.0.6.0 install/usr/lib
cp ../vagrantui install/usr/bin
