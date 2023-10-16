#ifndef GyverOLEDMenu_h
#define GyverOLEDMenu_h

#include <GyverOLED.h>

#define VAL_ACTION 0
#define VAL_INTEGER 1
#define VAL_FLOAT 2
#define VAL_DOUBLE 3
#define VAL_BYTE 4
#define VAL_BOOLEAN 5

#define MENU_SELECTED_H 10
#define MENU_ITEM_SELECT_W 127

#define MENU_PARAMS_LEFT_OFFSET 100
#define MENU_ITEM_PADDING_LEFT 2
#define MENU_ITEM_PADDING_TOP 2
#define MENU_PAGE_ITEMS_COUNT 6
#define MENU_FAST_K 4
#define MENU_BOOLEAN_TRUE "On"
#define MENU_BOOLEAN_FALSE "Off"


typedef void (*cbOnChange)(int index, void* val, int valType);

template< typename TGyverOLED >
class OledMenuItem {
public:
  boolean isInit = false;
  boolean isSelect = false;
  boolean isChange = false;

  OledMenuItem() {}

  void setParams(int index, const char* str, byte valType, void* inc, void* val, boolean _isSelect = false) {
    isInit = true;

    _index = index;
    _str = str;
    _valType = valType;
    _inc = inc;
    _val = val;
    isSelect = _isSelect;
  }

  void setPosition(int x, int y) {
    _x = x;
    _y = y; 
  }

  void setOledInstance(TGyverOLED* oled) {
    _oled = oled;
  }

  void drawItem(boolean update = false) {
    _oled->rect(_x, _y, MENU_ITEM_SELECT_W, _y + MENU_SELECTED_H, (isSelect && !isChange) ? OLED_FILL : OLED_CLEAR);

    if (isChange) {
      _oled->roundRect(MENU_PARAMS_LEFT_OFFSET - 4, _y, MENU_ITEM_SELECT_W, _y + MENU_SELECTED_H, OLED_STROKE);
    }

    _oled->textMode((isSelect && !isChange) ? BUF_SUBTRACT : BUF_ADD);

    _oled->setCursorXY(_x + MENU_ITEM_PADDING_LEFT, _y + MENU_ITEM_PADDING_TOP);
    _oled->print(_str);
    
    if (_valType != VAL_ACTION) {
      _oled->setCursorXY(MENU_PARAMS_LEFT_OFFSET, _y + MENU_ITEM_PADDING_TOP);

      switch (_valType) {
        case VAL_INTEGER:
          _oled->print(*(int*)_val);
          break;
        case VAL_BYTE:
          _oled->print(*(int*)_val);
          break;
        case VAL_FLOAT:
          _oled->print(*(float*)_val);
          break;         
        case VAL_DOUBLE:
          _oled->print(*(double*)_val);
          break;               
        case VAL_BOOLEAN:
          _oled->print(*(boolean*)_val ? MENU_BOOLEAN_TRUE: MENU_BOOLEAN_FALSE);
          break;
      }
    }
    if (update) {
      _oled->update();
    }
  }

  void unselect(boolean update = false) {
    isSelect = false;

    drawItem(update);
  }

  void select(boolean update = false) {
    isSelect = true;

    drawItem(update);
  }

  void toggleChange() {
    if (_valType == VAL_ACTION) {
      callCb();

      return;
    }
    
    isChange = !isChange;

    drawItem(true);

    if (!cbImmediate && !isChange) {
      callCb();
    }    
  }
 
  void increment(boolean isFast = false) {
    if (_valType == VAL_ACTION) {
      return;
    }

    prepareValUpdate();

    switch (_valType) {
      case VAL_INTEGER:
      case VAL_BYTE:
        *(int*)_val = *(int*)_val + (isFast ? ((*(int*)_inc) * MENU_FAST_K) : *(int*)_inc);
        _oled->print(*(int*)_val);
        break;
      case VAL_FLOAT:
        *(float*)_val = *(float*)_val + (isFast ? ((*(float*)_inc) * MENU_FAST_K) : *(float*)_inc);
        _oled->print(*(float*)_val);
        break;  
      case VAL_DOUBLE:
        *(double*)_val = *(double*)_val + (isFast ? ((*(double*)_inc) * MENU_FAST_K) : *(double*)_inc);
        _oled->print(*(double*)_val);
        break;               

      case VAL_BOOLEAN:
        *(boolean*)_val = !*(boolean*)_val;
        _oled->print(*(boolean*)_val ? MENU_BOOLEAN_TRUE: MENU_BOOLEAN_FALSE);
        break;
    }

    _oled->update();

    if (cbImmediate) {
      callCb();
    }
  }

  void decrement(boolean isFast = false) {
    if (_valType == VAL_ACTION) {
      return;
    }

    prepareValUpdate();

    switch (_valType) {
      case VAL_INTEGER:
      case VAL_BYTE:
        *(int*)_val = *(int*)_val - (isFast ? ((*(int*)_inc) * MENU_FAST_K) : *(int*)_inc);
        _oled->print(*(int*)_val);
        break;
      case VAL_DOUBLE:
        *(double*)_val = *(double*)_val - (isFast ? ((*(double*)_inc) * MENU_FAST_K) : *(double*)_inc);
        _oled->print(*(double*)_val);
        break;           
      case VAL_FLOAT:
        *(float*)_val = *(float*)_val - (isFast ? ((*(float*)_inc) * MENU_FAST_K) : *(float*)_inc);
        _oled->print(*(float*)_val);
        break;     

      case VAL_BOOLEAN:
        *(boolean*)_val = !*(boolean*)_val;
        _oled->print(*(boolean*)_val ? "On": "Off");
        break;
    }

    _oled->update();
    
    if (cbImmediate) {
      callCb();
    }
  }

  void onChange(cbOnChange cb, boolean immediate = false) {
    _onChange = cb;
    cbImmediate = immediate;
  }

  void callCb() {
    if (_onChange == nullptr) {
      return;
    }

    _onChange(_index, _val, _valType);
  }

private:
  TGyverOLED* _oled = nullptr;
  int _index;
  const char* _str;
  int _x;
  int _y;
  byte _valType;
  void* _inc = nullptr;
  void* _val = nullptr;
  cbOnChange _onChange = nullptr;
  boolean cbImmediate = false;

  void prepareValUpdate() {
    _oled->rect(MENU_PARAMS_LEFT_OFFSET - 4, _y, MENU_ITEM_SELECT_W, _y + MENU_SELECTED_H, OLED_CLEAR);
    _oled->roundRect(MENU_PARAMS_LEFT_OFFSET - 4, _y, MENU_ITEM_SELECT_W, _y + MENU_SELECTED_H, OLED_STROKE);
    
    _oled->textMode(BUF_ADD);
    _oled->setCursorXY(MENU_PARAMS_LEFT_OFFSET, _y + MENU_ITEM_PADDING_TOP);    
  }  
};

// =================================================================================================================================


template< uint16_t _MS_SIZE, typename TGyverOLED >
class OledMenu {
public:
  byte currentPage = 1;
  boolean isMenuShowing = false;
  boolean cbImmediate = false;

  OledMenu(TGyverOLED* oled): _oled(oled)  {}

  // val
  
  void addItem(const char* str) {
    doAddItem(str, VAL_ACTION, nullptr, nullptr);
  }

  void addItem(const char* str, int inc, int val) {
    doAddItem(str, VAL_INTEGER, &inc, &val);
  }

  void addItem(const char* str, double inc, double val) {
    doAddItem(str, VAL_DOUBLE, &inc, &val);
  }  

  void addItem(const char* str, float inc, float val) {
    doAddItem(str, VAL_FLOAT, &inc, &val);
  }    

  void addItem(const char* str, byte inc, byte val) {
    doAddItem(str, VAL_BYTE, &inc, &val);
  }

  void addItem(const char* str, boolean val) {
    doAddItem(str, VAL_BOOLEAN, nullptr, &val);
  }

  // pt

  void addItem(const char* str, int inc, int* val) {
    doAddItem(str, VAL_INTEGER, &inc, val);
  }

  void addItem(const char* str, double inc, double* val) {
    doAddItem(str, VAL_DOUBLE, &inc, val);
  }  

  void addItem(const char* str, float inc, float* val) {
    doAddItem(str, VAL_FLOAT, &inc, val);
  }    

  void addItem(const char* str, byte inc, byte* val) {
    doAddItem(str, VAL_BYTE, &inc, val);
  }

  void addItem(const char* str, boolean* val) {
    doAddItem(str, VAL_BOOLEAN, nullptr, val);
  }  

  void selectNext(boolean isFast = false) {
    if (!isMenuShowing) {
      return;
    }

    int selectedIdx = getSelectedItemIndex();

    if (selectedIdx == -1 || selectedIdx == (_MS_SIZE - 1)) {
      return;
    }

    if (oledMenuItems[selectedIdx].isChange) {
      oledMenuItems[selectedIdx].increment(isFast);
      return;
    }


    int nextIndex = selectedIdx + 1;

   
    oledMenuItems[selectedIdx].unselect();
    
    if (!indexInCurrentPage(nextIndex)) {
      renderPage(currentPage + 1);
      return;
    }
    
    oledMenuItems[nextIndex].select(true);
  }


  void selectPrev(boolean isFast = false) {
    if (!isMenuShowing) {
      return;
    }

    int selectedIdx = getSelectedItemIndex();

    if (selectedIdx < 1) {
      return;
    }

    if (oledMenuItems[selectedIdx].isChange) {
      oledMenuItems[selectedIdx].decrement(isFast);
      return;
    }

    int nextIndex = selectedIdx - 1;

    oledMenuItems[selectedIdx].unselect();

    if (!indexInCurrentPage(nextIndex)) {
      renderPage(currentPage - 1, false);
      return;
    }

    oledMenuItems[nextIndex].select(true);
  }

  void toggleChangeSelected() {
    if (!isMenuShowing) {
      return;
    }

    int selectedIdx = getSelectedItemIndex();

    if (selectedIdx == -1) {
      return;
    }  

    oledMenuItems[selectedIdx].toggleChange();
  }

  void onChange(cbOnChange cb, boolean immediate = false) {
    _onItemChange = cb;
    cbImmediate = immediate;
  }

  void showMenu(boolean val, boolean update = true) {
    if (val == isMenuShowing) {
      return;
    }
    
    isMenuShowing = val;

    if (isMenuShowing) {
      if (update) {
        _oled->update();
      } 
    } else {
      _oled->clear();
      if (update) {
        _oled->update();
      }       
    }
  }


private:
  TGyverOLED* _oled;
  int initInterator = 0; 
  OledMenuItem<TGyverOLED> oledMenuItems[_MS_SIZE];
  cbOnChange _onItemChange = nullptr;

  int getSelectedItemIndex() {
    for (int i = 0; i < _MS_SIZE; i++) {
      if (oledMenuItems[i].isSelect) {
        return i;
      }
    }

    return -1;
  }

  int getChangeableItemIndex() {
    for (int i = 0; i < _MS_SIZE; i++) {
      if (oledMenuItems[i].isChange) {
        return i;
      }
    }

    return -1;
  } 

  void doAddItem(const char* str, byte valType, void* inc, void* val) {
    if (!(initInterator < _MS_SIZE)) {
      return;
    }

    oledMenuItems[initInterator].setOledInstance(_oled);
    oledMenuItems[initInterator].setParams(initInterator, str, valType, inc, val, initInterator == 0);

    if (initInterator < MENU_PAGE_ITEMS_COUNT) {
      oledMenuItems[initInterator].setPosition(0, initInterator * 10);
      oledMenuItems[initInterator].drawItem();
    }

    oledMenuItems[initInterator].onChange(_onItemChange, cbImmediate);

    initInterator++;
  }

  boolean indexInCurrentPage(int index) {
    int maxIndexInPage = (currentPage * MENU_PAGE_ITEMS_COUNT) - 1;
    int minIndexInPage = maxIndexInPage - (MENU_PAGE_ITEMS_COUNT - 1);

    return index >= minIndexInPage && index <= maxIndexInPage;
  }

  void renderPage(byte page, boolean firstSelect = true) {

    if (page < 1) {
      return;
    }
    
    _oled->clear();

    int maxInPage = page * MENU_PAGE_ITEMS_COUNT;
    int minInPage = maxInPage - MENU_PAGE_ITEMS_COUNT;

    maxInPage = maxInPage > _MS_SIZE ? _MS_SIZE : maxInPage;

    int ordinalInc = 0;

    for (int i = minInPage; i < maxInPage; i++) {
      oledMenuItems[i].setPosition(0, ordinalInc * 10);
      //oledMenuItems[initInterator].onChange(_onItemChange);
      oledMenuItems[i].drawItem();

      if (firstSelect && ordinalInc == 0) {
        oledMenuItems[i].select();
      }

      ordinalInc++;
    }

    if (!firstSelect) {
      oledMenuItems[maxInPage - 1].select();
    }  

    currentPage = page;

    _oled->update();
  }
};

#endif