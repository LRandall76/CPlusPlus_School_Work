//	Levi Randall
//  James Samawi
//  minideque.h
//  minidequeproject
//
//////////////////////////////////// CURRENT STATUS (10/21/18) :: 25 / 25 passing 
#ifndef minideque_h
#define minideque_h

#include <vector>
#include <stack>
#include <algorithm>
#include <string>
#include <iostream>


using namespace std;

template <typename T>
class minideque {
private:
  std::vector<T> fronthalf_;			// front half vector of the deque
  std::vector<T> backhalf_;				// back half vector of the deque
  std::vector<T> resizevector_;			// contains front and back vectors


public:
  minideque() = default;                                   // do not change, C++ defaults are ok
  minideque(const minideque<T>& other) = default;          // rule of three
  minideque& operator=(const minideque<T>& other) = default;
  ~minideque() = default;

  size_t size() const {return fronthalfsize() + backhalfsize();}           

  size_t fronthalfsize() const { return fronthalf_.size();}      
  
  size_t backhalfsize() const { return backhalf_.size(); }    
  
  bool empty() const { return fronthalf_.empty() && backhalf_.empty(); }    
  // Returns if both front and back vectors are empty when called.      

  
  void pop_front(){ // Pops value at FRONT of vec
	  
	  fronthalf_.pop_back();

	  int freshsize = this->size(); //for resizing

/////////////////////EXTRACTING/////////////////////////// conetents of front and back vectors moved to resizevector

	if (fronthalfsize() != empty()) {
	  for (unsigned int x = 0; x < fronthalfsize(); x++) {

		  resizevector_.push_back(fronthalf_.at(x));	
	  }
	}
	
	if (backhalfsize() != empty()) {
	  for (unsigned int x = 0; x < backhalfsize(); x++) {

		  resizevector_.push_back(backhalf_.at(backhalfsize() - 1 - x));
	  }
	}  
	
/////////////////////REINSERTING/////////////////////////// resizing process (specific cases)

	  fronthalf_.clear(); //call clear function
	  backhalf_.clear();
  
	  if (freshsize % 2 == 1) { // if odd
		  for (int x = 0; x < (freshsize / 2); x++) {

			  this->push_front(resizevector_.at(x));
		  }

		  for (int x = 0; x < (freshsize / 2 + 1); x++) {

			  this->push_back(resizevector_.at(fronthalf_.size() + x));
		  }
		  
	  } else { // if even
		  for (int x = 0; x < (freshsize / 2); x++) {

			  this->push_front(resizevector_.at(x));
		  }

		  for (int x = 0; x < (freshsize / 2); x++) {

			  this->push_back(resizevector_.at(fronthalf_.size() + x));
		  }
	  }

	  if (this->size() == 1) {
			this->push_front(backhalf_.at(0));
			backhalf_.clear();
	  }

		resizevector_.clear(); 

  }
  
  
  void pop_back(){backhalf_.pop_back();}		// Pops value at BACK of vec 2
  

  void push_front(const T& value) {fronthalf_.push_back(value);} // Pushes value to FRONT of vec 1 (passes value)


  void push_back(const T& value) {backhalf_.emplace( backhalf_.begin(), value);} // Pushes value to BACK of vec 2 (passes value)

//////////////////////////////////////////////////////////////////

  const T& front() const{   // COPY OF FRONT
	if (fronthalf_.empty()) { 
		return backhalf_.front();		
		
    }else{ return fronthalf_.back(); } 
	  
  }
  
  const T& back() const {    // COPY OF BACK
	if (backhalf_.empty()) { 
		return fronthalf_.back();		
		
    }else{ return backhalf_.front(); }  
	  
  }      
	  
  const T& operator[](size_t index) const {  //COPY OF OPERATOR[]
  if (index < fronthalf_.size()) {
		  return fronthalf_ [ (fronthalf_.size() - 1) - index ];
		  
	  } else { return backhalf_ [ index - fronthalf_.size() ]; }
	  
  }

//////////////////////////////////////////////////////////////////
  
  T& front() {      		// This returns the first value in the deque            
	
	if (fronthalf_.empty()) { 
		return backhalf_.front();		// return front of backhalf IF FRONTHALF EMPTY
		
    }else{ return fronthalf_.back(); }  // return back of fronthalf
	  
  }
  
  
  T& back()  {                // This returns the last value in the deque   
	  
	if (backhalf_.empty()) { 
		return fronthalf_.back();		// return back of fronthalf IF BACKHALF EMPTY
		
    }else{ return backhalf_.front(); }  // return front of backhalf 
	  
  }    
  

  T& operator[](size_t index) {              
	  
	   if (index < fronthalf_.size()) {
		  return fronthalf_ [ (fronthalf_.size() - 1) - index ];
		  
	  } else { return backhalf_ [ index - fronthalf_.size() ]; }
	  
  }


  void clear() { return fronthalf_.clear() && backhalf_.clear(); } 	
  // Clears both front and back vectors if called.


/////////////////////////////////////////////////////////////////////////////////////////////////
  friend std::ostream& operator<<(std::ostream& os, minideque<T>& dq) {    // do not change
    if (dq.empty()) { return os << "minideque is empty"; }

    dq.printFronthalf(os);
    os << "| ";
    dq.printBackhalf(os);
    os << ", front:" << dq.front() << ", back:" << dq.back() << ", size:" << dq.size();
    return os;
  }

  void printFronthalf(std::ostream& os=std::cout) const {                    // do not change
    if (empty()) { std::cout << "fronthalf vector is empty"; }

    for (typename std::vector<T>::const_reverse_iterator crit = fronthalf_.crbegin();
         crit != fronthalf_.crend(); ++crit) {
      os << *crit << " ";
    }
  }

  void printBackhalf(std::ostream& os=std::cout) const {                     // do not change
    if (empty()) { os << "backhalf vector is empty"; }

    for (typename std::vector<T>::const_iterator cit = backhalf_.cbegin();
         cit != backhalf_.cend(); ++cit) {
      os << *cit << " ";
    }
  }
};

#endif /* minideque_h */
