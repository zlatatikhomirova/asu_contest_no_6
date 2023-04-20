# Problem B. Sorting of wagons

<picture>
<img alt='A picture to explain the location of the railways and the dead end.' src="https://github.com/zlatatikhomirova/problem_solving_with_abstract_data_types/blob/main/Problem_B_Sorting_of_wagons/picture_railways.png">
</picture>

To the dead end from the first railway (look at the picture) a train has arrived.  It is allowed to unhook one or several first wagons from the train at once and bring them to the dead end (if desired, you can even bring the entire train to a dead end at once).  After that, take some of these wagons to the second railway. After that, you can bring a few more wagons to the dead end and again take some of the remaining wagons to the second railway. And so on: so that each wagon only once can go from the first railway to a dead end, and then leave the dead end onto the second railway once. It is prohibited to go into the dead end from the second railway or leave the dead end onto the first railway. It is impossible to get from the first railway to the second railway without stopping at a dead end.

It is known in what order wagons initially go.  Using the operations that have been described in the text, it is required to make the wagons go in order (the first, then the second, etc., counting from the head of the train traveling on the second railway away from the dead end).

We need to determine whether the wagons can be sorted in this way or not.

## Input data

Enter the number N — the number of wagons in the train (1≤N≤100).  Next are the wagons' numbers in order from the head of the train traveling on the first railway towards the dead end. The wagons are numbered with natural numbers from 1 to N, each of which occurs exactly once.

## Possible input errors and foolproofing

1. N < 1 or N > 100.
2. Any of the wagons' numbers occurs twice.
3. Input not a natural number.

## Output

If you can make the wagons go in order from 1 to N, counting from the head of the train, when the train takes the second railway from the dead end, you need to print the message YES, if this is not possible, you need to print NO.

## Examples
```
>> 3
>> 3 2 1
<< YES

>> 4
>> 4 1 3 2
<< YES

>> 3
>> 2 3 1
<< NO

```
## Comments on examples

The first test suggests the following: you need to bring the entire train to a dead end, and then take it entirely to the second railway.

In the second test, the following is proposed: first, you need to bring two wagons to the dead end, leave one of them in the dead end, and take the second to the second railway, then bring two more wagons to the dead end and take out 3 wagons standing at the dead end to the second railway.

## Understanding

The observation is that if the number of the wagon at the dead end is less than the number of the wagon that we want to drive, then it must be taken to second railway, otherwise it will go after the new wagon.  So the actions here are completely determined by the conditions:
1) we start the counter, with the help of it we will track the sequence of wagons on the second railway. At the beginning, we will set it to 1;
2) we get a new wagon from the first railway, if we run out go to (8);
3) if there are no wagons in the dead end, then we send this wagon to the dead end and start again from point (2);
4) compare the number of the wagon in the dead end and the new wagon's.  If there is a larger number in the dead end, then we take a new wagon to the dead end and go to point (2);
5) if the numbers are equal, then something is wrong with the input data - an error;
6) if the number is less, then we compare the number of the wagon from the dead end with the counter, if they are different, then we can't line up the wagons in the correct order, we stop the execution of the algorithm;
7) if everything is fine, then increase the counter, put the wagon on the second railway and return to point (3);
8) push the wagons to the second railway comparing with the counter, if they are equal, everything is fine, increase the counter to the next wagon, if not, then some wagon was not in the sequence, the problem with the input data is an error.
