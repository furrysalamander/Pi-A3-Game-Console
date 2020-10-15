#include <Wire.h>

enum pins {
  BTN_X = 0,       // X
  BTN_Y = 1,       // Y
  Z_SDA = 2,       //
  Z_SCL = 3,       //
  DPAD_U = 4,      // UP
  BTN_L = 5,       // L
  DPAD_R = 6,      // RIGHT
  BTN_B = 7,       // B
  BTN_START = 8,   // START
  BTN_HOME = 9,    // HOME
  BTN_SELECT = 10, // SELECT
  BTN_A = 11,      // A
  DPAD_L = 12,     // LEFT
  BTN_R = 13,      // R
  Z_MISO = 14,     //
  LCD_PWM = 15,    //
  Z_MOSI = 16,     //
  LJOY_BTN = 17,   //
  LJOY_Y = A0,     //
  LJOY_X = A1,     //
  RJOY_Y = A2,     //
  RJOY_X = A3,     //
  BTN_MOD = A4,    // MOD
  RJOY_BTN = A5,   //
  DPAD_D = 30,     // DOWN
};

const uint8_t BUTTON_COUNT = 16;
const uint8_t AXIS_COUNT = 4;

uint8_t BUTTONS[BUTTON_COUNT] = {
    BTN_A, BTN_B, BTN_X,    BTN_Y,    DPAD_U,    DPAD_D,     DPAD_L,   DPAD_R,
    BTN_L, BTN_R, LJOY_BTN, RJOY_BTN, BTN_START, BTN_SELECT, BTN_HOME, BTN_MOD,
};

uint8_t AXIS[AXIS_COUNT] = {
    LJOY_X,
    LJOY_Y,
    RJOY_X,
    RJOY_Y,
};

struct tx_buffer_t {
  uint8_t buttons[BUTTON_COUNT];
  int16_t axis[AXIS_COUNT];
};

tx_buffer_t tx_buffer;

void setup() {
  for (uint8_t i = 0; i < BUTTON_COUNT; i++) {
    pinMode(BUTTONS[i], INPUT_PULLUP);
  }
  Wire.begin(8);
  Wire.onRequest(RequestEvent);
  tx_buffer.axis[2] = 0;
  tx_buffer.axis[3] = 0;
  //Serial.begin(1000000);
  //Serial1.begin(1000000);
}

void loop() {


  // Serial.write(27);    // ESC command
  // Serial.print("[2J"); // clear screen command
  // Serial.write(27);
  // Serial.print("[H"); // cursor to home command
  // Serial.println("Buttons:");
  // for (uint8_t i = 0; i < BUTTON_COUNT; i++) {
  //   Serial.print(int(tx_buffer.buttons[i]));
  // }
  // Serial.println('\n');
  // Serial.println("Joystick:");
  // for (uint8_t i = 0; i < AXIS_COUNT; i++) {
  //   Serial.print(tx_buffer.axis[i]);
  //   Serial.write(' ');
  // }
  // Serial.println();
  // Serial1.write(reinterpret_cast<uint8_t *>(&tx_buffer), sizeof(tx_buffer_t));
  delay(5);
}

void RequestEvent() {
  
  for (uint8_t i = 0; i < BUTTON_COUNT; i++) {
    tx_buffer.buttons[i] = !digitalRead(BUTTONS[i]);
  }

  for (uint8_t i = 0; i < AXIS_COUNT; i++) {
    int tmp_val = analogRead(AXIS[i]);
    tmp_val = map(tmp_val, 0, 1023, -512, 512);
    tx_buffer.axis[i] = tmp_val;
  }
  Wire.write(reinterpret_cast<uint8_t *>(&tx_buffer), sizeof(tx_buffer_t));
}


