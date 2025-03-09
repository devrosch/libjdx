#!/usr/bin/env python

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

# see: https://curiousprog.com/2018/10/08/serving-webassembly-files-with-a-development-web-server/

import BaseHTTPServer, SimpleHTTPServer

port=8000
print "Running on port %d" % port

SimpleHTTPServer.SimpleHTTPRequestHandler.extensions_map['.wasm'] =    'application/wasm' 
httpd = BaseHTTPServer.HTTPServer(('localhost', port),
    SimpleHTTPServer.SimpleHTTPRequestHandler)

print "Mapping \".wasm\" to \"%s\"" % SimpleHTTPServer.SimpleHTTPRequestHandler.extensions_map['.wasm']
httpd.serve_forever()
