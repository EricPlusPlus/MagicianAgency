#include <limits>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <Windows.h>
#include <direct.h>
#include <list>
#include <vector>
#include <string>
#include <map>
using namespace std;

class Reservation { // use information from the reservation objects  
private:
	string customer;
	string magician;
	string date;
public:
	Reservation();
	Reservation(string customer, string date, string magician);

	//Mutator functions
	void setMagician(string magician);
	void setCustomer(string customer);
	void setHoliday(string holiday);
	void clearMagician();

	//Accessor functions
	string getHoliday();
	string getCustomerName();
	string getMagicianName();

};
Reservation::Reservation(string customer, string date, string magician) {
	this->customer = customer;
	this->date = date;
	this->magician = magician;
}
void Reservation::setMagician(string magician) { // change the magician instance variable when a new magician is added or when a reservation needs to be rescheduled
	this->magician = magician;
}
void Reservation::setHoliday(string holiday) {
	this->date = holiday;
}
void Reservation::setCustomer(string customer) {
	this->customer = customer;
}
void Reservation::clearMagician() { // clear the magician instance variable when a magician is removed 
	magician.clear();
}
string Reservation::getHoliday() {
	return date;
}
string Reservation::getCustomerName() {
	return customer;
}
string Reservation::getMagicianName() {
	return magician;
}


//Containers
map<string, vector<string>> lookupByMagician; // dates available for each magician. i.e. Magician "Bob" is available on Christmas, Halloween, etc...
map<string, vector<string>> lookupByDate; // magicians available for each holiday i.e. On Christmas, the magicians "Bob", "John", etc... are available

vector<string> v_magicianNames;  // list of all the magicians in the program. 
vector<string> v_holidays; // list of all holidays in the program. 

list<Reservation> waitingList; // list of reservations waiting to be booked.
list<Reservation> reservations; // list of active reservations

string PROJECTNAME; //global string containing the name of the current project, this will change when users save, load and use sample projects


//Manipulation functions. Manipulate our containers
void schedule();
void addToReservations(string customer, string holiday, string magician);
void addToWaitingList(string customer, string holiday);
void cancelReservation();
void checkWaitingList(string holiday);
void removeMagician();
void updateReservations(string magicianName);
void addMagician();
void updateWaitingList(string magician);
void status();

//logic functions
bool doesHolidayExist(string date);
bool isHolidayAvailable(string lookupHoliday);
bool doesMagicianExist(string magicianName);

// display functions
void displayWelcomeScreen(); // displays choices for user
void viewReservationsByHoliday(string holiday);// creates a temporary list for a specific holiday. Organizes the list in alphabetical order by customer name.
void viewReservationsByMagician(string magicianName);
void viewReservationsOnWaitlist(string holiday);

// IO functions
void saveCurrentSettings();
void saveReservationList();
void saveWaitingList();
void saveAllHolidays();
void saveAllMagicians();
void saveLookupByMagicianMap();
void saveLookupByHolidayMap();
void saveReport(string reportType, string MagicanOrHoliday, string **p_p_report, int rows);

string loadSettings(); // returns name of the loaded project, so we can overwrite the project during save
void loadMagicians();
void loadReservationList();
void loadWaitingList();
void loadHolidays();
void loadLookupByHolidayMap();
void loadLookupByMagicianMap();

//custom Settings.
string createNewProject(); // returns name of the new project
void createDirectory(string folder);
void createSampleData(); 
void createMagicians();
void createHolidays();
void createLookUpByMagicianMap(); // these 2 functions should only be called ONCE AND ONLY ONCE. 
void createLookUpByHolidayMap(); // They are used to create the lookup maps
void searchCustomer();
void clear(); // clear the screen
void clearAllLists(); // clear all containers before loading another project


int main() {
	string userInput;
	string filename;
	createDirectory("MySampleProject");

	cout << "Welcome!" << endl;
	cout << "Type 'New' to create a new project." << endl;
	cout << "Type 'Load' to load an existing project" << endl;
	cout << "Type 'Sample' to view a sample project." << endl;

	do {
		cout << "Enter one of the options or type 'Exit' to quit." << endl;
		getline(cin, userInput);

		if (userInput == "Sample") {
			createSampleData();
			PROJECTNAME = "MySampleProject";
		}
		if (userInput == "New") {
			PROJECTNAME = createNewProject();
		}
		if (userInput == "Load") {
			PROJECTNAME = loadSettings();
		}
	} while (userInput != "New" && userInput != "Load" && userInput != "Sample" && userInput != "Exit");
	system("pause");
	system("cls");
	
	while (userInput != "Exit") {
		displayWelcomeScreen();
		cout << "Which action would you like to take?" << endl;
		getline(cin, userInput);

		if (userInput == "Schedule") {
			schedule();
			system("pause");
			clear();
		}
		else if (userInput == "Cancel") {
			cancelReservation();
			system("pause");
			clear();
		}
		else if (userInput == "Sign up") {
			addMagician();
			system("pause");
			clear();

		}
		else if (userInput == "Dropout") {
			removeMagician();
			system("pause");
			clear();
		}
		else if (userInput == "Exit") {
			saveCurrentSettings();
			system("pause");
			clear();
			break;
		}
		else if (userInput == "Status") {
			status();
			system("pause");
			clear();
		}
		else if (userInput == "Save") {
			saveCurrentSettings();
			system("pause");
			clear();
		}
		else if (userInput == "Load") {
			clearAllLists();
			PROJECTNAME.clear();

			PROJECTNAME = loadSettings();
			system("pause");
			clear();
		}
		else {
			cout << "Input not recognized" << endl;
			clear();
		}
	}
	return 0;
}

bool doesHolidayExist(string date) {
	vector<string>::iterator it = find(v_holidays.begin(), v_holidays.end(), date); // searches for holiday using the private variable 'date' as a parameter
	if (it == v_holidays.end()) { // if search criteria cannot be found iterator equals .end() and returns false
		return false;
	}
	return true;
}
bool doesMagicianExist(string magicianName) {
	vector<string>::iterator it_magicianName;
	it_magicianName = find(v_magicianNames.begin(), v_magicianNames.end(), magicianName);

	if (it_magicianName == v_magicianNames.end()) {
		return false;
	}
	return true;
}
bool isHolidayAvailable(string lookupHoliday) {
	if (lookupByDate[lookupHoliday].size() != 0) { // does the user input match one of the keys? if the user enters an invalid string, no magican name will exist and .length() = 0
		cout << "Good news! There are " << lookupByDate[lookupHoliday].size() << " magicians available." << endl;
		return true;
	}
	else {
		return false;
	}
}

void schedule() {
	/* Prompt the customer for the holiday they are looking for. Check if the holiday they entered is valid. Once they enter a valid holiday, check to see
	magicians are available for that date. If there are magicians available, prompt the user to confirm. Once confirmed, remove the magician and date from respective maps.
	Then create a reservation object.

	Basically, we check a bunch of conditions before booking the reservation. We don't want to book reservations with magicans or dates that don't exist, nor do we want to book a
	reservation and then delete it because no magicians are available.
	*/
	string customer_name, date, confirm;

	cout << "Which holiday are you trying to book for? ";
	getline(cin, date);

	while (doesHolidayExist(date) == false) { // check if input is valid, if invalid, force user to re-enter
		cout << "Invalid holiday, please re-enter the holiday are you looking for: ";
		getline(cin, date);
	}

	if (isHolidayAvailable(date)) { // check if the requested holiday is available
		string magicianName = lookupByDate[date][0]; // magicianName is the name of the first magician available;

		cout << "You will be booked with: " << magicianName << " for " << date << endl;
		cout << "Please enter a name for the reservation: ";
		getline(cin, customer_name);
		cout << "Type 'confirm' to confirm the reservation for " << customer_name << " with the magician " << magicianName << " on " << date << " or type 'cancel' to cancel the reservation. " << endl;
		getline(cin, confirm);

		while (confirm != "confirm" && confirm != "cancel") {
			cout << "Confirmation failed, you didn't enter the correct string." << endl;
			cout << "Please type 'confirm' to confirm your reservation or 'cancel' to cancel your reservation." << endl; // note the reservation hasn't been booked yet, 
			getline(cin, confirm);

		}
		if (confirm == "confirm") {
			addToReservations(customer_name, date, magicianName);
			cout << "Reservation Complete!" << endl;
		}
	}
	else { // if its not available, prompt user to put their reservation on waiting list. 
		cout << "The holiday you are trying to book for is currently full." << " We can place you on the waiting list for " << date << "." << endl;
		cout << "Type 'yes' to be placed onto the waiting list, or type 'cancel' to cancel the reservation: ";
		getline(cin, confirm);
		while (confirm != "yes" && confirm != "cancel") {
			cout << "Please enter a valid string." << endl;
			getline(cin, confirm);
		}
		if (confirm == "yes") {
			cout << "Please enter a name for the reservation: ";
			getline(cin, customer_name);

			addToWaitingList(customer_name, date);

			cout << "Reservation Complete!" << endl;
		}

	}
}
void addToReservations(string customer, string holiday, string magician) {
	Reservation temp_reservation(customer, holiday, magician);

	vector<string>::iterator it_date = find(lookupByMagician[magician].begin(), lookupByMagician[magician].end(), holiday);
	vector<string>::iterator it_magician = find(lookupByDate[holiday].begin(), lookupByDate[holiday].end(), magician);

	lookupByDate[holiday].erase(it_magician); // remove magician from lookupByDate
	lookupByMagician[magician].erase(it_date); //remove holiday 

	reservations.push_back(temp_reservation);
}
void addToWaitingList(string customer, string holiday) {
	Reservation temp_reservation(customer, holiday, "");

	waitingList.push_back(temp_reservation);
}
void cancelReservation() { // this isn't finished,i need to touch it up, we need to delete reservations on the waiting list
	string customer_name, holiday, userInput;
	bool  doesCustomerExist = false;

	cout << "Please enter the name reservation is under and the holiday." << endl;
	cout << "Name: ";
	getline(cin, customer_name);

	while (doesCustomerExist == false) { // this could be its own function and made reading the info below easier. Maybe ill fix this later. 2/13/16
		for (list<Reservation>::iterator it = reservations.begin(); it != reservations.end(); ++it) { // loop through all the objects in reservations
			if (it->getCustomerName() == customer_name) { // does the customer name match a customer name in the list of reservations?	
				doesCustomerExist = true; // if they match, change status to true to exit the initial while loop

				cout << "Holiday: "; // Have the customer enter the holiday to verify they made the reservation; 
				getline(cin, holiday);

				while (holiday != it->getHoliday() && holiday != "cancel") { // if the holiday entered is incorrect, force them to enter it correctly or quit
					cout << "The holiday you entered doesn't match the holiday on the reservation." << endl;
					cout << "Please re-enter the holiday or type 'cancel' to go back: ";
					getline(cin, holiday);
				}

				if (it->getHoliday() == holiday) { // if the customer name and holiday match the reservation info, then delete the reservation.
					cout << "Are you sure you want to delete the reservation for " << customer_name << " on " << holiday << "?" << endl;
					cout << "Type 'yes' to delete the reservation or type 'cancel' to return home.";
					getline(cin, userInput);

					while (userInput != "yes" && userInput != "cancel") {
						cout << "Input not recognized." << endl;
						getline(cin, userInput);
					}

					if (userInput == "yes") {
						lookupByDate[holiday].push_back(it->getMagicianName()); // add the holiday to the magicians availability.
						lookupByMagician[it->getMagicianName()].push_back(holiday);

						reservations.erase(it); // delete the Reservation object from the list of reservations
						cout << "Your reservation has been deleted." << endl;
						checkWaitingList(holiday);
					}
					break;
				}
			}
		}
		if (doesCustomerExist); // if the customer name was found, we can exit the while loop and quit.
			else { // otherwise the customer name was not found, and the user will be prompted to re-enter it.
				cout << "The customer name you entered does not exist. Please re-enter the name or type 'cancel' to return home: ";
				getline(cin, customer_name);
				if (customer_name == "cancel") {
					doesCustomerExist = true; // set this value to true to avoid an infinte while loop
				}
			}
	}
}
void checkWaitingList(string holiday) { // "Someone just deleted a reservation for ____ holiday, lets check the waiting list to see if anyone is waiting for that same holiday to open up"
	string customer_name;

	for (list<Reservation>::iterator it = waitingList.begin(); it != waitingList.end(); ++it) { // loop through all the objects in the waiting list
		if (it->getHoliday() == holiday) { // does the holiday in the reservation match the newly available holiday?

			// since people on the waiting list don't have an assigned magician, we need to assign them the magician.

			string magicianName = *lookupByDate[holiday].begin(); // assign them to the first magician in the list.
			cout << "The person on the waitlist is being assigned to magician: " << magicianName << endl;
			it->setMagician(magicianName);
			it->getCustomerName() = customer_name;

			Reservation temp = *it;
			reservations.push_back(temp);

			vector<string>::iterator it_date = find(lookupByMagician[magicianName].begin(), lookupByMagician[magicianName].end(), holiday);


			lookupByMagician[magicianName].erase(it_date);

			lookupByDate[holiday].erase(lookupByDate[holiday].begin()); //there should only be 1 magician in lookupByDate[holiday], so we can just delete the first magician.
																		//cout << "After deleting the magician, the size should be 0. The size is: " << lookupByDate[holiday].size() << endl;
																		// then delete the object from the waiting list. 
			waitingList.erase(it);

			break;
		}
	}
}
void displayWelcomeScreen() {
	cout << "Type 'Schedule' to schedule a reservation for a holiday." << endl;
	cout << "Type 'Cancel' to cancel an existing reservation." << endl;
	cout << "Type 'Sign up' to add another magician." << endl;
	cout << "Type 'Dropout' to remove a magician." << endl;
	cout << "Type 'Status' to find out information for a specific magician or holiday." << endl;
	cout << "Type 'Save' to save your current project." << endl;
	cout << "Type 'Load' to load an additional project." << endl;
	cout << "Type 'Exit' to leave and save your information." << endl;
	cout << '\n';

	cout << "Choose a holiday from the following: " << endl;
	for (int i = 0; i < v_holidays.size(); i++) {
		cout << v_holidays[i] << endl;
	}
}
void createLookUpByMagicianMap() {
	for (int i = 0; i < v_magicianNames.size(); i++) {
		lookupByMagician[v_magicianNames[i]] = v_holidays; // create a key with the name of the magician and assign its value as a vector of holidays
	}
}
void createLookUpByHolidayMap() {
	for (int i = 0; i < v_holidays.size(); i++) {
		lookupByDate[v_holidays[i]] = v_magicianNames;
	}
}
void status() {
	string userInput;
	cout << "Active Reservations: " << reservations.size() << endl;
	cout << "Reservations on waiting list: " << waitingList.size() << endl;

	cout << "Type 'Magician' to view the schedule of a specific magician." << endl;
	cout << "Type 'Holiday' to view reservations for a specific holiday." << endl;
	cout << "Type 'Waitlist' to see the waitlist for a specific holiday." << endl;
	cout << "Type 'Search' to search for a reservation by customer." << endl;
	cout << "Or type 'cancel' to go back." << endl;
	getline(cin, userInput);

	while (userInput != "Magician" && userInput != "Holiday" && userInput != "cancel" && userInput != "Waitlist" && userInput != "Search") {
		cout << "Please type 'Magician', 'Holiday', 'Waitlist' or 'cancel' to return home: ";
		getline(cin, userInput);
	}
	if (userInput == "Magician") {
		cout << "Please enter the name of the magician you'd like to look up: ";
		getline(cin, userInput);

		while (doesMagicianExist(userInput) == false) {
			cout << "The magician name you entered does not exist, please re-enter the name: " << endl;
			getline(cin, userInput);
			if (userInput == "cancel") {
				break;
			}
		}
		viewReservationsByMagician(userInput);
	}
	else if (userInput == "Holiday") {
		cout << "Please enter the name of the holiday you'd like to look up: ";
		getline(cin, userInput);

		while (doesHolidayExist(userInput) == false) {
			cout << "The holiday name you entered does not exist, please re-enter the holiday: " << endl;
			getline(cin, userInput);
			if (userInput == "cancel") {
				break;
			}
		}
		viewReservationsByHoliday(userInput);
	}
	else if (userInput == "Waitlist") {
		cout << "Please enter the name of the holiday you'd like to look up: ";
		getline(cin, userInput);

		while (doesHolidayExist(userInput) == false) {
			cout << "The holiday name you entered does not exist, please re-enter the holiday: " << endl;
			getline(cin, userInput);
			if (userInput == "cancel") {
				break;
			}
		}
		viewReservationsOnWaitlist(userInput);
	}
	else if (userInput == "Search") {
		searchCustomer();
	}
}
void addMagician() {
	string magicianName;
	
	cout << "Please enter the name of the magician who is signing up: ";
	getline(cin, magicianName);
	while (magicianName.length() > 19) {
		cout << "Please enter a name under 20 characters" << endl;
		getline(cin, magicianName);
	}
	// add possible confirmation function here. "Are you sure you want to add 'xyz' to the list of magicians?"
	v_magicianNames.push_back(magicianName);

	lookupByMagician[magicianName] = v_holidays;
	for (int i = 0; i < v_holidays.size(); i++) { // add the magician's availability to each holiday 
		lookupByDate[v_holidays[i]].push_back(magicianName);
	}

	cout << "Magician Added Successfully" << endl;

	if (waitingList.size() > 0) {
		cout << "Updating waitlist." << endl;
		updateWaitingList(magicianName);
	}

	cout << magicianName << " has been added to the list of magicians." << endl;
}
void updateWaitingList(string magician) {
	vector<list<Reservation>::iterator> reservationsToMove; // a vector of iterators; keeps list of all the objects were moving 

	for (int i = 0; i < v_holidays.size(); i++) {
		bool isUpdated = false; // makes sure one and ONLY one, reservation per holiday gets updated. If 3 people are on the waitlist for Easter, we only want to reschedule the first guy, not all 3

		for (list<Reservation>::iterator it = waitingList.begin(); it != waitingList.end(); ++it) {
			if (isUpdated == false) {
				if (it->getHoliday() == v_holidays[i]) {
					it->setMagician(magician);
					reservationsToMove.push_back(it);
					isUpdated = true;

					vector<string>::iterator tit = find(lookupByMagician[magician].begin(), lookupByMagician[magician].end(), v_holidays[i]);
					lookupByMagician[magician].erase(tit);

					lookupByDate[v_holidays[i]].erase(lookupByDate[v_holidays[i]].begin());

				}
			}
		}
	}

	cout << "Moving Reservations..." << endl;

	for (int i = 0; i < reservationsToMove.size(); i++) {
		reservations.splice(reservations.end(), waitingList, reservationsToMove[i]); //move from waitinglist to reservations.
	}
	reservationsToMove.clear();
}
void removeMagician() {
	string magicianName;

	cout << "Please enter the name of the magician you'd like to remove: ";
	getline(cin, magicianName);

	while (doesMagicianExist(magicianName) == false) {
		cout << "The name you entered does not exist. Please re-enter the name or type 'cancel' to go back: ";
		getline(cin, magicianName);
		if (magicianName == "cancel") {
			break;
		}
	}
	if (doesMagicianExist(magicianName)) {
		vector<string>::iterator it_magicianName = find(v_magicianNames.begin(), v_magicianNames.end(), magicianName);

		v_magicianNames.erase(it_magicianName);

		cout << magicianName << " has been removed. " << endl;


		for (int i = 0; i < v_holidays.size(); i++) {
			vector<string>::iterator it = find(lookupByDate[v_holidays[i]].begin(), lookupByDate[v_holidays[i]].end(), magicianName);

			// The magician won't nessecarily be in every lookupByDate vector, once the magician is booked, it's removed from the list and find function would return .end()

			if (it != lookupByDate[v_holidays[i]].end()) {
				lookupByDate[v_holidays[i]].erase(it);
			}
		}
		updateReservations(magicianName);
	}
}
void updateReservations(string magicianName) {
  /*
  Lets find each reservation booked for the magician who was recently removed. We will assume no magicians available and set the magician in each reservation to NULL.
  If another magician is available for that date, we will update the reservation and move it to the list of reservations.
  */
	int numReschedules = 0; // how many reservations we successfully moved
	int numMovedToWaiting = 0; // how many reservations were moved to the waiting list. 

	vector<list<Reservation>::iterator> reservationsToMove; // a vector of iterators; keeps list of all the objects were moving 

	for (list<Reservation>::iterator it = reservations.begin(); it != reservations.end(); ++it) {
		if (it->getMagicianName() == magicianName) { // does the private member 'magician' of the current object equal the local variable magicianName?
		}
	}

	for (list<Reservation>::iterator it = reservations.begin(); it != reservations.end(); ++it) { // check every reservation and update the ones matching the magician who was removed
		if (it->getMagicianName() == magicianName) { // does the private member 'magician' of the current object equal the local variable magicianName?
			string holiday = it->getHoliday(); //check the holiday the reservation is for
			it->clearMagician(); // if it does, set the magician member equal to nothing in the object
			cout << "Attempting to reschedule " << it->getCustomerName() << "'s reservation for " << holiday << "." << endl;
			int numMagiciansAvailable = 0; // assume there are no magicians available.

			numMagiciansAvailable = lookupByDate[holiday].size(); // counts number of magicians in vector of magicians for that holiday

			if (numMagiciansAvailable > 0) { // if magicians are available, modify the reservation and update the that magicians availability 

				it->setMagician(*lookupByDate[holiday].begin()); // set the private member magician to the first element in the list of magicians available for that day
				lookupByDate[holiday].erase(lookupByDate[holiday].begin()); // remove the magician from the list of magicians available on that holiday

				numReschedules++;

				// Since we were able to find another magician, we don't need to move the reservation object to the waiting list. 
			}
			else {

				cout << "No alternate magicians available, moving " << it->getCustomerName() << " 's reservation to the waitlist" << endl;

				reservationsToMove.push_back(it); // we CANNOT move objects from reservations as we iterate through it!!!!!

				numMovedToWaiting++;
			}
		}
	}

	for (int i = 0; i < reservationsToMove.size(); i++) {
		waitingList.splice(waitingList.begin(), reservations, reservationsToMove[i]); // move from reservations to waitingList
	}

	reservationsToMove.clear();
}
void viewReservationsByHoliday(string holiday) {
	string yesno;
	int bookingsForHoliday = v_magicianNames.size() - lookupByDate[holiday].size();

	string **p_p_holidays = new string*[bookingsForHoliday]; // how many reservations are booked for the holiday?
															 // the num of reservations is equal to the total - available, if no magicians are available, they're all booked.
	int z = 0; // takes the same value as bookings for holiday, used to sequentially add values to the 2D array.

	for (list<Reservation>::iterator it = reservations.begin(); it != reservations.end(); ++it) {

		if (it->getHoliday() == holiday) {

			p_p_holidays[z] = new string[2]; // how many strings exist for reservation? 2, the customer name and magician.

			p_p_holidays[z][0] = it->getCustomerName(); // add the customer name to temp list
			p_p_holidays[z][1] = it->getMagicianName(); // add the magician to the temp list

			z++;
		}
	}
	for (int i = 0; i < z; i++) {
		for (int j = i + 1; j < z; j++) {
			if (p_p_holidays[i][0]> p_p_holidays[j][0]) {

				string temp_name = p_p_holidays[i][0];
				string temp_magician = p_p_holidays[i][1];

				p_p_holidays[i][0] = p_p_holidays[j][0];
				p_p_holidays[i][1] = p_p_holidays[j][1];

				p_p_holidays[j][0] = temp_name;
				p_p_holidays[j][1] = temp_magician;
			}
		}
	}
	cout << '\t' << "Customer: " << '\t' << '\t' << "Magician: \n" << endl;

	for (int i = 0; i < z; i++) {
		cout << '\t' << p_p_holidays[i][0] << '\t' << '\t' << p_p_holidays[i][1] << endl;
	}
	cout << "Would you like to save the report? y/n ";
	getline(cin, yesno);
	if (yesno == "y") {
		saveReport("", holiday, p_p_holidays, z);
		cout << "Report Saved!" << endl;
	}
	
	delete[] p_p_holidays;
}
void viewReservationsByMagician(string magicianName) {
	string yesno;
	int bookingsForMagician = v_holidays.size() - lookupByMagician[magicianName].size();

	string **p_p_magicians = new string*[bookingsForMagician];

	int z = 0;
	for (list<Reservation>::iterator it = reservations.begin(); it != reservations.end(); ++it) {

		if (it->getMagicianName() == magicianName) {

			p_p_magicians[z] = new string[2]; // how many strings exist for reservation? 2, the customer name and magician.

			p_p_magicians[z][0] = it->getHoliday(); // add the customer name to temp list
			p_p_magicians[z][1] = it->getCustomerName(); // add the magician to the temp list

			z++;
		}
	}
	for (int i = 0; i < z; i++) {
		for (int j = i + 1; j < z; j++) {
			if (p_p_magicians[i][0]> p_p_magicians[j][0]) {

				string temp_holiday = p_p_magicians[i][0];
				string temp_cust = p_p_magicians[i][1];

				p_p_magicians[i][0] = p_p_magicians[j][0];
				p_p_magicians[i][1] = p_p_magicians[j][1];

				p_p_magicians[j][0] = temp_holiday;
				p_p_magicians[j][1] = temp_cust;
			}
		}
	}
	cout << '\t' << "Holiday: " << '\t' << '\t' << "Customer: \n" << endl;
	for (int i = 0; i < z; i++) {
		cout << '\t' << p_p_magicians[i][0] << '\t' << '\t' << p_p_magicians[i][1] << endl;
	}

	cout << "Would you like to save the report? y/n ";
	getline(cin, yesno);
	if (yesno == "y") {
		saveReport("Magician", magicianName, p_p_magicians, z);
		cout << "Report Saved!" << endl;
	}
	delete[] p_p_magicians;
}
void viewReservationsOnWaitlist(string holiday) {
	int i = 0;
	cout << "The following customers are on the waitlist for: " << holiday << endl;
	for (list<Reservation>::iterator it = waitingList.begin(); it != waitingList.end(); ++it) {
		if (it->getHoliday() == holiday) {
			i++;
			cout << '\t' << i << ". " << '\t' << it->getCustomerName() << endl;
		}
	}
}

void saveCurrentSettings() {

	createDirectory(PROJECTNAME);

	saveReservationList();
	saveWaitingList();
	saveAllHolidays();
	saveAllMagicians();
	saveLookupByHolidayMap();
	saveLookupByMagicianMap();
}
void saveReservationList(){
	string filepath = "C:\\MagicianAgency\\" + PROJECTNAME + "\\reservations.txt";

	ofstream reservationsFile;
	reservationsFile.open(filepath);

	for (list<Reservation>::iterator it = reservations.begin(); it != reservations.end(); ++it) {
		reservationsFile << it->getCustomerName() << '\t' << it->getHoliday() << '\t' << it->getMagicianName() << '\n';
	}
	reservationsFile.close();
}
void saveWaitingList(){
	string filepath = "C:\\MagicianAgency\\" + PROJECTNAME + "\\waitinglist.txt";

	ofstream waitingListFile;
	waitingListFile.open(filepath);

	for (list<Reservation>::iterator it = waitingList.begin(); it != waitingList.end(); ++it) {
		waitingListFile << it->getCustomerName() << '\t' << it->getHoliday() << endl;
	}
	waitingListFile.close();
}
void saveAllMagicians(){
	string filepath = "C:\\MagicianAgency\\" + PROJECTNAME + "\\magicians.txt";

	ofstream magicianNames;
	magicianNames.open(filepath);

	for (int i = 0; i < v_magicianNames.size(); i++) {
		magicianNames << v_magicianNames[i] << endl;
	}

	magicianNames.close();
}
void saveAllHolidays(){
	string filepath = "C:\\MagicianAgency\\" + PROJECTNAME + "\\holidays.txt";

	ofstream holidays;
	holidays.open(filepath);

	for (int i = 0; i < v_holidays.size(); i++) {
		holidays << v_holidays[i] << endl;
	}

	holidays.close();
}
void saveLookupByHolidayMap(){
	string filepath = "C:\\MagicianAgency\\" + PROJECTNAME + "\\lookupByHoliday.txt";

	ofstream holidayMapFile;
	holidayMapFile.open(filepath);


	for (int i = 0; i < v_holidays.size(); i++) {
		holidayMapFile << v_holidays[i] << '\t'; // insert the map key, then a tab ex. 'Easter	'

		string magicians;
		magicians.clear();

		for (int j = 0; j < lookupByDate[v_holidays[i]].size(); j++) {
			magicians += lookupByDate[v_holidays[i]][j] + "|"; // create a string of the magicians delimited by a pipe ex. MagicianA|TestGuy|John|
		}

		if (lookupByDate[v_holidays[i]].size() > 0) {
			magicians.pop_back();
			holidayMapFile << magicians;
		}
		if (i != v_holidays.size() - 1) {
			holidayMapFile << '\n';
		}

	}

	holidayMapFile.close();
}
void saveLookupByMagicianMap() {
	string filepath = "C:\\MagicianAgency\\" + PROJECTNAME + "\\lookupByMagician.txt";

	ofstream magicianMapFile;
	magicianMapFile.open(filepath);
	
	for (int i = 0; i < v_magicianNames.size(); i++) {
		magicianMapFile << v_magicianNames[i] << '\t';

		string holidays;
		holidays.clear();

		for (int j = 0; j < lookupByMagician[v_magicianNames[i]].size(); j++) {
			holidays += lookupByMagician[v_magicianNames[i]][j] + "|";
		}

		if (lookupByMagician[v_magicianNames[i]].size() > 0) {
			holidays.pop_back();
			magicianMapFile << holidays;
		}
		if (i != v_magicianNames.size() - 1) {
			magicianMapFile << '\n';
		}
	}
	
	magicianMapFile.close();
}
void saveReport(string reportType, string MagicanOrHoliday, string **p_p_report, int rows) {
	string filename;
	string filepath;

	filename = reportType + '_' + MagicanOrHoliday + "_Schedule.txt";
	filepath = "C:\\MagicianAgency\\" + PROJECTNAME + "\\SavedReports\\" + filename;

	createDirectory(PROJECTNAME + "\\SavedReports");

	ofstream customReport;
	customReport.open(filepath);

	if (reportType == "Magician") { //create the appopriate headers based on report type
		customReport << "Holiday: \t" << "Customer: " << endl;
	}
	else {
		customReport << "Customer: \t" << "Magician: " << endl;
	}
	for (int i = 0; i < rows; i++) {
		customReport << p_p_report[i][0] << '\t' << p_p_report[i][1] << endl;
	}

	customReport.close();
}

string loadSettings() {
	string filepath;
	cout << "Please enter the name of your project: ";
	getline(cin, PROJECTNAME);

	filepath = "C:\\MagicianAgency\\" + PROJECTNAME;

	if ((GetFileAttributes(filepath.c_str())) == INVALID_FILE_ATTRIBUTES) {
		cout << "The project you entered does not exist. Creating a sample project..." << endl;
		clearAllLists();
		createSampleData();
		return "MySampleProject";
	}
	else {
		cout << "Loading data..." << endl;
		
		loadReservationList();
		loadWaitingList();
		loadMagicians();
		loadHolidays();
		loadLookupByHolidayMap();
		loadLookupByMagicianMap();

		cout << "Project loaded successfully!" << endl;
		return PROJECTNAME;
	}
}
void loadReservationList() {
	string filepath = "C:\\MagicianAgency\\" + PROJECTNAME + "\\reservations.txt";

	ifstream reservationListFile;
	reservationListFile.open(filepath);

	string customer, holiday, magician;
	int count = 0;

	if (reservationListFile.fail()) {
		cerr << "Error opening file." << endl;
	}
	
	while (!reservationListFile.eof()) {
		customer.clear();
		holiday.clear();
		magician.clear();
		getline(reservationListFile, customer, '\t'); // load the file using tab delimiters
		if (customer.empty()) { //detects last line in document. 
			break;
		}
		getline(reservationListFile, holiday, '\t');
		getline(reservationListFile, magician);

		Reservation temp(customer, holiday, magician);
		reservations.push_back(temp);
	}
	reservationListFile.close();
}
void loadWaitingList() {
	string filepath = "C:\\MagicianAgency\\" + PROJECTNAME + "\\waitinglist.txt";

	ifstream reservationListFile;
	reservationListFile.open(filepath);

	string customer, holiday;
	while (!reservationListFile.eof()) {
			customer.clear();
			holiday.clear();

			getline(reservationListFile, customer, '\t'); // load the file using tab delimiters
			if (customer.empty()) {
				break;
			}
			getline(reservationListFile, holiday);

			Reservation temp(customer, holiday, ""); // initialize all the members of the of instance
			waitingList.push_back(temp);
	}

	reservationListFile.close();
}
void loadMagicians() {
	string filepath = "C:\\MagicianAgency\\" + PROJECTNAME + "\\magicians.txt";

	ifstream magiciansFile;
	magiciansFile.open(filepath);

	while (!magiciansFile.eof()) {
		string magician;

		getline(magiciansFile, magician);
		if (!magician.empty()) {
			v_magicianNames.push_back(magician);
			magician.clear();
		}
	}

	magiciansFile.close();
}
void loadHolidays() {
	string filepath = "C:\\MagicianAgency\\" + PROJECTNAME + "\\holidays.txt";

	ifstream holidaysFile;
	holidaysFile.open(filepath);

	while (!holidaysFile.eof()) {
		string holiday;

		getline(holidaysFile, holiday);
		if (!holiday.empty()) {
			v_holidays.push_back(holiday);
			holiday.clear();
		}
	}
	holidaysFile.close();
}
void loadLookupByHolidayMap() {
	string filepath = "C:\\MagicianAgency\\" + PROJECTNAME + "\\lookupByHoliday.txt";

	ifstream mapFile;
	mapFile.open(filepath);

	string key, magician, s_line;

	while (!mapFile.eof()) {
		getline(mapFile, s_line); // grab the enite line from the file
		stringstream line(s_line); //convert it into a string

		getline(line, key, '\t'); // input all text until tab is found

		while (line.good()) { //for the rest of the line...
			getline(line, magician, '|'); //grab each piece of text between the delimeter
			if (!magician.empty()) { // if the magician string contains text, it's valid; add to it the vector.
				lookupByDate[key].push_back(magician); // insert each piece of text (magician) to the vector
				magician.clear();
			}
		}
	}
	mapFile.close();
}
void loadLookupByMagicianMap() {
	string filepath = "C:\\MagicianAgency\\" + PROJECTNAME + "\\lookupByMagician.txt";
	ifstream mapFile;
	mapFile.open(filepath);

	string key, holiday, s_line;

	while (!mapFile.eof()) {
		getline(mapFile, s_line);
		stringstream line(s_line);

		getline(line, key, '\t');

		while (line.good()) {
			getline(line, holiday, '|');
			if (!holiday.empty()) {
				lookupByMagician[key].push_back(holiday);
				holiday.clear();
			}
		}
	}
	mapFile.close();
}

void createDirectory(string folder) {

	string folderpath = "C:\\MagicianAgency\\"; // we can only create one new folder at a time

	if ((GetFileAttributes(folderpath.c_str())) == INVALID_FILE_ATTRIBUTES) { // create MagicianAgency folder on C Drive
		CreateDirectory(folderpath.c_str(), 0);

		cout << "A folder for the project was created on your C Drive. All files will be saved there. " << endl;
	}

	folderpath += folder; // create our custom project folder

	if ((GetFileAttributes(folderpath.c_str())) == INVALID_FILE_ATTRIBUTES) {
		CreateDirectory(folderpath.c_str(), 0);
	}
	cout << "The current directory is: " << folderpath << endl;
}
void createSampleData() {
	v_holidays = { "Christmas", "Easter", "Halloween", "Thanksgiving", "New Years", "Independance Day" };
	v_magicianNames = { "Mr. Swift", "Saul Goodman", "BlackMagic", "MrMagician" };

	createLookUpByHolidayMap();
	createLookUpByMagicianMap();

	addToReservations("Richard Synder", "Halloween", "Saul Goodman");
	addToReservations("Eric Pratt", "Halloween", "Mr. Swift");
	addToReservations("Anthony McCoy", "Christmas", "MrMagician");
	addToReservations("Mariah Lanford", "Halloween", "BlackMagic");
	addToReservations("Jesse Pinkman", "Halloween", "MrMagician");
	addToReservations("Gus Fring", "New Years", "Mr. Swift");
	addToReservations("Mr White", "Thanksgiving", "Mr. Swift");
	addToReservations("Mike Erhmantrout", "Independance Day", "Mr. Swift");
	addToReservations("Skinny Pete", "New Years", "Saul Goodman");
	addToReservations("Badger", "New Years", "BlackMagic");
	addToReservations("Hank Schrader", "Christmas", "Mr. Swift");
	addToReservations("Skyler White", "Easter", "Mr. Swift");
}
string createNewProject() {

	createHolidays();
	createMagicians();
	createLookUpByHolidayMap();
	createLookUpByMagicianMap();

	cout << "Please enter a name for this project: ";
	getline(cin, PROJECTNAME);

	return PROJECTNAME;
}
void createMagicians() {
	string userInput;
	cout << "Enter some names for magicians, press enter to enter the next magician. Type 'Done' when finished." << endl;
	while (userInput != "Done") {
		getline(cin, userInput);
		if (userInput == "Done") {
			break;
		}
		v_magicianNames.push_back(userInput);
	}

	cout << v_magicianNames.size() << " magicians have been added" << endl;
}
void createHolidays() {
	string userInput;
	cout << "Enter some holidays, press enter to enter the next holiday. Type 'Done when finished." << endl;
	while (userInput != "Done") {
		getline(cin, userInput);
		if (userInput == "Done") {
			break;
		}
		v_holidays.push_back(userInput);
	}

	cout << v_holidays.size() << " holidays have been added" << endl;
}
void searchCustomer() {
	string customer;
	bool customerExist = false;
	cout << "Enter a customer name to search for their reservation: ";
	getline(cin, customer);

	for (list<Reservation>::iterator it = reservations.begin(); it != reservations.end(); ++it) {
		if (it->getCustomerName() == customer) {
			cout << "Customer: \t" << customer << endl;
			cout << "Holiday: \t" << it->getHoliday() << endl;
			cout << "Magician: \t" << it->getMagicianName() << endl;
			customerExist = true;
		}
	}
	for (list<Reservation>::iterator it = waitingList.begin(); it != waitingList.end(); ++it) {
		if (it->getCustomerName() == customer) {
			cout << "Customer: \t" << customer << endl;
			cout << "Holiday: \t" << it->getHoliday() << endl;
			cout << "* On Waiting List." << endl;
			customerExist = true;
		}
	}
	if (customerExist == false) {
		cout << "Customer: " << customer << " does not exist." << endl;
	}
}
void clear() {
	system("cls");
}
void clearAllLists() {
	lookupByDate.clear();
	lookupByMagician.clear();
	v_holidays.clear();
	v_magicianNames.clear();
	reservations.clear();
	waitingList.clear();
}
