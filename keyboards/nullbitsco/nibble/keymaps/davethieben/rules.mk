
OLED_ENABLE = yes
OLED_DRIVER = SSD1306
WPM_ENABLE = yes
# VIA_ENABLE = yes
CONSOLE_ENABLE = yes
MOUSEKEY_ENABLE = yes

# Nibble options
ANSI_ENABLE = 		yes
ISO_ENABLE = 		no
REMOTEKB_ENABLE = 	no
BITC_LED_ENABLE = 	no
BIG_LED_ENABLE = 	no


SRC += oled_display.c \
	   encoder.c
