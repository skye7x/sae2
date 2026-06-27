 REPLACE
const express = require('express');
const fs = require('fs');
const https = require('https');
const path = require('path');
require('dotenv').config();

// Import the main app
const app = require('./app');

// Load SSL certificates
const sslOptions = {
  key: fs.readFileSync(path.join(process.cwd(), process.env.SSL_KEY_PATH || '/cert/privkey.pem'), 'utf8'),
  cert: fs.readFileSync(path.join(process.cwd(), process.env.SSL_CERT_PATH || '/cert/fullchain.pem'), 'utf8')
};

const PORT = process.env.PORT || 443;

// Create HTTPS server
const server = https.createServer(sslOptions, app);

// Start server
server.listen(PORT, () => {
  console.log(`HTTPS Server running on port ${PORT}`);
});

module.exports = app;