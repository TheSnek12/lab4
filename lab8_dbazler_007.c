/*
David Bazler G01582055
CS 262 Lab Section 007
Lab 8
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

const unsigned int MAX_SIZE = 256;
const unsigned int ROWS = 9;

int main(int argc, char const *argv[])
{
    if (argc != 3){
        printf("Error Msg!\n");
        exit(1);

    }

    FILE* input_file = fopen(argv[1], "r");

    char* data[ROWS];

    for (int i = 0; i < ROWS; i++){
        data[i] = malloc(MAX_SIZE);
        char line[MAX_SIZE];
        fgets(line, MAX_SIZE, input_file);
    
        char thing[MAX_SIZE];
        //i dont even know
        sscanf(line, "%*[a-zA-Z ]\t|\t%[a-zA-Z0-9/.- ]", thing);
        memcpy(data[i], thing, MAX_SIZE);
    }

    FILE* output_file = fopen(argv[2], "w");
    fprintf(output_file, "Customer: \t%s\n", data[0]);
    fprintf(output_file, "Account: \t%s\n", data[1]);
    fprintf(output_file, "Reporting Date: \t%s\n", data[2]);
    //probably should be done during compile time but i dont want to and it works 
    fprintf(output_file, "%-10s%-15s%-15s", "Stock", "Open Price", "Close Price");
    float diff = (atof(data[8])-atof(data[5]))*atoi(data[4]);
    fprintf(output_file, "%s\n", diff > 0 ? "Gain" : diff < 0 ? "Loss" : "Unchanged" );

    fprintf(output_file, "%-10s%-15s%-15s%.2f", data[3], data[5], data[8], fabs(diff));


    return 0;
}
