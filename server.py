from flask import Flask, request, send_file, send_from_directory
from flask_cors import CORS
import subprocess
import os

app = Flask(__name__)
CORS(app)

@app.route('/')
def home():
    # FIXED TYPO: Serve frontend.html instead of non-existent frontend.htm
    return send_file('frontend/frontend.html')

@app.route('/output/<path:filename>')
def serve_output(filename):
    """Serve files from the output directory"""
    return send_from_directory('output', filename)

@app.route('/generate', methods=['POST'])
def handle_generation():
    data = request.json
    markdown_text = data.get('markdown', '')
    template_id = data.get('template', '1') # Fallback to template 1
    
    # Save markdown to input.md
    with open('input.md', 'w', encoding='utf-8') as file:
        file.write(markdown_text)
    
    # Run dynamic C++ program with dynamic template identifier
    try:
        # Check running OS context (add .exe only if on windows context)
        exe_path = './backend/main.exe' if os.name == 'nt' else './backend/main'
        
        # Pass template_id as a command line string parameter
        subprocess.run([exe_path, str(template_id)], check=True)
        
        return {"status": "success", "message": f"Portfolio generated using Template {template_id}!"}, 200
    except FileNotFoundError:
        return {"status": "error", "message": "C++ program executable not found. Make sure to compile backend/main.cpp first!"}, 500
    except subprocess.CalledProcessError:
        return {
            "status": "error", 
            "message": "C++ program is saying that the structure of md is disturbed. Try reloading..."
        }, 500
    except Exception as e:
        return {"status": "error", "message": str(e)}, 500

if __name__ == '__main__':
    port = int(os.environ.get('PORT', 8000))
    app.run(host='0.0.0.0', port=port, debug=False)
