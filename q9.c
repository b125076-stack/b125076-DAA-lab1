#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static size_t n;
static uint64_t *weight;
static uint64_t *prefix;
static uint64_t *cost;
static size_t *split_at;

#define CELL(i, j) ((i) * n + (j))

static void print_tree(size_t left, size_t right) {
    if (left == right) {
        printf("%zu", left + 1); /* one-based leaf index */
        return;
    }

    size_t k = split_at[CELL(left, right)];
    putchar('(');
    print_tree(left, k);
    putchar(',');
    print_tree(k + 1, right);
    putchar(')');
}

int main(void) {
    if (scanf("%zu", &n) != 1 || n == 0) {
        fprintf(stderr, "Enter a positive number of weights.\n");
        return 1;
    }

    if (n > SIZE_MAX / n) {
        fprintf(stderr, "Input size is too large.\n");
        return 1;
    }

    weight = malloc(n * sizeof(*weight));
    prefix = calloc(n + 1, sizeof(*prefix));
    cost = calloc(n * n, sizeof(*cost));
    split_at = malloc(n * n * sizeof(*split_at));

    if (!weight || !prefix || !cost || !split_at) {
        fprintf(stderr, "Memory allocation failed.\n");
        free(weight);
        free(prefix);
        free(cost);
        free(split_at);
        return 1;
    }

    for (size_t i = 0; i < n; i++) {
        if (scanf("%" SCNu64, &weight[i]) != 1 || weight[i] == 0) {
            fprintf(stderr, "Weights must be positive integers.\n");
            free(weight);
            free(prefix);
            free(cost);
            free(split_at);
            return 1;
        }
        if (UINT64_MAX - prefix[i] < weight[i]) {
            fprintf(stderr, "Sum of weights exceeds supported range.\n");
            free(weight);
            free(prefix);
            free(cost);
            free(split_at);
            return 1;
        }
        prefix[i + 1] = prefix[i] + weight[i];
    }

    /* Intervals of length 1 are leaves and have cost 0. */
    for (size_t length = 2; length <= n; length++) {
        for (size_t left = 0; left + length <= n; left++) {
            size_t right = left + length - 1;
            uint64_t interval_sum = prefix[right + 1] - prefix[left];
            uint64_t best = UINT64_MAX;
            size_t best_split = left;

            for (size_t k = left; k < right; k++) {
                uint64_t a = cost[CELL(left, k)];
                uint64_t b = cost[CELL(k + 1, right)];

                if (UINT64_MAX - a < b || UINT64_MAX - (a + b) < interval_sum) {
                    fprintf(stderr, "Tree cost exceeds supported range.\n");
                    free(weight);
                    free(prefix);
                    free(cost);
                    free(split_at);
                    return 1;
                }

                uint64_t candidate = a + b + interval_sum;
                if (candidate < best) {
                    best = candidate;
                    best_split = k;
                }
            }

            cost[CELL(left, right)] = best;
            split_at[CELL(left, right)] = best_split;
        }
    }

    printf("Minimum weighted path length: %" PRIu64 "\n", cost[CELL(0, n - 1)]);
    printf("Alphabetic tree (leaves are indices 1..%zu): ", n);
    print_tree(0, n - 1);
    putchar('\n');

    free(weight);
    free(prefix);
    free(cost);
    free(split_at);
    return 0;
}