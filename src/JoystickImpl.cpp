#include "JoystickImpl.h"

JoystickImpl::JoystickImpl() {
  joySt.xAxis = 0;
  joySt.xAxis = 0;
  joySt.yAxis = 0;
  joySt.zAxis = 0;
  joySt.xRotAxis = 0;
  joySt.yRotAxis = 0;
  joySt.zRotAxis = 0;
  joySt.throttle = 0;
  joySt.rudder = 0;
  joySt.hatSw1 = 0;
  joySt.hatSw2 = 0;
  joySt.buttons = 0;
}

void JoystickImpl::pressButton(int num) {
  if(! bState[num]) {
    Serial.print(num);
    Serial.println(" pressed");
    joySt.buttons |= 1 << num;
    bState[num] = 1;
  }
}

void JoystickImpl::releaseButton(int num) {
  if(bState[num]) {
    Serial.print(num);
    Serial.println(" released");
    joySt.buttons ^= 1 << num;
    bState[num] = 0;
  }
}

void JoystickImpl::pressEitherButton(int num, int *either, int size) {
  Serial.println("--- either");
  for(int i = 0; i < size; i++) {
    releaseButton(either[i]);
  }
  pressButton(num);
  Serial.println("----------");
}

void JoystickImpl::hat1(int i) {
  if(hState1 != i) {
    Serial.print("HAT1: ");
    Serial.println(i);
    hState1 = i;
    joySt.hatSw1 = i;
  }
}

void JoystickImpl::hat2(int i) {
  if(hState2 != i) {
    Serial.print("HAT2: ");
    Serial.println(i);
    hState2 = i;
    joySt.hatSw2 = i;
    joySt.hatSw2 = i;
  }
}

void JoystickImpl::update() {
  Joystick.setState(&joySt);
}
