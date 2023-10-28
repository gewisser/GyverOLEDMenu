#define EB_FAST_TIME 120

#include <GyverOLED.h>
#include <EncButton.h>
#include "GyverOLEDMenu.h"

EncButton eb(3, 4, 2, INPUT_PULLUP);
GyverOLED<SSH1106_128x64> oled;

OledMenu<9, GyverOLED<SSH1106_128x64>> menu(&oled);

int d_p = 10;
int d_i = 1000;
int d_d = 50;
int tt11 = 1000;
float tt1 = 0.5;
boolean lgh = false;
int tt3 = 1000;
int tt4 = 1000;

const int inc1 = 1;
const float inc_f = 0.01;


void setup() {
  oled.init();
  Wire.setClock(400000L);
  oled.clear();
  oled.update();

  menu.onChange(onItemChange, true);

  menu.addItem(PSTR("<- ВЫХОД")); // 0
  menu.addItem(PSTR("КОЭФ. P"), &inc1, &d_p, 0, 100);
  menu.addItem(PSTR("КОЭФ. I"), &inc1, &d_i, -5, 20);
  menu.addItem(PSTR("КОЭФ. D"), &inc1, &d_d, 0, 7000); // 3
  menu.addItem(PSTR("ВРЕМЯ ОПР."), &inc_f, &tt1, 1, 20);
  menu.addItem(PSTR("TIMER 1"), &inc1, &tt11, 1, 255); // 5

  menu.addItem(PSTR("ПОДСВЕТКА"), &lgh); // page 2
  menu.addItem(PSTR("TIMER 3"), &inc1, &tt3, 1, 5);
  menu.addItem(PSTR("TIMER 4"), &inc1, &tt4, 0, 10);

  menu.showMenu(true);


  eb.attach(cb);
}

void onItemChange(int index, void* val, int valType) {
  if (valType == VAL_ACTION) {
    if (index == 0) {
      menu.showMenu(false);
    }
  }
}


void cb() {
  switch (eb.action()) {
    case EB_TURN:
      if (eb.dir() == 1) {
        menu.selectPrev(eb.fast());
      } else {
        menu.selectNext(eb.fast());
      }

      break;

    case EB_CLICK:
      menu.toggleChangeSelected();
      break;
  }
}


void loop() {
  eb.tick();
}
