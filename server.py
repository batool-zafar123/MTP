from flask import Flask, request, send_file
from flask_cors import CORS
import subprocess

app = Flask(__name__)
CORS(app)

@app.route('/')
def home():
    return send_file('index.html')  # make sure index.html is in the same folder

@app.route('/generate', methods=['POST'])
def handle_generation():
    data = request.json
    markdown_text = data['markdown']
    
    with open('input.md', 'w') as file:
        file.write(markdown_text)
    
    # subprocess.run(['./generator_program'])
    
    return "Success", 200

if __name__ == '__main__':
    app.run(port=8000)