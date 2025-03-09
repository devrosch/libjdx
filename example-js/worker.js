/* Copyright (C) 2025 Robert Schiwon
 *
 * This file is part of libjdx.
 *
 * libjdx is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * libjdx is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libjdx. If not, see <https://www.gnu.org/licenses/>.
 */

onmessage = function(e) {
    console.log('Module:');
    console.log(Module);
    
    const f = e.data[0];

    const dir = '/work';
    const dirExists = FS.analyzePath(dir, false).exists;
    if (!dirExists) {
        FS.mkdir(dir);
    }
    FS.mount(WORKERFS, { files: [f] }, dir);

    let sfr = new Module.StubFileParser();
    //console.log('JS (Service Worker): "/work/' + f.name + '" found: ' + sfr.isResponsible('/work/' + f.name));
    console.log('JS (Service Worker): "/work/' + f.name + '" found: ' + sfr.isRecognized('/work/' + f.name));
    sfr.delete();

    FS.unmount(dir);
}

self.importScripts('libsf.js');
