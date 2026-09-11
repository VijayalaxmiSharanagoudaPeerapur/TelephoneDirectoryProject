TELEPHONE DIRECTORY MAINTENANCE SYSTEM
======================================

Files:
    FileFunctions.h
    FileFunctions.c
    DeptMaint.c
    EmployeeMaint.c
    TelDir.c
    dept.txt
    emp.txt

Compile with GCC:
    gcc TelDir.c DeptMaint.c EmployeeMaint.c FileFunctions.c -o Telephone.exe

Run on Windows:
    Telephone.exe

IMPORTANT:
    Keep dept.txt and emp.txt in the same folder as Telephone.exe.

DATA FORMAT
-----------
dept.txt:
    DepartmentCode|DepartmentName

emp.txt:
    EmployeeId|EmployeeName|DepartmentCode|DepartmentName|Location|TelephoneList

Use '-' in the last field when no telephone number has been allocated.
Multiple telephone numbers are stored with commas.

TEST LOGIN
----------
The sample emp.txt contains employee IDs:
    1000
    1001
    1002

For example, login with:
    1000

TEST TELEPHONE ENQUIRY
----------------------
The sample data contains:
    1000001 -> Vijayalaxmi / DWD / Computer Science
    1000002 -> Vijayalaxmi / DWD / Computer Science
    1001001 -> Rahul / HUB / Electronics

The telephone enquiry screen is intentionally formatted to match
Figure 7-11 in the supplied project specification.
