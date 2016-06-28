
#include <string>
#include <list>
#include <vector>
#include <mutex>
#include <map>
#include <memory>
#include <iostream>
#include <typeinfo>
#include <unordered_map>

using namespace std;

namespace art
{
  struct Event
  {
    template <typename PROD> void put(PROD&& p,std::string const& i)
    {
      for(auto& it:p)
	cout << "Event got " << it.first << " "
	     << *it.second << " " << i << "\n";
    }
  };
}

struct HeldThing
{ 
  virtual ~HeldThing() { } 
  virtual void put(art::Event& e) =0;
};

template <typename PROD>
struct HeldProduct : public HeldThing
{
  typedef std::pair<size_t, std::unique_ptr<PROD>> IDProd;
  typedef std::vector<IDProd> IDProds;
  typedef std::map<string, IDProds> Prods;

  ~HeldProduct() { cout << "HeldProduct going away " << "\n"; }
  
  void put(art::Event& e)
  {
    for(auto& i:all_)
      {
	// would really need to make one "collection of collections"
	// per unique instance name, and make sure the collection type
	// is really
	// vector< pair<size_t, std::unique_ptr<PROD>>>
	e.put(std::move(i.second),i.first);
      }
  }

  void add(std::unique_ptr<PROD>&& prod,
	   std::string const& inst,
	   int id)
  {
    auto it = all_.insert(std::make_pair(inst,IDProds()));
    it.first->second.push_back(std::make_pair(id,std::move(prod)));    
  }

  // even better here would be to have all this be
  // map<string, vector<pair<int, std::unique_ptr<PROD>>>>
  // now the event 'puts' can be very simple

  Prods all_;
  
};

class FakeEvent
{
public:
  FakeEvent(art::Event& e): mutex_(), pens_(), e_(e) { }
  ~FakeEvent()
  {
    for( auto& p : pens_) { p.second->put(e_); }
  }
  
  template <typename PROD> void put(std::unique_ptr<PROD> && product,
				    std::string const& inst,
				    int id)
  {
    size_t hash = typeid(PROD).hash_code();
    
    std::lock_guard<std::mutex> l(mutex_);
    auto it = pens_.find(hash);
    if(it==pens_.end())
      {
	auto res = pens_.insert(std::make_pair(hash,std::unique_ptr<HeldThing>(new HeldProduct<PROD>())));
	if(!res.second) cerr << "Bad insert into map\n";
	it = res.first;
      }
    HeldProduct<PROD>* hp = dynamic_cast<HeldProduct<PROD>*>(it->second.get());
    hp->add(std::move(product),inst,id);
  }
private:
  std::mutex mutex_;
  std::unordered_map<size_t, std::unique_ptr<HeldThing>> pens_;
  art::Event& e_;
};

void func(art::Event& e)
{
  FakeEvent fe(e);
  std::unique_ptr<double> p1(new double(5.5));
  std::unique_ptr<int> p2(new int(5));
  fe.put(std::move(p1),"1",10);
  fe.put(std::move(p2),"1",20);
  fe.put(std::move(std::unique_ptr<double>(new double(6.5))),"2",30);
  fe.put(std::move(std::unique_ptr<int>(new int(6))),"2",40);
}

int main()
{
  art::Event e;
  func(e);
  return 0;
}
