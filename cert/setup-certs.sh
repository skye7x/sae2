#!/bin/bash

# Setup script for SSL certificates
echo "Setting up SSL certificates..."

# Create directory if it doesn't exist
mkdir -p /cert

# Make the generate script executable
chmod +x /cert/generate-certs.sh

echo "Certificate setup complete."