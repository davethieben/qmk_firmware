
OLED_ENABLE 				= yes
OLED_DRIVER = SSD1306

AUTO_SHIFT_ENABLE 			= no
CONSOLE_ENABLE 				= yes
COMMAND_ENABLE 				= no
KEY_OVERRIDE_ENABLE 		= no
MAGIC_ENABLE 				= no
MOUSEKEY_ENABLE 			= yes
SPACE_CADET_ENABLE 			= no
TAP_DANCE_ENABLE 			= no
VELOCIKEY_ENABLE 			= no
# VIA_ENABLE 				= yes
WPM_ENABLE 					= no
RAW_ENABLE 					= yes

# Nibble options
ANSI_ENABLE = 		yes
ISO_ENABLE = 		no
REMOTEKB_ENABLE = 	no
BITC_LED_ENABLE = 	no
BIG_LED_ENABLE = 	no


SRC += oled_display.c \
	   layer_lights.c \
	   encoder.c \
	   rawhid.c
