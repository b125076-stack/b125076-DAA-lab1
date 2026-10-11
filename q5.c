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
        if (heap[parent] >= value) break;
        heap[i] = heap[parent];
        i = parent;
    }
    heap[i] = value;
}

static uint64_t pop_max(void) {
    uint64_t result = heap[0];
    uint64_t last = heap[--heap_size];

    if (heap_size > 0) {
        size_t i = 0;
        while (2 * i + 1 < heap_size) {
            size_t child = 2 * i + 1;
            if (child + 1 < heap_size && heap[child + 1] > heap[child])
                child++;
            if (heap[child] <= last) break;
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
        fprintf(stderr, "Enter a positive array size.\n");
        return 1;
    }

    heap = malloc(n * sizeof(*heap));
    if (!heap) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }

    uint64_t minimum = UINT64_MAX;

    for (size_t i = 0; i < n; i++) {
        uint64_t value;
        if (scanf("%" SCNu64, &value) != 1 || value == 0) {
            fprintf(stderr, "Enter positive integer values.\n");
            free(heap);
            return 1;
        }

        if (value & 1) {
            if (value > UINT64_MAX / 2) {
                fprintf(stderr, "Doubling an odd value exceeds supported range.\n");
                free(heap);
                return 1;
            }
            value *= 2;
        }

        if (value < minimum) minimum = value;
        push(value);
    }

    uint64_t best = UINT64_MAX;

    while (1) {
        uint64_t maximum = pop_max();
        uint64_t deviation = maximum - minimum;
        if (deviation < best) best = deviation;

        /* An odd maximum cannot be reduced further. */
        if (maximum & 1)
            break;

        uint64_t reduced = maximum / 2;
        if (reduced < minimum) minimum = reduced;
        push(reduced);
    }

    printf("%" PRIu64 "\n", best);
    free(heap);
    return 0;
}