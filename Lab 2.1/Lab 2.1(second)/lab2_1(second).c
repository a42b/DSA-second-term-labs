#include <stdio.h>

struct aStructure{
    double item;
    double sum;
};

struct aStructure calculateSum (unsigned int n, double x) {
    struct aStructure result = {0.0, 0.0};
    unsigned int i;
    i = n;
    printf("i%d = %f,\t sum = %f\n", n, result.item, result.sum);
    if (n == 1){
        printf("hit the base case: \t");
        result.item = 1;
    } else if (n == 2) {
        result = calculateSum(--n,x);
        result.item = -x/2;
    } else {
        result = calculateSum(--n,x);
        result.item *= x*(2*i-3)/(2*i);
    }
    result.sum += result.item;
    printf("i%d = %f,\t sum = %f\n", i, result.item, result.sum);
    return result;
}



int main() {
    unsigned int n;
    double x;

    printf("Enter x:");
    scanf("%lf", &x);
    printf("Enter n:");
    scanf("%d", &n);
    struct aStructure summa = calculateSum(n, x);
    printf("Sum: %lf\n", summa.sum);
    return 0;
}
