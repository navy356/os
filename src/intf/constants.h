#pragma once

#include <stdint.h>
#include <stddef.h>
#include "multiboot.h"

#ifndef KEYCODES
#define KEYCODES

/* The different modifier keys we support */
#define MOD_NONE  0
#define MOD_CTRL  (1 << 0)
#define MOD_SHIFT (1 << 1)
#define MOD_ALT   (1 << 2)

#define KERNEL_OFFSET 0x40000000
#define MAPPED_END 0x00003fdfffff

extern void * multiboot_ptr;
extern unsigned long magic;
extern uint64_t kernel_end_virtual;
//extern multiboot_info_t *multiboot_ptr;
/* The modifier keys currently pressed */
static unsigned char mod_keys = 0;
/* Keycode definitions. */
/*#define KEY_LEFT_ARROW  0x4B
#define KEY_RIGHT_ARROW 0x4D
#define KEY_UP_ARROW    0x48
#define KEY_DOWN_ARROW  0x50
#define KEY_SPACE       0x39
#define KEY_ESC         0x01
#define KEY_A           0x1E
#define KEY_B           0x30
#define KEY_C           0x2E
#define KEY_D           0x20
#define KEY_E           0x12
#define KEY_F           0x21
#define KEY_G           0x22
#define KEY_H           0x23
#define KEY_I           0x17
#define KEY_J           0x24
#define KEY_K           0x25
#define KEY_L           0x26
#define KEY_M           0x32
#define KEY_N           0x31
#define KEY_O           0x18
#define KEY_P           0x19
#define KEY_Q           0x10
#define KEY_R           0x13
#define KEY_S           0x1F
#define KEY_T           0x14
#define KEY_U           0x16
#define KEY_V           0x2F
#define KEY_W           0x11
#define KEY_X           0x2D
#define KEY_Y           0x15
#define KEY_Z           0x2C
#define KEY_1           0x02
#define KEY_2           0x03
#define KEY_3           0x04
#define KEY_4           0x05
#define KEY_5           0x06
#define KEY_6           0x07
#define KEY_7           0x08
#define KEY_8           0x09
#define KEY_9           0x0A
#define KEY_0           0x0B
#define KEY_DASH        0x0C
#define KEY_EQUAL       0x0D
#define KEY_LBRACKET    0x1A
#define KEY_RBRACKET    0x1B
#define KEY_SEMICOLON   0x27
#define KEY_RQUOTE      0x28
#define KEY_LQUOTE      0x29
#define KEY_PEROID      0x33
#define KEY_COMMA       0x34
#define KEY_SLASH       0x35
#define KEY_BACKSLASH   0x2B
#define KEY_F1          0x3B
#define KEY_F2          0x3C
#define KEY_F3          0x3D
#define KEY_F4          0x3E
#define KEY_F5          0x3F
#define KEY_F6          0x40
#define KEY_F7          0x41
#define KEY_F8          0x42
#define KEY_F9          0x43
#define KEY_F10         0x44
#define KEY_F11         0x57
#define KEY_F12         0x58
#define KEY_ESC         0x01
#define KEY_BACKSPACE   0x0E
#define KEY_TAB         0x0F
#define KEY_ENTER       0x1C
#define KEY_CNTRL       0x1D
#define KEY_LSHIFT      0x2A
#define KEY_RSHIFT      0x36
#define KEY_PRTSCRN     0x37
#define KEY_ALT         0x38
#define KEY_SPACE       0x39
#define KEY_CAPS        0x3A
#define KEY_NUMLOCK     0x45
#define KEY_SCRLLLOCK   0x46
#define KEY_HOME        0x47
#define KEY_UP          0x48
#define KEY_PGUP        0x49
#define KEY_MINUS       0x4A
#define KEY_LEFT        0x4B
#define KEY_CENTER      0x4C
#define KEY_RIGHT       0x4D
#define KEY_PLUS        0x4E
#define KEY_END         0x4F
#define KEY_DOWN        0x50
#define KEY_PGDWN       0x51
#define KEY_INS         0x52
#define KEY_DEL         0x53
*/
#endif

enum KYBRD_ENCODER_IO { //ENCODER PORT

	KYBRD_ENC_INPUT_BUF	=	0x60,
	KYBRD_ENC_CMD_REG	=	0x60
};

enum KYBRD_ENC_CMDS {	//ENCODER COMMANDS

	KYBRD_ENC_CMD_SET_LED				=	0xED,
	KYBRD_ENC_CMD_ECHO					=	0xEE,
	KYBRD_ENC_CMD_SCAN_CODE_SET			=	0xF0,
	KYBRD_ENC_CMD_ID					=	0xF2,
	KYBRD_ENC_CMD_AUTODELAY				=	0xF3,
	KYBRD_ENC_CMD_ENABLE				=	0xF4,
	KYBRD_ENC_CMD_RESETWAIT				=	0xF5,
	KYBRD_ENC_CMD_RESETSCAN				=	0xF6,
	KYBRD_ENC_CMD_ALL_AUTO				=	0xF7,
	KYBRD_ENC_CMD_ALL_MAKEBREAK			=	0xF8,
	KYBRD_ENC_CMD_ALL_MAKEONLY			=	0xF9,
	KYBRD_ENC_CMD_ALL_MAKEBREAK_AUTO	=	0xFA,
	KYBRD_ENC_CMD_SINGLE_AUTOREPEAT		=	0xFB,
	KYBRD_ENC_CMD_SINGLE_MAKEBREAK		=	0xFC,
	KYBRD_ENC_CMD_SINGLE_BREAKONLY		=	0xFD,
	KYBRD_ENC_CMD_RESEND				=	0xFE,
	KYBRD_ENC_CMD_RESET					=	0xFF
};

// keyboard controller ---------------------------------------

enum KYBRD_CTRL_IO {  //PORT

	KYBRD_CTRL_STATS_REG=	0x64,
	KYBRD_CTRL_CMD_REG	=	0x64
};

enum KYBRD_CTRL_STATS_MASK {	//PORT STATUS (read this byte and do stuff)

	KYBRD_CTRL_STATS_MASK_OUT_BUF	=	1,		//00000001
	KYBRD_CTRL_STATS_MASK_IN_BUF	=	2,		//00000010
	KYBRD_CTRL_STATS_MASK_SYSTEM	=	4,		//00000100
	KYBRD_CTRL_STATS_MASK_CMD_DATA	=	8,		//00001000
	KYBRD_CTRL_STATS_MASK_LOCKED	=	0x10,	//00010000
	KYBRD_CTRL_STATS_MASK_AUX_BUF	=	0x20,	//00100000
	KYBRD_CTRL_STATS_MASK_TIMEOUT	=	0x40,	//01000000
	KYBRD_CTRL_STATS_MASK_PARITY	=	0x80	//10000000
};

enum KYBRD_CTRL_CMDS {			//CONTROLLER COMMANDS

	KYBRD_CTRL_CMD_READ				=	0x20,
	KYBRD_CTRL_CMD_WRITE			=	0x60,
	KYBRD_CTRL_CMD_SELF_TEST		=	0xAA,
	KYBRD_CTRL_CMD_INTERFACE_TEST	=	0xAB,
	KYBRD_CTRL_CMD_DISABLE			=	0xAD,
	KYBRD_CTRL_CMD_ENABLE			=	0xAE,
	KYBRD_CTRL_CMD_READ_IN_PORT		=	0xC0,
	KYBRD_CTRL_CMD_READ_OUT_PORT	=	0xD0,
	KYBRD_CTRL_CMD_WRITE_OUT_PORT	=	0xD1,
	KYBRD_CTRL_CMD_READ_TEST_INPUTS	=	0xE0,
	KYBRD_CTRL_CMD_SYSTEM_RESET		=	0xFE,
	KYBRD_CTRL_CMD_MOUSE_DISABLE	=	0xA7,
	KYBRD_CTRL_CMD_MOUSE_ENABLE		=	0xA8,
	KYBRD_CTRL_CMD_MOUSE_PORT_TEST	=	0xA9,
	KYBRD_CTRL_CMD_MOUSE_WRITE		=	0xD4
};

// scan error codes ------------------------------------------

enum KYBRD_ERROR {

	KYBRD_ERR_BUF_OVERRUN			=	0,
	KYBRD_ERR_ID_RET				=	0x83AB,
	KYBRD_ERR_BAT					=	0xAA,	//note: can also be L. shift key make code
	KYBRD_ERR_ECHO_RET				=	0xEE,
	KYBRD_ERR_ACK					=	0xFA,
	KYBRD_ERR_BAT_FAILED			=	0xFC,
	KYBRD_ERR_DIAG_FAILED			=	0xFD,
	KYBRD_ERR_RESEND_CMD			=	0xFE,
	KYBRD_ERR_KEY					=	0xFF
};

const static int NUM_COLS = 80;
const static int NUM_ROWS = 25;

enum KEYCODE {  //In this project we CAN'T PRINT ALL OF THIS. Can you fix it? :)

// Alphanumeric keys ////////////////

	KEY_SPACE             = ' ',
	KEY_0                 = '0',
	KEY_1                 = '1',
	KEY_2                 = '2',
	KEY_3                 = '3',
	KEY_4                 = '4',
	KEY_5                 = '5',
	KEY_6                 = '6',
	KEY_7                 = '7',
	KEY_8                 = '8',
	KEY_9                 = '9',

	KEY_A                 = 'a',
	KEY_B                 = 'b',
	KEY_C                 = 'c',
	KEY_D                 = 'd',
	KEY_E                 = 'e',
	KEY_F                 = 'f',
	KEY_G                 = 'g',
	KEY_H                 = 'h',
	KEY_I                 = 'i',
	KEY_J                 = 'j',
	KEY_K                 = 'k',
	KEY_L                 = 'l',
	KEY_M                 = 'm',
	KEY_N                 = 'n',
	KEY_O                 = 'o',
	KEY_P                 = 'p',
	KEY_Q                 = 'q',
	KEY_R                 = 'r',
	KEY_S                 = 's',
	KEY_T                 = 't',
	KEY_U                 = 'u',
	KEY_V                 = 'v',
	KEY_W                 = 'w',
	KEY_X                 = 'x',
	KEY_Y                 = 'y',
	KEY_Z                 = 'z',

	KEY_RETURN            = '\r',
	KEY_ESCAPE            = 0x1001,
	KEY_BACKSPACE         = '\b',

// Arrow keys ////////////////////////

	KEY_UP                = 0x1100,
	KEY_DOWN              = 0x1101,
	KEY_LEFT              = 0x1102,
	KEY_RIGHT             = 0x1103,

// Function keys /////////////////////

	KEY_F1                = 0x1201,
	KEY_F2                = 0x1202,
	KEY_F3                = 0x1203,
	KEY_F4                = 0x1204,
	KEY_F5                = 0x1205,
	KEY_F6                = 0x1206,
	KEY_F7                = 0x1207,
	KEY_F8                = 0x1208,
	KEY_F9                = 0x1209,
	KEY_F10               = 0x120a,
	KEY_F11               = 0x120b,
	KEY_F12               = 0x120b,
	KEY_F13               = 0x120c,
	KEY_F14               = 0x120d,
	KEY_F15               = 0x120e,

	KEY_DOT               = '.',
	KEY_COMMA             = ',',
	KEY_COLON             = ':',
	KEY_SEMICOLON         = ';',
	KEY_SLASH             = '/',
	KEY_BACKSLASH         = '\\',
	KEY_PLUS              = '+',
	KEY_MINUS             = '-',
	KEY_ASTERISK          = '*',
	KEY_EXCLAMATION       = '!',
	KEY_QUESTION          = '?',
	KEY_QUOTEDOUBLE       = '\"',
	KEY_QUOTE             = '\'',
	KEY_EQUAL             = '=',
	KEY_HASH              = '#',
	KEY_PERCENT           = '%',
	KEY_AMPERSAND         = '&',
	KEY_UNDERSCORE        = '_',
	KEY_LEFTPARENTHESIS   = '(',
	KEY_RIGHTPARENTHESIS  = ')',
	KEY_LEFTBRACKET       = '[',
	KEY_RIGHTBRACKET      = ']',
	KEY_LEFTCURL          = '{',
	KEY_RIGHTCURL         = '}',
	KEY_DOLLAR            = '$',
	KEY_POUND             = ' ',//'£',
	KEY_EURO              = '$',
	KEY_LESS              = '<',
	KEY_GREATER           = '>',
	KEY_BAR               = '|',
	KEY_GRAVE             = '`',
	KEY_TILDE             = '~',
	KEY_AT                = '@',
	KEY_CARRET            = '^',

// Numeric keypad //////////////////////

	KEY_KP_0              = '0',
	KEY_KP_1              = '1',
	KEY_KP_2              = '2',
	KEY_KP_3              = '3',
	KEY_KP_4              = '4',
	KEY_KP_5              = '5',
	KEY_KP_6              = '6',
	KEY_KP_7              = '7',
	KEY_KP_8              = '8',
	KEY_KP_9              = '9',
	KEY_KP_PLUS           = '+',
	KEY_KP_MINUS          = '-',
	KEY_KP_DECIMAL        = '.',
	KEY_KP_DIVIDE         = '/',
	KEY_KP_ASTERISK       = '*',
	KEY_KP_NUMLOCK        = 0x300f,
	KEY_KP_ENTER          = 0x3010,

	KEY_TAB               = 0x4000,
	KEY_CAPSLOCK          = 0x4001,

// Modify keys ////////////////////////////

	KEY_LSHIFT            = 0x4002,
	KEY_LCTRL             = 0x4003,
	KEY_LALT              = 0x4004,
	KEY_LWIN              = 0x4005,
	KEY_RSHIFT            = 0x4006,
	KEY_RCTRL             = 0x4007,
	KEY_RALT              = 0x4008,
	KEY_RWIN              = 0x4009,

	KEY_INSERT            = 0x400a,
	KEY_DELETE            = 0x400b,
	KEY_HOME              = 0x400c,
	KEY_END               = 0x400d,
	KEY_PAGEUP            = 0x400e,
	KEY_PAGEDOWN          = 0x400f,
	KEY_SCROLLLOCK        = 0x4010,
	KEY_PAUSE             = 0x4011,

	KEY_UNKNOWN,
	KEY_NUMKEYCODES
};

static uint16_t _kkybrd_scancode_std [] = {

	//! key			scancode
	KEY_UNKNOWN,	//0
	KEY_ESCAPE,		//1
	KEY_1,			//2
	KEY_2,			//3
	KEY_3,			//4
	KEY_4,			//5
	KEY_5,			//6
	KEY_6,			//7
	KEY_7,			//8
	KEY_8,			//9
	KEY_9,			//0xa
	KEY_0,			//0xb
	KEY_MINUS,		//0xc
	KEY_EQUAL,		//0xd
	KEY_BACKSPACE,	//0xe
	KEY_TAB,		//0xf
	KEY_Q,			//0x10
	KEY_W,			//0x11
	KEY_E,			//0x12
	KEY_R,			//0x13
	KEY_T,			//0x14
	KEY_Y,			//0x15
	KEY_U,			//0x16
	KEY_I,			//0x17
	KEY_O,			//0x18
	KEY_P,			//0x19
	KEY_LEFTBRACKET,//0x1a
	KEY_RIGHTBRACKET,//0x1b
	KEY_RETURN,		//0x1c
	KEY_LCTRL,		//0x1d
	KEY_A,			//0x1e
	KEY_S,			//0x1f
	KEY_D,			//0x20
	KEY_F,			//0x21
	KEY_G,			//0x22
	KEY_H,			//0x23
	KEY_J,			//0x24
	KEY_K,			//0x25
	KEY_L,			//0x26
	KEY_SEMICOLON,	//0x27
	KEY_QUOTE,		//0x28
	KEY_GRAVE,		//0x29
	KEY_LSHIFT,		//0x2a
	KEY_BACKSLASH,	//0x2b
	KEY_Z,			//0x2c
	KEY_X,			//0x2d
	KEY_C,			//0x2e
	KEY_V,			//0x2f
	KEY_B,			//0x30
	KEY_N,			//0x31
	KEY_M,			//0x32
	KEY_COMMA,		//0x33
	KEY_DOT,		//0x34
	KEY_SLASH,		//0x35
	KEY_RSHIFT,		//0x36
	KEY_KP_ASTERISK,//0x37
	KEY_RALT,		//0x38
	KEY_SPACE,		//0x39
	KEY_CAPSLOCK,	//0x3a
	KEY_F1,			//0x3b
	KEY_F2,			//0x3c
	KEY_F3,			//0x3d
	KEY_F4,			//0x3e
	KEY_F5,			//0x3f
	KEY_F6,			//0x40
	KEY_F7,			//0x41
	KEY_F8,			//0x42
	KEY_F9,			//0x43
	KEY_F10,		//0x44
	KEY_KP_NUMLOCK,	//0x45
	KEY_SCROLLLOCK,	//0x46
	KEY_HOME,		//0x47
	KEY_KP_8,		//0x48	//keypad up arrow
	KEY_PAGEUP,		//0x49
	KEY_KP_2,		//0x50	//keypad down arrow
	KEY_KP_3,		//0x51	//keypad page down
	KEY_KP_0,		//0x52	//keypad insert key
	KEY_KP_DECIMAL,	//0x53	//keypad delete key
	KEY_UNKNOWN,	//0x54
	KEY_UNKNOWN,	//0x55
	KEY_UNKNOWN,	//0x56
	KEY_F11,		//0x57
	KEY_F12			//0x58
};