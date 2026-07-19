#include <iostream>
#include <thread>
#include <chrono>
#include <random>
#include <vector>
int main();
using namespace std;
int randomLow() {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> dist(1, 75);
    return dist(gen);
}

int randomHigh() {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> dist(76, 150);
    return dist(gen);
}

string randomEnemy() {
    static random_device rd;
    static mt19937 gen(rd());

    vector<string> enemies = {
        "Goblin",
        "Orc",
        "Skeleton",
        "Dragon",
        "Slime"
    };

    uniform_int_distribution<> dist(0, enemies.size() - 1);
    return enemies[dist(gen)];
}

void battles() {
		int enemyHP = randomHigh();
        cout << "Enemy: " << randomEnemy();
        cout << "\nHP: " << enemyHP;
        cout << "\n|----------|";
        cout << "\n|----------|";
        cout << "\nYour stats:";
		int playerHP = randomHigh();
        cout << "\n HP: " << playerHP;
        cout << "\nLet's Fight!....\n";
        this_thread::sleep_for(std::chrono::seconds(5));
		system("clear");
		int Damage_player = randomLow();
		cout << "You attack the enemy and deal " << Damage_player << " damage!";
		enemyHP -= Damage_player;
		this_thread::sleep_for(std::chrono::seconds(3));
		int Damage_enemy = randomLow();
		cout << "\nThe enemy attacks you and deals " << Damage_enemy << " damage!" << endl;
		this_thread::sleep_for(std::chrono::seconds(3));
		playerHP -= Damage_enemy;
		cout << "\nDrummblelrollll please....." << endl;
		this_thread::sleep_for(std::chrono::seconds(5));
		system("clear");
		cout << "\nYour HP: " << playerHP;
		cout << "\nEnemy HP: " << enemyHP;
		if (playerHP >= enemyHP) {
			cout << "\nYou win!\n";
		}
		else {
			cout << "\nYou lose!\n";
		}
		this_thread::sleep_for(std::chrono::seconds(3));
		cout << "GAME OVER!\n";
		// Ask the player if they want to play again
		cout << "Play again? (Select no if you want to return to main menu y/n)\n";
		string response;
		cin >> response;
		if (response == "y" || response == "Y") {
			system("clear");
			battles();
		}
		else {
			cout << "Thanks for playing!\n";
			system("clear");
			main(); // Fixed The bug
			}
}

void rpg_game() {
    cout << "\n Game Launching....\n";
    this_thread::sleep_for(std::chrono::seconds(3));
	system("clear");
    battles();
	cout << "\n";

}
