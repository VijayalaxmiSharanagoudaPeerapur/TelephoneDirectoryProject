#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "FileFunctions.h"

/* Remove CR/LF from a string. */
static void removeNewline(char *text)
{
    text[strcspn(text, "\r\n")] = '\0';
}

/* Remove leading and trailing spaces/tabs. */
static void trim(char *text)
{
    char *start = text;
    char *end;

    while (*start == ' ' || *start == '\t')
        start++;

    if (start != text)
        memmove(text, start, strlen(start) + 1);

    end = text + strlen(text);

    while (end > text && (end[-1] == ' ' || end[-1] == '\t'))
        end--;

    *end = '\0';
}

void clearScreen(void)
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseScreen(void)
{
    int ch;

    printf("\n\t\tPress any key to continue");
    fflush(stdout);

    /* Wait for one key and then consume the rest of that input line. */
    ch = getchar();
    (void)ch;

    while (ch != '\n' && ch != EOF)
        ch = getchar();
}

int readLine(const char *prompt, char *buffer, int size)
{
    int ch;
    size_t len;

    if (size <= 0)
        return 0;

    printf("%s", prompt);
    fflush(stdout);

    buffer[0] = '\0';

    if (fgets(buffer, size, stdin) == NULL)
    {
        buffer[0] = '\0';
        return 0;
    }

    len = strlen(buffer);

    if (len > 0 && buffer[len - 1] == '\n')
    {
        buffer[len - 1] = '\0';
        if (len > 1 && buffer[len - 2] == '\r')
            buffer[len - 2] = '\0';
    }
    else
    {
        /*
         * The buffer is full.  The input may still be valid when the
         * user entered exactly size-1 characters and the next character
         * is the line ending.  Otherwise the input is too long.
         */
        ch = getchar();

        if (ch == '\n')
        {
            trim(buffer);
            return 1;
        }

        if (ch == '\r')
        {
            ch = getchar();
            if (ch == '\n' || ch == EOF)
            {
                trim(buffer);
                return 1;
            }
        }

        while (ch != '\n' && ch != EOF)
            ch = getchar();

        trim(buffer);
        return 0;
    }

    trim(buffer);
    return 1;
}

int isValidText(const char *text, int maxLength)
{
    if (text == NULL || text[0] == '\0')
        return 0;

    if ((int)strlen(text) > maxLength)
        return 0;

    return 1;
}

int isFourDigitCode(const char *text)
{
    int i;

    if (text == NULL || strlen(text) != 4)
        return 0;

    for (i = 0; i < 4; i++)
    {
        if (!isdigit((unsigned char)text[i]))
            return 0;
    }

    return 1;
}

int equalsIgnoreCase(const char *a, const char *b)
{
    if (a == NULL || b == NULL)
        return 0;

    while (*a != '\0' && *b != '\0')
    {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b))
            return 0;
        a++;
        b++;
    }

    return *a == '\0' && *b == '\0';
}

int departmentExists(const char *deptCode)
{
    FILE *fp;
    char line[LINE_SIZE];
    char code[DEPT_CODE_SIZE];
    char name[DEPT_NAME_SIZE];

    fp = fopen(DEPT_FILE, "r");
    if (fp == NULL)
        return 0;

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        removeNewline(line);

        if (sscanf(line, "%4[^|]|%31[^\r\n]", code, name) == 2)
        {
            if (strcmp(code, deptCode) == 0)
            {
                fclose(fp);
                return 1;
            }
        }
    }

    fclose(fp);
    return 0;
}

int getDepartmentName(const char *deptCode, char *deptName)
{
    FILE *fp;
    char line[LINE_SIZE];
    char code[DEPT_CODE_SIZE];
    char name[DEPT_NAME_SIZE];

    if (deptName == NULL)
        return 0;

    deptName[0] = '\0';

    fp = fopen(DEPT_FILE, "r");
    if (fp == NULL)
        return 0;

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        removeNewline(line);

        if (sscanf(line, "%4[^|]|%31[^\r\n]", code, name) == 2)
        {
            if (strcmp(code, deptCode) == 0)
            {
                strcpy(deptName, name);
                fclose(fp);
                return 1;
            }
        }
    }

    fclose(fp);
    return 0;
}

int departmentNameExists(const char *deptName)
{
    FILE *fp;
    char line[LINE_SIZE];
    char code[DEPT_CODE_SIZE];
    char name[DEPT_NAME_SIZE];

    fp = fopen(DEPT_FILE, "r");
    if (fp == NULL)
        return 0;

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        removeNewline(line);

        if (sscanf(line, "%4[^|]|%31[^\r\n]", code, name) == 2)
        {
            if (equalsIgnoreCase(name, deptName))
            {
                fclose(fp);
                return 1;
            }
        }
    }

    fclose(fp);
    return 0;
}

int getNextDepartmentCode(char *nextCode)
{
    FILE *fp;
    char line[LINE_SIZE];
    char code[DEPT_CODE_SIZE];
    char name[DEPT_NAME_SIZE];
    int maxCode = 999;
    int value;

    fp = fopen(DEPT_FILE, "r");

    if (fp != NULL)
    {
        while (fgets(line, sizeof(line), fp) != NULL)
        {
            removeNewline(line);

            if (sscanf(line, "%4[^|]|%31[^\r\n]", code, name) == 2)
            {
                if (isFourDigitCode(code))
                {
                    value = atoi(code);
                    if (value > maxCode)
                        maxCode = value;
                }
            }
        }
        fclose(fp);
    }

    if (maxCode >= 9999)
        return 0;

    sprintf(nextCode, "%04d", maxCode + 1);
    return 1;
}

int employeeExists(const char *empId)
{
    FILE *fp;
    char line[LINE_SIZE];
    char id[EMP_ID_SIZE];

    fp = fopen(EMP_FILE, "r");
    if (fp == NULL)
        return 0;

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        removeNewline(line);

        if (sscanf(line, "%4[^|]", id) == 1)
        {
            if (strcmp(id, empId) == 0)
            {
                fclose(fp);
                return 1;
            }
        }
    }

    fclose(fp);
    return 0;
}

int getEmployeeDetails(const char *empId,
                       char *empName,
                       char *deptCode,
                       char *deptName,
                       char *location,
                       char *phoneList)
{
    FILE *fp;
    char line[LINE_SIZE];
    char id[EMP_ID_SIZE];

    if (empName) empName[0] = '\0';
    if (deptCode) deptCode[0] = '\0';
    if (deptName) deptName[0] = '\0';
    if (location) location[0] = '\0';
    if (phoneList) phoneList[0] = '\0';

    fp = fopen(EMP_FILE, "r");
    if (fp == NULL)
        return 0;

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        removeNewline(line);

        if (sscanf(line,
                   "%4[^|]|%25[^|]|%4[^|]|%31[^|]|%5[^|]|%255[^\r\n]",
                   id, empName, deptCode, deptName, location, phoneList) == 6)
        {
            if (strcmp(id, empId) == 0)
            {
                fclose(fp);
                return 1;
            }
        }
    }

    fclose(fp);
    return 0;
}

int getNextEmployeeId(char *nextId)
{
    FILE *fp;
    char line[LINE_SIZE];
    char id[EMP_ID_SIZE];
    int maxId = 999;
    int value;

    fp = fopen(EMP_FILE, "r");

    if (fp != NULL)
    {
        while (fgets(line, sizeof(line), fp) != NULL)
        {
            removeNewline(line);

            if (sscanf(line, "%4[^|]", id) == 1)
            {
                if (isFourDigitCode(id))
                {
                    value = atoi(id);
                    if (value > maxId)
                        maxId = value;
                }
            }
        }
        fclose(fp);
    }

    if (maxId >= 9999)
        return 0;

    sprintf(nextId, "%04d", maxId + 1);
    return 1;
}

int telephoneExists(const char *telephone)
{
    FILE *fp;
    char line[LINE_SIZE];
    char id[EMP_ID_SIZE];
    char name[EMP_NAME_SIZE];
    char code[DEPT_CODE_SIZE];
    char dept[DEPT_NAME_SIZE];
    char loc[LOCATION_SIZE];
    char phones[PHONE_LIST_SIZE];

    fp = fopen(EMP_FILE, "r");
    if (fp == NULL)
        return 0;

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        removeNewline(line);

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
                    if (strcmp(number, telephone) == 0)
                    {
                        fclose(fp);
                        return 1;
                    }
                    number = strtok(NULL, ",");
                }
            }
        }
    }

    fclose(fp);
    return 0;
}

int getNextTelephoneNumber(const char *deptCode, char *telephone)
{
    FILE *fp;
    char line[LINE_SIZE];
    char id[EMP_ID_SIZE];
    char name[EMP_NAME_SIZE];
    char code[DEPT_CODE_SIZE];
    char dept[DEPT_NAME_SIZE];
    char loc[LOCATION_SIZE];
    char phones[PHONE_LIST_SIZE];
    int maxNumber = 0;

    fp = fopen(EMP_FILE, "r");
    if (fp != NULL)
    {
        while (fgets(line, sizeof(line), fp) != NULL)
        {
            removeNewline(line);

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
                        if (strncmp(number, deptCode, 4) == 0 &&
                            strlen(number) == 7)
                        {
                            int serial = atoi(number + 4);
                            if (serial > maxNumber)
                                maxNumber = serial;
                        }

                        number = strtok(NULL, ",");
                    }
                }
            }
        }
        fclose(fp);
    }

    if (maxNumber >= 999)
        return 0;

    sprintf(telephone, "%s%03d", deptCode, maxNumber + 1);
    return 1;
}

int addTelephoneToEmployee(const char *empId, const char *telephone)
{
    FILE *source;
    FILE *temp;
    char line[LINE_SIZE];
    int updated = 0;

    source = fopen(EMP_FILE, "r");
    if (source == NULL)
        return 0;

    temp = fopen(TEMP_FILE, "w");
    if (temp == NULL)
    {
        fclose(source);
        return 0;
    }

    while (fgets(line, sizeof(line), source) != NULL)
    {
        char id[EMP_ID_SIZE];
        char name[EMP_NAME_SIZE];
        char code[DEPT_CODE_SIZE];
        char dept[DEPT_NAME_SIZE];
        char loc[LOCATION_SIZE];
        char phones[PHONE_LIST_SIZE];

        removeNewline(line);

        if (sscanf(line,
                   "%4[^|]|%25[^|]|%4[^|]|%31[^|]|%5[^|]|%255[^\r\n]",
                   id, name, code, dept, loc, phones) == 6)
        {
            if (!updated && strcmp(id, empId) == 0)
            {
                if (strcmp(phones, "-") == 0 || phones[0] == '\0')
                {
                    fprintf(temp, "%s|%s|%s|%s|%s|%s\n",
                            id, name, code, dept, loc, telephone);
                }
                else
                {
                    fprintf(temp, "%s|%s|%s|%s|%s|%s,%s\n",
                            id, name, code, dept, loc, phones, telephone);
                }

                updated = 1;
            }
            else
            {
                fprintf(temp, "%s\n", line);
            }
        }
        else
        {
            fprintf(temp, "%s\n", line);
        }
    }

    fclose(source);
    fclose(temp);

    if (!updated)
    {
        remove(TEMP_FILE);
        return 0;
    }

    if (remove(EMP_FILE) != 0)
    {
        remove(TEMP_FILE);
        return 0;
    }

    if (rename(TEMP_FILE, EMP_FILE) != 0)
    {
        return 0;
    }

    return 1;
}

void printDepartmentHeader(void)
{
    printf("\n\t\tDepartment Code\tDepartment Name\n");
    printf("\t\t-------------------------------------\n");
}

void printEmployeeHeader(void)
{
    printf("\n\t\tEmployee Id\tEmployee Name\tDepartment Code\tDepartment Name\tLocation\tTelephone\n");
    printf("\t\t-----------------------------------------------------------------------------------------------\n");
}
