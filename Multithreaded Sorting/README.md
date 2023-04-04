# Parallel Sorting
There are two files here: one in C++ and the other in C. You may choose either one to modify (or both, if you like). Each program expects a series of integers on the command line and builds a vector (or array) of the integers and then sorts the array.

However, the array is sorted in a single thread. Surely, we can do better than this. We all have multicore computers, so why not take advantage of those extra cores.

Your mission is to modify one of the programs here to implement a multithreaded sort. The sort must first be correct. A really fast sorting algorithm that doesn't reliably sort the data doesn't make any sense, right? After that, we care about performance.

You need to think about how you can divide up the work among threads. Which parts of the algorithm can be done in parallel? Which parts must be handled by a single thread? What happens if one thread gets done before another?

You're welcome to do this assignment on your own machine (the C++ code was initially developed on Windows and is portable to Linux with no modification... the C code isn't likely to be as portable).