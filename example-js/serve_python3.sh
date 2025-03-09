#!/usr/bin/env python3

# Copyright (C) 2025 Robert Schiwon
#
# This file is part of libjdx.
#
# libjdx is free software: you can redistribute it and/or modify it under the
# terms of the GNU Lesser General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option) any
# later version.
#
# libjdx is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
# details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with libjdx. If not, see <https://www.gnu.org/licenses/>.

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