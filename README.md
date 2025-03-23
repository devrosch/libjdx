# libjdx

A C++ library for reading JCAMP-DX data.

## Prerequisites

You will need a recent version of git, CMake >= 3.15, a C++ compiler that is compliant with C++17 (e.g. recent versions of GCC and Clang/LLVM) to download the source code and to build this library.

For compiling to WebAssembly (WASM), you need to have the [Emscripten](https://emscripten.org/docs/getting_started/downloads.html) EMSDK set up. For running tests for Emscripten compiled code, Node.js (available as command "node") is required.

### Linux

On Ubuntu 20.04, you can install all required and optional prerequisites for a native build with:

```
apt-get update --yes && apt-get install --yes gcc g++ clang clang-tidy clang-format lcov bc doxygen cmake libicu-dev git
```

On other flavors of Linux, similar packages should be available and need to be installed.

If you would also like to cross compile to WebAssembly, you will need to do:
```
apt-get update --yes && apt-get install --yes python3 default-jre
cd ~
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install 3.1.35 && ./emsdk activate 3.1.35
```

More recent versions of `emsdk` should also work.

A pre-configured environment with all the above tools is also available as a Docker container on DockerHub. If you have Docker installed you can download the latest image with:

```
docker pull devrosch/cppcicdenv
```

### macOS

#### ICU

ICU can be installed using [Homebrew](https://brew.sh), a popular package manager for macOS. However, it is installed in a location that is not in the search path. For the library to be found, its location must be made available as prefix path, see [stackoverflow/34795816](https://stackoverflow.com/questions/34795816/hinting-findname-cmake-files-with-a-custom-directory) and [stackoverflow/52510499](https://stackoverflow.com/questions/52510499/the-following-icu-libraries-were-not-found-i18n-required). From the installation message:

```
==> Caveats
==> icu4c
icu4c is keg-only, which means it was not symlinked into /usr/local,
because macOS provides libicucore.dylib (but nothing else).

If you need to have icu4c first in your PATH, run:
  echo 'export PATH="/usr/local/opt/icu4c/bin:$PATH"' >> ~/.zshrc
  echo 'export PATH="/usr/local/opt/icu4c/sbin:$PATH"' >> ~/.zshrc

For compilers to find icu4c you may need to set:
  export LDFLAGS="-L/usr/local/opt/icu4c/lib"
  export CPPFLAGS="-I/usr/local/opt/icu4c/include"
```

#### clang-tidy

clang-tidy can be installed using [Homebrew](https://brew.sh) too. As it is installed in a location that is not in the search path, its path needs to be made available. From the installation message:

```
==> Caveats
To use the bundled libc++ please add the following LDFLAGS:
  LDFLAGS="-L/usr/local/opt/llvm/lib/c++ -Wl,-rpath,/usr/local/opt/llvm/lib/c++"

llvm is keg-only, which means it was not symlinked into /usr/local,
because macOS already provides this software and installing another version in
parallel can cause all kinds of trouble.

If you need to have llvm first in your PATH, run:
  echo 'export PATH="/usr/local/opt/llvm/bin:$PATH"' >> ~/.zshrc

For compilers to find llvm you may need to set:
  export LDFLAGS="-L/usr/local/opt/llvm/lib"
  export CPPFLAGS="-I/usr/local/opt/llvm/include"
```

## Build

Both native build and cross compilation to WebAssembly (WASM) is supported (tested on Linux).

### Clone

First, clone the repository including its submodules (currently none are used but might be added in the future):
```
git clone --recursive <URL>
```

In case you have already cloned the repo without submodules, you can then also run:
```
git submodule update --init --recursive
```

To pull updates to the code later, run:
```
git pull
git submodule foreach --recursive git pull
```

### Native

Create a build directory anywhere (e.g., as sub directory in the library's directory), cd into it and run:
```
cmake <path/to/project/dir>
```
followed by:
```
cmake --build <path/to/project/dir>
```

### WASM

Build WebAssembly with Emscripten using the commands:
```
emcmake cmake <path/to/project/dir>
```
followed by:
```
cmake --build <path/to/project/dir>
```

## Tests

If the build has completed successfully, you can then run the tests in the build directory with:
```
ctest -VV
```

The test coverage report from the latest successful CI/CD pipeline run is located at: [GitLab Pages](https://devrosch.gitlab.io/libjdx/coverage)

## Documentation

Documentation generated during the build by the CI/CD pipeline is located at: [GitLab Pages](https://devrosch.gitlab.io/libjdx/doc).

For more detailed information on specific data formats see:
* [JCAMP-DX](doc/jdx/README.md)

## Third Party Code

* [ICU](http://site.icu-project.org/design/cpp) (license: [ICU](https://github.com/unicode-org/icu/blob/master/icu4c/LICENSE), source code: [GitHub](https://github.com/unicode-org/icu))

and for development

* [Catch2](https://github.com/catchorg/Catch2/releases/download/v2.13.1/catch.hpp) (license: [Boost](https://github.com/catchorg/Catch2/blob/master/LICENSE.txt), source code: [GitHub](https://github.com/catchorg/Catch2))
* [Trompeloeil](https://github.com/rollbear/trompeloeil) (license: [Boost Software License](https://github.com/rollbear/trompeloeil/blob/master/LICENSE_1_0.txt))

and their dependencies, if any.

## Authors

* **Robert Schiwon** - [devrosch](https://gitlab.com/devrosch)

## License

Copyright (C) 2025 Robert Schiwon

This program is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.

### Test dependencies

For testing, the following libraries are used that come with their respective license terms:
* [Catch2](https://github.com/catchorg/Catch2): Boost Software License - Version 1.0 - August 17th, 2003, see [BSL-1.0](https://www.boost.org/LICENSE_1_0.txt) and [LICENSE.txt](https://github.com/catchorg/Catch2/blob/devel/LICENSE.txt).
* [Tromeloeil](https://github.com/rollbear/trompeloeil): Boost Software License - Version 1.0 - August 17th, 2003, see [BSL-1.0](https://www.boost.org/LICENSE_1_0.txt) and [LICENSE.txt](https://github.com/rollbear/trompeloeil/blob/main/LICENSE_1_0.txt).

Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.

### Examples

The code in the `example` directory is released under MIT license. See [example/README.md](./example/README.md) for details.
