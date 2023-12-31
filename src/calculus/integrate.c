#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#include "../utility/utility.h"

double integral(double a, double b);
size_t split_interval(double a, double b);

double f(double x) {
    return cos(x);
}

int main(int argc, char* argv[]) {
    (void) argc;
    (void) argv;

    printf("%f (should be 18)\n", integral(-2, 4));
}

double integral(double a, double b) {
    /* integral from a to a is 0 */
    if (feq(a, b)) {
        return 0;
    }

    size_t num_regions = split_interval(a, b);
    double delta_x = (b - a) / num_regions;
    double x = a;
    double sum = 0;

    for (size_t i = 0; i < num_regions; i++) {
        sum += f(x) * delta_x;
        x += delta_x;
    }

    return sum;
}

size_t split_interval(double a, double b) {
    return 1000000 + labs((long) (b - a));
}
