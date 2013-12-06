#include <stdio.h>

int main() {
    double n, sum;
    int i, j;

    // Harmonic series is a fine way to burn CPU
    sum = n = 0.0;
    j = 0;
    while(1){
        for (i=0; i<10000000; i++) {
            n += 1.0;
            sum += 1.0 / n;
        }
        j++;
        printf("Sum 1/n for in in [1..%d0000000] = %f\r\n", j, sum);
    }
}
