#pragma once
#include <string>
#include <vector>
#include <chrono>
#include<map>
using namespace std;
using namespace std::chrono;
class bike {
private:
	static int count; // for the number of bikes 
	string brand;// like giant , trek  and so on 
	string model;// any model
	int bikeID;// this will represent the serial number of the bike

	string type;// types like usual bike or mountain bike or electric bike and so on 
	string framesize;// S  M  L  XL  
	bool isavailable;
	double rate_persec; // the cost for renting will differ from bike to bike 
	double mileage;  // this for the totl km we have ridden ( for tracking the bike as well ) and it's 0 for the new bike 

	//  the following are used to determine the period of renting while we closed the program 
	double previoustime = 0;
	time_t lastshutdowntime = 0;
	time_t programrestarttime = 0;
	double  offlineperiod = 0.0;

public:
	chrono::steady_clock::time_point rentTime;
	bike() {}
	// here is the constructor 
	bike(string br, string mod, int NUM, string ty, string size, double cost);
	void setprevioustime(double seconds);
	double getprevioustime();

	void recordshutdowntime();
	time_t getlastshutdowntime()const;
	void recordrestarttime();

	void loading_time(time_t shutdown_t);
	void setofflineperiod(double seconds);
	double getofflineperiod()const;

	// here is the setters methods 
	void setavailablty(bool status);
	void setnewmileage(double km);

	// here is the getters methods 
	string getbrand()const;
	string getmodel()const;
	int getbikeID()const;
	string gettype()const;
	string getsize()const;
	bool getavailability()const;
	double priceof_Renting()const;
	double getmileage()const;
	//	string last_dateforM()const;

	// timing functions   and they will be responsible for storing the time of each bike 
	void startrenting();
	double getduration();
	//we will get the status from the next two lines 
	void setrented();// here the status will change to rented
	void setreturning();// here the status will change to norented

	void resetrentedstate();
	static int getTotalBikes();// this will be for displaying the number of bikes 

};


//***************************************************************************************************///


class bikesystem {
private:
	vector<bike*> bikes;   // creating dyanamic array of pointers  of the class bike
	map<int, int >customerBIKE;
public:

	void addbike();
	bool bikeIDexist(int number);
	bike* createbike();
	void displaybikes()const;
	void printbikedeatails(const bike* a)const;
	bike* findbikebyID(const int ID)const;
	bool showavaiablebikes()const;
	void rentingbike(const int ID,const int userID=0);
	void returningbike(const int ID,const int userID=0);

	void deletingbike(const int ID);
	void searchingbyID(const int ID)const;
	void searchingbyBRAND(const string& b)const;
	void veiwingRentals();

	void savingbike()const;
	void loadingbikes();
	bool confirmexit();
	~bikesystem();

};
void lookingfor_Bike(const bikesystem& search);



class user {
protected :
	string name;
	string password;
	int userID;
public :
	user (string n , string p , int ID):name(n),password(p),userID(ID){}
	
	string getname();
	string getpassword();
	int getuserID();
	bool checkname(string username);
	bool checkpassword(string pass)const;
	virtual void showmenu(bikesystem& system) = 0;
	virtual bool isadmin();
	virtual ~user();
};

class admin :public user {
public :
	admin(string n, string pw , int ID) :user(n, pw,ID){}
	void showmenu(bikesystem& obj)override;
	bool isadmin();

};

class customer :public user {
private :
	vector <int> rentedbikesID;// this will only the store the bike's ID of the customer
public :
	customer (string n , string ps , int ID):user(n,ps,ID){}

	void addrental(int bikeId);
	void removeRental(int bikeID);
	void showmenu(bikesystem& obj)override;
};

class usermanagement {
private :
	vector <user*>users; 

public :
	customer* registercustomer(string name, string password, int ID);
	admin* registerAdmin(string name, string pass, int ID);
	user* login(string name, string password, int ID);
	void saveusers()const;
	void loadingusers();
	~usermanagement();
};

void handlemenu2();