
#include "Graphic.h"
#include "Control.h"
#include "Datas.h"
#include <stdbool.h>

struct Area {
    struct Position a_pos;
};

enum Mode {Load, Save};


int select_menu_item(const struct Position position, const struct ButtonGroup group);
void selected_event(int selected_id);
void main_menu();
void csv_forms(bool mode);
void add_data_forms(bool show_default);
void find_data_forms();
void update_data_forms();
void del_data_forms();
int check_valid(int form_id, bool mode);
void set_text(struct Position pos, char *text);
void clear_employee_data();
void set_employee_data(struct Position position, int opt);
void print_employee_data(int opt);
void find_by_keyword(int n, const char *keyword);
void print_find_result(int find_count);
int dialogbox(struct Position position, struct Size box_size, const char title[32], const char text[256], enum dia_type type, char *return_text);
int form_box(struct Position position, struct Form form, bool show_default);
int select_form_item(const struct Position start_pos, const struct Form form);
void init();
void destroy();
void init_temp_employees();

