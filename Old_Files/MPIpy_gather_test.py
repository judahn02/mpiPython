from random import randint as rand
from time import perf_counter as clock
from sys import argv
import MPIpy_v3
MPI = MPIpy_v3.MPIpy()


test = [MPI.rank, MPI.rank*10] # [0,10],[1,11]
print(test)
MPI.gather(test, 0, )
if MPI.rank == 0:
    print("Master: ",test)