#include <stdbool.h>
#include <stdio.h>
#include <math.h>

int main(int argc, char const *argv[])
{
    int val_a;
    int val_b;

    while (true)
    {
        printf("Enter the lower bound A [1 to 5000]: ");

        scanf(" %d", &val_a);

        if (val_a < 1 || val_a > 5000){
            printf("Error! A must be in the range [1 to 5000]\n\n");
            continue;
        }

        printf("Enter the upper bound B [1 to 5000]: ");

        scanf(" %d", &val_b);

        if (val_b < 1 || val_b > 5000){
            printf("Error! B must be in the range [1 to 5000]\n\n");
            continue;
        }

        if (val_a > val_b){
            printf("Error! Make sure A <= B\n\n");
            continue;
        }
        break;
    }
    printf("\n");
    
    int count = 0;
    for (int i = val_a; i <= val_b; i++){
        int num = i;
        int digits[(int)(log10(num))+1];
        int j = 0;

        bool unique = true;
        while (num != 0){

            digits[j] = num%10;
            for (int k = 0; k < j; k++){
                if (digits[k] == digits[j]){
                    unique = false;
                    break;
                }
            }
            if (!unique)
                break;

            num /= 10;
            j++;
        }


        if (unique){
            count++;
            printf("%d\n", i);
        }
        

    }

    printf("%d\n", count);

    return 0;
}
