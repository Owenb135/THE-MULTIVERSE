#include <iostream>
#include <thread>
#include <chrono>
#include <random>
#include <vector>
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
    while (true) {   
        cout << "Enemy: " << randomEnemy();
        cout << "\nHP: " << randomHigh();
        cout << "\n|----------|";
        cout << "\n|----------|";
        cout << "\nYour stats:";
        cout << "\n HP: " << randomHigh();
        cout << "\nLet's Fight!....";
        this_thread::sleep_for(std::chrono::seconds(10));
        
    }    
}
int main() {
    cout << "\n Game Launching....";
	this_thread::sleep_for(std::chrono::seconds(3));
	battles();
}
