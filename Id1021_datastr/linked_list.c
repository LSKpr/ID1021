#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
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
int *create_arr(int n)
{
    int *arr = malloc(n * (sizeof(int)));
    for (int i = 0; i < n; i++)
    {
        arr[i] = rand() % (2 * n);
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
typedef struct cell cell;

struct cell
{
    int value;
    cell *tail;
};

typedef struct linked
{
    cell *first;
} linked;

linked *create_linked()
{
    linked *new = (linked *)malloc(sizeof(linked));
    new->first = NULL;
    return new;
}
void free_linked(linked *lnk)
{
    cell *nxt = lnk->first;
    while (nxt != NULL)
    {
        cell *tmp = nxt->tail;
        free(nxt);
        nxt = tmp;
    }
    free(lnk);
}
void add(linked *lnk, int item)
{
    cell *new = (cell *)malloc(sizeof(cell));
    new->value = item;
    new->tail = NULL;
    cell *last = lnk->first;
    cell *prv = NULL;
    while (last != NULL)
    {
        prv = last;
        last = last->tail;
    }
    if (prv == NULL)
    {
        lnk->first = new;
        return;
    }
    prv->tail = new;
}

int length(linked *lnk)
{
    int len = 0;
    cell *node = lnk->first;
    while (node != NULL)
    {
        node = node->tail;
        len++;
    }
    return len;
}
bool find(linked *lnk, int item)
{
    cell *node = lnk->first;
    while (node != NULL)
    {
        if (node->value == item)
            return true;
        node = node->tail;
    }
    return false;
}
void remove_item(linked *lnk, int item)
{
    cell *node = lnk->first;
    cell *previous = NULL;
    while (node != NULL)
    {
        if (node->value == item)
        {
            if (previous == NULL)
                lnk->first = node->tail;
            else
                previous->tail = node->tail;
            free(node);
            return;
        }
        previous = node;
        node = node->tail;
    }
}

void append(linked *a, linked *b)
{
    cell *nxt = a->first;
    cell *prv = NULL;
    while (nxt != NULL)
    {
        prv = nxt;
        nxt = nxt->tail;
    }
    if (prv != NULL)
    {
        prv->tail = b->first;
        b->first = NULL;
    }
    else
    {
        a->first = b->first;
        b->first = NULL;
    }
}
void print(linked *lnk)
{
    cell *node = lnk->first;
    while (node != NULL)
    {
        printf("%d ", node->value);
        node = node->tail;
    }
    printf("\n");
}
linked *init_list(int n)
{
    linked *a = create_linked();
    for (int i = 0; i < n; i++)
    {
        add(a, i);
    }
    return a;
}
int* append_array(int* a, int len_a, int* b, int len_b)
{
    int* new = malloc((len_a+len_b)*sizeof(int));
    for (int i = 0; i < len_a; i++)
        new[i] = a[i];
    for (int i = 0; i < len_b; i++)
        new[len_a+i] = b[i];

    free(a);
    free(b);
    return new;
}


int bench(int n, int loop, int random)
{
    struct timespec t_start, t_stop;
    linked* a = init_list(2);
    linked* b = init_list(n);
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    // for (int i = 0; i < loop; i++)
    // {
        append(a,b);
    // }
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    long wall = nano_seconds(&t_start, &t_stop);
    return wall;
}
void small_main()
{
    int n = 20;
    int * a = create_arr(n);
    int *b = create_arr(20);
    // linked *lnk = create_linked();
    // linked *lnk2 = create_linked();
    for (int i = 0; i < n; i++)
    {
        // add(lnk, i);
        append_array(a,n,b,200);
    }
    printf("\n");
    // print(lnk);
    // printf("len: %d\n", length(lnk));
    // remove_item(lnk, 5);
    // print(lnk);
    // printf("len: %d\n", length(lnk));
    // printf(" 5: %d, 6: %d\n", find(lnk, 5), find(lnk, 6));
    // append(lnk2, lnk);
    // print(lnk);
    // print(lnk2);

    // for (int i = 0; i < n / 2; i++)
    // {
    //     remove_item(lnk, i);
    // }
    // print(lnk);
}
int main()
{
    // small_main();
    // return 0;
    int sizes[] = {1000, 2000, 4000, 8000, 16000, 32000, 64000, 128000, 256000, (1 << 9) * 1000, (1 << 10) * 1000};
    int k = 1;
    int loop = 1; //keep it 1
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