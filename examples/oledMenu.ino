#define EB_FAST_TIME 120

#include <GyverOLED.h>
#include <EncButton.h>
#include "GyverOLEDMenu.h"

EncButton eb(6, 7, 5, INPUT_PULLUP);
GyverOLED<SSH1106_128x64> oled;

OledMenu<9, GyverOLED<SSH1106_128x64>> menu(&oled);

int d_p = 10;
int d_i = 1000;
int d_d = 50;
byte tt11 = 10;
float tt1 = 0.5;
boolean lgh = false;
int tt3 = 1000;
int tt4 = 1000;

void setup() {
  oled.init();
  Wire.setClock(400000L);
  oled.clear();
  oled.update();

  menu.onChange(onItemChange, true);

  menu.addItem(PSTR("<- ВЫХОД")); // 0
  menu.addItem(PSTR("КОЭФ. P"), GM_N_INT(1), &d_p, GM_N_INT(0), GM_N_INT(100));
  menu.addItem(PSTR("КОЭФ. I"), GM_N_INT(1), &d_i, GM_N_INT(-5), GM_N_INT(20));
  menu.addItem(PSTR("КОЭФ. D"), GM_N_INT(1), &d_d, GM_N_INT(0), GM_N_INT(7000)); // 3
  menu.addItem(PSTR("ВРЕМЯ ОПР."), GM_N_FLOAT(0.01), &tt1, GM_N_FLOAT(1), GM_N_FLOAT(20));
  menu.addItem(PSTR("TIMER 1"), GM_N_BYTE(1), &tt11, GM_N_BYTE(1), GM_N_BYTE(255)); // 5

  menu.addItem(PSTR("ПОДСВЕТКА"), &lgh); // page 2
  menu.addItem(PSTR("TIMER 3"), GM_N_INT(1), &tt3, GM_N_INT(1), GM_N_INT(5));
  menu.addItem(PSTR("TIMER 4"), GM_N_INT(1), &tt4, GM_N_INT(0), GM_N_INT(10));

  menu.showMenu(true);


  eb.attach(cb);
}

void onItemChange(int index, void* val, byte valType) {
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
