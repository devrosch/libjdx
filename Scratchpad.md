# Emscripten

## Compile and serve

Build WebAssembly with Emscripten using the following commands:
```
emcmake cmake <path/to/project/dir>
```
followed by:
```
make
```
or
```
cmake --build . --verbose
```

For running tests for Emscripten compiled code, Node.js (available as command "node") is required. Run:

```
ctest -VV
```

see: [Emscripten](https://emscripten.org/docs/getting_started/downloads.html)
see: [Stack Overflow](https://stackoverflow.com/questions/15724357/using-boost-with-emscripten)

## Docker

```
docker run --rm -v $(pwd):/src -u $(id -u):$(id -g) emscripten/emsdk emcc -std=c++17 USE_BOOST_HEADERS=1 -Iinclude src/binary_reader.cpp -o binary_reader.js
```

```
docker run --rm -v $(pwd):/src -u $(id -u):$(id -g) emscripten/emsdk emcc -std=c++17 -s USE_ICU=1 -Iinclude src/binary_reader.cpp apps/main.cpp -o binary_reader.html
```

```
python -m SimpleHTTPServer
```

```
docker run --rm -v $(pwd):/build -p 8080:8080 -u $(id -u):$(id -g) emscripten/emsdk emrun --port=8080 --no_browser /build/main.html
```

## Compile and serve manually

em++ --bind -lworkerfs.js -sFORCE_FILESYSTEM=1 -sWASM=1 -Wl,--whole-archive ./src/libsciwrap.a -Wl,--no-whole-archive -sDISABLE_EXCEPTION_CATCHING=0 ../apps/Main.cpp -o ./apps/sciwrap_main.js

python -m SimpleHTTPServer 8000

## Misc

Avoid "undefined symbol" errors, see: https://stackoverflow.com/a/37869647
```
target_link_options(stub_test PRIVATE "--bind")
```
