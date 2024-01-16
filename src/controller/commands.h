

#ifndef COMMAND_MACRO // Macro used to integrate command as string

#define EXTERN_COMMAND 1000
#define EXTERN_COMMAND_END 2000
#define EFFECT_COMMAND 2000
#define EFFECT_COMMAND_END 3000
#define TOWER_COMMAND 3000
#define TOWER_COMMAND_END 4000
#define GRAPHIC_COMMAND 4000
#define GRAPHIC_COMMAND_END 5000
#define MAP_COMMAND 5000
#define MAP_COMMAND_END 6000

#define COMMAND_NONE 0
#define COMMAND_EXIT 1
#define COMMAND_SELECT 2
#define COMMAND_UNSELECT 3
#define COMMAND_BUILD 4
#define COMMAND_START 5
#define COMMAND_PAUSE 6
#define COMMAND_RESUME 7
#define COMMAND_CHANGE_PLAYER 8



#define GRAPHIC_COMMAND_ZOOM_MORE				GRAPHIC_COMMAND + 1
#define GRAPHIC_COMMAND_ZOOM_LESS				GRAPHIC_COMMAND + 2
#define GRAPHIC_COMMAND_MOVE_LEFT				GRAPHIC_COMMAND + 3
#define GRAPHIC_COMMAND_MOVE_RIGHT				GRAPHIC_COMMAND + 4
#define GRAPHIC_COMMAND_MOVE_UP					GRAPHIC_COMMAND + 5
#define GRAPHIC_COMMAND_MOVE_DOWN				GRAPHIC_COMMAND + 6
#define GRAPHIC_COMMAND_FULLSCREEN				GRAPHIC_COMMAND + 7
#define GRAPHIC_COMMAND_RELOAD_SHADERS			GRAPHIC_COMMAND + 8
#define GRAPHIC_COMMAND_RELOAD_COLORS			GRAPHIC_COMMAND + 9
#define GRAPHIC_COMMAND_DISPLAY_GRID			GRAPHIC_COMMAND + 10

#else

COMMAND_MACRO(COMMAND_EXIT)
COMMAND_MACRO(COMMAND_SELECT)
COMMAND_MACRO(COMMAND_UNSELECT)
COMMAND_MACRO(COMMAND_BUILD)
COMMAND_MACRO(COMMAND_START)
COMMAND_MACRO(COMMAND_PAUSE)
COMMAND_MACRO(COMMAND_RESUME)
COMMAND_MACRO(COMMAND_CHANGE_PLAYER)
COMMAND_MACRO(GRAPHIC_COMMAND_ZOOM_MORE)
COMMAND_MACRO(GRAPHIC_COMMAND_ZOOM_LESS)
COMMAND_MACRO(GRAPHIC_COMMAND_MOVE_LEFT)
COMMAND_MACRO(GRAPHIC_COMMAND_MOVE_RIGHT)
COMMAND_MACRO(GRAPHIC_COMMAND_MOVE_UP)
COMMAND_MACRO(GRAPHIC_COMMAND_MOVE_DOWN)
COMMAND_MACRO(GRAPHIC_COMMAND_FULLSCREEN)
COMMAND_MACRO(GRAPHIC_COMMAND_RELOAD_SHADERS)
COMMAND_MACRO(GRAPHIC_COMMAND_RELOAD_COLORS)
COMMAND_MACRO(GRAPHIC_COMMAND_DISPLAY_GRID)

#undef COMMAND_MACRO
#endif