import mpiPython
MPI = mpiPython.MPIpy()



total = MPI.reduceSumDouble(4, 0)
if MPI.Rank() == 0:
    print(total)
    print(4 * MPI.Size())