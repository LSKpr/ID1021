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
typedef struct node
{
    int value;
    struct node *next;
} node;

typedef struct queue
{
    int *arr;
    int first;
    int last;
    int arr_len;
} queue;

queue *create_queue(int n)
{
    if (n < 4)
        n = 4;
    queue *q = (queue *)malloc(sizeof(queue));
    q->arr = malloc(n * sizeof(int));
    q->arr_len = n;
    q->first = -1;
    q->last = -1;
    return q; 
}
int empty(queue *q)
{
    if (q->first == -1)
        return 1;
    return 0;
}
// 0 1 2 3 4 5 6 7
//     l     f
// X X X X X X X X
int queue_size(queue *q)
{
    if (empty(q))
        return 0;
    return (q->last - q->first + q->arr_len) % q->arr_len + 1;
}
void resize(queue *q, double mult)
{
    int new_size = (int)((double)q->arr_len * mult);
    // printf("RESIZE: %f old_size: %d, new_size: %d\n", mult, q->arr_len, new_size);

    int *new_arr = malloc(new_size * sizeof(int));
    for (int i = 0; i < queue_size(q); i++)
    {
        new_arr[i] = q->arr[(i + q->first) % q->arr_len];
    }
    if (empty(q))
    {
        q->first = -1;
        q->last = -1;
    }
    else
    {
        q->last = queue_size(q) - 1;
        q->first = 0;
    }
    q->arr_len = new_size;
    free(q->arr);
    q->arr = new_arr;
}

void enque(queue *q, int v)
{
    if (empty(q))
    {
        q->first = 0;
        q->last = 0;
        q->arr[q->last] = v;
    }
    else
    {
        if (queue_size(q) == q->arr_len)
            resize(q, 2);
        q->last++;
        q->last %= q->arr_len;
        q->arr[q->last] = v;
    }
}
int dequeue(queue *q)
{
    int res = 0;
    if (!empty(q))
    {
        res = q->arr[q->first];
        if (queue_size(q) == 1)
        {
            q->first = -1;
            q->last = -1;
        }
        else
        {
            q->first++;
            q->first %= q->arr_len;
        }
    }
    if (q->arr_len > 4 && queue_size(q) * 4 < q->arr_len)
        resize(q, 0.5);
    return res;
}

int bench(int n, int loop, int random)
{
    struct timespec t_start, t_stop;
    queue *q = create_queue(4);
    for (int i = 0; i < n; i++)
    {
        // enque(q, i);
    }

    clock_gettime(CLOCK_MONOTONIC, &t_start);
    for (int i = 0; i < n; i++)
    {
        enque(q,i);
        // dequeue(q);
    }
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    long wall = nano_seconds(&t_start, &t_stop) / n;
    free(q);
    return wall;
}
void print_que(queue *q)
{
    for (int i = 0; i < q->arr_len; i++)
    {
        printf("%d ", q->arr[i]);
    }
    printf("\n");
}
void small_main()
{
    int n = 20;
    int *a = create_arr(n);
    int *b = create_arr(20);
    queue *q = create_queue(4);
    // // linked *lnk = create_linked();
    // // linked *lnk2 = create_linked();
    for (int i = 1; i <= 128; i++)
    {
        enque(q, i);
    }
    // printf("\n");
    // // print(lnk);
    // // printf("len: %d\n", length(lnk));
    // // remove_item(lnk, 5);
    // // print(lnk);
    // // printf("len: %d\n", length(lnk));
    // // printf(" 5: %d, 6: %d\n", find(lnk, 5), find(lnk, 6));
    // // append(lnk2, lnk);
    // // print(lnk);
    // // print(lnk2);

    // for (int i = 0; i < n + 5; i++)
    // {
    //     printf("%d \n", dequeue(q));
    // }
    queue *qa = create_queue(100);
    print_que(qa);
    char c;
    int x;
    scanf(" %d", &x); // Added space before %c and used & for c and x
    while (x != -1)
    {
        if (x == 0)
            printf("deq: %d\n", dequeue(q));
        else
            enque(q, x);

        printf("QUEUE_EL_SIZE: %d , first: %d, last: %d, arr_siz: %d\n", queue_size(q), q->first, q->last, q->arr_len);
        print_que(q);
        // scanf(" %d", &x); // Added space before %c and used & for c and x
    }
}
int main()
{
    // small_main();
    // return 0;
    int sizes[] = {1000, 2000, 4000, 8000, 16000, 32000, 64000, 128000, 256000, (1 << 9) * 1000, (1 << 10) * 1000};
    int k = 160;
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