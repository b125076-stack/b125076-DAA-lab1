#include <stdio.h>

int main()
{
    int n, i, j, duplicate = 0;
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
        for(j = i + 1; j < n; j++)
        {
            if(a[i] == a[j])
            {
                duplicate = 1;
                break;
            }
        }

        if(duplicate)
            break;
    }

    if(duplicate)
        printf("Duplicate elements are present.\n");
    else
        printf("All elements are unique.\n");

    return 0;
}