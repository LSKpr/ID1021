#include <stdlib.h>
#include <stdio.h>
#include <linux/time.h>
#define MIN_STACK_SIZE 128
long nano_seconds(struct timespec *t_start, struct timespec *t_stop)
{
    return (t_stop->tv_nsec - t_start->tv_nsec) +
           (t_stop->tv_sec - t_start->tv_sec) * 1000000000;
}
typedef struct stack
{
    int top;
    int size;
    int *array;
} stack;
stack *new_stack(int size)
{
    if(size < MIN_STACK_SIZE)
        size = MIN_STACK_SIZE;
    int *array = (int *)malloc(size * sizeof(int));
    stack *stk = (stack *)malloc(sizeof(stack));
    stk->array = array;
    stk->size = size;
    stk->top = 0;
    return stk;
}
void push(stack *stk, int val)
{
    if (stk->top >= stk->size)
    {   
        // printf("RESIZE +");
        int old_size = stk->size;
        stk->size *= 2;
        int *copy = (int*)malloc(stk->size * sizeof(int));
        for (int i = 0; i < old_size; i++)
        {
            copy[i] = *(stk->array + i);
        }
        free(stk->array);
        stk->array = copy;
    }
    *(stk->array + stk->top) = val;
    stk->top ++;
}
int pop(stack *stk)
{
    
    if(((stk->top-1)*4 <= stk->size) && stk->size >= MIN_STACK_SIZE*2)
    {
        // printf("RESIZE -, new_size = %d ", stk->size/2);

        stk->size /= 2; 
        int *copy = (int*)malloc(stk->size * sizeof(int));
        for (int i = 0; i < stk->top; i++)
        {
            copy[i] = *(stk->array + i);
        }
        free(stk->array);
        stk->array = copy;
    }
    
    if (stk->top <= 0)
    {
        printf("\nSTACK EMPTY!!!\n");
        return -2147483648;
    }        
    stk->top--;
    return *(stk->array + stk->top);
}

int bench(int n, int loop, int random)
{
    struct timespec t_start, t_stop;
    if (random == 1)
    {
        int *random_walk = malloc(n * sizeof(int));
        for (int i = 0; i < n; i++)
        {
            random_walk[i] = rand() % 2;
        }

        stack *stk = new_stack(n);
        clock_gettime(CLOCK_MONOTONIC, &t_start);
        for (int i = 0; i < loop; i++)
        {
            if (random_walk[i] == 1)
                pop(stk);
            else
                push(stk,1);
        }
        clock_gettime(CLOCK_MONOTONIC, &t_stop);
        long wall = nano_seconds(&t_start, &t_stop);
        free(stk);
        free(random_walk);
        return wall;
    }

    stack *stk = new_stack(n);
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    for (int i = 0; i < loop; i++)
    {
        for (int j = 0; j < n; j++)
            push(stk, 32);
        for (int j = 0; j < n; j++)
            pop(stk);
    }
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    long wall = nano_seconds(&t_start, &t_stop)/n;
    free(stk);
    return wall;
}
int main()
{
    int sizes[] = {1000, 2000, 4000, 8000, 16000, 32000,64000, 128000, 256000};
    int k = 10;
    int loop = 1000;
    for (int i = 0; i < 9; i++)
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
    }
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