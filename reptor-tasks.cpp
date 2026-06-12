#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

const string FILENAME = "tasks.txt";
void saveTasks(const vector<string>& tasks) {
    ofstream file(FILENAME);
    for (const string& task : tasks) {
        file << task << "\n";
    }
}

vector<string> loadTasks() {
    vector<string> tasks;
    ifstream file(FILENAME);
    string task;
    while (getline(file, task)) {
        if (!task.empty()) {
            tasks.push_back(task);
        }
    }
    return tasks;
}

int start() {
    string version = "1.0.0";
    string author = "Owenb135/Owen0963";
    
    cout << R"(
 ██████╗ ███████╗██████╗ ████████╗██████╗ ██████╗     ████████╗█████╗ ███████╗██╗  ██╗███████╗
██╔══██╗██╔════╝██╔══██╗╚══██╔══╝██╔══██╗██╔══██╗    ╚══██╔══╝██╔══██╗██╔════╝██║ ██╔╝██╔════╝
██████╔╝█████╗  ██████╔╝   ██║   ██║  ██║██████╔╝       ██║   ███████║███████╗█████╔╝ ███████╗
██╔══██╗██╔══╝  ██╔═══╝    ██║   ██║  ██║██╔══██╗       ██║   ██╔══██║╚════██║██╔═██╗ ╚════██║
██║  ██║███████╗██║        ██║   ╚██████╔╝██║  ██║      ██║   ██║  ██║███████║██║  ██╗███████║
╚═╝  ╚═╝╚══════╝╚═╝        ╚═╝    ╚═════╝ ╚═╝  ╚═╝      ╚═╝   ╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚══════╝
)";
                    
    cout << "\n                               Version " << version << " by " << author << "\n";
    return 0;
}

void tasks() {
    vector<string> list = loadTasks();
    string choice;

    while (true) {
        cout << "\n===============================\n";
        cout << "         🦖 REPTOR TASKS       \n";
        cout << "===============================\n";

        if (list.empty()) {
            cout << "[Your task list is empty!]\n";
        } else {
            for (size_t i = 0; i < list.size(); ++i) {
                cout << "[" << i + 1 << "] " << list[i] << "\n";
            }
        }

        cout << "\nCommands: 'add [task]', 'done [number]', 'exit'\n";
        cout << "Enter command: ";
        
        string fullInput;
        getline(cin >> ws, fullInput);

        stringstream ss(fullInput);
        string command;
        ss >> command;

        if (command == "add") {
            string taskDetails;
            getline(ss >> ws, taskDetails);
            if (!taskDetails.empty()) {
                list.push_back(taskDetails);
                saveTasks(list);
                cout << "➕ Task added!\n";
            }
        } 
        else if (command == "done") {
            int id;
            if (ss >> id) {
                if (id > 0 && id <= static_cast<int>(list.size())) {
                    cout << "✔ Completed: " << list[id - 1] << "\n";
                    list.erase(list.begin() + (id - 1));
                    saveTasks(list);
                } else {
                    cout << "❌ Invalid task number.\n";
                }
            }
        } 
        else if (command == "exit") {
            cout << "Goodbye!\n";
            break;
        } 
        else {
            cout << "❌ Unknown command. Try 'add', 'done', or 'exit'.\n";
        }

        this_thread::sleep_for(chrono::seconds(1));
        system("clear");
    }
}

void r10() {
    start();
    this_thread::sleep_for(chrono::seconds(3));
    system("clear");
    tasks();
}
