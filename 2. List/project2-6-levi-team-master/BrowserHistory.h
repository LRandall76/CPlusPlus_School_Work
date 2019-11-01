// By James and Levi ©2018
#pragma once

#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <stdexcept>
#include "Webpage.h"

using std::string;
using std::logic_error;


class BrowserHistory { //STATUS::PASSES 26/26 TESTS
private:
	std::list<Webpage> navHistory;			//USED FOR ACTUALLY AUGMENTING LIST. DELETION!
	std::list<Webpage>::iterator navPos;	//USED TO IDENTIFY CURRENT WEBSITE
	std::list<Webpage> sitesVisited;		//USED FOR HOLDING ALL SITES BEEN TO. NO DELETION!
	//int numSites;							//Not used.


public:
	BrowserHistory() {} //done 
	void readHistory(string fileName); //done 
	size_t getNavSize() { return navHistory.size(); } //done 
	std::list<Webpage> getSitesVisited() { return sitesVisited; } //done 

	void visitSite(Webpage newSite) {

		if (navHistory.size()) {

			while (navPos->getUrl() != navHistory.back().getUrl() || navPos->getTime() != navHistory.back().getTime()) {
				navHistory.pop_back();
			}		//Originally I (Levi) was comparing both elements directly using != to see if the iterator matched the 
				    //last cell of the list however that was failing and I couldn't find out why. This works.
		}

		navHistory.push_back(newSite);
		sitesVisited.push_back(newSite);

		if (navHistory.size() == 1) { 
			navPos = navHistory.begin(); 
		
		} else { navPos++;}

	}

	string back() {

		if (sitesVisited.size()) { navPos--; }
		else { 
			throw logic_error("invalid argument"); 
		}

		return navPos->getUrl();

	}

	string forward() {

		if (sitesVisited.size()) { navPos++; }
		else { 
			throw logic_error("invalid argument"); 
		}

		return navPos->getUrl();

	}

	string getUrl() {
		if (navHistory.empty()) {
			throw logic_error("invalid argument"); 
		}

		return navPos->getUrl();
	}

};


void BrowserHistory::readHistory(string fileName) {
	string temp;
	int newTime;
	Webpage newSite;
	std::ifstream inFile(fileName.c_str());


	while (inFile >> temp) {
		if (temp == "new") {
			inFile >> temp >> newTime;
			newSite = Webpage(temp, time_t(newTime));
			visitSite(newSite);
		}
		else if (temp == "back") {
			back();
		}
		else if (temp == "forward") {
			forward();
		}
		else {
			throw logic_error("invalid command");
		}
	}
}
