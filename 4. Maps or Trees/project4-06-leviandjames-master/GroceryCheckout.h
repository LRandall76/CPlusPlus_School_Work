//Levi Randall
//James Samawi
#ifndef GroceryCheckout_h
#define GroceryCheckout_h

#include <fstream>
#include <map>
#include <vector>

using namespace std;

struct GroceryItem		///Done Already
{
	GroceryItem() { }
	GroceryItem(const string& name, int quantity, float price, bool taxable)
	: name_(name), quantity_(quantity), price_(price), taxable_(taxable) { }
	
	string	name_;
	int		quantity_;
	float	price_;
	bool	taxable_;

	
	// Functions------------------------------------------------------------
	//Compares the two elements -- My First Attempt at Overloading by Myself 
	bool operator==(GroceryItem rhs) {


		if (this->name_ == rhs.name_)
		{
			if (this->quantity_ == rhs.quantity_)
			{
				if (this->price_== rhs.price_)
				{
					if(this->taxable_ == rhs.taxable_)
					{
						return true;
					
					}
				}
			}
		}


		return false;

	}


	GroceryItem * getItem();
	void setquantity(int quantity);
	int getquantity();

	float getprice();
	bool gettax();
};

struct ReceiptItem		///Done Already
{
	ReceiptItem() { }
	ReceiptItem(const string& name, float price)
	: name_(name), price_(price) { }
	
	string	name_;
	float	price_;


};

struct	Receipt			
{
	vector<ReceiptItem>	item_;
	float				subtotal_;
	float				taxAmount_;
	float				total_;

	void pushback(ReceiptItem item);
	void setsubtotal(float subtotal);
	void settaxAmount(float tax);
	void settotal(float total);

	float getsubtotal();
};



class GroceryInventory	
{
	public:
		
		
	




		//Functions
		bool		AddItem(const string& name, int quantity, float price, bool taxable);
		
		
		void			CreateFromFile( string fileName);

		size_t			Size();

		void			SetTaxRate(float taxRate);

		GroceryItem*	FindItem(const string& name);


		bool			RemoveItem(const string& name);
	
		Receipt			CreateReceipt( string filenName);


		
		
	private:
		



		map<string, GroceryItem> mapofinventory;

		float taxrate_;





};

#endif
