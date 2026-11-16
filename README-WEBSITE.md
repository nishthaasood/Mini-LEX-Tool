# Mini LEX Web — Black & Grey

A web interface for the original C++17 Mini LEX lexical analyzer. The C++ implementation remains the tokenization engine. Flask sends source code to the compiled executable using a temporary input file and reads JSON output.

## Features
- Arithmetic and C-like `.lex` specifications
- Code editor, file upload, sample code, token table, token summary and diagnostics
- Line/column positions and longest-prefix token matching from the original C++ lexer
- Dark monochrome responsive design

## Run locally (Windows PowerShell)
Install Python 3.10+ and a C++17 compiler (`g++`).

```powershell
python -m venv .venv
.\.venv\Scripts\Activate.ps1
pip install -r requirements.txt
g++ -std=c++17 -O2 -Wall main.cpp -o mini_lex.exe
```

The backend automatically selects `mini_lex.exe` on Windows and `mini_lex` on Linux/macOS.

Then run:

```powershell
python app.py
```

Open http://127.0.0.1:5000

## Run locally (Linux/macOS)
```bash
python3 -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
bash build.sh
python app.py
```

## Deploy to Render
1. Push the new website files into your existing `nishthaasood/Mini-LEX-Tool` GitHub repository (do not delete the original files).
2. Visit https://dashboard.render.com/ and choose **New > Blueprint**.
3. Connect GitHub and select `nishthaasood/Mini-LEX-Tool`.
4. Render detects `render.yaml`. Review the service and deploy.
5. Wait for build and deployment, then open the Render-provided URL.

Alternatively choose **New > Web Service**, connect the same repo, select Python runtime, and use:
- Build command: `bash build.sh && pip install -r requirements.txt`
- Start command: `gunicorn --bind 0.0.0.0:$PORT --workers 2 --timeout 30 app:app`

The free plan may sleep when idle. Render's available plans and terms can change.

## Existing repository push
From the existing local repository root, copy these files in (preserving `.git`) and run:

```bash
git status
git add .
git commit -m "Add black-grey Mini LEX web app"
git push origin main
```

If you have no local repo folder, clone first, then copy the contents of this ZIP into the cloned folder:

```bash
git clone https://github.com/nishthaasood/Mini-LEX-Tool.git
cd Mini-LEX-Tool
```

## Security notes
- Only bundled lexer specifications can be selected.
- The input size is limited to 32 KB and lexer execution has an 8-second timeout.
- Untrusted code is analyzed as text; the application does not compile or execute user-submitted source code.
- Suitable for educational demos; for public high-volume use, add rate limiting and stronger process isolation.
