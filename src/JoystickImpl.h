#include <Arduino.h>

class JoystickImpl {
private:
  JoyState_t joySt;
  int bState[32];
  int hState[2][8];
  int hState1;
  int hState2;
public:
  JoystickImpl();
  void pressButton(int num);
  void releaseButton(int num);
  void pressEitherButton(int num, int *either, int size);
  void update();
  void hat1(int i);
  void hat2(int i);
};
