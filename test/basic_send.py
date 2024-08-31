"""
File: basic_send.py
Modification Date: 8/13/24
Time Modified: 11:21pm CT
Created by: Judah Nava
Last Modified By: Judah Nava
Organization: Parallel Solvit LLC and MSUM CSIS Department
"""
from mpiPython import MPIpy
from random import randint

MPI = MPIpy()

rank = MPI.Rank()
size = MPI.Size()

if (size != 2):
    print("This program needs two nodes")
    exit(-1)

if (rank == 0):
    value = randint(0,10)
    print("rank 0 sending ", value)
    MPI.send_int(value, 1, 1)
    value = MPI.recv_int(1, 1)
    print("rank 0 recieved ", value)

else:
    value = MPI.recv_int(0, 1)
    print("rank 1 recieved ", value)
    value = randint(0,10)
    print("rank 0 sending ", value)
    MPI.send_int(value, 0, 1)