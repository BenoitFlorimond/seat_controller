# Seat controller
## Overview
This project is intended to control the position of a gaming seat. Two motors can move the seat.
An ESP32 receive orders by wifi ap and control both motors to reach the position requested.
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
