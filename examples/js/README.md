# JavaScrip Example

An JavaScript example application that reads JCAMP-DX files using libjdx.

## Prerequisites

### Node.js

Download and install [Node.js](https://nodejs.org/en/download/package-manager).

To install the dependencies for serving the application, in the `examples/js` directory run:

```
npm install
```

Note that this is merely required for serving HTML and JavaScript to the browser. All application logic runs in the browser.

### C++/Emscripten

For building the JavaScript library, follow the directions in [README.md](../../README.md).

You need to copy `libjdx.js` and `libjdx.wasm` to this example's `public` directory. If the resulting library is available in the `build-wasm/apps` directory, there is a convenience script that does that for you by running `npm run import-wasm`.

## Run example

To start the application, in the `examples/js` directory run:

```
npm start
```

This will start a web application that can be opened at https://localhost:3000.
