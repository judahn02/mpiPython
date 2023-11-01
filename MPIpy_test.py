from random import randint as rand
from time import perf_counter as clock
from sys import argv
import copy_MPIpy_v3
MPI = copy_MPIpy_v3.MPIpy()
# !! side length has to be equal or bigger than node count minus one.
# to run: $mpirun.mpich -np 3 python3 ./multi_node.py 4

def myWork(side: int, size: int, rank:int):
    temp = [0 for a in range(size)]
    ind = size - 1
    for i in range(side):
        temp[ind] += 1
        ind -= 1
        if ind == -1:
            ind = size - 1
    # print(temp)
    temp2 = [0]
    for i in range(size):
        tmp = temp2[-1] + temp[i]
        temp2.append(tmp)
    # temp2.append(side)
    # print(temp2)
    if rank == 0:
        return temp2[0:side+1]
    else:
        return temp[rank-1:rank]
    
side = int(argv[-1])
work = myWork(side, MPI.sizeS, MPI.rank)

if MPI.rank == 0:
    list_one = [float(a) for a in range(side * side)]
    list_two = [float(a) for a in range(side * side)]
    list_the = []

    tik = clock()  # Define tik here

    # Broadcast data to all worker nodes
    MPI.Bcast_double(list_two, 0)

    for i in range(1, MPI.size):
        MPI.send_double_array(list_one[work[i-1] * side: work[i] * side], i, i)

    for i in range(1, MPI.size):
        temp = []
        MPI.recv_double_array(temp, i, i)
        list_the = list_the + temp

    tok = clock()

    # Gather results from worker nodes
    MPI.Gather(list_two, 0)

    print(" side= {:d}, time= {:f}, MFLOPS= {:f}".format(
        side, tok - tik, (float(side * side * side) / (1000000 * (tok - tik)))
    ))
else:
    # Worker nodes do work
    list_one_part = []
    list_two = []
    list_the = [0.0 for a in range(side * work[0])]

    # Broadcast data from processor 0
    MPI.Bcast_double(list_two, 0)
    MPI.recv_double_array(list_one_part, 0, MPI.rank)

    # Perform work here
    for a in range(work[0]):
        for b in range(side):
            for c in range(side):
                list_the[a * side + b] += list_one_part[a * side + c] * list_two[c * side + b]

    # Send results back to processor 0
    MPI.send_double_array(list_the, 0, MPI.rank)

# Gather results from worker nodes on processor 0
MPI.Gather(list_two, 0)

print(MPI.Get_processor_name())
