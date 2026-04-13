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

struct item_struct
{
    int itemId;
    char* itemName;
    int quantity;
    double pricePerItem;
};

typedef struct item_struct Item;
Item* expand_array(Item* arr, int* arrSize);
void free_array(Item* arr, int lastIndex);

int main(int argc, char const *argv[])
{   
    int inventorySize = 5;
    int nextIndex = 0;
    Item* itemInventory = malloc(inventorySize*sizeof(Item));

    while (1){

        printf("Enter your choice:\n"
                "'i' - Insert an item\n"
                "'u' - Update the database\n"
                "'s' - Search the database\n"
                "'p' - Print the database\n"
                "'q' - Quit the program\n");
        char in;
        
        scanf(" %c", &in);

        switch (tolower(in))
        {
        case 'i':
            {
            printf("Enter item id: ");
            int id;
            scanf(" %d", &id);
            printf("Enter item name: ");
            char* name = malloc(64);
            scanf(" %s", name);
            printf("Enter quantity: ");
            int quantity;
            scanf(" %d", &quantity);
            printf("Enter price per item: $");
            double price;
            scanf(" %lf", &price);


            int valid = 1;
            for (int i = 0; i < nextIndex; i++){
                if (itemInventory[i].itemId == id){
                    printf("Error Inserting Item");
                    valid = 0;
                    break;
                }
            }

            if (!valid){
                continue;
            }

            if (inventorySize <= nextIndex){
                Item* expanded_array = expand_array(itemInventory, &inventorySize);
                free(itemInventory);
                itemInventory = expanded_array;

            }
            Item elm = {
                id,
                name,
                quantity,
                price
            };
            itemInventory[nextIndex++] = elm;
            }               
            break;
        case 'u':
            {
            int idx;
            int found = 0;
            printf("Enter item id: ");
            int id;
            scanf(" %d", &id);

            for(int i = 0; i < nextIndex; i++){
                if (itemInventory[i].itemId == id){
                    found = 1;
                    idx = i;
                }
            }

            if (!found){
                printf("Item Not Found");
                continue;
            }
            printf("Enter new quantity: ");
            int quan;
            scanf(" %d", &quan);
            itemInventory[idx].quantity = quan;
            }
            break;
        case 's':
            {
            int idx;
            int found = 0;
            printf("Enter item id: ");
            int id;
            scanf(" %d", &id);

            for(int i = 0; i < nextIndex; i++){
                if (itemInventory[i].itemId == id){
                    found = 1;
                    idx = i;
                }
            }

            if (!found){
                printf("Item Not Found");
                continue;
            }

            printf("*********************************************************\n");
            printf("\tItem ID: %d\n", itemInventory[idx].itemId);
            printf("\tItem Name: %s\n", itemInventory[idx].itemName);
            printf("\tQuantity: %d\n", itemInventory[idx].quantity);
            printf("\tPrice per item: $%.2lf\n", itemInventory[idx].pricePerItem);
            printf("*********************************************************\n");
            }
            break;
        case 'p':
            {
                printf("*********************************************************\n");
                printf("     %-10s%-15s%-15s%s\n", "ID", "Name", "Quantity", "Price");
                for (int i = 0; i < nextIndex; i++){
                    printf("     %03d       %-15s%-15d%.2lf\n", itemInventory[i].itemId, itemInventory[i].itemName, itemInventory[i].quantity, itemInventory[i].pricePerItem);
                }

                printf("*********************************************************\n");
            }
            break;
        case 'q':
            free_array(itemInventory, nextIndex);
            exit(0);
            break;
        default:
            printf("%c is not a valid choice", in);
            break;
        }
    }
}

Item* expand_array(Item* arr, int* arrSize){
    
    Item* new_arr = malloc(*arrSize*2*sizeof(Item));
    memcpy(new_arr, arr, *arrSize*sizeof(Item));
    *(arrSize) *= 2;
    return new_arr;

}

void free_array(Item* arr, int lastIndex){
    for (int i = 0; i < lastIndex; i++){
        free(arr[i].itemName);
    }
    free(arr);
}