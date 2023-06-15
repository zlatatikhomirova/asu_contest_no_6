#define _CRT_SECURE_NO_WARNINGS

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>

struct Node {
    /*
    * Definition of the structure corresponding to the node.
    * It keeps track of a single element, as well as a pointer to the next
    * element in the stack.
    *
    * Attributes:
    *   data    (char)             the value that the element contains
    *   next    (struct Node *)    a pointer to the node that directly follows this node
    *   prev    (struct Node *)    a pointer to the previous node
    */
    char data;
    struct Node* next, * prev;
};

struct Stack {
    /*
    * Definition of the structure corresponding to the stack.
    * It stores pointers to the beginning and end of the stack.
    *
    * Attributes:
    *   Head    (struct Node *)    a pointer to the beginning of the stack
    *   Tail    (struct Node *)    a pointer to the end of the stack
    */
    struct Node* Head, * Tail;
};

int Push(struct Stack* S, char x) {
    /*
    * To insert a new value on top of the stack.
    * @param S    (struct Stack *)    a pointer to the stack
    * @param x    (char)              a value to push onto the stack
    *
    * returns     (int)               whether it's possible to allocate memory
    *                                 for a new node
    */
    struct Node* NewNode;
    NewNode = (struct Node*)malloc(sizeof(struct Node)); // create a new node
    if (NewNode == NULL) // if it is not possible to allocate memory for a new node 
        return 0;
    NewNode->data = x; // and fill it with data
    // the next node for this node is a current top of the stack
    NewNode->next = S->Head;
    // the previous node for this node doesn't exist
    NewNode->prev = NULL;
    if (S->Head) // add to the top of the stack
        S->Head->prev = NewNode;
    S->Head = NewNode;
    if (S->Tail == NULL)
        S->Tail = S->Head;
    return 1;
}

char Pop(struct Stack* S) {
    /*
    * To remove the top value from the stack.
    * @param S    (struct Stack *)    a pointer to the stack
    *
    * returns     (char)              a popped element
    */
    struct Node* TopNode = S->Head; // the top of the stack
    char x; // data that was stored in the popped node
    if (TopNode == NULL) // if the stack is empty
        return (char)(255); // return a character with code 255
    x = TopNode->data; // save data to variable
    S->Head = TopNode->next; // new top of the stack is a previous node
    if (S->Head)
        S->Head->prev = NULL; // the previous node for this node doesn't exist
    else
        S->Tail = NULL;
    free(TopNode);
    return x;
}



int main() {
    /*
    *  int N - number of wagons,
    *  int i = 0 - counter for cycle,
    *  int trains_railway2 = 1 - counter of wagons,
    *  int train - current wagon's number, 
    *  int last_train - the number of the last wagon in the deadend, 
    *  int OK = 1 - if we can sort these wagons, 
    *  int pushed - if we've pushed wagon to the deadend. 
    * 
    * Notice, when we say 'train' we mean 'wagon'.
    */ 
    struct Stack dead_end; // character stack
    dead_end.Head = NULL; dead_end.Tail = NULL; // first the stack is empty
    int N, i = 0, trains_railway2 = 1, train, last_train, OK = 1, pushed; 
    if (scanf("%d", &N) != 1) // if N is not an integer
        return -1;
    if ((N < 1) || (N > 100)) // if N is not in its domain
        return -1;
    while ((i < N) && OK) {
        if (scanf("%d", &train) != 1) // if train's number is not an integer
            return -1; 
        i++;
        pushed = 0;
        while (!pushed) {
            if (dead_end.Head == NULL) {
                Push(&dead_end, train);
                // stack was empty, but we've pushed wagon to the deadend
                pushed = 1;
            }
            else {
                last_train = dead_end.Head->data;
                if (last_train > train) { // if the number of the last train is bigger than current train's number, 
                                          // then we push it to the deadend
                    Push(&dead_end, train);
                    pushed = 1;
                }
                else if (last_train == train) { // if the number of the last train is equal to the current train's number,
                                                // then something goes wrong - errors in data
                    OK = 0;
                    break;
                }
                else {
                    // the last train's number is smaller than current train's number, that's great
                    if (trains_railway2 != last_train) { // the counter of wagons is not equal to the last train's number,
                                                         // so, we can't sort wagons
                        OK = 0;
                        break;
                    }
                    else {
                        // the counter of wagons is equal to the last train's number,
                        // so, we extract the last wagon from the deadend
                        trains_railway2++;
                        Pop(&dead_end);

                    }
                }
            }
        }
    }
    // extract wagons while there are wagons and the counter of wagons is equal to the last wagon's number
    while (OK && dead_end.Head != NULL) {
        train = Pop(&dead_end);
        if (train == trains_railway2++);
        else
            OK = 0;
    }

    // 'yes', if we can sort wagons, 'no' - otherwise
    printf("%s", (OK) ? "Yes" : "No");
    return 0;
}
