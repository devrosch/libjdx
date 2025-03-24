// Note: For clarity the below examples use simplified error handling.
// In a production application you should ensure that file descriptors get closed and readers get freed in all potential error cases.

// ------------------------------
// Initialize library and worker.
// ------------------------------

// Load and initialize package.
self.importScripts('libjdx.js');
console.log('Worker: initialized libjdx');

const WORKINGDIR = '/work';

// -----------------
// Process messages.
// -----------------

// Process data sent by main thread.
onmessage = (e) => {
  const { command, file } = e.data;
  console.log(`Worker: received from main script: ${JSON.stringify(e.data)}`);

  switch (command) {
    case 'read':
      read(file)
      break;
    default:
      console.log(`Error, command not recognized: ${JSON.stringify(command)}. Data: ${JSON.stringify(data)}.`);
  }
};

const read = async (file) => {
  const fileName = file.name;
  console.log(`file name: ${fileName}`);
  const filePath = mountFile(fileName, file);
  const reader = getReader(filePath, file);
  if (!reader) {
    unmountFile(filePath);
    return;
  }

  // Send file name to main thread for displaying.
  postMessage({ command: 'showName', data: fileName });
  // Read contents starting with the root node ''.
  readNodes(reader, '/');

  unmountFile(filePath);
  reader.delete();
}

// Iterate through all nodes depth first.
const readNodes = (reader, searchPath) => {
  const path = searchPath || '/';
  // node is value object => no delete()
  const node = reader.read(path);

  const nodeData = nodeToJson(node);
  // Send node data to main thread for displaying.
  postMessage({ command: 'showNodeContent', data: { path, node: nodeData } });

  // Read child nodes.
  for (let i = 0; i < nodeData.childNodeNames.length; i += 1) {
    const childPath = path === '/' ? `/${i}` : path + `/${i}`;
    readNodes(reader, childPath);
  }
}

const getReader = (filePath) => {
  // Check if file is expected type.
  const scanner = new Module.JdxScanner();
  const isRecognized = scanner.isRecognized(filePath);
  // A File is a Blob and can be used directly.
  // Processing a Blob is only possible in a worker, not in the main thread.
  if (!isRecognized) {
    console.log(`Unrecognized file format: : ${filePath}`);
    scanner.delete();
    return;
  }
  console.log(`Recognized file format: ${filePath}`);

  // Get the suitable reader for reading file contents.
  const reader = scanner.getConverter(filePath);
  scanner.delete();
  return reader;
}


/**
 * Mounts a file in Emscripten's filesystem's "work" directory.
 * @param fileName The file name.
 * @param blob A Blob containing the file data.
 * 
 * @returns The file path at which the file has been mounted.
 */
const mountFile = (fileName, blob) => {
  const workingDirExists = FS.analyzePath(WORKINGDIR, false).exists;
  if (!workingDirExists) {
    FS.mkdir(WORKINGDIR);
  }
  FS.mount(
    WORKERFS,
    {
      blobs: [{ name: fileName, data: blob }],
    },
    WORKINGDIR,
  );
  return `${WORKINGDIR}/${fileName}`;
};

/**
 * Unmounts a file from the filesystem's "work" directory.
 * @param filePath The file path.
 */
const unmountFile = (filePath) => {
  const fileExists = FS.analyzePath(filePath, false).exists;
  if (fileExists) {
    FS.unmount(WORKINGDIR);
  }
};

/**
 * Maps node data to pure JSON.
 * @param node The node corresponding to the URL.
 * @returns WorkerNodeData JSON.
 */
const nodeToJson = (node) => {
  const json = {};

  // parameters
  const params = node.parameters;
  const paramsSize = params.size();
  const jsonParameters = [];
  for (let index = 0; index < paramsSize; index += 1) {
    const keyValuePair = params.get(index);
    const key = keyValuePair.key;
    const value = keyValuePair.value;
    jsonParameters.push({ key, value });
  }
  json.parameters = jsonParameters;
  params.delete();

  // data
  const data = node.data;
  const dataSize = data.size();
  const jsonData = [];
  for (let index = 0; index < dataSize; index += 1) {
    const point = data.get(index);
    const x = point.x;
    const y = point.y;
    jsonData.push({ x, y });
  }
  json.data = jsonData;
  data.delete();

  // metadata
  const metadata = node.metadata;
  const metadataKeys = metadata.keys();
  const jsonMetadata = {};
  for (let i = 0; i < metadataKeys.size(); i += 1) {
    const key = metadataKeys.get(i);
    const value = metadata.get(key);
    jsonMetadata[key] = value;
  }
  json.metadata = jsonMetadata;
  metadataKeys.delete();
  metadata.delete();

  // table
  const jsonTable = { columnNames: [], rows: [] };

  const table = node.table;
  const tableColumnNames = table.columnNames;
  const peakColumnCount = tableColumnNames.size();
  for (let index = 0; index < peakColumnCount; index += 1) {
    const columnKeyValuePair = tableColumnNames.get(index);
    jsonTable.columnNames.push({
      key: columnKeyValuePair.first,
      value: columnKeyValuePair.second,
    });
    // columnKeyValuePair is value object => no delete()
  }

  const rows = table.rows;
  const rowCount = rows.size();
  for (let rowIndex = 0; rowIndex < rowCount; rowIndex += 1) {
    const jsonRow = {};
    const row = rows.get(rowIndex);
    for (const column of jsonTable.columnNames) {
      const columnKey = column.key;
      const peakValue = row.get(columnKey);
      jsonRow[columnKey] = peakValue;
    }
    jsonTable.rows.push(jsonRow);
    row.delete();
  }
  json.table = jsonTable;
  rows.delete();

  // child node names
  const childNodeNames = node.childNodeNames;
  const childNodesSize = childNodeNames.size();
  const jsonChildNodes = [];
  for (let index = 0; index < childNodesSize; index += 1) {
    const childNodeName = childNodeNames.get(index);
    jsonChildNodes.push(childNodeName);
  }
  json.childNodeNames = jsonChildNodes;
  childNodeNames.delete();

  return json;
};
