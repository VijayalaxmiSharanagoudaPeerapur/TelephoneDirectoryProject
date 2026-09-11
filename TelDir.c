#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FileFunctions.h"

/*
 * The sample employee record supplied for this project uses 1000
 * as Vijayalaxmi's employee id.  The specification says the User ID
 * for login must be your employee id.
 */
#define LOGIN_EMPLOYEE_ID "1000"

void departmentMenu(void);
void employeeMenu(void);
void addEmployee(void);
void viewEmployees(void);
void addDepartment(void);
void viewDepartments(void);

void addTelephone(void);
void telephoneDirectoryMenu(void);
void telephoneEnquiryMenu(void);
void enquiryByName(void);
void enquiryByTelephone(void);

static int login(void)
{
    char userId[EMP_ID_SIZE];

    clearScreen();

    printf("\n\t\tTelephone Directory Maintenance System\n");
    printf("\t\t=======================================\n\n");
    printf("\t\t\tLogin\n");
    printf("\t\t\t=====\n\n");

    if (readLine("\t\tEnter User ID : ", userId, sizeof(userId)) &&
        strcmp(userId, LOGIN_EMPLOYEE_ID) == 0)
        return 1;

    printf("\n\t\tLogin Denied");
    pauseScreen();
    return 0;
}

void addTelephone(void)
{
    char empId[EMP_ID_SIZE];
    char empName[EMP_NAME_SIZE];
    char deptCode[DEPT_CODE_SIZE];
    char deptName[DEPT_NAME_SIZE];
    char location[LOCATION_SIZE];
    char phoneList[PHONE_LIST_SIZE];
    char telephone[PHONE_SIZE];

    clearScreen();

    printf("\n\t\tTelephone Directory Maintenance System\n");
    printf("\t\t=======================================\n\n");
    printf("\t\t\tAdd a Telephone Number\n");
    printf("\t\t\t======================\n\n");

    while (1)
    {
        if (!readLine("\t\tEnter Employee Id       : ",
                       empId, sizeof(empId)) ||
            !employeeExists(empId))
        {
            printf("\n\t\tError: Employee Id does not exist.");
            pauseScreen();

            clearScreen();
            printf("\n\t\tTelephone Directory Maintenance System\n");
            printf("\t\t=======================================\n\n");
            printf("\t\t\tAdd a Telephone Number\n");
            printf("\t\t\t======================\n\n");
            continue;
        }

        break;
    }

    if (!getEmployeeDetails(empId,
                            empName,
                            deptCode,
                            deptName,
                            location,
                            phoneList))
    {
        printf("\n\t\tError: Employee details could not be found.");
        pauseScreen();
        return;
    }

    printf("\n\t\tLocation                  : %s", location);
    printf("\n\t\tDepartment code           : %s", deptCode);

    if (!getNextTelephoneNumber(deptCode, telephone))
    {
        printf("\n\n\t\tError: Telephone number limit reached for this department.");
        pauseScreen();
        return;
    }

    printf("\n\t\tTelephone Number Allocated : %s", telephone);

    if (!addTelephoneToEmployee(empId, telephone))
    {
        printf("\n\n\t\tError: Unable to update employee file.");
        pauseScreen();
        
        return;
    }

    pauseScreen();
}

void telephoneDirectoryMenu(void)
{
    int choice;
    char input[10];

    do
    {
        clearScreen();

        printf("\n\t\tTelephone Directory Maintenance System\n");
        printf("\t\t=======================================\n\n");
        printf("\t\t\tTelephone Directory Maintenance Menu\n");
        printf("\t\t\t====================================\n\n");
        printf("\t\t1. Add Telephone Number\n");
        printf("\t\t2. Goto Main Menu\n\n");

        readLine("\t\tEnter your choice : ", input, sizeof(input));
        choice = atoi(input);

        switch (choice)
        {
            case 1:
                addTelephone();
                break;
            case 2:
                break;
            default:
                printf("\n\t\tInvalid choice.");
                pauseScreen();
        }
    } while (choice != 2);
}

void enquiryByName(void)
{
    FILE *fp;
    char searchName[EMP_NAME_SIZE];
    char line[LINE_SIZE];
    int found = 0;

    clearScreen();

    printf("\n\t\tTelephone Directory Maintenance System\n");
    printf("\t\t=======================================\n\n");
    printf("\t\t\tTelephone Number Enquiry by Name\n");
    printf("\t\t\t================================\n\n");

    if (!readLine("\t\tEnter Employee Name     : ",
                   searchName, sizeof(searchName)) ||
        !isValidText(searchName, 25))
    {
        printf("\n\t\tError: Employee name is mandatory and must not exceed 25 characters.");
        pauseScreen();
        return;
    }

    fp = fopen(EMP_FILE, "r");

    if (fp == NULL)
    {
        printf("\n\t\tError: Employee file could not be opened.");
        pauseScreen();
        return;
    }

    printf("\n\t\t%-16s%-16s%-24s%s\n",
           "Name", "Location", "DeptName", "TelNumber");
    printf("\t\t---------------------------------------------------------------------\n");

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        char id[EMP_ID_SIZE];
        char name[EMP_NAME_SIZE];
        char code[DEPT_CODE_SIZE];
        char dept[DEPT_NAME_SIZE];
        char loc[LOCATION_SIZE];
        char phones[PHONE_LIST_SIZE];

        if (sscanf(line,
                   "%4[^|]|%25[^|]|%4[^|]|%31[^|]|%5[^|]|%255[^\r\n]",
                   id, name, code, dept, loc, phones) == 6)
        {
            if (equalsIgnoreCase(name, searchName))
            {
                found = 1;

                if (strcmp(phones, "-") == 0 || phones[0] == '\0')
                {
                    printf("\t\t%-16s%-16s%-24s%s\n",
                           name, loc, dept, "Not allocated");
                }
                else
                {
                    char copy[PHONE_LIST_SIZE];
                    char *number;

                    strcpy(copy, phones);
                    number = strtok(copy, ",");

                    while (number != NULL)
                    {
                        printf("\t\t%-16s%-16s%-24s%s\n",
                               name, loc, dept, number);
                        number = strtok(NULL, ",");
                    }
                }
            }
        }
    }

    fclose(fp);

    if (!found)
        printf("\n\t\tError: Employee name does not exist.");

    pauseScreen();
}

void enquiryByTelephone(void)
{
    FILE *fp;
    char searchPhone[PHONE_SIZE];
    char line[LINE_SIZE];
    int found = 0;

    char employeeName[EMP_NAME_SIZE];
    char location[LOCATION_SIZE];
    char departmentName[DEPT_NAME_SIZE];

    clearScreen();

    printf("\n\t\tTelephone Directory Maintenance System\n");
    printf("\t\t=======================================\n\n");
    printf("\t\t\tTelephone Number Enquiry\n");
    printf("\t\t\t========================\n\n");

    if (!readLine("\t\tEnter Telephone Number   : ",
                   searchPhone, sizeof(searchPhone)) ||
        searchPhone[0] == '\0')
    {
        printf("\n\t\tError: Telephone Number is mandatory.");
        pauseScreen();
        return;
    }

    fp = fopen(EMP_FILE, "r");

    if (fp == NULL)
    {
        printf("\n\t\tError: Employee file could not be opened.");
        pauseScreen();
        return;
    }

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        char id[EMP_ID_SIZE];
        char name[EMP_NAME_SIZE];
        char code[DEPT_CODE_SIZE];
        char dept[DEPT_NAME_SIZE];
        char loc[LOCATION_SIZE];
        char phones[PHONE_LIST_SIZE];

        if (sscanf(line,
                   "%4[^|]|%25[^|]|%4[^|]|%31[^|]|%5[^|]|%255[^\r\n]",
                   id, name, code, dept, loc, phones) == 6)
        {
            if (strcmp(phones, "-") != 0)
            {
                char copy[PHONE_LIST_SIZE];
                char *number;

                strcpy(copy, phones);
                number = strtok(copy, ",");

                while (number != NULL)
                {
                    if (strcmp(number, searchPhone) == 0)
                    {
                        snprintf(employeeName, sizeof(employeeName), "%s", name);
                        snprintf(location, sizeof(location), "%s", loc);
                        snprintf(departmentName, sizeof(departmentName), "%s", dept);
                        found = 1;
                        break;
                    }
                    number = strtok(NULL, ",");
                }
            }
        }

        if (found)
            break;
    }

    fclose(fp);

    if (found)
    {
        printf("\t\tEmployee Name             : %s\n", employeeName);
        printf("\t\tLocation                  : %s\n", location);
        printf("\t\tDepartment Name           : %s\n", departmentName);
    }
    else
    {
        printf("\n\t\tError: Telephone Number does not exist.");
    }

    pauseScreen();
}

void telephoneEnquiryMenu(void)
{
    int choice;
    char input[10];

    do
    {
        clearScreen();

        printf("\n\t\tTelephone Directory Maintenance System\n");
        printf("\t\t=======================================\n\n");
        printf("\t\t\tTelephone Enquiry Menu\n");
        printf("\t\t\t======================\n\n");
        printf("\t\t1. Enquiry on Employee Name\n");
        printf("\t\t2. Enquiry on Telephone Number\n");
        printf("\t\t3. Goto Main Menu\n\n");

        readLine("\t\tEnter your choice : ", input, sizeof(input));
        choice = atoi(input);

        switch (choice)
        {
            case 1:
                enquiryByName();
                break;
            case 2:
                enquiryByTelephone();
                break;
            case 3:
                break;
            default:
                printf("\n\t\tInvalid choice.");
                pauseScreen();
        }
    } while (choice != 3);
}

void mainMenu(void)
{
    int choice;
    char input[10];

    do
    {
        clearScreen();

        printf("\n\t\tTelephone Directory Maintenance System\n");
        printf("\t\t=======================================\n\n");
        printf("\t\t\t\tMain Menu\n");
        printf("\t\t\t\t=========\n\n");
        printf("\t\t1. Department Maintenance\n");
        printf("\t\t2. Employee Maintenance\n");
        printf("\t\t3. Telephone Directory Maintenance\n");
        printf("\t\t4. Telephone Enquiry\n");
        printf("\t\t5. Exit\n\n");

        readLine("\t\tEnter your choice : ", input, sizeof(input));
        choice = atoi(input);

        switch (choice)
        {
            case 1:
                departmentMenu();
                break;
            case 2:
                employeeMenu();
                break;
            case 3:
                telephoneDirectoryMenu();
                break;
            case 4:
                telephoneEnquiryMenu();
                break;
            case 5:
                break;
            default:
                printf("\n\t\tInvalid choice.");
                pauseScreen();
        }
    } while (choice != 5);
}

int main(void)
{
    if (login())
        mainMenu();

    return 0;
}
