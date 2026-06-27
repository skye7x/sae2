const fs = require("fs");
const https = require("https");
require("dotenv").config();
const app = require("./app");
const sslOptions = {
  key: fs.readFileSync(process.env.SSL_KEY_PATH || "/cert/privkey.pem", "utf8"),
  cert: fs.readFileSync(process.env.SSL_CERT_PATH || "/cert/fullchain.pem", "utf8")
};
const PORT = process.env.PORT || 443;
const server = https.createServer(sslOptions, app);
server.listen(PORT, () => { console.log("HTTPS Server running on port " + PORT); });
module.exports = app;
