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

#define MOVE_UP		0
#define MOVE_LEFT	1
#define MOVE_RIGHT	2
#define MOVE_DOWN	3

typedef struct Config {
	bool is_config_modable;
	char* exe_path;
	//Colors

	int PLAYER_1_COLOR;
	int PLAYER_2_COLOR;
	int WALL_COLOR;
	int BACKGROUND_COLOR;
	int FOOD_COLOR;
	int TEXT_1_COLOR;
	int TEXT_2_COLOR;

	//Keyboard

	int PLAYER_1_UP;
	int PLAYER_1_LEFT;
	int PLAYER_1_RIGHT;
	int PLAYER_1_DOWN;
	int PLAYER_2_UP;
	int PLAYER_2_LEFT;
	int PLAYER_2_RIGHT;
	int PLAYER_2_DOWN;

	//Game Area

	int SCREEN_OFFSET_X;
	int SCREEN_OFFSET_Y;

	//Game
	
	int PLAYER_1_POINT;
	int PLAYER_2_POINT;
	int SNAKE_LENGTH;
} Config;

void config_info();
char* get_dir_path(Config* pConfig);
Config* create_config();
void write_config(const char* config_file, Config* pConfig);
void init_config_default(Config* config);
Config* read_config();
void delete_config(Config* pConfig);

#endif /* CONFIG_H_ */
