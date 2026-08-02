#include <stdio.h>

int main()
{
    int n, i;
    int a[100];

    printf("Enter number of elements: ");
    scanf("%d", &n);

    printf("Enter the elements: ");

    for(i = 0; i < n; i++)
    {
        scanf("%d", &a[i]);
    }

    for(i = 0; i < n - 1; i++)
    {
        if(a[i] == 0 && a[i + 1] == 1)
        {
            printf("Transition point is between index %d and %d\n", i, i + 1);
            break;
        }
    }

    return 0;
}