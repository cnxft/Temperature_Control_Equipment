#include "MENU/menu.h"

uint8_t Page_State = 0;
uint8_t BgColor = 0x00;   // 1为白天模式，0为黑夜模式
uint8_t Options_Time = 2; // 选项缓动动画持续时间（次数）
uint8_t Dialog_Time = 1;  // 对话框缓动动画持续时间（次数）

// 状态机？
typedef struct Error
{
    float error;
    float sum_srror;
    float last_error;
} Error;

Error Cursor;

void Cursor_clear(Error *Obj)
{
    Obj->error = 0;
    Obj->sum_srror = 0;
    Obj->last_error = 0;
}

/* Page*/
xpMenu NowPage;
xMenu
    MainPage,                     // 一级一个Page
    Page3,                        // 二级一个Page
    No3Page1, No3Page2, No3Page3; // 三级三个Page

/**
 *  item
 *  相当于先编个号？
 */
// 一级Page的item
xpItem Selectitm;
xItem Mainitem1, Mainitem2, Mainitem3, Mainitem4;
// 二级Page的item
xItem Page3item1, Page3item2, Page3item3, Page3item4;
// 三级Page的item
xItem No3Page1item1, No3Page1item2, No3Page1item3, No3Page1item4,
    No3Page2item1, No3Page2item2, No3Page2item3, No3Page2item4,
    No3Page3item1, No3Page3item2, No3Page3item3, No3Page3item4;
/**
 * AddPage
 * 一个静态char类型变量，指向名字
 * 指向页
 * 头和尾都置空
 */
void AddPage(const char *name, xpMenu page)
{
    page->PageName = name; // 先把菜单的名字保存好
    page->itemHead = NULL; // 上一页先为空
    page->itemTail = NULL; // 下一页也先为空
}
/**
 * 加载项目函数additem
 * 静态char 名字
 * 哪个项目
 * 在哪也菜单上
 * 指向的下一页
 * 获知有函数没有
 */
void AddItem(const char *Name, xpItem item, xpMenu LocalPage, xpMenu nextpage, Itemfunction function)
{
    item->itemName = Name;
    item->location = LocalPage;
    item->JumpPage = nextpage;
    item->Itemfunction = function;
    /* 新建item的下一个肯定是null */
    item->nextiTem = NULL;
    /* 如果可以跳转，那么此item是跳转页面的父级 */
    if (nextpage != NULL)
        nextpage->ParentiTem = item;
    /* 链式结构创建item */
    if (LocalPage->itemHead == NULL) // 如果是第一个iTem
    {
        item->lastiTem = item;      // 我指向我自己
        LocalPage->itemHead = item; // 我指向我自己
        LocalPage->itemTail = item; // 我指向我自己
        LocalPage->len = 1;         // 数量记作1
    }
    else // 不是第一个item
    {
        item->lastiTem = LocalPage->itemTail;                // 新item的last指向Local的tailitem
        LocalPage->itemTail->nextiTem = item;                // 让尾巴的next指向新的item，连接起来
        LocalPage->itemTail = LocalPage->itemTail->nextiTem; // 让尾巴指向新的item
        LocalPage->len++;
    }
    item->Number = LocalPage->len;
}

/**
 * @brief 线性增长函数用于坐标移动
 *
 * @param AllTime 总时长
 * @param Time_Now 当前时间
 * @param Targrt 目标值
 * @param Now 当前值
 * @return uint8_t
 */
int8_t Line(uint8_t AllTime, uint8_t Time_Now, int8_t Targrt, int8_t Now)
{
    return (Targrt - Now) * Time_Now / AllTime + Now;
}

uint8_t PID(int8_t Targrt, int8_t Now, Error *Obj)
{
    int x = Now;
    float Kp = 0.5, ki = 0.1, kd = 0.25;
    Obj->error = Targrt - x;
    Obj->sum_srror += Obj->error;
    float delta_error = Obj->error - Obj->last_error;
    float velocity = Kp * Obj->error + ki * Obj->sum_srror + kd * delta_error;
    x += velocity;
    Obj->last_error = Obj->error;
    return x;
}
/**
 * 绘制加载动画
 * 分子
 * 分母
 */
void Draw_Process(uint16_t molecule, uint16_t denominator)
{
    // 显示欢迎字
    u8g2_ClearBuffer(&u8g2);
    u8g2_SetFont(&u8g2, u8g2_font_profont15_mf);
    u8g2_DrawStr(&u8g2, 42, 16, "cxjg");
    u8g2_SetFont(&u8g2, MENU_FONT);
    // 显示进度条
    char buff[6];
    int percentage = (int)(molecule * 100 / denominator);
    sprintf(buff, "%02d%%", percentage);
    uint16_t length = (int)(percentage * 0.7);
    u8g2_DrawStr(&u8g2, 100, 41, buff);
    u8g2_DrawRBox(&u8g2, 16, 32, 10 + length, 10, 4);
    u8g2_DrawRFrame(&u8g2, 16, 32, 80, 10, 4);
    u8g2_SendBuffer(&u8g2);
}

void Draw_DialogBox(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h)
{
    u8g2_SetDrawColor(u8g2, BgColor ^ 0x01);
    u8g2_DrawFrame(u8g2, x, y, w, h);
    u8g2_SetDrawColor(u8g2, BgColor);
    u8g2_DrawBox(u8g2, x + 1, y + 1, w - 2, h - 2);
    u8g2_SetDrawColor(u8g2, BgColor ^ 0x01);
}

void Draw_DialogRBox(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h, u8g2_uint_t r)
{
    u8g2_SetDrawColor(u8g2, BgColor ^ 0x01);
    u8g2_DrawRFrame(u8g2, x, y, w, h, r);
    u8g2_SetDrawColor(u8g2, BgColor);
    u8g2_DrawRBox(u8g2, x + 1, y + 1, w - 2, h - 2, r);
    u8g2_SetDrawColor(u8g2, BgColor ^ 0x01);
}
/**
 * @brief 对话框出现函数
 *
 * @param u8g2 U8G2
 * @param x 初始位置x
 * @param y 初始位置y
 * @param Targrt_w 目标宽度
 * @param Targrt_h 目标高度
 */
void DialogScale_Show(u8g2_t *u8g2, uint16_t x, uint16_t y, uint16_t Targrt_w, uint16_t Targrt_h)
{
    uint8_t t = 0;
    uint16_t Init_w = 0, Init_h = 0;

    do
    {
        t++;
        Init_w = Line(Dialog_Time, t, Targrt_w, Init_w);
        Init_h = Line(Dialog_Time, t, Targrt_h, Init_h);
        Draw_DialogBox(u8g2, x, y, Init_w, Init_h);
        u8g2_SendBuffer(u8g2);
    } while (t < Dialog_Time);
}
/**
 * @brief 渐变消失函数
 *
 *
 */
uint8_t ui_disapper(uint8_t disapper)
{
    short disapper_temp = 0;
    int len = 8 * u8g2_GetBufferTileHeight(&u8g2) * u8g2_GetBufferTileWidth(&u8g2);
    uint8_t *p = u8g2_GetBufferPtr(&u8g2);
    if (BgColor == 0)
    {
        for (int i = 0; i < len; i++)
        {
            p[i] = p[i] & (rand() % 0xff) >> disapper;
        }
    }
    else
    {
        for (int i = 0; i < len; i++)
        {
            p[i] = p[i] | (rand() % 0xff) >> disapper;
        }
    }
    disapper += 2;
    if (disapper >= 8)
    {
        disapper = 0;
    }
    u8g2_SendBuffer(&u8g2);
    disapper_temp = disapper;
    return disapper_temp;
}
/**
 * @brief 选项栏
 * 绘制选项位置
 * @param now_time
 * @param now_item
 * @param next_item
 */
void Draw_OptionPlace(uint8_t now_time, xpItem now_item, xpItem next_item)
{
    // 现在的y，这是用来画右侧菜单位置的
    static uint8_t now_Y = 0;
    // 下一个y
    static uint8_t next_Y = 0;
    next_Y = (next_item->Number - 1) * (64 / next_item->location->len);
    // 绘制一条竖线
    u8g2_DrawVLine(&u8g2, 122, 2, 64);
    // 绘制右侧刻度
    for (size_t i = 0; i < next_item->location->len; i++)
    {
        u8g2_DrawHLine(&u8g2, 119, i * (64 / next_item->location->len) + 2, 6);
    }
    // 把这个光标的y坐标算出来
    now_Y = Line(Options_Time, now_time, next_Y, now_Y);
    // 绘制光标
    u8g2_DrawBox(&u8g2, 118, now_Y, 8, 4);
}

/**
 * Draw_Page
 * 绘制页
 * 包括标题，选项等
 */
void Draw_Page(uint8_t pos, xpMenu Page, uint8_t LineSpacing, xpItem now_item, xpItem next_item)
{
    static int8_t first_line = FirstLine;
    xpItem temp = Page->itemHead;

    if (next_item == now_item->JumpPage->itemHead && next_item != now_item) // 切换页面时变量初始化
        first_line = FirstLine;
    if ((next_item->Number - now_item->Number > 0) && Page_State == CURSOR_STATIC)
    {
        Page_State = MENU_MOVE;
        if ((next_item->Number - now_item->Number) > (Page->len - MaxVisible_Number))
            first_line -= ((Page->len - MaxVisible_Number) * Font_Size); // 除去不移动时的项目数
        else
            first_line -= Font_Size;
    }
    else if ((next_item->Number - now_item->Number < 0) && Page_State == CURSOR_STATIC)
    {
        Page_State = MENU_MOVE;
        if ((now_item->Number - next_item->Number) > (Page->len - MaxVisible_Number))
            first_line += ((Page->len - MaxVisible_Number) * Font_Size); // 除去不移动时的项目数
        else
            first_line += Font_Size;
    }
    for (size_t i = 1; i <= Page->len; i++)
    {
        if ((first_line + i * LineSpacing) <= FirstLine)
            ; // 菜单移动时不让标题移动
        else
            u8g2_DrawStr(&u8g2, pos, first_line + i * LineSpacing, temp->itemName);
        temp = temp->nextiTem;
    }
    u8g2_DrawStr(&u8g2, pos, FirstLine, Page->PageName);
}

/**
 * Draw_Menu
 * 绘制选项上的光标
 * pos：位置 position
 * page：页
 * LineSpacing：行间距
 * now_item：现在是哪个选项
 * next_item：下一个是哪个选项
 */
void Draw_Menu(uint8_t pos, xpMenu Page, uint8_t LineSpacing, xpItem now_item, xpItem next_item)
{
    uint8_t t = 0;
    uint8_t item_wide = strlen(now_item->itemName) * 6 + 4;
    static uint8_t item_line = LINE_MIN;
    static int8_t Targrt_line = 0;
    static uint8_t first = 0; // 初始状态

    u8g2_SetMaxClipWindow(&u8g2);   // 设置最大剪辑窗口
    u8g2_SetFont(&u8g2, MENU_FONT); // 设置字体

    if (next_item == now_item->JumpPage->itemHead && next_item != now_item) // 切换页面时变量初始化
    {
        item_line = LINE_MIN;
        Targrt_line = 0;
        first = 0;
        Page_State = 0;
    }
    if ((next_item->Number - now_item->Number == 0 && first == 0) || next_item == now_item->JumpPage->itemHead)
    {
        Targrt_line = LINE_MIN;
        first = 1;
    }
    else if (next_item->Number - now_item->Number > 0)
    {
        Targrt_line += ((next_item->Number - now_item->Number) * Font_Size);
        if (Targrt_line > LINE_MAX) // 防止光标溢出可视范围
        {
            Page_State = CURSOR_STATIC;
            Targrt_line = LINE_MAX;
        }
    }
    else if (next_item->Number - now_item->Number < 0)
    {
        Targrt_line -= ((now_item->Number - next_item->Number) * Font_Size);
        if (Targrt_line < LINE_MIN) // 防止光标溢出可视范围
        {
            Page_State = CURSOR_STATIC;
            Targrt_line = LINE_MIN;
        }
    }
    Page->itemTail->nextiTem = Page->itemHead;
    Page->itemHead->lastiTem = Page->itemTail;
    do
    {
        u8g2_ClearBuffer(&u8g2);
        t++;
        u8g2_SetDrawColor(&u8g2, BgColor);
        u8g2_DrawBox(&u8g2, 0, 0, 128, 64);
        u8g2_SetDrawColor(&u8g2, BgColor ^ 0x01);
        Draw_OptionPlace(t, now_item, next_item);
        Draw_Page(pos, Page, LineSpacing, now_item, next_item);
        u8g2_SetDrawColor(&u8g2, 2);
        item_line = PID(Targrt_line, item_line, &Cursor);
        if (t >= Options_Time)
            item_line = Targrt_line;
        item_wide = PID(strlen(next_item->itemName) * 6 + 4, item_wide, &Cursor);
        if (t >= Options_Time)
            item_wide = strlen(next_item->itemName) * 6 + 4;
        u8g2_DrawRBox(&u8g2, pos + 1, item_line - 1, item_wide, Font_Size, 4);
        u8g2_SendBuffer(&u8g2);
    } while (t < Options_Time);
}

/**
 * Menu_Team(void)
 * 添加菜单的函数
 * 菜单初始化
 */
void Menu_Team(void)
{
    NowPage = &MainPage;
    Selectitm = &Mainitem1;
    MainPage.ParentiTem = NULL;

    // 主菜单列表
    AddPage("[MainPage]", &MainPage);
    AddItem(" -DetectionNTC", &Mainitem1, &MainPage, NULL, DetectionNTC);
    AddItem(" -DetectionOUT", &Mainitem2, &MainPage, NULL, DetectionOUT);
    AddItem(" -Setting", &Mainitem3, &MainPage, &Page3, NULL);
    AddItem(" -Dashboard", &Mainitem4, &MainPage, NULL, Dashboard);

    // setting列表
    AddPage("[Setting]", &Page3);
    AddItem(" -PID", &Page3item1, &Page3, &No3Page1, NULL);
    AddItem(" -NTC", &Page3item2, &Page3, &No3Page2, NULL);
    AddItem(" -OUT", &Page3item3, &Page3, &No3Page3, NULL);
    AddItem(" -Return", &Page3item4, &Page3, &MainPage, NULL);

    // pid设置
    AddPage("[PID]", &No3Page1);
    AddItem(" -KP", &No3Page1item1, &No3Page1, NULL, KPSET);
    AddItem(" -KI", &No3Page1item2, &No3Page1, NULL, KISET);
    AddItem(" -KD", &No3Page1item3, &No3Page1, NULL, KDSET);
    AddItem(" -Return", &No3Page1item4, &No3Page1, &Page3, NULL);

    // ntc设置
    AddPage("[NTC]", &No3Page2);
    AddItem(" -ntcb", &No3Page2item1, &No3Page2, NULL, NTCBSET);
    AddItem(" -ntcr1", &No3Page2item2, &No3Page2, NULL, NTCR1SET);
    AddItem(" -ntcr2", &No3Page2item3, &No3Page2, NULL, NTCR2SET);
    AddItem(" -Return", &No3Page2item4, &No3Page2, &Page3, NULL);

    // out设置
    AddPage("[OUT]", &No3Page3);
    AddItem(" -time_delta", &No3Page3item1, &No3Page3, NULL, TIME_DELTA_SET);
    AddItem(" -Return", &No3Page3item2, &No3Page3, &Page3, NULL);
}

uint8_t MENU_STATE = MENU_RUN;

uint8_t disapper = 1;
xpItem temp_item = &Mainitem1;
Itemfunction App_Function;

/**
 * 选择菜单运行状态
 */
void Switch_Menu_State(uint8_t state)
{
    MENU_STATE = state;
}

/**
 * 菜单运行
 */
void Process_Menu_Run(uint8_t Dir)
{
    switch (Dir)
    {
    case MENU_UP:
        Draw_Menu(FirstPos, temp_item->location, Font_Size, temp_item, temp_item->lastiTem);
        temp_item = temp_item->lastiTem;
        break;
    case MENU_DOWN:
        Draw_Menu(FirstPos, temp_item->location, Font_Size, temp_item, temp_item->nextiTem);
        temp_item = temp_item->nextiTem;
        break;
    case MENU_LEFT:
    {
        // if (MENU_STATE == MENU_RUN)
        // {
        //     Switch_Menu_State(MENU_RUN);
        //     // for (size_t i = 0; i < 8; i++)
        //     // {
        //     //     disapper = ui_disapper(disapper);
        //     // }
        //     Draw_Menu(FirstPos, temp_item->JumpPage, Font_Size, temp_item, temp_item->JumpPage->itemHead);
        //     temp_item = temp_item->JumpPage->itemHead;
        // }
        if (MENU_STATE == APP_BREAK)
        {
            Switch_Menu_State(MENU_RUN);
            // for (size_t i = 0; i < 8; i++)
            // {
            //     disapper = ui_disapper(disapper);
            // }
            Draw_Menu(FirstPos, temp_item->location, Font_Size, temp_item, temp_item);
        }
    }
    break;
    case MENU_RIGHT:
        if (temp_item->Itemfunction != NULL)
        {
            // ui_disapper(disapper);
            App_Function = temp_item->Itemfunction;
            Switch_Menu_State(APP_RUN);
        }
        else
        {
            Switch_Menu_State(MENU_RUN);
            // for (size_t i = 0; i < 8; i++)
            // {
            //     disapper = ui_disapper(disapper);
            // }
            Draw_Menu(FirstPos, temp_item->JumpPage, Font_Size, temp_item, temp_item->JumpPage->itemHead);
            temp_item = temp_item->JumpPage->itemHead;
        }
        break;
    case MENU_ENTER:
        // 当点到了Enter，判断后边是app程序还是下一级菜单
        if (MENU_STATE == MENU_RUN)
        {
            if (temp_item->Itemfunction != NULL)
            {
                // ui_disapper(disapper);
                App_Function = temp_item->Itemfunction;
                Switch_Menu_State(APP_RUN);
            }
            else
            {
                Switch_Menu_State(MENU_RUN);
                // for (size_t i = 0; i < 8; i++)
                // {
                //     disapper = ui_disapper(disapper);
                // }
                Draw_Menu(FirstPos, temp_item->JumpPage, Font_Size, temp_item, temp_item->JumpPage->itemHead);
                temp_item = temp_item->JumpPage->itemHead;

                // 添加一个清空PID参数的函数，防止换页时的震动
                Cursor_clear(&Cursor);
            }
        }
        if (MENU_STATE == APP_BREAK)
        {
            Switch_Menu_State(MENU_RUN);
            // for (size_t i = 0; i < 8; i++)
            // {
            //     disapper = ui_disapper(disapper);
            // }
            Draw_Menu(FirstPos, temp_item->location, Font_Size, temp_item, temp_item);
        }
        break;
    default:
        break;
    }
}

void Process_App_Run(void)
{
    (*App_Function)();
}

void Menu_Init(void)
{
    Menu_Team(); // 菜单初始化的核心
    Draw_Menu(FirstPos, &MainPage, Font_Size, &Mainitem1, &Mainitem1);
}
