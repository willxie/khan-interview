# Installation

Specification is in `spec.txt`

To compile:

```
g++ -std=c++11 infection.cpp -o infection -Wall -Werror -Wextra
```

To run:

```
./infection
```

# Design decisions

I have modeled the problem as a graph with directed edges connecting masters to apprentices (mentor to pupils). I have implemented a few variations of the infection. For the basic total infection, I used flood fill and breath first search (BFS) to change the version of every node connected to the starting node. I made the function as general as possible; it avoids cycles by tagging each session with a unique number. There are two version of the limited infection. The first, it simply stops after updating a set number of users. Based on the problem setup, I have decided to process immediate masters and apprentices of the first infected user first. This way, the update can be contained in a cluster making all infected users from this session with the fewest number of hops from the source as possible, making effect easy to observe. The second variation, another constraint is added to make sure that every master and all its apprentices are on the same version. This version updates each subgraph in the connected components similar to before (close to far) but skips all mentor with apprentices count exceeding the remaining user slots. It traverses through the entire connected components set to try to update as many subgraphs as possible.

Limited infection with exact number of users can be done by first breaking the graph into mentor and apprentices subgraphs first. Then, the program must try every combination of subgraphs until it finds a set that adds up to the exact number. Dynamic programming could speed up the process. I did not have the time to implement it as it is rather labor intensive to get it correct under all corner cases.
