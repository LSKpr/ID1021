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
typedef struct qnode
{
    int value;
    struct qnode *next;
} qnode;

typedef struct queue
{
    qnode *first;
} queue;

queue *create_queue()
{
    queue *q = (queue *)malloc(sizeof(queue));
    q->first = NULL;
    return q;
}
int empty(queue *q)
{
    if (q->first == NULL)
        return 1;
    return 0;
}
void enque(queue *q, int v)
{
    qnode *nd = malloc(sizeof(qnode));
    nd->value = v;
    nd->next = NULL;
    qnode *prv = NULL;
    qnode *nxt = q->first;
    while (nxt != NULL)
    {
        prv = nxt;
        nxt = nxt->next;
    }
    if (prv != NULL)
    {
        prv->next = nd;
    }
    else
    {
        q->first = nd;
    }
}
int dequeue(queue *q)
{
    int res = 0;
    if (q->first != NULL)
    {
        res = q->first->value;
        qnode *prv = q->first;
        q->first = q->first->next;
        free(prv);
    }
    return res;
}

int bench(int n, int loop, int random)
{
    struct timespec t_start, t_stop;
    queue *q = create_queue();
    // for (int i = 0; i < n; i++)
    // {
    //      enque(q, i);
    // }
    
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    for (int i = 0; i < n; i++)
    {
        enque(q,i);
        // dequeue(q);
    }
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    long wall = nano_seconds(&t_start, &t_stop)/n;
    free(q);
    return wall;
}
void small_main()
{
    int n = 20;
    int *a = create_arr(n);
    int *b = create_arr(20);
    queue *q = create_queue();
    // linked *lnk = create_linked();
    // linked *lnk2 = create_linked();
    for (int i = 0; i < n; i++)
    {
        enque(q, i);
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

    for (int i = 0; i < n + 1; i++)
    {
        printf("%d \n", dequeue(q));
    }
}
int main()
{
    // small_main();
    // return 0;
    int sizes[] = {1000, 2000, 4000, 8000, 16000, 32000, 64000, 128000, 256000, (1 << 9) * 1000, (1 << 10) * 1000};
    int k = 16;
    int loop = 1; // keep it 1
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