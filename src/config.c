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
#include <ncurses.h>
#include "config.h"
#include "error_message.h"
#include <setjmp.h>
#include <string.h>
#ifdef __MINGW32__
#include <windows.h>
#include <Shlwapi.h>
#elif __linux__
#include <unistd.h>
#include <libgen.h>
#elif __APPLE__
#include <unistd.h>
#include <mach-o/dyld.h>
#include <libgen.h>
#endif

extern jmp_buf jmp_buffer10;

void config_info() {
	mvprintw(error_line, 0, "Press any key to continue. . .");
	refresh();
	error_line++;
	getchar();
	clear();
}

void get_config_path(Config* pConfig) {
#ifdef __MINGW32__
	char* buffer = (char*)malloc(MAX_PATH);
	if (buffer == NULL) {
		error_message("Error: Can't get file path. Changing the configurations won't be permanent.");
		config_info();
	}

	DWORD path_size = GetModuleFileName(NULL, buffer, MAX_PATH);
	if (path_size == 0 || path_size == MAX_PATH) {
		free(buffer);
		error_message("Error: Can't get file path. Changing the configurations won't be permanent.");
		config_info();
	}
	PathRemoveFileSpec(buffer);
	char* exe_path = _strdup(buffer);
	if (exe_path == NULL) {
		free(buffer);
		error_message("Error: Can't get file path. Changing the configurations won't be permanent.");
		config_info();
	}

#elif __linux__
	char* buffer = (char*)malloc(PATH_MAX);
	if (buffer == NULL) {
		error_message("Error: Can't get file path. Changing the configurations won't be permanent.");
		config_info();
	}
	
	ssize_t path_size  = readlink("/proc/self/exe", buffer, PATH_MAX - 1);
	if (path_size  == -1) {
		free(buffer);
		error_message("Error: Can't get file path. Changing the configurations won't be permanent.");
		config_info();
	}
	
	buffer[path_size] = '\0';
	char* dir_path = dirname(buffer);
	char* exe_path = strdup(dir_path);
	if (exe_path == NULL) {
		free(buffer);
		error_message("Error: Can't get file path. Changing the configurations won't be permanent.");
		config_info();
	}
#elif __APPLE__
	//TODO I don't have any apple device so test if this even works
	char* buffer = (char*)malloc(PATH_MAX);
	if (buffer == NULL) {
		error_message("Error: Can't get file path. Changing the configurations won't be permanent.");
		config_info();
	}
	uint32_t path_size = PATH_MAX;
	if (_NSGetExecutablePath(buffer, &path_size) != 0) {
		free(buffer);
		error_message("Error: Can't get file path. Changing the configurations won't be permanent.");
		config_info();
    }
	char* dir_path = dirname(buffer);
	char* exe_path = strdup(dir_path);
	if (exe_path == NULL) {
		free(buffer);
		config_info();
	}
#else
	mvprintw(error_line, 0, "Error: Unsupported Platform. Changing the configurations won't be permanent.\nPress any key to continue. . .");
	refresh();
	error_line++;
	getchar();
	clear();
#endif
#ifdef __MINGW32__
	snprintf(pConfig->config_path, MAX_PATH, "%s\\configurations.txt", exe_path);
#else
	char pConfig->config_path[PATH_MAX];
	snprintf(pConfig->config_path, PATH_MAX, "%s/configurations.txt", exe_path);
#endif
	free(buffer);
	free(exe_path);
	pConfig->is_configurable = true;
}

Config* create_config() {
	Config* config = (Config*)malloc(sizeof(Config));
	if (config == NULL) {
		error_message("ERROR func create_config");
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
		error_message("ERROR func write_config");
		config_info();
		pConfig->is_configurable = false;
		return;
	}
	
	fprintf(configurations, "PLAYER_1_COLOR=%i\n", pConfig->configs[PLAYER_1_COLOR]);
	fprintf(configurations, "PLAYER_2_COLOR=%i\n", pConfig->configs[PLAYER_2_COLOR]);
	fprintf(configurations, "WALL_COLOR=%i\n", pConfig->configs[WALL_COLOR]);
	fprintf(configurations, "BACKGROUND_COLOR=%i\n", pConfig->configs[BACKGROUND_COLOR]);
	fprintf(configurations, "FOOD_COLOR=%i\n", pConfig->configs[FOOD_COLOR]);
	fprintf(configurations, "TEXT_1_COLOR=%i\n", pConfig->configs[TEXT_1_COLOR]);
	fprintf(configurations, "TEXT_2_COLOR=%i\n", pConfig->configs[TEXT_2_COLOR]);
	fprintf(configurations, "PLAYER_1_UP=%i\n", pConfig->configs[PLAYER_1_UP]);
	fprintf(configurations, "PLAYER_1_LEFT=%i\n", pConfig->configs[PLAYER_1_LEFT]);
	fprintf(configurations, "PLAYER_1_RIGHT=%i\n", pConfig->configs[PLAYER_1_RIGHT]);
	fprintf(configurations, "PLAYER_1_DOWN=%i\n", pConfig->configs[PLAYER_1_DOWN]);
	fprintf(configurations, "PLAYER_2_UP=%i\n", pConfig->configs[PLAYER_2_UP]);
	fprintf(configurations, "PLAYER_2_LEFT=%i\n", pConfig->configs[PLAYER_2_LEFT]);
	fprintf(configurations, "PLAYER_2_RIGHT=%i\n", pConfig->configs[PLAYER_2_RIGHT]);
	fprintf(configurations, "PLAYER_2_DOWN=%i\n", pConfig->configs[PLAYER_2_DOWN]);
	fprintf(configurations, "SCREEN_OFFSET_X=%i\n", pConfig->configs[SCREEN_OFFSET_X]);
	fprintf(configurations, "SCREEN_OFFSET_Y=%i\n", pConfig->configs[SCREEN_OFFSET_Y]);
	fprintf(configurations, "PLAYER_1_POINT=%i\n", pConfig->configs[PLAYER_1_POINT]);
	fprintf(configurations, "PLAYER_2_POINT=%i\n", pConfig->configs[PLAYER_2_POINT]);
	fprintf(configurations, "SNAKE_LENGTH=%i\n", pConfig->configs[SNAKE_LENGTH]);
	fclose(configurations);
}

void init_config_default(Config* pConfig) {
	pConfig->configs[PLAYER_1_COLOR] = 3;
	pConfig->configs[PLAYER_2_COLOR] = 5;
	pConfig->configs[WALL_COLOR] = 2;
	pConfig->configs[BACKGROUND_COLOR] = 1;
	pConfig->configs[FOOD_COLOR] = 7;
	pConfig->configs[TEXT_1_COLOR] = 50;
	pConfig->configs[TEXT_2_COLOR] = 4;
	pConfig->configs[PLAYER_1_UP] = 119;
	pConfig->configs[PLAYER_1_LEFT] = 97;
	pConfig->configs[PLAYER_1_RIGHT] = 100;
	pConfig->configs[PLAYER_1_DOWN] = 115;
	pConfig->configs[PLAYER_2_UP] = KEY_UP;
	pConfig->configs[PLAYER_2_LEFT] = KEY_LEFT;
	pConfig->configs[PLAYER_2_RIGHT] = KEY_RIGHT;
	pConfig->configs[PLAYER_2_DOWN] = KEY_DOWN;
	pConfig->configs[SCREEN_OFFSET_X] = 3;
	pConfig->configs[SCREEN_OFFSET_Y] = 3;
	pConfig->configs[PLAYER_1_POINT] = 0;
	pConfig->configs[PLAYER_2_POINT] = 0;
	pConfig->configs[SNAKE_LENGTH] = 10;
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
	fscanf(configurations, "PLAYER_1_COLOR=%i\n", &pConfig->configs[PLAYER_1_COLOR]);
	fscanf(configurations, "PLAYER_2_COLOR=%i\n", &pConfig->configs[PLAYER_2_COLOR]);
	fscanf(configurations, "WALL_COLOR=%i\n", &pConfig->configs[WALL_COLOR]);
	fscanf(configurations, "BACKGROUND_COLOR=%i\n", &pConfig->configs[BACKGROUND_COLOR]);
	fscanf(configurations, "FOOD_COLOR=%i\n", &pConfig->configs[FOOD_COLOR]);
	fscanf(configurations, "TEXT_1_COLOR=%i\n", &pConfig->configs[TEXT_1_COLOR]);
	fscanf(configurations, "TEXT_2_COLOR=%i\n", &pConfig->configs[TEXT_2_COLOR]);
	fscanf(configurations, "PLAYER_1_UP=%i\n", &pConfig->configs[PLAYER_1_UP]);
	fscanf(configurations, "PLAYER_1_LEFT=%i\n", &pConfig->configs[PLAYER_1_LEFT]);
	fscanf(configurations, "PLAYER_1_RIGHT=%i\n", &pConfig->configs[PLAYER_1_RIGHT]);
	fscanf(configurations, "PLAYER_1_DOWN=%i\n", &pConfig->configs[PLAYER_1_DOWN]);
	fscanf(configurations, "PLAYER_2_UP=%i\n", &pConfig->configs[PLAYER_2_UP]);
	fscanf(configurations, "PLAYER_2_LEFT=%i\n", &pConfig->configs[PLAYER_2_LEFT]);
	fscanf(configurations, "PLAYER_2_RIGHT=%i\n", &pConfig->configs[PLAYER_2_RIGHT]);
	fscanf(configurations, "PLAYER_2_DOWN=%i\n", &pConfig->configs[PLAYER_2_DOWN]);
	fscanf(configurations, "SCREEN_OFFSET_X=%i\n", &pConfig->configs[SCREEN_OFFSET_X]);
	fscanf(configurations, "SCREEN_OFFSET_Y=%i\n", &pConfig->configs[SCREEN_OFFSET_Y]);
	fscanf(configurations, "PLAYER_1_POINT=%i\n", &pConfig->configs[PLAYER_1_POINT]);
	fscanf(configurations, "PLAYER_2_POINT=%i\n", &pConfig->configs[PLAYER_2_POINT]);
	fscanf(configurations, "SNAKE_LENGTH=%i\n", &pConfig->configs[SNAKE_LENGTH]);
	fclose(configurations);
	return pConfig;
}

void delete_config(Config* pConfig) {
	if (pConfig == NULL) {
		return;
	}
	free(pConfig);
	pConfig = NULL;
}
