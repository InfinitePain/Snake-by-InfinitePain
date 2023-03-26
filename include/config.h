/*
 ============================================================================
 Name        : config.h
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Header file for Snake-by-InfinitePain
 ============================================================================
 */
#ifndef CONFIG_H_
#define CONFIG_H_

#include <stdbool.h>
#include <limits.h>

#define MOVE_UP		0
#define MOVE_LEFT	1
#define MOVE_RIGHT	2
#define MOVE_DOWN	3

//Colors
#define PLAYER_1_COLOR 0
#define PLAYER_2_COLOR 1
#define WALL_COLOR 2
#define BACKGROUND_COLOR 3
#define FOOD_COLOR 4
#define TEXT_1_COLOR 5
#define TEXT_2_COLOR 6

//Keyboard
#define PLAYER_1_UP 7
#define PLAYER_1_LEFT 8
#define PLAYER_1_RIGHT 9
#define PLAYER_1_DOWN 10
#define PLAYER_2_UP 11
#define PLAYER_2_LEFT 12
#define PLAYER_2_RIGHT 13
#define PLAYER_2_DOWN 14

//Game Area
#define SCREEN_OFFSET_X 15
#define SCREEN_OFFSET_Y 16

//Game
#define PLAYER_1_POINT 17
#define PLAYER_2_POINT 18
#define SNAKE_LENGTH 19

//TODO Static width and height
//#define GAME_AREA_WIDTH 
//#define GAME_AREA_HEIGHT 

typedef struct Config {
	bool is_configurable;
	char config_path[PATH_MAX];
	int configs[20];
} Config;

void config_info();

void get_config_path(Config* pConfig);

Config* create_config();
void write_config(Config* pConfig);
void init_config_default(Config* config);
Config* read_config();
void delete_config(Config* pConfig);

#endif /* CONFIG_H_ */
