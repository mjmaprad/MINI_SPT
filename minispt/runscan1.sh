# ! /bin/bash
cd build
cp gps.mac temp.mac

./cubedoz gun.mac
cp lysocube.root ../CoGmethod/RootFiles/lyso-500keV-gamma-0x0y.root

sed -i -e 's/0 30 0 mm/3 30 0 mm/g' temp.mac
./cubedoz gun.mac
cp lysocube.root ../CoGmethod/RootFiles/lyso-500keV-gamma-3x0y.root

cp gun.mac temp.mac
sed -i -e 's/0 30 0 mm/6 30 0 mm/g' temp.mac
./cubedoz gun.mac
cp lysocube.root ../CoGmethod/RootFiles/lyso-500keV-gamma-6x0y.root

cp gun.mac temp.mac
sed -i -e 's/0 30 0 mm/9 30 0 mm/g' temp.mac
./cubedoz gun.mac
cp lysocube.root ../CoGmethod/RootFiles/lyso-500keV-gamma-9x0y.root

cp gun.mac temp.mac
sed -i -e 's/0 30 0 mm/12 30 0 mm/g' temp.mac
./cubedoz gun.mac
cp lysocube.root ../CoGmethod/RootFiles/lyso-500keV-gamma-12x0y.root

cp gun.mac temp.mac
sed -i -e 's/0 30 0 mm/0 30 3 mm/g' temp.mac
./cubedoz gun.mac
cp lysocube.root ../CoGmethod/RootFiles/lyso-500keV-gamma-0x3y.root

cp gun.mac temp.mac
sed -i -e 's/0 30 0 mm/0 30 6 mm/g' temp.mac
./cubedoz gun.mac
cp lysocube.root ../CoGmethod/RootFiles/lyso-500keV-gamma-0x6y.root

cp gun.mac temp.mac
sed -i -e 's/0 30 0 mm/0 30 9 mm/g' temp.mac
./cubedoz gun.mac
cp lysocube.root ../CoGmethod/RootFiles/lyso-500keV-gamma-0x9y.root

cp gun.mac temp.mac
sed -i -e 's/0 30 0 mm/0 30 12 mm/g' temp.mac
./cubedoz gun.mac
cp lysocube.root ../CoGmethod/RootFiles/lyso-500keV-gamma-0x12y.root

cp gun.mac temp.mac
sed -i -e 's/0 30 0 mm/-3 30 -3 mm/g' temp.mac
./cubedoz gun.mac
cp lysocube.root ../CoGmethod/RootFiles/lyso-500keV-gamma-n3x3y.root

cp gun.mac temp.mac
sed -i -e 's/0 30 0 mm/-6 30 -6 mm/g' temp.mac
./cubedoz gun.mac
cp lysocube.root ../CoGmethod/RootFiles/lyso-500keV-gamma-n6x6y.root

cp gun.mac temp.mac
sed -i -e 's/0 30 0 mm/-9 30 -9 mm/g' temp.mac
./cubedoz gun.mac
cp lysocube.root ../CoGmethod/RootFiles/lyso-500keV-gamma-n9x9y.root

cp gun.mac temp.mac
sed -i -e 's/0 30 0 mm/-12 30 -12 mm/g' temp.mac
./cubedoz gun.mac
cp lysocube.root ../CoGmethod/RootFiles/lyso-500keV-gamma-n12x12y.root

rm temp.mac
cd ..

