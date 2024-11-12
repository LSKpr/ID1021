#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
// #include <linux/time.h>
#define AREAS 10000
#define BUFFER 200
int RECURSIVE_MAX = 0;
int data[1 << 20];
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
typedef struct area
{
    int zip;
    char *name;
    int pop;
} area;
typedef struct codes
{
    area *areas;
    int n;
} codes;
typedef struct bucket
{
    area **array;
    int size;
} bucket;
// bucket -> arr*
#define MOD 2000
bool lookup_bucket(bucket *buckets, int zip)
{
    int bucket_id = zip % MOD;
    // printf("%d\n", bucket_id);
    for (int i = 0; i < buckets[bucket_id].size; i++)
    {
        // printf("%d\n", buckets[bucket_id].array[i]->zip);
        if (buckets[bucket_id].array[i]->zip == zip)
        {
            return 1;
        }
    }

    return 0;
}

void add_to_bucket(bucket *buckets, area *ar)
{
    int bucket_id = ar->zip % MOD;

    // if (bucket_id == 238)
    // {
    //     printf("da\n");
    // }

    bucket *old = &buckets[bucket_id];
    if (old->array == NULL)
    {
        old->size = 0;
    }

    area **new_arr = realloc(old->array, (old->size + 1) * sizeof(area *));
    old->array = new_arr;
    old->array[old->size] = ar;
    old->size++;
}

codes *read_postcodes(char *file)
{
    for (int i = 0; i < 100000; i++)
    {
        data[i] = 0;
    }

    codes *postnr = (codes *)malloc(sizeof(codes));
    area *areas = (area *)malloc(sizeof(area) * AREAS);
    // Open a file in read mode
    FILE *fptr = fopen(file, "r");
    int k = 0;
    char *lineptr = malloc(sizeof(char) * BUFFER);
    size_t n = BUFFER;
    while ((k < AREAS) && (getline(&lineptr, &n, fptr) > 0))
    {
        char *copy = (char *)malloc(sizeof(char) * n);
        strcpy(copy, lineptr);
        if (n != BUFFER)
        {
            // the line was longer than BUFFER
            free(lineptr);
            n = BUFFER;
        }
        // divide the copy into three strings
        area a;
        char *zip = strtok(copy, ",");
        a.zip = atoi(zip) * 100 + atoi(zip + 3);
        data[a.zip]++;
        // a.zip = strtok(copy, ",");
        a.name = strtok(NULL, ",");
        a.pop = atoi(strtok(NULL, ","));
        // update the array of areas
        areas[k++] = a;
    }
    fclose(fptr);
    postnr->areas = areas;
    postnr->n = k;
    return postnr;
}
void collisions(codes *postnr, int mod)
{
    int mx = 20;
    // int data[mod];
    int cols[mx];
    for (int i = 0; i < mod; i++)
    {
        data[i] = 0;
    }
    for (int i = 0; i < mx; i++)
    {
        cols[i] = 0;
    }
    for (int i = 0; i < postnr->n; i++)
    {
        int index = (int)(postnr->areas[i].zip) % mod;
        data[index]++;
    }
    int sum = 0;
    for (int i = 0; i < mod; i++)
    {
        sum += data[i];
        if (data[i] < mx)
            cols[data[i]]++;
    }
    printf("%d (%d) : ", mod, sum);
    for (int i = 1; i < mx; i++)
    {
        printf("%6d ", cols[i]);
    }
    printf("\n");
}
int compare_areas(area *a, area *b)
{
    // a->name == b->name && a->pop == b->pop &&
    if (a->zip == b->zip)
        return 1;
    return 0;
}
int str_comp(char *a, char *b)
{
    for (int i = 0;; i++)
    {
        if (a[i] < b[i])
            return -1;
        if (a[i] > b[i])
            return 1;
        if (a[i] == 0 && b[i] == 0)
            return 0;
    }
    return 0;
}
int linear_search(codes *cd, area *target)
{
    for (int i = 0; i < cd->n; i++)
    {
        // if(target->zip == cd->areas[i].zip) return 1;
        if (str_comp(cd->areas[i].zip, target->zip) == 0)
            return 1;
        // printf("%s\n", cd->areas->zip[i]);
    }
    return 0;
}
int bin_search_int(codes *cd, area *target)
{
    int st = 0, end = cd->n - 1;
    int mid = (st + end) / 2;
    while (st + 1 != end)
    {
        mid = (st + end) / 2;
        if (target->zip == cd->areas[mid].zip)
            return 1;
        if (target->zip < cd->areas[mid].zip)
            end = mid;
        else
            st = mid;
    }
    if (target->zip == cd->areas[st].zip == 0)
        return 1;
    if (target->zip == cd->areas[end].zip == 0)
        return 1;
    return 0;
}
int bin_search(codes *cd, area *target)
{
    int st = 0, end = cd->n - 1;
    int mid = (st + end) / 2;
    while (st + 1 != end)
    {
        mid = (st + end) / 2;
        if (str_comp(target->zip, cd->areas[mid].zip) == 0)
            return 1;
        if (str_comp(target->zip, cd->areas[mid].zip) == -1)
            end = mid;
        else
            st = mid;
    }
    if (str_comp(target->zip, cd->areas[st].zip) == 0)
        return 1;
    if (str_comp(target->zip, cd->areas[end].zip) == 0)
        return 1;
    return 0;
}
int arr_lookup(char *zip)
{
    int key = atoi(zip) * 100 + atoi(zip + 3);
    return (data[key] > 0);
}
int add_hasharr(area **areas, area *ar, int mod)
{
    int index = ar->zip % mod;
    int cnt = 0;

    while (areas[index] != NULL)
    {
        index++;
        index %= mod;
        cnt++;
        if (cnt >= mod)
            return -1;
    }

    areas[index] = ar;
    return cnt;
}
bool lookup_hasharr(area **areas, int zip, int mod)
{
    int index = zip % mod;
    int cnt = 0;
    while (areas[index]->zip != zip)
    {
        if (areas[index] == NULL)
            return 0;
        index++;
        index %= mod;
        cnt++;
        if (cnt > mod)
            return 0;
    }
    return 1;
}

double bench(int n, int loop, int random)
{
    // struct timespec t_start, t_stop;
    codes *cd = read_postcodes("./postnummer.csv");
    int x = 0;
    area *tg = malloc(sizeof(area));
    area *arstable = malloc(sizeof(area *) * n);
    double sum = 0;
    for (int i = 0; i < cd->n; i++)
    {
        int x = add_hasharr(arstable, &cd->areas[i], n);
        sum += (double)x;
        if (x < 0)
            sum += (double)100000;
        // if (x > 0)
        //     printf("%d, ", x);
    }
    return sum / cd->n;
    printf("\n%d\n", lookup_hasharr(arstable, 98499, n));
    // tg->zip = "984 99";
    // tg->zip = 98499;
    // tg->zip = "111 15";
    // tg->zip = 11115
    // collisions(cd, n);
    // clock_gettime(CLOCK_MONOTONIC, &t_start);
    for (int i = 0; i < n; i++)
    {
        // x = linear_search(cd,tg);
        // x = bin_search_int(cd,tg);
        // x = arr_lookup("111 15");
    }
    // clock_gettime(CLOCK_MONOTONIC, &t_stop);
    // long wall = nano_seconds(&t_start, &t_stop) / n;
    // return wall;
    return 0;
}
void small_main()
{
    int n = 20;
    int *a = create_arr(n);
    int *b = create_arr(20);

    codes *cd = read_postcodes("./postnummer.csv");
    // for (int i = 0; i < 10; i++)
    // {
    //     printf("%s, %s\n",cd->areas[i].name, cd->areas[i].zip);
    // }
    bucket *buckets = malloc(MOD * sizeof(bucket));
    for (int i = 0; i < cd->n; i++)
    {
        add_to_bucket(buckets, &cd->areas[i]);
    }

    if (lookup_bucket(buckets, 98238))
        printf("lalal");
    // area* tg = malloc(sizeof(area));
    // tg->zip = "999 39";
    // printf("linear: %d bin: %d\n",linear_search(cd,tg),bin_search(cd,tg));
    // if("982 38" == cd->areas[9654].zip) printf("adaas");
    // printf("%s",cd->areas[9654].zip);
}
int main()
{
    // small_main();
    // return 0;
    int sizes[] = {1000, 2000, 4000, 8000, 16000, 32000, 64000, 128000, 256000, (1 << 9) * 1000, (1 << 10) * 1000};
    int k = 1;
    int loop = 1; // keep it 1
    for (int i = 0; i < 20; i++)
    {
        int n = 10000 + (i) * 5000;
        long min = 2147483647;
        for (int i = 0; i < k; i++)
        {
            long wall = bench(n, loop, 0);
            if (wall < min)
                min = wall;
        }
        // printf("%d %f ns\n", n, (double)min / loop);
        printf("%d %0.2f\n", n, bench(n, loop, 0));
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