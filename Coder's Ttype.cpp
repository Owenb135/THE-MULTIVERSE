#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <ctime>

struct Data {
    int streak = 0;
    std::string lastDate;
    std::map<std::string, int> minutesPerDay;
};

std::string today() {
    time_t now = time(nullptr);
    tm* local = localtime(&now);

    char buffer[11];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", local);

    return buffer;
}

void save(const Data& d) {
    std::ofstream f("data.txt");

    f << d.streak << "\n";
    f << d.lastDate << "\n";

    for (auto& [date, mins] : d.minutesPerDay) {
        f << date << " " << mins << "\n";
    }
}

bool load(Data& d) {
    std::ifstream f("data.txt");
    if (!f) return false;

    f >> d.streak;
    f >> d.lastDate;

    std::string date;
    int mins;

    while (f >> date >> mins) {
        d.minutesPerDay[date] = mins;
    }

    return true;
}

void updateStreak(Data& d, const std::string& t) {
    if (d.minutesPerDay[t] <= 0) return;

    if (d.lastDate.empty()) {
        d.streak = 1;
    } else {
        time_t now = time(nullptr);
        tm* tmNow = localtime(&now);
        tmNow->tm_mday -= 1;
        mktime(tmNow);

        char buffer[11];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d", tmNow);

        std::string yesterday = buffer;

        if (d.lastDate == yesterday) {
            d.streak++;
        } else {
            d.streak = 1;
        }
    }

    d.lastDate = t;
}

void showCalendar(const Data& d) {
    std::cout << "\n=== CODING CALENDAR ===\n";

    for (auto& [date, mins] : d.minutesPerDay) {
        std::cout << date << " : " << mins << " min\n";
    }

    std::cout << "\nCurrent Streak: "
              << d.streak << " days\n";
}

int CODERS_TTYPE() {
    Data data;
    load(data);

    std::string t = today();

    std::cout << "=== CODING TIMER ===\n";

    std::cout << "1. Start coding session\n";
    std::cout << "2. Add manual minutes\n";
    std::cout << "Choose: ";

    int choice;
    std::cin >> choice;

    int minutes = 0;

    if (choice == 1) {
        std::cout << "Press ENTER to start...";
        std::cin.ignore();

        std::cin.get();
        time_t start = time(nullptr);

        std::cout << "Press ENTER to stop...";
        std::cin.get();

        time_t end = time(nullptr);

        minutes = (end - start) / 60;
    }
    else {
        std::cout << "Enter minutes: ";
        std::cin >> minutes;
    }

    data.minutesPerDay[t] += minutes;

    updateStreak(data, t);

    save(data);

    showCalendar(data);

    std::cout << "\nToday's minutes: "
              << data.minutesPerDay[t] << "\n";

    return 0;
}