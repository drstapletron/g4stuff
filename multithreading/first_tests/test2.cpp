
#include <iostream>
using std::cout;
using std::endl;
#include <thread>
using std::thread;

static const int n_threads = 30;

void call_from_thread(int thread_id) {
	cout << "Hello (from thread " << thread_id << ")..." << endl;
}

int main() {
	thread thread_array[n_threads];
	
	cout << "Launching " << n_threads << " threads." << endl;
	for (int i=0; i<n_threads; i++) thread_array[i] = thread(call_from_thread, i);
	cout << "Launched from the main." << endl << endl;
	
	cout << "Joining threads:" << endl;
	for (int i=0; i<n_threads; i++) {
		cout << "  joining thread " << i << "...";
		thread_array[i].join();
		cout << " ...done." << endl;
	}
	cout << "Finished joining threads." << endl;
	
	return 0;
}

