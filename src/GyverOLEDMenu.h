#ifndef GyverOLEDMenu_h
#define GyverOLEDMenu_h

#define VAL_ACTION 0
#define VAL_INTEGER 1
#define VAL_FLOAT 2
#define VAL_DOUBLE 3
#define VAL_BYTE 4
#define VAL_BOOLEAN 5
#define VAL_U_INTEGER 6


#ifndef MENU_SELECTED_H
#define MENU_SELECTED_H 10
#endif

#define MENU_ITEM_SELECT_W 127

#ifndef MENU_PARAMS_LEFT_OFFSET
#define MENU_PARAMS_LEFT_OFFSET 92
#endif

#define MENU_ITEM_PADDING_LEFT 2
#define MENU_ITEM_PADDING_TOP 2

#ifndef MENU_PAGE_ITEMS_COUNT
#define MENU_PAGE_ITEMS_COUNT 6
#endif

#ifndef MENU_FAST_K
#define MENU_FAST_K 4
#endif


typedef void (*cbOnChange)(int index, void* val, int valType);

const char* MENU_BOOLEAN_TEXT[]  = { "Off", "On" };

template< typename TGyverOLED >
class OledMenuItem {
public:
  boolean isSelect = false;
  boolean isChange = false;

  OledMenuItem() {}

  void setParams(const void* str, const void* inc, void* val, byte valType) {
    _str = str;
    _valType = valType;
    _inc = inc;
    _val = val;
  }

  void setPosition(int x, int y) {
    _x = x;
    _y = y;
    _y1 = _y + MENU_SELECTED_H;
    _text_y = _y + MENU_ITEM_PADDING_TOP;
  }

  void setOledInstance(TGyverOLED* oled, int index) {
    _index = index;
    _oled = oled;
  }

  void drawItem(boolean update = false) {
    _oled->rect(_x, _y, MENU_ITEM_SELECT_W, _y1, (isSelect && !isChange) ? OLED_FILL : OLED_CLEAR);

    if (isChange) {
      _oled->roundRect(MENU_PARAMS_LEFT_OFFSET - 4, _y, MENU_ITEM_SELECT_W, _y1, OLED_STROKE);
    }

    _oled->textMode((isSelect && !isChange) ? BUF_SUBTRACT : BUF_ADD);

    _oled->setCursorXY(_x + MENU_ITEM_PADDING_LEFT, _text_y);

    _oled->print((const __FlashStringHelper*)_str);


    if (_valType != VAL_ACTION) {
      _oled->setCursorXY(MENU_PARAMS_LEFT_OFFSET, _text_y);

      switch (_valType) {
        case VAL_INTEGER:
          _oled->print(*(int*)_val);
          break;
        case VAL_U_INTEGER:
          _oled->print(*(unsigned int*)_val);
          break;
        case VAL_BYTE:
          _oled->print(*(byte*)_val);
          break;
        case VAL_FLOAT:
          _oled->print(*(float*)_val);
          break;
        case VAL_DOUBLE:
          _oled->print(*(double*)_val);
          break;
        case VAL_BOOLEAN:
          _oled->print(MENU_BOOLEAN_TEXT[*(boolean*)_val]);
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

  void setMinMax(const void* min, const void* max) {
    _min = min;
    _max = max;
  }

  void increment(boolean isFast = false) {
    if (_valType == VAL_ACTION) {
      return;
    }

    prepareValUpdate();

    switch (_valType) {
      case VAL_INTEGER:
        *(int*)_val = constrain(*(int*)_val + (isFast ? ((*(int*)_inc) * MENU_FAST_K) : *(int*)_inc), *(int*)_min, *(int*)_max);
        _oled->print(*(int*)_val);
        break;

      case VAL_U_INTEGER:
        *(unsigned int*)_val = constrain(*(unsigned int*)_val + (isFast ? ((*(unsigned int*)_inc) * MENU_FAST_K) : *(unsigned int*)_inc), *(unsigned int*)_min, *(unsigned int*)_max);
        _oled->print(*(unsigned int*)_val);
        break;

      case VAL_BYTE:
        *(byte*)_val = constrain(*(byte*)_val + (isFast ? ((*(byte*)_inc) * MENU_FAST_K) : *(byte*)_inc), *(byte*)_min, *(byte*)_max);
        _oled->print(*(byte*)_val);
        break;

      case VAL_DOUBLE:
        *(double*)_val = constrain(*(double*)_val + (isFast ? ((*(double*)_inc) * MENU_FAST_K) : *(double*)_inc), *(double*)_min, *(double*)_max);
        _oled->print(*(double*)_val);
        break;

      case VAL_FLOAT:
        *(float*)_val = constrain(*(float*)_val + (isFast ? ((*(float*)_inc) * MENU_FAST_K) : *(float*)_inc), *(float*)_min, *(float*)_max);
        _oled->print(*(float*)_val);
        break;
      case VAL_BOOLEAN:
        *(boolean*)_val = !*(boolean*)_val;
        _oled->print(MENU_BOOLEAN_TEXT[*(boolean*)_val]);
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

        *(int*)_val = constrain(*(int*)_val - (isFast ? ((*(int*)_inc) * MENU_FAST_K) : *(int*)_inc), *(int*)_min, *(int*)_max);
        _oled->print(*(int*)_val);
        break;

      case VAL_U_INTEGER:
        *(unsigned int*)_val = constrain(*(unsigned int*)_val - (isFast ? ((*(unsigned int*)_inc) * MENU_FAST_K) : *(unsigned int*)_inc), *(unsigned int*)_min, *(unsigned int*)_max);
        _oled->print(*(unsigned int*)_val);
        break;

      case VAL_BYTE:
        *(byte*)_val = constrain(*(byte*)_val - (isFast ? ((*(byte*)_inc) * MENU_FAST_K) : *(byte*)_inc), *(byte*)_min, *(byte*)_max);
        _oled->print(*(byte*)_val);
        break;

      case VAL_DOUBLE:
        *(double*)_val = constrain(*(double*)_val - (isFast ? ((*(double*)_inc) * MENU_FAST_K) : *(double*)_inc), *(double*)_min, *(double*)_max);
        _oled->print(*(double*)_val);
        break;

      case VAL_FLOAT:
        *(float*)_val = constrain(*(float*)_val - (isFast ? ((*(float*)_inc) * MENU_FAST_K) : *(float*)_inc), *(float*)_min, *(float*)_max);
        _oled->print(*(float*)_val);
        break;

      case VAL_BOOLEAN:
        *(boolean*)_val = !*(boolean*)_val;
        _oled->print(MENU_BOOLEAN_TEXT[*(boolean*)_val]);
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
  int _index = 0;
  const void* _str = nullptr;
  int _x;
  int _y;
  int _y1;
  int _text_y;
  byte _valType;
  const void* _inc = nullptr;
  const void* _min = nullptr;
  const void* _max = nullptr;
  void* _val = nullptr;
  cbOnChange _onChange = nullptr;
  boolean cbImmediate = false;

  void prepareValUpdate() {
    _oled->rect(MENU_PARAMS_LEFT_OFFSET - 4, _y, MENU_ITEM_SELECT_W, _y1, OLED_CLEAR);
    _oled->roundRect(MENU_PARAMS_LEFT_OFFSET - 4, _y, MENU_ITEM_SELECT_W, _y1, OLED_STROKE);

    _oled->textMode(BUF_ADD);
    _oled->setCursorXY(MENU_PARAMS_LEFT_OFFSET, _text_y);
  }
};

// =================================================================================================================================


template< uint16_t _MS_SIZE, typename TGyverOLED >
class OledMenu {
public:
  byte currentPage = 1;
  boolean isMenuShowing = false;
  boolean cbImmediate = false;

  OledMenu(const TGyverOLED* oled): _oled((TGyverOLED*)oled)  {}

  // val

  void addItem(PGM_P str) {
    doAddItem(str, VAL_ACTION, nullptr, nullptr, nullptr, nullptr);
  }

  void addItem(PGM_P str, const int* inc, int* val, const int min, const int max) {
    doAddItem(str, VAL_INTEGER, inc, val, &min, &max);
  }

  void addItem(PGM_P str, const unsigned int* inc, unsigned int* val, const unsigned int min, const unsigned int max) {
    doAddItem(str, VAL_U_INTEGER, inc, val, &min, &max);
  }

  void addItem(PGM_P str, const double* inc, double* val, const double min, const double max) {
    doAddItem(str, VAL_DOUBLE, inc, val, &min, &max);
  }

  void addItem(PGM_P str, const float* inc, float* val, const float min, const float max) {
    doAddItem(str, VAL_FLOAT, inc, val, &min, &max);
  }

  void addItem(PGM_P str, const byte* inc, byte* val, const byte min, const byte max) {
    doAddItem(str, VAL_BYTE, inc, val, &min, &max);
  }

  void addItem(PGM_P str, boolean* val) {
    doAddItem(str, VAL_BOOLEAN, nullptr, val, nullptr, nullptr);
  }


  void selectNext(boolean isFast = false) {
    if (!isMenuShowing) {
      return;
    }

    int selectedIdx = getSelectedItemIndex();

    if (selectedIdx == -1) {
      return;
    }

    if (oledMenuItems[selectedIdx].isChange) {
      oledMenuItems[selectedIdx].increment(isFast);
      return;
    }

    if (selectedIdx == (_MS_SIZE - 1)) {
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

    if (selectedIdx == -1) {
      return;
    }

    if (oledMenuItems[selectedIdx].isChange) {
      oledMenuItems[selectedIdx].decrement(isFast);
      return;
    }

    if (selectedIdx < 1) {
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
      renderPage(currentPage);
    } else {
      _oled->clear();
      if (update) {
        _oled->update();
      }
    }
  }

  void refresh() {
    renderPage(currentPage);
  }


private:
  TGyverOLED* _oled = nullptr;
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

  void doAddItem(const void* str, byte valType, const void* inc, void* val, const void* min, const void* max) {
    if (!(initInterator < _MS_SIZE)) {
      return;
    }

    oledMenuItems[initInterator].setOledInstance(_oled, initInterator);
    oledMenuItems[initInterator].setParams(str, inc, val, valType);
    oledMenuItems[initInterator].onChange(_onItemChange, cbImmediate);
    oledMenuItems[initInterator].setMinMax(min, max);

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

    boolean selectedExist = getSelectedItemIndex() != -1;

    for (int i = minInPage; i < maxInPage; i++) {
      oledMenuItems[i].setPosition(0, ordinalInc * 10);

      if (!selectedExist && (firstSelect && ordinalInc == 0)) {
        oledMenuItems[i].select();
      }

      oledMenuItems[i].drawItem();

      ordinalInc++;
    }

    if (!selectedExist && !firstSelect) {
      oledMenuItems[maxInPage - 1].select();
    }

    currentPage = page;

    _oled->update();
  }
};

#endif
