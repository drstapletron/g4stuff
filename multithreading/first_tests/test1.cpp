
#include <iostream>
using std::cout;
using std::endl;
#include <thread>
using std::thread;

void call_from_thread() {
	cout << "Hello (from a thread)..." << endl;
}

int main() {
	thread t1(call_from_thread);
	t1.join();
	return 0;
}

