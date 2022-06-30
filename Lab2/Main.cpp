#include <windows.h>
#include "..\common\Readers.h"
#include "PolinomData.h"

void PolinomInput(PolinomData& polinom_data, string& name_message)
{
	IntReader ir;
	DoubleReader dr;

	ir.Read(polinom_data.polynomial_degree, ("Enter polynomial " + name_message + " degree: ").c_str(),
		[](int& i) {return i >= 0; }, "Polynomial degree can't be negative");
	polinom_data.polynomial_coefficients = new double[polinom_data.polynomial_degree];
	for (int i = 0; i < polinom_data.polynomial_degree; i++)
		dr.Read(polinom_data.polynomial_coefficients[i], ("Enter coefficient " + std::to_string(i) + " : ").c_str());
}

DWORD WINAPI Polynom(LPVOID param)
{
	PolinomData* data = static_cast<PolinomData*>(param);
	if (nullptr == data || !data->IsCorrect()) {
		cout << "Incorrect data!" << endl;
		return 0;
	}

	data->result = 0;
	double current_step;
	for (int i = 0; i < data->polynomial_degree; i++) {
		current_step = data->polynomial_coefficients[i];
		for (int j = 0; j < data->polynomial_degree - i; j++) {
			current_step *= data->variable_X;
		}
		data->result += current_step;
		Sleep(15);
	}

	cout << "p(" << data->variable_X << ") = " << data->result << endl;
	return 0;
}

bool ComputeRationalFunction()
{
	PolinomData* polinom_data = new PolinomData[2];

	DoubleReader dr;

	string numerator("numerator");
	string denominator("denominator");
	double x;

	dr.Read(x, "Enter variable x: ");
	PolinomInput(polinom_data[0], numerator);
	PolinomInput(polinom_data[1], denominator);
	polinom_data[0].variable_X = x;
	polinom_data[1].variable_X = x;

	HANDLE handle[2];

	handle[0] = CreateThread(NULL, 0, Polynom, &polinom_data[0], NULL, NULL);
	handle[1] = CreateThread(NULL, 0, Polynom, &polinom_data[1], NULL, NULL);

	if (handle[0] == NULL || handle[1] == NULL) {
		cout << "Can't create thread!" << endl;
		return false;
	}

	WaitForMultipleObjects(2, handle, TRUE, INFINITE);
	CloseHandle(handle[0]);
	CloseHandle(handle[1]);

	if (0 == polinom_data[1].result) {
		cout << "Division by 0 error!" << endl;
		return false;
	}

	cout << "x: " << x << endl << "f(" << x << ") = " << polinom_data[0].result / polinom_data[1].result << endl;

	delete[] polinom_data;

	return true;
}


int main()
{
	setlocale(LC_ALL, ".1251");

	ComputeRationalFunction();

	return 0;
}