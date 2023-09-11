#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct linked_list {
    char info[11];
    struct linked_list* next_p;
    struct linked_list* prev_p;
}node;

node* myList;
int n;

//initialises a new list, returns a pointer to the head
node* list_start(char line[11]);

//adds a new node to the head of the list and returns a pointer to it;
node* list_add(node* list_p, char line[11]);

//deletes last element of the list, returns pointer to the list;
node* del_list(node* list_p);

//prints  all the elements in list
void print_list(node* list_p);

//swaps to elements in the list(first one is the one closer to the head) by their pointer and return pointer to the list
node* swap(node* list_p, node* first_p, node* second_p);

//sorts list in alphabetical order and return a pointer to it
node* list_sort(node* list_p);

//frees all the memory allocated to the list
void free_list(node* list_p);

//fills the list from keyboard
node* list_fill(node* list_p);

int main() {
    printf("Enter n:");
    scanf("%d", &n);
    myList = list_fill(myList);
    print_list(myList);
    myList = list_sort(myList);
    print_list(myList);
    free_list(myList);
    return 0;
}

node* list_start (char line[11]){
    node* this_p;
    this_p = malloc(sizeof(node));
    strcpy(this_p->info,line);// copying line array into info
    this_p->next_p = NULL;
    this_p->prev_p = NULL;
    //printf("this pointer: %p,\tinfo: %s\t next pointer: %p\n",this_p, this_p->info, this_p->next_p);
    return this_p;
};

node* list_add(node* list_p, char line[11]) {
    if(list_p == NULL){
        return NULL;
    }
    node* this_p;
    this_p = malloc(sizeof(node));
    strcpy(this_p->info,line);
    this_p->next_p = list_p;
    this_p->prev_p = NULL;
    list_p->prev_p = this_p;
    //printf("this pointer: %p,\tinfo: %s\t next pointer: %p\n",this_p, this_p->info, this_p->next_p);
    return this_p;
};

node* del_list(node* list_p){
    if (list_p != NULL) {
        node* next_p = list_p->next_p;
        free(list_p);
        return next_p;
    }
    return NULL;
}

void print_list(node* list_p){
    node* this_p = list_p;
    while(this_p != NULL){
        printf("info: %s,\tthis pointer: %p,\tnext pointer: %p;\tprev pointer: %p\n",this_p->info, this_p, this_p->next_p, this_p->prev_p);
        this_p = this_p->next_p;
    }
    printf("list ended :( \n\n");
}

node* swap(node* list_p, node* first_p, node* second_p){
    if (first_p == NULL || second_p == NULL) {
        return list_p;
    }

    if (first_p == second_p) {
        return list_p;
    }

    if (first_p == list_p) {
        list_p = second_p;
    } else if (second_p == list_p) {
        list_p = first_p;
    }
    if (first_p->next_p == second_p && second_p->prev_p == first_p){ //if the nodes are neighbours
        first_p->next_p = second_p->next_p;
        second_p->prev_p = first_p->prev_p;
        first_p->prev_p = second_p;
        second_p->next_p = first_p;

        if (first_p->next_p != NULL) {
            first_p->next_p->prev_p = first_p;
        }

        if (second_p->prev_p != NULL) {
            second_p->prev_p->next_p = second_p;
        }
    } else {
        node *temp = first_p->next_p;
        first_p->next_p = second_p->next_p;
        second_p->next_p = temp;

        temp = first_p->prev_p;
        first_p->prev_p = second_p->prev_p;
        second_p->prev_p = temp;

        if (first_p->next_p != NULL) {
            first_p->next_p->prev_p = first_p;
        }
        if (second_p->next_p != NULL) {
            second_p->next_p->prev_p = second_p;
        }
        if (first_p->prev_p != NULL) {
            first_p->prev_p->next_p = first_p;
        }
        if (second_p->prev_p != NULL) {
            second_p->prev_p->next_p = second_p;
        }
    }
    return list_p;
}

node* list_sort(node* list_p){
    node* this_p = list_p;
    while(this_p != NULL){
//        printf("this_p is: %s \n", this_p->info);
        node* smallest_p = this_p;
        node* curr_p = this_p->next_p;
        while (curr_p != NULL){
//            printf("Smallest: %s\t current: %s\n", smallest_p-> info, curr_p->info);
            if (strcmp(curr_p->info, smallest_p->info) <= 0){
                smallest_p = curr_p;
//                printf("now the smallest is %s\n", curr_p-> info);
            }
            curr_p = curr_p->next_p;
        }
        if (this_p != smallest_p) {
            //printf("swapping %s \tand %s\n", this_p, smallest_p);
            list_p = swap(list_p, this_p, smallest_p);
        }
        // printf("printing inside!!\n");
        //print_list(list_p);
        this_p = smallest_p->next_p;
    }
    return list_p;
}

void free_list(node* list_p) {
    node* this_p = list_p;
    while(this_p != NULL){
        node* next_p = this_p->next_p;
        free(this_p);
        this_p = next_p;
    }
}

node* list_fill(node* list_p){
    char word[11];
    printf("Enter the first word in the list under 10 characters (%d left): \n", n);
    scanf("%s", word);
    list_p = list_start(word);
    for (int i = 0; i < n-1; i++){
        printf("Enter next word under 10 characters (%d left): \n", n-i-1);
        scanf("%s", word);
        list_p = list_add(list_p, word);
    }
    return list_p;
}