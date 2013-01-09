//////////////////////////////////////////////////////////////////////////////
/// @file CodeEvenement.h
/// @author Charles-Étienne Lalonde
/// @date 2012-01-20
/// @version 1.0 
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

/**
	Copier-coller des codes de touches de KeyEvent.java
	pour avoir accès à ceux-ci dans le code C++.
*/

#ifndef __INTERFACE_CODEEVENEMENTS_H__
#define __INTERFACE_CODEEVENEMENTS_H__
    /* Virtual key codes. */

    #define VJAK_ENTER           '\n'
    #define VJAK_BACK_SPACE      '\b'
    #define VJAK_TAB             '\t'
    #define VJAK_CANCEL          0x03
    #define VJAK_CLEAR 0x0C
    #define VJAK_SHIFT           0x10
    #define VJAK_CONTROL         0x11
    #define VJAK_ALT             0x12
    #define VJAK_PAUSE           0x13
    #define VJAK_CAPS_LOCK       0x14
    #define VJAK_ESCAPE          0x1B
    #define VJAK_SPACE           0x20
    #define VJAK_PAGE_UP         0x21
    #define VJAK_PAGE_DOWN       0x22
    #define VJAK_END             0x23
    #define VJAK_HOME            0x24

    /**
     * Constant for the non-numpad <b>left</b> arrow key.
     * @see #VJAK_KP_LEFT
     */
    #define VJAK_LEFT            0x25

    /**
     * Constant for the non-numpad <b>up</b> arrow key.
     * @see #VJAK_KP_UP
     */
    #define VJAK_UP              0x26

    /**
     * Constant for the non-numpad <b>right</b> arrow key.
     * @see #VJAK_KP_RIGHT
     */
    #define VJAK_RIGHT           0x27

    /**
     * Constant for the non-numpad <b>down</b> arrow key.
     * @see #VJAK_KP_DOWN
     */
    #define VJAK_DOWN            0x28

    /**
     * Constant for the comma key, ","
     */
    #define VJAK_COMMA           0x2C

    /**
     * Constant for the minus key, "-"
     * @since 1.2
     */
    #define VJAK_MINUS           0x2D

    /**
     * Constant for the period key, "."
     */
    #define VJAK_PERIOD          0x2E

    /**
     * Constant for the forward slash key, "/"
     */
    #define VJAK_SLASH           0x2F

    /** VJAK_0 thru VJAK_9 are the same as ASCII '0' thru '9' (0x30 - 0x39) */
    #define VJAK_0               0x30
    #define VJAK_1               0x31
    #define VJAK_2               0x32
    #define VJAK_3               0x33
    #define VJAK_4               0x34
    #define VJAK_5               0x35
    #define VJAK_6               0x36
    #define VJAK_7               0x37
    #define VJAK_8               0x38
    #define VJAK_9               0x39

    /**
     * Constant for the semicolon key, ""
     */
    #define VJAK_SEMICOLON       0x3B

    /**
     * Constant for the equals key, ""
     */
    #define VJAK_EQUALS          0x3D

    /** VJAK_A thru VJAK_Z are the same as ASCII 'A' thru 'Z' (0x41 - 0x5A) */
    #define VJAK_A               0x41
    #define VJAK_B               0x42
    #define VJAK_C               0x43
    #define VJAK_D               0x44
    #define VJAK_E               0x45
    #define VJAK_F               0x46
    #define VJAK_G               0x47
    #define VJAK_H               0x48
    #define VJAK_I               0x49
    #define VJAK_J               0x4A
    #define VJAK_K               0x4B
    #define VJAK_L               0x4C
    #define VJAK_M               0x4D
    #define VJAK_N               0x4E
    #define VJAK_O               0x4F
    #define VJAK_P               0x50
    #define VJAK_Q               0x51
    #define VJAK_R               0x52
    #define VJAK_S               0x53
    #define VJAK_T               0x54
    #define VJAK_U               0x55
    #define VJAK_V               0x56
    #define VJAK_W               0x57
    #define VJAK_X               0x58
    #define VJAK_Y               0x59
    #define VJAK_Z               0x5A

    /**
     * Constant for the open bracket key, "["
     */
    #define VJAK_OPEN_BRACKET    0x5B

    /**
     * Constant for the back slash key, "\"
     */
    #define VJAK_BACK_SLASH      0x5C

    /**
     * Constant for the close bracket key, "]"
     */
    #define VJAK_CLOSE_BRACKET   0x5D

    #define VJAK_NUMPAD0         0x60
    #define VJAK_NUMPAD1         0x61
    #define VJAK_NUMPAD2         0x62
    #define VJAK_NUMPAD3         0x63
    #define VJAK_NUMPAD4         0x64
    #define VJAK_NUMPAD5         0x65
    #define VJAK_NUMPAD6         0x66
    #define VJAK_NUMPAD7         0x67
    #define VJAK_NUMPAD8         0x68
    #define VJAK_NUMPAD9         0x69
    #define VJAK_MULTIPLY        0x6A
    #define VJAK_ADD             0x6B

    /** 
     * This constant is obsolete, and is included only for backwards
     * compatibility.
     * @see #VJAK_SEPARATOR
     */
    #define VJAK_SEPARATER       0x6C

    /** 
     * Constant for the Numpad Separator key. 
     * @since 1.4
     */
    #define VJAK_SEPARATOR       VJAK_SEPARATER

    #define VJAK_SUBTRACT        0x6D
    #define VJAK_DECIMAL         0x6E
    #define VJAK_DIVIDE          0x6F
    #define VJAK_DELETE          0x7F /* ASCII DEL */
    #define VJAK_NUM_LOCK        0x90
    #define VJAK_SCROLL_LOCK     0x91

    /** Constant for the F1 function key. */
    #define VJAK_F1              0x70

    /** Constant for the F2 function key. */
    #define VJAK_F2              0x71

    /** Constant for the F3 function key. */
    #define VJAK_F3              0x72

    /** Constant for the F4 function key. */
    #define VJAK_F4              0x73

    /** Constant for the F5 function key. */
    #define VJAK_F5              0x74

    /** Constant for the F6 function key. */
    #define VJAK_F6              0x75

    /** Constant for the F7 function key. */
    #define VJAK_F7              0x76

    /** Constant for the F8 function key. */
    #define VJAK_F8              0x77

    /** Constant for the F9 function key. */
    #define VJAK_F9              0x78

    /** Constant for the F10 function key. */
    #define VJAK_F10             0x79

    /** Constant for the F11 function key. */
    #define VJAK_F11             0x7A

    /** Constant for the F12 function key. */
    #define VJAK_F12             0x7B

    /**
     * Constant for the F13 function key.
     * @since 1.2
     */
    /* F13 - F24 are used on IBM 3270 keyboard use random range for constants. */
    #define VJAK_F13             0xF000
 
    /**
     * Constant for the F14 function key.
     * @since 1.2
     */
    #define VJAK_F14             0xF001
 
    /**
     * Constant for the F15 function key.
     * @since 1.2
     */
    #define VJAK_F15             0xF002
 
    /**
     * Constant for the F16 function key.
     * @since 1.2
     */
    #define VJAK_F16             0xF003
 
    /**
     * Constant for the F17 function key.
     * @since 1.2
     */
    #define VJAK_F17             0xF004
 
    /**
     * Constant for the F18 function key.
     * @since 1.2
     */
    #define VJAK_F18             0xF005
 
    /**
     * Constant for the F19 function key.
     * @since 1.2
     */
    #define VJAK_F19             0xF006
 
    /**
     * Constant for the F20 function key.
     * @since 1.2
     */
    #define VJAK_F20             0xF007
 
    /**
     * Constant for the F21 function key.
     * @since 1.2
     */
    #define VJAK_F21             0xF008
 
    /**
     * Constant for the F22 function key.
     * @since 1.2
     */
    #define VJAK_F22             0xF009
 
    /**
     * Constant for the F23 function key.
     * @since 1.2
     */
    #define VJAK_F23             0xF00A
 
    /**
     * Constant for the F24 function key.
     * @since 1.2
     */
    #define VJAK_F24             0xF00B
 
    #define VJAK_PRINTSCREEN     0x9A
    #define VJAK_INSERT          0x9B
    #define VJAK_HELP            0x9C
    #define VJAK_META            0x9D

    #define VJAK_BACK_QUOTE      0xC0
    #define VJAK_QUOTE           0xDE

    /**
     * Constant for the numeric keypad <b>up</b> arrow key.
     * @see #VJAK_UP
     * @since 1.2
     */
    #define VJAK_KP_UP           0xE0

    /**
     * Constant for the numeric keypad <b>down</b> arrow key.
     * @see #VJAK_DOWN
     * @since 1.2
     */
    #define VJAK_KP_DOWN         0xE1

    /**
     * Constant for the numeric keypad <b>left</b> arrow key.
     * @see #VJAK_LEFT
     * @since 1.2
     */
    #define VJAK_KP_LEFT         0xE2

    /**
     * Constant for the numeric keypad <b>right</b> arrow key.
     * @see #VJAK_RIGHT
     * @since 1.2
     */
    #define VJAK_KP_RIGHT        0xE3
    
    /* For European keyboards */
    /** @since 1.2 */
    #define VJAK_DEAD_GRAVE                0x80
    /** @since 1.2 */
    #define VJAK_DEAD_ACUTE                0x81
    /** @since 1.2 */
    #define VJAK_DEAD_CIRCUMFLEX           0x82
    /** @since 1.2 */
    #define VJAK_DEAD_TILDE                0x83
    /** @since 1.2 */
    #define VJAK_DEAD_MACRON               0x84
    /** @since 1.2 */
    #define VJAK_DEAD_BREVE                0x85
    /** @since 1.2 */
    #define VJAK_DEAD_ABOVEDOT             0x86
    /** @since 1.2 */
    #define VJAK_DEAD_DIAERESIS            0x87
    /** @since 1.2 */
    #define VJAK_DEAD_ABOVERING            0x88
    /** @since 1.2 */
    #define VJAK_DEAD_DOUBLEACUTE          0x89
    /** @since 1.2 */
    #define VJAK_DEAD_CARON                0x8a
    /** @since 1.2 */
    #define VJAK_DEAD_CEDILLA              0x8b
    /** @since 1.2 */
    #define VJAK_DEAD_OGONEK               0x8c
    /** @since 1.2 */
    #define VJAK_DEAD_IOTA                 0x8d
    /** @since 1.2 */
    #define VJAK_DEAD_VOICED_SOUND         0x8e
    /** @since 1.2 */
    #define VJAK_DEAD_SEMIVOICED_SOUND     0x8f

    /** @since 1.2 */
    #define VJAK_AMPERSAND                 0x96
    /** @since 1.2 */
    #define VJAK_ASTERISK                  0x97
    /** @since 1.2 */
    #define VJAK_QUOTEDBL                  0x98
    /** @since 1.2 */
    #define VJAK_LESS                      0x99

    /** @since 1.2 */
    #define VJAK_GREATER                   0xa0
    /** @since 1.2 */
    #define VJAK_BRACELEFT                 0xa1
    /** @since 1.2 */
    #define VJAK_BRACERIGHT                0xa2

    /**
     * Constant for the "@" key.
     * @since 1.2
     */
    #define VJAK_AT                        0x0200
 
    /**
     * Constant for the ":" key.
     * @since 1.2
     */
    #define VJAK_COLON                     0x0201
 
    /**
     * Constant for the "^" key.
     * @since 1.2
     */
    #define VJAK_CIRCUMFLEX                0x0202
 
    /**
     * Constant for the "$" key.
     * @since 1.2
     */
    #define VJAK_DOLLAR                    0x0203
 
    /**
     * Constant for the Euro currency sign key.
     * @since 1.2
     */
    #define VJAK_EURO_SIGN                 0x0204
 
    /**
     * Constant for the "!" key.
     * @since 1.2
     */
    #define VJAK_EXCLAMATION_MARK          0x0205
 
    /**
     * Constant for the inverted exclamation mark key.
     * @since 1.2
     */
    #define VJAK_INVERTED_EXCLAMATION_MARK  0x0206
 
    /**
     * Constant for the "(" key.
     * @since 1.2
     */
    #define VJAK_LEFT_PARENTHESIS          0x0207
 
    /**
     * Constant for the "#" key.
     * @since 1.2
     */
    #define VJAK_NUMBER_SIGN               0x0208
 
    /**
     * Constant for the "+" key.
     * @since 1.2
     */
    #define VJAK_PLUS                      0x0209
 
    /**
     * Constant for the ")" key.
     * @since 1.2
     */
    #define VJAK_RIGHT_PARENTHESIS         0x020A
 
    /**
     * Constant for the "_" key.
     * @since 1.2
     */
    #define VJAK_UNDERSCORE                0x020B
 
    /**
     * Constant for the Microsoft Windows "Windows" key.
     * It is used for both the left and right version of the key.  
     * @see #getKeyLocation()
     * @since 1.5
     */
    #define VJAK_WINDOWS                   0x020C
 
    /**
     * Constant for the Microsoft Windows Context Menu key.
     * @since 1.5
     */
    #define VJAK_CONTEXT_MENU              0x020D
 
    /* for input method support on Asian Keyboards */

    /* not clear what this means - listed in Microsoft Windows API */
    #define VJAK_FINAL                     0x0018
    
    /** Constant for the Convert function key. */
    /* Japanese PC 106 keyboard, Japanese Solaris keyboard: henkan */
    #define VJAK_CONVERT                   0x001C

    /** Constant for the Don't Convert function key. */
    /* Japanese PC 106 keyboard: muhenkan */
    #define VJAK_NONCONVERT                0x001D
    
    /** Constant for the Accept or Commit function key. */
    /* Japanese Solaris keyboard: kakutei */
    #define VJAK_ACCEPT                    0x001E

    /* not clear what this means - listed in Microsoft Windows API */
    #define VJAK_MODECHANGE                0x001F

    /* replaced by VJAK_KANA_LOCK for Microsoft Windows and Solaris 
       might still be used on other platforms */
    #define VJAK_KANA                      0x0015

    /* replaced by VJAK_INPUT_METHOD_ON_OFF for Microsoft Windows and Solaris 
       might still be used for other platforms */
    #define VJAK_KANJI                     0x0019

    /**
     * Constant for the Alphanumeric function key.
     * @since 1.2
     */
    /* Japanese PC 106 keyboard: eisuu */
    #define VJAK_ALPHANUMERIC              0x00F0
 
    /**
     * Constant for the Katakana function key.
     * @since 1.2
     */
    /* Japanese PC 106 keyboard: katakana */
    #define VJAK_KATAKANA                  0x00F1
 
    /**
     * Constant for the Hiragana function key.
     * @since 1.2
     */
    /* Japanese PC 106 keyboard: hiragana */
    #define VJAK_HIRAGANA                  0x00F2
 
    /**
     * Constant for the Full-Width Characters function key.
     * @since 1.2
     */
    /* Japanese PC 106 keyboard: zenkaku */
    #define VJAK_FULL_WIDTH                0x00F3
 
    /**
     * Constant for the Half-Width Characters function key.
     * @since 1.2
     */
    /* Japanese PC 106 keyboard: hankaku */
    #define VJAK_HALF_WIDTH                0x00F4
 
    /**
     * Constant for the Roman Characters function key.
     * @since 1.2
     */
    /* Japanese PC 106 keyboard: roumaji */
    #define VJAK_ROMAN_CHARACTERS          0x00F5
 
    /**
     * Constant for the All Candidates function key.
     * @since 1.2
     */
    /* Japanese PC 106 keyboard - VJAK_CONVERT + ALT: zenkouho */
    #define VJAK_ALL_CANDIDATES            0x0100
 
    /**
     * Constant for the Previous Candidate function key.
     * @since 1.2
     */
    /* Japanese PC 106 keyboard - VJAK_CONVERT + SHIFT: maekouho */
    #define VJAK_PREVIOUS_CANDIDATE        0x0101
 
    /**
     * Constant for the Code Input function key.
     * @since 1.2
     */
    /* Japanese PC 106 keyboard - VJAK_ALPHANUMERIC + ALT: kanji bangou */
    #define VJAK_CODE_INPUT                0x0102
 
    /**
     * Constant for the Japanese-Katakana function key.
     * This key switches to a Japanese input method and selects its Katakana input mode.
     * @since 1.2
     */
    /* Japanese Macintosh keyboard - VJAK_JAPANESE_HIRAGANA + SHIFT */
    #define VJAK_JAPANESE_KATAKANA         0x0103
 
    /**
     * Constant for the Japanese-Hiragana function key.
     * This key switches to a Japanese input method and selects its Hiragana input mode.
     * @since 1.2
     */
    /* Japanese Macintosh keyboard */
    #define VJAK_JAPANESE_HIRAGANA         0x0104
 
    /**
     * Constant for the Japanese-Roman function key.
     * This key switches to a Japanese input method and selects its Roman-Direct input mode.
     * @since 1.2
     */
    /* Japanese Macintosh keyboard */
    #define VJAK_JAPANESE_ROMAN            0x0105

    /**
     * Constant for the locking Kana function key.
     * This key locks the keyboard into a Kana layout.
     * @since 1.3
     */
    /* Japanese PC 106 keyboard with special Windows driver - eisuu + Control Japanese Solaris keyboard: kana */
    #define VJAK_KANA_LOCK                 0x0106

    /**
     * Constant for the input method on/off key.
     * @since 1.3
     */
    /* Japanese PC 106 keyboard: kanji. Japanese Solaris keyboard: nihongo */
    #define VJAK_INPUT_METHOD_ON_OFF       0x0107

    /* for Sun keyboards */
    /** @since 1.2 */
    #define VJAK_CUT                       0xFFD1
    /** @since 1.2 */
    #define VJAK_COPY                      0xFFCD
    /** @since 1.2 */
    #define VJAK_PASTE                     0xFFCF
    /** @since 1.2 */
    #define VJAK_UNDO                      0xFFCB
    /** @since 1.2 */
    #define VJAK_AGAIN                     0xFFC9
    /** @since 1.2 */
    #define VJAK_FIND                      0xFFD0
    /** @since 1.2 */
    #define VJAK_PROPS                     0xFFCA
    /** @since 1.2 */
    #define VJAK_STOP                      0xFFC8
    
    /**
     * Constant for the Compose function key.
     * @since 1.2
     */
    #define VJAK_COMPOSE                   0xFF20
 
    /**
     * Constant for the AltGraph function key.
     * @since 1.2
     */
    #define VJAK_ALT_GRAPH                 0xFF7E

    /**
     * Constant for the Begin key.
     * @since 1.5
     */
    #define VJAK_BEGIN                     0xFF58

    /**
     * This value is used to indicate that the keyCode is unknown.
     * KEY_TYPED events do not have a keyCode value this value 
     * is used instead.  
     */
    #define VJAK_UNDEFINED       0x0

    /**
     * KEY_PRESSED and KEY_RELEASED events which do not map to a
     * valid Unicode character use this for the keyChar value.
     */
    #define CHAR_UNDEFINED    0xFFFF

    /**
     * A constant indicating that the keyLocation is indeterminate
     * or not relevant.
     * <code>KEY_TYPED</code> events do not have a keyLocation this value
     * is used instead.
     * @since 1.4
     */
    #define KEY_LOCATION_UNKNOWN   0

    /**
     * A constant indicating that the key pressed or released
     * is not distinguished as the left or right version of a key,
     * and did not originate on the numeric keypad (or did not
     * originate with a virtual key corresponding to the numeric
     * keypad).
     * @since 1.4
     */
    #define KEY_LOCATION_STANDARD  1

    /**
     * A constant indicating that the key pressed or released is in
     * the left key location (there is more than one possible location
     * for this key).  Example: the left shift key.
     * @since 1.4
     */
    #define KEY_LOCATION_LEFT      2

    /**
     * A constant indicating that the key pressed or released is in
     * the right key location (there is more than one possible location
     * for this key).  Example: the right shift key.
     * @since 1.4
     */
    #define KEY_LOCATION_RIGHT     3

    /**
     * A constant indicating that the key event originated on the
     * numeric keypad or with a virtual key corresponding to the
     * numeric keypad.
     * @since 1.4
     */
    #define KEY_LOCATION_NUMPAD    4

#endif

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


