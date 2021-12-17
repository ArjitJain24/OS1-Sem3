#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <math.h>
#include <string.h>
#define MAX_ARR_SIZE 50000

int array_num[MAX_ARR_SIZE];
int size_arr;

//Struct for giving parameters to merge_sort function
struct arguements_multithread
{
    int first_index;
    int last_index;
};

void merge_array_func(int start_index, int mid_index, int end_index)
{
    int left_index = mid_index - start_index + 1;
    int right_index = end_index - mid_index;

    int left_arr[left_index], right_arr[right_index];
    for (int i = 0; i < left_index; i++)
    {
        left_arr[i] = array_num[start_index + i];
    }
    for (int i = 0; i < right_index; i++)
    {
        right_arr[i] = array_num[mid_index + 1 + i];
    }
    int left_arr_loop = 0, right_arr_loop = 0;
    int k;
    for (k = start_index; left_arr_loop < left_index && right_arr_loop < right_index; k++)
    {
        if (left_arr[left_arr_loop] <= right_arr[right_arr_loop])
        {
            array_num[k] = left_arr[left_arr_loop];
            left_arr_loop++;
        }
        else
        {
            array_num[k] = right_arr[right_arr_loop];
            right_arr_loop++;
        }
    }
    while (left_arr_loop < left_index)
    {
        array_num[k] = left_arr[left_arr_loop];
        left_arr_loop++;
        k++;
    }
    while (right_arr_loop < right_index)
    {
        array_num[k] = right_arr[right_arr_loop];
        right_arr_loop++;
        k++;
    }
}

// void merge_sort(int p, int r)
// {
//     if(p < r)
//     {
//         int q = floor((p+r)/2);
//         //spawn merge_sort(arr, p, q);
//         merge_sort(p, q);
//         merge_sort(q+1, r);
//         merge_array_func(p, q, r);
//     }
// }

void *merge_sort(void *arg)
{
    int p = ((struct arguements_multithread *)arg)->first_index;
    int r = ((struct arguements_multithread *)arg)->last_index;

    if (p < r)
    {
        int q = (p + r) / 2;

        struct arguements_multithread *left_sub_arg = malloc(sizeof(struct arguements_multithread));
        left_sub_arg->first_index = p;
        left_sub_arg->last_index = q;
        merge_sort(left_sub_arg);

        struct arguements_multithread *right_sub_arg = malloc(sizeof(struct arguements_multithread));
        right_sub_arg->first_index = q + 1;
        right_sub_arg->last_index = r;
        merge_sort(right_sub_arg);

        merge_array_func(p, q, r);
    }

    return NULL;
}

void random_array_generator(int array_size, int num_of_threads)
{
    for (int i = 0; i < array_size; i++)
    {
        array_num[i] = rand() % array_size;
    }
}

//Input Output Part of the Asignment
int main()
{
    srand(time(NULL));

    int num_threads = 4;
    char file_name[20] = "input.txt";

    //Taking input from file part
    FILE *ptr = fopen(file_name, "r");
    if(ptr == NULL)
    {
        printf("FILE DOES NOT EXISTS\n");
        return EXIT_FAILURE;
    }
    fscanf(ptr, "%d", &num_threads);
    fscanf(ptr, "%d", &size_arr);
    int tmp;

    //Place the number in the global array
    for (int i = 0; i < size_arr; i++)
    {
        fscanf(ptr, "%d", &tmp);
        array_num[i] = tmp;
    }
    fclose(ptr);

    //Starting point of the clock
    clock_t t = clock();

    //Creating an array of threads
    pthread_t threads_arr[num_threads];

    //Array of arguments that need to be passed into the merge sort function
    struct arguements_multithread *argruments_merge_sort[num_threads];
    for (int i = 0; i < num_threads; i++)
    {
        argruments_merge_sort[i] = malloc(sizeof(struct arguements_multithread));
    }

    //Looping through and creating multiple threads for each subarray of our main global array
    int p_loop = 0, r_loop = (size_arr / num_threads) - 1;
    for (int i = 0; i < num_threads; i++)
    {
        if (i == num_threads - 1)
        {
            argruments_merge_sort[i]->first_index = p_loop;
            argruments_merge_sort[i]->last_index = size_arr - 1;
            pthread_attr_t attr;
            pthread_attr_init(&attr);
            pthread_create(&threads_arr[i], &attr, merge_sort, argruments_merge_sort[i]);
        }
        else
        {
            argruments_merge_sort[i]->first_index = p_loop;
            argruments_merge_sort[i]->last_index = r_loop;
            pthread_attr_t attr;
            pthread_attr_init(&attr);
            pthread_create(&threads_arr[i], &attr, merge_sort, argruments_merge_sort[i]);

            p_loop = p_loop + (size_arr / num_threads);
            r_loop = r_loop + (size_arr / num_threads);
        }
    }

    //Joining all the threads after they get completed
    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(threads_arr[i], NULL);
    }

    //Merging all the sub arrays to give us the final output array
    int p_merge = 0, q_merge = (size_arr/num_threads) - 1, r_merge = q_merge + (size_arr/num_threads);
    for(int i = 0; i < num_threads - 1; i++)
    {
        if(i == num_threads - 2)
        {
            r_merge = size_arr - 1;
            merge_array_func(p_merge, q_merge, r_merge);
        }
        else
        {
            merge_array_func(p_merge, q_merge, r_merge);
            q_merge = q_merge + (size_arr/num_threads);
            r_merge = r_merge + (size_arr/num_threads);
        }
    }

    //End point for the clock
    t = clock() - t;
    for (int i = 0; i < size_arr; i++)
    {
        printf("%d ", array_num[i]);
    }
    printf("\n");
    printf("Time taken: %f\n", (double)t / CLOCKS_PER_SEC);

    FILE *output_ptr = fopen("output.txt", "w");
    for(int i = 0; i < size_arr; i++)
    {
        fprintf(output_ptr, "%d ", array_num[i]);
    }
    fclose(output_ptr);

    //Running for the case where number of threads are varrying

    return EXIT_SUCCESS;
}

// int main()
// {
//     srand(time(NULL));

//     int num_threads = 4;

//     /****************************************************************
//      * This is the place where you need to make changes to size of array and number of threads
//     */
//     size_arr = 50000;
//     num_threads = 8;
//     /****************************************************************
//     */

//     random_array_generator(size_arr, num_threads);
//     //Starting point of the clock
//     clock_t t = clock();

//     //Creating an array of threads
//     pthread_t threads_arr[num_threads];

//     //Array of arguments that need to be passed into the merge sort function
//     struct arguements_multithread *argruments_merge_sort[num_threads];
//     for (int i = 0; i < num_threads; i++)
//     {
//         argruments_merge_sort[i] = malloc(sizeof(struct arguements_multithread));
//     }

//     //Looping through and creating multiple threads for each subarray of our main global array
//     int p_loop = 0, r_loop = (size_arr / num_threads) - 1;
//     for (int i = 0; i < num_threads; i++)
//     {
//         if (i == num_threads - 1)
//         {
//             argruments_merge_sort[i]->first_index = p_loop;
//             argruments_merge_sort[i]->last_index = size_arr - 1;
//             pthread_attr_t attr;
//             pthread_attr_init(&attr);
//             pthread_create(&threads_arr[i], &attr, merge_sort, argruments_merge_sort[i]);
//         }
//         else
//         {
//             argruments_merge_sort[i]->first_index = p_loop;
//             argruments_merge_sort[i]->last_index = r_loop;
//             pthread_attr_t attr;
//             pthread_attr_init(&attr);
//             pthread_create(&threads_arr[i], &attr, merge_sort, argruments_merge_sort[i]);

//             p_loop = p_loop + (size_arr / num_threads);
//             r_loop = r_loop + (size_arr / num_threads);
//         }
//     }

//     //Joining all the threads after they get completed
//     for (int i = 0; i < num_threads; i++)
//     {
//         pthread_join(threads_arr[i], NULL);
//     }

//     //Merging all the sub arrays to give us the final output array
//     int p_merge = 0, q_merge = (size_arr / num_threads) - 1, r_merge = q_merge + (size_arr / num_threads);
//     for (int i = 0; i < num_threads - 1; i++)
//     {
//         if (i == num_threads - 2)
//         {
//             r_merge = size_arr - 1;
//             merge_array_func(p_merge, q_merge, r_merge);
//         }
//         else
//         {
//             merge_array_func(p_merge, q_merge, r_merge);
//             q_merge = q_merge + (size_arr / num_threads);
//             r_merge = r_merge + (size_arr / num_threads);
//         }
//     }

//     //End point for the clock
//     t = clock() - t;

//     printf("Time taken for sorting size: %d threads: %d is: %f\n", size_arr, num_threads, (double)t / CLOCKS_PER_SEC);
//     for (int i = 0; i < num_threads; i++)
//     {
//         free(argruments_merge_sort[i]);
//     }
//     //FILE *output_csv = fopen("outputcsv.txt", "a");
//     FILE *output_csv = fopen("outputcsv_b.txt", "a");
//     fprintf(output_csv, "%d, %d, %f\n", size_arr, num_threads, (double)t / CLOCKS_PER_SEC);

//     return EXIT_SUCCESS;
// }