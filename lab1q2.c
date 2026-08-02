#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    int n, i;
    int fairHeads = 0, biasedHeads = 0;
    double fairProbability, biasedProbability;

    srand(time(NULL));

    printf("Enter number of coin tosses: ");
    scanf("%d", &n);

    for(i = 0; i < n; i++)
    {
        if(rand() % 2 == 0)
            fairHeads++;

        if((rand() % 100) < 70)
            biasedHeads++;
    }

    fairProbability = (double)fairHeads / n;
    biasedProbability = (double)biasedHeads / n;

    printf("\nFair Coin:\n");
    printf("Heads = %d\n", fairHeads);
    printf("Probability of Head = %.4f\n", fairProbability);

    printf("\nBiased Coin:\n");
    printf("Heads = %d\n", biasedHeads);
    printf("Probability of Head = %.4f\n", biasedProbability);

    return 0;
}