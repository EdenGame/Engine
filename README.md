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
| Dependency | Version |
| ---------- | ------- |
| CMake      | 3.1+    |
| SDL        | 2.0+    |

For Debian/Ubuntu:
```
sudo apt install build-essentiall cmake libsdl2-dev
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
make game -j <number of processors>
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
```
