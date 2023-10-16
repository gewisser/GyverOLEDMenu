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

int d_k = 50;

void setup() {
#ifdef DEBUG_ENABLE
  Serial.begin(9600);
#endif

  oled.init();
  Wire.setClock(400000L);
  oled.clear();
  oled.update();

  menu.onChange(onItemChange, true);

  menu.addItem("<- ВЫХОД"); // 0
  menu.addItem("КОЭФ. P", 1, 10);
  menu.addItem("КОЭФ. I", 1, 1000);
  menu.addItem("КОЭФ. D", 1, &d_k); // 3
  menu.addItem("ВРЕМЯ ОПР.", 0.01, 0.5);
  menu.addItem("TIMER 1", 1, 1000); // 5 

  menu.addItem("ПОДСВЕТКА", false); // page 2
  menu.addItem("TIMER 3", 1, 1000);
  menu.addItem("TIMER 4", 1, 1000);  

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
