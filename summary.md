Max Isaacson
CS 452
Project 3 Threads
11/12/24

# Summary of Multithreaded Mergesort
This project is to put our skills that we have bee nlearning to the test. We must implement a thread safe solution when merging all of the threads
## 1. Project Management
I managed this project poorly compared to the last ones. I had a very strange apporach for this project, I worked backwards from the unit test. I first fully understood what the test was actually doing before I went to coding. This means that I had the single thread test done first, then second, and finally third. 90% of the time spent on this project was trying to get the 3rd test to pass.

## 2. Deliverables
``````````````````````````````````````````
usage : ./myprogram [size] [thread_count]
```````````````````````````````````````
Refer to README.md for make commands
```````````````````````````````````````
usage : ./createplot.sh -s [size] -f [filename]
`````````````````````````````````````````````
There was no self-modification done to the program - it is very bare bones just so I could get it to pass the tests with the short time I had.

Please note that the bash files, analysis, and driver app image are all located in this root file

## 3. Explanations
### Input and Configuration:

The function mergesort_mt takes an integer array A, the array's length n, and the desired number of threads num_threads.
num_threads is adjusted if it exceeds either the array size or a predefined MAX_THREADS limit.

### Chunk Division and Thread Assignment:

The array is divided into num_threads chunks. Each chunk is roughly n / num_threads elements, with adjustments for any remainder to ensure all elements are covered.
Each thread is assigned a specific chunk of the array to sort, and these threads are created using pthread_create.
Each thread sorts its assigned chunk using a single-threaded mergesort algorithm.

### Concurrent Sorting of Chunks:

Threads operate independently to sort their respective chunks concurrently, reducing the time taken for the initial sorting phase.
The pthread_join function waits for each thread to finish sorting its chunk before proceeding to the merging phase.

### Iterative Merging of Sorted Chunks:

Once all threads complete sorting, the algorithm performs an iterative, two-way merging process:
Starting with each sorted chunk, pairs of adjacent chunks are merged into larger sorted segments.
This doubling process repeats until the entire array is merged into one fully sorted segment.
Merging is handled by the helper function merge_s, which merges two sorted subarrays into one in-place.

### Final Sorted Array:

After all merging iterations are complete, the original array A contains the sorted elements in ascending order.


## Results & Reflection
The project was much harder than the last one so I did not manage my time well enough. I believe I have a memory leak but I have never been taught how to troubleshoot that. My code does work and it accurately sorts, I just ran into the problem of getting AddressSanitizer:DEADLYSIGNAL and had to figure it out on my own. I am glad that the entire implementation of the mergesort wasnt up to us, as half of it was given in the handout. However I am proud that I got all of the unit tests passing, for the longest time I was one number away from the array being fully sorted. If I had extra time I would find the memory bug hiding in my project, however I've been looking for over a day now and my time has run out. Also I didn't use any AI tools to help debug. Well I tried but they couldn't find it either!
