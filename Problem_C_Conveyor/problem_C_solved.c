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
    int N - the number of tests,
    int K - the number of containers,
    int i = 0 - counter for cycle,
    int j - counter for cycle,
    int OK - if we can sort these containers,
    int pushed - if we've pushed container to shop B,
    int flag - if we haven't sent anything to shop B yet,
    double container - the degree of urgency of current container,
    double last_sent - the degree of urgency of the last sent to shop B container,
    double last_container - the degree of urgency of the last sent to stock container;
    */
    FILE* test, *ans;
    test = fopen("test.txt", "r"); // input file
    ans = fopen("ans.txt", "w"); // output file
    if (test == NULL) // this file does not exist
        return -1;

    int N, K, i = 0, j, OK, pushed, flag;
    double container, last_sent, last_container;
    struct Stack stock; // stock means warehouse
    stock.Head = NULL; stock.Tail = NULL; // first the stack is empty

    if ((fscanf(test, "%d", &N) != 1)) // if N is not an integer
        return -1;
    for (; (i < N) || (!feof(test)); i++) { // if N < 0, then this cycle will not be executed; 
        // if there are fewer than N+1 lines in the file, then we will read the data while we can
        if (fscanf(test, "%d", &K) != 1) // if K is not an integer
            return -1;
        if ((K < 1) || (K > 10000)) // if K is not in its domain
            return -1;
        j = flag = 0, OK = 1;
        while ((j < K) && OK) {
            if (fscanf(test, "%lg", &container) != 1) // if the degree of urgency is not a number
                return -1; j++;
            pushed = 0;
            while (!pushed) { // while we have something in the stock
                if (stock.Head == NULL) {
                    Push(&stock, container);  // stock was empty, but we've pushed container to stock
                    pushed = 1;
                }
                else {
                    last_container = stock.Head->data;
                    if (last_container >= container) {
                        // if the last container's degree of urgency is less or equal to the current container's degree of urgency,
                        // then we will send it to stock
                        Push(&stock, container);
                        pushed = 1;
                    }
                    else { // if we've sent containers to the store B and
                        // if the last sent container's degree of urgency is less than the last container's degree of urgency,
                        if ((!flag) && (last_sent > last_container)) { // then we can't sort containers
                            OK = 0;
                            break;
                        }
                        else // we send last container from stock to shop B
                            last_sent = Pop(&stock);
                    }
                }
            }
        }
        while (OK && stock.Head != NULL) { // while there are containers in the stock
            container = Pop(&stock);
            if (last_sent <= container) // and we can sort them 
                last_sent = container; // if the last sent container's degree of urgency is more or equal to the last container's degree of urgency
            else
                OK = 0;
        }
        fprintf(ans, "%d\n", OK); // answer on the test
    }
    return 0;
}
