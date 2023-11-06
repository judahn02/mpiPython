#!/bin/bash
for i in $(seq 1 50)
do
{ time mpirun.mpich -np 8 python3 02_bw_plot_mock.py; } 2>> "time_data/mpiPython_8.txt"
echo " " >> "time_data/mpiPython_8.txt"
{ time python3 02_bw_plot_mockO.py; } 2>> "time_data/Python_seq.txt"
echo " " >> "time_data/Python_seq.txt"
{ time ./C_seq ; } 2>> "time_data/C_seq.txt"
echo " " >> "time_data/C_seq.txt"
{ time mpirun.mpich -np 8 ./mpich_mock ; } 2>> "time_data/C_mpich.txt"
echo " " >> "time_data/C_mpich.txt"
done
