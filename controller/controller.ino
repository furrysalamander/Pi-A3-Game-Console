//#include <Wire.h>

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

const char BUTTON_COUNT = 16;
const char AXIS_COUNT = 4;
const char TX_BUF_LEN = BUTTON_COUNT + AXIS_COUNT * sizeof(int);

char INPUTS[BUTTON_COUNT] = {
    BTN_A, BTN_B, BTN_X,    BTN_Y,    DPAD_U,    DPAD_D,     DPAD_L,   DPAD_R,
    BTN_L, BTN_R, LJOY_BTN, RJOY_BTN, BTN_START, BTN_SELECT, BTN_HOME, BTN_MOD,
};

char tx_buffer[BUTTON_COUNT + AXIS_COUNT];
//uint16_t *analog_ptr = reinterpret_cast<uint16_t *>(tx_buffer + BUTTON_COUNT);
int analog_tx_buffer[2];

char AXIS[AXIS_COUNT] = {LJOY_X, LJOY_Y, RJOY_X, RJOY_Y,};

void setup() {
  for (char i = 0; i < BUTTON_COUNT; i++) {
    pinMode(INPUTS[i], INPUT_PULLUP);
  }
  //Wire.begin();
  Serial.begin(1000000);
}

void loop() {
  for (char i = 0; i < BUTTON_COUNT; i++) {
    tx_buffer[i] = digitalRead(INPUTS[i]);
  }

  for (char i = 0; i < 2; i++) {
    analog_tx_buffer[i] = analogRead(AXIS[i]);
  }

  Serial.write(27);       // ESC command
  Serial.print("[2J");    // clear screen command
  Serial.write(27);
  Serial.print("[H");     // cursor to home command
  Serial.println("Buttons:");
  for (char i = 0; i < BUTTON_COUNT; i++) {
    Serial.print(int(tx_buffer[i]));
  }
  Serial.println('\n');
  Serial.println("Joystick:");
  for (char i = 0; i < 2; i++) {
    Serial.print(analog_tx_buffer[i]);
    Serial.write(' ');
  }
  delay(40);
}