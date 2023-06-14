# Problem D. Histogram
<picture>
<img alt='A picture to explain the location of the railways and the dead end.' src="https://github.com/zlatatikhomirova/problem_solving_with_abstract_data_types/blob/main/Problem_D_Histogram/histogram.jpg">
</picture>

A histogram is a polygon formed from a series of rectangles aligned on a common baseline.  The rectangles have the same width but can have different heights.  For example, the figure on the left shows a histogram that consists of rectangles with heights 2, 1, 4, 5, 1, 3, 3. All the rectangles in this figure have a width of 1.

Typically, histograms are used to represent discrete distributions, such as the frequency of characters in texts.  Note that the order of the rectangles is very important.  Calculate the area of the largest rectangle in the histogram that is also on the common baseline.  In the picture, the shaded figure is the largest aligned rectangle in the depicted histogram.

## Input data

 The first line of the input file contains the number N (0 < N <= 10^6) the number of histogram rectangles.  This is followed by N integers , where h_1, ..., h_N, 0 ≤ h_i ≤ 10^9.  These numbers indicate the heights of the histogram boxes from left to right.  The width of each rectangle is 1.

## Possible errors and foolproofing
1. The file may not exist.
2. The file is empty.
3. N is not an integer or N < 0 or N > 10^6.
4. h_i is not an integer or h_i < 0 or h_i > 10^9.
## Output

 Print the area of the largest rectangle in the histogram.
 Remember that this rectangle must be on a common baseline.

## Examples

```
 >> 7 2 1 4 5 1 3 3
 << 8
```

## Understanding

We need to find maximum rectangular area under given histogram with n bars. (Note that 'bars' means 'rectangles', 'rectangles' means 'bars'.)

1. We create an empty stack. The stack will be used for holding indexes of heights. The bars stored in the stack are always in increasing order of their heights
2. We run through all bars of a given histogram:
  * start the counter, set it to 0;
  * if the stack is empty or current bar is higher than the bar with index which is on the top of the stack, push it to stack and increase the counter;   
  * if the height of the current bar is less than or equal to the height of the bar at the top of the stack, then pop the index of bar from the stack and calculate the area of the maximum rectangle inscribed in the histogram.  Note that its left coordinate is the index of the bar that lies before this bar on the stack (it is the rightmost bar, which is to the left of the removed one and at the same time lower in height) plus one.  And the right coordinate is the one that we are now considering. Suspicious for a rectangle with maximum area will be a rectangle with sides (current counter - the top of the stack).
