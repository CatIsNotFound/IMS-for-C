
#include "Datas.h"

FILE *rfile, *wfile;
struct Company company;

// 初始化职工信息
void init_employees() {
    company.employee_count = 0;
    for (int i = 0; i < 1000; ++i) {
        strcpy(company.employees[i].id, "");
        strcpy(company.employees[i].name, "");
        strcpy(company.employees[i].sex, "");
        company.employees[i].age = -1;
        strcpy(company.employees[i].degree, "");
        company.employees[i].salary = 0.0;
        strcpy(company.employees[i].department, "");
        strcpy(company.employees[i].phone_num, "");
    }
}

// 寻找空的数据
int find_empty_space() {
    for (int i = 0; i < 1000; ++i) {
        if (!strcmp(company.employees[i].id, "")) {
            return i;
        }
    }
    return -1;
}

// 设置职工信息
void update_employee(struct Employee *new_employee, struct Employee temp_employee) {
    strcpy(new_employee->id, temp_employee.id);
    strcpy(new_employee->name, temp_employee.name);
    strcpy(new_employee->sex, temp_employee.sex);
    new_employee->age = temp_employee.age;
    strcpy(new_employee->degree, temp_employee.degree);
    new_employee->salary = temp_employee.salary;
    strcpy(new_employee->department, temp_employee.department);
    strcpy(new_employee->phone_num, temp_employee.phone_num);
}

// 添加职工
void add_employee(struct Employee temp_employee) {
    int c = find_empty_space();
    if (c > -1) {
        company.employee_count++;
        update_employee(&company.employees[c], temp_employee);
    }
}

// 修改职工
void modify_employee(struct Employee employee) {
    for (int i = 0; i < 1000; ++i) {
        if (!strcmp(company.employees[i].id, employee.id)) {
            update_employee(&company.employees[i], employee);
            break;
        }
    }
}

// 通过编号删除职工
void del_employee_by_id(const char *id) {
    for (int i = 0; i < 1000; ++i) {
        if (!strcmp(company.employees[i].id, id)) {
            strcpy(company.employees[i].id, "");
            strcpy(company.employees[i].name, "");
            strcpy(company.employees[i].sex, "");
            company.employees[i].age = -1;
            strcpy(company.employees[i].degree, "");
            company.employees[i].salary = 0.0;
            strcpy(company.employees[i].department, "");
            strcpy(company.employees[i].phone_num, "");
            break;
        }
    }
}

struct Employee find_employee_by_id(const char *id) {
    struct Employee result = {"", "", "", -1, "", 0.0, "", ""};
    for (int i = 0; i < 1000; ++i) {
        if (!strcmp(company.employees[i].id, id)) {
            update_employee(&result, company.employees[i]);
            break;
        }
    }
    return result;
}

int find_all_employees_by_id(const char *id, struct Employee employees[1000]) {
    int k = 0;
    for (int i = 0; i < 1000; ++i) {
        if (!strcmp(company.employees[i].id, id)) {
            update_employee(&employees[k++], company.employees[i]);
        }
    }
    return k;
}

int find_employee_by_sex(const char *sex, struct Employee employees[1000]) {
    int k = 0;
    for (int i = 0; i < 1000; ++i) {
        if (!strcmp(company.employees[i].sex, sex)) {
            update_employee(&employees[k++], company.employees[i]);
        }
    }
    return k;
}

int find_employee_by_degree(const char *degree, struct Employee *employees) {
    int k = 0;
    for (int i = 0; i < 1000; ++i) {
        if (!strcmp(company.employees[i].degree, degree)) {
            update_employee(&employees[k++], company.employees[i]);
        }
    }
    return k;
}

int find_employee_by_department(const char *department, struct Employee *employees) {
    int k = 0;
    for (int i = 0; i < 1000; ++i) {
        if (!strcmp(company.employees[i].department, department)) {
            update_employee(&employees[k++], company.employees[i]);
        }
    }
    return k;
}

// 导入 CSV 表格
int import_csv(const char *file_name) {
    rfile = fopen(file_name, "r");
    if (rfile == NULL) {
        return 1;   // 读取失败
    }

    char buffer[1024] = {'\0'};
    while (fgets(buffer, sizeof(buffer), rfile) == NULL) {
        fclose(rfile);
        return 2;   // 读取失败
    }
    init_employees();
    char *temp;
    int c = 0;
    while (fgets(buffer, sizeof(buffer), rfile) != NULL) {
        temp = strtok(buffer, ",");
        if (temp != NULL) {
            strcpy(company.employees[c].id, temp);
        }
        temp = strtok(NULL, ",");
        if (temp != NULL) {
            strcpy(company.employees[c].name, temp);
        }
        temp = strtok(NULL, ",");
        if (temp != NULL) {
            strcpy(company.employees[c].sex, temp);
        }
        temp = strtok(NULL, ",");
        if (temp != NULL) {
            company.employees[c].age = atoi(temp);
        }
        temp = strtok(NULL, ",");
        if (temp != NULL) {
            strcpy(company.employees[c].degree, temp);
        }
        temp = strtok(NULL, ",");
        if (temp != NULL) {
            company.employees[c].salary = atof(temp);
        }
        temp = strtok(NULL, ",");
        if (temp != NULL) {
            strcpy(company.employees[c].department, temp);
        }
        temp = strtok(NULL, "\n");
        if (temp != NULL) {
            strcpy(company.employees[c].phone_num, temp);
        }
        c += 1;
    }
    company.employee_count = c;
    fclose(rfile);
    return 0;   // 正常读取
}

// 导出 CSV 表格
int export_csv(const char *file_name) {
    wfile = fopen(file_name, "w");
    if (wfile == NULL) {
        return 1;
    }

    fprintf(wfile, "职工编号,职工姓名,职工性别,职工年龄,个人学历,所得工资,所属部门,手机号码\n");
    struct Employee tmp_emp;
    for (int i = 0; i < 1000; ++i) {
        tmp_emp = company.employees[i];
        if (!strcmp(tmp_emp.id, "")) continue;
        fprintf(wfile, "%s,%s,%s,%d,%s,%.2lf,%s,%s\n",
                tmp_emp.id, tmp_emp.name, tmp_emp.sex, tmp_emp.age,
                tmp_emp.degree, tmp_emp.salary, tmp_emp.department, tmp_emp.phone_num);
    }
    fclose(wfile);
    return 0;
}