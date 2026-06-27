const errorHandler = (err, req, res, next) => {
  console.error(err.stack);

  // Log error using the logger module
  const logger = require('./logger');
  logger.error('Unhandled error', {
    error: err.message,
    stack: err.stack,
    url: req.url,
    method: req.method,
    timestamp: new Date().toISOString()
  });

  res.status(err.statusCode || 500).json({
    error: err.message || 'Internal Server Error',
    statusCode: err.statusCode || 500
  });
};

module.exports = errorHandler;
