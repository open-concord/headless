## concord | headless
A neat little package that allows Concord$Nodes to run headless.

### compiling
Add hdls.hpp and hdls.cpp to your compile path. *If you move `hdls.hpp` to a local include path, make sure to update `hdls.cpp`'s include path for `hdls.hpp`*


### notes
hdls assumes that you're an experienced programmer (you're using a headless modified version for a distributed messaging protocal written in cpp lmao). Bad design utilizing hdls **can result in memory leaks**, check your code.

### usage
**You should only be using the functions explicitly marked as exposed, use of the underlying functions may result in unintended behavior**


#### Signal Mapping

*Incomplete*

---

#### Functions

```cpp
void Spawn(char kChar);
```
Spawns in a new headless node, which the shared memory key'd to kChar


```cpp
void Send(pid_t pid, int sig);
```
Sends a `sig` to the process running at `pid`

```cpp
void Stop(pid_t pid);
```
Forcefully stops the process running at `pid`
