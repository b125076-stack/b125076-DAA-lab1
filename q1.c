#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    const char X[] = "ABCBDAB";
    const char Y[] = "BDCABA";
    int m = (int)strlen(X);
    int n = (int)strlen(Y);

    int *dp = calloc((size_t)(m + 1) * (n + 1), sizeof *dp);
    if (dp == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    #define DP(i, j) dp[(size_t)(i) * (n + 1) + (j)]

    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (X[i - 1] == Y[j - 1])
                DP(i, j) = DP(i - 1, j - 1) + 1;
            else
                DP(i, j) = DP(i - 1, j) > DP(i, j - 1)
                         ? DP(i - 1, j) : DP(i, j - 1);
        }
    }

    int length = DP(m, n);
    char *lcs = malloc((size_t)length + 1);
    if (lcs == NULL) {
        free(dp);
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    lcs[length] = '\0';
    int i = m, j = n, k = length;

    while (i > 0 && j > 0) {
        if (X[i - 1] == Y[j - 1]) {
            lcs[--k] = X[i - 1];
            --i;
            --j;
        } else if (DP(i - 1, j) >= DP(i, j - 1)) {
            --i;
        } else {
            --j;
        }
    }

    printf("LCS length: %d\n", length);
    printf("LCS: %s\n", lcs);

    free(lcs);
    free(dp);
    return 0;
}