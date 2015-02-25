#include "Button.h"
#include "Encoder.h"
#include "AnalogButton.h"
#include "Multiplex.h"
#include "JoystickImpl.h"

JoystickImpl joyStick;

byte muxChannels[] = {
  0b0000, // ch 0
  0b1000, // ch 1
  0b0100, // ch 2
  0b1100, // ch 3
  0b0010, // ch 4
  0b1010, // ch 5
  0b0110, // ch 6
  0b1110, // ch 7
  0b0001, // ch 8
  0b1001, // ch 9
  0b0101, // ch 10
  0b1101, // ch 11
  0b0011, // ch 12
  0b1011, // ch 13
  0b0111, // ch 14
  0b1111  // ch 15
};
int mplex0_ctrl[] = {2, 3, 4, 5};
int mplex1_ctrl[] = {6, 7, 8, 9};
int mplex2_ctrl[] = {10, 11, 12, 13};

Multiplex *mplex0; // buttons 1 - 16
Multiplex *mplex1; // buttons 17 - 32
Multiplex *mplex2; // unused

#define MAX_BUTTONS 32
Button buttons[MAX_BUTTONS];

#define MAX_ENCODERS 5
Encoder encoders[MAX_ENCODERS];

#define MAX_ANALOGBUTTONS 4
AnalogButton analogButtons[MAX_ANALOGBUTTONS];

int analogButtonResistors[2] = {
        123, // up
        456  // down
};

#define TWAY_CHANNEL 4
#define TWAY_PIN A3
AnalogButton twayAnalogButton;
int twayResistorValues[12] = {
        576,
        968,
        578,
        1008,
        1016,
        975,
        1020,
        671,
        737,
        806,
        668,
        732
};


void setup()
{
  Serial.begin(9600);
 // pins left A4-A5

  // initialize multiplexer
  mplex0 = new Multiplex(A0, mplex0_ctrl, 4, muxChannels);
  mplex1 = new Multiplex(A1, mplex1_ctrl, 4, muxChannels);
  mplex2 = new Multiplex(A2, mplex2_ctrl, 4, muxChannels);

  // initialize first 11 buttons, all on mplex0
  for(int i = 0; i < 11; i++) {
    buttons[i].setPin(mplex0->getCommon());
  }
  // initialize encoders
  for(int i = 0; i < MAX_ENCODERS; i++) {
    encoders[i].setPins(mplex0->getCommon(), mplex1->getCommon());
  }
  // initialize analog buttons on mplex1
  for(int i = 0; i < MAX_ANALOGBUTTONS; i++) {
    analogButtons[i].setPin(mplex1->getCommon());
    analogButtons[i].setResistorValues(analogButtonResistors, 2);
  }
  // initialize 12way analog button, mplex1 channel 4
  //twayAnalogButton.setPin(mplex1->getCommon());
  twayAnalogButton.setPin(TWAY_PIN);
  twayAnalogButton.setResistorValues(twayResistorValues, 12);





  delay(3000);
}

uint32_t dtimer = 0;
void loop() {
  /*Serial.print(analogRead(A0));
  Serial.print(" ");
  Serial.print(analogRead(A1));
  Serial.print(" ");
  Serial.print(analogRead(A2));
  Serial.print(" ");
  Serial.print(analogRead(A3));
  Serial.print(" ");
  Serial.print(analogRead(A4));
  Serial.print(" ");
  Serial.println(analogRead(A5));*/
  // read first 11 buttons from multiplexer 0
  if(dtimer == 0) dtimer = millis();
  if((millis() - dtimer) >= 2000) {
  Serial.println("mplex0");
  debug_mplex(mplex0);
  Serial.println("mplex1");
  debug_mplex(mplex1);
  Serial.println("mplex2");
  debug_mplex(mplex2);
  dtimer = 0;
  }
  for(int i = 0; i <= 10; i++) {
    mplex0->selectChannel(i);
    //if(buttons[i].isPressed()) {
    if(digitalRead(mplex0->getCommon()) == HIGH) {
        joyStick.pressButton(i);
    } else {
        joyStick.releaseButton(i);

    }
  }
  // mplex0 pins 0 - 10 taken

  // next read the encoders
  for(int i = 11; i < (11+MAX_ENCODERS); i++) {
    mplex0->selectChannel(i);
    mplex1->selectChannel(i);
    int e = i - 11;
    if(encoders[e].isClickedLeft()) {
        joyStick.releaseButton(i+MAX_ENCODERS);
        joyStick.pressButton(i);
    } else if(encoders[e].isClickedRight()) {
        joyStick.releaseButton(i);
        joyStick.pressButton(i+MAX_ENCODERS);
    } else {
        joyStick.releaseButton(i);
        joyStick.releaseButton(i+MAX_ENCODERS);
    }
  }
  // mplex0 and mplex1 pins 11 - 16 taken
  // joystick 0 - 20 taken
  // mplex1 left pin 0 - 10


  // read analogbuttons (2 positions each)
  for(int i = 0; i < MAX_ANALOGBUTTONS; i++) {
    if(analogButtons[i].read() != -1) {
      mplex1->selectChannel(i);
      int base = 20 + (i*2);
      int btn = base + analogButtons[i].read();
      int either[2] = { base + 1, base + 2 };
      joyStick.pressEitherButton(btn, either, 2);
    }
  }
  // mplex1 pins 0-3 taken
  // joystick 0 - 28 taken

  // read 12way switch, hat1 0-7, hat2 0-3
  // mplex1->selectChannel(TWAY_CHANNEL);
  int val = twayAnalogButton.read();
  if(val != -1) {
    if(val < 8) {
      joyStick.hat2(8);
      joyStick.hat1(val-1);
    } else {
      joyStick.hat1(8);
      joyStick.hat2(val-8);
    }
  }
  // mplex1 left pins 5-10
  // 4 joystick buttons left
  // 4 (4-7) hat2 positions left
  // mplex2 16 pins left
  delay(2000);
  //joyStick.update();
}

void debug_mplex(Multiplex *mplex) {
        int dmuxValue[16];
        int amuxValue[16];
        for(int i = 0; i < 16; i++) {
                mplex->selectChannel(i);
                amuxValue[i] = mplex->analogRead();
        }
        for(int i = 0; i < 16; i++) {
                mplex->selectChannel(i);
                dmuxValue[i] = mplex->digitalRead();
        }
        for(int i = 0; i < 16; i++) {
                Serial.print("Channel ");
                Serial.print(i);
                Serial.print("\t");
                Serial.print("D: ");
                Serial.print(dmuxValue[i]);
                Serial.print(" A: ");
                Serial.println(amuxValue[i]);
        }
}
