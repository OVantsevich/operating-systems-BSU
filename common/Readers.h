#pragma once
#include <functional>
#include <string>
#include <iostream>

using std::string;
using std::getline;
using std::cout;
using std::cin;
using std::endl;

template<class T>
class TReader {

public:
	void Read(T& dest, const char* message = "Enter value: ",
		bool(*IsValid)(T&) = nullptr, const char* validation_error_message = "The entered value is incorrect!")
	{
		string str("");
		T current_value;
		bool if_conversion_unsuccessful = false;

		cout << message;
		getline(cin, str);
		while ((if_conversion_unsuccessful = !Conversion(current_value, str)) ||
			(IsValid != nullptr ? !IsValid(current_value) : false)) {
			system("cls");
			if_conversion_unsuccessful ? cout << "Input Error. Check input value!" << endl : cout << validation_error_message << endl;
			cout << message;
			getline(cin, str);
		}

		dest = current_value;
	}

	virtual bool Conversion(T&, const string&) = 0;
};

class IntReader : public TReader<int> {

	bool IsInt(const string& str)
	{
		if (str.empty()) return false;

		string str_copy = str;

		int spaces_in_front = 0;
		while (spaces_in_front < str_copy.size() && ' ' == str_copy[spaces_in_front]) ++spaces_in_front;
		if (spaces_in_front == str_copy.size()) return false;

		int spaces_behind = str_copy.size() - 1;
		while (spaces_behind >= 0 && ' ' == str_copy[spaces_behind]) --spaces_behind;

		str_copy = str_copy.substr(spaces_in_front, spaces_behind - spaces_in_front + 1);
		if (str_copy.size() > 11) return false;

		return str_copy[0] == '-'
			? std::find_if(str_copy.begin() + 1, str_copy.end(), [](unsigned char c) { return !std::isdigit(c); }) == str_copy.end() && stoll(str_copy) >= INT_MIN
			: std::find_if(str_copy.begin(), str_copy.end(), [](unsigned char c) { return !std::isdigit(c); }) == str_copy.end() && stoll(str_copy) <= INT_MAX;
	}


	virtual bool Conversion(int& destination, const string& str)
	{
		if (!IsInt(str)) return false;
		destination = stoi(str);
		return true;
	}
};

class DoubleReader : public TReader<double> {

	bool IsDouble(const string& str)
	{
		if (str.empty()) return false;

		char* endptr = 0;
		double d = strtod(str.c_str(), &endptr);

		if (*endptr != '\0')
			return false;
		return true;
	}

	virtual bool Conversion(double& destination, const string& str) 
	{
		if (!IsDouble(str)) return false;
		destination = stod(str);
		return true;
	}
};

class StringReader : public TReader<string> {

	virtual bool Conversion(string& destination, const string& str)
	{
		if (str.empty()) return false;
		destination = str;
		return true;
	}
};
