#include <fstream>
#include <windows.h>
#include "..\common\Employee.h"
#include "..\common\Readers.h"

using std::ifstream;

bool Creator(string& output_file_name, int employees_numbers)
{
	if (employees_numbers < 1 || output_file_name.empty()) return false;

	string args_str = "Creator.exe " + output_file_name + " " + std::to_string(employees_numbers);
	char* args = (char*)(args_str).c_str();
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	if (!CreateProcess(NULL, args, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
		cout << "Can't create process \'Creator\'!" << endl;
		return false;
	}

	WaitForSingleObject(pi.hProcess, INFINITE);

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	return true;
}

bool Reporter(string& binary_file_name, string& repor_file_name, double salary)
{
	if (salary < 0 || repor_file_name.empty() || binary_file_name.empty()) return false;

	string args_str = "Reporter.exe " + binary_file_name + " " + repor_file_name + " " + std::to_string(salary);
	char* args = (char*)(args_str).c_str();

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	if (!CreateProcess(NULL, args, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
		cout << "Can't create process \'Reporter\'!" << endl;
		return false;
	}

	WaitForSingleObject(pi.hProcess, INFINITE);

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	return true;
}

bool OutEmployee(string& input_file_name, int employees_number)
{
	ifstream in(input_file_name, std::ios::binary);
	if (!in.is_open()) {
		cout << "The file you are looking for does not exist!" << endl;
		return false;
	}

	Employee* employees = new Employee[employees_number];

	in.read((char*)employees, static_cast<std::streamsize>(sizeof(Employee)) * employees_number);
	if (employees == nullptr) {
		cout << "Data error!" << endl;
		in.close();
		return false;
	}

	for (int i = 0; i < employees_number; i++)
		cout << employees[i];

	delete[] employees;
	in.close();
	return true;
}

bool OutReportFile(string& repor_file_name) {

	ifstream in(repor_file_name + ".txt");
	if (!in.is_open()) {
		cout << "The file you are looking for does not exist!" << endl;
		return false;
	}

	string str;
	while (std::getline(in, str)) {
		cout << str << endl;
	}

	cout << endl;
	in.close();
	return true;
}

bool Create(string& binary_file_name)
{
	int employees_number;
	StringReader sr;
	IntReader ir;

	sr.Read(binary_file_name, "Enter file name: ",
		[](string& str) {return str.size() < 10 && string::npos == str.find(' '); }, "File name is not valid!");
	ir.Read(employees_number, "Enter employees number: ",
		[](int& i) {return i > 0; }, "Employees number can't be negativ!");

	if (!Creator(binary_file_name, employees_number)) {
		cout << "Input data error!" << endl;
		return false;
	}

	OutEmployee(binary_file_name, employees_number);
	return true;
}

bool Report(string& binary_file_name)
{
	double salary;
	string repor_file_name;
	StringReader sr;
	DoubleReader dr;

	sr.Read(repor_file_name, "Enter report file name: ",
		[](string& str) {return str.size() < 10 && string::npos == str.find(' '); }, "File name is not valid!");
	dr.Read(salary, "Enter employees salary: ",
		[](double& d) {return d > 0; }, "Employees salary can't be negativ!");

	if (!Reporter(binary_file_name, repor_file_name, salary)) {
		return false;
	}

	if (!OutReportFile(repor_file_name)) return false;

	return true;
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, ".1251");

	string binary_file_name;
	if (!Create(binary_file_name)) return 0;

	if (!Report(binary_file_name)) return 0;

	system("pause");
	return 0;
}