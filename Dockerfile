# Use python base image
FROM python:3.9-slim

# Install system dependencies (build-essential contains g++)
RUN apt-get update && apt-get install -y \
    build-essential \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy requirement files and install
COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt

# Copy the rest of the application files
COPY . .

# Compile C++ backend
RUN g++ backend/main.cpp -o backend/main

# Make output directory writable
RUN chmod -R 777 output/

# Expose Hugging Face default port
EXPOSE 7860

# Set dynamic port environment variable for Hugging Face
ENV PORT=7860

# Run python server
CMD ["python", "server.py"]
