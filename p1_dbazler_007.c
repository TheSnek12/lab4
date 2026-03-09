/*
David Bazler G01582055
CS 262 Lab Section 007
Project 1
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

const int MAX_NAME_LEN = 25;
const int SEQ_LEN = 18;

//represents information intrinsic to the game at runtime, including a quit state
struct game_state
{
    char* user_name;
    int user_score;
    int computer_score;

    int round;

    int quitting;
};

void next_round(struct game_state* state);
void print_standing(struct game_state* state);
void quit_game(struct game_state* state);
void gen_seq(int *arr, unsigned int len);
int find_primes(int *seq, int seq_len, int *arr,  unsigned int step);
int is_prime(int number);


int main() {
    char name[MAX_NAME_LEN];
    
    srand(time(NULL));

    //inital game state with nothing yet
    struct game_state state = {
        NULL,
        0,
        0,
        0,
        0
    };

    printf("*******************************\n*     Prime of Life game      *\n*******************************\n\n");

    while(1){
        int valid = 1;
        printf("Please enter your name:\n");
        scanf(" %s", name);
        for (int i = 0; i < strlen(name); i++){
            if((name[i] < 'A' || name[i] > 'z') && name[i] != ' '){
                printf("Invalid symbol: %c\n", name[i]);
                valid = 0;
            }
        }
        if (valid)
            break;
    }
    state.user_name = name;


    printf("Welcome %s\n", name);

    while(!state.quitting){

        printf("Enter your choice:\n1: Generate your prime sequence and Continue\n2: Check your game score so far against the computer game score\n3: End Game\n");

        int choice;
        scanf(" %d", &choice);

        //move to pass-by-reference functions, since game functions influence game state, could be a pass-by-copy but 1 pointer is cheaper than a string and 5 integers
        switch (choice)
        {
        case 1:
            next_round(&state);
            break;
        case 2:
            print_standing(&state);
            break;
        case 3:
            quit_game(&state);
            break;
        
        default:
            printf("Invalid Choice\n");
            break;
        }
    
    }

}

void next_round(struct game_state* state){
    printf("Round %d\n", ++state->round);

    int usr_seq[SEQ_LEN];
    int comp_seq[SEQ_LEN];
    

    gen_seq(usr_seq, SEQ_LEN);
    printf("%s sequence is: ", state->user_name);
    for (int i = 0; i < SEQ_LEN; i++)
        printf("%d", usr_seq[i]);
    printf("\n");

    gen_seq(comp_seq, SEQ_LEN);
    printf("Computer sequence is: ");
    for (int i = 0; i < SEQ_LEN; i++)
        printf("%d", comp_seq[i]);
    printf("\n");

    int round_score;
    int prime_score;
    int primes[SEQ_LEN];

    prime_score = find_primes(usr_seq, SEQ_LEN, primes, 1);
    printf ("%s single digit primes are: ", state->user_name);
    for (int i = 0; i < prime_score; i++)
        printf("%d ", primes[i]);
    printf("\n");
    printf("%s single-digit prime score is: %d\n", state->user_name, prime_score);
    round_score = prime_score;


    prime_score = find_primes(usr_seq, SEQ_LEN, primes, 2);
    printf ("%s double digit primes are: ", state->user_name, state->user_name);
    for (int i = 0; i < prime_score; i++)
        printf("%d ", primes[i]);
    printf("\n");
    printf("%s double-digit prime score is: %d\n", state->user_name, prime_score*2);
    round_score += prime_score*2;

    prime_score = find_primes(usr_seq, SEQ_LEN, primes, 3);
    printf ("%s triple digit primes are: ", state->user_name);
    for (int i = 0; i < prime_score; i++)
        printf("%d ", primes[i]);
    printf("\n");
    printf("%s triple-digit prime score is: %d\n", state->user_name, prime_score*3);
    round_score += prime_score*3;

    printf("%s total round score is: %d\n", state->user_name, round_score);
    state->user_score += round_score;
    printf("%s total game score is: %d\n\n", state->user_name, state->user_score);


    prime_score = find_primes(comp_seq, SEQ_LEN, primes, 1);
    printf ("Computer single digit primes are: ");
    for (int i = 0; i < prime_score; i++)
        printf("%d ", primes[i]);
    printf("\n");
    printf("Computer single-digit prime score is: %d\n", prime_score);
    round_score = prime_score;


    prime_score = find_primes(comp_seq, SEQ_LEN, primes, 2);
    printf ("Computer double digit primes are: ", state->user_name);
    for (int i = 0; i < prime_score; i++)
        printf("%d ", primes[i]);
    printf("\n");
    printf("Computer double-digit prime score is: %d\n", prime_score*2);
    round_score += prime_score*2;

    prime_score = find_primes(comp_seq, SEQ_LEN, primes, 3);
    printf ("Computer triple digit primes are: ");
    for (int i = 0; i < prime_score; i++)
        printf("%d ", primes[i]);
    printf("\n");
    printf("Computer triple-digit prime score is: %d\n", prime_score*3);
    round_score += prime_score*3;

    printf("Computer total round score is: %d\n", round_score);
    state->computer_score += round_score;
    printf("Computer total game score is: %d\n\n", state->computer_score);

    if (state->round == 10){
        printf("GAME OVER\n");
        printf("%s game score: %d\n", state->user_name, state->user_score);
        printf("Computer game score: %d\n", state->computer_score);
        if(state->computer_score > state->user_score){
            printf("HARD LUCK. COMPUTER WINS");
        } else {
            printf("CONGRATULATIONS YOU WON CHAMP");
        }
        state->quitting = 1;
    }
}

//fill int array with random integers from 1-9 inclusive
void gen_seq(int *arr, unsigned int len){
    for (int i = 0; i < len; i++)
        arr[i] = rand() % 9 +1;
}

//generalization of finding n-size primes in a sequence of integers, dont ask
int find_primes(int *seq, int seq_len, int *arr,  unsigned int step){
    if (step < 1) return 0;

    int next = 0;

    for (int i = 0; i + step-1 < seq_len; i+= step){
        int num = 0;
        for (int j = 0; j < step; j++){

            int pow = 1;
            for (int k = 0; k < step-j-1; k++) pow *= 10;

            num += seq[i+j]*pow;
        }

        if (is_prime(num)){
            arr[next] = num;
            next++;
        }

    }
    return next;
}

void print_standing(struct game_state* state){
    printf("%s game score so far: %d\n", state->user_name, state->user_score);
    printf("Computer game score so far: %d\n", state->computer_score);
}

void quit_game(struct game_state* state){
    state->quitting = 1;
    printf("Game Ended by %s\n", state->user_name);
    printf("%s game score: %d\n", state->user_name, state->user_score);
    printf("Computer game score: %d\n", state->computer_score);

}

int is_prime(int number) {
    int prime = 1;
    if (number == 1)
    {
        return 0;
    }
    for (int i = 2; i <= number / 2; i++)
    {
        if (number % i == 0)
        {
            prime = 0;
            break;
        }
    }
    return prime;
}