#!/bin/bash

#Default installation directories:
 #   * /usr/local/bin - TORCS command (directory should be in your PATH)
 #   * /usr/local/lib/torcs - TORCS dynamic libs (directory MUST be in your LD_LIBRARY_PATH if you don't use the torcs shell)
  #  * /usr/local/share/games/torcs - TORCS data files

installdir=/usr/local/share/games/torcs/tcse
installdirbin=/usr/local/bin

mkdir $installdir

chmod 777 $installdir

cp ./torcs-car-setup-editor $installdir/tcse-bin

#isntall all data
cp -r ./data $installdir/

cp -r ./car-setups $installdir/

cp -r ./documentation $installdir/

cp -r ./ac3dLoader $installdir/

cp ./tcse $installdirbin/