#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(void) {
    int n;

    if (scanf("%d", &n) != 1 || n < 0) {
        fprintf(stderr, "Expected a nonnegative rod length.\n");
        return 1;
    }

    long long *price = malloc(((size_t)n + 1) * sizeof *price);
    long long *revenue = malloc(((size_t)n + 1) * sizeof *revenue);
    int *first_piece = malloc(((size_t)n + 1) * sizeof *first_piece);

    if (!price || !revenue || !first_piece) {
        fprintf(stderr, "Memory allocation failed.\n");
        free(price);
        free(revenue);
        free(first_piece);
        return 1;
    }

    price[0] = 0;
    for (int i = 1; i <= n; ++i) {
        if (scanf("%lld", &price[i]) != 1) {
            fprintf(stderr, "Expected %d prices.\n", n);
            free(price);
            free(revenue);
            free(first_piece);
            return 1;
        }
    }

    revenue[0] = 0;
    first_piece[0] = 0;

    for (int length = 1; length <= n; ++length) {
        revenue[length] = price[length];
        first_piece[length] = length;

        for (int piece = 1; piece < length; ++piece) {
            long long candidate = price[piece] + revenue[length - piece];
            if (candidate > revenue[length]) {
                revenue[length] = candidate;
                first_piece[length] = piece;
            }
        }
    }

    printf("Maximum revenue: %lld\n", revenue[n]);
    printf("Piece lengths:");
    for (int remaining = n; remaining > 0; ) {
        int piece = first_piece[remaining];
        printf(" %d", piece);
        remaining -= piece;
    }
    if (n == 0) printf(" none");
    printf("\n");

    free(price);
    free(revenue);
    free(first_piece);
    return 0;
}