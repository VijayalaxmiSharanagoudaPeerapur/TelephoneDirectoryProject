#include <stdio.h>
#include <stdlib.h>
#include "FileFunctions.h"

void viewDepartmentsForEmployee(void)
{
    FILE *fp;
    char line[LINE_SIZE];
    char code[DEPT_CODE_SIZE];
    char name[DEPT_NAME_SIZE];
    int found = 0;

    fp = fopen(DEPT_FILE, "r");

    if (fp == NULL)
    {
        printf("\n\t\tError: Department file could not be opened.\n");
        return;
    }

    printf("\n\t\tExisting Departments\n");
    printf("\t\t---------------------\n");
    printf("\t\tCode\tDepartment Name\n");
    printf("\t\t--------------------------------\n");

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (sscanf(line, "%4[^|]|%31[^\r\n]", code, name) == 2)
        {
            printf("\t\t%s\t%s\n", code, name);
            found = 1;
        }
    }

    fclose(fp);

    if (!found)
        printf("\t\tNo departments available.\n");
}

void addEmployee(void)
{
    FILE *fp;
    char empName[EMP_NAME_SIZE];
    char empId[EMP_ID_SIZE];
    char deptCode[DEPT_CODE_SIZE];
    char deptName[DEPT_NAME_SIZE];
    char location[LOCATION_SIZE];

    clearScreen();

    printf("\n\t\tTelephone Directory Maintenance System\n");
    printf("\t\t=======================================\n\n");
    printf("\t\t\tAdd an Employee\n");
    printf("\t\t\t===============\n\n");

    while (1)
    {
        if (!readLine("\t\tEnter Employee Name    : ",
                       empName, sizeof(empName)) ||
            !isValidText(empName, 25))
        {
            printf("\n\t\tError: Employee name is mandatory and must not exceed 25 characters.");
            pauseScreen();
            clearScreen();

            printf("\n\t\tTelephone Directory Maintenance System\n");
            printf("\t\t=======================================\n\n");
            printf("\t\t\tAdd an Employee\n");
            printf("\t\t\t===============\n\n");
            continue;
        }
        break;
    }

    if (!getNextEmployeeId(empId))
    {
        printf("\n\t\tError: Employee Id limit reached.");
        pauseScreen();
        return;
    }

    printf("\t\tEmployee Id            : %s\n", empId);

    while (1)
    {
        viewDepartmentsForEmployee();

        if (!readLine("\n\t\tEnter Department Code  : ",
                       deptCode, sizeof(deptCode)) ||
            !isFourDigitCode(deptCode) || !departmentExists(deptCode))
        {
            printf("\n\t\tError: Department code is invalid or does not exist.");
            pauseScreen();
            clearScreen();

            printf("\n\t\tTelephone Directory Maintenance System\n");
            printf("\t\t=======================================\n\n");
            printf("\t\t\tAdd an Employee\n");
            printf("\t\t\t===============\n\n");
            printf("\t\tEnter Employee Name    : %s\n", empName);
            printf("\t\tEmployee Id            : %s\n", empId);
            continue;
        }

        break;
    }

    if (!getDepartmentName(deptCode, deptName))
    {
        printf("\n\t\tError: Department details could not be found.");
        pauseScreen();
        return;
    }

    printf("\t\tDepartment Name        : %s\n", deptName);

    while (1)
    {
        if (!readLine("\t\tEnter Location          : ",
                       location, sizeof(location)) ||
            !isValidText(location, 5))
        {
            printf("\n\t\tError: Location is mandatory and must not exceed 5 characters.");
            pauseScreen();
            clearScreen();

            printf("\n\t\tTelephone Directory Maintenance System\n");
            printf("\t\t=======================================\n\n");
            printf("\t\t\tAdd an Employee\n");
            printf("\t\t\t===============\n\n");
            printf("\t\tEnter Employee Name    : %s\n", empName);
            printf("\t\tEmployee Id            : %s\n", empId);
            printf("\t\tEnter Department Code  : %s\n", deptCode);
            printf("\t\tDepartment Name        : %s\n", deptName);
            continue;
        }

        break;
    }

    fp = fopen(EMP_FILE, "a");

    if (fp == NULL)
    {
        printf("\n\t\tError: Unable to open employee file.");
        pauseScreen();
        return;
    }

    fprintf(fp, "%s|%s|%s|%s|%s|-\n",
            empId, empName, deptCode, deptName, location);

    fclose(fp);

    pauseScreen();
}

void viewEmployees(void)
{
    FILE *fp;
    char line[LINE_SIZE];
    char id[EMP_ID_SIZE];
    char name[EMP_NAME_SIZE];
    char code[DEPT_CODE_SIZE];
    char dept[DEPT_NAME_SIZE];
    char loc[LOCATION_SIZE];
    char phones[PHONE_LIST_SIZE];
    int found = 0;

    clearScreen();

    printf("\n\t\tTelephone Directory Maintenance System\n");
    printf("\t\t=======================================\n\n");
    printf("\t\t\tEmployee Details\n");
    printf("\t\t\t================\n");

    fp = fopen(EMP_FILE, "r");

    if (fp == NULL)
    {
        printf("\n\t\tError: Employee file could not be opened.");
        pauseScreen();
        return;
    }

    printEmployeeHeader();

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (sscanf(line,
                   "%4[^|]|%25[^|]|%4[^|]|%31[^|]|%5[^|]|%255[^\r\n]",
                   id, name, code, dept, loc, phones) == 6)
        {
            printf("\t\t%-12s%-18s%-16s%-20s%-10s%s\n",
                   id, name, code, dept, loc, phones);
            found = 1;
        }
    }

    fclose(fp);

    if (!found)
        printf("\n\t\tNo employee records found.");

    pauseScreen();
}

void employeeMenu(void)
{
    int choice;
    char input[10];

    do
    {
        clearScreen();

        printf("\n\t\tTelephone Directory Maintenance System\n");
        printf("\t\t=======================================\n\n");
        printf("\t\t\tEmployee Maintenance Menu\n");
        printf("\t\t\t=========================\n\n");
        printf("\t\t1. Add Employee\n");
        printf("\t\t2. Print Employee Details\n");
        printf("\t\t3. Goto Main Menu\n\n");

        readLine("\t\tEnter your choice : ", input, sizeof(input));
        choice = atoi(input);

        switch (choice)
        {
            case 1:
                addEmployee();
                break;
            case 2:
                viewEmployees();
                break;
            case 3:
                break;
            default:
                printf("\n\t\tInvalid choice.");
                pauseScreen();
        }
    } while (choice != 3);
}
