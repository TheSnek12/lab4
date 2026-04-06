/*
David Bazler G01582055
CS 262 Lab Section 007
Project 2
*/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

enum Difficulty{
    EASY = 5,
    MEDIUM = 20,
    HARD = 35,
    NONE = 0
};

int get_bomb_percent(char *level);
int get_num_bomb(int rows, int cols, int bomb_percent);
char **setup(int rows, int cols);
void init(char **gboard, int rows, int cols, int num_bomb);
void print_board(char **board, int rows, int cols);
void walker(char **pboard, char **gboard, int rows, int cols, int click_r, int click_c);
void play(char **pboard, char **gboard, int rows, int cols, int num_bomb);
void free_broad(char **board, int rows, int cols);

int main(int argc, char const *argv[])
{
    if (argc != 5){
        printf("Usage: %s <rows> <cols> <”easy” | “medium” | “hard”> <seed>\n", argv[0]);
        exit(1);
    }
    int rows = atoi(argv[1]);
    int cols = atoi(argv[2]);
    
    char* input_difficulty = malloc(strlen(argv[3])+1);
    memcpy(input_difficulty, argv[3], strlen(argv[3])+1);
    enum Difficulty difficulty = get_bomb_percent(input_difficulty);
    free(input_difficulty);

    if (rows < 0 || cols < 0 || difficulty == NONE){
        printf("invalid inputs\n");
        exit(1);
    }

    srand(atoi(argv[4]));
    
    int total_bombs = get_num_bomb(rows, cols, difficulty);
    
    char** game_board = setup(rows, cols);
    char** user_board = setup(rows, cols);

    init(game_board, rows, cols, total_bombs);
    printf("This is the game board after initialization\n");
    print_board(game_board, rows, cols);
    printf("This is the play board after setup\n");
    print_board(user_board, rows, cols);
    printf("The game will start\n");

    while(1)
        play(user_board, game_board, rows, cols, total_bombs);

}
int get_bomb_percent(char *level){
    enum Difficulty difficulty = NONE;

    char* input_diff;
    input_diff = malloc(strlen(level)+1);
    for (int i = 0; i < strlen(level); i++) input_diff[i] = tolower(level[i]);
    input_diff[strlen(level)] = '\0';
    if (strcmp(input_diff, "easy") == 0){
        difficulty = EASY;
    }
    if (strcmp(input_diff, "medium") == 0){
        difficulty = MEDIUM;
    }
    if (strcmp(input_diff, "hard") == 0){
        difficulty = HARD;
    }

    free(input_diff);
    return difficulty;
}
int get_num_bomb(int rows, int cols, int bomb_percent){
    return (int)(bomb_percent*0.01*rows*cols);
}

char **setup(int rows, int cols){
    char** board = malloc(cols*sizeof(char*));
    for (int i = 0; i < cols; i++){
        board[i] = malloc(rows);
    }

    for (int x = 0; x < cols; x++){
        for (int y = 0; y < rows; y++){
            board[x][y] = '.';
        }
    }

    return board;
}

void init(char **gboard, int rows, int cols, int num_bomb){
    for (int i = 0; i < num_bomb; i++){
        int x = rand()%cols;
        int y = rand()%rows;
        if (gboard[x][y] == '*'){
            i--;
            continue;
        }
        gboard[x][y] = '*';
    }

    for (int y = 0; y < rows; y++){
        for (int x = 0; x < cols; x++){
            if ((gboard[x][y]) == '*') continue;

            int adjacent_bombs = 0;  
            for (int p_x = -1; p_x < 2; p_x++){
                for (int p_y = -1; p_y < 2; p_y++){
                    if (p_x+x >= cols || p_x+x < 0) continue;
                    if (p_y+y >= rows || p_y+y < 0) continue;
                    if (gboard[p_x+x][p_y+y] == '*') adjacent_bombs++;

                }
            }

            gboard[x][y] = adjacent_bombs+48;            
        }
    }

}

void print_board(char **board, int rows, int cols){
    for (int y = 0; y < rows; y++){
        for (int x = 0; x < cols; x++){
            printf("%c ", board[x][y]);
        }
        printf("\n");
    }
    printf("\n");
}

void walker(char **pboard, char **gboard, int rows, int cols, int click_r, int click_c){
    int n = gboard[click_c][click_r];
    pboard[click_c][click_r] = n;
    if (n == '0'){
        for (int p_x = -1; p_x < 2; p_x++){
            for (int p_y = -1; p_y < 2; p_y++){
                if (p_x+click_c >= cols || p_x+click_c < 0) continue;
                if (p_y+click_r >= rows || p_y+click_r < 0) continue;
                if (pboard[p_x+click_c][p_y+click_r] != '.') continue;
                if (gboard[p_x+click_c][p_y+click_r] == '*') continue;
                walker(pboard, gboard, rows, cols, p_y+click_r, p_x+click_c);
            }
        }
    }
}

void play(char **pboard, char **gboard, int rows, int cols, int num_bomb){
    int x = rand()%cols;
    int y = rand()%rows;
    if (gboard[x][y] == '*'){
        pboard[x][y] = '!';
        printf("Bomb Exploded! Game Over!\n");
        printf("This is the game board after explosion\n");
        print_board(pboard, rows, cols);

        free_broad(gboard, rows, cols);
        free_broad(pboard, rows, cols);
        exit(0);
    }
    if (pboard[x][y] == '.'){
        printf("Click at (%d,%d)\n", y,x);
        walker(pboard, gboard, rows, cols, y, x);
        print_board(pboard, rows, cols);
    }
    int unturned = 0;
    for (int y = 0; y < rows; y++){
        for (int x = 0; x < cols; x++){
            if (pboard[x][y] == '.')
                unturned++;
        }
    }

    if (unturned <= num_bomb){
        printf("Game Completed!\n");
        print_board(pboard, rows, cols);
        free_broad(gboard, rows, cols);
        free_broad(pboard, rows, cols);
        exit(0);
    }
}

void free_broad(char **board, int rows, int cols){
    for (int i = 0; i < cols; i++){
        free(board[i]);
    }
    free(board);
}