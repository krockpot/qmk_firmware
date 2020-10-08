#pragma once

#define KEYLOGGER_LENGTH    5
#define MEDIA_TIMEOUT       750
#define KEYLOGGER_TIMEOUT   750

/* Buffers for storing log info. */
static char keylogger_str[KEYLOGGER_LENGTH + 1] = {"\n"};
static uint16_t media_kc    = 0;
/* Timers for phasing out stale log info. */
uint32_t keylogger_timer    = 0;
uint32_t media_timer        = 0;

/* Keycode map*/
static const char PROGMEM code_to_name[0xFF] = {
/*   0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F    */
    ' ', ' ', ' ', ' ', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l',  // 0x
    'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '1', '2',  // 1x
    '3', '4', '5', '6', '7', '8', '9', '0',  20,  19,  27,  26,  22, '-', '=', '[',  // 2x
    ']','\\', '#', ';','\'', '`', ',', '.', '/', 128, ' ', ' ', ' ', ' ', ' ', ' ',  // 3x
    ' ', ' ', ' ', ' ', ' ', ' ', 'P', 'S', ' ', ' ', ' ', ' ',  16, ' ', ' ', ' ',  // 4x
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  // 5x
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  // 6x
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  // 7x
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  // 8x
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  // 9x
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  // Ax
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  // Bx
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  // Cx
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  // Dx
    'C', 'S', 'A', 'C', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  // Ex
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '        // Fx
};

/**
 * Store last 5 keypresses for later rendering on OLED.
 */
void save_keylogger_state(uint16_t keycode) {
  if ((keycode >= QK_MOD_TAP && keycode <= QK_MOD_TAP_MAX) || (keycode >= QK_LAYER_TAP && keycode <= QK_LAYER_TAP_MAX) || (keycode >= QK_MODS && keycode <= QK_MODS_MAX)) {
    keycode = keycode & 0xFF;
  } else if (keycode > 0xFF) {
    keycode = 0;
  }

  for (uint8_t i = (KEYLOGGER_LENGTH - 1); i > 0; --i) {
    keylogger_str[i] = keylogger_str[i - 1];
  }

  if (keycode < (sizeof(code_to_name) / sizeof(char))) {
    keylogger_str[0] = pgm_read_byte(&code_to_name[keycode]);
  }
  keylogger_timer = timer_read32();
}


/**
 * Store keypress for later rendering on OLED.
 */
void save_media_state(uint16_t keycode) {
    media_kc = keycode;
    media_timer = timer_read32();
}

/**
 * Timeout media keypress entries to keep OLED clean.
 */
void update_media(void) {
    if (timer_elapsed(media_timer) > MEDIA_TIMEOUT) {
        save_media_state(0);
    }
}

/**
 * Timeout keylogger entries to keep OLED clean.
 */
void update_keylogger(void) {
    if (timer_elapsed(keylogger_timer) > KEYLOGGER_TIMEOUT) {
        save_keylogger_state(0);
    }
}

/**
 * Entrypoint from OLED control loop for timing out loggers.
 */
void update_loggers(void) {
    update_media();
    update_keylogger();
}

/**
 * Primary entrypoint from keymap into logging keycodes for various loggers.
 */
void log_keycode(uint16_t keycode) {
    save_keylogger_state(keycode);
    save_media_state(keycode);
}
