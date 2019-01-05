/* Copyright 2018 Eucalyn
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H
#ifdef PROTOCOL_LUFA
#include "lufa.h"
#include "split_util.h"
#endif

// JIS Key MAP for Windows OS
#include "keymap_jp.h"

extern keymap_config_t keymap_config;

#ifdef RGBLIGHT_ENABLE
//Following line allows macro to read current RGB settings
extern rgblight_config_t rgblight_config;

//RGB LIGHT COLOR SETTING 0x00 - 0xFF
//暗めに設定している
#define RGB_COLOR_NONE  0x00, 0x00, 0x00
#define RGB_COLOR_RED   0x02, 0x00, 0x00
#define RGB_COLOR_GREEN 0x00, 0x02, 0x00
#define RGB_COLOR_BLUE  0x00, 0x00, 0x02
#define RGB_COLOR_PERPLE 0x10, 0x02, 0x10
#define RGB_COLOR_WHITE 0x02, 0x02, 0x02


#endif

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
// レイヤ名定義
// レイヤーの値は大きくし過ぎると、実際の定義有無に関係なくファームサイズを確保してしまう。
// firmware のサイズが大きくなるので、小さい値に収めること。(90とか入れるとサイズオーバーする）
#define _WINUS     0  // USキー配列キーボードで、WINDOWS JIS 接続。記号はUS配列のまま利用できる
#define _SHIFT     1  // 上記シフト時用レイヤ
#define _MOVE      3  // 移動、マクロなど各種機能用レイヤ
#define _MACUS     5  // US MACにキーボードを接続して利用する場合のレイヤ 接続後に _RGBFNC レイヤの Mキーにて切り替え
                      // 戻るときは右矢印の上にあるキーにて戻る
#define _MACSHIFT  6  // 上記シフト時用レイヤ
#define _MACMOVE   7  // US MACを利用するときの機能用レイヤ
#define _RGBFNC   10  // MINT60の初期設定機能レイヤを一部改変


// キーコード別名定義
#define _______ KC_TRNS            // 若番側レイヤのキーコードを使う
#define XXXXXXX KC_NO              // 無効キー
#define ALT_F4  LALT(KC_F4)        // ALT + F4
#define C_A_D   LCTL(LALT(KC_DEL)) // CTRL + ALT + DELETE

// 独自キーコード定義
enum custom_keycodes {
  RGBRST = SAFE_RANGE,
  WN_SCLN,              // JIS の「;」と「:」
  MCR1                  // マクロ1
};

// レイヤ定義
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // USキーボードでWINDOWS JIS接続基本レイヤ
  [_WINUS] = LAYOUT( \
    KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,      KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    JP_MINS, JP_EQL,    KC_BSPC,  \
    KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,      KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    JP_LBRC, JP_RBRC, JP_YEN,  \
    KC_LCTL,     KC_A,    KC_S,    KC_D,    KC_F,    KC_G,      KC_H,    KC_J,    KC_K,    KC_L,    WN_SCLN, JP_QUOT,        KC_ENT,   \
 LM(_SHIFT,MOD_LSFT), KC_Z, KC_X,    KC_C,    KC_V,    KC_B,      KC_N,    KC_M,  JP_COMM, JP_DOT,  JP_SLSH, LM(_SHIFT,MOD_RSFT),   KC_UP, KC_CALC, \
    MO(_RGBFNC),    KC_LCTL,  KC_LGUI,   KC_LALT,  LT(_MOVE,KC_SPC),        KC_SPC,  LT(_MOVE,KC_SPC),  LALT(KC_GRV),        KC_LEFT,KC_DOWN,KC_RGHT \
  ),

  // USキーボードでWINDOWS JIS接続のSHIFT時レイヤ _______ の透過部分は MOD_LSFT でシフトキー押下状態で透過
  [_SHIFT] = LAYOUT( \
    JP_TILD, JP_EXLM,   JP_AT, JP_HASH,  JP_DLR, JP_PERC,   JP_CIRC, JP_AMPR, JP_ASTR, JP_LPRN, JP_RPRN, JP_UNDS, JP_PLUS, _______,       \
    _______,   _______, _______, _______, _______, _______,   _______, _______, _______, _______, _______, JP_LCBR, JP_RCBR, JP_PIPE,     \
    _______,     _______, _______, _______, _______, _______,   _______, _______, _______, _______, _______,    JP_DQT,      _______,   \
    XXXXXXX,       _______, _______, _______, _______, _______,   _______, _______,  JP_LT,   JP_GT, JP_QUES, _______, _______, _______, \
    _______,   _______,   _______,   _______,     _______,        _______, _______, _______, _______, _______, _______ \
  ),

  // USキーボードでWINDOWS JIS接続の移動時レイヤ vim キーバインドライク
  [_MOVE] = LAYOUT( \
     JP_GRV,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,     KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_PSCR,     \
    _______,     C_A_D,  ALT_F4, _______, _______, _______,   _______, KC_PGUP, KC_HOME, _______, _______, _______, _______, _______,     \
    KC_CAPS,        MCR1, _______, KC_PGDN, _______, _______,   KC_LEFT,KC_DOWN,    KC_UP,  KC_RGHT,  KC_INS,   KC_DEL,      _______,     \
    _______,       _______, _______, _______, _______, _______,    KC_END, _______, _______, _______, _______, _______, _______, _______, \
    _______,   _______,   _______,   _______,     _______,        JP_ZHTG, _______, _______, _______, _______, _______ \
  ),

  // USキーボード MAC OS を利用する
  [_MACUS] = LAYOUT( \
    KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,      KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,    KC_BSPC,  \
    KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,      KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,  \
    KC_LCTL,     KC_A,    KC_S,    KC_D,    KC_F,    KC_G,      KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,        KC_ENT,   \
 LM(_MACSHIFT,MOD_LSFT),       KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,      KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, LM(_MACSHIFT,MOD_RSFT),   KC_UP,  TO(_WINUS),   \
 MO(_RGBFNC),    KC_LCTL,    KC_LALT,  KC_LGUI,  LT(_MACMOVE,KC_SPC),        KC_SPC, LT(_MACMOVE,KC_SPC), LALT(KC_GRV),  KC_LEFT,KC_DOWN,KC_RGHT \
  ),

  [_MACSHIFT] = LAYOUT( \
     KC_GRV, _______, _______, _______, _______, _______,   _______, _______, _______, _______, _______, _______, _______, _______,       \
    _______,   _______, _______, _______, _______, _______,   _______, _______, _______, _______, _______, _______, _______, _______,     \
    _______,     _______, _______, _______, _______, _______,   _______, _______, _______, _______, _______, _______, _______,   \
    _______,       _______, _______, _______, _______, _______,   _______, _______, _______, _______, _______, _______, _______, _______, \
    _______,   _______,   _______,   _______,     _______,        _______, _______, _______, _______, _______, _______ \
  ),
  // MAC OS 移動用レイヤ _MOVEレイヤを活用するため _MOVE レイヤの直上に配置
  [_MACMOVE] = LAYOUT( \
     KC_GRV,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,     KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_PSCR,     \
    _______,   _______, _______, _______, _______, _______,   _______, KC_PGUP, KC_HOME, _______, _______, _______, _______, _______,     \
    KC_CAPS,    _______, _______, KC_PGDN, _______, _______,   KC_LEFT,KC_DOWN,    KC_UP,  KC_RGHT,  KC_INS,   KC_DEL,      _______,     \
    _______,       _______, _______, _______, _______, _______,    KC_END, _______, _______, _______, _______, _______, _______, _______, \
    _______,   _______,   _______,   _______,     _______,       _______, _______, _______, _______, _______, _______ \
  ),

  // MINT60 機能レイヤ
  [_RGBFNC] = LAYOUT( \
    KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,     KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,    KC_DEL, \
    RGB_TOG,   RGBRST,  RGB_HUI, RGB_SAI, RGB_VAI, XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    XXXXXXX,     RGB_MOD, RGB_HUD, RGB_SAD, RGB_VAD, XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,        XXXXXXX, \
    _______,       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   XXXXXXX, TO(_MACUS), XXXXXXX, XXXXXXX, XXXXXXX, _______, KC_PGUP, _______, \
    XXXXXXX,   _______,    _______,    _______,    XXXXXXX,       XXXXXXX, XXXXXXX, XXXXXXX,                   KC_HOME, KC_PGDN, KC_END \
  )

  /* レイヤ作成用テンプレ
   * [_ALLTRNS] = LAYOUT( \
    _______, _______, _______, _______, _______, _______,   _______, _______, _______, _______, _______, _______, _______, _______,       \
    _______,   _______, _______, _______, _______, _______,   _______, _______, _______, _______, _______, _______, _______, _______,     \
    _______,     _______, _______, _______, _______, _______,   _______, _______, _______, _______, _______, _______, _______,   \
    _______,       _______, _______, _______, _______, _______,   _______, _______, _______, _______, _______, _______, _______, _______, \
    _______,   _______,   _______,   _______,     _______,        _______, _______, _______, _______, _______, _______ \
  )
  */
};

const uint16_t PROGMEM fn_actions[] = {

};

// define variables for reactive RGB
bool TOG_STATUS = false;
int RGB_current_mode;


const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
  // MACRODOWN only works in this function
      switch(id) {
        case 0:
          if (record->event.pressed) {
            register_code(KC_RSFT);
          } else {
            unregister_code(KC_RSFT);
          }
          break;
      }
    return MACRO_NONE;
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

  // WN_SCLN 用変数定義
  static bool lshift = false;
  static bool rshift = false;

  switch (keycode) {
    case RGBRST:
      #ifdef RGBLIGHT_ENABLE
        if (record->event.pressed) {
          eeconfig_update_rgblight_default();
          rgblight_enable();
          RGB_current_mode = rgblight_config.mode;
        }
      #endif
      break;
    case WN_SCLN:                // JIS の「;」と「:」を模擬するための実装
        if (record->event.pressed) {
          lshift = keyboard_report->mods & MOD_BIT(KC_LSFT);
          rshift = keyboard_report->mods & MOD_BIT(KC_RSFT);
          if (lshift || rshift) {
            if (lshift) unregister_code(KC_LSFT);
            if (rshift) unregister_code(KC_RSFT);
            register_code(JP_COLN);
            unregister_code(JP_COLN);
            if (lshift) register_code(KC_LSFT);
            if (rshift) register_code(KC_RSFT);
          } else {
            register_code(JP_SCLN);
            unregister_code(JP_SCLN);
          }
        }
        return false;
        break;
    case MCR1:                   // 文字列入力マクロ
        if (record->event.pressed) {
          SEND_STRING("hogehogefugafuga"); // keydown時に自動入力
        }
        break;
  }
 return true;
}

void matrix_init_user(void) {

}

void matrix_scan_user(void) {

}

void led_set_user(uint8_t usb_led) {

}

uint32_t layer_state_set_user(uint32_t state) {
  switch (biton32(state)) {

    case _SHIFT: // _SHIFTレイヤ時
      rgblight_setrgb(RGB_COLOR_GREEN); // RGB LIGHT を光らせる
      break;

    case _MOVE: // _MOVEレイヤ時
      rgblight_setrgb(RGB_COLOR_BLUE);
      break;

    case _MACUS:
      rgblight_setrgb(RGB_COLOR_WHITE);
      break;

    case _MACSHIFT: // _SHIFTレイヤ時
      rgblight_setrgb(RGB_COLOR_GREEN); // RGB LIGHT を光らせる
      break;

    case _MACMOVE:
      rgblight_setrgb(RGB_COLOR_BLUE);
      break;

    case _RGBFNC:
      rgblight_setrgb(RGB_COLOR_RED);
      break;

    default:  // デフォルトレイヤ (_WINUS) の時は光らない
      //rgblight_setrgb(0x00, 0x00, 0x00);
      rgblight_setrgb(RGB_COLOR_NONE);
  }
  return state;
}
