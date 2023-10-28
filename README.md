[![Foo](https://img.shields.io/badge/README-ENGLISH-blueviolet.svg?style=flat-square)](https://github-com.translate.goog/GyverLibs/GyverOLED?_x_tr_sl=ru&_x_tr_tl=en)

# GyverOLED MENU

### Что это:
Это возможность быстро добавить в ваш проект меню настрокек или мини-панель управления какими-то парметрами

https://github.com/gewisser/GyverOLEDMenu/assets/5417292/46c5d222-9373-4607-a67f-1dd8f44a5d07

Бибилиотека в себе использует библиотеку GyverOLED, как зависимость для работы с OLED дисплем https://github.com/GyverLibs/GyverOLED

- Поддерживает многостраничное отображение меню;
- Для каждого элемента меню настраивается значение инкременты / декременты;
- Для каждого элемента меню настраивается минимальное и максимальное допустимое значение;
- По измнению значений можно добавить колбэк срабатывающий после завершения редактирования или на каждом этапе приращения инкременты / декременты.

### Совместимость
Совместима со всеми Arduino платформами (используются Arduino-функции)

## Содержание
- [Установка](#install)
- [Инициализация](#init)
- [Использование](#usage)
- [Пример](#example)
- [Версии](#versions)
- [Баги и обратная связь](#feedback)

<a id="install"></a>
## Установка
- [Скачать библиотеку](https://github.com/GyverLibs/GyverOLED/archive/refs/heads/main.zip) .zip архивом для ручной установки:
  - Распаковать и положить в *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
  - Распаковать и положить в *C:\Program Files\Arduino\libraries* (Windows x32)
  - Распаковать и положить в *Документы/Arduino/libraries/*
  - (Arduino IDE) автоматическая установка из .zip: *Скетч/Подключить библиотеку/Добавить .ZIP библиотеку…* и указать скачанный архив
- Читай более подробную инструкцию по установке библиотек [здесь](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Обновление
- Рекомендую всегда обновлять библиотеку: в новых версиях исправляются ошибки и баги, а также проводится оптимизация и добавляются новые фичи
- Через менеджер библиотек IDE: найти библиотеку как при установке и нажать "Обновить"
- Вручную: **удалить папку со старой версией**, а затем положить на её место новую. "Замену" делать нельзя: иногда в новых версиях удаляются файлы, которые останутся при замене и могут привести к ошибкам!


<a id="init"></a>
## Инициализация
```cpp
#include <GyverOLED.h>
#include "GyverOLEDMenu.h"

// дефайны меню которые можно переопределить
// #define MENU_SELECTED_H 10 // высота элемента меню
// #define MENU_PARAMS_LEFT_OFFSET 92 // смещение вправо для рендеринга значений
// #define MENU_PAGE_ITEMS_COUNT 6 // Количесвто элементов меню на одной странице
// #define MENU_FAST_K 4 // коэффициент для ускоренного приращения (см. isFast ниже)

GyverOLED<SSH1106_128x64> oled;
OledMenu<9, GyverOLED<SSH1106_128x64>> menu(&oled); // Где 9 - количество элементов в меню
```

<a id="usage"></a>
## Использование
```cpp
// ===== СЕРВИС =====

boolean menu.isMenuShowing; // Показано ли сейчас меню
byte menu.currentPage = 1; // Текущая страница

/*
  addItem
  Полседовательное конфигурирование элементов меню в массиве 
*/
void menu.addItem(PGM_P str); // Добавит экшен (просто кликабельный item)
void menu.addItem(PGM_P str, const int* inc, int* val, const int min, const int max);
void menu.addItem(PGM_P str, const double* inc, double* val, const double min, const double max);
void menu.addItem(PGM_P str, const float* inc, float* val, const float min, const float max);
void menu.addItem(PGM_P str, const byte* inc, byte* val, const byte min, const byte max);
void menu.addItem(PGM_P str, boolean* val);

/*
  showMenu
  Показать / скрыть меню  (val - true/false)
  update - вызовет обновление oled дисплея после готовности к отрисовке меню
*/
void menu.showMenu(boolean val, boolean update = true);

/*
  refresh
  Текущая страница меню заново будет отрисована
*/
void menu.refresh();



// ===== КУРСОР =====

/*
  selectNext
  Переместит курсор на следующий item. В режиме редактирования этот метод сделает
  инкремент значения, где isFast = true, увеличит инкремент в 4 раза (#define MENU_FAST_K 4)
*/
void menu.selectNext(boolean isFast = false);

/*
  selectPrev
  Переместит курсор на предидущий item. В режиме редактирования этот метод сделает
  декремент значения, где isFast = true, увеличит декремент в 4 раза (#define MENU_FAST_K 4)
*/
void menu.selectPrev(boolean isFast = false)

/*
  Циклично переключает выделенный элемент в режим редактирования / выход из режима редактирования
*/
void menu.toggleChangeSelected();


// ===== CALLBACK =====

/*
  cbOnChange
  Колбек вызывается всякий раз когда происходит выход из режима редактирования
  или непосредсвтенно при изменении значения

  index - текущий индекс меню в массиве. Первый элемент меню будет с индексом - 0
  val - указатель на изменённую переменную
  valType - тип изменённой переменной:
    #define VAL_ACTION 0
    #define VAL_INTEGER 1
    #define VAL_FLOAT 2
    #define VAL_DOUBLE 3
    #define VAL_BYTE 4
    #define VAL_BOOLEAN 5
*/
typedef void (*cbOnChange)(int index, void* val, int valType);

/*
  Добавление колбэка

  cb - вызываемый метод
  immediate - отвечает за то когда будет вызван колбек.
    если immediate == false, то колбэк будет вызван после выхода из режима редактирования параметра,
    иначе любое изменение приведёт к вызову колбека
  
*/
void onChange(cbOnChange cb, boolean immediate = false);

```

<a id="example"></a>
## Пример
```cpp
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
```

<a id="versions"></a>
## Версии
- v0.1 (28.10.2023) - Первая стабильная версия

<a id="feedback"></a>
## Баги и обратная связь
При нахождении багов создавайте **Issue**
Библиотека открыта для доработки и ваших **Pull Request**'ов!

При сообщении о багах или некорректной работе библиотеки нужно обязательно указывать:
- Версия библиотеки
- Какой используется МК
- Версия SDK (для ESP)
- Версия Arduino IDE
- Корректно ли работают ли встроенные примеры, в которых используются функции и конструкции, приводящие к багу в вашем коде
- Какой код загружался, какая работа от него ожидалась и как он работает в реальности
- В идеале приложить минимальный код, в котором наблюдается баг. Не полотно из тысячи строк, а минимальный код
