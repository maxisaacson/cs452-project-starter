# CSC 452/552 Operations Systems  
## Project 2: The Classic Bounded Buffer Problem  

**Name:** Max Isaacson  
**Bronco ID:** 114162063  
**Date:** 10/22/2024  

---

### 1. Project Overview  
The project involved implementing a solution to the classic Bounded Buffer (Producer-Consumer) problem in C using multi-threading. The goal was to create a FIFO queue with a fixed capacity and proper synchronization mechanisms that would allow multiple producer and consumer threads to interact concurrently. The queue should block threads attempting to enqueue when full and dequeue when empty, while also handling shutdown conditions.

### 2. Project Management Plan  
- **Task 1:** Initialize the code starter and set up the project environment.  
- **Task 2:** Define the `struct queue` with capacity, size, shutdown flag, buffer array, and synchronization variables (mutex and condition variables).  
- **Task 3:** Implement the `queue_init()` function to initialize the queue structure, including setting up locks and condition variables.  
- **Task 4:** Implement `enqueue()` function to add items to the queue with synchronization to handle full queue conditions.  
- **Task 5:** Implement `dequeue()` to remove items from the queue, considering empty queue conditions.  
- **Task 6:** Implement `queue_shutdown()` function to set the shutdown flag and signal all waiting threads.  
- **Task 7:** Implement helper functions `is_empty()` and `is_shutdown()` with necessary synchronization.  
- **Task 8:** Debug and test the solution with unit tests to ensure all functionality is correct.  
- **Task 9:** Optimize the code for efficiency.

### 3. Project Deliveries  
- **a) How to compile and use my code?**  
   To compile the code, use the following command in the terminal:  
   ```bash
   make
   ```
    *Also refer to the provided README.md from starter code*  
      
   Run the code using this format in the terminal once it has been built:
   ```bash
   ./myprogram -c [num_consumer] -p [num_producer] -i [num_items] -s [queue_size] [-d]
   ```

- **b) Any self-modification?**  
   - The queue's internal data structure was chosen as an array for simplicity.  
   - Added condition variables to signal when the queue is full or empty. 
   - Added error handling and shutdown functionality.

- **c) Summary of Results**  
   The implementation successfully met the requirements of handling producer and consumer threads with proper synchronization. The only area of improvement would be the running time (the examples time was faster than mine).

## 4. Self-Reflection of Project 2  
This project helped me understand the complexities of multi-threaded programming and synchronization. I learned the most from hands on experience with mutex locks, now that I've actually used them I have a much better understanding.