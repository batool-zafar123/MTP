from flask import Flask, request, send_file, send_from_directory
from flask_cors import CORS
import subprocess
import os

app = Flask(__name__)
CORS(app)

@app.route('/')
def home():
    return send_file('index.html')

@app.route('/output/<path:filename>')
def serve_output(filename):
    """Serve files from the output directory"""
    return send_from_directory('output', filename)

@app.route('/generate', methods=['POST'])
def handle_generation():
    data = request.json
    markdown_text = data['markdown']
    
    # Save markdown to input.md
    with open('input.md', 'w', encoding='utf-8') as file:
        file.write(markdown_text)
    
    # Run your C++ program
    try:
        # subprocess.run(['.main.exe'], check=True)
        subprocess.run(['./backend/main.exe'], check=True)
        
        return {"status": "success", "message": "Portfolio generated!"}, 200
    except FileNotFoundError:
        return {"status": "error", "message": "C++ program not found"}, 500
    except Exception as e:
        return {"status": "error", "message": str(e)}, 500

if __name__ == '__main__':
    app.run(port=8000, debug=False)