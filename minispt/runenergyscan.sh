#!/bin/bash

energyarray=( 60 140 500 1200 )
# 4000 10000 24000 )
#declare -i c=0

cd build
cp -v gps.mac temp.mac

for i in "${energyarray[@]}"
do
    sed -i -e "s/energy 511 keV/energy ${i} keV/g" gps.mac
    cat gps.mac | grep "gps/energy"
    ./cubedoz gun.mac
    hadd -f gamcam.root gamcam_t*.root
    cp -v gamcam.root ../CoGmethod/RootFiles/LYSO-H12mm-gamma${i}keV-8x6y-20k.root
    #cmd="hadd -f gamcam.root gamcam_t*.root ; cp -v gamcam.root ../CoGmethod/RootFiles/BC408-H10-gamma${i}keV-8x6y-20k.root"
    #$cmd
    cp -v temp.mac gps.mac
done

rm temp.mac
cd ..

