#include "updater.h"
#include "core.h"

#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

using namespace std::chrono_literals;

    // 2. Clear path pointing directly to your live manifest file on GitHub
    static const std::string MANIFEST_URL = "https://raw.githubusercontent.com/Owenb135/THE-MULTIVERSE/refs/heads/main/upds/manifest.json";

    // Lightweight inline string locator to extract JSON values without heavy external libraries
    std::string parse_json_value(const std::string& json, const std::string& key) {
        size_t pos = json.find("\"" + key + "\"");
        if (pos == std::string::npos) return "";
        pos = json.find(":", pos);
        size_t start = json.find("\"", pos);
        size_t end = json.find("\"", start + 1);
        if (start == std::string::npos || end == std::string::npos) return "";
        return json.substr(start + 1, end - start - 1);
    }

    // Compare semantic versions (e.g., "1.3.3" vs "1.2.5")
    // Returns: 1 if v1 > v2, -1 if v1 < v2, 0 if v1 == v2
    int compare_versions(const std::string& v1, const std::string& v2) {
        std::istringstream v1_stream(v1), v2_stream(v2);
        int major1, minor1, patch1, major2, minor2, patch2;
        char dot;

        v1_stream >> major1 >> dot >> minor1 >> dot >> patch1;
        v2_stream >> major2 >> dot >> minor2 >> dot >> patch2;

        if (major1 != major2) return major1 > major2 ? 1 : -1;
        if (minor1 != minor2) return minor1 > minor2 ? 1 : -1;
        if (patch1 != patch2) return patch1 > patch2 ? 1 : -1;
        return 0;
    }

    void handle_automatic_updates() {
        std::cout << "[Updater] Connecting to GitHub update stream...\n";

        // Download manifest.json from your branch
        std::string downloadCmd = "wget -q \"" + MANIFEST_URL + "\" -O /tmp/manifest.json";
        int status = system(downloadCmd.c_str());

        if (status != 0) {
            std::cout << "[Updater] Unable to reach GitHub updates. Launching offline mode.\n\n";
            return;
        }

        // Read the entire file into a C++ string stream
        std::ifstream file("/tmp/manifest.json");
        if (!file.is_open()) return;
        std::string manifest((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();

        // Extract key pieces of information from the JSON string
        std::string latest_version = parse_json_value(manifest, "latest_version");
        std::string release_notes = parse_json_value(manifest, "release_notes");

        // Clean up the temp manifest file
        std::remove("/tmp/manifest.json");

        // 3. Compare the version numbers!
        int version_cmp = compare_versions(latest_version, get_version());
        if (version_cmp > 0 && !latest_version.empty()) {
            std::cout << "\n=============================================\n";
            std::cout << "UPDATE AVAILABLE: New version v" << latest_version << " is ready!\n";
            if (!release_notes.empty()) {
                std::cout << "Changelog: " << release_notes << "\n";
            }
            std::cout << "=============================================\n";
            std::cout << "Would you like to download and install this update? (y/n): ";

            char choice;
            std::cin >> choice;
            if (choice == 'y' || choice == 'Y')
            {
              std::string platform_key = "ubuntu_deb";

#if defined(_WIN32)
              platform_key = "windows_exe";
#endif

              // Find the selected platform data block inside the manifest string
              size_t plat_pos = manifest.find("\"" + platform_key + "\"");
              if (plat_pos == std::string::npos) {
                std::cout << "[Error] Platform settings missing from manifest. Launching game...\n\n";
                return;
              }
              std::string plat_block = manifest.substr(plat_pos, manifest.find("}", plat_pos) - plat_pos);

              std::string download_url = parse_json_value(plat_block, "url");
              std::string filename = parse_json_value(plat_block, "filename");

              if (download_url.empty() || filename.empty()) {
                std::cout << "[Error] Failed to read download metadata from manifest. Launching game...\n\n";
                return;
              }

              std::cout << "[Updater] Downloading package via curl...\n";
              std::string tmpPath = "/tmp/" + filename;
              std::string download_cmd = "curl -fSL \"" + download_url + "\" -o " + tmpPath + " 2>/tmp/update_log.txt";
              int progress_res = system(download_cmd.c_str());

              if (progress_res != 0) {
                std::cout << "[Error] Download failed. See /tmp/update_log.txt for details.\n\n";
                return;
              }

              // Validate .deb file before attempting install
              std::cout << "[Updater] Verifying downloaded package...\n";
              std::string verify_cmd = "dpkg-deb -I " + tmpPath + " >/tmp/update_log.txt 2>&1";
              int verify_res = system(verify_cmd.c_str());
              if (verify_res != 0) {
                std::cout << "[Error] Downloaded file is not a valid .deb. See /tmp/update_log.txt\n\n";
                return;
              }

              std::cout << "[Updater] Launching background installer (will ask for authentication)...\n";

#if defined(_WIN32)
              // Windows: Run the installer executable silently
              std::ofstream batch("apply_update.bat");
              batch << "@echo off\n"
                    << "timeout /t 1 /nobreak > nul\n"
                    << "start /wait /tmp\\" << filename << " /SILENT\n"
                    << "del \"%~f0\"\n";
              batch.close();
              system("start /b apply_update.bat");
              exit(0);
#else
              // Create a robust installer script that attempts dpkg -i and falls back to apt-get -f install
              std::ofstream sh("/tmp/apply_update.sh");
              sh << "#!/bin/bash\n"
                 << "set -e\n"
                 << "touch /tmp/update_started.flag\n"
                 << "# Try installing the package, record output to /tmp/update_log.txt\n"
                 << "if dpkg -i \"" << tmpPath << "\" >/tmp/update_log.txt 2>&1; then\n"
                 << "  echo \"dpkg install succeeded\" >> /tmp/update_log.txt\n"
                 << "  touch /tmp/update_success.flag\n"
                 << "else\n"
                 << "  echo \"dpkg install failed; attempting to repair with apt-get -f install\" >> /tmp/update_log.txt\n"
                 << "  apt-get update >> /tmp/update_log.txt 2>&1 || true\n"
                 << "  apt-get -f install -y >> /tmp/update_log.txt 2>&1 || true\n"
                 << "  # Try installing again after fixing deps\n"
                 << "  if dpkg -i \"" << tmpPath << "\" >> /tmp/update_log.txt 2>&1; then\n"
                 << "    touch /tmp/update_success.flag\n"
                 << "  fi\n"
                 << "fi\n"
                 << "rm -f /tmp/update_started.flag\n"
                 << "# Remove this script after running\n"
                 << "rm -- \"$0\"\n";
              sh.close();

              std::remove("/tmp/update_started.flag");
              system("chmod +x /tmp/apply_update.sh");

              // Run via pkexec so the entire script runs with elevated privileges and prompts for auth
              std::string run_cmd = "nohup pkexec /bin/bash /tmp/apply_update.sh >/dev/null 2>&1 &";
              system(run_cmd.c_str());

              std::cout << "[Updater] Waiting for authentication terminal hook...\n";

              int timeout_counter = 0;
              while (!std::ifstream("/tmp/update_started.flag") && timeout_counter < 30) {
                  std::this_thread::sleep_for(100ms);
                  timeout_counter++;
              }

              std::this_thread::sleep_for(500ms);
              // Exit now so the installed system package can replace the binary if needed
              exit(0);
#endif
            }
        } else {
            std::cout << "[Updater] Version check passed. Core build (v" << get_version() << ") is active.\n\n";
        }
        std::this_thread::sleep_for(3000ms);
        clear_screen();
    }
