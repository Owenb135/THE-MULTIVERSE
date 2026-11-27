// Text-Based RPG Engine (menu option [7]).
//
// A turn-based survival RPG: fight wave after wave of monsters, level up, find potions and
// spend gold at the camp between fights. Every 5th wave is a boss.
//
// NOTE: this file keeps its old name (test.cpp) so CMakeLists.txt does not have to change.
#include "core.h"

#include <algorithm>
#include <cctype>
#include <chrono>
#include <cstddef>
#include <exception>
#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <vector>

void rpg_game();

namespace {

// ---------------------------------------------------------------------------------------------
// Tuning
// ---------------------------------------------------------------------------------------------
constexpr int kPotionHeal   = 45;   // HP restored by a potion
constexpr int kPotionPrice  = 15;   // gold per potion at the camp
constexpr int kMaxPotions   = 9;
constexpr int kBossEvery    = 5;    // every Nth wave is a boss fight
constexpr int kDefendHeal   = 6;    // HP regained when defending
constexpr double kScaling   = 0.12; // enemies get 12% stronger every wave

// ---------------------------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------------------------
std::mt19937& rng() {
    static std::mt19937 gen{std::random_device{}()};
    return gen;
}

int roll(int lo, int hi) {
    return std::uniform_int_distribution<int>(lo, hi)(rng());
}

bool chance(int percent) {
    return roll(1, 100) <= percent;
}

void pause_ms(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

// "[#####---------------]" - a living fighter always shows at least one block.
std::string bar(int value, int maxValue, int width = 20) {
    int filled = maxValue > 0 ? std::clamp(value * width / maxValue, 0, width) : 0;
    if (value > 0 && filled == 0) filled = 1;
    return "[" + std::string(static_cast<std::size_t>(filled), '#') +
           std::string(static_cast<std::size_t>(width - filled), '-') + "]";
}

// Reads a menu choice in [lo, hi]. Returns false when the input stream is closed.
// (Token based on purpose: it happily skips the newline the main menu leaves behind.)
bool read_choice(int lo, int hi, int& out) {
    std::string token;
    while (true) {
        std::cout << "> " << std::flush;
        if (!(std::cin >> token)) return false;
        try {
            std::size_t used = 0;
            const int value = std::stoi(token, &used);
            if (used == token.size() && value >= lo && value <= hi) {
                out = value;
                return true;
            }
        } catch (const std::exception&) {
            // not a number: fall through to the hint below
        }
        std::cout << "Please enter a number from " << lo << " to " << hi << ".\n";
    }
}

bool ask_yes_no(const std::string& question) {
    std::string answer;
    while (true) {
        std::cout << question << " (y/n) " << std::flush;
        if (!(std::cin >> answer)) return false;
        const char c = static_cast<char>(std::tolower(static_cast<unsigned char>(answer[0])));
        if (c == 'y') return true;
        if (c == 'n') return false;
    }
}

// ---------------------------------------------------------------------------------------------
// Data
// ---------------------------------------------------------------------------------------------
struct Player {
    int hp = 100, maxHp = 100;
    int minAtk = 10, maxAtk = 18;
    int level = 1, xp = 0, xpToNext = 30;
    int potions = 3, gold = 0;
};

struct EnemyType {
    const char* name;
    int hp, minAtk, maxAtk, xp, gold;
};

struct Enemy {
    std::string name;
    int hp = 0, maxHp = 0;
    int minAtk = 0, maxAtk = 0;
    int xp = 0, gold = 0;
    bool boss = false;
};

const EnemyType kEnemies[] = {
    {"Slime",    28,  4,  8, 12,  4},
    {"Goblin",   42,  6, 11, 18,  6},
    {"Skeleton", 58,  8, 13, 26,  9},
    {"Orc",      80, 10, 16, 38, 14},
};
const EnemyType kBoss = {"Dragon", 110, 9, 15, 100, 50};

Enemy spawn_enemy(int wave) {
    const bool boss = (wave % kBossEvery == 0);
    // tougher monsters join the pool as the waves go on
    const int pool = std::min(4, 2 + (wave - 1) / 2);
    const EnemyType& type = boss ? kBoss : kEnemies[roll(0, pool - 1)];
    const double f = 1.0 + kScaling * (wave - 1);

    Enemy e;
    e.name   = type.name;
    e.boss   = boss;
    e.maxHp  = e.hp = static_cast<int>(type.hp * f);
    e.minAtk = static_cast<int>(type.minAtk * f);
    e.maxAtk = static_cast<int>(type.maxAtk * f);
    e.xp     = static_cast<int>(type.xp * f);
    e.gold   = static_cast<int>(type.gold * f);
    return e;
}

// ---------------------------------------------------------------------------------------------
// Battle
// ---------------------------------------------------------------------------------------------
enum class Outcome { Won, Lost, Fled, Quit };

void draw_battle(const Player& p, const Enemy& e, int wave,
                 const std::vector<std::string>& log, bool showMenu) {
    clear_screen();
    std::cout << "============ WAVE " << wave << (e.boss ? "  -  BOSS FIGHT" : "") << " ============\n\n";
    std::cout << " " << e.name << "\n"
              << "   HP " << bar(e.hp, e.maxHp) << " " << e.hp << "/" << e.maxHp << "\n\n";
    std::cout << " You (Lv " << p.level << ")\n"
              << "   HP " << bar(p.hp, p.maxHp) << " " << p.hp << "/" << p.maxHp << "\n"
              << "   XP " << p.xp << "/" << p.xpToNext
              << "   Potions: " << p.potions << "   Gold: " << p.gold << "\n\n";

    for (const std::string& line : log) std::cout << "  " << line << "\n";
    if (!log.empty()) std::cout << "\n";

    if (showMenu) {
        std::cout << "------------------------------------------\n"
                  << " [1] Attack        95% hit, 10% crit (x2)\n"
                  << " [2] Power Strike  50% hit, 2.5x damage\n"
                  << " [3] Defend        take half damage, heal " << kDefendHeal << " HP\n"
                  << " [4] Use Potion    +" << kPotionHeal << " HP\n"
                  << " [5] Flee          50% chance" << (e.boss ? " (not against a boss!)" : "") << "\n\n";
    }
}

Outcome battle(Player& p, Enemy& e, int wave) {
    std::vector<std::string> log;
    log.push_back(e.boss ? "The mighty " + e.name + " blocks your path!"
                         : "A wild " + e.name + " appears!");

    while (true) {
        draw_battle(p, e, wave, log, true);
        log.clear();
        bool defending = false;

        // ----- your turn (invalid actions don't cost your turn) -----
        while (true) {
            int choice = 0;
            if (!read_choice(1, 5, choice)) return Outcome::Quit;

            if (choice == 1) {
                if (chance(95)) {
                    int dmg = roll(p.minAtk, p.maxAtk);
                    const bool crit = chance(10);
                    if (crit) dmg *= 2;
                    e.hp = std::max(0, e.hp - dmg);
                    log.push_back("You hit the " + e.name + " for " + std::to_string(dmg) +
                                  " damage" + (crit ? " - CRITICAL HIT!" : "."));
                } else {
                    log.push_back("You swing at the " + e.name + " and miss!");
                }
                break;
            }
            if (choice == 2) {
                if (chance(50)) {
                    const int dmg = roll(p.minAtk, p.maxAtk) * 5 / 2;
                    e.hp = std::max(0, e.hp - dmg);
                    log.push_back("POWER STRIKE! The " + e.name + " takes " + std::to_string(dmg) + " damage!");
                } else {
                    log.push_back("Your power strike misses completely!");
                }
                break;
            }
            if (choice == 3) {
                defending = true;
                const int healed = std::min(kDefendHeal, p.maxHp - p.hp);
                p.hp += healed;
                log.push_back("You brace yourself" + (healed > 0 ? " (+" + std::to_string(healed) + " HP)." : "."));
                break;
            }
            if (choice == 4) {
                if (p.potions == 0) {
                    std::cout << "You have no potions left!\n";
                    continue;
                }
                if (p.hp == p.maxHp) {
                    std::cout << "You are already at full health.\n";
                    continue;
                }
                const int healed = std::min(kPotionHeal, p.maxHp - p.hp);
                p.hp += healed;
                --p.potions;
                log.push_back("You drink a potion and recover " + std::to_string(healed) + " HP.");
                break;
            }
            // choice == 5
            if (e.boss) {
                std::cout << "There is no escaping a boss!\n";
                continue;
            }
            if (chance(50)) {
                draw_battle(p, e, wave, {"You got away safely!"}, false);
                return Outcome::Fled;
            }
            log.push_back("You failed to escape!");
            break;
        }

        if (e.hp <= 0) {
            log.push_back("The " + e.name + " is defeated!");
            draw_battle(p, e, wave, log, false);
            return Outcome::Won;
        }

        // ----- enemy turn -----
        if (chance(85)) {
            int dmg = roll(e.minAtk, e.maxAtk);
            std::string verb = " hits you";
            if (e.boss && chance(20)) {
                dmg *= 2;
                verb = " breathes fire at you";
            }
            if (defending) dmg = (dmg + 1) / 2;
            p.hp = std::max(0, p.hp - dmg);
            log.push_back("The " + e.name + verb + " for " + std::to_string(dmg) + " damage" +
                          (defending ? " (blocked half)." : "."));
        } else {
            log.push_back("The " + e.name + " attacks but misses!");
        }

        if (p.hp <= 0) {
            log.push_back("You collapse...");
            draw_battle(p, e, wave, log, false);
            return Outcome::Lost;
        }
    }
}

// ---------------------------------------------------------------------------------------------
// Between fights
// ---------------------------------------------------------------------------------------------
void award_victory(Player& p, const Enemy& e) {
    p.xp += e.xp;
    p.gold += e.gold;
    std::cout << "  +" << e.xp << " XP, +" << e.gold << " gold\n";

    if (p.potions < kMaxPotions && chance(25)) {
        ++p.potions;
        std::cout << "  The " << e.name << " dropped a potion!\n";
    }

    while (p.xp >= p.xpToNext) {
        p.xp -= p.xpToNext;
        ++p.level;
        p.maxHp += 15;
        p.minAtk += 2;
        p.maxAtk += 3;
        p.hp = std::min(p.maxHp, p.hp + 30);
        p.xpToNext = 30 + 20 * (p.level - 1);
        std::cout << "  LEVEL UP! You are now level " << p.level << " (max HP " << p.maxHp
                  << ", attack " << p.minAtk << "-" << p.maxAtk << ")\n";
    }

    const int rest = std::min(p.maxHp / 5, p.maxHp - p.hp);
    if (rest > 0) {
        p.hp += rest;
        std::cout << "  You catch your breath (+" << rest << " HP).\n";
    }
}

// Returns false if the player wants to end the run (or input was closed).
bool camp(Player& p, int nextWave) {
    while (true) {
        std::cout << "\n------------------ CAMP ------------------\n"
                  << " HP " << p.hp << "/" << p.maxHp << "   Potions: " << p.potions
                  << "   Gold: " << p.gold << "\n"
                  << " [1] Continue to wave " << nextWave
                  << (nextWave % kBossEvery == 0 ? "  (BOSS!)" : "") << "\n"
                  << " [2] Buy a potion (" << kPotionPrice << " gold)\n"
                  << " [3] End this run\n";
        int choice = 0;
        if (!read_choice(1, 3, choice)) return false;

        if (choice == 1) return true;
        if (choice == 3) return false;

        if (p.potions >= kMaxPotions)      std::cout << "You can't carry any more potions.\n";
        else if (p.gold < kPotionPrice)    std::cout << "Not enough gold.\n";
        else {
            p.gold -= kPotionPrice;
            ++p.potions;
            std::cout << "You bought a potion.\n";
        }
    }
}

} // namespace

// ---------------------------------------------------------------------------------------------
// Entry point (called from the main menu)
// ---------------------------------------------------------------------------------------------
void rpg_game() {
    static int bestWave = 0; // best result of this session

    clear_screen();
    std::cout << "=============== TEXT RPG ===============\n\n"
              << "Survive as many waves as you can.\n"
              << "Type a number and press Enter to choose an action.\n"
              << "Every " << kBossEvery << "th wave is a boss - stock up on potions!\n";
    pause_ms(2500);

    do {
        Player player;
        int wave = 1; // the wave currently being fought (or about to be fought)
        std::string ending;

        while (true) {
            Enemy enemy = spawn_enemy(wave);
            const Outcome outcome = battle(player, enemy, wave);

            if (outcome == Outcome::Quit) return;

            if (outcome == Outcome::Lost) {
                ending = "You were defeated by the " + enemy.name + " on wave " + std::to_string(wave) + ".";
                break;
            }
            if (outcome == Outcome::Fled) {
                pause_ms(1200);
                continue; // same wave, new monster
            }

            // Won
            award_victory(player, enemy);
            ++wave;
            if (!camp(player, wave)) {
                ending = "You decided to end your adventure.";
                break;
            }
        }

        const int cleared = wave - 1; // waves that were actually won
        bestWave = std::max(bestWave, cleared);

        std::cout << "\n=============== GAME OVER ===============\n"
                  << ending << "\n"
                  << "Waves cleared: " << cleared << "   Level: " << player.level
                  << "   Gold: " << player.gold << "\n"
                  << "Best this session: " << bestWave << " waves\n\n";
    } while (ask_yes_no("Play again?"));

    std::cout << "Thanks for playing! Returning to main menu...\n";
    pause_ms(1200);
}
