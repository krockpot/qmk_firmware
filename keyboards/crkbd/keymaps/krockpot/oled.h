#pragma once

#define SLEEP_TIMEOUT 30000

extern uint8_t is_master;
static uint32_t oled_timer = 0;
/* Override control loop functions */
oled_rotation_t oled_init_user(oled_rotation_t);
void oled_task_user(void);
/* Display complete renders */
void render_status_primary(void);
void render_status_secondary(void);
/* State-based render functions */
void render_layer_state(void);
void render_mod_state(uint8_t);
void render_media_state(void);
void render_keylogger_state(void);
/* Modifier related render functions */
void render_mod_gui(bool);
void render_mod_shift(bool);
void render_mod_ctrl(bool);
void render_mod_alt(bool);
/* Media related render functions */
void render_mute(void);
void render_volume_up(void);
void render_volume_down(void);
void render_media_next(void);
void render_media_previous(void);
void render_play(void);
void render_pause(void);
/* Static render functions */
void render_space(void);
void render_logo(void);
void render_avatar(void);
