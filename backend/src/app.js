const express = require("express");
require("dotenv").config();
const helmet = require("helmet");
const cors = require("cors");
const rateLimit = require("express-rate-limit");
const connectDB = require("./database/mongo");
const logger = require("./middleware/logger");
const errorHandler = require("./middleware/errorHandler");
const submissionRoutes = require("./routes/submissionRoutes");
connectDB();
const app = express();
app.use(helmet());
app.use(cors());
const limiter = rateLimit({
  windowMs: parseInt(process.env.RATE_LIMIT_WINDOW_MS) || 900000,
  max: parseInt(process.env.RATE_LIMIT_MAX_REQUESTS) || 100,
  message: { error: "Too many requests from this IP, please try again later." }
});
app.use(limiter);
app.use(express.json({ limit: "10mb" }));
app.use(express.urlencoded({ extended: true }));
app.use(logger.requestLogger);
app.use("/api/submissions", submissionRoutes);
app.get("/health", (req, res) => { res.status(200).json({ status: "OK", timestamp: new Date().toISOString() }); });
app.use(errorHandler);
module.exports = app;
