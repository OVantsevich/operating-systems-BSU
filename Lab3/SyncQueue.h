#pragma once
#include <Windows.h>
class SyncQueue 
{
public:
	SyncQueue(int queue_size);
	~SyncQueue();
	void Insert(int element);
	int Remove();
private:
	int* array_;
	int insert_index_;
	int remove_index_;
	int queue_size_;
	HANDLE insert_semaphore_;
	HANDLE remove_semaphore_;
	CRITICAL_SECTION critical_section_;
};
