# Bamboo - (Soon to be) A Rudimentary Network Simulator

Utilizing an in-house library of various data structures and functions, 
mostly based on the available structures offered from the Rust Programming Language's standard library.

### Build Prerequisites:
- This project makes use of Clang's "Blocks" extension for C, so clang must be used to compile the project
    
On Ubuntu, install the required packages with:
```
sudo apt install cmake llvm clang libblocksruntime-dev
```
    
Clone this repo and cd into the repo folder:
```
git clone https://github.com/three014/Bamboo.git && cd Bamboo
```

Then run this script to set the C compiler to clang (or feel free to set clang as your 
default C compiler elsewhere if you don't want to run this everytime you CMake):
```
source ./clang_vars.sh
```

After that, the standard CMake commands:
```
mkdir build
cmake -S . -B build
```

I also provide a quick makefile that sets/resets the build folder. You can run it with:
```
make setup
```
though it's not required if you prefer to use the standard CMake commands.

Finally, make the project with:
```
cd build && make
```

The executable will be located at {bamboo_source_folder}/build/rns/rns*








## How to config files:

Nodes are configured in _json_ format, while the simulation is configured in a _csv_ format.

- For the nodes, each node is an object '{}' containing a,
  - Node "id" field 
  - "queueDelay" field to give the node artificial traffic delay
  - "directNeighbors" array field, where each array contains the 
    node ids of the current node's directly connected nodes.

This will create a directed graph of all the nodes in the system.

Here is a sample node .json config file:
```
{
  "id": 0,
  "queueDelay": 4,
  "directNeighbors": [1, 5]
},
{
  "id": 1,
  "queueDelay": 3,
  "directNeighbors": [0, 2, 4]
},
{
  "id": 2,
  "queueDelay": 5,
  "directNeighbors": [2, 3, 5]
},
{
  "id": 3,
  "queueDelay": 1,
  "directNeighbors": [1]
},
{
  "id": 4,
  "queueDelay": 0,
  "directNeighbors": [0, 3]
},
{
  "id": 5,
  "queueDelay": 2,
  "directNeighbors": [0, 1, 3, 4]
}
```

and here is a sample simulation .csv config file:
```
MSG,3,1,5
MSG,5,2,3
REP,7,315
MSG,8,4,5
MSG,8,0,3
REP,9,803
MSG,10,5,2
REP,15
END
```
