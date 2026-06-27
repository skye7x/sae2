const mongoose = require('mongoose');
require('dotenv').config();

const connectDB = async () => {
  try {
    // NOTE: useNewUrlParser and useUnifiedTopology are removed in Mongoose 7+
    const conn = await mongoose.connect(
      process.env.MONGODB_URI || 'mongodb://localhost:27017/sae2'
    );

    console.log(`MongoDB Connected: ${conn.connection.host}`);
  } catch (error) {
    console.error('Database connection error:', error);
    process.exit(1);
  }
};

module.exports = connectDB;
