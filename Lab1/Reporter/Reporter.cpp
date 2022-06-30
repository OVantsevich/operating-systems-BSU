#include <iostream>
#include <fstream>
#include <algorithm>
#include "..\Employee.h"
#include "..\..\common\Readers.h"

using std::ifstream;
using std::ofstream;

bool EmployeeNumComparator(Employee a, Employee b)
{
	return (a.num < b.num);
}

int GetEmployeesNumber(string& input_file_name)
{
	ifstream in(input_file_name, std::ios::binary | std::ios::ate);
	if (!in.is_open()) {
		cout << "The file you are looking for does not exist!" << endl;
		return -1;
	}

	int employees_number = in.tellg() / sizeof(Employee);
	if (employees_number < 0) {
		cout << "Data error!" << endl;
		return -1;
	}

	in.close();
	return employees_number;
}

bool ReadEmployee(string& input_file_name, const Employee* employees, int employees_number)
{
	ifstream in(input_file_name, std::ios::binary);
	if (!in.is_open()) {
		cout << "The file you are looking for does not exist!" << endl;
		return false;
	}

	in.read((char*)employees, static_cast<std::streamsize>(sizeof(Employee)) * employees_number);
	if (employees == nullptr) {
		cout << "Data error!" << endl;
		in.close();
		return false;
	}

	std::sort((Employee*)employees, (Employee*)employees + employees_number, EmployeeNumComparator);

	in.close();
	return true;
}

bool CreateReport(string& output_file_name, string& input_file_name, int employees_number, const Employee* employees, double salary) 
{

	if (salary < 0) {
		cout << "Can't create report!\nInvalid salary value!" << endl;
		return false;
	}
	if (employees_number <= 0) {
		cout << "Can't create report!\nInvalid employees number!" << endl;
		return false;
	}

	ofstream out(output_file_name + ".txt");

	out << "File \'" << input_file_name << "\' report" << std::endl;
	out << "Employee number, Employee name, Hours, Salary." << std::endl;

	for (int i = 0; i < employees_number; i++) {
		out << employees[i].num << " " << employees[i].name << " " << employees[i].hours << " " << static_cast<double>(employees[i].hours * salary) << std::endl;
	}

	out.close();
	return true;
}

int main(int argc, char* argv[]) 
{
	setlocale(LC_ALL, ".1251");

	string input_file_name = argv[1];
	string output_file_name = argv[2];
	double salary = stod(string(argv[3]));

	int employees_number = GetEmployeesNumber(input_file_name);
	if (employees_number < 0) return 0;

	Employee* employees = new Employee[employees_number];

	if(ReadEmployee(input_file_name, employees, employees_number)) 
		CreateReport(output_file_name, input_file_name, employees_number, employees, salary);

	delete[] employees;

	return 0;
}