#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    uint64_t distance;
    uint64_t fuel;
} Station;


static void push(uint64_t *heap, size_t *size, uint64_t value) {
    size_t i = (*size)++;
    while (i > 0) {
        size_t parent = (i - 1) / 2;
        if (heap[parent] >= value) break;
        heap[i] = heap[parent];
        i = parent;
    }
    heap[i] = value;
}

static uint64_t pop_max(uint64_t *heap, size_t *size) {
    uint64_t result = heap[0];
    uint64_t last = heap[--(*size)];

    if (*size > 0) {
        size_t i = 0;
        while (2 * i + 1 < *size) {
            size_t child = 2 * i + 1;
            if (child + 1 < *size && heap[child + 1] > heap[child])
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
    uint64_t D, F;

    if (scanf("%" SCNu64 " %" SCNu64 " %zu", &D, &F, &n) != 3) {
        fprintf(stderr, "Expected: D F n, followed by n station rows.\n");
        return 1;
    }

    Station *stations = malloc((n + 1) * sizeof(*stations));
    uint64_t *heap = malloc((n + 1) * sizeof(*heap));
    if (!stations || !heap) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }

    for (size_t i = 0; i < n; i++) {
        if (scanf("%" SCNu64 " %" SCNu64,
                  &stations[i].distance, &stations[i].fuel) != 2) {
            fprintf(stderr, "Invalid station row.\n");
            return 1;
        }
        if (stations[i].distance > D ||
            (i > 0 && stations[i].distance < stations[i - 1].distance)) {
            fprintf(stderr, "Stations must be ordered by distance within [0, D].\n");
            return 1;
        }
    }

    stations[n].distance = D;
    stations[n].fuel = 0;

    uint64_t reach = F; 
    size_t heap_size = 0, stops = 0;

    for (size_t k = n + 1; k > 0; k--) {
        Station point = stations[k - 1];

        while (reach < point.distance) {
            if (heap_size == 0) {
                printf("-1\n"); 
                free(heap);
                free(stations);
                return 0;
            }

            uint64_t added_fuel = pop_max(heap, &heap_size);
            if (UINT64_MAX - reach < added_fuel) {
                fprintf(stderr, "Reach exceeds supported numeric range.\n");
                free(heap);
                free(stations);
                return 1;
            }
            reach += added_fuel;
            stops++;
        }

        if (k > 1)
            push(heap, &heap_size, point.fuel);
    }

    printf("%zu\n", stops);
    free(heap);
    free(stations);
    return 0;
}