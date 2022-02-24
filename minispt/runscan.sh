#!/bin/bash

#gunarray=( 0 3 6 9 12 -3 -6 -9 -12 )
#scanpos=( 0 3 6 9 12 n3 n6 n9 n12)
gunarray=( 0 2 4 -2 -4 )
scanpos=( 0 2 4 n2 n4 )
#declare -i c=0

cd build
cp gps.mac temp.mac

#for i in "${array[@]}"
for i in {0..4}
do
    for j in {0..4}
    do
	sed -i -e "s/0 30 0 mm/${gunarray[i]} 30 ${gunarray[j]} mm/g" gps.mac
	cat gps.mac | grep "gps/position"
	./cubedoz gun.mac &> logs/${scanpos[i]}x${scanpos[j]}y.txt
	cmd="cp -v gamcam.root ../CoGmethod/RootFiles/Lyso12mmCube/cube-lyso12mm-500keV-gamma-${scanpos[i]}x${scanpos[j]}y-10k.root"
	$cmd
	cp temp.mac gps.mac
    done
done

rm temp.mac
cd ..

