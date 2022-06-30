#include <windows.h>
#include "SyncQueue.h"
#include "..\common\Readers.h"

HANDLE start;

struct ProducerData {
	int serial_number;
	int number_of_cycles;
	SyncQueue* queue_ref;
	ProducerData() {
		serial_number = 0;
		number_of_cycles = 0;
		queue_ref = nullptr;
	}
};

struct ConsumerData {
	int number_of_cycles;
	SyncQueue* queue_ref;
	ConsumerData() {
		number_of_cycles = 0;
		queue_ref = nullptr;
	}
};

DWORD WINAPI producer(LPVOID data)
{
	WaitForSingleObject(start, INFINITE);

	ProducerData* producer_Data = (ProducerData*)data;

	for (int i = 0; i < producer_Data->number_of_cycles; i++)
	{
		producer_Data->queue_ref->Insert(producer_Data->serial_number);
		cout << "Produced number: " << producer_Data->serial_number << endl;
		Sleep(7);
	}

	return 0;
}

DWORD WINAPI consumer(LPVOID data)
{
	WaitForSingleObject(start, INFINITE);

	ConsumerData* consumer_Data = (ConsumerData*)data;

	for (int i = 0; i < consumer_Data->number_of_cycles; i++)
	{
		int x = consumer_Data->queue_ref->Remove();
		cout << "Consumed number: " << x << endl;
		Sleep(7);
	}

	return 0;
}

int NumberOfProducers()
{
	IntReader ir;
	int number_of_producers = 0;
	ir.Read(number_of_producers, "Enter number of producers: ", [](int& i) {return i > 0; }, "Number of producers can't be negative!");
	return number_of_producers;
}

int NumberOfConsumers()
{
	IntReader ir;
	int number_of_cunsumers = 0;
	ir.Read(number_of_cunsumers, "Enter number of cunsumers: ", [](int& i) {return i > 0; }, "Number of cunsumers can't be negative!");
	return number_of_cunsumers;
}

void InputProducersData(SyncQueue& queue, ProducerData* producers_data, int number_of_producers)
{
	IntReader ir;

	for (int i = 0; i < number_of_producers; i++)
	{
		string message("Enter number of numbers for producer No.");
		message += std::to_string(i + 1);
		message += ": ";
		ir.Read(producers_data[i].number_of_cycles, message.c_str(), [](int& i) {return i > 0; }, "Number of numbers can't be negative!");
		producers_data[i].serial_number = i + 1;
		producers_data[i].queue_ref = &queue;
	}
}

void InputConsumersData(SyncQueue& queue, ConsumerData* consumers_data, int number_of_cunsumers)
{
	IntReader ir;

	for (int i = 0; i < number_of_cunsumers; i++)
	{
		string message("Enter number of numbers for cunsumer No.");
		message += std::to_string(i + 1);
		message += ": ";
		ir.Read(consumers_data[i].number_of_cycles, message.c_str(), [](int& i) {return i > 0; }, "Number of numbers can't be negative!");
		consumers_data[i].queue_ref = &queue;
	}
}

bool CreateProducers(HANDLE* producers, ProducerData* producers_data, int number_of_producers)
{
	for (int i = 0; i < number_of_producers; i++)
	{
		producers[i] = CreateThread(NULL, 0, producer, &producers_data[i], NULL, NULL);
		if (NULL == producers[i]) {
			cout << "An error occurred while creating the thread producer!" << endl;
			return false;
		}
	}
	return true;
}

bool CreateConsumers(HANDLE* consumers, ConsumerData* consumers_data, int number_of_consumers)
{
	for (int i = 0; i < number_of_consumers; i++)
	{
		consumers[i] = CreateThread(NULL, 0, consumer, &consumers_data[i], NULL, NULL);
		if (NULL == consumers[i]) {
			cout << "An error occurred while creating the thread consumer!" << endl;
			return false;
		}
	}
	return true;
}

bool Clear(HANDLE* data, int data_size) 
{
	if (data != nullptr) {
		for (int i = 0; i < data_size; i++)
			CloseHandle(data[i]);
		delete[] data;
	}
	else return false;
	return true;
}

int main() 
{
	setlocale(LC_ALL, ".1251");

	start = CreateEvent(NULL, TRUE, FALSE, NULL);

	IntReader ir;

	int queue_size = 0;
	ir.Read(queue_size, "Enter queue size: ", [](int& i) {return i > 0; }, "Queue size can't be negative!");
	SyncQueue queue(queue_size);

	int number_of_producers = NumberOfProducers();
	int number_of_consumers = NumberOfConsumers();

	ProducerData* producers_data = new ProducerData[number_of_producers];
	ConsumerData* consumers_data = new ConsumerData[number_of_consumers];

	InputProducersData(queue, producers_data, number_of_producers);
	InputConsumersData(queue, consumers_data, number_of_consumers);

	HANDLE* producers = new HANDLE[number_of_producers];
	HANDLE* consumers = new HANDLE[number_of_consumers];

	CreateProducers(producers, producers_data, number_of_producers);
	CreateConsumers(consumers, consumers_data, number_of_consumers);

	SetEvent(start);

	WaitForMultipleObjects(number_of_producers, producers, TRUE, INFINITE);
	WaitForMultipleObjects(number_of_consumers, consumers, TRUE, INFINITE);
	
	Clear(producers, number_of_producers);
	Clear(consumers, number_of_consumers);

	delete[] producers_data;
	delete[] consumers_data;

	return 0;
}