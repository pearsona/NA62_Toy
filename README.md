# NA62_Toy
This is a toy model of using shared memory (via the [boost interprocess library](http://www.boost.org/doc/libs/1_60_0/doc/html/interprocess.html)) to process "serialized data" (currently arbitrary strings) more efficiently than copying data everytime another process needs it. Specifically, [message_queue](http://www.boost.org/doc/libs/1_60_0/doc/html/interprocess/synchronization_mechanisms.html#interprocess.synchronization_mechanisms.message_queue) is being used to communicate which pieces of data are to be processed and [managed_shared_memory](http://www.boost.org/doc/libs/1_55_0/doc/html/interprocess/managed_memory_segments.html#interprocess.managed_memory_segments.managed_shared_memory) is being used to access the data across multiple processes (managed_shared_memory having been chosen over [shared_memory_object](http://www.boost.org/doc/libs/1_60_0/doc/html/interprocess/sharedmemorybetweenprocesses.html#interprocess.sharedmemorybetweenprocesses.sharedmemory.shared_memory_creating_shared_memory_segments) due to the easy construction/finding of data via char* labels, therefore not having to deal with the mess of pointers across multiple processes).

# Getting Started
Executing ./[run.sh](run.sh) and following the prompts should do everything necessary to use this model from compilation to running

# What's in Here?
In the root directory, there should be: <br/>
   (i) [run.sh](run.sh): Described above <br/>
   (ii) [A.cpp](A.cpp): Loads data into memory (-l) and enqueues data to be checked <br/>
   (iii) [B.cpp](B.cpp): Checks/"processes" data <br/>
   (iv) [viewData.cpp](viewData.cpp): Prints shared memory to screen <br/>
   (v) [Clean.cpp](Clean.cpp): Removes all instances of shared memory created by the above processes <br/>
   (vi) [shared.hpp](shared.hpp): Contains library includes/variable declarations/functions for use by above processes <br/>
   (vii) [Makefile](Makefile): make <br/>
   (viii) [int/](int/): Contains simpler/older versions of above that work with ints as "data" instead of strings <br/>
   (ix) [oldies/](oldies/): Contains miscellaneous old relevant files <br/>
