import MPIpy_v3
MPI = MPIpy_v3.MPIpy()


test = [1,2,3,4]
test = MPI.reduceChoiceInt(test, 0, "PROD")
if MPI.rank == 0:
    print(test)
