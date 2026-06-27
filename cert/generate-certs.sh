#!/bin/bash

# Generate self-signed certificate for development purposes
# This is a simplified script - in production, you would use proper certificates from Let's Encrypt or your CA

echo "Generating self-signed SSL certificate..."

# Create directory if it doesn't exist
mkdir -p /cert

# Generate private key
openssl genrsa -out /cert/privkey.pem 2048

# Generate certificate
openssl req -new -x509 -key /cert/privkey.pem -out /cert/fullchain.pem -days 365 -subj "/C=US/ST=State/L=City/O=Organization/CN=localhost"

echo "SSL certificates generated successfully!"
echo "Files created:"
echo "- /cert/privkey.pem (private key)"
echo "- /cert/fullchain.pem (certificate)"

# Set proper permissions
chmod 600 /cert/privkey.pem
chmod 644 /cert/fullchain.pem

echo "Permissions set."