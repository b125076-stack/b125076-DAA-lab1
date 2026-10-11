#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define EPS 1e-12

typedef struct {
    int id;
    double v;
    double w;
    double lambda;
    double fraction;
    double start_time;
    double contribution;
} Item;

int compare_lambda(const void *a, const void *b)
{
    const Item *x = (const Item *)a;
    const Item *y = (const Item *)b;

    if (x->lambda < y->lambda) return -1;
    if (x->lambda > y->lambda) return 1;

    if (x->v / x->w > y->v / y->w) return -1;
    if (x->v / x->w < y->v / y->w) return 1;

    return x->id - y->id;
}

int main(void)
{
    int n;
    double W;

    if (scanf("%d %lf", &n, &W) != 2 || n <= 0 || W < 0.0) {
        fprintf(stderr, "Invalid input.\n");
        return EXIT_FAILURE;
    }

    Item *items = malloc((size_t)n * sizeof(Item));
    if (items == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < n; ++i) {
        items[i].id = i + 1;
        items[i].fraction = 0.0;
        items[i].start_time = 0.0;
        items[i].contribution = 0.0;

        if (scanf("%lf %lf %lf",
                  &items[i].v,
                  &items[i].w,
                  &items[i].lambda) != 3 ||
            items[i].v < 0.0 ||
            items[i].w <= 0.0 ||
            items[i].lambda <= 0.0) {
            fprintf(stderr, "Invalid item %d.\n", i + 1);
            free(items);
            return EXIT_FAILURE;
        }
    }

    qsort(items, (size_t)n, sizeof(Item), compare_lambda);

    double remaining = W;
    double time = 0.0;
    double total_value = 0.0;

    for (int i = 0; i < n && remaining > EPS; ++i) {
        double amount = items[i].w;

        if (amount > remaining)
            amount = remaining;

        items[i].fraction = amount / items[i].w;
        items[i].start_time = time;

        
        double density =
            items[i].v / items[i].w -
            items[i].lambda * items[i].start_time;

        items[i].contribution = amount * density;

        
        if (density <= 0.0) {
            items[i].fraction = 0.0;
            items[i].contribution = 0.0;
            break;
        }

        total_value += items[i].contribution;
        remaining -= amount;
        time += amount;
    }

    printf("Optimal total value: %.10f\n", total_value);
    printf("Selected schedule:\n");

    for (int i = 0; i < n; ++i) {
        if (items[i].fraction > EPS) {
            printf("Item %d: fraction = %.10f, start = %.10f, "
                   "contribution = %.10f\n",
                   items[i].id,
                   items[i].fraction,
                   items[i].start_time,
                   items[i].contribution);
        }
    }

    free(items);
    return EXIT_SUCCESS;
}
