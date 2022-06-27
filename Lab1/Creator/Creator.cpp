#include <fstream>
#include "..\..\common\Employee.h"
#include "..\..\common\Readers.h"

bool FindID(const Employee* employees, int employee_number, int searched_id)
{
	if (nullptr == employees || employee_number <= 0) return false;

	for (int i = 0; i < employee_number; i++)
		if (searched_id == employees[i].num) return true;
	return false;
}

bool EmployeeArrayInput(Employee* employees, int employee_number,
	const char* non_existing_array_error = "Array does not exist!",
	const char* invalid_array_size_error = "Array size is invalid!")
{
	if (nullptr == employees) {
		cout << non_existing_array_error << endl;
		return false;
	}
	if (employee_number < 0) {
		cout << invalid_array_size_error << endl;
		return false;
	}

	IntReader ir;
	StringReader sr;
	DoubleReader dr;

	for (int i = 0; i < employee_number; i++) {
		ir.Read(employees[i].num, "Enter employee identification number: ", [](int& i) {return i > 0; },
			"The entered employee identification number is invalid!");
		while (FindID(employees, i, employees[i].num)){
			system("cls");
			cout << "Employee with this id already exists!" << endl;
			ir.Read(employees[i].num, "Enter employee identification number: ", [](int& i) {return i > 0; },
				"The entered employee identification number is invalid!");
		}

		string name_str;
		sr.Read(name_str, "Enter employee name: ", [](string& str) {return str.size() < 10; },
			"The entered name is too long!");
		strcpy_s(employees[i].name, name_str.c_str());

		dr.Read(employees[i].hours, "Enter the number of hours worked: ", [](double& d) {return d > 0; },
			"The number of hours cannot be negative!");

		system("cls");
	}
	return true;
}

bool WriteEmployee(const Employee* employees, int employee_number, const char* file_name)
{
	if (nullptr == employees) {
		cout << "Input error!" << endl;
		return false;
	}
	if (employee_number < 0) {
		cout << "Input error!" << endl;
		return false;
	}

	std::ofstream out(file_name, std::ios::binary);
	if (!out.is_open()) {
		cout << "Error opening file!" << endl;
		return false;
	}

	out.write((char*)employees, static_cast<std::streamsize>(sizeof(Employee)) * employee_number);

	out.close();
	return true;
}

int main(int argc, char* argv[]) 
{
	setlocale(LC_ALL, ".1251");

	const char* file_name = argv[1];
	int employee_number = atoi(argv[2]);

	Employee* employees = new Employee[employee_number];

	if (EmployeeArrayInput(employees, employee_number))
		WriteEmployee(employees, employee_number, file_name);

	delete[] employees;
	return 0;
}