#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SYMBOL 127

typedef struct {
    uint64_t weight;
    int left, right;       
    char *symbol;            
    const char *min_symbol;  
} Node;

typedef struct {
    int node;
    size_t length;
} Entry;

static Node *nodes;
static int *heap;
static size_t heap_size, next_serial;

static int less_node(int a, int b) {
    if (nodes[a].weight != nodes[b].weight)
        return nodes[a].weight < nodes[b].weight;

    int cmp = strcmp(nodes[a].min_symbol, nodes[b].min_symbol);
    if (cmp != 0) return cmp < 0;
    return nodes[a].serial < nodes[b].serial;
}

static void heap_push(int x) {
    size_t i = heap_size++;
    while (i > 0) {
        size_t p = (i - 1) / 2;
        if (!less_node(x, heap[p])) break;
        heap[i] = heap[p];
        i = p;
    }
    heap[i] = x;
}

static int heap_pop(void) {
    int result = heap[0];
    int last = heap[--heap_size];

    if (heap_size > 0) {
        size_t i = 0;
        while (2 * i + 1 < heap_size) {
            size_t child = 2 * i + 1;
            if (child + 1 < heap_size &&
                less_node(heap[child + 1], heap[child]))
                child++;
            if (!less_node(heap[child], last)) break;
            heap[i] = heap[child];
            i = child;
        }
        heap[i] = last;
    }
    return result;
}

static void get_lengths(int node, size_t depth, Entry *entries,
                        size_t *count) {
    if (nodes[node].left == -1) {
        entries[*count].node = node;
        
        entries[*count].length = depth ? depth : 1;
        (*count)++;
        return;
    }
    get_lengths(nodes[node].left, depth + 1, entries, count);
    get_lengths(nodes[node].right, depth + 1, entries, count);
}

static int compare_entries(const void *a, const void *b) {
    const Entry *x = a, *y = b;
    if (x->length != y->length)
        return x->length < y->length ? -1 : 1;
    return strcmp(nodes[x->node].symbol, nodes[y->node].symbol);
}

int main(void) {
    size_t n;
    if (scanf("%zu", &n) != 1 || n == 0) {
        fprintf(stderr, "Enter a positive number of symbols.\n");
        return 1;
    }

    nodes = calloc(2 * n - 1, sizeof(*nodes));
    heap = malloc((2 * n - 1) * sizeof(*heap));
    Entry *entries = malloc(n * sizeof(*entries));
    if (!nodes || !heap || !entries) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }

    for (size_t i = 0; i < n; i++) {
        char buf[MAX_SYMBOL + 1];
        uint64_t frequency;

        if (scanf("%127s %" SCNu64, buf, &frequency) != 2 ||
            frequency == 0) {
            fprintf(stderr, "Each row must contain a symbol and positive frequency.\n");
            return 1;
        }

        for (size_t j = 0; j < i; j++) {
            if (strcmp(buf, nodes[j].symbol) == 0) {
                fprintf(stderr, "Symbols must be unique.\n");
                return 1;
            }
        }

        nodes[i].symbol = malloc(strlen(buf) + 1);
        if (!nodes[i].symbol) {
            fprintf(stderr, "Memory allocation failed.\n");
            return 1;
        }
        strcpy(nodes[i].symbol, buf);

        nodes[i].weight = frequency;
        nodes[i].left = nodes[i].right = -1;
        nodes[i].min_symbol = nodes[i].symbol;
        nodes[i].serial = next_serial++;
        heap_push((int)i);
    }

    size_t node_count = n;
    while (heap_size > 1) {
        int a = heap_pop(), b = heap_pop();

        if (UINT64_MAX - nodes[a].weight < nodes[b].weight) {
            fprintf(stderr, "Frequency sum exceeds the supported range.\n");
            return 1;
        }

        int parent = (int)node_count++;
        nodes[parent].weight = nodes[a].weight + nodes[b].weight;
        nodes[parent].left = a;
        nodes[parent].right = b;
        nodes[parent].min_symbol =
            strcmp(nodes[a].min_symbol, nodes[b].min_symbol) < 0
                ? nodes[a].min_symbol : nodes[b].min_symbol;
        nodes[parent].serial = next_serial++;
        heap_push(parent);
    }

    size_t count = 0;
    get_lengths(heap_pop(), 0, entries, &count);
    qsort(entries, count, sizeof(*entries), compare_entries);

    char *code = calloc(n + 1, 1);
    if (!code) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }

    size_t prev_len = entries[0].length;
    printf("Symbol\tFrequency\tLength\tCanonical code\n");
    for (size_t i = 0; i < count; i++) {
        size_t len = entries[i].length;

        if (i > 0) {
            size_t k = prev_len;
            while (k > 0 && code[k - 1] == '1') {
                code[k - 1] = '0';
                k--;
            }
            if (k == 0) {
                fprintf(stderr, "Invalid canonical length sequence.\n");
                return 1;
            }
            code[k - 1] = '1';
            for (size_t j = prev_len; j < len; j++)
                code[j] = '0';
        }

        code[len] = '\0';
        int node = entries[i].node;
        printf("%s\t%" PRIu64 "\t%zu\t%s\n",
               nodes[node].symbol, nodes[node].weight, len, code);
        prev_len = len;
    }

    for (size_t i = 0; i < n; i++) free(nodes[i].symbol);
    free(code);
    free(entries);
    free(heap);
    free(nodes);
    return 0;
}