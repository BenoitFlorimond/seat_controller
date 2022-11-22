#!/bin/sh

export IDF_PATH=$(pwd)/esp-idf
export IDF_TOOLS_PATH=${HOME}/.espressif

git submodule update --init --recursive

sudo apt-get install cmake ninja-build

python3 ${IDF_PATH}/tools/idf_tools.py install --targets=esp32

PYTHON_ENV="./python-env"

python3 -m venv "${PYTHON_ENV}"
. "${PYTHON_ENV}/bin/activate"
python -m pip install --upgrade pip 
python -m pip install -r "${IDF_PATH}/requirements.txt"