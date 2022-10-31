#include <stdlib.h>
#include <stdio.h>



double very_important_function(size_t N, const float *A, const float *B, float *R);


int main() {
    float* a = aligned_alloc(32, 32000 * sizeof(float));
    float* b = aligned_alloc(32, 32000 * sizeof(float));
    float* r = aligned_alloc(32, 32000 * sizeof(float));

    for (int i = 0; i < 32000; ++i) {
        a[i] = i;
        b[i] = i + 1;
    }

    double x = very_important_function(32000, a, b, r);
    printf("%lf", x);

    free(a);
    free(b);
    free(r);
}