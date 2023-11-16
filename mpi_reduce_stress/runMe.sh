#!/bin/bash

echo " " >> "mpich_base.txt"
echo " " >> "mpich_base2.txt"
echo " " >> "mpich_test.txt"
for i in $(seq 1 50)
do
{ time time mpirun.mpich -np 4 ./mrBase ; } 2>> "mpich_base.txt"
echo " " >> "mpich_base.txt"
{ time mpirun.mpich -np 4 ./mrBase2; } 2>> "mpich_base2.txt"
echo " " >> "mpich_base2.txt"
{ time mpirun.mpich -np 4 ./mrTest; } 2>> "mpich_test.txt"
echo " " >> "mpich_test.txt"
done
