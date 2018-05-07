#K-Nearest Neighbor

his code implements KNN problem with kd-tree and bruteforce.

## Get Started
To run the code
```
make -j
```
this will output two binary, `gen` and `main`
`./gen` can generate some testdata, and the format are quite simple
```
N D
p[1,1] p[1,2] ... p[1,D]
p[2,1] p[2,2] ... p[2,D]
...
Q
K1 t[1,1] t[1,2] ... t[1,D]
K2 t[2,1] t[2,2] ... t[2,D]
...
```
N is the number of training samples, D is the dimension, Q is the number of test samples.

`./main` read these data from standard input, and use both kdtree and bruteforce to run the samples.
Finally, it will compare the results of the two algorithm, and terminate if the results are different.

You can simply type `make run` to show the output of the program.

---
To use the code in your project, include `knn.h`, and see the example in `main.cc`

_This code hasn't been test comprehensively, do not use it in production._
