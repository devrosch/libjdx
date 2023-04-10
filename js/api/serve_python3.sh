#!/usr/bin/env python3

# see: https://gist.github.com/prideout/09af26cef84eef3e06a1e3f20a499a48
# https://www.reddit.com/r/workingsolution/comments/xrvx5w/python_wasm_python_http_server_with_wasm_support/

import http.server
import socketserver

port=8000
print("Running on port", port)

http.server.SimpleHTTPRequestHandler.extensions_map.update({
    '.wasm': 'application/wasm',
})

socketserver.TCPServer.allow_reuse_address = True
with socketserver.TCPServer(("", port), http.server.SimpleHTTPRequestHandler) as httpd:
    httpd.allow_reuse_address = True
    httpd.serve_forever()