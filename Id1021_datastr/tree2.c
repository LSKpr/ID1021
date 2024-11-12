#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
// #include <linux/time.h>
int RECURSIVE_MAX = 0;
// long nano_seconds(struct timespec *t_start, struct timespec *t_stop)
// {
//     return (t_stop->tv_nsec - t_start->tv_nsec) +
//            (t_stop->tv_sec - t_start->tv_sec) * 1000000000;
// }
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
    struct node *right;
    struct node *left;
} node;



typedef struct tree
{
    node *root;
} tree;

typedef struct qnode {
    node *value;         // Changed from int to node*
    struct qnode *next;
} qnode;

typedef struct queue {
    qnode *first;
} queue;


queue *create_queue() {
    queue *q = (queue *)malloc(sizeof(queue));
    q->first = NULL;
    return q;
}

int empty(queue *q) {
    return (q->first == NULL);
}

void enqueue(queue *q, node *v) {  // Changed parameter type to node*
    qnode *nd = (qnode *)malloc(sizeof(qnode));
    nd->value = v;
    nd->next = NULL;

    if (q->first == NULL) {
        q->first = nd;
    } else {
        qnode *current = q->first;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = nd;
    }
}

node *dequeue(queue *q) {  // Changed return type to node*
    if (q->first != NULL) {
        node *res = q->first->value;
        qnode *temp = q->first;
        q->first = q->first->next;
        free(temp);
        return res;
    } else {
        printf("QUEUE EMPTY!!!\n");
        return NULL;
    }
}

tree *construct_tree()
{
    tree *tr = (tree *)malloc(sizeof(tree));
    tr->root = NULL;
    return tr;
}
// free node / lumberjack
void lumberjack(node *v)
{
    if (v->left != NULL)
        lumberjack(v->left);
    if (v->right != NULL)
        lumberjack(v->right);
    free(v);
}
void free_tree(tree *tr)
{
    lumberjack(tr->root);
    free(tr);
}

node *construct_node(int val)
{
    node *nd = (node *)malloc(sizeof(node));
    nd->value = val;
    nd->left = NULL;
    nd->right = NULL;
    return nd;
}
void add(tree *tr, int value) // iterative
{
    node *v = construct_node(value);
    node *now = tr->root;
    node *prv = NULL;
    if (now == NULL)
    {
        tr->root = v;
        return;
    }
    while (now != NULL)
    {
        prv = now;
        if (value > now->value)
            now = now->right;
        else
            now = now->left;
    }
    if (value > prv->value)
        prv->right = v;
    else
        prv->left = v;
}
bool sherlock(node *v, int value)
{
    if (v->value == value)
        return 1;
    if (value > v->value)
        return sherlock(v->right, value);
    else
        return sherlock(v->left, value);
}
bool lookup(tree *tr, int value) // recursive
{
    return sherlock(tr->root, value);
}
void print_bfs(tree *tr)
{
    queue* q = create_queue();
    node *cur = tr->root;
    enqueue(q, cur);
    while(!empty(q))
    {
        cur = dequeue(q);
        printf("%d ",cur->value);
        if(cur->left!=NULL) enqueue(q,cur->left);
        if(cur->right!=NULL) enqueue(q,cur->right);   
    }
    free(q);
}
typedef struct sequence
{
    queue *q ;
}sequence;

sequence *create_sequence(tree *tr)
{
    sequence* seq = malloc(sizeof(sequence));
    seq->q = create_queue();
    node *cur = tr->root;
    enqueue(seq->q, cur);
    return seq;
}
int next(sequence* seq)
{
    if(empty(seq->q))   return 0; 
    node* cur = dequeue(seq->q);
    if(cur->left!=NULL) enqueue(seq->q,cur->left);
    if(cur->right!=NULL) enqueue(seq->q,cur->right); 
    return cur->value;
    
}
static void print_in_ord(node *nd)
{
    if (nd != NULL)
    {
        print_in_ord(nd->left);
        printf("%d ", nd->value);
        print_in_ord(nd->right);
    }
}
void print_tree(tree *tr)
{
    if (tr->root != NULL)
        print_in_ord(tr->root);
    printf("\n");
}

// int bench(int n, int loop, int random)
// {
//     struct timespec t_start, t_stop;
//     tree *tr = construct_tree();
//     for (int i = 0; i < n; i++)
//     {
//         add(tr, n - i * (-1)^(i));
//         printf("%d ", i);
//     }

//     clock_gettime(CLOCK_MONOTONIC, &t_start);
//     for (int i = 0; i < n; i++)
//     {
//         // dequeue(q);
//     }
//     clock_gettime(CLOCK_MONOTONIC, &t_stop);
//     long wall = nano_seconds(&t_start, &t_stop) / n;
//     free_tree(tr);
//     return wall;
// }
void small_main()
{
    //     int n = 10;
    tree *tr = construct_tree();
    //     for (int i = 0; i < n; i++)
    //     {
    //         add(tr, n - i * (-1)^(i));
    //         printf("%d ", i);
    //     }
    add(tr, 6);
    add(tr, 3);
    add(tr, 10);
    add(tr, 2);
    add(tr, 5);
    add(tr, 4);
    add(tr, 8);
    add(tr, 7);
    add(tr, 9);
    add(tr, 13);
    add(tr, 12);

    print_tree(tr);
    printf("\nbfs\n");
    print_bfs(tr);
    printf("\nnext\n");
    int x = 0;
    sequence *seq = create_sequence(tr);
    while(x=next(seq))  printf("%d ",x);
    free_tree(tr);
}
int main()
{
    small_main();
    return 0;
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