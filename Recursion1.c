#include<stdio.h>
#include<string.h>

int fact(int n){
    if (n == 0 || n == 1)  // Base case
        return 1;
    int factnm1 = fact(n-1);
    int factN = factnm1 * n;
    return factN;
}

int main() {
    int n;
    printf("Enter a number to get it's factorial: ");
    scanf("%d", &n);
    int factN = fact(n);
    printf("%d is the factorial", factN);
    return 0;
}