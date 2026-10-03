#include <stdio.h>
#include <stdlib.h>

long long max_sum_increasing_subsequence(const int a[], int n) {
    long long *dp = malloc((size_t)n * sizeof *dp);
    if (dp == NULL) return -1;

    long long best = 0;

    for (int i = 0; i < n; ++i) {
        dp[i] = a[i];

        for (int j = 0; j < i; ++j) {
            if (a[j] < a[i] && dp[j] + a[i] > dp[i]) {
                dp[i] = dp[j] + a[i];
            }
        }

        if (dp[i] > best) best = dp[i];
    }

    free(dp);
    return best;
}

int main(void) {
    int n;

    if (scanf("%d", &n) != 1 || n <= 0) {
        fprintf(stderr, "Expected a positive array length.\n");
        return 1;
    }

    int *a = malloc((size_t)n * sizeof *a);
    if (a == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }

    for (int i = 0; i < n; ++i) {
        if (scanf("%d", &a[i]) != 1 || a[i] <= 0) {
            fprintf(stderr, "Expected %d positive integers.\n", n);
            free(a);
            return 1;
        }
    }

    long long result = max_sum_increasing_subsequence(a, n);
    free(a);

    if (result < 0) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }

    printf("%lld\n", result);
    return 0;
}