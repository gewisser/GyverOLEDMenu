#ifndef GyverOLEDMenu_h
#define GyverOLEDMenu_h

#define GM_N_INT(x) (__extension__({static const int __m_d_i = (x); &__m_d_i;}))
#define GM_N_U_INT(x) (__extension__({static const unsigned int __m_d_u_i = (x); &__m_d_u_i;}))
#define GM_N_FLOAT(x) (__extension__({static const float __m_d_f = (x); &__m_d_f;}))
#define GM_N_DOUBLE(x) (__extension__({static const double __m_d_d = (x); &__m_d_d;}))
#define GM_N_BYTE(x) (__extension__({static const byte __m_d_b = (x); &__m_d_b;}))

#define VAL_ACTION 0
#define VAL_INTEGER 1
#define VAL_FLOAT 2
#define VAL_DOUBLE 3
#define VAL_BYTE 4
#define VAL_BOOLEAN 5
#define VAL_U_INTEGER 6

#define MENU_IP_PRINT 0
#define MENU_IP_INC 1
#define MENU_IP_DEC 2

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


typedef void (*cbOnChange)(const int index, const void* val, const byte valType);
typedef boolean (*cbOnPrintOverride)(const int index, const void* val, const byte valType);

const char* MENU_BOOLEAN_TEXT[]  = { "Off", "On" };

template< typename TGyverOLED >
class OledMenuItem {
public:
  boolean isSelect = false;
  boolean isChange = false;

  OledMenuItem() {}

  void setParams(const void* str, const void* inc, void* val, const byte valType) {
    _str = str;
    _valType = valType;
    _inc = inc;
    _val = val;
  }

  void setPosition(const int x, const int y) {
    _x = x;
    _y = y;
    _y1 = _y + MENU_SELECTED_H;
    _text_y = _y + MENU_ITEM_PADDING_TOP;
  }

  void setOledInstance(TGyverOLED* oled, const int index) {
    _index = index;
    _oled = oled;
  }

  void drawItem(const boolean update = false) {
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
          internalPrint<int>(MENU_IP_PRINT);
          break;
        case VAL_U_INTEGER:
          internalPrint<unsigned int>(MENU_IP_PRINT);
          break;
        case VAL_BYTE:
          internalPrint<byte>(MENU_IP_PRINT);
          break;
        case VAL_FLOAT:
          internalPrint<float>(MENU_IP_PRINT);
          break;
        case VAL_DOUBLE:
          internalPrint<double>(MENU_IP_PRINT);
          break;
        case VAL_BOOLEAN:
          printBoolean();
          break;
      }
    }
    if (update) {
      _oled->update();
    }
  }

  void unselect(const boolean update = false) {
    isSelect = false;

    drawItem(update);
  }

  void select(const boolean update = false) {
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

  void increment(const boolean isFast = false) {
    if (_valType == VAL_ACTION) {
      return;
    }

    prepareValUpdate();

    switch (_valType) {
      case VAL_INTEGER:
        internalPrint<int>(MENU_IP_INC, isFast);
        break;

      case VAL_U_INTEGER:
        internalPrint<unsigned int>(MENU_IP_INC, isFast);
        break;

      case VAL_BYTE:
        internalPrint<byte>(MENU_IP_INC, isFast);
        break;

      case VAL_DOUBLE:
        internalPrint<double>(MENU_IP_INC, isFast);
        break;

      case VAL_FLOAT:
        internalPrint<float>(MENU_IP_INC, isFast);
        break;

      case VAL_BOOLEAN:
        printBoolean(MENU_IP_DEC);
        break;
    }

    _oled->update();

    if (cbImmediate) {
      callCb();
    }
  }

  void decrement(const boolean isFast = false) {
    if (_valType == VAL_ACTION) {
      return;
    }

    prepareValUpdate();

    switch (_valType) {
      case VAL_INTEGER:
        internalPrint<int>(MENU_IP_DEC, isFast);
        break;

      case VAL_U_INTEGER:
        internalPrint<unsigned int>(MENU_IP_DEC, isFast);
        break;

      case VAL_BYTE:
        internalPrint<byte>(MENU_IP_DEC, isFast);
        break;

      case VAL_DOUBLE:
        internalPrint<double>(MENU_IP_DEC, isFast);
        break;

      case VAL_FLOAT:
        internalPrint<float>(MENU_IP_DEC, isFast);
        break;

      case VAL_BOOLEAN:
        printBoolean(MENU_IP_INC);
        break;
    }

    _oled->update();

    if (cbImmediate) {
      callCb();
    }
  }

  template<typename T>
  void internalPrint(const byte mode, const boolean isFast = false) {
    float nextVal = 0;

    if (mode != MENU_IP_PRINT) {
      if (mode == MENU_IP_INC) {
        nextVal = (float)*(T*)_val + (isFast ? ((*(T*)_inc) * MENU_FAST_K) : *(T*)_inc);
      } else if (mode == MENU_IP_DEC) {
        nextVal = (float)*(T*)_val - (isFast ? ((*(T*)_inc) * MENU_FAST_K) : *(T*)_inc);
      }

      if (nextVal > *(T*)_max) {
        *(T*)_val = *(T*)_min;
      } else if (nextVal < *(T*)_min) {
        *(T*)_val = *(T*)_max;
      } else {
        *(T*)_val = (T)nextVal;
      }
    }

    if (!callPrintOverride()) {
      _oled->print(*(T*)_val);
    }
  }

  void printBoolean(const byte mode = MENU_IP_PRINT) {
    if (mode != MENU_IP_PRINT) {
      *(boolean*)_val = !*(boolean*)_val;
    }

    if (!callPrintOverride()) {
      _oled->print(MENU_BOOLEAN_TEXT[*(boolean*)_val]);
    }
  }

  void onChange(cbOnChange cb, const boolean immediate = false) {
    _onChange = cb;
    cbImmediate = immediate;
  }

  void onPrintOverride(cbOnPrintOverride cb) {
    _onPrintOverride = cb;
  }

  void callCb() {
    if (_onChange == nullptr) {
      return;
    }

    _onChange(_index, _val, _valType);
  }

  boolean callPrintOverride() {
    if (_onPrintOverride == nullptr) {
      return false;
    }

    return _onPrintOverride(_index, _val, _valType);
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
  cbOnPrintOverride _onPrintOverride = nullptr;
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

  void addItem(PGM_P str, const int* inc, int* val, const int* min, const int* max) {
    doAddItem(str, VAL_INTEGER, inc, val, min, max);
  }

  void addItem(PGM_P str, const unsigned int* inc, unsigned int* val, const unsigned int* min, const unsigned int* max) {
    doAddItem(str, VAL_U_INTEGER, inc, val, min, max);
  }

  void addItem(PGM_P str, const double* inc, double* val, const double* min, const double* max) {
    doAddItem(str, VAL_DOUBLE, inc, val, min, max);
  }

  void addItem(PGM_P str, const float* inc, float* val, const float* min, const float* max) {
    doAddItem(str, VAL_FLOAT, inc, val, min, max);
  }

  void addItem(PGM_P str, const byte* inc, byte* val, const byte* min, const byte* max) {
    doAddItem(str, VAL_BYTE, inc, val, min, max);
  }

  void addItem(PGM_P str, boolean* val) {
    doAddItem(str, VAL_BOOLEAN, nullptr, val, nullptr, nullptr);
  }


  void selectNext(const boolean isFast = false) {
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

    gotoIndex(selectedIdx, selectedIdx + 1);
  }


  void selectPrev(const boolean isFast = false) {
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

    gotoIndex(selectedIdx, selectedIdx - 1);
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

  void onChange(cbOnChange cb, const boolean immediate = false) {
    _onItemChange = cb;
    cbImmediate = immediate;
  }

  void onPrintOverride(cbOnPrintOverride cb) {
    _onItemPrintOverride = cb;
  }

  void showMenu(const boolean val, const boolean update = true) {
    if (val == isMenuShowing) {
      return;
    }

    isMenuShowing = val;

    if (isMenuShowing) {
      renderPage(currentPage);
    } else {
      _oled->clear();
      setDefaultOledParams();

      if (update) {
        _oled->update();
      }
    }
  }

  void refresh() {
    renderPage(currentPage);
  }

  byte pageCount() {
    return getPageByIndex(_MS_SIZE - 1);
  }

private:
  TGyverOLED* _oled = nullptr;
  int initInterator = 0;
  OledMenuItem<TGyverOLED> oledMenuItems[_MS_SIZE];
  cbOnChange _onItemChange = nullptr;
  cbOnPrintOverride _onItemPrintOverride = nullptr;

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
    oledMenuItems[initInterator].onPrintOverride(_onItemPrintOverride);
    oledMenuItems[initInterator].setMinMax(min, max);

    initInterator++;
  }

  void renderPage(const byte page, const boolean firstSelect = true) {
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

  void gotoIndex(const byte selectedIdx, int nextIdx) {
    byte nextIndexPage = 0;
    boolean isFirstSelect = true;

    oledMenuItems[selectedIdx].unselect();

    if (nextIdx < 0) {
      nextIndexPage = pageCount();
      isFirstSelect = false;
    } else if (nextIdx > (_MS_SIZE - 1)) {
      nextIndexPage = 1;
      isFirstSelect = true;
    } else {
      nextIndexPage = getPageByIndex(nextIdx);
      isFirstSelect = nextIndexPage > currentPage;
    }

    if (nextIndexPage != currentPage) {
      renderPage(nextIndexPage, isFirstSelect);
      return;
    }

    if (nextIdx < 0) {
      nextIdx = _MS_SIZE - 1;
    } else if (nextIdx > (_MS_SIZE - 1)) {
      nextIdx = 0;
    }

    oledMenuItems[nextIdx].select(true);
  }

  byte getPageByIndex(const byte index) {
        // ((index + 1) + MENU_PAGE_ITEMS_COUNT - 1) - one(1) was cut
    return (index + MENU_PAGE_ITEMS_COUNT) / MENU_PAGE_ITEMS_COUNT;
  }

  void setDefaultOledParams() {
    _oled->textMode(BUF_REPLACE); // default in GyverOLED.h
  }
};

#endif
