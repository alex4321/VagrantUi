#!/bin/sh
dpkg -b install
mv install.deb vagrantui.deb
dpkg -i vagrantui.deb
