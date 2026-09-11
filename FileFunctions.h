#ifndef FILEFUNCTIONS_H
#define FILEFUNCTIONS_H

#define DEPT_FILE "dept.txt"
#define EMP_FILE  "emp.txt"
#define TEMP_FILE "emp.tmp"

/* Size includes the terminating '\0'. */
#define DEPT_CODE_SIZE 5       /* 4 characters */
#define DEPT_NAME_SIZE 32      /* storage for department names; new names are validated to 15 */
#define EMP_ID_SIZE 5          /* 4 characters */
#define EMP_NAME_SIZE 26      /* maximum 25 characters */
#define LOCATION_SIZE 6        /* maximum 5 characters */
#define PHONE_SIZE 8           /* 7 digits */
#define PHONE_LIST_SIZE 256
#define LINE_SIZE 512

void clearScreen(void);
void pauseScreen(void);
int readLine(const char *prompt, char *buffer, int size);

int isValidText(const char *text, int maxLength);
int isFourDigitCode(const char *text);
int equalsIgnoreCase(const char *a, const char *b);

int departmentExists(const char *deptCode);
int getDepartmentName(const char *deptCode, char *deptName);
int departmentNameExists(const char *deptName);
int getNextDepartmentCode(char *nextCode);

int employeeExists(const char *empId);
int getEmployeeDetails(const char *empId,
                       char *empName,
                       char *deptCode,
                       char *deptName,
                       char *location,
                       char *phoneList);
int getNextEmployeeId(char *nextId);

int telephoneExists(const char *telephone);
int getNextTelephoneNumber(const char *deptCode, char *telephone);
int addTelephoneToEmployee(const char *empId, const char *telephone);

void printDepartmentHeader(void);
void printEmployeeHeader(void);

#endif
