const { body, validationResult } = require('express-validator');
const submissionService = require('../services/submissionService');

// Validation rules for submission data
const validateSubmission = [
  body('hostname')
    .notEmpty()
    .withMessage('Hostname is required')
    .isLength({ max: 255 })
    .withMessage('Hostname must be less than 255 characters'),
  body('website')
    .notEmpty()
    .withMessage('Website is required')
    .isLength({ max: 255 })
    .withMessage('Website must be less than 255 characters'),
  body('login')
    .notEmpty()
    .withMessage('Login is required')
    .isLength({ max: 100 })
    .withMessage('Login must be less than 100 characters'),
  body('password')
    .notEmpty()
    .withMessage('Password is required')
    .isLength({ min: 8 })
    .withMessage('Password must be at least 8 characters long')
];

// Create new submission
const createSubmission = async (req, res) => {
  try {
    // Validate input
    const errors = validationResult(req);
    if (!errors.isEmpty()) {
      return res.status(400).json({
        error: 'Validation failed',
        details: errors.array()
      });
    }

    const submissionData = req.body;
    
    // Create the submission
    const newSubmission = await submissionService.createSubmission(submissionData);

    res.status(201).json({
      message: 'Submission created successfully',
      data: newSubmission
    });
  } catch (error) {
    console.error('Error creating submission:', error);
    res.status(500).json({
      error: 'Failed to create submission'
    });
  }
};

// Get submissions grouped by hostname
const getSubmissionsByHostname = async (req, res) => {
  try {
    const submissions = await submissionService.getSubmissionsByHostname();
    
    res.json({
      data: submissions,
      count: submissions.length
    });
  } catch (error) {
    console.error('Error fetching submissions:', error);
    res.status(500).json({
      error: 'Failed to fetch submissions'
    });
  }
};

// Get all submissions with pagination
const getAllSubmissions = async (req, res) => {
  try {
    const page = parseInt(req.query.page) || 1;
    const limit = parseInt(req.query.limit) || 10;
    
    const result = await submissionService.getAllSubmissions(page, limit);
    
    res.json({
      data: result.submissions,
      pagination: result.pagination
    });
  } catch (error) {
    console.error('Error fetching all submissions:', error);
    res.status(500).json({
      error: 'Failed to fetch all submissions'
    });
  }
};

module.exports = {
  createSubmission,
  getSubmissionsByHostname,
  getAllSubmissions,
  validateSubmission
};