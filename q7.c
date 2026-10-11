#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ASCII_SIZE 256
#define MAX_INPUT 1000000

typedef struct {
    unsigned char ch;
    size_t count;
} Item;

typedef struct {
    Item item;
    size_t release_at;
} Held;

static Item heap[ASCII_SIZE];
static size_t heap_size;

static int higher_priority(Item a, Item b) {
    if (a.count != b.count)
        return a.count > b.count;
    return a.ch < b.ch; }

static void heap_push(Item value) {
    size_t i = heap_size++;
    while (i > 0) {
        size_t parent = (i - 1) / 2;
        if (!higher_priority(value, heap[parent])) break;
        heap[i] = heap[parent];
        i = parent;
    }
    heap[i] = value;
}

static Item heap_pop(void) {
    Item result = heap[0];
    Item last = heap[--heap_size];

    if (heap_size > 0) {
        size_t i = 0;
        while (2 * i + 1 < heap_size) {
            size_t child = 2 * i + 1;
            if (child + 1 < heap_size &&
                higher_priority(heap[child + 1], heap[child]))
                child++;
            if (!higher_priority(heap[child], last)) break;
            heap[i] = heap[child];
            i = child;
        }
        heap[i] = last;
    }
    return result;
}

int main(void) {
    char input[MAX_INPUT + 1];
    size_t k;

    if (scanf("%1000000s", input) != 1 || scanf("%zu", &k) != 1) {
        fprintf(stderr, "Expected a string followed by K.\n");
        return 1;
    }

    size_t length = strlen(input);
    size_t frequency[ASCII_SIZE] = {0};

    for (size_t i = 0; i < length; i++)
        frequency[(unsigned char)input[i]]++;

    if (k <= 1) {
        puts(input);
        return 0;
    }

    for (size_t c = 0; c < ASCII_SIZE; c++) {
        if (frequency[c] > 0) {
            Item item = {(unsigned char)c, frequency[c]};
            heap_push(item);
        }
    }

    char *result = malloc(length + 1);
    Held *held = malloc(length * sizeof(*held));
    if (!result || !held) {
        fprintf(stderr, "Memory allocation failed.\n");
        free(result);
        free(held);
        return 1;
    }

    size_t held_head = 0, held_tail = 0;

    for (size_t pos = 0; pos < length; pos++) {
        while (held_head < held_tail &&
               held[held_head].release_at <= pos) {
            heap_push(held[held_head].item);
            held_head++;
        }

        if (heap_size == 0) {
            puts("");
            free(result);
            free(held);
            return 0;
        }

        Item chosen = heap_pop();
        result[pos] = (char)chosen.ch;
        chosen.count--;

        if (chosen.count > 0) {
            held[held_tail].item = chosen;
            held[held_tail].release_at = pos + k;
            held_tail++;
        }
    }

    result[length] = '\0';
    puts(result);

    free(result);
    free(held);
    return 0;
}