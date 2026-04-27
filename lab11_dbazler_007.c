/*
David Bazler G01582055
CS 262 Lab Section 007
Lab 11
*/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main(int argc, char const *argv[])
{
    printf("Enter a float value (up to two decimal places): ");
    float in;
    scanf(" %f", &in);
    printf("User entered value:\t\t\t\t%.10f\n", in);

    unsigned int i;
    //why instead of casting as a pointer to reinterpret the value though?
    memcpy(&i, &in, sizeof(unsigned int));

    unsigned int sign = (i&0x80000000) >> 0x1F;
    unsigned int exp = (i&0x7f800000) >> 0x17;
    unsigned int mant = i&0x007fffff;

    printf("Copied unsigned int:\t\t\t\t0x%08X\n", i);
    printf("Sign:\t\t\t\t\t\t%u\n", sign);
    printf("Exponent:\t\t\t\t\t%u\n", exp);
    printf("Mantissa:\t\t\t\t\t%u\n",mant);
    
    printf("Reconstructed value:\t\t\t\t");
    if (exp == 0){
        printf("%.10f\n", pow(-1,sign) * (mant / pow(2,23)) * pow(2,(1-127)));
        return 0;
    }

    if (exp == 255){
        printf("Infinity or NaN\n");
        return 0;
    }

    printf("%.10f\n", pow(-1,sign) * (1+mant / pow(2,23)) * pow(2,(exp-127)));

    return 0;
}
