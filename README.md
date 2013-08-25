Delta-Atomicity-Calculator
==========================

In this collaborative project, which began at [HP Labs](http://www.hpl.hp.com/), we seek to understand the behaviour of eventually-consistent key-value storage systems (e.g., Cassandra). Initial goal is to analyze the consistency of data operations in such systems for various workloads, and quantify to what extent these systems deviate from the "gold standard" of linearizability. To that end, we leverage a methodology inspired by known algorithmic techniques for verifying shared memories.  

The algorithm described in Section 4.1 of the [paper](http://www.hpl.hp.com/techreports/2011/HPL-2011-6.pdf). This algorithm takes as input a trace comprising a sequence of records representing operations applied to a key-value storage system.  The output is the minimum Delta such that the trace is Delta-atomic.

Install [Boost](http://www.boost.org/) by using command

`sudo apt-get install libboost-all-dev`

Install [Intel Threading Building Blocks](http://threadingbuildingblocks.org/) by using command

`sudo apt-get install libtbb-dev`

Install [Google Log](https://code.google.com/p/google-glog/) library by using command

`sudo apt-get install libgoogle-glog-dev`

There are two implementations of this algorithm. Both of them works fine. One is
built using [Intel Threading Building Blocks](http://threadingbuildingblocks.org/) and another using [OpenMP](http://www.openmp.org/). Inspiration behind implementing this algorithm is to take advantage of modern multi-core processors and cache-aligned data structures. 

Build `make makefile -b`

Run `./DeltaComputation tracefile1.log ... tracefilen.log`