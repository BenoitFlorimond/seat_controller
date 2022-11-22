PYTHON_ENV="./python-env"

python3 -m venv "${PYTHON_ENV}"
source "${PYTHON_ENV}/bin/activate"

source ${IDF_PATH}/export.sh