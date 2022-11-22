@echo on

set VENV_DIR=.\python-env

git submodule update --init --recursive

python -m venv %VENV_DIR%
call %VENV_DIR%\Scripts\activate.bat && python -m pip install --upgrade pip && python -m pip install -r %IDF_PATH%\requirements.txt
