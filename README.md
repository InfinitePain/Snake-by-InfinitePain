# Snake by InfinitePain

Snake by InfinitePain is a terminal-based snake game that features single player and multiplayer modes, as well as a settings menu to customize various aspects of the game. Developed as a university assignment, Snake by InfinitePain has grown into a more comprehensive project that aims to provide a fun and enjoyable gaming experience.

## Features

- Single player mode
- Multiplayer mode
- Customizable settings
- Display of best score, current score, and game time

## Prerequisites

- ncurses library

## Installation

Windows:

Download the .exe installer from the [GitHub Releases](https://github.com/InfinitePain/snake-by-infinitepain/releases) page and follow the installation instructions.

Arch Linux:

A PKGBUILD file is provided for Arch Linux users. You can use this file to build and install the game. Here's how:

1. Download the PKGBUILD file using the following command:

    ```bash
    curl -O https://raw.githubusercontent.com/InfinitePain/Snake-by-InfinitePain/main/Installers/arch_linux/PKGBUILD
    ```

2. Build and install the package with the following commands:

    ```bash
    makepkg -si
    ```

Please note that you need to have `base-devel` and `git` installed on your system to build the package.

## Configuration File

The game stores its configuration settings in a file that is created in the following locations:

Windows: %APPDATA%\Snake-by-InfinitePain\configurations.txt

Linux: ~/.config/Snake-by-InfinitePain/configurations.txt

macOS: ~/Library/Application Support/Snake-by-InfinitePain/configurations.txt

## License

This project is licensed under the Apache License 2.0. See the [LICENSE.txt](LICENSE.txt) file for details.

## Third-Party Libraries

This project uses the following third-party libraries:

- pthread (POSIX License)
- ncurses (MIT License)
- menu (MIT License, part of ncurses)

The full license texts for these libraries can be found in the respective subfolders under the [external](external) folder included with this project.

## GCC Runtime Library Exception

This project links the GCC runtime libraries statically, as permitted by the GCC Runtime Library Exception. For more information about the exception, please refer to the [GCC Runtime Library Exception](https://www.gnu.org/licenses/gcc-exception-3.1.en.html) provided by the Free Software Foundation.

## Changelog

For a detailed list of changes in each version, please refer to the [changelog](changelog.md).

## Distribution

Snake by InfinitePain will be distributed as an .exe installer for Windows and as a PKGBUILD for Arch Linux. The goal is to make Snake by InfinitePain easily installable on a wide range of platforms. Releases and source code are available on the [GitHub repository](https://github.com/InfinitePain/snake-by-infinitepain).
