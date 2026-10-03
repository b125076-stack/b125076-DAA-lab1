#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    KEEP, SUBSTITUTE, DELETE, INSERT
} Operation;

static size_t index_of(size_t i, size_t j, size_t cols) {
    return i * cols + j;
}

int main(void) {
    char *a = NULL, *b = NULL;
    size_t cap_a = 0, cap_b = 0;

    if (getline(&a, &cap_a, stdin) < 0 ||
        getline(&b, &cap_b, stdin) < 0) {
        fprintf(stderr, "Expected two input lines.\n");
        free(a);
        free(b);
        return 1;
    }

    a[strcspn(a, "\r\n")] = '\0';
    b[strcspn(b, "\r\n")] = '\0';

    size_t m = strlen(a), n = strlen(b);
    if (m == SIZE_MAX || n == SIZE_MAX) {
        fprintf(stderr, "Input too large.\n");
        free(a);
        free(b);
        return 1;
    }

    size_t rows = m + 1, cols = n + 1;
    if (rows > SIZE_MAX / cols || rows * cols > SIZE_MAX / sizeof(size_t)) {
        fprintf(stderr, "Input too large.\n");
        free(a);
        free(b);
        return 1;
    }

    size_t cells = rows * cols;
    size_t *dp = malloc(cells * sizeof *dp);
    Operation *choice = malloc(cells * sizeof *choice);
    if (!dp || !choice) {
        fprintf(stderr, "Memory allocation failed.\n");
        free(a); free(b); free(dp); free(choice);
        return 1;
    }

    for (size_t i = 0; i <= m; ++i) {
        dp[index_of(i, 0, cols)] = i;
        if (i > 0) choice[index_of(i, 0, cols)] = DELETE;
    }
    for (size_t j = 0; j <= n; ++j) {
        dp[index_of(0, j, cols)] = j;
        if (j > 0) choice[index_of(0, j, cols)] = INSERT;
    }

    for (size_t i = 1; i <= m; ++i) {
        for (size_t j = 1; j <= n; ++j) {
            size_t k = index_of(i, j, cols);
            size_t diagonal = dp[index_of(i - 1, j - 1, cols)]
                            + (a[i - 1] != b[j - 1]);
            size_t deletion = dp[index_of(i - 1, j, cols)] + 1;
            size_t insertion = dp[index_of(i, j - 1, cols)] + 1;

            dp[k] = diagonal;
            choice[k] = a[i - 1] == b[j - 1] ? KEEP : SUBSTITUTE;
            if (deletion < dp[k]) {
                dp[k] = deletion;
                choice[k] = DELETE;
            }
            if (insertion < dp[k]) {
                dp[k] = insertion;
                choice[k] = INSERT;
            }
        }
    }

    Operation *script = malloc((m + n) * sizeof *script);
    if (!script && m + n > 0) {
        fprintf(stderr, "Memory allocation failed.\n");
        free(a); free(b); free(dp); free(choice);
        return 1;
    }

    size_t count = 0, i = m, j = n;
    while (i > 0 || j > 0) {
        Operation op = choice[index_of(i, j, cols)];
        script[count++] = op;
        if (op == KEEP || op == SUBSTITUTE) { --i; --j; }
        else if (op == DELETE) { --i; }
        else { --j; }
    }

    printf("Edit distance: %zu\n", dp[index_of(m, n, cols)]);
    printf("Edit script (positions are zero-based):\n");

    i = 0;
    j = 0;
    for (size_t r = count; r > 0; --r) {
        Operation op = script[r - 1];
        if (op == KEEP) {
            ++i; ++j;
        } else if (op == SUBSTITUTE) {
            printf("Substitute position %zu: '%c' -> '%c'\n", i, a[i], b[j]);
            ++i; ++j;
        } else if (op == DELETE) {
            printf("Delete '%c' at position %zu\n", a[i], i);
            ++i;
        } else {
            printf("Insert '%c' at position %zu\n", b[j], i);
            ++j;
        }
    }

    free(a); free(b); free(dp); free(choice); free(script);
    return 0;
}