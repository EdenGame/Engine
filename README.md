WIP.

## Compiling

Begin with downloading the repository.

**Clone** the repository (take note of the recursive option):
```
git clone https://github.com/EdenGame/Engine --recursive
```
or, if you've already cloned it but not recursively:
```
git submodule update --recursive --remote
```

~~Or, **download** the source as a ZIP directly from GitHub.~~ If you've done this, you've made a mistake. This project depends upon a Git submodule to fetch the libraries, meaning that their must be a proper Git structure in order to update them. In the future there will also be a Python script to make this option once again viable.

Now that you have the source, you can begin building. Choose a platform from below and jump to the section related to it.

* <a href="#linux">Linux</a>
* Windows (Coming Soon!)
* Mac (Coming Soon!)

### <span id="linux">Linux</span>

##### Dependencies
| Dependency | Version | Commentary                      |
| ---------- | ------- | ------------------------------- |
| GCC        | 8.0+    | Can be replaced with Clang 5.0+ |
| CMake      | 3.8+    |                                 |
| SDL        | 2.0+    |                                 |

For Debian/Ubuntu:
```
sudo apt install build-essentiall cmake libsdl2-dev
```

##### Compiler

Some of the source of this project utilizes C++17 syntax and, most importantly, the new filesystem library, requiring a minimum of GCC 8 and Clang 5. To check what version you currently have enabled for CMake to use, run `c++ -v`. If you do not already have a new enough version, you must first install it.

For Debian/Ubuntu:
```
sudo apt install gcc-8 g++-8
# or
sudo apt install clang-5.0 # or newer
```

However, your system will still continue to use the old version of the compiler. You must either change this globally for your entire system via `update-alternatives` or locally via environment variables.

Globally:
```
sudo update-alternatives --install /usr/bin/c++ c++ /usr/bin/g++-8 100
# or
sudo update-alternatives --install /usr/bin/c++ c++ /usr/bin/clang-5.0 100
# undo (optional):
sudo update-alternatives --remove c++ /usr/bin/g++-8
# or
sudo update-alternatives --remove c++ /usr/bin/clang-5.0
# change current version in use
sudo update-alternatives --config c++
```

Locally
```
export CXX=/usr/bin/g++-8
# or
export CXX=/usr/bin/clang-5.0
# undo (optional):
unset CXX
```

Now that the proper compiler is installed the project can be built. **Warning**: If `cmake` has already been run once, changing the compiler will have no effect. First the CMake cache must be cleared before `cmake` is run again:
```
rm CMakeCache.txt
cmake .. # executed from <project source>/build
```

##### Build

Create and switch to the `build` directory:
```
mkdir build && cd build
```

Build with CMake and make:
```
cmake ..
make game
# or, to specify a number of processors
make game -j<number of processors>
# or, to use the number in your computer
make game -j$(nproc)
```

Run it:
```
./ProjectEden
```

##### Build Unit Tests

Switch to the `build` directory and build with make:
```
cd build
make tests
./UnitTests
```
