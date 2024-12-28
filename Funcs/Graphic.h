
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/* Misc */
// 方向
enum Direction {Up = 'A', Down, Right, Left};
// 定义坐标 位置
struct Position {
    int line, col;
};
// 定义尺寸大小
struct Size {
    int width, height;
};

// 定义按钮
struct Button {
    int id;
    char title[64];
};

// 定义按钮组
struct ButtonGroup {
    int gid;
    struct Button *groups;
    int count;
};

// 定义对话框类型
enum dia_type {INFO, YES_NO, YES_NO_CANCEL, INPUT};

// 定义项目
struct Item {
    int id;
    char title[64];
};

// 定义项目类型
enum Item_type {
    Input,
    Int_input,
    Float_input,
    Combo
};

// 定义表单
struct Form {
    int id;
    char form_title[64];
    struct Item *items;
    int item_count;
};

/* SCREEN */
void clear_scr();
void move_cursor(const int LINE, const int COL);
void move_cursor_by_pos(const struct Position pos);
void move_cur(const enum Direction, const int);
void show_cursor();
void hide_cursor();

/* Widgets */
void draw_frame(const struct Position, const struct Size, const char* TITLE, const char* TEXT);
void draw_button(const struct Position, const struct Button button, const struct Size btn_size);
void draw_buttonGroup(const struct Position, const struct Button groups[16], const int btn_count);
void draw_input_line_box(const struct Position, const int width);
void draw_forms(const struct Position form_pos, const char* form_title, const struct Item *items, const int item_count);
void set_focus(const struct Position position, const struct ButtonGroup buttonGroup, const int INDEX, const int is_focused);
