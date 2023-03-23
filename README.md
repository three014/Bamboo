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
