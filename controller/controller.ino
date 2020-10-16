#include <Wire.h>

// Frequency modes for TIMER1
#define PWM62k 1 // 62500 Hz
#define PWM8k 2  // 7812 Hz
#define PWM1k 3  //  976 Hz
#define PWM244 4 //  244 Hz
#define PWM61 5  //   61 Hz

// Direct PWM change variables
#define PWM10 OCR1B

// Configure the PWM clock
// The argument is one of the 5 previously defined modes
void pwm91011configure(int mode) {
  // TCCR1A configuration
  //  00 : Channel A disabled D9
  //  00 : Channel B disabled D10
  //  00 : Channel C disabled D11
  //  01 : Fast PWM 8 bit
  TCCR1A = 1;

  // TCCR1B configuration
  // Clock mode and Fast PWM 8 bit
  TCCR1B = mode | 0x08;

  // TCCR1C configuration
  TCCR1C = 0;
}

// Set PWM to D10
// Argument is PWM between 0 and 255
void pwmSet10(int value) {
  OCR1B = value;  // Set PWM value
  DDRB |= 1 << 6; // Set Output Mode B6
  TCCR1A |= 0x20; // Set PWM value
}

/*************** ADDITIONAL DEFINITIONS ******************/

// Macro to converts from duty (0..100) to PWM (0..255)
#define DUTY2PWM(x) ((255 * (x)) / 100)

/**********************************************************/

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
  LCD_PWM = 10,    //
  BTN_A = 11,      // A
  DPAD_L = 12,     // LEFT
  BTN_R = 13,      // R
  Z_MISO = 14,     //
  BTN_SELECT = 15, // SELECT
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

const uint8_t BACKLIGHT_OPTIONS_COUNT = 8;
const uint8_t BACKLIGHT_OPTIONS[BACKLIGHT_OPTIONS_COUNT] = {
  1, 5, DUTY2PWM(8), DUTY2PWM(20), DUTY2PWM(40), DUTY2PWM(60), DUTY2PWM(80), DUTY2PWM(100)
};
uint8_t backlight_val = 0;

struct tx_buffer_t {
  uint8_t buttons[BUTTON_COUNT];
  int16_t axis[AXIS_COUNT];
};

tx_buffer_t tx_buffer;

void setup() {
  for (uint8_t i = 0; i < BUTTON_COUNT; i++) {
    pinMode(BUTTONS[i], INPUT_PULLUP);
  }

  pinMode(LCD_PWM, OUTPUT);

  Wire.begin(8);
  Wire.onRequest(RequestEvent); // Setup callback for communication requests
  tx_buffer.axis[2] = 0;
  tx_buffer.axis[3] = 0;

  // Configure Timer 1 (Pins 9, 10 and 11)
  // It will operate at 62kHz
  // Valid options are:
  //      PWM62k, PWM8k, PWM1k, PWM244 and PWM61
  pwm91011configure(PWM62k);

  // Generate PWM at pin 11 with 30% duty
  // We need to call pwm91011configure before
  // We use here the DUTY2PWM macro
  pwmSet10(DUTY2PWM(20));
}

uint8_t last_macro_state = 1;

void loop() {
  if (!last_macro_state && (tx_buffer.buttons[1]))
  {
    PWM10 = BACKLIGHT_OPTIONS[++backlight_val & 0b111];
  }
  last_macro_state = tx_buffer.buttons[1];
  delay(20);
}

void RequestEvent() {
  for (uint8_t i = 0; i < BUTTON_COUNT; i++) {
    tx_buffer.buttons[i] = !digitalRead(BUTTONS[i]);
  }

  for (uint8_t i = 0; i < AXIS_COUNT - 2; i++) {
    tx_buffer.axis[i] = analogRead(AXIS[i]);
  }
  Wire.write(reinterpret_cast<uint8_t *>(&tx_buffer), sizeof(tx_buffer_t));
}
