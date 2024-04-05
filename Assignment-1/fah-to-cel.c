#include <stdio.h>

int main() {
    double fahrenheit, celsius;

    printf("Enter temperature in Fahrenheit: ");
    scanf("%lf", &fahrenheit);

    // Convert Fahrenheit to Celsius
    celsius = (fahrenheit - 32) * 5 / 9;

    // Display the result
    printf("%.2f Fahrenheit is %.2f Celsius.\n", fahrenheit, celsius);

    return 0;
}
