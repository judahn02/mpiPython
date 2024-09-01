import MPIpy_v3
MPI = MPIpy_v3.MPIpy()

FILE0 = "numbers0.txt"
FILE1 = "numbers1.txt"
FILE2 = "numbers2.txt"
FILE3 = "numbers3.txt"

METASIZE = 300000000

if (MPI.size != 4):
    print("needs to have nodes exact")

if (MPI.rank == 0):
    f = open(FILE0, "r")
elif (MPI.rank == 1):
    f = open(FILE1, "r")
elif (MPI.rank == 2):
    f = open(FILE2, "r")
elif (MPI.rank == 3):
    f = open(FILE3, "r")
else:
    print("How did it get here?")

tmp = f.readline()
tmp = tmp.split(",")
print(MPI.rank, ":",len(tmp))
print(MPI.rank, ": ", tmp[-20:-1:1])
f.close()