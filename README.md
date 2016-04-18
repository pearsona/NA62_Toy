# NA62_Toy
Running ./run.sh and following the prompts should do everything necessary to use this model from compilation to running

# What's in Here
In the top level directory, there should be:
   (i) *run.sh*: Described above
   (ii) *A3.cpp*: Loads data into memory (-l) and enqueues data to be checked (-c)
   (iii) *B3.cpp*: Checks/"processes" data
   (iv) *viewData.cpp*: Prints shared memory to screen
   (v) *ab3Clean.cpp*: Removes all instances of shared memory created by the above processes
   (vi) *shared.hpp*: Contains library includes/variable declarations/functions for use by above processes
   (vii) *Makefile*: make
   (viii) *int/*: Contains simpler/older versions of above that work with ints as "data" instead of strings
   (ix) *oldies/*: Contains miscellaneous old relevant files

