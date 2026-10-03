#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int min_coins(const int coins[], int n, int V) {
    if (V < 0 || n <= 0) return -1;

    int *dp = malloc((size_t)(V + 1) * sizeof *dp);
    if (dp == NULL) return -1;

    dp[0] = 0;
    for (int amount = 1; amount <= V; ++amount)
        dp[amount] = INT_MAX;

    for (int amount = 1; amount <= V; ++amount) {
        for (int i = 0; i < n; ++i) {
            int coin = coins[i];
            if (coin <= 0) {     
                free(dp);
                return -1;
            }
            if (coin <= amount && dp[amount - coin] != INT_MAX &&
                dp[amount - coin] + 1 < dp[amount]) {
                dp[amount] = dp[amount - coin] + 1;
            }
        }
    }

    int result = dp[V] == INT_MAX ? -1 : dp[V];
    free(dp);
    return result;
}

int main(void) {
    int coins[] = {1, 3, 4};
    int V = 6;
    int n = (int)(sizeof coins / sizeof coins[0]);

    int answer = min_coins(coins, n, V);
    printf("Minimum coins for %d: %d\n", V, answer); 
    return 0;
}