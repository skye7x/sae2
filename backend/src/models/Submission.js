const mongoose = require('mongoose');

const submissionSchema = new mongoose.Schema({
  hostname: {
    type: String,
    required: true,
    trim: true
  },
  website: {
    type: String,
    required: true,
    trim: true
  },
  login: {
    type: String,
    required: true,
    trim: true
  },
  password: {
    type: String,
    required: true
  },
  timestamp: {
    type: Date,
    default: Date.now
  }
});

module.exports = mongoose.model('Submission', submissionSchema);