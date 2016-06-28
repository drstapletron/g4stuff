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

good answer: http://stackoverflow.com/questions/9042571/is-stdvector-or-boostvector-thread-safe

Trying to get read/write collisions to occur on writing to STL vectors.  This is
the only error I could get so far (with 80 threads):
	main(): end
	test4(62417,0x7fff7380a000) malloc: *** error for object 0x7febf47fffe0: pointer being freed was not allocated
	*** set a breakpoint in malloc_error_break to debug
	Abort trap: 6
	(returned 134)

Ok, got this after switching to Poisson worker threads:
	finished adding record 27 to list
	adding record 66 to list...
	test4(62543,0x7000021c4000) malloc: *** error for object 0x7ff6eb7fffe0: pointer being freed was not allocated
	*** set a breakpoint in malloc_error_break to debug
	adding record 67 to list...
	finished adding record 28 to list
	adding record 68 to list...
	finished adding record 29 to list
	adding record 69 to list...
	finished adding record 30 to list
	adding record 70 to list...
	launch_worker_thread()[12]: end
	adding record 71 to list...
	finished adding record 31 to list
	Abort trap: 6
	(returned 134)
*/
static const int n_threads = 80;
int sleeptime_us = 0; //10;
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
typedef std::thread Thread;

#include <gsl/gsl_rng.h>

struct Record {
	int thread_id;
	double r0;
	double r1;
	int count;
};
typedef vector<Record> RecordList;


//Globals
Thread thread_array[n_threads];
RecordList record_list;
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
void print_record_list(RecordList rlist) {
	printf("------------------\nRecord list:\n");
	for (auto r : rlist) {
		printf("  ");
		print_record(r);
	}
	printf("------------------\n");
}

Record _worker_thread_sqrts(int thread_id);
Record _worker_thread_binom(int thread_id);
Record _worker_thread_Poisson(int thread_id);
void launch_worker_thread(int thread_id) { //ALL threads start via this function
	printf("launch_worker_thread()[%d]: begin\n",thread_id);
	//Record r = _worker_thread_sqrts(thread_id);
	//Record r = _worker_thread_binom(thread_id);
	Record r = _worker_thread_Poisson(thread_id);
	print_record(r);
	
	/*printf("adding record %d to record_list...\n",thread_id);
	record_list.push_back(r);
	printf("finished adding record %d to record_list\n",thread_id);*/
	
	printf("launch_worker_thread()[%d]: end\n",thread_id);
}
Record _worker_thread_sqrts(int thread_id) {
	Record r;
	r.thread_id = thread_id;
	r.count = 0;
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
Record _worker_thread_binom(int thread_id) {
	Record r;
	r.thread_id = thread_id;
	r.count = 0;
	r.r0 = gsl_rng_uniform(rng);
	double P = 0.9 + 0.1*r.r0; //make binomial probs close to 1
	r.r1 = gsl_rng_uniform(rng);
	while (r.r1<P) {
		r.r1 = gsl_rng_uniform(rng);
		r.count++;
	}
	return r;
}
Record _worker_thread_Poisson(int thread_id) {
	Record r;
	r.thread_id = thread_id;
	r.count = 0;
	r.r0 = 0.; r.r1 = 0.;
	double lamda = 100.;
	double stepsize = 0.00001;
	//double x = 0.;
	bool going = true;
	while (going) {
		double rand_float = gsl_rng_uniform(rng);
		if (rand_float>stepsize/lamda) {
			r.count++;
		}
		else {
			going = false;
		}
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
	
	print_record_list(record_list);
	
	printf("main(): end\n");
}

