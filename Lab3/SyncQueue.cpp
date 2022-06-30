#include "SyncQueue.h"
#include <iostream>

using std::cout;
using std::endl;

SyncQueue::SyncQueue(int queue_size)
{
	if (queue_size <= 0) {
		cout << "Error. queue size can't be negative!" << endl;
		return;
	}

	this->array_ = new int[queue_size];
	this->queue_size_ = queue_size;
	this->insert_index_ = 0;
	this->remove_index_ = 0;

	for (int i = 0; i < this->queue_size_; i++) this->array_[i] = 0;

	insert_semaphore_ = CreateSemaphore(NULL, 0, this->queue_size_, NULL);
	remove_semaphore_ = CreateSemaphore(NULL, this->queue_size_, this->queue_size_, NULL);
	InitializeCriticalSection(&critical_section_);
}

SyncQueue::~SyncQueue()
{
	DeleteCriticalSection(&critical_section_);
	CloseHandle(insert_semaphore_);
	CloseHandle(remove_semaphore_);
	if (array_ != nullptr) {
		delete[] array_;
		array_ = nullptr;
	}
}

void SyncQueue::Insert(int element)
{
	WaitForSingleObject(insert_semaphore_, INFINITE);

	EnterCriticalSection(&critical_section_);

	array_[insert_index_] = element;
	insert_index_ = (insert_index_ + 1) % queue_size_;

	ReleaseSemaphore(remove_semaphore_, 1, NULL);

	LeaveCriticalSection(&critical_section_);
}

int SyncQueue::Remove() 
{
	WaitForSingleObject(remove_semaphore_, INFINITE);

	EnterCriticalSection(&critical_section_);

	int return_value = array_[remove_index_];
	array_[remove_index_] = 0;
	remove_index_ = (remove_index_ + 1) % queue_size_;

	ReleaseSemaphore(insert_semaphore_, 1, NULL);

	LeaveCriticalSection(&critical_section_);

	return return_value;
}

