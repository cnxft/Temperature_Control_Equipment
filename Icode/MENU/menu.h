#ifndef __MENU_H
#define __MENU_H

#include "USERAPP/UserApp.h"

typedef struct
{
    u8g2_uint_t x;
    u8g2_uint_t y;
    char *str;
} GUI_MAIN_LIST;

typedef struct
{
    char *str;
} SETTING_LIST;

typedef struct
{
    uint8_t val;
    uint8_t last_val;
} KEY_T;

#define FirstLine 9
#define FirstPos 0
#define Font_Size 12
#define LINE_MAX 48
#define LINE_MIN 12
#define MaxVisible_Number 4
#define MENU_FONT u8g2_font_profont12_mf
#define MENU_INTERVAL_TIME 100

/**
 * 菜单状态
 */
enum MenuState
{
    MENU_RUN,
    APP_RUN,
    APP_BREAK,
    MENU_MOVE,
    CURSOR_STATIC,

    MENU_UP,
    MENU_DOWN,
    MENU_LEFT,
    MENU_RIGHT,
    MENU_ENTER
};

typedef void (*Itemfunction)(void);
typedef struct MenuPage *xpMenu;
typedef struct Item *xpItem;

/**
 * 菜单页
 */
typedef struct MenuPage
{
    const char *PageName;
    uint8_t len;
    xpItem ParentiTem; // 父级选项或者从哪个选项跳转过来的
    xpItem itemHead;   // 选项头
    xpItem itemTail;   // 选项尾
} xMenu;

/**
 * 选项
 * 并形成链表
 */
typedef struct Item
{
    const char *itemName;
    uint8_t Number;
    xpMenu location; // 在哪一页上
    xpMenu JumpPage; // 指向哪一页
    xpItem lastiTem; // 最后的选项？
    xpItem nextiTem; // 下一个选项
    void (*Itemfunction)();
} xItem;

extern uint8_t MENU_STATE;

void Menu_Team(void);
void AddPage(const char *name, xpMenu page);
void AddItem(const char *Name, xpItem item, xpMenu LocalPage, xpMenu nextpage, Itemfunction function);
int8_t Line(uint8_t AllTime, uint8_t Time_Now, int8_t Tgt, int8_t Now);
void Draw_Process(uint16_t molecule, uint16_t denominator);
void Draw_OptionPlace(uint8_t now_time, xpItem now_item, xpItem next_item);
void Draw_DialogBox(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h);
void Draw_DialogRBox(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h, u8g2_uint_t r);
void DialogScale_Show(u8g2_t *u8g2, uint16_t x, uint16_t y, uint16_t Targrt_w, uint16_t Targrt_h);
void Draw_Page(uint8_t pos, xpMenu Page, uint8_t LineSpacing, xpItem now_item, xpItem next_item);
void Draw_Menu(uint8_t pos, xpMenu Page, uint8_t LineSpacing, xpItem now_item, xpItem next_item);
uint8_t ui_disapper(uint8_t disapper);
void Switch_Menu_State(uint8_t state);
void Menu_Init(void);

void Process_Menu_Run(uint8_t Dir);
void Process_App_Run(void);

#endif /* __MENU_H */
