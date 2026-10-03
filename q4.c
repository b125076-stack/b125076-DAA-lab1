#include <stdio.h>
#include <stdlib.h>

int lis_length(const int a[], int n) {
    if (n <= 0) return 0;

    int *tails = malloc((size_t)n * sizeof *tails);
    if (tails == NULL) return -1;  

    int length = 0;

    for (int i = 0; i < n; ++i) {
        int lo = 0, hi = length;

        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (tails[mid] < a[i])
                lo = mid + 1;
            else
                hi = mid;
        }

        tails[lo] = a[i];
        if (lo == length) ++length;
    }

    free(tails);
    return length;
}

int main(void) {
    int n;

    if (scanf("%d", &n) != 1 || n < 0) {
        fprintf(stderr, "Expected a nonnegative array length.\n");
        return 1;
    }

    int *a = n > 0 ? malloc((size_t)n * sizeof *a) : NULL;
    if (n > 0 && a == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }

    for (int i = 0; i < n; ++i) {
        if (scanf("%d", &a[i]) != 1) {
            fprintf(stderr, "Expected %d integers.\n", n);
            free(a);
            return 1;
        }
    }

    int result = lis_length(a, n);
    free(a);

    if (result < 0) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }

    printf("%d\n", result);
    return 0;
}