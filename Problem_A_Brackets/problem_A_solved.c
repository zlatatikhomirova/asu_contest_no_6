#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

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
    struct Node *next, *prev;
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
    struct Node *Head, *Tail;
};

int Push(struct Stack *S, char x) {
  /* 
  * To insert a new value on top of the stack.
  * @param S    (struct Stack *)    a pointer to the stack
  * @param x    (char)              a value to push onto the stack
  *
  * returns     (int)               whether it's possible to allocate memory
  *                                 for a new node
  */
  struct Node *NewNode;
  NewNode = (struct Node *)malloc(sizeof(struct Node)); // create a new node
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

char Pop(struct Stack *S) {
   /*
   * To remove the top value from the stack.
   * @param S    (struct Stack *)    a pointer to the stack
   *
   * returns     (char)              a popped element
   */
    struct Node *TopNode = S->Head; // the top of the stack
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
  char br1[3] = { '(', '[', '{' }; // opening brackets
  char br2[3] = { ')', ']', '}' }; // closing brackets
  char cur_elem, upper; // current element, top of the stack
  int i, k, OK = 1, N;
  struct Stack S; // character stack
  S.Head = NULL; S.Tail = NULL; // first the stack is empty
  // if we've scanned not an integer or scanned integer not in [1; 100,000]
  if ((scanf("%d", &N) != 1) || (N < 1) || (N > 100000))
    return -1; // 
  fflush(stdout);
  cur_elem = getchar();
  for (i = 0; (i < N) && ((cur_elem = getchar()) != '\n'); i++) {
    fflush(stdout);
    for (k = 0; k < 3; k++) // check 3 types of brackets
    {
      if (cur_elem == br1[k]) {
        // if current element is an opening bracket
        Push(&S, cur_elem);
        break;
      }
      if (cur_elem == br2[k]) {
        // if current element is a closing bracket
        upper = Pop(&S);
        if (upper != br1[k])
          // if brackets do not make a necessary pair
          OK = 0;
        break;
      }
      if (k == 2) {
        // if current element is not a bracket
        return -1;
      }
    }
  }
  if (i < N) // if N in its domain, but there were not enough brackets
    return -1;
  if (OK && (S.Head == NULL))
    printf("Yes");
  else
    printf("No");
  return 0;
}
