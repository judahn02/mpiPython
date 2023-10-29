# mpiPython
The files above are what were used for the first paper. I will work on developing a test script for the code.
- Make sure you are working in a Linux environment
	- have Python 3.11 installed
	- have MPICH installed
	- can compile C code and libraries

## What to do
- Implement MPI_Get_processor_name() function. 
- Implement MPI_Gather() function.
- Remove the need to separate Python Integer-based commands and Python Double-based commands
- Implement all features of MPI_Reduce()
I wrote the order of objectives from top to bottom implying the difficulty of easiest at top and most difficult at bottom.

We need to also look at how much of the function features to be implemented in C rather than in Python as that will help efficiency.

### Jinu,
Once I get the test script running, then I would like you to make the MPI_Get_processor_name() function. Look at how C first does it in the mpi_example code, then look at how Python needs to be set up in order to run the function by looking at the source code. I will try to edit the code to be more clear to follow. 
