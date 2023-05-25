/*
 ============================================================================
 Name        : app_status.h
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Header file for Snake-by-InfinitePain
=============================================================================
*/

#ifndef APP_STATUS_H_
#define APP_STATUS_H_

typedef enum GameState {
	NOT_STARTED,
	STARTED,
	GAME_OVER,
	SETTINGS,
	QUIT,
	CRITICAL_ERROR,
} GameState;


typedef enum GameMode {
	NOT_SELECTED,
	SINGLE_PLAYER,
	MULTIPLAYER
} GameMode;

extern GameState GAME_STATE;
extern GameMode GAME_MODE;

#endif /*APP_STATUS_H_*/