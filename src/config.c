/*
 ============================================================================
 Name        : config.h
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Source file for Snake-by-InfinitePain
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#define NCURSES_STATIC
#include <ncurses.h>
#include "config.h"
#include "error_message.h"
#include <setjmp.h>
#include <string.h>
#ifdef __MINGW32__
#include <windows.h>
#include <Shlwapi.h>
#include <shlobj.h>
#include <direct.h>
#elif __linux__
#include <unistd.h>
#include <libgen.h>
#include <sys/stat.h>
#elif __APPLE__
#include <limits.h>
#include <unistd.h>
#include <mach-o/dyld.h>
#include <libgen.h>
#include <sys/stat.h>
#endif

extern jmp_buf jmp_buffer10;
char* config_names[NUM_CONFIGS] = {
	"PLAYER 1 COLOR",
	"PLAYER 2 COLOR",
	"WALL COLOR",
	"BACKGROUND COLOR",
	"FOOD COLOR",
	"PLAYER 1 UP",
	"PLAYER 1 LEFT",
	"PLAYER 1 RIGHT",
	"PLAYER 1 DOWN",
	"PLAYER 2 UP",
	"PLAYER 2 LEFT",
	"PLAYER 2 RIGHT",
	"PLAYER 2 DOWN",
	"SCREEN WIDTH",
	"SCREEN HEIGHT",
	"SNAKE LENGTH",
	"FOOD SINGLE PLAYER",
	"FOOD MULTIPLAYER",
	"GAME SPEED",
	"MAX POINT",
	//TODO if you add more configs, adjust this
};

void config_info() {
	mvprintw(error_line, 0, "Press any key to continue. . .");
	refresh();
	error_line++;
	getchar();
	clear();
}

void get_config_path(Config* pConfig) {
#ifdef __MINGW32__
	pConfig->config_path = (char*)malloc(MAX_PATH);
	if (pConfig->config_path == NULL) {
		error_message("ERROR: Can't allocate memory for path. Changing the configurations won't be permanent.");
		config_info();
		return;
	}

	char* appdata_path = (char*)malloc(MAX_PATH);
	if (appdata_path == NULL) {
		free(pConfig->config_path);
		error_message("ERROR: Can't allocate memory for appdata path. Changing the configurations won't be permanent.");
		config_info();
		return;
	}
	
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA | CSIDL_FLAG_CREATE, NULL, 0, appdata_path))) {
		char dir_path[MAX_PATH];
		snprintf(dir_path, MAX_PATH, "%s\\Snake-by-InfinitePain", appdata_path);
		_mkdir(dir_path);
		snprintf(pConfig->config_path, MAX_PATH, "%s\\configurations.txt", dir_path);
	}
	else {
		error_message("ERROR: Can't get AppData folder path. Changing the configurations won't be permanent.");
		free(appdata_path);
		config_info();
		return;
	}
	free(appdata_path);
	
#elif __linux__
	long path_max = pathconf("/", _PC_PATH_MAX);
	if (path_max == -1) {
		error_message("ERROR: Can't get max path length. Changing the configurations won't be permanent.");
		config_info();
		return;
	}
	
	pConfig->config_path = (char*)malloc(path_max);
	if (pConfig->config_path == NULL) {
		error_message("ERROR: Can't allocate memory for path. Changing the configurations won't be permanent.");
		config_info();
		return;
	}
	
	char* home_dir = getenv("HOME");
	if (home_dir == NULL) {
		error_message("ERROR: Can't get home directory. Changing the configurations won't be permanent.");
		config_info();
		return;
	}
	char dir_path[path_max];
	snprintf(dir_path, path_max, "%s/.config/Snake-by-InfinitePain", home_dir);
	mkdir(dir_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	snprintf(pConfig->config_path, path_max, "%s/configurations.txt", dir_path);

#elif __APPLE__
	//TODO I don't have any apple device so test if this even works
	pConfig->config_path = (char*)malloc(PATH_MAX);
	if (pConfig->config_path == NULL) {
		free(buffer);
		error_message("ERROR: Can't get file path. Changing the configurations won't be permanent.");
		config_info();
		return;
	}

	char* home_dir = getenv("HOME");
	if (home_dir == NULL) {
		error_message("ERROR: Can't get home directory. Changing the configurations won't be permanent.");
		config_info();
		return;
	}
	char dir_path[PATH_MAX];
	snprintf(dir_path, PATH_MAX, "%s/Library/Application Support/Snake-by-InfinitePain", home_dir);
	mkdir(dir_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	snprintf(pConfig->config_path, PATH_MAX, "%s/configurations.txt", dir_path);
	
#else
	mvprintw(error_line, 0, "ERROR: Unsupported Platform. Changing the configurations won't be permanent.\nPress any key to continue. . .");
	refresh();
	error_line++;
	getchar();
	clear();
#endif
	
	pConfig->is_configurable = true;
}

Config* create_config() {
	Config* config = (Config*)malloc(sizeof(Config));
	if (config == NULL) {
		error_message("ERROR func create_config(): malloc");
		return NULL;
	}
	get_config_path(config);
	return config;
}

void write_config(Config* pConfig) {
	if (!pConfig->is_configurable) {
		return;
	}
	
	FILE* configurations = fopen(pConfig->config_path, "w");
	if (configurations == NULL) {
		error_message("ERROR func write_config(): fopen");
		config_info();
		pConfig->is_configurable = false;
		return;
	}

	for (int i = 0; i < NUM_CONFIGS; i++) {
		fprintf(configurations, "%s=%i\n", config_names[i], pConfig->configs[i]);
	}
	fclose(configurations);
}

void init_config_default(Config* pConfig) {
	pConfig->configs[PLAYER_1_COLOR] = 3;
	pConfig->configs[PLAYER_2_COLOR] = 5;
	pConfig->configs[WALL_COLOR] = 2;
	pConfig->configs[BACKGROUND_COLOR] = 1;
	pConfig->configs[FOOD_COLOR] = 7;
	pConfig->configs[PLAYER_1_UP] = 119;
	pConfig->configs[PLAYER_1_LEFT] = 97;
	pConfig->configs[PLAYER_1_RIGHT] = 100;
	pConfig->configs[PLAYER_1_DOWN] = 115;
	pConfig->configs[PLAYER_2_UP] = KEY_UP;
	pConfig->configs[PLAYER_2_LEFT] = KEY_LEFT;
	pConfig->configs[PLAYER_2_RIGHT] = KEY_RIGHT;
	pConfig->configs[PLAYER_2_DOWN] = KEY_DOWN;
	pConfig->configs[SCREEN_WIDTH] = 80;
	pConfig->configs[SCREEN_HEIGHT] = 24;
	pConfig->configs[SNAKE_LENGTH] = 10;
	pConfig->configs[FOOD_AMOUNT_SINGLE_PLAYER] = 1;
	pConfig->configs[FOOD_AMOUNT_MULTIPLAYER] = 2;
	pConfig->configs[GAME_SPEED] = 4;
	pConfig->configs[MAX_POINT] = 0;
	//TODO if you add more configs, adjust this
}

Config* read_config() {
	Config* pConfig = create_config();
	if (pConfig == NULL) {
		longjmp(jmp_buffer10, 1);
	}

	if (!pConfig->is_configurable) {
		init_config_default(pConfig);
		return pConfig;
	}

	FILE* configurations = fopen(pConfig->config_path, "r");
	if (configurations == NULL) {
		init_config_default(pConfig);
		write_config(pConfig);
		return pConfig;
	}
	for (int i = 0; i < NUM_CONFIGS; i++) {
		fscanf(configurations, "%*[^=]=%i\n", &pConfig->configs[i]);
	}
	fclose(configurations);
	return pConfig;
}

void delete_config(Config* pConfig) {
	if (pConfig == NULL) {
		return;
	}
	free(pConfig->config_path);
	free(pConfig);
	pConfig = NULL;
}
