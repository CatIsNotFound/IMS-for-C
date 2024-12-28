
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <conio.h>

struct Files {
    int id[64];
    char name[64][255];
    int count;
};

struct Employee {
    char id[16];
    char name[64];
    char sex[4];
    int age;
    char degree[32];
    double salary;
    char department[64];
    char phone_num[11];
};

struct Company {
    struct Employee employees[1000];
    int employee_count;
};

void init_employees();
int find_empty_space();
void update_employee(struct Employee *new_employee, struct Employee temp_employee);
void add_employee(struct Employee employee);
void modify_employee(struct Employee employee);
void del_employee_by_id(const char *id);
struct Employee find_employee_by_id(const char *id);
int find_all_employees_by_id(const char *id, struct Employee *employees);
int find_employee_by_sex(const char *sex, struct Employee *employees);
int find_employee_by_degree(const char *degree, struct Employee *employees);
int find_employee_by_department(const char *department, struct Employee *employees);
int import_csv(const char *file_name);
int export_csv(const char *file_name);