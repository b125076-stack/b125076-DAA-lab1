#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static uint64_t *heap;
static size_t heap_size;

static void push(uint64_t value) {
    size_t i = heap_size++;
    while (i > 0) {
        size_t parent = (i - 1) / 2;
        if (heap[parent] <= value) break;
        heap[i] = heap[parent];
        i = parent;
    }
    heap[i] = value;
}

static uint64_t pop_min(void) {
    uint64_t result = heap[0];
    uint64_t last = heap[--heap_size];

    if (heap_size > 0) {
        size_t i = 0;
        while (2 * i + 1 < heap_size) {
            size_t child = 2 * i + 1;
            if (child + 1 < heap_size && heap[child + 1] < heap[child])
                child++;
            if (heap[child] >= last) break;
            heap[i] = heap[child];
            i = child;
        }
        heap[i] = last;
    }
    return result;
}

int main(void) {
    size_t n;

    if (scanf("%zu", &n) != 1 || n == 0) {
        fprintf(stderr, "Enter a positive number of sticks.\n");
        return 1;
    }

    heap = malloc(n * sizeof(*heap));
    if (!heap) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }

    for (size_t i = 0; i < n; i++) {
        uint64_t length;
        if (scanf("%" SCNu64, &length) != 1) {
            fprintf(stderr, "Invalid stick length.\n");
            free(heap);
            return 1;
        }
        push(length);
    }

    uint64_t total_cost = 0;

    while (heap_size > 1) {
        uint64_t x = pop_min();
        uint64_t y = pop_min();

        if (UINT64_MAX - x < y) {
            fprintf(stderr, "Combined stick length exceeds supported range.\n");
            free(heap);
            return 1;
        }
        uint64_t combined = x + y;

        if (UINT64_MAX - total_cost < combined) {
            fprintf(stderr, "Total cost exceeds supported range.\n");
            free(heap);
            return 1;
        }
        total_cost += combined;
        push(combined);
    }

    printf("%" PRIu64 "\n", total_cost);
    free(heap);
    return 0;
}