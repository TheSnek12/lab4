/*
David Bazler G01582055
CS 262 Lab Section 007
Lab 5
*/
#include <string.h>
#include <stdio.h>
#include <ctype.h>
const int SIZE = 20;
const int WSIZE = 50;

void get_subject(char letter, char* subject);
void get_verb(char* month, char* verb);
void get_adverb(int digit, char* adverb);

int main(int argc, char const *argv[])
{
    printf("Welcome to Lab 5 (A \"Random\" Sentence Generator): \n");
    while (1) {

        char c;
        char m[4];
        int d;

        

        while(1){
            printf("Enter the first letter of your last name[A-Z] ");
            scanf(" %c", &c);
            c = toupper(c);
            if (c < 'A' || c > 'Z'){
                printf("Error! %c is not valid, please enter a valid input\n", c);
                continue;
            }
            break;
        }

        while(1){
            printf("Enter your birth month [JAN|FEB|...|DEC] ");
            scanf(" %3s", &m);
            for (int i = 0; i < 3; i++) 
                m[i] = toupper(m[i]);
            m[3] = '\0';
            
            if (!(
                strcmp(m, "JAN") == 0| 
                strcmp(m, "FEB") == 0| 
                strcmp(m, "MAR") == 0| 
                strcmp(m, "APR") == 0| 
                strcmp(m, "MAY") == 0| 
                strcmp(m, "JUN") == 0| 
                strcmp(m, "JUL") == 0| 
                strcmp(m, "AUG") == 0| 
                strcmp(m, "SEP") == 0| 
                strcmp(m, "OCT") == 0| 
                strcmp(m, "DEC") == 0
                
            )){
                printf("Error! %s is not valid, please enter a valid input\n", m);
                continue;
            }
            break;
        }

        while(1){
            printf("Enter the last digit of your cellphone [0-9] ");
            scanf(" %d", &d);
            if (d < 0 || d > 9){
                printf("Error! %d is not valid, please enter a valid input\n", d);
                continue;
            }
            break;
        }

        printf("\n");

        char subject[SIZE];
        char verb[SIZE];
        char adverb[SIZE];
        char sentence[WSIZE];

        get_subject(c, subject);
        get_verb(m, verb);
        get_adverb(d, adverb);
        strcat(sentence, subject);
        strcat(sentence, " ");
        strcat(sentence, verb);
        strcat(sentence, " ");
        strcat(sentence, adverb);

        printf("The \"random\" sentence is:\n\t%s\n\n", sentence);

        
        printf("Do you want to quit [Y|y]? ");
        scanf(" %c", &c);
        c = toupper(c);

        if (c == 'Y') break;
    }
}

void get_subject(char letter, char *subject) {
    if (letter <= 'E'){
        strcpy(subject, "The bird");
        return;
    }
    if (letter <= 'J'){
        strcpy(subject, "The dragon");
        return;
    }
    if (letter <= 'O'){
        strcpy(subject, "The fish");
        return;
    }
    if (letter <= 'T'){
        strcpy(subject, "The elephant");
        return;
    }
    if (letter <= 'Z'){
        strcpy(subject, "The cow");
        return;
    }
}

void get_verb(char *month, char *verb) {
    if (
            strcmp(month, "JAN") == 0| 
            strcmp(month, "FEB") == 0| 
            strcmp(month, "MAR") == 0
    ){
        strcpy(verb, "pranced");
    }
    if (
            strcmp(month, "APR") == 0| 
            strcmp(month, "MAY") == 0| 
            strcmp(month, "JUN") == 0
    ){
        strcpy(verb, "swam");
    }
        if (
            strcmp(month, "JUL") == 0| 
            strcmp(month, "AUG") == 0| 
            strcmp(month, "SEP") == 0
    ){
        strcpy(verb, "fled");
    }
    if (
            strcmp(month, "OCT") == 0| 
            strcmp(month, "DEC") == 0      
    ){
        strcpy(verb, "dived");
    }
}

void get_adverb(int digit, char *adverb) {
    if (digit <= 1){
        strcpy(adverb, "remarkably");
        return;
    }
    if (digit <= 3){
        strcpy(adverb, "dashingly");
        return;
    }
    if (digit <= 5){
        strcpy(adverb, "impressively");
        return;
    }
    if (digit <= 7){
        strcpy(adverb, "diabolically");
        return;
    }
    if (digit <= 9){
        strcpy(adverb, "outlandishly");
        return;
    }
}
