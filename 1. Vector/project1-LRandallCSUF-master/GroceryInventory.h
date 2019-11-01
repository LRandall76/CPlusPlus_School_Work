#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include "GroceryItem.h"
using namespace std;

class GroceryInventory {                             // Contains CLASS GroceryItem as well as Tax Rates
	//friend class GroceryItem;
private:
	vector<GroceryItem> _inventory;					//Don't Get Confused this IS the GroceryItem class
	float _taxRate;

public:
	//Default Constructor
	GroceryInventory();
	//Copy Constructor
	GroceryInventory(const GroceryInventory &newinventory);

	//Search for Entry
	GroceryItem& getEntry(const string& name);


	//Member Functions
	void addEntry(const string& name, const int& quantity, const float& price, const bool& taxyn);
	float getTaxRate() const;
	void setTaxRate(const float& newtax);

	//File Adder
	void createListFromFile(const string&);
	//File Adder ^

	//Calculations
	float calculateUnitRevenue() const;
	float calculateTaxRevenue() const;
	float calculateTotalRevenue() const;


	//Operator Overloader
	GroceryInventory& operator=( GroceryInventory& _inventory);
};

//Constructors
GroceryInventory::GroceryInventory(){}																		
GroceryInventory::GroceryInventory(const GroceryInventory& oldinventory) {
	
	_inventory = oldinventory._inventory;
	this->setTaxRate( oldinventory.getTaxRate());


}				

//Search for Entry
GroceryItem& GroceryInventory::getEntry(const string& name) {
	int breaknumber = 0;
	exception e;
	for (int i = 0; i < _inventory.size() ; i++)
	{
		breaknumber = i;
		
		if (name == _inventory[i].getName())
		{
			

			break;
		}
	
		else if (breaknumber == _inventory.size() - 1)
		{

			throw e;				//Required Exception to pass test

		

		}
		
	}

	

	return  _inventory[breaknumber];
}

//Member Functions
void GroceryInventory::addEntry(const string& name, const int& quantity, const float& price, const bool& taxyn ) {
	
	_inventory.resize(_inventory.size() + 1);			
	

	//cout << newsize;
	_inventory[_inventory.size() - 1].setName(name);							//Encoutered an out of range bug here which broke the program... note to self arrays start at 0!!!
	_inventory[_inventory.size() - 1].setQuantity(quantity);
	_inventory[_inventory.size() - 1].setUnitPrice(price);
	_inventory[_inventory.size() - 1].setTaxable(taxyn);
}
float GroceryInventory::getTaxRate() const { return _taxRate; }															///DONE
void GroceryInventory::setTaxRate(const float& newtax) { _taxRate = newtax; }											///DONE

//////DON'T TOUCH --- File Adder Thing
void GroceryInventory::createListFromFile(const string& filename) {
	ifstream input_file(filename);

	//input_file.open(filename);														//I pasted the .txt file in all of the project files because this function couldn't find the file


	

	if (input_file.is_open()) {
		cout << "Successfully opened file " << filename << endl;
		string name;
		int quantity;
		float unit_price;
		bool taxable;
		while (input_file >> name >> quantity >> unit_price >> taxable) {
			addEntry(name, quantity, unit_price, taxable);
		}
		input_file.close();
	}


	else {
		throw invalid_argument("Could not open file " + filename);
	}
}
//////DON'T TOUCH

//Calculations
float GroceryInventory::calculateUnitRevenue() const {
	float all=0;
	

	
	for (int i = 0; i < _inventory.size(); i++) {
		all += _inventory[i].getQuantity() * _inventory[i].getUnitPrice();
	}


	return all;
}
float GroceryInventory::calculateTaxRevenue() const {
	float all=0;
	float percentagething = .01;			//For some reason the test will only pass if i have a variable multiplied by tax rate instead of the number .01 must be some weird truncation error
	for (int i = 0; i < _inventory.size(); i++) {

		if (_inventory[i].isTaxable())
		{
			all += (_inventory[i].getQuantity() * _inventory[i].getUnitPrice() * (percentagething*_taxRate)); 
		}

	}


	return all;
}
float GroceryInventory::calculateTotalRevenue() const {
	float all=0;
	float all2 = 0;
	float percentagething = .01;			//For some reason the test will only pass if i have a variable multiplied by tax rate instead of the number .01 must be some weird truncation error

	for (int i = 0; i < _inventory.size(); i++) {
		all += _inventory[i].getQuantity() * _inventory[i].getUnitPrice();
	}
																				
		for (int i = 0; i < _inventory.size(); i++) {								//Adding taxes
			if (_inventory[i].isTaxable()) {
				all2 += _inventory[i].getQuantity() * _inventory[i].getUnitPrice() * ( percentagething *_taxRate);
			}

		}


		all = all + all2;					//I had to do this because the truncation was breaking the answer
		
	return all;
}		//Truncation Error Fixed

//Operator Overloader
GroceryInventory& GroceryInventory::operator=( GroceryInventory& oldinventory) {

	this->_inventory.resize(oldinventory._inventory.size());

	for (int i = 0; i < _inventory.size(); i++)
	{
		this->setTaxRate(oldinventory.getTaxRate());
		this->_inventory[i].setName(oldinventory._inventory[i].getName());
		this->_inventory[i].setQuantity(oldinventory._inventory[i].getQuantity());
		this->_inventory[i].setUnitPrice(oldinventory._inventory[i].getUnitPrice());
		this->_inventory[i].setTaxable(oldinventory._inventory[i].isTaxable());
		   
	}


	return (*this);



	//Old setup that did the reverse of what I wanted
//newinventory._inventory.resize(_inventory.size());

/*
		newinventory.setTaxRate(this->getTaxRate());
		newinventory._inventory[i].setName( _inventory[i].getName());
		newinventory._inventory[i].setQuantity( _inventory[i].getQuantity());
		newinventory._inventory[i].setUnitPrice( _inventory[i].getUnitPrice());
		newinventory._inventory[i].setTaxable( _inventory[i].isTaxable());
		   */
}




