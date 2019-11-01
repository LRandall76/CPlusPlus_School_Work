//By James and Levi ©2018
#pragma once

#include <string>

using std::string;

class Webpage { //STATUS::PASSES 6/6 TESTS
private:
	string url;
	time_t time;


public:
	Webpage() { url = " "; time = time_t(NULL); }	//Default Contstructor
	Webpage(const string& tabUrl, const time_t& timeVisited) { // Initialized Constructor
		url = tabUrl;	
		time = timeVisited; 
	} 

	Webpage& operator=(Webpage other) { //Overloaded Operator		
		//CONST causes error so I (Levi) removed it, it used to be const Webpage& other.
		this->setTime(other.getTime());
		this->setUrl(other.getUrl());

		return(*this);
	} 
	~Webpage() {} //destructor 


	void setUrl(const string& newurl) { url = newurl; }
	void setTime(const time_t& newtime) { time = newtime; }
	string getUrl() { return url; }
	time_t getTime() { return time; }

};
