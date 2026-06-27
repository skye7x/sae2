const express = require('express');
const router = express.Router();
const { createSubmission, getSubmissionsByHostname, getAllSubmissions, validateSubmission } = require('../controllers/submissionController');

// POST /api/submissions - Create a new submission
router.post('/', validateSubmission, createSubmission);

// GET /api/submissions/grouped - Get submissions grouped by hostname
router.get('/grouped', getSubmissionsByHostname);

// GET /api/submissions - Get all submissions with pagination
router.get('/', getAllSubmissions);

module.exports = router;