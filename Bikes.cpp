#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include "Bikes.h"
#include <string>
#include <cstring>
#include <limits>
#include <chrono>
#include <type_traits>
#include <fstream>
#include <sstream>
#include <ctime>
using namespace std;
using namespace std::chrono;
int bike::count = 0;

bike::bike(string br, string mod, int NUM, string ty, string size, double cost) :brand(br), model(mod), bikeID(NUM), type(ty), framesize(size)
, isavailable(true), rate_persec(cost), mileage(0.0) {
	count++; // increasing the number of bikes whenver we add a new one 
}

void bike::setprevioustime(double seconds)
{

	previoustime = seconds;// this will store the number of seconds of the duration before i exit the program

}

double bike::getprevioustime()
{
	return previoustime;
}

void bike::recordshutdowntime()
{
	auto now = system_clock::now();
	lastshutdowntime = system_clock::to_time_t(now);// this will convert the time (now) to seconds 
}

time_t bike::getlastshutdowntime()const
{
	return lastshutdowntime;
}

void bike::recordrestarttime()
{
	auto now = system_clock::now();
	programrestarttime = system_clock::to_time_t(now);
	if (lastshutdowntime > 0) {
		offlineperiod += difftime(programrestarttime, lastshutdowntime);
	}
}


void bike::loading_time(time_t shutdown_t)
{
	lastshutdowntime = shutdown_t;
}

void bike::setofflineperiod(double seconds)
{
	offlineperiod = seconds;
}

double bike::getofflineperiod()const
{
	return offlineperiod;
}

void bike::setavailablty(bool status)
{
	isavailable = status;
}

void bike::setnewmileage(double km)
{
	mileage += km;
}

string bike::getbrand() const
{
	return brand;
}

string bike::getmodel() const
{
	return model;
}

int bike::getbikeID() const
{
	return bikeID;
}

string bike::gettype() const
{
	return type;
}

string bike::getsize() const
{
	return framesize;
}

bool bike::getavailability() const
{
	return isavailable;
}

double bike::priceof_Renting() const
{
	return rate_persec;

}

double bike::getmileage() const
{
	return mileage;
}

void bike::startrenting()
{
	rentTime = steady_clock::now();
	setrented();
}

double bike::getduration()
{

	auto end = steady_clock::now();
	auto duration = duration_cast<seconds>(end - rentTime);
	return duration.count();// this will return the number of seconds in this duration
}

void bike::setrented()
{
	isavailable = false;
}

void bike::setreturning()
{
	isavailable = true;
}

void bike::resetrentedstate()
{
	previoustime = 0;
	offlineperiod = 0;
	lastshutdowntime = 0;
	programrestarttime = 0;
}

int bike::getTotalBikes()
{
	return count;
}
void bikesystem::addbike()
{
	bikes.push_back(createbike());

}

bool bikesystem::bikeIDexist(int number)
{
	for (auto bike : bikes) {
		if (bike->getbikeID() == number) {
			return true;
		}
	}
	return false;
}

bike* bikesystem::createbike()
{
	string brand;
	string model;
	int bikeID;
	string type;
	string framesize;
	double hourlyrate;

	// i still should add the exception handling 
	do {
		cout << " Enter a unique bikeNUM" << endl;
		cin >> bikeID;
		if (bikeIDexist(bikeID)) {
			cout << "This bike number already exists. Please enter a unique number." << endl;
		}
	} while (bikeIDexist(bikeID));

	cout << "Enter the brand" << endl;
	cin.ignore();
	getline(cin, brand);
	cout << "Enter the model" << endl;
	getline(cin, model);
	cout << "Enter the type " << endl;
	getline(cin, type);
	cout << "Enter the framesize" << endl;
	getline(cin, framesize);
	cout << " *** price ***" << endl;
	if (type == "electrical") {
		hourlyrate = 3.0;
		cout << "Electrical bike rate set to $3/hour" << endl;
	}
	else {
		hourlyrate = 1.5;
		cout << "Regular bike rate set to $1.5/hour" << endl;
	}
	// now let's create a objet's pointer to store the new bike  (new bike ) 
	bike* newbike = new bike(brand, model, bikeID, type, framesize, hourlyrate);// here we have called the constructor 
	return newbike;// returning the address of this object 
}



void bikesystem::displaybikes()const
{
	cout << "=============================================\n";
	cout << "           CURRENT BIKES INVENTORY                    \n";
	cout << "=============================================\n\n";

	if (bikes.empty()) {
		cout << " Error : There are no bikes on the system " << endl;
		return;
	}
	cout << left
		<< setw(5) << "NO."      // For the numbering
		<< setw(15) << "BRAND"
		<< setw(12) << "MODEL"
		<< setw(8) << "ID"
		<< setw(10) << "TYPE"
		<< setw(8) << "SIZE"
		<< setw(10) << "PRICE"
		<< setw(10) << "KM"
		<< setw(12) << "AVAILABILITY"
		<< endl;
	cout << "----------------------------------------------------------------------------------------" << endl;
	int num = 1; // will represent the number of the bike
	for (auto bike : bikes) {
		cout << setw(5) << num++  // prefix 
			<< setw(15) << bike->getbrand()
			<< setw(12) << bike->getmodel()
			<< setw(8) << bike->getbikeID()
			<< setw(10) << bike->gettype()
			<< setw(10) << bike->getsize()
			<< setw(10) << fixed << setprecision(2) << bike->priceof_Renting()
			<< setw(10) << bike->getmileage()
			<< setw(10) << (bike->getavailability() ? "Available" : "Rented     ")
			<< endl;
	}
	cout << "========================================================================================\n";

	bool count = count_if(bikes.begin(), bikes.end(), [](const bike* bike) { return bike->getavailability(); });  // here i used lamada function instead of decaling an isolated function

	cout << "Total bikes: " << bikes.size() << " -> " << count << " available" << endl;
	system("pause"); // this will pause the system before going to the next thing 
}

void bikesystem::printbikedeatails(const bike* a)const
{
	cout << "Brand : " << a->getbrand() << "\n"
		<< "Model : " << a->getmodel() << "\n"
		<< " ID   : " << a->getbikeID() << "\n"
		<< "Type  : " << a->gettype() << "\n"
		<< "Size  : " << a->getsize() << "\n"
		<< "Price : " << fixed << setprecision(2) << a->priceof_Renting() << "\n"
		<< "mileage : " << a->getmileage() << "\n"
		<< "availability : " << (a->getavailability() ? "Available" : "Rented     \n")
		<< endl;
}

bike* bikesystem::findbikebyID(const int ID)const
{
	for (const auto bike : bikes) { // here the pointer is constant = bike*const bike  ( constant pointer to an object ) 
		if (bike->getbikeID() == ID) {
			return bike;  // returning a pointer 
		}
	}
	return nullptr;
}

bool bikesystem::showavaiablebikes() const
{
	if (bikes.empty()) {
		cout << " Error : There are no bikes on the system " << endl;
		return false;
	}
	cout << left << setw(12) << "BRAND" << setw(12) << "MODEL" << setw(8) << "ID" << setw(10) << "TYPE" << setw(8) << "SIZE" << setw(10) << "PRICE" << setw(10) << "KM" << setw(12) << "AVAILABILITY" << endl;
	for (auto bike : bikes) {
		if (bike->getavailability()) {
			cout << setw(12) << bike->getbrand() << setw(12) << bike->getmodel() << setw(8) << bike->getbikeID()
				<< setw(10) << bike->gettype() << setw(10) << bike->getsize() << setw(10) << fixed << setprecision(2) << bike->priceof_Renting()
				<< setw(10) << bike->getmileage() << setw(10) << (bike->getavailability() ? "Available" : "Rented     ") << endl;
		}
	}
	return true;
}


void bikesystem::rentingbike(const int ID,const int userID)
{
	bike* biketorent = findbikebyID(ID);
	// checking if the bike exist or not
	if (!biketorent) {
		cout << "Error: Bike " << ID << " not found on our system!\n";
		return;
	}
	// checking for availabilty for renting
	if (!biketorent->getavailability()) {
		cout << "the bike of ID " << biketorent->getbikeID() << " is already rented " << endl;
		return;
	}

	// renting the bike 
	biketorent->startrenting();
	cout << " (: bike of ID " << ID << " is rented sucessfully :) !! Time is Running \n";
	customerBIKE[userID] = ID;
}

void bikesystem::returningbike(const int ID,const int userID)
{

	bike* biketoreturn = findbikebyID(ID);
	if (!biketoreturn) {
		cout << "Error: Bike " << ID << " not found on our system!\n";
		return;
	}
	auto it = customerBIKE.find(ID);
	if (it != customerBIKE.end()) {
		customerBIKE.erase(it);
	}
	// checking for availabilty for returning
	if (biketoreturn->getavailability()) {
		cout << "Error : bike of ID " << ID << " was not rented " << endl;
		return;
	}
	// returning the bike 
	double period = biketoreturn->getprevioustime() + biketoreturn->getduration() + biketoreturn->getofflineperiod();
	//double offline_period = biketoreturn->getreopentime() - biketoreturn->getshutdowntime();
	biketoreturn->setreturning();
	biketoreturn->resetrentedstate();
	double cost = period * biketoreturn->priceof_Renting();
	cout << " Rental Duration: " << period << " seconds\n";
	cout << " Total Cost :  " << cost << " HUF" << "\n";
	cout << " Bike returned successfully!\n";

}

void bikesystem::deletingbike(const int ID)
{// add here to not delete any rented bikes , only the available ones 
	auto iterator = find_if(bikes.begin(), bikes.end(), [ID](const bike* b) { return b->getbikeID() == ID;});  // using stl and lamada instead of calling the function getbikebyID

	if (!(iterator == bikes.end())) {
		delete* iterator;  // it will delete the pointer which the iterator is pointing to 
		bikes.erase(iterator);
		cout << "Bike " << ID << " is deleted successfully.\n";
	}
	else {
		cout << "Error : Bike of ID " << ID << " is not found\n";
	}
}
void bikesystem::searchingbyID(const int ID)const
{
	const bike* biketodisplay = findbikebyID(ID);
	if (!biketodisplay) {
		cout << "Error : This Bike ID " << ID << " is not on the system\n";
		return;
	}
	printbikedeatails(biketodisplay);
}

void bikesystem::searchingbyBRAND(const string& b)const
{
	bool found = false;
	for (auto bike : bikes) {
		if (bike->getbrand() == b) {
			found = true;
			printbikedeatails(bike);
		}
	}
	if (!found) {
		cout << "NO Matching bikes for this brand : " << b << endl;
	}
}

void bikesystem::veiwingRentals()
{
	if (customerBIKE.empty()) {
		cout << "NO active rentals\n";
		return;
	}
	cout << "\n=== ACTIVE RENTALS ===\n";
	cout << "-------------------------\n";
	cout << left << setw(15) << "Customer ID"
		<< setw(10) << "Bike ID" << "\n";
	cout << "-------------------------\n";
	for (auto it = customerBIKE.begin(); it != customerBIKE.end(); ++it) {
		cout << setw(15) << it->first
			<< setw(10) << it->second
			<< "\n";
	}
}


void bikesystem::savingbike() const
{
	ofstream out_file("bikes_data.txt");
	if (out_file.is_open()) {

		for (auto bike : bikes) {

			out_file << bike->getbrand() << "," << bike->getmodel() << "," << bike->getbikeID() << "," << bike->gettype() << "," << bike->getsize() << ","
				<< bike->priceof_Renting() << "," << bike->getmileage() << "," << bike->getavailability() << ",";
			if (!bike->getavailability()) {
				bike->recordshutdowntime();
				bike->setprevioustime(bike->getprevioustime() + bike->getduration());
				out_file << bike->getprevioustime() << ","
					<< bike->getlastshutdowntime() << ","
					<< bike->getofflineperiod();

			}
			else {
				out_file << 0 << "," << 0 << "," << 0;
			}
			out_file << endl;

		}
		out_file.close();

	}
	else {
		cout << "Error : The file cannot be opened\n";
	}

}
// in the loadingbikes function , we will just copy the information of the bikes that are stoed in the file  
void bikesystem::loadingbikes()
{
	ifstream in_file("bikes_data.txt"); // Open the file for reading
	string brand, model, type, framesize;
	int bikeID;
	double hourlyrate, mileage;
	bool availabilty;
	double secnumber;
	time_t shutdowntime;
	double offlinesecs;
	string line;
	if (in_file.is_open()) {
		while (getline(in_file, line)) {  // it will stop reading when it sees a new line
			stringstream bikestream(line);
			getline(bikestream, brand, ',');
			getline(bikestream, model, ',');
			bikestream >> bikeID;
			bikestream.ignore();  // this will let us ignore the next thing and it will be the comma in our case (,) 
			getline(bikestream, type, ',');
			getline(bikestream, framesize, ',');
			bikestream >> hourlyrate;
			bikestream.ignore();
			bikestream >> mileage;
			bikestream.ignore();
			bikestream >> availabilty;
			bikestream.ignore();
			bikestream >> secnumber;      /// this will be get the previous time 
			bikestream.ignore();
			bikestream >> shutdowntime;
			bikestream.ignore();
			bikestream >> offlinesecs;
			bike* newbike = new bike(brand, model, bikeID, type, framesize, hourlyrate);
			newbike->setavailablty(availabilty);
			newbike->setnewmileage(mileage);

			if (!newbike->getavailability()) {

				newbike->setprevioustime(secnumber);
				auto now = steady_clock::now();
				newbike->rentTime = now;
				newbike->loading_time(shutdowntime);
				newbike->setofflineperiod(offlinesecs); // this will update the previous offlineperoid 
				newbike->recordrestarttime(); // this will get the restart time and then modify the offlineperoid by adding the previous one to the new one

			}
			else {
				newbike->setprevioustime(secnumber);
			}
			bikes.push_back(newbike);
		}
	}
	else {
		cout << "Error : The file cannot be opened\n";
	}
}

bool bikesystem::confirmexit()
{
	char answer;
	cout << "Are you sure u want to exit the program : (y/n) " << endl;
	cin >> answer;
	if (answer == 'y' || answer == 'Y') {

		return true;
	}
	else {
		return false;
	}
}

bikesystem::~bikesystem()
{
	cout << "cleaning memroy leaks" << endl;
	savingbike();
	for (bike* bike : bikes) { // here auot =  bike* 
		delete bike;  // freeing the allocated memory 
	}
	bikes.clear();  // cuz when we overload the bikes again , we must have been deleted the previous bikes
}


void lookingfor_Bike(const bikesystem& search) {  // passing by refrence in c++ style 
	string brand;
	int bikeID;
	cout << "1: search by ID  2:search by Brand\n\n";
	int choice;
	cout << "Enter your choice\n";
	cin >> choice;
	switch (choice) {
	case 1:
		cout << "Enter bikeID\n";
		cin >> bikeID;
		search.searchingbyID(bikeID);
		break;
	case 2:
		cout << "Enter the Brand\n";
		cin.ignore();
		getline(cin, brand);
		search.searchingbyBRAND(brand);

	}
}


string user::getname()
{
	return name ;
}

string user::getpassword()
{
	return password;
}

int user::getuserID()
{
	return userID;
}

bool user::checkname(string username)
{
	return name==username;
}

bool user::checkpassword(string pass)const
{
	return password == pass;
}

bool user::isadmin()
{
	return false;
}

user::~user()
{
}

 void admin::showmenu(bikesystem& obj)
 {
	 enum adminoptions { display = 1, searching, add, deleting,viewing_Rentals,logingOut };  // the enumuration will improve the read ability 
	 underlying_type_t<adminoptions>choice;
	 while(true ) {
		 system("cls");
		 cout << "\n=====ADMIN MENU======\n"
			 << "1: displaying Bikes\n"
			 << "2: searching\n"
			 << "3: adding bike\n"
			 << "4: deleting bike\n"
			 << "5: view All Rentals\n"
			 << "6: loging out\n";
		 cout << "Enter choice: ";
		 while (!(cin >> choice)) {
			 cin.clear();
			 cin.ignore(numeric_limits<streamsize>::max(), '\n');
			 cout << "Invalid input, Try again!\n";
		 }
		 switch (choice) {
		 case display:
			 system("cls");
			 obj.displaybikes();		
			 break;
		 case searching:
			 system("cls");
			 cout << "SEARCHING FOR A BIKE \n";
			 lookingfor_Bike(obj);
			 system("pause");			
			 break;
		 case add:
			 system("cls");
			 cout << "=== ADDING BIKE ===\n";
			 obj.addbike();
			 //	object.savingbike();
			 system("pause");
			 break;
		 case deleting:
			 cout << "====== DELEEING BIKE ======\n";
			 cout << "Enter the ID\n";
			 int ID;
			 cin >> ID;
			 obj.deletingbike(ID);
			 //	object.savingbike();
			 system("pause");
			 break;
		 case viewing_Rentals:
			 obj.veiwingRentals();
			 system("pause");
			 break;
		 case logingOut:
			 cout << "loging out.....\n";
			 system("pause");
			 return;
		  }	  
	} 
 }

 bool admin::isadmin()
 {
	 return true;
 }
 vector<int> admin::getrentedbikesID() const
 {
	 return {};
 }
void customer::addrental(int bikeId)
{
	rentedbikesID.push_back(bikeId);
}

void customer::removeRental(int bikeID)
{
	auto it = find(rentedbikesID.begin(), rentedbikesID.end(), bikeID);
	if (it != rentedbikesID.end()) {
		rentedbikesID.erase(it);
	}
}

void customer::showmenu(bikesystem& object)
{
	enum custmoeroptions { renting = 1, returning, viewingmyBike,logingOut };  // the enumuration will improve the read ability 
	underlying_type_t<custmoeroptions>input;
	while (true) {
		cout << "\n===== CUSTOMER MENU =====\n"
			<< "1. Rent a Bike\n"
			<< "2. Return a Bike\n"
			<< "3. View my bike\n"
			<< "4. Logout\n";
			cout << "choose an operation\n";
			while (!(cin >> input)) {  
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Invalid input, Try again!\n";
			}
			switch (input) {
			case renting:
				system("cls");
				cout << "=== RENTING  BIKE ===\n";
				cout << "\n Available Bikes\n";
				if (!object.showavaiablebikes()) {
					break;
				}
				int bikeID;
				cout << "\nEnter the bikeID to Rent : \n";
				cin >> bikeID;
				object.rentingbike(bikeID,userID);
				addrental(bikeID);
			//	object.savingbike();
				system("pause");
				break;
			case returning:
				system("cls");
				cout << "=== RETURNING BIKE ===\n";
				cout << "Enter the bikeID for Returning\n";
				cin >> bikeID;
				if (find(rentedbikesID.begin(), rentedbikesID.end(), bikeID) != rentedbikesID.end()) {
					object.returningbike(bikeID, userID);
					removeRental(bikeID);
					//object.savingbike();
				}
				else {
					cout << "YOU Didnot rent any bikes\n";
					
				}
				system("pause");
				break;
			case viewingmyBike:
				system("cls");
				if (rentedbikesID.empty()) {
					cout << "\nYou didn't rent any Bikes\n";
				}
				else {
					for (int ID : rentedbikesID) {
						object.searchingbyID(ID);
					}
				}
				system("pause");
				break;
			case logingOut:
				cout << "loging out....\n";
				return;
		}
	}
	
}

vector<int> customer::getrentedbikesID() const
{
	return rentedbikesID;
}




 /////////////////////////////////////////////////////////////////////////////

customer* usermanagement::registercustomer(string name, string password, int ID)
{
	for (auto u : users) {
		if (u->getuserID() == ID) {
			return nullptr;
		}
	}
	customer* newcustomer = new customer(name, password, ID);
	users.push_back(newcustomer);
	return newcustomer;
}

admin* usermanagement::registerAdmin(string name, string pass, int ID)
{
	admin* newadmin = new admin(name, pass, ID);
	users.push_back(newadmin);
	return newadmin;
}

user* usermanagement::login(string name, string password, int ID)
{
	for (auto u : users) {
		if (u->getuserID() == ID&&u->checkpassword(password)&&u->checkname(name)){
			return  u; // this will return  admin object or customer object depending on the data 
		}
	}
	return nullptr;
}

void usermanagement::saveusers() const
{
	ofstream out_file("users_data.txt");
	if (out_file.is_open()) {
		for (auto user : users) {
			if (user->isadmin()) 
				continue;
			out_file << user->getname() << "," << user->getpassword() << "," << user->getuserID() << "," << "customer" << ",";
			vector <int>rentedbikesID = user->getrentedbikesID();
			for (auto ID : rentedbikesID) {
				out_file << ID << ",";
			}
			out_file << endl;
		}
		
		out_file.close();
	}
	else {
		cout << "Error: the file cannot be opened";
	}
}

void usermanagement::loadingusers()
{
	ifstream in_file("users_data.txt");
	string name, password, type;
	int userID, bikeID;
	string line;
	//customer* newcustomer;
	if (in_file.is_open()) {
		while (getline(in_file, line)) {  /// it will keep loading till it finds a new line and go to the next line and so on tell we finsih all the lines
			stringstream userstream(line);
			getline(userstream, name, ',');
			getline(userstream, password, ',');
			userstream >> userID;
			userstream.ignore(); // will ignore the , 
			getline(userstream, type, ',');

			if (type == "customer") {
				customer* newcustomer = registercustomer(name, password, userID);

				while (userstream >> bikeID) {
					newcustomer->addrental(bikeID);// to load the bikes and then link them with the customer 
					userstream.ignore();
				}
			}
		}
		in_file.close();
	}
	else {
		cout << "the file cannot be opened\n";
	}

} 

usermanagement::~usermanagement()
{
	cout << "good bye";
	saveusers();
	for (auto user : users) {
		delete user;
	}
	users.clear();

}

void showusermenu() {
	system("cls");
	cout << "\n===== BIKE RENTAL SYSTEM =====\n"
		<< "1. Login\n"
		<< "2. Register as Customer\n"
		<< "3. Exit\n";

}
enum menuoptions2{login =1,Registering,Exiting};
underlying_type_t<menuoptions2>inputchoice;

void handlemenu2() {
	usermanagement manger;
	bikesystem object;
	object.loadingbikes();
	manger.loadingusers();
	manger.registerAdmin("mahmoud", "medo", 562005);
	showusermenu();
	while (true) {
		string username, password; 
		int userID;
		cout << "Enter the choice you want " << endl;
		while (!(cin >> inputchoice)) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid input, Try again!\n";
		}
		switch (inputchoice) {
		case login:
		{
			system("cls");
			cout << "Username : ";
			cin >> username;
			cout << "password : ";
			cin >> password;
			cout << "Your ID : ";
			cin >> userID;

			user* userobj = manger.login(username, password, userID);

			if (userobj) {
				cout << "loging is successful\n";
				// here i will add admin and the customer menu 
				userobj->showmenu(object);
			}
			else {
				cout << "Invalid data\n";
				system("pause");
			}
			break;
		}
		case Registering:
		{
			system("cls");
			cout << "Enter a unique username : ";
			cin >> username;
			cout << "Enter a password: ";
			cin >> password;
			cout << "Enter an ID: ";
			cin >> userID;
			user* newcustomer = manger.registercustomer(username, password, userID);
			if (newcustomer) {
				cout << "Registration is successful\n";
			}
			else {
				cout << "UserID already exists\n";
			}
			system("pause");
			break;
		}
		case Exiting:
			cout << "=== EXITING ===\n";
			if (object.confirmexit())
				return;
			break;
		}
		showusermenu();
	}
}


