#ifndef SETTINGS_H
#define SETTINGS_H
typedef struct {
	//GLUT settings
	int GLUT_UPDATE_MILLIS;
	//Player Settings
	float PLAYER_MAX_SPEED;
	float PLAYER_ACCELERATION; 
	float PLAYER_FRICTION;
	float PLAYER_MAX_RADIUS; 
	float ZOOM_VALUE;
	float PLAYER_STARTING_BUDGET;
	int PILL_COUNT;
	float PILL_SIZE;
}Setting_list;
#endif