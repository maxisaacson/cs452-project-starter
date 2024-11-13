#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <pthread.h>
#include "lab.h"

/**
 * @brief Standard insertion sort that is faster than merge sort for small arrays
 *
 * @param A The array to sort
 * @param p The starting index
 * @param r The ending index
 */
static void insertion_sort(int A[], int p, int r)
{
  int j;

  for (j = p + 1; j <= r; j++)
  {
    int key = A[j];
    int i = j - 1;
    while ((i > p - 1) && (A[i] > key))
    {
      A[i + 1] = A[i];
      i--;
    }
    A[i + 1] = key;
  }
}

/**
 * @brief Single-threaded merge sort implementation
 *
 * @param A The array to sort
 * @param p The starting index
 * @param r The ending index
 */
void mergesort_s(int A[], int p, int r)
{
  if (r - p + 1 <= INSERTION_SORT_THRESHOLD)
  {
    insertion_sort(A, p, r);
  }
  else
  {
    int q = (p + r) / 2;
    mergesort_s(A, p, q);
    mergesort_s(A, q + 1, r);
    merge_s(A, p, q, r);
  }
}

/**
 * @brief Merges two sorted subarrays A[p..q] and A[q+1..r]
 *
 * @param A The array containing the subarrays to merge
 * @param p The starting index of the first subarray
 * @param q The ending index of the first subarray
 * @param r The ending index of the second subarray
 */
void merge_s(int A[], int p, int q, int r)
{
  int *B = (int *)malloc(sizeof(int) * (r - p + 1));

  int i = p;
  int j = q + 1;
  int k = 0;
  int l;

  while ((i <= q) && (j <= r))
  {
    if (A[i] < A[j])
    {
      B[k] = A[i];
      i++;
    }
    else
    {
      B[k] = A[j];
      j++;
    }
    k++;
  }

  if (i <= q)
  {
    for (l = i; l <= q; l++)
    {
      B[k] = A[l];
      k++;
    }
  }
  else
  {
    for (l = j; l <= r; l++)
    {
      B[k] = A[l];
      k++;
    }
  }

  k = 0;
  for (l = p; l <= r; l++)
  {
    A[l] = B[k];
    k++;
  }
  // After merging is done, verify the array
//  for (int i = p; i <= r; i++)
//   {
//     printf("%d ", A[i]);
//   }
//   printf("\n");
  free(B);
}

/**
 * @brief Function executed by each thread to perform a single-threaded merge sort on its chunk
 *
 * @param args Pointer to a struct containing the array and chunk indices
 * @return void* Always returns NULL
 */
void *parallel_mergesort(void *args)
{
  struct parallel_args *data = (struct parallel_args *)args;
  mergesort_s(data->A, data->start, data->end);
  return NULL;
}

int min(int a, int b)
{
  return a < b ? a : b;
}

/**
 * @brief Multi-threaded merge sort implementation
 *
 * @param A The array to sort
 * @param n The size of the array
 * @param num_threads The number of threads to use
 */
void mergesort_mt(int *A, int n, int num_threads) {
    if (n <= 0) {
        return;
    }

    // Limit threads to array size and MAX_THREADS
    num_threads = min(n, min(num_threads, MAX_THREADS));

    // Allocate memory for thread arguments
    struct parallel_args *args = malloc(num_threads * sizeof(struct parallel_args));
    if (!args) {
        fprintf(stderr, "Memory allocation failed for parallel_args\n");
        exit(EXIT_FAILURE);
    }

    // Assign each thread a chunk of the array
    int chunk_size = n / num_threads;
    int remainder = n % num_threads;

    for (int i = 0; i < num_threads; i++) {
        args[i].A = A;
        args[i].start = i * chunk_size + (i < remainder ? i : remainder);
        args[i].end = args[i].start + chunk_size - 1 + (i < remainder ? 1 : 0);

        // Initialize tid and check thread creation
        if (pthread_create(&args[i].tid, NULL, parallel_mergesort, &args[i]) != 0) {
            fprintf(stderr, "Error creating thread %d\n", i);
            free(args);
            exit(EXIT_FAILURE);
        }
        }

    // Wait for all threads to complete sorting their chunks
    for (int i = 0; i < num_threads; i++) {
        if (pthread_join(args[i].tid, NULL) != 0) {
            perror("Thread join failed");
            exit(1);
        }
    }

    // Iteratively merge sorted chunks across the entire array
    int current_size = 1; //changed to 1 temporarily
    while (current_size < n) {
        for (int left = 0; left < n; left += 2 * current_size) {
            int mid = min(left + current_size - 1, n - 1); //adding min() on here fixed test 3
            int right = min(left + 2 * current_size - 1, n - 1);
            if (mid < right) {
                merge_s(A, left, mid, right);
            }
        }
        current_size *= 2;
    }
    free(args);
}

/**
 * @brief Returns the current time in milliseconds
 *
 * @return Current time in milliseconds
 */
double getMilliSeconds()
{
  struct timeval now;
  gettimeofday(&now, NULL);
  return now.tv_sec * 1000.0 + now.tv_usec / 1000.0;
}
