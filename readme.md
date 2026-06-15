
# Portify - A Markdown-to-Portfolio Generator Engine

Portify is an automated pipeline that converts structured Markdown into a fully responsive portfolio website. It allows developers to maintain and update their portfolios without writing frontend code, effectively bridging high-level UI design with low-level system programming.

## Program Flow and Logic

### Multi-Page Frontend (`frontend/`)

Portify now features a multi-page web application with three pages:

* **`frontend.html`** (Editor): Dual-pane layout with a Markdown editor and live preview iframe. Includes Run, Generate, and Download buttons.
* **`templates.html`** (Templates): 15 unique portfolio template designs displayed in a 2-column grid. Users select a template, which is stored in `localStorage` under the key `selected_template`.
* **`about.html`** (About): Developer profile cards for the Portify team.

**Theme System**: All three pages share a unified dark/light theme toggle persisted via `localStorage` under `portify-theme`. CSS custom properties (root variables) define the shared design tokens.

**Template Selection Flow**:
1. User selects a template on `templates.html` → saved to `localStorage.setItem('selected_template', id)`
2. On the Editor page, the template ID is read from `localStorage` and sent as `template` in the JSON body of the `/generate` POST request
3. The Flask server (`server.py`) extracts the `template` field and passes it as a command-line argument to the compiled C++ engine

### server.py

1. Acts as the bridge between the browser and the C++ engine
2. Receives Markdown input + template ID from the frontend
3. Stores Markdown in `input.md`
4. Executes the compiled C++ engine with the template ID: `./backend/main.exe <template_id>`
5. Returns the generated `output/index.html`
6. Displays it in the preview iframe

### main.cpp (Engine):

This is the core processing unit of Portify.

* Built using Object-Oriented Programming in C++
* Uses hierarchical inheritance with `Portfolio` as the base class and `Profile`, `Skills`, `Projects`, and `Contact` as derived classes.
* **Static member `templatePath`**: The base `Portfolio` class now declares a `static string templatePath` that holds the path to the selected template file. This is set dynamically from the command-line argument.
* **Command-line argument**: `main()` reads `argv[1]` as the template number, constructs the path `templates/<id>.html`, verifies the file exists, and sets `Portfolio::templatePath`.

**Core Functionality:**
* Extract structured content from Markdown (`input.md`)
* Map it into predefined sections of the selected template via string parsing
* Identifies errors if found while mapping
* Generate a complete `index.html` file

## Features

**Multi-Page Workspace**
Editor, Templates, and About pages with consistent design

**Dual-Pane Editor**
IDE-style workspace with real-time preview

**Adjustable Layout**
Draggable divider for flexible workspace

**15 Production Templates**
Bento Grid, Minimalist, Cyberpunk, Cinematic, Scrollytelling, Typography, Gamified, Horizontal, Split-Screen, Neumorphic, Timeline, Glow, Magazine, Glassmorphism, Terminal

**Theme Toggle**
Dark/Light mode with localStorage persistence across all pages

**State-Aware Controls**
* Run → preview in iframe
* Generate → full-page output
* Download → save portfolio.html

**Engine**
Supports structured sections:
* Profile
* Skills
* Projects
* Contact

**Error Handling System**
Detects invalid Markdown structure and engine failures, and notifies the user

## Tech Stack & Concepts

* **Object-Oriented Programming (C++)**
  * Encapsulation (Profile, Skills, Projects classes)
  * Inheritance (Portfolio base class)
  * Dynamic Polymorphism (Late Binding)
  * Static class members for shared state (templatePath)
* **File Handling**
  * `<fstream>` for reading/writing files
* **String Processing**
  * Custom parsing using std::string functions
* **Exception Handling**
  * Error signaling via exit codes from C++ engine to frontend
* **Full-Stack Integration**
  * HTML/CSS/JS (Frontend with multi-page support)
  * Python Flask (Server with CORS)
  * C++ (Processing Engine with dynamic template selection)
* **Browser APIs**
  * localStorage for theme and template selection persistence
  * Fetch API for async server communication

## Instructions to run

**1-** Download the project files

**2-** Compile main.cpp

     cd backend
     g++ main.cpp -o main.exe
     cd ..

**3-** Run python server.py

    python server.py

**4-** Open in browser

    http://127.0.0.1:8000/

**5-** Use the application

* Write your Markdown in the editor
* Go to Templates page to select a design (default is Template 1: Bento Box Grid)
* Click Run to preview with selected template
* Click Generate to open the final portfolio in a new tab
* Click Download to save the portfolio as an HTML file

**Manual C++ Testing** (without server):
```
./backend/main.exe <template_id>
```
Example: `./backend/main.exe 3` generates output/index.html using Template 3 (Cyberpunk).

## Authors

* **Batool Zafar**
* **Muhammad Hashim**
