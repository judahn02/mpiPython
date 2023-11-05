#!/bin/bash
for i in $(seq 1 50)
do
{ time mpirun.mpich -np 8 python3 02_bw_plot_mock.py; } 2>> "mpiPython_8.txt"
echo " " >> "mpiPython_8.txt"
{ time python3 02_bw_plot_mockO.py; } 2>> "Python_seq.txt"
echo " " >> "Python_seq.txt"
{ time ./C_seq ; } 2>> "C_seq.txt"
echo " " >> "C_seq.txt"
done

# for i in $(seg 1 50)
# do
# time python3 02_bw_plot_mockO.py >> "Python_seq.txt"
# echo " " >> "Python_seq.txt"
# done

# for i in $(seq 1 50)
# do
# time ./a.out >> "C_seq.txt"
# echo " " >> "C_seq.txt"