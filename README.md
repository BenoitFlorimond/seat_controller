# ESP32 project template
## Overview
This is a template project to easily start a new project on ESP32.
This template is based on esp-idf version 4.2.2 which is the latest stable version to this day.
>***This section need to be updated when starting a new project***
## Prerequisites
 - [VSCode](https://code.visualstudio.com/download)
 - Recommended VSCode extensions are specified in `.vscode/.estensions.json` file. They will be automatically proposed by VS Code.
 - [git](https://git-scm.com/downloads) / [git for windows](https://gitforwindows.org/)
 - [Python 3.7+](https://www.python.org/downloads/)
## Setting up the environment
1. Clone this repository
    - `git clone [...]`
2. Upade submodules
    - `git submodule update --init -- recursive`
3. Open VSCode, and open the repository folder you just cloned
4. Launch the VS Code task `Run requirements`
## Building/debugging the project
The project come with needed VS Code tasks:
- `Build`: build the whole project
- `Flash`: flash the binary on the board
- `Clean`: clean applicative built objects
- `Clean all`: full clean all objects
- `Monitor`: monitor the log output
- `Size`: get the size of the built app
- `Erase`: erase the device flash
- `Menu config`: launch the configuration tool
- `Run requirements`: install all required packages needed by esp-idf
