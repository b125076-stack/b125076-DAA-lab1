#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    size_t n;

    /* Input: n, followed by n ratings. */
    if (scanf("%zu", &n) != 1 || n == 0) {
        fprintf(stderr, "Enter a positive number of children.\n");
        return 1;
    }

    int64_t *ratings = malloc(n * sizeof(*ratings));
    uint64_t *candies = malloc(n * sizeof(*candies));
    if (!ratings || !candies) {
        fprintf(stderr, "Memory allocation failed.\n");
        free(ratings);
        free(candies);
        return 1;
    }

    for (size_t i = 0; i < n; i++) {
        if (scanf("%" SCNd64, &ratings[i]) != 1) {
            fprintf(stderr, "Invalid rating.\n");
            free(ratings);
            free(candies);
            return 1;
        }
        candies[i] = 1;
    }

    /* Enforce the constraint relative to the left neighbour. */
    for (size_t i = 1; i < n; i++) {
        if (ratings[i] > ratings[i - 1])
            candies[i] = candies[i - 1] + 1;
    }

    /* Enforce the constraint relative to the right neighbour. */
    for (size_t i = n - 1; i > 0; i--) {
        size_t left = i - 1;
        if (ratings[left] > ratings[i] &&
            candies[left] <= candies[i]) {
            candies[left] = candies[i] + 1;
        }
    }

    uint64_t total = 0;
    for (size_t i = 0; i < n; i++) {
        if (UINT64_MAX - total < candies[i]) {
            fprintf(stderr, "Candy total exceeds supported range.\n");
            free(ratings);
            free(candies);
            return 1;
        }
        total += candies[i];
    }

    printf("%" PRIu64 "\n", total);

    free(ratings);
    free(candies);
    return 0;
}