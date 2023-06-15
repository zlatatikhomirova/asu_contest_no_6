# Problem C. Conveyor

A conveyor is used to transport materials from shop A to shop B.  The materials are packed in identical containers and placed on the belt one by one in production order in shop A.
Each container has a degree of urgency for processing in shop B. To arrange the containers by degree of urgency, a store is used, which is located at the end of the conveyor before entering shop B.

The accumulator works step by step, at each step the following actions are possible:
* the accumulator moves the first container from the belt to shop B;
* the accumulator moves the first container from the row to the warehouse (in the warehouse, each next container is placed on the previous one);
* the accumulator moves the top container from storage to shop B.

By the sequence of containers, we need to determine whether it is possible to sort them by degree of urgency using the described accumulator.

## Input data

The first line of the input file contains the number of tests N. This is followed by N lines, each of which describes a separate test and contains an integer K (1 ≤ K ≤ 10,000) — the number of containers in the sequence and K real numbers — the degrees of urgency of the containers in the order in which they arrived from shop A (smaller numbers correspond to a greater degree of urgency).

## Possible errors and foolproofing

1. The file may not exist.
2. N < 0 or N is not an integer.
3. There are fewer than N+1 lines in the file.
4. K < 1 or K > 10,000, or K is not an integer.
5. There are fewer or more than K+1 elements in the line.
6. The degree of urgency is not a number.

Note that we do not consider it as an error when there are more than N+1 lines in the file, or more than K containers in the test, explicitly.

## Output data 

Each line of the output file must contain the answer for one test. Output 1 if the required ordering is possible, or 0 otherwise.

## Examples
```
>> 2
>> 2 2.9 2.1 
>> 3 5.6 9.0 2.0 

<< 1
<< 0
```

## Understanding

This problem is similar to the wagon sorting problem, only here we do not have a counter, there the counter played the role of a rule according to which the wagons obviously leave.  Now we have to track when the order of container urgency starts to break down when transporting containers to shop B. We will store the last sent container to shop B, and when we send the next container, we will check that the order of container urgency is correct.

Algorithm:
1) we create a variable that will store the degree of urgency of the last container sent to shop B, with the help of it we will monitor that the sequence of sending containers is correct;
2) the accumulator receives a new container from shop A, if there are no more containers, go to (7);
3) if there are no containers in the warehouse, then we send this container to the warehouse and start again from point (2);
4) compare the degree of urgency of the container in the warehouse and the new container.  If the container in the warehouse has a degree of urgency less than that of the new container, or equal to its degree of urgency, then we send the new container to the warehouse and go to point (2);
5) if the degree of urgency of the container in the warehouse is higher, then we compare it with the degree of urgency of the last container sent to shop B, if the degree of urgency of the new container is greater, then we cannot sort the containers by degree of urgency, we exit the algorithm;
6) if the degree of urgency of the new container is less than or equal to the degree of urgency of the last container sent to shop B, then we send a new container to shop B, remembering that this container is the last of the containers sent to shop B, we return to point (3);
7) we send the containers remaining in the warehouse to shop B, comparing the degree of urgency of each of them and the degree of urgency of the last container sent to shop B, if less or equal, then we send the container to shop B, remembering that this container is the last of the containers sent  to workshop B, if not, then we can not sort the containers by urgency, we exit the algorithm.​​
