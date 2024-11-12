#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define MIN_STACK_SIZE 8
typedef struct stack
{
    int top;
    int size;
    int *array;
} stack;
stack *new_stack(int size)
{
    if (size < MIN_STACK_SIZE)
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
        int *copy = (int *)malloc(stk->size * sizeof(int));
        for (int i = 0; i < old_size; i++)
        {
            copy[i] = *(stk->array + i);
        }
        free(stk->array);
        stk->array = copy;
    }
    *(stk->array + stk->top) = val;
    stk->top++;
}
int pop(stack *stk)
{

    if (((stk->top - 1) * 4 <= stk->size) && stk->size >= MIN_STACK_SIZE * 2)
    {
        // printf("RESIZE -, new_size = %d ", stk->size/2);

        stk->size /= 2;
        int *copy = (int *)malloc(stk->size * sizeof(int));
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
int main()
{
    stack *stk = new_stack(0);
    printf("HP-35 pocket calculator\n");
    size_t n = 10;
    char *buffer = (char *)malloc(n * sizeof(char));
    int run = 1;
    while (run)
    {
        printf(" > ");
        getline(&buffer, &n, stdin);
        if (strcmp(buffer, "\n") == 0)
        {
            run = 0;
        }
        else if (strcmp(buffer, "+\n") == 0)
        {
            int a = pop(stk);
            int b = pop(stk);
            push(stk, a + b);
        }
        else if (strcmp(buffer, "*\n") == 0)
        {
            int a = pop(stk);
            int b = pop(stk);
            push(stk, a * b);
        }
        else if (strcmp(buffer, "-\n") == 0)
        {
            int a = pop(stk);
            int b = pop(stk);
            push(stk, b - a);
        }
        else if (strcmp(buffer, "/\n") == 0)
        {
            int a = pop(stk);
            int b = pop(stk);
            push(stk, b / a);
        }
        else
        {
            int val = atoi(buffer);
            push(stk, val);
        }
    }
    // 4 2 3 * 4 + 4 * + 2 -
    printf("the result is: %d\n\n", pop(stk));
    printf("I love reversed polish notation, don't you?\n");
}