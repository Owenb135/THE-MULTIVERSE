#include "updater.h"
#include "core.h"

#include <iostream>
#include <string>

#if defined(__linux__)
#include <cctype>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <vector>
#include <cerrno>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#endif

namespace {
#if defined(__linux__)
const std::string MANIFEST_URL =
    "https://raw.githubusercontent.com/Owenb135/THE-MULTIVERSE/refs/heads/main/upds/manifest.json";

std::string parse_json_value(const std::string& json, const std::string& key) {
    size_t pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos + key.size() + 2);
    if (pos == std::string::npos) return "";
    pos = json.find_first_not_of(" \t\r\n", pos + 1);
    if (pos == std::string::npos || json[pos] != '"') return "";

    std::string value;
    for (++pos; pos < json.size(); ++pos) {
        if (json[pos] == '"') return value;
        if (json[pos] != '\\') {
            value += json[pos];
            continue;
        }

        if (++pos >= json.size()) return "";
        switch (json[pos]) {
        case '"': value += '"'; break;
        case '\\': value += '\\'; break;
        case '/': value += '/'; break;
        case 'b': value += '\b'; break;
        case 'f': value += '\f'; break;
        case 'n': value += '\n'; break;
        case 'r': value += '\r'; break;
        case 't': value += '\t'; break;
        default: return "";
        }
    }
    return "";
}

bool parse_version(const std::string& version, unsigned int parts[3]) {
    std::istringstream input(version);
    char dot1, dot2;
    if (!(input >> parts[0] >> dot1 >> parts[1] >> dot2 >> parts[2]) ||
        dot1 != '.' || dot2 != '.') {
        return false;
    }
    return input.peek() == std::char_traits<char>::eof();
}

int compare_versions(const std::string& first, const std::string& second) {
    unsigned int a[3], b[3];
    if (!parse_version(first, a) || !parse_version(second, b)) return 0;
    for (int i = 0; i < 3; ++i) {
        if (a[i] != b[i]) return a[i] > b[i] ? 1 : -1;
    }
    return 0;
}

class TemporaryDirectory {
public:
    TemporaryDirectory() {
        char path[] = "/tmp/multiverse-update-XXXXXX";
        char* created = mkdtemp(path);
        if (created) path_ = created;
    }

    ~TemporaryDirectory() { cleanup(); }

    const std::string& path() const { return path_; }
    bool valid() const { return !path_.empty(); }

    void cleanup() {
        if (path_.empty()) return;
        std::error_code error;
        std::filesystem::remove_all(path_, error);
        if (error) {
            std::cerr << "[Updater] Could not remove temporary update files from "
                      << path_ << ": " << error.message() << '\n';
        }
        path_.clear();
    }

private:
    std::string path_;
};

int run_process(const std::vector<std::string>& arguments, std::string* output = nullptr) {
    if (arguments.empty()) return -1;

    int pipe_fds[2] = {-1, -1};
    if (output && pipe(pipe_fds) != 0) {
        std::cerr << "[Updater] Could not create a process output pipe: "
                  << std::strerror(errno) << '\n';
        return -1;
    }

    const pid_t child = fork();
    if (child < 0) {
        if (output) {
            close(pipe_fds[0]);
            close(pipe_fds[1]);
        }
        std::cerr << "[Updater] Could not start " << arguments[0] << ": "
                  << std::strerror(errno) << '\n';
        return -1;
    }

    if (child == 0) {
        if (output) {
            close(pipe_fds[0]);
            if (dup2(pipe_fds[1], STDOUT_FILENO) < 0) _exit(126);
            close(pipe_fds[1]);
        }
        std::vector<char*> argv;
        argv.reserve(arguments.size() + 1);
        for (const auto& argument : arguments) {
            argv.push_back(const_cast<char*>(argument.c_str()));
        }
        argv.push_back(nullptr);
        execvp(argv[0], argv.data());
        _exit(errno == ENOENT ? 127 : 126);
    }

    if (output) {
        close(pipe_fds[1]);
        char buffer[1024];
        ssize_t count;
        while ((count = read(pipe_fds[0], buffer, sizeof(buffer))) > 0) {
            output->append(buffer, static_cast<size_t>(count));
        }
        close(pipe_fds[0]);
    }

    int status = 0;
    while (waitpid(child, &status, 0) < 0) {
        if (errno == EINTR) continue;
        std::cerr << "[Updater] Could not wait for " << arguments[0] << ": "
                  << std::strerror(errno) << '\n';
        return -1;
    }
    return WIFEXITED(status) ? WEXITSTATUS(status) : 128;
}

bool download_file(const std::string& url, const std::string& destination) {
    return run_process({"curl", "--fail", "--location", "--silent", "--show-error",
                        "--retry", "2", "--connect-timeout", "10", "--max-time", "120",
                        "--output", destination, url}) == 0;
}

bool valid_deb_filename(const std::string& filename) {
    if (filename.size() < 5 || filename.substr(filename.size() - 4) != ".deb") return false;
    for (unsigned char ch : filename) {
        if (!(std::isalnum(ch) || ch == '.' || ch == '_' || ch == '-')) return false;
    }
    return true;
}

bool trusted_download_url(const std::string& url) {
    return url.rfind(
        "https://github.com/Owenb135/THE-MULTIVERSE/raw/refs/heads/main/upds/", 0) == 0 ||
        url.rfind(
        "https://raw.githubusercontent.com/Owenb135/THE-MULTIVERSE/refs/heads/main/upds/", 0) == 0;
}

bool valid_update_package(const std::string& package_path, const std::string& expected_version) {
    std::string fields;
    if (run_process({"dpkg-deb", "--field", package_path, "Package", "Version", "Architecture"},
                    &fields) != 0) {
        std::cerr << "[Updater] Downloaded file is not a valid Debian package.\n";
        return false;
    }

    std::istringstream input(fields);
    std::string package, version, architecture, extra;
    if (!(input >> package >> version >> architecture) || input >> extra ||
        package != "themultiverse" || version != expected_version) {
        std::cerr << "[Updater] The downloaded package identity or version does not match "
                     "the update manifest.\n";
        return false;
    }

    std::string system_architecture;
    if (run_process({"dpkg", "--print-architecture"}, &system_architecture) != 0) {
        std::cerr << "[Updater] Could not determine this system's Debian architecture.\n";
        return false;
    }
    while (!system_architecture.empty() &&
           (system_architecture.back() == '\n' || system_architecture.back() == '\r')) {
        system_architecture.pop_back();
    }
    if (architecture != "all" && architecture != system_architecture) {
        std::cerr << "[Updater] Package architecture " << architecture
                  << " does not match this system (" << system_architecture << ").\n";
        return false;
    }
    return true;
}

void handle_linux_updates() {
    std::cout << "[Updater] Checking for updates...\n";
    TemporaryDirectory temporary_directory;
    if (!temporary_directory.valid()) {
        std::cerr << "[Updater] Could not create a private temporary directory.\n";
        return;
    }

    const std::string manifest_path = temporary_directory.path() + "/manifest.json";
    if (!download_file(MANIFEST_URL, manifest_path)) {
        std::cout << "[Updater] Could not reach the update manifest; continuing offline.\n\n";
        return;
    }

    std::ifstream manifest_file(manifest_path);
    if (!manifest_file) {
        std::cerr << "[Updater] Could not read the downloaded update manifest.\n";
        return;
    }
    const std::string manifest((std::istreambuf_iterator<char>(manifest_file)),
                               std::istreambuf_iterator<char>());

    const std::string latest_version = parse_json_value(manifest, "latest_version");
    const std::string release_notes = parse_json_value(manifest, "release_notes");
    unsigned int latest_parts[3];
    if (!parse_version(latest_version, latest_parts)) {
        std::cerr << "[Updater] The update manifest contains an invalid version; continuing offline.\n";
        return;
    }

    const int version_comparison = compare_versions(latest_version, get_version());
    if (version_comparison <= 0) {
        if (version_comparison == 0) {
            std::cout << "[Updater] Version check passed. Core build (v"
                      << get_version() << ") is active.\n\n";
        } else {
            std::cerr << "[Updater] The manifest version is older than this build; "
                         "ignoring it.\n";
        }
        clear_screen();
        return;
    }

    std::cout << "\n=============================================\n"
              << "UPDATE AVAILABLE: New version v" << latest_version << " is ready!\n";
    if (!release_notes.empty()) std::cout << "Changelog: " << release_notes << '\n';
    std::cout << "=============================================\n"
              << "Would you like to download and install this update? (y/n): ";

    char choice = 'n';
    if (!(std::cin >> choice) || (choice != 'y' && choice != 'Y')) {
        clear_screen();
        return;
    }

    const size_t platform_position = manifest.find("\"ubuntu_deb\"");
    const size_t block_start = platform_position == std::string::npos
        ? std::string::npos : manifest.find('{', platform_position);
    const size_t block_end = block_start == std::string::npos
        ? std::string::npos : manifest.find('}', block_start);
    if (block_end == std::string::npos) {
        std::cerr << "[Updater] Debian package settings are missing from the manifest.\n";
        return;
    }
    const std::string platform_block = manifest.substr(block_start, block_end - block_start + 1);
    const std::string download_url = parse_json_value(platform_block, "url");
    const std::string filename = parse_json_value(platform_block, "filename");
    if (!trusted_download_url(download_url) || !valid_deb_filename(filename)) {
        std::cerr << "[Updater] The manifest contains invalid or untrusted package download settings.\n";
        return;
    }

    const std::string package_path = temporary_directory.path() + "/" + filename;
    std::cout << "[Updater] Downloading Debian package...\n";
    if (!download_file(download_url, package_path)) {
        std::cerr << "[Updater] Package download failed; the installed version was not changed.\n";
        return;
    }
    if (!valid_update_package(package_path, latest_version)) return;

    std::cout << "[Updater] Starting the system package installer. Authenticate if prompted.\n";
    const std::vector<std::string> install_command = geteuid() == 0
        ? std::vector<std::string>{"/usr/bin/apt", "install", "-y", "--", package_path}
        : std::vector<std::string>{"pkexec", "/usr/bin/apt", "install", "-y", "--", package_path};
    const int install_status = run_process(install_command);
    if (install_status != 0) {
        std::cerr << "[Updater] Installation did not complete (exit status "
                  << install_status << "); continuing with the installed version.\n";
        return;
    }

    temporary_directory.cleanup();
    std::cout << "[Updater] Update installed successfully. Restart the application to use it.\n";
    std::exit(0);
}
#endif
}

void handle_automatic_updates() {
#if defined(__linux__)
    handle_linux_updates();
#else
    std::cout << "[Updater] Automatic package updates are currently supported on Linux only.\n\n";
#endif
}
