
#### To build and run

```
python3 -m venv .venv
source .venv/bin/activate
python -m pip install --upgrade pip setuptools wheel
pip install -r requirements.txt

gunicorn dash_trader:server --bind 0.0.0.0:8050
```
