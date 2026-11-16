import json
import os
import subprocess
import tempfile
from pathlib import Path
from flask import Flask, jsonify, render_template, request

BASE = Path(__file__).resolve().parent
EXECUTABLE = BASE / ('mini_lex.exe' if os.name == 'nt' else 'mini_lex')
SPECS = {'arithmetic': BASE / 'specs' / 'arithmetic.lex', 'clike': BASE / 'specs' / 'clike.lex'}
SAMPLES = {
    'arithmetic': (BASE / 'samples' / 'expression.txt').read_text(encoding='utf-8'),
    'clike': (BASE / 'samples' / 'program.c.txt').read_text(encoding='utf-8'),
}
app = Flask(__name__)
app.config['MAX_CONTENT_LENGTH'] = 64 * 1024

@app.get('/')
def home():
    return render_template('index.html', samples=SAMPLES)

@app.get('/api/health')
def health():
    return jsonify(status='ok', engine_ready=EXECUTABLE.is_file())

@app.post('/api/analyze')
def analyze():
    payload = request.get_json(silent=True)
    if not isinstance(payload, dict):
        return jsonify(error='Send a JSON object.'), 400
    language = payload.get('language')
    source = payload.get('source')
    if language not in SPECS:
        return jsonify(error='Select Arithmetic or C-like.'), 400
    if not isinstance(source, str):
        return jsonify(error='Source must be text.'), 400
    if len(source.encode('utf-8')) > 32_000:
        return jsonify(error='Source is too large (32 KB maximum).'), 413
    if not EXECUTABLE.is_file():
        return jsonify(error='Lexer executable missing. Run build.sh first.'), 503
    filename = None
    try:
        with tempfile.NamedTemporaryFile(mode='w', suffix='.txt', encoding='utf-8', delete=False) as f:
            filename = f.name
            f.write(source)
        result = subprocess.run(
            [str(EXECUTABLE), str(SPECS[language]), filename, '--json'],
            capture_output=True, text=True, timeout=8, cwd=str(BASE), check=False
        )
        if result.returncode not in (0, 2):
            return jsonify(error='Lexer failed.', detail=result.stderr.strip()[:500]), 422
        try:
            return jsonify(json.loads(result.stdout))
        except json.JSONDecodeError:
            return jsonify(error='Invalid lexer response.'), 500
    except subprocess.TimeoutExpired:
        return jsonify(error='Analysis timed out.'), 504
    except OSError:
        return jsonify(error='Could not run lexer.'), 500
    finally:
        if filename:
            try:
                os.unlink(filename)
            except OSError:
                pass

if __name__ == '__main__':
    app.run(debug=True, port=int(os.environ.get('PORT', 5000)))
