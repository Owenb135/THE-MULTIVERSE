#include <iostream>
#include <thread> 
#include <chrono> 
#include <random>
using namespace std;

void start_cppge() {
    random_device rd;
mt19937 gen(rd());
	cout << "CPPGE\n \n \n MENU: \n 1) Launch\n";
    int le;
    cin >> le;
    if (le==1) {
     while (true) {
      int le1;
      cout << "\n 0) Exit \n 1) Credits \n 2) Start Game (BETA) \n";
      cin >> le1;
     if (le1 == 1) {
    cout << "\n \n \n CREDITS: \n Created by: Owen \n In dedication for becoming a better programer \n";
	this_thread::sleep_for(std::chrono::seconds(3));
	cout << "\n ----------------------------------------- \n";
}
    else if (le1 == 0)  {
return;
}
    else if (le1 == 2) {
    cout << "\n Game Launching....";
	this_thread::sleep_for(std::chrono::seconds(3));
	cout << "\n--------------------------------------------------\n";
	cout << "Tired Robot \n";
	cout << "HP: 50 \n";
	cout << "Reward: \n";
	cout << "  BANNA \n";
	cout << "Chose an option:\n";
	string idea;
	cout << "Fight\n";
	cin >> idea;
	if (idea == "Fight" || idea == "fight") {
		cout << "You Attack...\n";
		this_thread::sleep_for(std::chrono::seconds(3));
		cout << "-10 HP\n";
		cout << "Tired Robot\n";
		cout << "HP: 40\n";
		this_thread::sleep_for(std::chrono::seconds(5));
		cout << "------------------------------------------------------\n";
		cout << "Tired robot hits you...\n";
		cout << "-10 HP\n";
		cout << "You: 90 HP\n";
	}
	this_thread::sleep_for(std::chrono::seconds(4));
		cout << "-----------------------------------------------------\n";
		cout << "Chose an option:\n";
	string idea1;
	cout << "Fight\n";
	cin >> idea1;
	if (idea1 == "Fight" || idea1 == "fight") {
		cout << "You Attack...\n";
		this_thread::sleep_for(std::chrono::seconds(3));
		cout << "-10 HP\n";
		cout << "Tired Robot\n";
		cout << "HP: 30\n";
		this_thread::sleep_for(std::chrono::seconds(5));
		cout << "------------------------------------------------------\n";
		cout << "Tired robot hits you...\n";
		cout << "-20 HP\n";
		cout << "You: 70 HP\n";
	}
	this_thread::sleep_for(std::chrono::seconds(4));
		cout << "-----------------------------------------------------\n";
		cout << "Chose an option:\n";
	string idea2;
	cout << "Fight  \n";
	cin >> idea2;
	if (idea2 == "Fight" || idea2 == "fight") {
		cout << "You Attack...\n";
		this_thread::sleep_for(std::chrono::seconds(3));
		cout << "-15 HP\n";
		cout << "Tired Robot\n";
		cout << "HP: 15\n";
		this_thread::sleep_for(std::chrono::seconds(5));
		cout << "------------------------------------------------------\n";
		cout << "Tired robot hits you...\n";
		cout << "-50 HP\n";
		cout << "You: 20 HP\n";
	}
	this_thread::sleep_for(std::chrono::seconds(4));
		cout << "-----------------------------------------------------\n";
		cout << "You have a 50% chance to survive\n";
		this_thread::sleep_for(std::chrono::seconds(3));
		cout << "Chose an option:\n";
	string idea3;
	cout << "Fight  Run\n";
	cin >> idea3;
	if (idea3 == "Fight" || idea3 == "fight") {
		uniform_int_distribution<> dist(1, 10);
		int num = dist(gen);
		cout << "You Attack...\n";
		this_thread::sleep_for(std::chrono::seconds(3));
		if (num >= 5) {
			cout << "YOU SURVIVE!\n";
			cout << "Tired Robot drops Banna, Eat? (Y or N)";
			string yon;
			cin >> yon;
			if (yon == "Y" || yon == "y") {
				this_thread::sleep_for(std::chrono::seconds(5));
				cout << "Banna was rotten -10 HP";
				cout << "HP : 10";
				cout << "Thank you for playing!\n";
				this_thread::sleep_for(std::chrono::seconds(3));
			}
			else {
				cout << "Thank you for playing!\n";
				this_thread::sleep_for(std::chrono::seconds(3));
				return;
			}
		}
		else {
			cout << "Tired Robot gets you, you lose :(\n";
			this_thread::sleep_for(std::chrono::seconds(3));
			return;
		}
	}
    
	break;
}
    }
    };
    return;
}