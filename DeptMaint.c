#include <stdio.h>
#include <stdlib.h>
#include "FileFunctions.h"

static void showAddDepartmentScreen(const char *name)
{
    clearScreen();

    printf("\n\t\tTelephone Directory Maintenance System\n");
    printf("\t\t=======================================\n\n");
    printf("\t\t\tAdd a Department\n");
    printf("\t\t\t================\n\n");

    if (name != NULL)
        printf("\t\tEnter Department Name  : %s\n", name);
}

void addDepartment(void)
{
    FILE *fp;
    char deptName[DEPT_NAME_SIZE];
    char deptCode[DEPT_CODE_SIZE];

    showAddDepartmentScreen(NULL);

    while (1)
    {
        if (!readLine("\t\tEnter Department Name  : ",
                       deptName, sizeof(deptName)) ||
            !isValidText(deptName, 15))
        {
            printf("\n\t\tError: Department name is mandatory and must not exceed 15 characters.");
            pauseScreen();
            showAddDepartmentScreen(NULL);
            continue;
        }

        if (departmentNameExists(deptName))
        {
            printf("\n\t\tError: Department name already exists.");
            pauseScreen();
            showAddDepartmentScreen(NULL);
            continue;
        }

        break;
    }

    if (!getNextDepartmentCode(deptCode))
    {
        printf("\n\t\tError: Department code limit reached.");
        pauseScreen();
        return;
    }

    fp = fopen(DEPT_FILE, "a");

    if (fp == NULL)
    {
        printf("\n\t\tError: Unable to open department file.");
        pauseScreen();
        return;
    }

    fprintf(fp, "%s|%s\n", deptCode, deptName);
    fclose(fp);

    printf("\n\t\tDepartment Code        : %s", deptCode);
    pauseScreen();
}

void viewDepartments(void)
{
    FILE *fp;
    char line[LINE_SIZE];
    char code[DEPT_CODE_SIZE];
    char name[DEPT_NAME_SIZE];
    int found = 0;

    clearScreen();

    printf("\n\t\tTelephone Directory Maintenance System\n");
    printf("\t\t=======================================\n\n");
    printf("\t\t\tDepartment Details\n");
    printf("\t\t\t==================\n");

    fp = fopen(DEPT_FILE, "r");

    if (fp == NULL)
    {
        printf("\n\t\tError: Department file could not be opened.");
        pauseScreen();
        return;
    }

    printDepartmentHeader();

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (sscanf(line, "%4[^|]|%31[^\r\n]", code, name) == 2)
        {
            printf("\t\t%-16s%-20s\n", code, name);
            found = 1;
        }
    }

    fclose(fp);

    if (!found)
        printf("\n\t\tNo department records found.");

    pauseScreen();
}

void departmentMenu(void)
{
    int choice;
    char input[10];

    do
    {
        clearScreen();

        printf("\n\t\tTelephone Directory Maintenance System\n");
        printf("\t\t=======================================\n\n");
        printf("\t\t\tDepartment Maintenance Menu\n");
        printf("\t\t\t===========================\n\n");
        printf("\t\t1. Add Department\n");
        printf("\t\t2. Print Department Details\n");
        printf("\t\t3. Goto Main Menu\n\n");

        readLine("\t\tEnter your choice : ", input, sizeof(input));
        choice = atoi(input);

        switch (choice)
        {
            case 1:
                addDepartment();
                break;
            case 2:
                viewDepartments();
                break;
            case 3:
                break;
            default:
                printf("\n\t\tInvalid choice.");
                pauseScreen();
        }
    } while (choice != 3);
}
