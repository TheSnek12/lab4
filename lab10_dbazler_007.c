/*
David Bazler G01582055
CS 262 Lab Section 007
Lab 10
*/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct node_struct
{
    char* title;
    struct node_struct* next;
} node;

node* create_node(char* title, node* next);
void insert_movie(node** head, char* title);
void delete_movie(node** head, int pos);
void display_movie(node* head);

int main(int argc, char const *argv[])
{
    node* head = NULL;

    while (1)
    {
        char buff[256];
        
        printf("Enter your choice:\n"
                "Add a movie:\t\tadd or Add\n"
                "Delete a movie:\t\tdelete or Delete\n"
                "Display a movie list:\tdisplay or Display\n"
                "Exit:\t\t\texit or Exit\n");
        scanf(" %s", buff);
        if (strcmp(buff, "add") == 0 || strcmp(buff, "Add") == 0){
            printf("Enter movie title:\n");
            scanf(" %s", buff);
            insert_movie(&head, buff);

        }
        if (strcmp(buff, "delete") == 0 || strcmp(buff, "Delete") == 0){
            int p;
            printf("Enter position, starting from 0:\n");
            scanf(" %d", &p);
            delete_movie(&head, p);
        }
        if (strcmp(buff, "display") == 0 || strcmp(buff, "Display") == 0){
            display_movie(head);
        }
        if (strcmp(buff, "exit") == 0 || strcmp(buff, "Exit") == 0){
            break;
        }
    }
    
    return 0;
}

void insert_movie(node** head, char* title){
    if (*head == NULL){
        (*head) = create_node(title, NULL);
        return;
    }

    node* prev = NULL;
    node* n = *head;
    while (n != NULL && strcmp(n->title, title) < 0){
        prev = n;
        n = n->next;
    }
    if (prev == NULL){
        node* new = create_node(title, n);
        (*head) = new;
        return;
    }
    if (n == NULL){
        node* new = create_node(title, NULL);
        prev->next = new;
        return;
    }
    node* new = create_node(title, n);
    prev->next = new;

}
node* create_node(char* title, node* next){
    node* n = malloc(sizeof(node));
    n->title = malloc(256);
    strcpy(n->title, title);
    n->next = next;
    return n;
}
void display_movie(node* head){
    while (head != NULL){
        printf("%s\n", head->title);
        head = head->next;
    }
}
void delete_movie(node** head, int pos){
    node* prev = NULL;
    node* n = *head;
    for (int i = 0; i < pos; i++){
        if (i != pos-1 && n->next == NULL){
            printf("out of range\n");
            return;
        }
        prev = n;
        n = n->next;
    }
    free(n->title);
    if (prev == NULL){
        (*head) = n->next;
        free(n);
        return;
    }
    prev->next = n->next;
    free(n);

}