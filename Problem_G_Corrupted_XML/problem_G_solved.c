#define _CRT_SECURE_NO_WARNINGS

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
* STR_LEN - length of the string for enumeration
* EXPR_LEN - maximal length for an expression
*/
#define STR_LEN 29
#define EXPR_LEN 1000

struct Node
{
	/* 
     * Definition of the structure corresponding to the node. 
     * It keeps track of a single element, as well as a pointer to the next 
     * element in the stack. 
     * 
     * Attributes: 
     *   data    (char *)             the value that the element contains 
     *   next    (struct Node *)    a pointer to the node that directly follows this node 
     *   prev    (struct Node *)    a pointer to the previous node 
     */
	char *data;
	struct Node *next, *prev;
};

struct Stack
{
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

int Push(struct Stack *S, char *x, int N)
{
	/*  
   * To insert a new value on top of the stack. 
   * @param S    (struct Stack *)    a pointer to the stack 
   * @param x    (char *)              a value to push onto the stack 
   * @param N (int) the length of the data string
   * 
   * returns     (int)               whether it's possible to allocate memory 
   *                                 for a new node 
   */
	struct Node *NewNode;
	NewNode = (struct Node *)malloc(sizeof(struct Node)); // create a new node
	if (NewNode == NULL)								  // if it is not possible to allocate memory for a new node
		return 0;
	NewNode->data = (char *)malloc(sizeof(char) * (N + 1));
	strcpy(NewNode->data, x); // and fill it with data
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

int Pop(struct Stack *S, char *x)
{
	/* 
    * To remove the top value from the stack. 
    * @param S    (struct Stack *)    a pointer to the stack 
    * @param x (char *) data that was stored in the popped element
    * 
    * returns     (int)             an error (-1) or len of popped data
    */
	struct Node *TopNode = S->Head; // the top of the stack
	if (TopNode == NULL)			// if the stack is empty
		return -1;					// return an error
	int N;
	N = strlen(TopNode->data);
	// save data to variable
	strcpy(x, TopNode->data);
	S->Head = TopNode->next; // new top of the stack is a previous node
	if (S->Head)
		S->Head->prev = NULL; // the previous node for this node doesn't exist
	else
		S->Tail = NULL;
	free(TopNode);
	return N;
}


char *customTok(char *input, char *delim)
{
    /*
    * A function to split a string by the other string.
    *
    * @param input (char *) input string
    * @param delim (char *) a delimiter
    *
    * returns temp (char *) xurrent token
    */
	static char *string; // string to tokenize
	if (input != NULL)	 // if a pointer to input string is not NULL
		string = input;	 // initialize

	if (string == NULL) // the exit case
		return string;

	// get a pointer of the entry
	char *end = strstr(string, delim);
	if (end == NULL)
	{						 // if there is no entry
		char *temp = string; // current token is the same as current string for tokenization
		string = NULL;		 // the next iteration will be the last
		return temp;
	}

	char *temp = string; // current token

	*end = '\0'; // it is needed to continue tokenization
	string = end + strlen(delim);
	return temp;
}

int correct(char *input, int N)
{
    /*
    * A function for checking if the input string is correct. 
    * It means, that the string contains only lowercase Latin letters and symbols '/', '<', '>'.
    *
    * @param input (char *) input string
    * @param N (int) a length of the string
    * 
    * returns (int) 1 or 0 if the string is correct or not
    */

	for (int i = 0; i < N; i++)
		if (!(islower(input[i]) || strchr("<>/", input[i])))
			return 0;
	return 1;
}

int check(char *changedExp, int N)
{
    /*
    * A function is meant to check that we can get a valid XML string with a given string transformation.
    *
    * @param changedExp (char *) transformated string
    * @param N (int) length of this string
    *
    * returns (int) 1 or 0 if we can get a valid XML string with a given string transformation or not
    */
	// Initialize the delimiter string
	char *delim = "><";

	// Split the line using customTok()
	char *token = customTok(changedExp, delim);
	// initialize stack
	struct Stack S;
	S.Head = S.Tail = NULL;
	int OK = 1; // everything is great at the beginning
	// while we can tokenize the string and everythig goes fine
	while ((token != NULL) && OK)
	{
		// if there are any errors at the transformated string
		if ((strchr(token, '<') != NULL) || (strchr(token, '>') != NULL) || (strchr(token + 1, '/') != NULL))
		{
			OK = 0;
		}
		if (token[0] != '/')
		{
			// add token without '/' (opened tag) onto the stack
			Push(&S, token, strlen(token));
		}
		else
		{
			char *x = (char *)malloc(sizeof(char) * (N));
			// otherwise, if stack is empty, return an error in the expression
			// else - exract top element from the stack
			if (S.Head == NULL)
				OK = 0;
			else
			{
				Pop(&S, x);
				// if opened and closed tags do not match then return an error in the expression
				if (strcmp(x, token + 1) != 0)
				{
					OK = 0;
				}
				free(x);
			}
		}
		// Get the next token
		token = customTok(NULL, delim);
	}
	token = NULL;
	// if the stack is not empty the we can't complete the task with this string, because there are some unmatched tags
	if (S.Tail != NULL)
		OK = 0;
	return OK; // can complete the task with this string
}

int main()
{
	FILE *test, *ans; // input and output files
	test = fopen("test.txt", "r");
	ans = fopen("ans.txt", "w");
  if (test == NULL) // if file does not exist
    return -1;
	// the string for enumeration
	const char str[] = "qwertyuiopasdfghjklzxcvbnm<>/";
	char *exp = (char *)malloc(sizeof(char) * (EXPR_LEN + 1)); // input string
	fscanf(test, "%s", exp);
	int N;			   // length of expression
	N = strlen(exp);
	if ((N > EXPR_LEN) || (N < 7)) // if N not in its domian
		return -1;
  if (!correct(exp, N)) // if expression contains something forbidden by the conditions
		return -1;	   // an error
	char *changedExp = (char *)malloc(sizeof(char) * (N)); // expression for checking in cycle
	strcpy(changedExp, exp + 1);
	changedExp[N - 2] = '\0';
	// check the line borders, opened and closed tags
	int start, end, checked = 0;
	start = exp[0] != '<';				// incorrect left border
	end = exp[N - 1] != '>';			// incorrect right border
	checked = check(changedExp, N - 2); // correctness of the string tags
	if (start)
	{				   // incorrect left border
		if (end)	   // and incorrect right border
			return -1; // is an error
		else if (checked)
		{ // incorrect left border and correct right border is OK, if tags are correct
			exp[0] = '<';
			fprintf(ans, "%s", exp);
			return 0;
		}
		else // otherwise we can't solve the task'
			return -1;
	}
	// if end of expression is not '>' and we have a correct left border
	else if (end)
	{
		if (checked)
		{ // if tags are correct, no error
			exp[N - 1] = '>';
			fprintf(ans, "%s", exp);
			return 0;
		}
		else
			return -1; // and an error otherwise
	}
	// there we transformate the expression, check the correctness of its tags and the give an answer
	for (int i = 0; i < N - 2; i++)
	{
		for (int j = 0; j < STR_LEN; j++)
		{
			strcpy(changedExp, exp + 1);
			changedExp[N - 2] = '\0';
			changedExp[i] = str[j];
			if (check(changedExp, N - 2))
			{
				exp[i + 1] = str[j];
				fprintf(ans, "%s", exp);
				return 0;
			}
		}
	}
	return 0;
}
