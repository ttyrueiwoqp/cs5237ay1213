1.Implemented Toom-Cook multiplication for LongInt multiplication. Details from http://en.wikipedia.org/wiki/Toom-Cook

2.Simplified determinant calculation for InCircle test. Details from the papers at http://www.cs.cmu.edu/~quake/robust.html

3.InCircle test will return 1 if the point is inside the circle, 0 if the point is on the circle and -1 if the point is outside the circle.

4.In the case that 3 points are collinear, InCircle test will return 1 if the 3 points are the same, -1 if they are all different. If 2 points are the same, treat the 2 different points as the diameter of the circle, and perform a test to determine whether the point is inside/on/outside the circle.