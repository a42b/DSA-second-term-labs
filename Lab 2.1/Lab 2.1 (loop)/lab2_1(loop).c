#include <stdio.h>

double calculateSum(unsigned int n, double x) {
    double sum = 0;
    double item;
    for (unsigned int i = 1; i <= n; i++) {
        if (i == 1) {
            item = 1;
        } else if (i == 2) {
            item = -x/2;
        } else {
            item *= x*(2*i-3)/(2*i);
        }
        sum += item;
    }
    return sum;
}


int main() {
    int n;
    double x;

    printf("Enter x: ");
    scanf("%lf", &x);
    printf("Enter n: ");
    scanf("%d", &n);
    double sum = calculateSum(n, x);
    printf("Sum: %lf\n", sum);
    return 0;
}
