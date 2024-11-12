#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
// #include <linux/time.h>
#include <wchar.h>
#include <locale.h>
#define BUFFER 200
int RECURSIVE_MAX = 0;
// long nano_seconds(struct timespec *t_start, struct timespec *t_stop)
// {
//     return (t_stop->tv_nsec - t_start->tv_nsec) +
//            (t_stop->tv_sec - t_start->tv_sec) * 1000000000;
// }
typedef struct node node;
typedef struct node
{
    bool valid;
    struct node *next[27];
};
typedef struct trie
{
    node *root;
} trie;
#include <stdlib.h>
#include <wchar.h>
#include <string.h>

void append(wchar_t ***list, wchar_t *str)
{
    int size = 0;
    if (*list != NULL)
    {
        while ((*list)[size] != NULL)
        {
            size++;
        }
    }
    wchar_t **new_list = realloc(*list, (size + 2) * sizeof(wchar_t *));
    *list = new_list;
    (*list)[size] = malloc((wcslen(str) + 1) * sizeof(wchar_t));
    if ((*list)[size] == NULL)
    {
        return;
    }
    wcscpy((*list)[size], str);
    (*list)[size + 1] = NULL;
}

int key_to_index(wchar_t w)
{
    if (w > '9' && w < '1')
        return -1;
    return (int)(w - '1');
}
static int code(wchar_t w)
{

    int ret = 0;
    if (w == 'q' || w == 'w')
        return -1;
    if (w > 'q')
        ret++;
    if (w > 'w')
        ret++;
    if (w >= 'a' && w <= 'z')
    {
        return (int)(w - 'a' - ret);
    }

    switch (w)
    {
    case 229:
        return 24;
    case 228:
        return 25;
    case 246:
        return 26;
    }
    printf("strange character in word: %d\n", w);
    return -1;
}
wchar_t inv_code(int v)
{
    wchar_t ret = 'a';
    switch (v)
    {
    case 24:
        return ret = 229;
    case 25:
        return ret = 228;
    case 26:
        return ret = 246;
    }
    int plus = 0;
    if (v >= ('q' - 'a'))
        plus++;
    if (v >= ('w' - 'a' - 1))
        plus++;
    return ret = ret + v + plus;
}

wchar_t char_to_key(wchar_t w)
{
    int key = code(w) / 3;
    wchar_t ret = ('1' + key);
    return ret;
}
node *add(node *nd, wchar_t *rest, int iter)
{
    // printf("%c considering deph: %d\n", rest[iter], iter);
    if (nd == NULL)
    {
        nd = malloc(sizeof(node));
        nd->valid = 0;
    }
    int c = code(rest[iter]);
    if (rest[iter + 1] == '\n')
    {

        if (nd->next[c] == NULL)
        {
            node *new;
            new = malloc(sizeof(node));
            new->valid = 1;
            nd->next[c] = new;
        }
        else
            nd->next[c]->valid = 1;

        // printf("%c valid deph: %d\n", rest[iter], iter);
    }
    else
    {
        nd->next[c] = add(nd->next[c], rest, iter + 1);
        // printf("%c added deph: %d\n", rest[iter], iter);
    }
    return nd;
}
void collect(wchar_t ***list, node *nd, wchar_t *keycode, int iter, wchar_t *ans, int ans_iter)
{
    int key = keycode[iter] - '1';

    for (int i = 0; i < 3; i++)
    {
        ans[ans_iter] = inv_code(key * 3 + i);
        ans[ans_iter + 1] = L'\0';

        if (nd->next[key * 3 + i] != NULL)
        {

            if (keycode[iter + 1] == '\n')
            {
                if (nd->next[key * 3 + i]->valid)
                    append(list, ans);
                // wprintf(L"%ls\n", ans);
                continue;
            }
            collect(list, nd->next[key * 3 + i], keycode, iter + 1, ans, ans_iter + 1);
        }
    }
}
wchar_t **decode(trie *tr, wchar_t *keycode)
{
    wchar_t **ret = malloc(sizeof(wchar_t *));
    ret[0] = NULL;
    wchar_t *word = malloc(sizeof(wchar_t) * 100); // 100 is max word len
    collect(&ret, tr->root, keycode, 0, word, 0);
    free(word);
    return ret;
}
trie *dict()
{
    setlocale(LC_ALL, "en_US.UTF-8");
    // Open the dictionary in read mode
    FILE *fptr = fopen("kelly.txt", "r");
    if (fptr == NULL)
        return NULL;
    trie *kelly = (trie *)malloc(sizeof(trie));
    kelly->root = NULL;
    wchar_t ws[BUFFER];
    while (fgetws(ws, BUFFER, fptr) != NULL)
    {
        // printf("adding %S", ws);
        kelly->root = add(kelly->root, ws, 0);
    }
    fclose(fptr);
    return kelly;
}
// int bench(int n, int loop, int random)
// {
//     struct timespec t_start, t_stop;
//     for (int i = 0; i < n; i++)
//     {
//         // enque(q, i);
//     }

//     clock_gettime(CLOCK_MONOTONIC, &t_start);
//     for (int i = 0; i < n; i++)
//     {
//         // dequeue(q);
//     }
//     clock_gettime(CLOCK_MONOTONIC, &t_stop);
//     long wall = nano_seconds(&t_start, &t_stop) / n;
//     return wall;
// }

void small_main()
{
    wchar_t *word = L"min\n";
    for (int i = 0;; i++)
    {
        if (word[i] == '\n')
            break;
        wprintf(L"%lc", char_to_key(word[i]));
    }
    printf("\n");
    wprintf(L"%ls\n", char_to_key);

    // wc = 'v'; // Use wide character literal
    // Correct format specifier for wide characters is "%lc"
    trie *t = dict();
    wchar_t *keycode = L"535\n";
    wchar_t **list = decode(t, keycode);
    for (int i = 0;; i++)
    {
        if(list[i]==NULL)   break;
        wprintf(L"%ls\n", list[i]);
    }
    
    // for (int i = 0; i < 27; i++)
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
            // long wall = bench(n, loop, 0);
            // if (wall < min)
            //     min = wall;
        }
        printf("%d %0.2f ns\n", n, (double)min / loop);
        // printf("%d %d\n", n, RECURSIVE_MAX);
        RECURSIVE_MAX = 0;
    }
}