/* MIT License
 *
 * Copyright (c) 2025 Robert Schiwon
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// Note: For clarity the below examples use simplified error handling.
// In a production application you should ensure that file descriptors get closed and readers get freed in all potential error cases.

// -----------------------------
// Initialize UI and web worker.
// -----------------------------

// Initialize display.
clearDisplay('fileInput');
const worker = new Worker(new URL("worker.js", import.meta.url));

// ------------------------------------------------
// Lazily load file in worker and display contents.
// ------------------------------------------------

window.onFileSelected = async function (input) {
  const selectedFiles = input.files;
  if (selectedFiles === null || typeof selectedFiles === 'undefined') {
    return;
  }
  clearDisplay();
  const file = selectedFiles[0];
  // Send file to worker for processing.
  worker.postMessage({ command: 'read', file });
};

// ----------------------------
// Process data sent by worker.
// ----------------------------

worker.onmessage = (e) => {
  console.log(`Message received from worker: ${JSON.stringify(e.data)}`);

  const { command, data } = e.data;

  switch (command) {
    case 'showName':
      showName(data);
      break;
    case 'showNodeContent':
      const { path, node } = data;
      showNodeContent(path, node);
      break;
    default:
      console.log(`Error, command not recognized: ${JSON.stringify(command)}. Data: ${JSON.stringify(data)}.`);
  }
};

// ------------------
// Utility functions.
// ------------------

function clearDisplay(...ids) {
  // Clear file inputs.
  for (let id of ids) {
    const fileinputEl = document.getElementById(id);
    fileinputEl.value = '';
  }
  // Clear file name.
  const fileContentEl = document.getElementById('fileContent');
  fileContentEl.value = '';
  // Clear textarea.
  const fileNameEl = document.getElementById('fileName');
  fileNameEl.textContent = '';
}

function showName(fileName) {
  const fileNameEl = document.getElementById('fileName');
  fileNameEl.textContent = fileName;
}

function showNodeContent(path, node) {
  const fileNameEl = document.getElementById('fileContent');
  fileNameEl.value += `Node path: "${path}"\n`;
  fileNameEl.value += `name: ${JSON.stringify(node.name)}\n`;
  fileNameEl.value += `parameters: ${JSON.stringify(node.parameters)}\n`;
  fileNameEl.value += `data: ${JSON.stringify(node.data)}\n`;
  fileNameEl.value += `metadata: ${JSON.stringify(node.metadata)}\n`;
  fileNameEl.value += `table: ${JSON.stringify(node.table)}\n`;
  fileNameEl.value += `childNodeNames: ${JSON.stringify(node.childNodeNames)}\n\n`;
}
