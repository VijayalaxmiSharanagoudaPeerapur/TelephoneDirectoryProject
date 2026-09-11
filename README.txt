☎️ Telephone Directory Maintenance System

A menu-driven Telephone Directory Maintenance System developed in C using file handling to maintain department, employee, and telephone directory information.

📌 Features

🔐 Employee ID based login
🏢 Department Code Maintenance
Add Department
View All Departments
👨‍💼 Employee Master Maintenance
Add Employee
View Employee
☎️ Telephone Directory Maintenance
Add Telephone Number
🔎 Telephone Number Enquiry
Enquiry by Employee Name
Enquiry by Telephone Number
💾 Persistent data storage using text files
✅ Input validation and error handling
🔤 Case-insensitive validations
🛠️ Technologies Used
Language: C
Storage: Text files
Concepts: File Handling, Functions, Structures, String Handling, Searching and Validation
Compiler: GCC
IDE: Visual Studio Code
📂 Project Structure
TelephoneDirectoryProject/
│
├── FileFunctions.h
├── FileFunctions.c
├── DeptMaint.c
├── EmployeeMaint.c
├── TelDir.c
├── dept.txt
├── emp.txt
├── README.md
└── .gitignore
💾 Data Files
dept.txt

Stores department codes and department names.

Example:

1000|Computer Science
1001|Electronics
emp.txt

Stores employee details and allocated telephone numbers.

Example:

1000|Vijayalaxmi|1000|Computer Science|DWD|1000001,1000002
1001|Rahul|1001|Electronics|HUB|1001001
1002|Anita|1000|Computer Science|BLR|-
🔐 Login

The Employee ID is used as the User ID.

If the entered Employee ID belongs to an employee, the Main Menu is displayed.
If another/invalid Employee ID is entered, the system displays:
Login Denied

and terminates the application.

🖥️ Screen Design Rules

The system follows the specified screen-design convention:

......... → User input
________ → System-generated/displayed value

Error handling:

Error messages are displayed on the screen.
The system waits for a key press.
After the key press, the error message is cleared.
Validations are performed case-insensitively wherever applicable.
▶️ Compilation

Open the project folder in the terminal and run:

gcc FileFunctions.c DeptMaint.c EmployeeMaint.c TelDir.c -o TelephoneDirectoryProject
▶️ Run

On Windows PowerShell:

.\TelephoneDirectoryProject.exe

🎯 Objective

The objective of this project is to implement a Telephone Directory Maintenance System using C programming and file handling, while applying modular programming, record management, searching, validation, and menu-driven programming concepts.

👩‍💻 Author

Vijayalaxmi Sharanagouda Peerapur

Computer Science Engineering Student

📄 Project

Programming Fundamentals Project – Telephone Directory Maintenance System
