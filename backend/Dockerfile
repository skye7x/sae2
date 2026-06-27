FROM node:18-alpine

WORKDIR /app

COPY package*.json ./
RUN npm ci --only=production

COPY . .

# Create logs directory so Winston can write to it on startup
RUN mkdir -p logs

EXPOSE 443

CMD ["npm", "start"]
