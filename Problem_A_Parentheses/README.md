# Problem A. Parentheses 

Given a sequence of N round, square and curly brackets. 
Find out if it is possible to add numbers and signs of arithmetic operations to it so that a correct arithmetic expression is obtained. 

## Input data 

The first line contains the number of brackets N, the second - N characters from the set (, ), [, ], {, }. 1 <= N <= 100,000. 

## Possible input errors and foolproofing

1. N < 1 or N > 100,000.
2. N is not an integer.
3. Any character is not a bracket.

## Output 

The word "Yes" is displayed if it is possible to obtain the correct arithmetic expression, or "No" if it is not possible. 

## Examples 

``` 
>> 2 
>> {} 
<< Yes 

>> 3 
>> {}( 
<< No 
```

## Understanding 

This problem can be solved using the stack. The stack is initially empty. We go through the entire given string from the beginning to the newline character. That is, we read each new character as we solve the problem, until we meet the newline character. The opening and closing brackets are stored in the arrays br1 and br2. If we encounter an opening parenthesis, we push it onto the stack. If a closing brace is encountered, then the corresponding opening brace must be on top of the stack. If so, pop it from the stack. If the stack is empty, or if there is another type of parenthesis at the top of the stack, the expression is invalid. At the end of the pass, the stack must be empty. 
The OK variable is used to exit the outer loop when an error is encountered (and not examine the rest of the line). It is set to zero if a brace of another type is found on the stack. 
The program below uses the Node, Stack structures and the Push and Pop operations.

