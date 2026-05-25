#ifndef CONSTANTS_H
#define CONSTANTS_H

// Screen dimensions
const int SCREEN_WIDTH = 1800;
const int SCREEN_HEIGHT = 920;

// Physics
const double GRAVITY = 9.8;
const int BASE_VELOCITY = 50;
const int BASE_ANGLE = 45;
const double BASE_POWER = 0.6;

// Player settings
const int PLAYER_HEALTH = 100;
const int MAX_HEALTH = 100;
const int SCREEN_DIFF_X = 150;
const int SCREEN_DIFF_Y = 150;
const int ARROW_RADIUS = 5;
const int PLAYER_RADIUS = 300;
const int HEADSHOT_RADIUS = 50;

// Damage values
const int HEADSHOT_DAMAGE = 80;
const int BODYYSHOT_DAMAGE = 20;

// Powerup damage values
const int POWERUP_DAMAGE_1 = 30;
const int POWERUP_DAMAGE_2 = 40;
const int POWERUP_HEAL_1 = 10;
const int POWERUP_HEAL_2 = 20;
const int POWERUP_HEAL_CAP_1 = 90;
const int POWERUP_HEAL_CAP_2 = 80;

// UI dimensions
const float TITLE_SCALE = 0.3f;
const float SPRITE_SCALE = 0.5f;
const float SMALL_SPRITE_SCALE = 0.2f;
const float PAUSE_SCALE = 0.1f;

// Timing
const int TARGET_FPS = 60;
const int FRAME_TIME_MS = 1000 / TARGET_FPS;

// Angle constraints
const int MIN_ANGLE = -45;
const int MAX_ANGLE = 90;

// Power constraints
const double MIN_POWER = 0.0;
const double MAX_POWER = 1.0;

// Animation
const int LOADING_FRAMES = 550;
const int LOADING_COMPLETE = 700;
const float LOAD_INCREMENT = 0.01f;

// Menu item offsets
const int MENU_OFFSET_X = 130;
const int MENU_OFFSET_Y = -190;
const int MENU_OFFSET_Y2 = -350;
const int MENU_OFFSET_Y3 = -320;

// Trajectory preview
const int TRAJECTORY_STEPS = 30;
const float TRAJECTORY_ALPHA_BASE = 0.25f;

// Arrow visual
const float ARROW_SCALE = 0.1f;

#endif // CONSTANTS_H