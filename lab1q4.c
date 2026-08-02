#include <stdio.h>
#include <math.h>

int main()
{
    int n, i;
    long long moves;

    printf("Enter number of discs: ");
    scanf("%d", &n);

    moves = pow(2, n) - 1;

    printf("\nNumber of discs = %d\n", n);
    printf("Total number of moves = %lld\n", moves);

    printf("\nn\tMoves\n");

    for(i = 1; i <= n; i++)
    {
        moves = pow(2, i) - 1;
        printf("%d\t%lld\n", i, moves);
    }

    return 0;
}