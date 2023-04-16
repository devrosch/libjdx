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
