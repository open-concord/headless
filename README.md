## concord | headless
A neat little package that allows Concord$Nodes to run headless.

### usage
Add hdls.hpp and hdls.cpp to your compile path. *If you move `hdls.hpp` to a local include path, make sure to update `hdls.cpp`'s include path for `hdls.hpp`*


### notes
hdls assumes that you're an experienced programmer (you're using a headless modified version for a distributed messaging protocal written in cpp lmao). Bad design utilizing hdls **can result in memory leaks**, check your code.
