#include "oled.h"
#include "keylogger.c"

/**
 * Orients both OLEDs to be rendered top down for convenience.
 */
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_270;
}

/**
 * Main processing loop: handles timing out the OLED to prevent burn-in as well
 * as rendering each screen.
 */
void oled_task_user(void) {
    if (timer_elapsed32(oled_timer) > SLEEP_TIMEOUT) {
        oled_off();
        return;
    }
#ifndef SPLIT_KEYBOARD
    else { oled_on(); }
#endif
    update_loggers();
    if (is_master) {
        render_status_primary();
    } else {
        render_status_secondary();
    }
}

/**
 * Primary display shows:
 * 5x3 logo
 * 5x2 spacing
 * 3x3 layer state (padded to 5x3)
 * 5x1 spacing
 * 5x1 keylog (last 5 characters)
 * 5x2 spacing
 * 5x2 modifier grid (dynamic highlight when pressed)
 */
void render_status_primary(void) {
    render_logo();
    render_space();
    render_space();
    render_layer_state();
    render_space();
    render_keylogger_state();
    render_space();
    render_space();
    render_mod_state(get_mods());
}

/**
 * Secondary display shows:
 * 3x9 avatar (taller logo, padded to 5x9)
 * 5x2 spacing
 * 5x1 media status line (short lived)
 */
void render_status_secondary(void) {
    render_avatar();
    render_space();
    render_space();
    render_media_state();
}

/* 5x1 blank row */
void render_space(void) {
    oled_write_P(PSTR("     "), false);
}

/* 3x3 Layer visual representation */
void render_layer_state(void) {
    static const char PROGMEM default_layer[] = {
        0x20, 0x94, 0x95, 0x96, 0x20,
        0x20, 0xb4, 0xb5, 0xb6, 0x20,
        0x20, 0xd4, 0xd5, 0xd6, 0x20, 0};
    static const char PROGMEM raise_layer[] = {
        0x20, 0x97, 0x98, 0x99, 0x20,
        0x20, 0xb7, 0xb8, 0xb9, 0x20,
        0x20, 0xd7, 0xd8, 0xd9, 0x20, 0};
    static const char PROGMEM lower_layer[] = {
        0x20, 0x9a, 0x9b, 0x9c, 0x20,
        0x20, 0xba, 0xbb, 0xbc, 0x20,
        0x20, 0xda, 0xdb, 0xdc, 0x20, 0};
    static const char PROGMEM adjust_layer[] = {
        0x20, 0x9d, 0x9e, 0x9f, 0x20,
        0x20, 0xbd, 0xbe, 0xbf, 0x20,
        0x20, 0xdd, 0xde, 0xdf, 0x20, 0};
    if(layer_state_is(_ADJUST)) {
        oled_write_P(adjust_layer, false);
    } else if(layer_state_is(_LOWER)) {
        oled_write_P(lower_layer, false);
    } else if(layer_state_is(_RAISE)) {
        oled_write_P(raise_layer, false);
    } else {
        oled_write_P(default_layer, false);
    }
}

/* 2x1 Modifiers -- Ctrl, Alt (Option), Shift, GUI */
void render_mod_gui(bool on) {
    static const char PROGMEM gui_icon[2][3] = {
        {0xc5, 0xc6, 0}, // Windows
        {0xc7, 0xc8, 0}, // Mac
    };
    oled_write_P(gui_icon[OS_ICON], on);
}

void render_mod_shift(bool on) {
    static const char PROGMEM shift_icon[] = {0xc9, 0xca, 0};
    oled_write_P(shift_icon, on);
}

void render_mod_ctrl(bool on) {
    static const char PROGMEM ctrl_icon[] = {0x89, 0x8a, 0};
    oled_write_P(ctrl_icon, on);
}

void render_mod_alt(bool on) {
    static const char PROGMEM alt_icon[] = {0xa9, 0xaa, 0};
    oled_write_P(alt_icon, on);
}

/**
 * Mods will appear in a 5 x 2 grid, space between ones sharing a line.
 * They will invert colors when pressed.
 */
void render_mod_state(uint8_t mods) {
    static const char PROGMEM spacing[] = {0x20, 0};
    render_mod_gui(mods & MOD_MASK_GUI);
    oled_write_P(spacing, false);
    render_mod_shift(mods & MOD_MASK_SHIFT);
    render_mod_ctrl(mods & MOD_MASK_CTRL);
    oled_write_P(spacing, false);
    render_mod_alt(mods & MOD_MASK_ALT);
}

/* 5x1 Media status row */
void render_mute(void) {
    static const char PROGMEM mute_icon[] = {0x20, 0x20, 0x85, 0x88, 0x20, 0};
    oled_write_P(mute_icon, false);
}

void render_volume_up(void) {
    static const char PROGMEM volu_icon[] = {0x20, 0x20, 0x85, 0x86, 0x20, 0};
    oled_write_P(volu_icon, false);
}

void render_volume_down(void) {
    static const char PROGMEM vold_icon[] = {0x20, 0x20, 0x85, 0x87, 0x20, 0};
    oled_write_P(vold_icon, false);
}

void render_media_next(void) {
    static const char PROGMEM mnxt_icon[] = {0x20, 0x20, 0xa5, 0xa6, 0x20, 0};
    oled_write_P(mnxt_icon, false);
}

void render_media_previous(void) {
    static const char PROGMEM mprv_icon[] = {0x20, 0x20, 0xa7, 0xa8, 0x20, 0};
    oled_write_P(mprv_icon, false);
}

void render_play(void) {
    static const char PROGMEM play[] = {0x20, 0x20, 0x10, 0x20, 0x20, 0};
    oled_write_P(play, false);
}

void render_pause(void) {
    static const char PROGMEM pause[] = {0x20, 0x20, 0x13, 0x20, 0x20, 0};
    oled_write_P(pause, false);
}

/**
 * Media keys will consume a status row, one at a time, based on the current
 * keycode pressed. They also will timeout relatively fast.
 */
void render_media_state(void) {
    switch (media_kc) {
        case KC_MUTE:
            render_mute();
            return;
        case KC_MSTP:
            render_pause();
            return;
        case KC_MPLY:
            render_play();
            return;
        case KC_VOLD:
            render_volume_down();
            return;
        case KC_VOLU:
            render_volume_up();
            return;
        case KC_MPRV:
            render_media_previous();
            return;
        case KC_MNXT:
            render_media_next();
            return;
        default:
            render_space();
            return;
    }
}

/* Logs recent keypresses (up to 5) and times out each press relatively fast. */
void render_keylogger_state(void) {
  oled_write(keylogger_str, false);
}

/* 5x3 logo */
void render_logo(void) {
    static const char PROGMEM corne_logo[] = {
        0x80, 0x81, 0x82, 0x83, 0x84,
        0xa0, 0xa1, 0xa2, 0xa3, 0xa4,
        0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0};
    oled_write_P(corne_logo, false);
    oled_write_P(PSTR("crkbd"), false);
}

/* 4x7 (padded to 5) avatar that toggles between 2 states */
void render_avatar(void) {
#ifdef USE_SOL_FONT
    static const char PROGMEM avatar[] = {
        0x20, 0x20, 0x8c, 0x8d, 0x20,
        0x8b, 0xcb, 0xac, 0xcc, 0x20,
        0xab, 0xad, 0xae, 0xcd, 0x20,
        0x20, 0x8e, 0xce, 0x8f, 0x20,
        0xaf, 0xcf, 0x90, 0xb0, 0x20,
        0xd0, 0x91, 0xb1, 0xd1, 0x20,
        0x92, 0xb2, 0xd2, 0x93, 0x20, 0};
#  ifdef ANIMATE_AVATAR
    static const char PROGMEM avatar_alt[] = {
        0x20, 0x20, 0x8c, 0x8d, 0x20,
        0x20, 0xcb, 0xac, 0x20, 0x20,
        0xd3, 0xad, 0xae, 0xb3, 0x20,
        0x20, 0x8e, 0xce, 0x8f, 0x20,
        0xaf, 0xcf, 0x90, 0xb0, 0x20,
        0xd0, 0x91, 0xb1, 0xd1, 0x20,
        0x92, 0xb2, 0xd2, 0x93, 0x20, 0};
#  endif
#endif
#ifdef USE_BUTT_FONT
    static const char PROGMEM avatar[] = {
        0x20, 0x20, 0x20, 0x20, 0x20,
        0x20, 0x20, 0x20, 0x20, 0x20,
        0x20, 0x20, 0x20, 0x20, 0x20,
        0x20, 0x8b, 0x8c, 0x8d, 0x8e,
        0x20, 0xab, 0xac, 0xad, 0xae,
        0x20, 0xcb, 0xcc, 0xcd, 0xce,
        0x20, 0x8f, 0x90, 0x91, 0x92, 0};
#  ifdef ANIMATE_AVATAR
    static const char PROGMEM avatar_alt[] = {
        0x20, 0x8b, 0x8c, 0x8d, 0x8e,
        0x20, 0xab, 0xac, 0xad, 0xb2,
        0x20, 0xaf, 0xb0, 0xb1, 0xb2,
        0x20, 0xaf, 0xb0, 0xb1, 0x93,
        0x20, 0xaf, 0xb0, 0xb1, 0xae,
        0x20, 0xcb, 0xcc, 0xcd, 0xce,
        0x20, 0x8f, 0x90, 0x91, 0x92, 0};
#  endif
#endif
#ifdef ANIMATE_AVATAR
    bool blink = (timer_read() % 1000) < 500;
    oled_write_P(blink? avatar : avatar_alt, false);
#else
    oled_write_P(avatar, false);
#endif
}
