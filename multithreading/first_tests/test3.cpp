/*

things to play with:
	n_threads: of course
	sleeptime_us: try sleeping for 0, 1, 30, 1000, 30000, 1000000, 30000000
	try writing to RecordList rec_list from thread (with AND without mutex)
	worker_thread: fiddle with parameters that set times?
	try doing weird stuff with thread_array (esp. from inside thread?)

try making a global timer that all threads can read (for performance reporting)

oddities:
	sometimes different threads get the same random number...?
	(also threads get randoms in non-reproduceable order)
*/
static const int n_threads = 4;
int sleeptime_us = 10;
//0: thread::join waits for a while
//60: main wakes up around time _worker_thread_sqrts start to print records
//100: all threads finish before main wakes

#include <iostream>
using std::cout;
using std::endl;
#include <vector>
using std::vector;
#include <cmath>
#include <unistd.h>

#include <thread>
//using std::thread;
typedef std::thread Thread;

#include <gsl/gsl_rng.h>

struct Record {
	int thread_id;
	double r0;
	double r1;
	int count = 0;
};
typedef vector<Record> RecordList;


//Globals
Thread thread_array[n_threads];
RecordList rec_list;
const gsl_rng_type * rng_type;
gsl_rng * rng;
void _gsl_rng_init() {
	//some magicals for GSL
	gsl_rng_env_setup();
	rng_type = gsl_rng_default;
	rng = gsl_rng_alloc(rng_type);
	//cout<<"Random uniform number: "<<gsl_rng_uniform(rng)<<endl;
}



void print_record(Record r) {
	printf("[%d, %f, %f, %d]\n", r.thread_id, r.r0, r.r1, r.count);
}

Record _worker_thread_sqrts(int thread_id);
void launch_worker_thread(int thread_id) {
	//ALL threads start with this
	printf("launch_worker_thread()[%d]: begin\n",thread_id);
	Record r = _worker_thread_sqrts(thread_id);
	print_record(r);
	printf("launch_worker_thread()[%d]: end\n",thread_id);
}
Record _worker_thread_sqrts(int thread_id) {
	Record r;
	r.thread_id = thread_id;
	//r.count = 0;
	r.r0 = gsl_rng_uniform(rng);
	r.r1 = r.r0;
	r.r1 *= 1.0e15;
	while (r.r1>2.) {
		//r.r1 = sqrt(r.r1);
		r.r1 = 0.9*r.r1;
		r.count++;
	}
	return r;
}

int main() {
	printf("main(): begin\n");
	_gsl_rng_init();
	
	
	printf("Launching %d threads...\n",n_threads);
	for (int i=0; i<n_threads; i++) thread_array[i] = Thread(launch_worker_thread, i);
	printf(" ...done launching threads.\n");
	
	
	if (sleeptime_us>0) {
		printf("Sleeping for %d us...\n", sleeptime_us);
		usleep(sleeptime_us);
		printf("...done sleeping\n");
	}
	
	
	printf("Joining threads:\n");
	for (int i=0; i<n_threads; i++) {
		printf("  joining thread %d...\n",i);
		thread_array[i].join();
		printf("  ...done joining thread %d.\n",i);
	}
	printf("Finished joining threads.\n");
	
	
	printf("main(): end\n");
}

