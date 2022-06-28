#pragma once
struct Employee {
	int num; //employee identification number
	char name[10]; //employee name
	double hours; //hours worked
};

std::ostream& operator<<(std::ostream& cout, Employee& e) {
	cout << "num: " << e.num << "\tname: " << e.name << "\thours: " << e.hours << std::endl;
	return cout;	
}