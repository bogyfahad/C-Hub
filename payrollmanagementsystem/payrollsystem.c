#include <stdio.h>
#include <string.h>
#include <stdlib.h>

 

/*constant to limit the number of employees in the system*/
#define MAX_EMPLOYEES 100

/*structure to hold the address information*/
typedef struct{
char city[100];
char district[100];
char parish [100];
char village[100];

}Address;

/*enum to specify the type of ID the user uses*/
typedef enum {
NATIONAL_ID,
PASSPORT,
REFUGEE_CARD
}IDtype;

/*union to hold on type of id*/
typedef union{
char nationalid[14];
char passport[9];
char refugeeCard[11];

}Identification;


typedef struct {
int id,tin_number,nssf_number,bank_account_no;
char fistname[100],lastname[100];
Address address; //this is a nested struct to cater for the address
float gross_pay,net_pay,housing_allowance,medical_allowance,transport_allowance,payee_tax,nssf_tax;
}Employee;


/*function to calculate gross pay which is the total amount of the employee with all his allowances and tax deductions*/
float calculategrosspay(const Employee *emp){
  return emp->net_pay + emp->housing_allowance + emp->medical_allowance + emp->transport_allowance + emp->nssf_tax + emp->payee_tax ;
}

/*functions to calculate  nssf and payee tax */
float calculateNSSF(float basicSalary) {
    return basicSalary * 0.05;  // Assuming NSSF is 5% of the basic salary
}

float calculatePayeeTax(float grossPay) {
    if (grossPay > 10000) {
        return grossPay * 0.10;  // Assuming PAYEE tax is 10% for simplicity
    } else {
        return 0;
    }
}

/*function to calculate netpay*/
float calculateNetPay(const Employee *emp) {
    // Calculate NSSF and PAYEE taxes
    float nssf = calculateNSSF(emp->gross_pay);
    float payee = calculatePayeeTax(emp->gross_pay);

    // Deduct taxes from the gross pay
    return emp->gross_pay - (nssf + payee);
}

/*function to print the salary slip for employees*/
void printSalarySlip(const Employee *emp) {
    float grossPay = calculategrosspay(emp);
    float netPay = calculateNetPay(emp);
    printf("\nSalary Slip for %s %s\n", emp->fistname, emp->lastname);
    printf("-------------------------------------------------\n");
    printf("Employee ID: %d\n", emp->id);
    printf("Basic Salary: %.2f\n", emp->gross_pay);  // Assuming gross_pay holds the basic salary
    printf("Housing Allowance: %.2f\n", emp->housing_allowance);
    printf("Medical Allowance: %.2f\n", emp->medical_allowance);
    printf("Transport Allowance: %.2f\n", emp->transport_allowance);
    printf("-------------------------------------------------\n");
    printf("Gross Salary: %.2f\n", grossPay);
    printf("Deductions:\n");
    printf("  NSSF: %.2f\n", calculateNSSF(emp->gross_pay));
    printf("  PAYEE: %.2f\n", calculatePayeeTax(emp->gross_pay));
    printf("-------------------------------------------------\n");
    printf("Net Salary: %.2f\n", netPay);
}

/*function to save employee information*/
void saveEmployeeData(Employee employees[], int employeeCount) {
    FILE *file = fopen("employee_data.bin", "wb");
    if (file == NULL) {
        printf("Error opening file for saving.\n");
        return;
    }

    // Write the number of employees first
    fwrite(&employeeCount, sizeof(int), 1, file);

    // Write employee data
    for (int i = 0; i < employeeCount; i++) {
        fwrite(&employees[i], sizeof(Employee), 1, file);
    }

    fclose(file);
    printf("Employee data saved successfully.\n");
}


/*function to read employee information from file*/
void loadEmployeeData(Employee employees[], int *employeeCount) {
    FILE *file = fopen("employee_data.bin", "rb");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }

    // Read the number of employees first
    fread(employeeCount, sizeof(int), 1, file);

    // Read employee data
    for (int i = 0; i < *employeeCount; i++) {
        fread(&employees[i], sizeof(Employee), 1, file);
    }

    fclose(file);
    printf("Employee data loaded successfully.\n");
}

/*function to add employee*/
void addEmployee(Employee employees[], int *employeeCount) {
    if (*employeeCount >= MAX_EMPLOYEES) {
        printf("Employee limit reached. Cannot add more employees.\n");
        return;
    }
    Employee *emp = &employees[*employeeCount];
    printf("Enter Employee ID: ");
    scanf("%d", &emp->id);
    printf("Enter First Name: ");
    scanf("%s", emp->fistname);
    printf("Enter Last Name: ");
    scanf("%s", emp->lastname);
    printf("Enter Basic Salary: ");
    scanf("%f", &emp->gross_pay);
    printf("Enter Housing Allowance: ");
    scanf("%f", &emp->housing_allowance);
    printf("Enter Medical Allowance: ");
    scanf("%f", &emp->medical_allowance);
    printf("Enter Transport Allowance: ");
    scanf("%f", &emp->transport_allowance);
    (*employeeCount)++;
    printf("Employee added successfully.\n");
}

/*function to display all employees*/
void displayAllEmployees(const Employee employees[], int employeeCount) {
    if (employeeCount == 0) {
        printf("No employees to display.\n");
        return;
    }
    for (int i = 0; i < employeeCount; i++) {
        printf("Employee ID: %d\n", employees[i].id);
        printf("Name: %s %s\n", employees[i].fistname, employees[i].lastname);
        printf("Basic Salary: %.2f\n", employees[i].gross_pay);
        printf("Housing Allowance: %.2f\n", employees[i].housing_allowance);
        printf("Medical Allowance: %.2f\n", employees[i].medical_allowance);
        printf("Transport Allowance: %.2f\n", employees[i].transport_allowance);
        printf("Net Salary: %.2f\n\n", calculateNetPay(&employees[i]));
    }
}
/*function to update a employees information by employee id*/
void updateEmployee(Employee employees[], int employeeCount) {
    int id;
    printf("Enter Employee ID to update: ");
    scanf("%d", &id);
    for (int i = 0; i < employeeCount; i++) {
        if (employees[i].id == id) {
            printf("Enter new Basic Salary: ");
            scanf("%f", &employees[i].gross_pay);
            printf("Enter new Housing Allowance: ");
            scanf("%f", &employees[i].housing_allowance);
            printf("Enter new Medical Allowance: ");
            scanf("%f", &employees[i].medical_allowance);
            printf("Enter new Transport Allowance: ");
            scanf("%f", &employees[i].transport_allowance);
            printf("Employee updated successfully.\n");
            return;
        }
    }
    printf("Employee with ID %d not found.\n", id);
}

/*function to delete an employee */
void deleteEmployee(Employee employees[], int *employeeCount) {
    int id;
    printf("Enter Employee ID to delete: ");
    scanf("%d", &id);
    for (int i = 0; i < *employeeCount; i++) {
        if (employees[i].id == id) {
            for (int j = i; j < *employeeCount - 1; j++) {
                employees[j] = employees[j + 1]; // Shift employees to fill the gap
            }
            (*employeeCount)--;
            printf("Employee deleted successfully.\n");
            return;
        }
    }
    printf("Employee with ID %d not found.\n", id);
}


/*function to search for an employee by id*/
void searchEmployeeById(const Employee employees[], int employeeCount) {
    int id;
    printf("Enter Employee ID to search: ");
    scanf("%d", &id);
    for (int i = 0; i < employeeCount; i++) {
        if (employees[i].id == id) {
            printf("Employee ID: %d\n", employees[i].id);
            printf("Name: %s %s\n", employees[i].fistname, employees[i].lastname);
            printf("Net Salary: %.2f\n", calculateNetPay(&employees[i]));
            return;
        }
    }
    printf("Employee with ID %d not found.\n", id);
}

/*function to generate payroll report*/
void generatePayrollReport(const Employee employees[], int employeeCount) {
    float totalGross = 0, totalNet = 0;
    for (int i = 0; i < employeeCount; i++) {
        totalGross += calculategrosspay(&employees[i]);
        totalNet += calculateNetPay(&employees[i]);
    }
    printf("\nPayroll Report\n");
    printf("Total Gross Salary: %.2f\n", totalGross);
    printf("Total Net Salary: %.2f\n", totalNet);
}


/*prototypes to tell the compiler about the functions to access and use */
void addEmployee(Employee employees[], int *employeeCount);
void displayAllEmployees(const Employee employees[], int employeeCount);
void updateEmployee(Employee employees[], int employeeCount);
void deleteEmployee(Employee employees[], int *employeeCount);
void searchEmployeeById(const Employee employees[], int employeeCount);
void generatePayrollReport(const Employee employees[], int employeeCount);




/*main function with file operations to enable saving and retrieving from employee info from files*/

int main() {
    Employee employees[MAX_EMPLOYEES];
    int employeeCount = 0;
    int choice;

    // Load employee data from file at the start
    loadEmployeeData(employees, &employeeCount);

    while (1) {
        printf("\n1. Add Employee\n2. Display All Employees\n3. Update Employee\n4. Delete Employee\n5. Search Employee\n6. Generate Payroll Report\n7. Save & Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addEmployee(employees, &employeeCount);
                break;
            case 2:
                displayAllEmployees(employees, employeeCount);
                break;
            case 3:
                updateEmployee(employees, employeeCount);
                break;
            case 4:
                deleteEmployee(employees, &employeeCount);
                break;
            case 5:
                searchEmployeeById(employees, employeeCount);
                break;
            case 6:
                generatePayrollReport(employees, employeeCount);
                break;
            case 7:
                saveEmployeeData(employees, employeeCount);
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid choice, please try again.\n");
        }
    }
    return 0;
}

























