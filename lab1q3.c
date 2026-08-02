#include <stdio.h>
#include <stdlib.h>
#include <time.h>

long bubbleOptimized(int a[], int n)
{
    long comparisons = 0;
    int i, j, temp, swapped;

    for(i = 0; i < n - 1; i++)
    {
        swapped = 0;

        for(j = 0; j < n - i - 1; j++)
        {
            comparisons++;

            if(a[j] > a[j + 1])
            {
                temp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = temp;
                swapped = 1;
            }
        }

        if(swapped == 0)
            break;
    }

    return comparisons;
}

long bubbleNormal(int a[], int n)
{
    long comparisons = 0;
    int i, j, temp;

    for(i = 0; i < n - 1; i++)
    {
        for(j = 0; j < n - i - 1; j++)
        {
            comparisons++;

            if(a[j] > a[j + 1])
            {
                temp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = temp;
            }
        }
    }

    return comparisons;
}

int main()
{
    int n, i;
    int a[1000], b[1000];
    long optimized, normal;

    srand(time(NULL));

    printf("n\tEarly Termination\tAll Passes\n");

    for(n = 10; n <= 100; n += 10)
    {
        for(i = 0; i < n; i++)
        {
            a[i] = rand() % 1000;
            b[i] = a[i];
        }

        optimized = bubbleOptimized(a, n);
        normal = bubbleNormal(b, n);

        printf("%d\t%ld\t\t\t%ld\n", n, optimized, normal);
    }

    return 0;
}