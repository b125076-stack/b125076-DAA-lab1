#include <stdio.h>
#include <stdlib.h> 

unsigned long long count_ways(const int coins[], int n, int V) {
    if (V < 0 || n < 0) return 0;

    unsigned long long *ways =
        calloc((size_t)V + 1, sizeof *ways);
    if (ways == NULL) return 0;

    ways[0] = 1;  

    for (int i = 0; i < n; ++i) {
        if (coins[i] <= 0) {
            free(ways);
            return 0; 
        }

       
        for (int amount = coins[i]; amount <= V; ++amount) {
            ways[amount] += ways[amount - coins[i]];
        }
    }

    unsigned long long result = ways[V];
    free(ways);
    return result;
}

int main(void) {
    int coins[] = {1, 2, 5};
    int V = 5;
    int n = (int)(sizeof coins / sizeof coins[0]);

    printf("Number of combinations: %llu\n", count_ways(coins, n, V));
  
    return 0;
}