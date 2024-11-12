
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
int* create_arr(int n)
{
    int *arr = malloc(n * (sizeof(int)) );
    for (int i = 0; i < n; i++)
    {
        arr[i] = rand() % (2*n);
    }
    return arr;
    
}
void swap(int *a, int *b)
{
    int buff = *a;
    *a = *b;
    *b = buff;
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
void insertion_sort(int *arr, int n)
{
    for (int i = 1; i < n; i++)
    {
        for (int j = i; j > 0; j--)
        {
            if (arr[j] < arr[j - 1])
                swap(&arr[j], &arr[j - 1]);
            else
                break;
            
        }
    }
}

void selection_sort(int *array, int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        int candidate = array[i];
        int pos = i;
        for (int j = i; j < n; j++)
        {
            if (candidate > array[j])
            {
                pos = j;
                candidate = array[j];
            }
        }
        swap(&array[pos], &array[i]);
    }
}
void merge(int *org, int *aux, int lo, int mid, int hi)
{
    for (int i = lo; i <= hi; i++)
    {
        aux[i] = org[i];
    }
    int i = lo;      
    int j = mid + 1; 
    for (int k = lo; k <= hi; k++)
    {
        if (i > mid)
        {
            org[k] = aux[j];
            j++;
        }else if(j > hi){
            org[k] = aux[i];
            i++;
        }else if(aux[j] <= aux[i])
        {
            org[k] = aux[j];
            j++;
        }
        else{
            org[k] = aux[i];
            i++;
        }
    }
}
void merge_sort(int *org, int *aux, int lo, int hi)
{
    if (lo != hi)
    {
        int mid = (lo + hi) / 2;
        merge_sort(org, aux, lo, mid);
        merge_sort(org, aux, mid + 1, hi);
        merge(org, aux, lo, mid, hi);
    }
}
void sort(int *org, int n)
{
    if (n == 0)
        return;
    int *aux = (int *)malloc(n * sizeof(int));
    merge_sort(org, aux, 0, n - 1);
    free(aux);
}
int bench(int n, int loop, int random)
{
    struct timespec t_start, t_stop;
    int *arr = create_arr(n);
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    for (int i = 0; i < loop; i++)
    {
        sort(arr,n);
    }
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    long wall = nano_seconds(&t_start, &t_stop);
    free(arr);
    return wall;
}
void small_main()
{
    int n = 20;
    int *arr = create_arr(n);
    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");

    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }
}
int main()
{
//     small_main();
//     return 0;
    int sizes[] = {1000, 2000, 4000, 8000, 16000, 32000, 64000, 128000, 256000, (1 << 9) * 1000, (1 << 10) * 1000};
    int k = 100;
    int loop = 1;
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
        printf("%d %0.2f ns\n", n, (double)min / loop);
        // printf("%d %d\n", n, RECURSIVE_MAX);
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