"""
File: double_array_send.py
Modification Date: 8/14/24
Time Modified: 12:45am CT
Created by: Judah Nava
Last Modified By: Judah Nava
Organization: Parallel Solvit LLC and MSUM CSIS Department
"""
from mpiPython import MPIpy
MPI = MPIpy()

rank = MPI.Rank()
size = MPI.Size()

if (size != 2):
    print("This program needs two nodes")
    exit(-1)

if (rank == 0):
    array = [i+.1 for i in range(10)]
    print("rank 0 sending ", array)
    MPI.send_double_array(array, 1, 1)
    MPI.recv_double_array(array, 1, 1)
    print("rank 0 recieved ", array)
else:
    array = []
    MPI.recv_double_array(array,0, 1)
    print("rank 1 recieved ", array)
    array = [round(i+0.03,2) for i in array]
    print("rank 1 sending ", array)
    MPI.send_double_array(array, 0, 1)