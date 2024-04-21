# Dijkstra-parallel
This is a study project of Dijkstra's algorithm for university, on the subject of parallel and distributed algorithms.
The project contains 4 implementations:

-C++ Threads

-C++ STL Parallel

-C++ OpenMP

-C++ MPI

The input for each algorithm will be extracted from an "input...txt" file inside project folder.
The input file has the following format:
no_of_nodes source_node
c00 c01 ... c0(n-1)
c10 c11 ... c1(n-1)
...................
c(n-1)0 c(n-1)1 ... c(n-1)(n-1) Where c is the adjacency matrix for graph.
The output will be given in a "output...txt" file inside the project folder.
To run project I used:

Visual Studio IDE
Eclipse IDE
To run MPI implementation, you must install the MPI support on Visual Studio IDE, see:
https://docs.microsoft.com/en-us/message-passing-interface/microsoft-mpi
https://docs.microsoft.com/en-us/archive/blogs/windowshpc/how-to-compile-and-run-a-simple-ms-mpi-program
