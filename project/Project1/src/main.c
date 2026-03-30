/*
 *  main.c
 *  version 1.0
 *  Created on: 2025/3/27
 *  Author: Jimmy 
*/
 
#include <time.h>
#include "utils.h"
#include <pthread.h>

# define NUM_THREADS 4


int main () {
    printf("Default\n");
    // size_t : unsigned integer type, auto-adjusted to the size of the platform (32-bit or 64-bit)
    size_t total_element = (size_t)rows * cols;
    // number of elements in the matrix = rows * cols
    printf("Total elements: %zu\n", total_element);

    // single precision float: 4 bytes
    // malloc return void* , float* can trasform void* to float*
    float *matrix = (float *)malloc(total_element * sizeof(float));
    // float **ptr = malloc(rows * sizeof(float *));
    // float *data = malloc(rows * cols * sizeof(float)); // 分配一整塊連續空間

    // for (int i = 0; i < rows; i++) {
    //     ptr[i] = &data[i * cols]; // 將指標指向連續空間的對應位置
    // }
    if (matrix == NULL) {
        // log error message to a file name "stderr"
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }
    // time_t time(time_t *tloc);
    // time return the current calendar time as 
    // a value of type time_t. 
    // If tloc is non-NULL, 
    // the return value is also stored 
    // in the memory pointed to by tloc.
    srand (time(NULL)); // seed the random number generator with current time
    for (size_t i = 0; i < total_element; i++) {
        // RAND_MAX is a constant defined in stdlib.h,
        // which represents the maximum value returned by the rand function
        matrix[i] = (float)rand() / RAND_MAX; // Initialize with some values, e.g., 0.0, 1.0, ..., 99.0, then repeat
    }

    printf ("Matrix initialization completed.\n");
    pthread_t threads[NUM_THREADS];
    thread_args_t thread_args[NUM_THREADS];
    thread_data_t thread_results[NUM_THREADS]; 
    
    size_t rows_per_thread = MATRIX_SIZE / NUM_THREADS;
    printf("\n %d 個thread開始作業...\n", NUM_THREADS);
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_args[i].thread_id = i;
        thread_args[i].start_row = i * rows_per_thread;
        
        // 如果是最後一個執行緒，就包辦剩下的所有行數 (避免無法整除的問題)
        thread_args[i].end_row = (i == NUM_THREADS - 1) ? MATRIX_SIZE : (i + 1) * rows_per_thread;
        thread_args[i].matrix = matrix;
        thread_args[i].thread_results = thread_results;

        // 建立執行緒並執行 worker_task
        pthread_create(&threads[i], NULL, worker_task, &thread_args[i]);
    }

    // 階段 2：等待所有執行緒完工 (Join/Barrier)
 
    pthread_join(threads[0], NULL); 
    pthread_join(threads[1], NULL);
    pthread_join(threads[2], NULL);
    pthread_join(threads[3], NULL);

    double global_sum = 0.0;
    for (int i = 0; i < NUM_THREADS; i++) {
        global_sum += thread_results[i].local_sum;
    }
    printf("\n所有執行緒完成計算，矩陣總和: %f\n", global_sum);
    free (matrix);
    printf("Matrix memory freed.\n");
    return 0;
}


