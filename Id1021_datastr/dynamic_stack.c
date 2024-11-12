#include <stdlib.h>
#include <stdio.h>

typedef struct stack
{
    int top;
    int size;
    int *array;
} stack;
stack *new_stack(int size)
{
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
    if (stk->top <= 0)
    {
        printf("\nSTACK EMPTY!!!\n");
        return -2147483648;
    }
    stk->top--;
    return *(stk->array + stk->top);
}
int main()
{
    stack *stk = new_stack(4);
    push(stk, 32);
    push(stk, 33);
    push(stk, 34);
    push(stk, 33);
    push(stk, 34);
    printf("pop : %d\n", pop(stk));
    printf("pop : %d\n", pop(stk));
    printf("pop : %d\n", pop(stk));
    printf("pop : %d\n", pop(stk));
    printf("pop : %d\n", pop(stk));
    printf("pop : %d\n", pop(stk));
    printf("stk size: %d", stk->size);
}