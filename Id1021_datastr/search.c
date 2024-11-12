#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <linux/time.h>
int RECURSIVE_MAX = 0;
long nano_seconds(struct timespec *t_start, struct timespec *t_stop)
{
    return (t_stop->tv_nsec - t_start->tv_nsec) +
           (t_stop->tv_sec - t_start->tv_sec) * 1000000000;
}
int unsorted_search(int *array, int length, int key)
{
    for (int index = 0; index < length; index++)
    {
        if (array[index] == key)
        {
            return 1;
        }
    }
    return 0;
}

int *sorted(int n)
{
    int *array = (int *)malloc(n * sizeof(int));
    int nxt = 0;
    for (int i = 0; i < n; i++)
    {
        // nxt += rand() % 10 + 1;
        nxt++;
        array[i] = nxt;
    }
    return array;
}

int binary_search(int* array, int length, int key)
{
    int first = 0;
    int last = length - 1;
    int index = (first + last)/2;

    while (first + 1 != last)
    {
        index = (first + last)/2;
        if(array[index] < key)  first = index;
        else last = index;
    }
    if(array[index] == key || array[key+1]==key)
        return 1;
    return 0;
}

int recursive(int* array, int length, int key, int first, int last, int recursive_cnt)
{
    recursive_cnt++;
    // printf("%d, %d \n", first, last);
    if(first > last) return 0;
    if(RECURSIVE_MAX < recursive_cnt) RECURSIVE_MAX = recursive_cnt;
    while (first + 1 != last)
    {
        int index = (first + last)/2;
        if (array[index] == key || array[index+1] == key)
        {
            return 1;
        }
        if (array[index] < key)
            return recursive(array, length, key, index, last, recursive_cnt);
        
        if (array[index] > key)
        {
            return recursive(array, length, key, first, index, recursive_cnt);
        }
    }
    return 0;
}
int bench(int n, int loop, int random)
{
    struct timespec t_start, t_stop;
    int* keys = malloc(loop * sizeof(int));
    for (int i = 0; i < loop; i++)
    {
        keys[i] = rand()%n + 1; 
    }
    
    int* arr = sorted(n);
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    for (int i = 0; i < loop; i++)
    {
        recursive(arr, n, keys[i],0,n-1,0);
    }
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    long wall = nano_seconds(&t_start, &t_stop);
    free(arr);
    free(keys);
    return wall;
}
int main()
{
    int sizes[] = {1000, 2000, 4000, 8000, 16000, 32000,64000, 128000, 256000, (1<<9) * 1000, (1<<10) * 1000};
    int k = 10000;
    int loop = 100;
    for (int i = 0; i < 11; i++)
    {
        int n = sizes[i];
        long min = 2147483647;
        for (int i = 0; i < k; i++)
        {
            long wall = bench(n, loop, 0);
            if (wall < min)
                min = wall;
        }
        // printf("%d %0.2f ns\n", n, (double)min / loop);
        printf("%d %d\n", n, RECURSIVE_MAX);
        RECURSIVE_MAX = 0;
    }
    // int n = 100;
    // int* arr = sorted(n);
    // for (int i = 0; i < n; i++)
    // {
    //     printf("%d \n", arr[i]);  
    // }
    
    // printf("\n%d ",binary_search(arr,n,121));
    // printf("\nRANDOM WALK:\n");
    // for (int i = 0; i < 6; i++)
    // {
    //     int n = sizes[i];
    //     long min = 2147483647;
    //     for (int i = 0; i < k; i++)
    //     {
    //         long wall = bench(n, loop, 1);
    //         if (wall < min)
    //             min = wall;
    //     }
    //     printf("%d %0.2f ns\n", n, (double)min / loop);
    // }
}