import express from 'express';

const app = express();
// Serve static content from 'public' directory.
app.use(express.static('public'));
app.listen(3000);

console.log('Serving on http://localhost:3000/');
