#include <stdio.h>

double calculateSum(unsigned int n, double x, unsigned int i, double item, double s) {
    printf("i%d = %f\t sum = %f\n",i,item, s);
    double res;
    if (i == n){
        res = s;
        printf("hit the base case : \t");
        printf("i%d = %f\t sum = %f\n",i,item, res);
        return res;
    }
    if (i == 1){
        item = -x/2;
        s -= x/2;
        res = calculateSum(n,x,++i,item,s);
        printf("i%d = %f\t sum = %f\n",i,item, res);

    } else {
        double k = x * (2 * (i + 1) - 3) / ((i + 1) * 2);
        item *= k;
        s += item;
        res = calculateSum(n, x, ++i, item, s);
        printf("i%d = %f\t sum = %f\n", i, item, res);
    }
    return res;
}

double calculateSumWrapper(unsigned int n, double x) {
    return calculateSum(n, x, 1, 1, 1);
}

int main() {
    unsigned int n;
    double x;
    printf("Enter x:");
    scanf("%lf", &x);
    printf("Enter n:");
    scanf("%d", &n);
    double sum = calculateSumWrapper(n, x);
    printf("Sum: %lf\n", sum);
    return 0;
}