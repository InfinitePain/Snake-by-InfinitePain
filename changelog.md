# Changelog

All notable changes to "Snake by InfinitePain" will be documented in this file.

## [1.1.1] - 2023-04-07

### Added

- Configuration file location information in README
- Changelog file and link in README

### Changed

- Updated README to provide a better explanation of the game and its features

## [1.1.0] - 2023-04-07

### Fixed

- Resolved bug where snake position didn't reset
- Ensured proper handling of reverse directions

## [1.0.0] - 2023-04-05

### Added

- Timer to display game time
- Show player points and best points on screen
- Adjustable game speed implemented

### Changed

- Game loop now based on real time, not CPU speed
- Improved input handling with buffer for direction

## [0.9.1] - 2023-04-04

### Added

- Display winner with appropriate color in multiplayer

### Changed

- Fixed pause menu navigation issue with settings menu
- Added dynamic food amount resizing without game restart

## [0.9.0] - 2023-04-03

### Added

- A thread to manage food coordinates and drawing
- Introduced a thread for collision detection/handling

## [0.8.0] - 2023-04-02

### Changed

- Switched to standard linked lists
- Removed unused functions related to previous list implementation

### Added

- Functions for food handling (not yet included in game)

## [0.7.3] - 2023-04-01

### Fixed

- Resolved settings display bug (incorrect strings, multiple changes)

### Changed

- Updated info at the bottom to show new possible key uses

## [0.7.2] - 2023-03-31

### Changed

- Settings menu is mostly functional with some known issues
- Going back goes to the main menu instead of the previous menu
- New keys for saving settings are not printed
- Code cleanup and improvements to menu functions
- Fixed bug with player movement when starting new game

## [0.7.1] - 2023-03-29

### Fixed

- Bug fix for Linux config path

## [0.7.0] - 2023-03-29

### Changed

- Fully implemented settings menu (requires bug fixes)
- Removed game area offset, now using a static value

## [0.6.0] - 2023-03-27

### Changed

- Improved menu visuals with increased padding and wider display area

### Added

- Key information at the bottom of the menu

## [0.5.0] - 2023-03-27

### Changed

- Replaced PATH_MAX with dynamic allocation for better portability
- Simplified configuration file reading and writing using a for loop and a char array
- Added static values for screen size, allowing for adjustable game sizes and a bounding box display
- Refactored window functions to use an array for managing all windows
- Improved error messages for better clarity and debugging

## [0.4.0] - 2023-03-26

### Added

- Settings menu (incomplete but functional for testing purposes)
- Configurations are now stored in an array, new macros added for readability
- Config struct now includes a member for the path of the config file
- Added two new functions for easier and dynamic menu titles: print_menu and erase_menu
