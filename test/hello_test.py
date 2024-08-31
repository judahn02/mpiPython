"""
File: hello_test.py
Modification Date: 8/13/24
Time Modified: 11:22pm CT
Created by: Judah Nava
Last Modified By: Judah Nava
Organization: Parallel Solvit LLC and MSUM CSIS Department
"""
from mpiPython import MPIpy
MPI = MPIpy()

rank = MPI.Rank()
size = MPI.Size()

print("Hello from process {} out of {}".format(rank, size))