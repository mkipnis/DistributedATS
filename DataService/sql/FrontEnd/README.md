python3 -m venv .venv
source .venv/bin/activate
python -m pip install --upgrade pip setuptools wheel
pip install -r requirements.txt

gunicorn DataServiceFrontEnd:server --bind 0.0.0.0:8050
