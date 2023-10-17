//#define DEBUG_ENABLE

#ifdef DEBUG_ENABLE
#define DEBUG(x) Serial.println(x)
#else
#define DEBUG(x)
#endif

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
float tt1 = 0.5;
int tt11 = 1000;
boolean lgh = false;
int tt3 = 1000;
int tt4 = 1000;

int inc1 = 1;
float inc_f = 0.01;


void setup() {
#ifdef DEBUG_ENABLE
  Serial.begin(9600);
#endif

  oled.init();
  Wire.setClock(400000L);
  oled.clear();
  oled.update();

  menu.onChange(onItemChange, true);

  menu.addItem(PSTR("<- ВЫХОД")); // 0
  menu.addItem(PSTR("КОЭФ. P"), &inc1, &d_p);
  menu.addItem(PSTR("КОЭФ. I"), &inc1, &d_i);
  menu.addItem(PSTR("КОЭФ. D"), &inc1, &d_d); // 3
  menu.addItem(PSTR("ВРЕМЯ ОПР."), &inc_f, &tt1);
  menu.addItem(PSTR("TIMER 1"), &inc1, &tt11); // 5

  menu.addItem(PSTR("ПОДСВЕТКА"), &lgh); // page 2
  menu.addItem(PSTR("TIMER 3"), &inc1, &tt3);
  menu.addItem(PSTR("TIMER 4"), &inc1, &tt4);

  menu.showMenu(true);


  eb.attach(cb);
}

void onItemChange(int index, void* val, int valType) {
  if (index == 3) {
    DEBUG(d_k);
  }

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
