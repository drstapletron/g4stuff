/*

*/

/*

Things in 'std::':
	unique_ptr
	lock_guard
	mutex
	move
	make_pair
	pair,vector,map,unordered_map
	string
*/

#include <string>
#include <list>
#include <vector>
#include <mutex>
#include <map>
#include <memory>
#include <iostream>
#include <typeinfo>
#include <unordered_map>

#include "DBGMSG.h"

//using namespace	std;
using std::unique_ptr;
using std::move;
using std::lock_guard;
using std::mutex;
using std::make_pair;
using std::pair;
using std::vector;
using std::map;
using std::unordered_map;
using std::string;
using std::cout;
using std::cerr;
using std::endl;

namespace art {
	struct Event {
		template < typename PROD > void put(PROD && p, string const &i){
			DBGMSG("art::Event::put(..): BEGIN");
			for (auto & it:p) cout<<"Event got "<<it.first<<" "<<*it.second<<" "<<i<<"\n";
			DBGMSG("art::Event::put(..): BEGIN");
		}
	};
}

struct HeldThing {
	virtual ~HeldThing() {
		DBGMSG("HeldThing::~HeldThing(): BEGIN");
		DBGMSG("HeldThing::~HeldThing(): BEGIN");
	}
	virtual void put(art::Event & e) = 0;
};

template < typename PROD >
struct HeldProduct : public HeldThing {
	typedef	pair < size_t, unique_ptr < PROD >> IDProd;
	typedef	vector < IDProd > IDProds;
	typedef	map < string, IDProds > Prods;

	~HeldProduct() {
		DBGMSG("HeldProduct::~HeldProduct(): BEGIN");
		//cout << "HeldProduct going away " << "\n";
		DBGMSG("HeldProduct::~HeldProduct(): BEGIN");
	}

	void put(art::Event & e) {
		DBGMSG("HeldProduct::put(...): BEGIN");
		for (auto & i:all_) {
			//would really need to make one "collection of collections"
			// per unique instance name, and make sure the collection type
			// is really
			// vector < pair < size_t, unique_ptr < PROD >> >
			DBGMSG("e.put(move(i.second),i.first)");
			e.put(move(i.second), i.first);
		}
		DBGMSG("...finished loop over all_");
		DBGMSG("HeldProduct::put(...): END");
	}

	void add( 
			unique_ptr < PROD > &&prod,
			string	const& inst, int id) {
		DBGMSG("HeldProduct::add(...): BEGIN");
		DBGMSG("all_.insert(make_pair(inst,IDProds()))");
		auto it = all_.insert(make_pair(inst, IDProds()));
		DBGMSG("it.first->second.push_back(make_pair(id,move(prod)))");
		it.first->second.push_back(make_pair(id, move(prod)));
		DBGMSG("HeldProduct::add(...): END");
	}

	//even better here would be to have all this be
	//map <	string, vector < pair < int, unique_ptr < PROD >> >>
	//now the event 'puts' can be very simple
	Prods all_;

};

class FakeEvent {
	public:
	FakeEvent(art::Event & e) : mutex_(), pens_(), e_(e) { }
	~FakeEvent() {
		DBGMSG("FakeEvent::~FakeEvent(): start");
		for (auto & p:pens_) p.second->put(e_);
		DBGMSG("FakeEvent::~FakeEvent(): start");
	}

	template <typename PROD> void put(
		unique_ptr <PROD> &&product,
		string	const &inst, int id) 
	{
		DBGMSG("FakeEvent::put(...): BEGIN");
		size_t hash = typeid(PROD).hash_code();

		lock_guard< mutex > l(mutex_);
		auto it = pens_.find(hash);
		if (it == pens_.end()) {
			auto res = pens_.insert(make_pair(hash, unique_ptr < HeldThing > (new HeldProduct < PROD > ())));
			if (!res.second) cerr << "Bad insert into map\n";
			it = res.first;
		}
		HeldProduct <	PROD > *hp = dynamic_cast < HeldProduct < PROD > *>(it->second.get());
		hp->add(move(product), inst, id);
		DBGMSG("FakeEvent::put(...): END");
	}
	
	private:
	mutex mutex_;
	unordered_map<size_t, unique_ptr<HeldThing>> pens_;
	art::Event & e_;
};

void func(art::Event & e)
{
	DBGMSG("func(e): BEGIN");
	FakeEvent fe(e);
	unique_ptr <double> p1(new double (5.5));
	unique_ptr <int> p2(new int (5));
	fe.put(move(p1), "1", 10);
	fe.put(move(p2), "1", 20);
	fe.put(move(unique_ptr <double>(new double (6.5))), "2", 30);
	fe.put(move(unique_ptr <int>(new int (6))), "2", 40);
	DBGMSG("func(e): END");
}

int main()
{
	DBGMSG("creating art event 'e'");
	art::Event e;
	
	func(e);
	return 0;
}
