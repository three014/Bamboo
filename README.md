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

### Nodes: 
Nodes are configured in _json_ formatting.

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
Any json file that can be parsed through [jsonlint](https://jsonlint.com)
should be parsable through RNS, but if any of the node fields are 
missing or incomplete, RNS will yield an error.

Empty arrays are allowed for the directNeighbors field, but
and node id that is mentioned in the config should be defined as well.


### Simulation:
The simulation file uses csv formatting.

Each line is a new instruction, with the following entries in this order:
- Instruction command - Required
  - "MSG" - New message to send from one node to another
  - "REP" - Report on a particular message or all of the messages sent so far
  - "END" - End the simulation
- Timestep - Required, tells the program how many units of time from the starting time
  to wait to execute this instruction
  - The instructions do not necessarily have to be in the order of their timestep, for
    the RNS parser will sort all the instructions by timestep
  - For instructions that have the same timestep, there are no guarantees on the ordering
    that the program executes those instructions
- Start Node (for MSG)/MSG Instruction (for REP) - Required for MSG instructions, optional 
  for REP instructions if the user wants to report on a specific message
  - For MSG instructions, this is the node that the message originates from and is the node id
    for that particular node
  - For REP instructions, this is the 3 numbers of a MSG instruction concatenated with a '-' in
    between each number
- End Node - Required for MSG instructions
  - Just like with the Start Node, this is the node id for the destination node


Here is a sample simulation .csv config file:
```
MSG,3,1,5
MSG,5,2,3
REP,7,3-1-5
MSG,8,4,5
MSG,8,0,3
REP,9,8-0-3
MSG,10,5,2
REP,15
END,20
```
