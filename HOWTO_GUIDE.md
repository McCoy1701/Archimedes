# Game Development with Archimedes Input System

The Archimedes input system provides a simple, efficient interface for handling keyboard, mouse, and text input in games and interactive applications. Built on SDL2, it offers cross-platform compatibility and real-time input processing perfect for game development.

## Core Input Processing

The input system centers around the `a_DoInput()` function, which should be called once per frame to process all pending input events. The system maintains global state in the `app` structure, making input queries fast and straightforward.

### Basic Input Loop

```c
#include "Archimedes.h"

int main(void) {
	a_Init(1280, 720, "My Game");

	while (app.running) {
		// Process input events
		a_DoInput();

		// Check input state
		if (app.keyboard[SDL_SCANCODE_ESCAPE]) {
			app.running = 0;
		}

		// Game logic here
		// Rendering here

		a_PresentScene();
	}

	a_Quit();
	return 0;
}
```

## Game Development Examples

### 1. 2D Platformer Character Controller

This example demonstrates classic 2D platformer controls with WASD movement, jumping, and mouse-based shooting mechanics.

```c
#include "Archimedes.h"

typedef struct {
	float x, y;
	float velocity_x, velocity_y;
	int on_ground;
	int health;
	int facing_right;
} Player;

void update_player(Player* player, float delta_time) {
	const float MOVE_SPEED = 200.0f;
	const float JUMP_FORCE = -400.0f;
	const float GRAVITY = 800.0f;

	// Horizontal movement
	if (app.keyboard[SDL_SCANCODE_A] || app.keyboard[SDL_SCANCODE_LEFT]) {
		player->velocity_x = -MOVE_SPEED;
		player->facing_right = 0;
	} else if (app.keyboard[SDL_SCANCODE_D] || app.keyboard[SDL_SCANCODE_RIGHT]) {
		player->velocity_x = MOVE_SPEED;
		player->facing_right = 1;
	} else {
		player->velocity_x *= 0.8f; // Friction
	}

	// Jumping
	if ((app.keyboard[SDL_SCANCODE_SPACE] || app.keyboard[SDL_SCANCODE_W]) && player->on_ground) {
		player->velocity_y = JUMP_FORCE;
		player->on_ground = 0;
	}

	// Mouse shooting
	if (app.mouse.pressed && app.mouse.button == SDL_BUTTON_LEFT) {
		// Calculate direction from player to mouse
		float shoot_dir_x = app.mouse.x - player->x;
		float shoot_dir_y = app.mouse.y - player->y;

		// Spawn bullet in that direction
		spawn_bullet(player->x, player->y, shoot_dir_x, shoot_dir_y);
	}

	// Apply gravity and update position
	player->velocity_y += GRAVITY * delta_time;
	player->x += player->velocity_x * delta_time;
	player->y += player->velocity_y * delta_time;

	// Simple ground collision
	if (player->y > 500) {
		player->y = 500;
		player->velocity_y = 0;
		player->on_ground = 1;
	}
}

void handle_player_input(Player* player) {
	// Check for special abilities
	if (app.keyboard[SDL_SCANCODE_LSHIFT] && app.mouse.pressed) {
		// Power attack with shift+click
		power_attack(player, app.mouse.x, app.mouse.y);
	}

	// Inventory management
	if (app.keyboard[SDL_SCANCODE_I]) {
		toggle_inventory();
	}

	// Quick item usage (number keys)
	for (int i = SDL_SCANCODE_1; i <= SDL_SCANCODE_9; i++) {
		if (app.keyboard[i]) {
			use_item_slot(i - SDL_SCANCODE_1);
		}
	}
}
```

### 2. Real-Time Strategy (RTS) Camera and Unit Control

This example shows how to implement RTS-style camera controls with mouse edge scrolling and unit selection/command systems.

```c
#include "Archimedes.h"

typedef struct {
	float x, y;
	float zoom;
	int drag_scrolling;
	int last_mouse_x, last_mouse_y;
} RTSCamera;

typedef struct {
	float x, y;
	int selected;
	int unit_type;
	float health;
} Unit;

void update_rts_camera(RTSCamera* camera, float delta_time) {
	const float SCROLL_SPEED = 300.0f;
	const float ZOOM_SPEED = 0.1f;
	const int EDGE_SCROLL_ZONE = 50;

	// Edge scrolling with mouse
	if (app.mouse.x < EDGE_SCROLL_ZONE) {
		camera->x -= SCROLL_SPEED * delta_time;
	}
	if (app.mouse.x > SCREEN_WIDTH - EDGE_SCROLL_ZONE) {
		camera->x += SCROLL_SPEED * delta_time;
	}
	if (app.mouse.y < EDGE_SCROLL_ZONE) {
		camera->y -= SCROLL_SPEED * delta_time;
	}
	if (app.mouse.y > SCREEN_HEIGHT - EDGE_SCROLL_ZONE) {
		camera->y += SCROLL_SPEED * delta_time;
	}

	// WASD camera movement
	if (app.keyboard[SDL_SCANCODE_W]) camera->y -= SCROLL_SPEED * delta_time;
	if (app.keyboard[SDL_SCANCODE_S]) camera->y += SCROLL_SPEED * delta_time;
	if (app.keyboard[SDL_SCANCODE_A]) camera->x -= SCROLL_SPEED * delta_time;
	if (app.keyboard[SDL_SCANCODE_D]) camera->x += SCROLL_SPEED * delta_time;

	// Mouse wheel zooming
	if (app.mouse.wheel > 0) {
		camera->zoom += ZOOM_SPEED;
		app.mouse.wheel = 0; // Reset wheel after processing
	} else if (app.mouse.wheel < 0) {
		camera->zoom -= ZOOM_SPEED;
		app.mouse.wheel = 0;
	}

	// Clamp zoom
	if (camera->zoom < 0.5f) camera->zoom = 0.5f;
	if (camera->zoom > 3.0f) camera->zoom = 3.0f;

	// Middle mouse drag scrolling
	if (app.mouse.button == SDL_BUTTON_MIDDLE && app.mouse.pressed) {
		if (!camera->drag_scrolling) {
			camera->drag_scrolling = 1;
			camera->last_mouse_x = app.mouse.x;
			camera->last_mouse_y = app.mouse.y;
		} else {
			int dx = app.mouse.x - camera->last_mouse_x;
			int dy = app.mouse.y - camera->last_mouse_y;
			camera->x -= dx / camera->zoom;
			camera->y -= dy / camera->zoom;
			camera->last_mouse_x = app.mouse.x;
			camera->last_mouse_y = app.mouse.y;
		}
	} else {
		camera->drag_scrolling = 0;
	}
}

void handle_unit_selection(Unit* units, int unit_count) {
	static int selection_start_x, selection_start_y;
	static int selecting = 0;

	// Start selection box
	if (app.mouse.button == SDL_BUTTON_LEFT && app.mouse.pressed && !selecting) {
		// Clear previous selection if not holding Ctrl
		if (!app.keyboard[SDL_SCANCODE_LCTRL]) {
			for (int i = 0; i < unit_count; i++) {
				units[i].selected = 0;
			}
		}

		selection_start_x = app.mouse.x;
		selection_start_y = app.mouse.y;
		selecting = 1;
	}

	// Finish selection
	if (selecting && !app.mouse.pressed) {
		// Select units in rectangle
		int min_x = (selection_start_x < app.mouse.x) ? selection_start_x : app.mouse.x;
		int max_x = (selection_start_x > app.mouse.x) ? selection_start_x : app.mouse.x;
		int min_y = (selection_start_y < app.mouse.y) ? selection_start_y : app.mouse.y;
		int max_y = (selection_start_y > app.mouse.y) ? selection_start_y : app.mouse.y;

		for (int i = 0; i < unit_count; i++) {
			if (units[i].x >= min_x && units[i].x <= max_x &&
				units[i].y >= min_y && units[i].y <= max_y) {
				units[i].selected = 1;
			}
		}
		selecting = 0;
	}

	// Right-click commands for selected units
	if (app.mouse.button == SDL_BUTTON_RIGHT && app.mouse.pressed) {
		for (int i = 0; i < unit_count; i++) {
			if (units[i].selected) {
				// Command unit to move to mouse position
				command_unit_move(&units[i], app.mouse.x, app.mouse.y);
			}
		}
	}

	// Hotkey group selection (Ctrl+1-9 to assign, 1-9 to select)
	for (int key = SDL_SCANCODE_1; key <= SDL_SCANCODE_9; key++) {
		if (app.keyboard[key]) {
			int group = key - SDL_SCANCODE_1;
			if (app.keyboard[SDL_SCANCODE_LCTRL]) {
				assign_units_to_group(units, unit_count, group);
			} else {
				select_unit_group(units, unit_count, group);
			}
		}
	}
}
```

### 3. Menu System with Text Input and Navigation

This example demonstrates how to create interactive menus with keyboard navigation, text input fields, and mouse interactions.

```c
#include "Archimedes.h"

typedef enum {
	MENU_MAIN,
	MENU_OPTIONS,
	MENU_NAME_ENTRY,
	MENU_CONFIRM
} MenuState;

typedef struct {
	MenuState current_menu;
	int selected_option;
	char player_name[32];
	int name_cursor_pos;
	int show_cursor;
	float cursor_blink_timer;
} MenuSystem;

void update_main_menu(MenuSystem* menu) {
	const char* options[] = {"New Game", "Load Game", "Options", "Quit"};
	const int option_count = 4;

	// Keyboard navigation
	if (app.keyboard[SDL_SCANCODE_UP] || app.keyboard[SDL_SCANCODE_W]) {
		menu->selected_option = (menu->selected_option - 1 + option_count) % option_count;
	}
	if (app.keyboard[SDL_SCANCODE_DOWN] || app.keyboard[SDL_SCANCODE_S]) {
		menu->selected_option = (menu->selected_option + 1) % option_count;
	}

	// Mouse navigation
	for (int i = 0; i < option_count; i++) {
		int option_y = 200 + i * 50;
		if (app.mouse.y >= option_y && app.mouse.y < option_y + 40) {
			menu->selected_option = i;

			// Mouse click selection
			if (app.mouse.pressed && app.mouse.button == SDL_BUTTON_LEFT) {
				activate_menu_option(menu, i);
			}
		}
	}

	// Enter key selection
	if (app.keyboard[SDL_SCANCODE_RETURN] || app.keyboard[SDL_SCANCODE_SPACE]) {
		activate_menu_option(menu, menu->selected_option);
	}

	// Escape to quit
	if (app.keyboard[SDL_SCANCODE_ESCAPE]) {
		app.running = 0;
	}
}

void update_text_input_menu(MenuSystem* menu, float delta_time) {
	// Handle text input for player name
	if (strlen(app.input_text) > 0) {
		// Add typed character to name if there's room
		if (strlen(menu->player_name) < sizeof(menu->player_name) - 1) {
			strncat(menu->player_name, app.input_text, 1);
			menu->name_cursor_pos++;
		}
		// Clear input buffer after processing
		memset(app.input_text, 0, sizeof(app.input_text));
	}

	// Backspace handling
	if (app.keyboard[SDL_SCANCODE_BACKSPACE]) {
		if (strlen(menu->player_name) > 0) {
			menu->player_name[strlen(menu->player_name) - 1] = '\0';
			menu->name_cursor_pos--;
			if (menu->name_cursor_pos < 0) menu->name_cursor_pos = 0;
		}
	}

	// Cursor blinking
	menu->cursor_blink_timer += delta_time;
	if (menu->cursor_blink_timer > 0.5f) {
		menu->show_cursor = !menu->show_cursor;
		menu->cursor_blink_timer = 0.0f;
	}

	// Confirm name entry
	if (app.keyboard[SDL_SCANCODE_RETURN]) {
		if (strlen(menu->player_name) > 0) {
			menu->current_menu = MENU_CONFIRM;
			menu->selected_option = 0;
		}
	}

	// Cancel and go back
	if (app.keyboard[SDL_SCANCODE_ESCAPE]) {
		menu->current_menu = MENU_MAIN;
		menu->selected_option = 0;
	}
}

void handle_menu_input(MenuSystem* menu, float delta_time) {
	switch (menu->current_menu) {
		case MENU_MAIN:
			update_main_menu(menu);
			break;

		case MENU_NAME_ENTRY:
			update_text_input_menu(menu, delta_time);
			break;

		case MENU_OPTIONS:
			// Handle options menu navigation
			handle_options_menu(menu);
			break;

		case MENU_CONFIRM:
			// Handle confirmation dialog
			if (app.keyboard[SDL_SCANCODE_Y]) {
				start_new_game(menu->player_name);
			}
			if (app.keyboard[SDL_SCANCODE_N] || app.keyboard[SDL_SCANCODE_ESCAPE]) {
				menu->current_menu = MENU_MAIN;
			}
			break;
	}
}

void activate_menu_option(MenuSystem* menu, int option) {
	switch (option) {
		case 0: // New Game
			menu->current_menu = MENU_NAME_ENTRY;
			memset(menu->player_name, 0, sizeof(menu->player_name));
			menu->name_cursor_pos = 0;
			break;

		case 1: // Load Game
			load_game_dialog();
			break;

		case 2: // Options
			menu->current_menu = MENU_OPTIONS;
			menu->selected_option = 0;
			break;

		case 3: // Quit
			app.running = 0;
			break;
	}
}
```

## Input System Best Practices

### Frame-by-Frame Input Processing

Always call `a_DoInput()` once per frame at the beginning of your game loop. The input system maintains state between frames, so you can query input status throughout your update logic.

### Mouse Wheel Handling

The mouse wheel value persists until overwritten by a new wheel event. After processing wheel input, reset it manually:

```c
if (app.mouse.wheel != 0) {
	// Process wheel input
	camera_zoom += app.mouse.wheel * zoom_speed;

	// Reset wheel for next frame
	app.mouse.wheel = 0;
}
```

### Text Input Management

Text input events populate `app.input_text` with UTF-8 text. Clear this buffer after processing to avoid duplicate input:

```c
if (strlen(app.input_text) > 0) {
	// Process text input
	append_to_text_field(app.input_text);

	// Clear for next frame
	memset(app.input_text, 0, sizeof(app.input_text));
}
```

### Key State vs. Key Events

- Use `app.keyboard[scancode]` for continuous input (movement, held actions)
- Implement your own key press detection for single-press actions by tracking previous frame state
- The `app.last_key_pressed` field contains the most recent key that was pressed

### Multi-Input Coordination

The input system handles simultaneous inputs naturally. You can check multiple input sources in the same frame:

```c
// Player can move and shoot simultaneously
if (app.keyboard[SDL_SCANCODE_W]) {
	player.y -= move_speed;
}
if (app.mouse.pressed && app.mouse.button == SDL_BUTTON_LEFT) {
	fire_weapon();
}
if (app.keyboard[SDL_SCANCODE_LSHIFT]) {
	// Running while moving and shooting
	move_speed *= 2.0f;
}
```

The Archimedes input system provides a solid foundation for any type of game input, from simple arcade games to complex strategy titles. Its SDL2 foundation ensures cross-platform compatibility while maintaining the simplicity needed for rapid game development.
