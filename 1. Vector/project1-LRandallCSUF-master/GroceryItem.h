#pragma once
#include <string>
#include<iostream>					//Added for Constructor
using namespace std;

class GroceryItem {
	friend class GroceryInventory;
private:
	string _name;
	int _quantity;
	float _unitPrice;
	bool _taxable;

	

public:
	//Constructors
	GroceryItem();
	GroceryItem(const string& newname, const int& newquantity, const float& newprice, const bool& taxyn);

	//Member Functions: Getters And Setters
	string getName() const;
	void setName(const string& newname);
	int getQuantity() const;
	void setQuantity(const int& newquantity);
	float getUnitPrice() const;
	void setUnitPrice(const float& newprice);
	bool isTaxable() const;
	void setTaxable(const bool& taxyn);
};

///Done

//Constructors
GroceryItem::GroceryItem() {}// cout << endl << "Item Class Created" << endl;						//May Cause Error
GroceryItem::GroceryItem(const string& newname, const int& newquantity, const float& newprice, const bool& taxyn) {
	_name = newname;
	_quantity =newquantity;
	_unitPrice =newprice;
	_taxable =taxyn;

	cout << endl << "Item Class Created" << endl;												//May Cause Error

}

//Member Functions
string GroceryItem::getName() const { return _name; }
void GroceryItem::setName(const string& newname) { _name = newname; }
int GroceryItem::getQuantity() const { return _quantity;}
void GroceryItem::setQuantity(const int& newquantity) { _quantity = newquantity; }
float GroceryItem::getUnitPrice() const { return _unitPrice; }
void GroceryItem::setUnitPrice(const float& newprice) { _unitPrice = newprice; }
bool GroceryItem::isTaxable() const { return _taxable; }
void GroceryItem::setTaxable(const bool& taxyn) { _taxable = taxyn; }
