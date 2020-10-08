/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>
Copyright 2020 @krockpot

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

/* Select hand configuration */
#define EE_HANDS
#define SPLIT_USB_DETECT

#define USE_SERIAL_PD2

/* Configure Mod Tap */
#define TAPPING_FORCE_HOLD
#define TAPPING_TERM 200

/* OLED Configuration */
#define BUTT_FONT   "keyboards/crkbd/keymaps/krockpot/glcdfonts/butt.c"
#define SOL_FONT    "keyboards/crkbd/keymaps/krockpot/glcdfonts/sol.c"
#ifdef OLED_DRIVER_ENABLE
#  define USE_SOL_FONT    // Select font with USE_<font_name>
#  define ANIMATE_AVATAR  // Setting to make avatar toggle between alt mode
#  ifdef USE_SOL_FONT
#    define OLED_FONT_H SOL_FONT
#  elif defined(USE_BUTT_FONT)
#    define OLED_FONT_H BUTT_FONT
#  endif
#  define OS_ICON       0 // 0 is Windows, 1 is Mac
#  undef SSD1306OLED
#endif
