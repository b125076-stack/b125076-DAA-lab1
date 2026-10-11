#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int64_t start;
    int64_t end;
} Meeting;

static int compare_meetings(const void *a, const void *b) {
    const Meeting *x = a;
    const Meeting *y = b;

    if (x->start < y->start) return -1;
    if (x->start > y->start) return 1;
    if (x->end < y->end) return -1;
    if (x->end > y->end) return 1;
    return 0;
}

/* Min-heap of room end times. */
static void push(int64_t *heap, size_t *size, int64_t value) {
    size_t i = (*size)++;
    while (i > 0) {
        size_t parent = (i - 1) / 2;
        if (heap[parent] <= value) break;
        heap[i] = heap[parent];
        i = parent;
    }
    heap[i] = value;
}

static int64_t pop_min(int64_t *heap, size_t *size) {
    int64_t result = heap[0];
    int64_t last = heap[--(*size)];

    if (*size > 0) {
        size_t i = 0;
        while (2 * i + 1 < *size) {
            size_t child = 2 * i + 1;
            if (child + 1 < *size && heap[child + 1] < heap[child])
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

    if (scanf("%zu", &n) != 1) {
        fprintf(stderr, "Expected the number of meetings.\n");
        return 1;
    }

    if (n == 0) {
        puts("0");
        return 0;
    }

    Meeting *meetings = malloc(n * sizeof(*meetings));
    int64_t *end_times = malloc(n * sizeof(*end_times));
    if (!meetings || !end_times) {
        fprintf(stderr, "Memory allocation failed.\n");
        free(meetings);
        free(end_times);
        return 1;
    }

    for (size_t i = 0; i < n; i++) {
        if (scanf("%" SCNd64 " %" SCNd64,
                  &meetings[i].start, &meetings[i].end) != 2 ||
            meetings[i].start >= meetings[i].end) {
            fprintf(stderr, "Each meeting must have start < end.\n");
            free(meetings);
            free(end_times);
            return 1;
        }
    }

    qsort(meetings, n, sizeof(*meetings), compare_meetings);

    size_t heap_size = 0;
    size_t rooms_needed = 0;

    for (size_t i = 0; i < n; i++) {
        while (heap_size > 0 && end_times[0] <= meetings[i].start)
            pop_min(end_times, &heap_size);

        if (heap_size == 0)
            rooms_needed++;

        push(end_times, &heap_size, meetings[i].end);
    }

    printf("%zu\n", rooms_needed);

    free(meetings);
    free(end_times);
    return 0;
}