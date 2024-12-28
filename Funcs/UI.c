
#include "UI.h"

#include <time.h>
struct Employee tmp_employee;
struct Employee t_employees[1000];
// 选择菜单项目
int select_menu_item(const struct Position position, const struct ButtonGroup group) {
    int option = 0;
    set_focus(position, group, option, 1);
    char ch = '\0';
    while (ch != K_Enter) {
        ch = get_key();
        set_focus(position, group, option, 0);
        switch (ch) {
            case 'w':
            case 'W':
            case 'i':
            case 'I':
                if (option > 0) option--;
                else option = group.count - 1;
                break;
            case 's':
            case 'S':
            case 'k':
            case 'K':
            case K_Tab:
                if (option < group.count - 1) option++;
                else option = 0;
                break;
            case K_Esc:
                return -1;
            default:
                break;
        }
        set_focus(position, group, option, 1);
    }
    return group.groups[option].id;
}

// 菜单项目选择事件
void selected_event(int selected_id) {
    switch (selected_id) {
    case 1:
        find_data_forms();
        main_menu();
        break;
    case 2:
        add_data_forms(false);
        main_menu();
        break;
    case 3:
        update_data_forms();
        main_menu();
        break;
    case 4:
        del_data_forms();
        main_menu();
        break;
    case 5:
        csv_forms(true);
        main_menu();
        break;
    case 6:
        csv_forms(false);
        main_menu();
        break;
    case 7:
        destroy();
        break;
    default:
        main_menu();
        break;
    }
}

// 主菜单
void main_menu() {
    clear_scr();
    struct Position pos = {4, 4};
    struct Size size = {76, 20};
    struct Button buttons[7] = {
        {1, "查询职工信息"},
        {2, "添加职工信息"},
        {3, "修改职工信息"},
        {4, "删除职工信息"},
        {5, "导入职工数据"},
        {6, "导出职工数据"},
        {7, "退出管理系统"}
    };
    struct ButtonGroup select_items = {1, buttons, 7};
    draw_frame(pos, size, "", "   【操作菜单说明】\n  Tab / W: 切换上一项，\n  S: 切换下一项 \n  Enter: 选择菜单项目\n  Esc: 取消选择");
    pos.line = 4;
    pos.col = 28;
    move_cursor_by_pos(pos);
    printf("欢迎使用 职工信息管理系统");
    pos.line = 6; pos.col = 31;
    draw_buttonGroup(pos, buttons, 7);
    int n = select_menu_item(pos, select_items);
    selected_event(n);
}

// 导入/导出 CSV 表格
void csv_forms(bool mode) {
    struct Position position = {7, 20};
    struct Size size = {41, 13};
    int opt, err;
    if (mode) {
        opt = dialogbox(position, size, "导入", "\n  即将从现有目录中导入 CSV 表格，\n  是否确定导入？", YES_NO, "");
        if (opt) {
            draw_frame(position, size, "", "\n\n\n  正在读取 CSV 表格文件...");
            err = import_csv("datas/sheet.csv");
            if (err) {
                dialogbox(position, size, "导入失败", "\n  找不到或无法正常读取 CSV 文件！", INFO, "");
            }
        }
    } else {
        opt = dialogbox(position, size, "导出", "\n  即将从现有数据导出为 CSV 表格，\n  是否确定导出？", YES_NO, "");
        if (opt) {
            draw_frame(position, size, "", "\n\n\n  正在导出 CSV 表格文件...");
            err = export_csv("datas/sheet.csv");
            if (err) {
                dialogbox(position, size, "导出失败", "\n  找不到现有的 datas 目录！\n  请先手动创建 datas 目录后再试！", INFO, "");
                return;
            }
            opt = dialogbox(position, size, "打开外部程序？", "\n  导出已完成！\n  是否尝试打开表格程序查看 CSV 表格？", YES_NO, "");
            if (opt) system("start datas/sheet.csv");
        }
    }
}

// 添加数据表单
void add_data_forms(bool show_default) {
    struct Position position = {3, 22};
    struct Item items[32] = {
            {1, "职工编号"},
            {2, "职工姓名"},
            {3, "职工性别"},
            {4, "职工年龄"},
            {5, "个人学历"},
            {6, "所得工资"},
            {7, "所属部门"},
            {8, "手机号码"},
    };
    struct Form form = {1, "", items, 8};
    if (show_default) {
        strcpy(form.form_title, "修改职工信息");
    } else {
        strcpy(form.form_title, "添加职工信息");
    }
    form_box(position, form, true);
    int e = select_form_item(position, form);
    if (e) {
        clear_employee_data();
        return;
    }
    int f = check_valid(form.id, show_default);
    while (!e && f) {
        form_box(position, form, true);
        e = select_form_item(position, form);
        f = check_valid(form.id, show_default);
    }
    if (!f) {
        if (!show_default) add_employee(tmp_employee);
        else modify_employee(tmp_employee);
    }
    clear_employee_data();
}

// 查找数据表单
void find_data_forms() {
    struct Position position = {7, 26};
    struct Size size = {28, 13};
    draw_frame(position, size, "选择某种方式搜索？", "");
    struct Button btn[4] = {
            {11, "职工编号"},
            {12, "职工性别"},
            {13, "个人学历"},
            {14, "所属部门"}
    };
    struct ButtonGroup btn_group = {2, btn, 4};
    struct Position btn_pos = {position.line + 2, position.col + 7};
    draw_buttonGroup(btn_pos, btn, 4);
    int n = select_menu_item(btn_pos, btn_group), t;
    position.col -= 2;
    size.width += 8;
    char find_key[64] = {'\0'};
    switch (n) {
        case 11:
            t = dialogbox(position, size, "查找职工编号", "\n 键入 Enter 以进行或退出编辑：", INPUT, find_key);
            break;
        case 12:
            t = dialogbox(position, size, "查找职工性别", "\n 键入 Enter 以进行或退出编辑：", INPUT, find_key);
            break;
        case 13:
            t = dialogbox(position, size, "查找个人学历", "\n 键入 Enter 以进行或退出编辑：", INPUT, find_key);
            break;
        case 14:
            t = dialogbox(position, size, "查找所属部门", "\n 键入 Enter 以进行或退出编辑：", INPUT, find_key);
            break;
    }
    if (t) {
        init_temp_employees();
        find_by_keyword(n, find_key);
    }
}

// 关键字查询
void find_by_keyword(int n, const char *keyword) {
    if (!strlen(keyword)) {
        struct Position position = {7, 26};
        struct Size size = {28, 13};
        dialogbox(position, size, "提示", "\n  请输入搜索关键字！", INFO, "");
        return;
    }
    int k;
    switch (n) {
        case 11:
            k = find_all_employees_by_id(keyword, t_employees);
            break;
        case 12:
            k = find_employee_by_sex(keyword, t_employees);
            break;
        case 13:
            k = find_employee_by_degree(keyword, t_employees);
            break;
        case 14:
            k = find_employee_by_department(keyword, t_employees);
            break;
    }
    print_find_result(k);
    wait();
}

// 显示查询结果
void print_find_result(int find_count) {
    clear_scr();
    struct Position position = {3, 3};
    struct Size size = {101, 5};
    draw_frame(position, size, "查询结果", " 以下为查询到的所有结果。\n 在此可使用鼠标滚轮查看；按任意键可返回至主菜单");
    move_cursor(9, 3);
    if (!find_count) {
        printf("查找不到匹配的数据！");
        return;
    }
    for (int j = 0; j <= 100; ++j) {
        printf("-");
    }
    printf("\n");
    printf("  |%10s|%16s|%8s|%8s|%10s|%8s|%16s|%16s|\n  ",
           "职工编号", "职工姓名", "职工性别", "职工年龄", "个人学历",
           "所得工资", "所属部门", "手机号码");
    for (int j = 0; j <= 100; ++j) {
        printf("-");
    }
    printf("\n");
    for (int i = 0; i < find_count; ++i) {
        printf("  |%10s|%16s|%8s|%8d|%10s|%8.2lf|%16s|%16s|\n  ",
               t_employees[i].id, t_employees[i].name, t_employees[i].sex,
               t_employees[i].age, t_employees[i].degree, t_employees[i].salary,
               t_employees[i].department, t_employees[i].phone_num);
        for (int j = 0; j <= 100; ++j) {
            printf("-");
        }
        printf("\n");
    }
}

// 更新数据表单
void update_data_forms() {
    char keys[64] = {'\0'};
    struct Position position = {7, 20};
    struct Size size = {40, 13};
    int t = dialogbox(position, size, "查找职工编号", "\n 键入 Enter 以进行或退出编辑：",
                      INPUT, keys);
    if (t) tmp_employee = find_employee_by_id(keys);
    else return;
    if (strcmp(tmp_employee.id, "")) {
        add_data_forms(true);
    } else {
        dialogbox(position, size, "查无此人", "\n  查找不到此职工！", INFO, "");
    }
    clear_employee_data();
}

// 删除数据表单
void del_data_forms() {
    char keys[64] = {'\0'};
    struct Position position = {7, 20};
    struct Size size = {40, 13};
    int t = dialogbox(position, size, "查找职工编号", "\n 键入 Enter 以进行或退出编辑：",
                      INPUT, keys);
    if (t) tmp_employee = find_employee_by_id(keys);
    else return;
    if (strcmp(tmp_employee.id, "")) {
        char text[64] = {'\0'};
        strcpy(text, "\n  查找到名为【");
        strcat(text, tmp_employee.name);
        strcat(text, "】的职工。\n\n  要删除该职员的所有信息吗");
        position.line = 7; position.col = 20;
        size.width = 40; size.height = 13;
        int opt = dialogbox(position, size, "确认删除？", text, YES_NO, "");
        if (opt == 1) {
            del_employee_by_id(tmp_employee.id);
        }
    } else {
        dialogbox(position, size, "查无此人", "\n  查找不到此职工！", INFO, "");
    }
    clear_employee_data();
}

// 检查输入的表单是否符合
int check_valid(int form_id, bool mode) {
    struct Position position = {9, 24};
    struct Size size = {29, 9};
    struct Employee find_emp;
    if (form_id == 1) {
        find_emp = find_employee_by_id(tmp_employee.id);
        if (strlen(tmp_employee.id) < 6) {
            dialogbox(position, size, "填写错误", "\n  职工编号不得小于 6 位！", INFO, "");
            return 1;
        } else if (!strcmp(find_emp.id, tmp_employee.id) && !mode) {
            dialogbox(position, size, "填写错误", "\n  职工编号存在重复！", INFO, "");
            return 1;
        } else if (strlen(tmp_employee.name) == 0) {
            dialogbox(position, size, "填写错误", "\n  职工姓名不能为空！", INFO, "");
            return 1;
        } else if (strlen(tmp_employee.sex) == 0) {
            dialogbox(position, size, "填写错误", "\n  职工性别不能为空！", INFO, "");
            return 1;
        } else if (strcmp(tmp_employee.sex, "男") != 0 && strcmp(tmp_employee.sex, "女") != 0) {
            dialogbox(position, size, "填写错误", "\n  职工性别只能填写：\n   “男” 或 “女”！", INFO, "");
            return 1;
        } else if (tmp_employee.age < 18 || tmp_employee.age > 60) {
            dialogbox(position, size, "填写错误", "\n  职工年龄只能在 18-60 岁之间！", INFO, "");
            return 1;
        } else if (strlen(tmp_employee.degree) == 0) {
            dialogbox(position, size, "填写错误", "\n  个人学历不能为空！", INFO, "");
            return 1;
        } else if (strlen(tmp_employee.department) == 0) {
            dialogbox(position, size, "填写错误", "\n  所属单位不能为空！", INFO, "");
            return 1;
        } else if (strlen(tmp_employee.phone_num) == 0) {
            dialogbox(position, size, "填写错误", "\n  手机号码不能为空！", INFO, "");
            return 1;
        } else if (strlen(tmp_employee.phone_num) != 11) {
            dialogbox(position, size, "填写错误", "\n  手机号码必须为 11 位！", INFO, "");
            return 1;
        }
    }
    return 0;
}


// 对话框
int dialogbox(struct Position position, struct Size box_size, const char *title, const char *text, enum dia_type type, char *return_text) {
    if (box_size.width < 20 || box_size.height < 9) {
        return -1;
    }
    draw_frame(position, box_size, title, text);
    struct Button ok, yes, no, cancel;
    struct Size t_size = {13, 3};
    struct Position input_pos = {position.line + box_size.height - 7, position.col + 1};
    switch (type) {
        case INFO:
            ok.id = 1;
            strcpy(ok.title, " 好(Enter)");
            struct Position t_pos = {position.line + box_size.height - 4,
                                position.col + (box_size.width / 2 - t_size.width / 2)};
            draw_button(t_pos, ok, t_size);
            break;
        case INPUT:
            yes.id = 1; no.id = 0;
            strcpy(yes.title, " 好(Ctl+Enter)");
            strcpy(no.title, " 取消(Esc)");
            struct Position acc_pos = {position.line + box_size.height - 4, position.col + 1};
            struct Position cl_pos = {position.line + box_size.height - 4,
                                position.col + box_size.width - t_size.width - 1};
            draw_button(cl_pos, no, t_size);
            t_size.width += 4;
            draw_button(acc_pos, yes, t_size);
            draw_input_line_box(input_pos, box_size.width - 3);
            break;
        case YES_NO:
            yes.id = 1; no.id = 0;
            strcpy(yes.title, " 是(Enter)");
            strcpy(no.title, " 否(Bcksp)");
            struct Position yes_pos = {position.line + box_size.height - 4, position.col + 1};
            struct Position no_pos = {position.line + box_size.height - 4,
                                position.col + box_size.width - t_size.width - 1};
            draw_button(yes_pos, yes, t_size);
            draw_button(no_pos, no, t_size);
            break;
        case YES_NO_CANCEL:
            yes.id = 1; no.id = 2; cancel.id = 0;
            strcpy(yes.title, " 是(Enter)");
            strcpy(no.title, " 否(Bcksp)");
            strcpy(cancel.title, " 取消(Esc)");
            struct Position y_pos = {position.line + box_size.height - 4, position.col + 1};
            struct Position n_pos = {position.line + box_size.height - 4,
                                position.col + (box_size.width / 2 - t_size.width / 2)};
            struct Position c_pos = {position.line + box_size.height - 4,
                                position.col + box_size.width - t_size.width - 1};
            draw_button(y_pos, yes, t_size);
            draw_button(n_pos, no, t_size);
            draw_button(c_pos, cancel, t_size);
            break;
    }
    while (true) {
        char ch = get_key();
        switch (type) {
            case INFO:
                if (ch == K_Enter) return ok.id;
                break;
            case INPUT:
                if (ch == K_Enter) {
                    draw_input_line_box(input_pos, box_size.width - 3);
                    set_text(input_pos, return_text);
                }
                if (ch == K_Ctrl_Enter) return yes.id;
                if (ch == K_Esc) return no.id;
                break;
            case YES_NO:
                if (ch == K_Enter) return yes.id;
                if (ch == K_BackSpace) return no.id;
                break;
            case YES_NO_CANCEL:
                if (ch == K_Enter) return yes.id;
                if (ch == K_BackSpace) return no.id;
                if (ch == K_Esc) return cancel.id;
                break;
        }
    }
}

void set_text(struct Position pos, char *text) {
    char r_text[64] = {'\0'};
    show_cursor();
    move_cursor_by_pos(pos);
    fgets(r_text, sizeof(r_text), stdin);
    int len = strlen(r_text);
    r_text[len - 1] = '\0';
    strcpy(text, r_text);
    hide_cursor();
}

// 创建表单
int form_box(struct Position position, struct Form form, bool show_default) {
    draw_forms(position, form.form_title, form.items, form.item_count);
    if (show_default) {
        switch (form.id) {
            case 1:
                for (int i = 0; i < 8; ++i) {
                    move_cursor(position.line + 2 * (i + 1), position.col + 14);
                    print_employee_data(i);
                }
                break;
            case 2:
                break;
        }
    }
    position.col += 35;
    struct Size size = {29, 12};
    draw_frame(position, size, "操作说明", "\n  Tab / S: 切换下一项\n  W: 切换上一项\n  Enter: 进入/退出编辑\n  [Note] 进入编辑后内容将被清空\n  R: 刷新表单\n  Ctrl+Enter: 提交\n  Esc: 取消");
    position.col -= 35;
    return 0;
}

// 选择表单项目
int select_form_item(const struct Position start_pos, const struct Form form) {
    int opt = 0;
    move_cursor(start_pos.line + 2 * (opt + 1), start_pos.col + 12);
    printf("→");
    while (true) {
        char ch = get_key();
        move_cursor(start_pos.line + 2 * (opt + 1), start_pos.col + 12);
        printf("  ");
        switch (ch) {
            // 切换项目
            case 'W':
            case 'w':
                if (opt == 0) opt = form.item_count - 1;
                else opt -= 1;
                break;
            case 'S':
            case 's':
            case K_Tab:
                if (opt < form.item_count - 1) opt += 1;
                else opt = 0;
                break;
            case 'R':
            case 'r':
                form_box(start_pos, form, true);
                break;
            // 进入编辑模式
            case K_Enter:
                // move_cursor(start_pos.line + 2 * (opt + 1), start_pos.col + 14);
                for (int i = 0; i < 18; ++i) {
                    printf(" ");
                }
                move_cur(Left, 18);
                show_cursor();
                if (form.id == 1) {
                    set_employee_data(start_pos, opt);
                }
                hide_cursor();
                break;
            // 提交表单
            case K_Ctrl_Enter:
                return 0;
            // 取消提交
            case K_Esc:
                return 1;
        }
        move_cursor(start_pos.line + 2 * (opt + 1), start_pos.col + 12);
        printf("→");
    }
}

// 设置职员数据
void set_employee_data(struct Position pos, int opt) {
    char tmp[128];
    int tmp_d, len;
    double tmp_ld;
    switch (opt) {
        case 0:
            fgets(tmp, 16, stdin);
            len = strlen(tmp);
            tmp[len - 1] = '\0';
            strcpy(tmp_employee.id, tmp);
            break;
        case 1:
            fgets(tmp, 32, stdin);
            len = strlen(tmp);
            tmp[len - 1] = '\0';
            strcpy(tmp_employee.name, tmp);
            break;
        case 2:
            fgets(tmp, 4, stdin);
            len = strlen(tmp);
            tmp[len - 1] = '\0';
            strcpy(tmp_employee.sex, tmp);
            break;
        case 3:
            fgets(tmp, 8, stdin);
            len = strlen(tmp);
            tmp[len - 1] = '\0';
            tmp_d = atoi(tmp);
            tmp_employee.age = tmp_d;
            break;
        case 4:
            fgets(tmp, 32, stdin);
            len = strlen(tmp);
            tmp[len - 1] = '\0';
            strcpy(tmp_employee.degree, tmp);
            break;
        case 5:
            fgets(tmp, 16, stdin);
            len = strlen(tmp);
            tmp[len - 1] = '\0';
            tmp_ld = atof(tmp);
            tmp_employee.salary = tmp_ld;
            break;
        case 6:
            fgets(tmp, 32, stdin);
            len = strlen(tmp);
            tmp[len - 1] = '\0';
            strcpy(tmp_employee.department, tmp);
            break;
        case 7:
            fgets(tmp, 32, stdin);
            len = strlen(tmp);
            tmp[len - 1] = '\0';
            strcpy(tmp_employee.phone_num, tmp);
            break;
    }
}

// 输出职工信息
void print_employee_data(int opt) {
    switch (opt) {
        case 0:
            printf("%s", tmp_employee.id);
            break;
        case 1:
            printf("%s", tmp_employee.name);
            break;
        case 2:
            printf("%s", tmp_employee.sex);
            break;
        case 3:
            printf("%d", tmp_employee.age);
            break;
        case 4:
            printf("%s", tmp_employee.degree);
            break;
        case 5:
            printf("%.2lf", tmp_employee.salary);
            break;
        case 6:
            printf("%s", tmp_employee.department);
            break;
        case 7:
            printf("%s", tmp_employee.phone_num);
            break;
    }
}

// 清空职员数据
void clear_employee_data() {
    strcpy(tmp_employee.id, "");
    strcpy(tmp_employee.name, "");
    strcpy(tmp_employee.sex, "");
    tmp_employee.age = 0;
    strcpy(tmp_employee.degree, "");
    tmp_employee.salary = 0.0;
    strcpy(tmp_employee.department, "");
    strcpy(tmp_employee.phone_num, "");
}


// 初始化
void init() {
    clear_scr();
    hide_cursor();
    printf("\n  正在加载管理系统...");
    init_employees();
    clear_employee_data();
    main_menu();
}

// 释放并退出
void destroy() {
    show_cursor();
    clear_scr();
    exit(0);
}

// 初始化临时职员数据
void init_temp_employees() {
    for (int i = 0; i < 1000; ++i) {
        strcpy(t_employees[i].id, "");
        strcpy(t_employees[i].name, "");
        strcpy(t_employees[i].sex, "");
        t_employees[i].age = -1;
        strcpy(t_employees[i].degree, "");
        t_employees[i].salary = 0.0;
        strcpy(t_employees[i].department, "");
        strcpy(t_employees[i].phone_num, "");
    }
}