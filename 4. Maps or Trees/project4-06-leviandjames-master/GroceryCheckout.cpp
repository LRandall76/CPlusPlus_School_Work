//Levi Randall
//James Samawi
//Project 4

#include <fstream>
#include <stdexcept>
#include <iostream>
#include "GroceryCheckout.h"
#include <string>




using namespace std;




//GroceryItem Functions 
GroceryItem* GroceryItem::getItem() { return this; }
void GroceryItem::setquantity(int quantity) { quantity_ = quantity; }
int GroceryItem::getquantity() { return quantity_; }
float GroceryItem::getprice() { return price_; }
bool GroceryItem::gettax() { return taxable_; }


//Reciept Functions----------------------------------------------
void Receipt::pushback(ReceiptItem item) { item_.push_back(item); }
void Receipt::setsubtotal(float subtotal) { subtotal_ = subtotal; }
void Receipt::settaxAmount(float tax) { taxAmount_ = tax; }					
void Receipt::settotal(float total) { total_ = total; }
float Receipt::getsubtotal() { return subtotal_; }
//Grocery Inventory-------------------------------------------------


//Get Size Function
size_t GroceryInventory::Size() { return mapofinventory.size(); }

//Set Tax Rate
void GroceryInventory::SetTaxRate(float taxRate) { taxrate_ = taxRate; }


//Creates an Element of GroceryItem to the Map
bool GroceryInventory::AddItem(const string& name, int quantity,
	float price, bool taxable)
{
	//Temporary Variable to Feed to Map
	GroceryItem TempItem(name, quantity, price, taxable);

	
	map<string, GroceryItem>::iterator ittomap;
	ittomap = mapofinventory.begin();

	//Check To See If New Element is a Duplicate

	for (ittomap = mapofinventory.begin(); ittomap != mapofinventory.end(); ittomap++)
	{if (ittomap == mapofinventory.find(name)){return false;}}



		


		//Inserting Into Map
		mapofinventory.insert(pair <string, GroceryItem>(name, TempItem));

	//Check to Validate Insertion
	if (mapofinventory[name] == TempItem)
	{
		return true;
	}
	else
	{
		return false;
	}


}

//File Read In
void GroceryInventory::CreateFromFile(string fileName)
{
    ifstream	file(fileName);
	string		name;
	float		price;
	int			quantity;
	bool		taxable;
	
	if (file.is_open())
	{
		while (true)
		{
			file >> name >> quantity >> price >> taxable;


			if (!file.fail())
			{
				AddItem(name, quantity, price, taxable);
			}
			else
			{
				break;
			}
         }
        file.close();
    }
	else
	{
	
        cout << "Could not open file." << endl;
    }
	
	return;
}




GroceryItem*	GroceryInventory::FindItem(const string& name)
{
	//Algorithm I made to search through the map for an element -- Checks for Duplicates
	map<string, GroceryItem>::iterator ittomap;
	ittomap = mapofinventory.begin();

	for (ittomap = mapofinventory.begin(); ittomap != mapofinventory.end(); ittomap++)
	{

		if (ittomap == mapofinventory.find(name))
		{

			return mapofinventory.find(name)->second.getItem();
		}

	}
	
	

	 return 0;
	
}


bool GroceryInventory::RemoveItem(const string& name)
{
	//Algorithm I made to search through the map for an element --Checks for Duplicates
	map<string, GroceryItem>::iterator ittomap;
	ittomap = mapofinventory.begin();

	for (ittomap = mapofinventory.begin(); ittomap != mapofinventory.end(); ittomap++) //For whole map
	{

		if (ittomap == mapofinventory.find(name))
		{
			mapofinventory.erase(name);		//Line That Actually Erases
			return 1;
		
		}

	}

	return 0;



}


//Searches Through Map to find the price of Items and if they are taxable then returns a class
Receipt GroceryInventory::CreateReceipt( string fileName)
{

	Receipt receipt;
	ifstream	file(fileName);
	string		name;
	float		price;
	int			quantity;
	bool		taxable;

	float taxtotal=0;
	//Make Sure it is initialized
	receipt.setsubtotal(0);


	//Load in From file	----I borrowed your read in File Code ;D
	if (file.is_open())
	{
		while (true)
		{
			file >> name;//Only Need name
			

			if (!file.fail())
			{


			


				//Fidning a Grocery Item and using it to put into a receipt
				ReceiptItem tempitem(name, mapofinventory.find(name)->second.getprice());
				receipt.pushback(tempitem);
				
				//Subtotal
				receipt.setsubtotal(mapofinventory.find(name)->second.getprice() + receipt.getsubtotal());
			

				//On Hand Adjustment for Quantity
				mapofinventory.find(name)->second.setquantity(mapofinventory.find(name)->second.getquantity() -1);


			}
			else
			{
				break;
			}


			
			if (mapofinventory.find(name)->second.gettax())
			{

				//Tax
				taxtotal = (mapofinventory.find(name)->second.getprice() * taxrate_ * .01) + taxtotal;
				
			}
		}
		file.close();
	}


	else
	{

		cout << "Could not open file." << endl;
	}
	

	//Tax Total
	receipt.settaxAmount(taxtotal);


	receipt.settotal(taxtotal + receipt.getsubtotal());

	


	return receipt;
}







