#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>

class User {
private:
    std::string username;
    std::string password;

public:
    User() = default;
    User(const std::string& uname, const std::string& pass)
        : username(uname), password(pass) {}

    std::string getUsername() const { return username; }
    std::string getPassword() const { return password; }
};

class AuthSystem {
private:
    const std::string filename = "users.txt";

    // Utility to check if a username already exists in the file
    bool isUsernameTaken(const std::string& username) {
        std::ifstream file(filename);
        if (!file.is_open()) return false;

        std::string line, u, p;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            if (ss >> u >> p) {
                if (u == username) {
                    file.close();
                    return true;
                }
            }
        }
        file.close();
        return false;
    }

public:
    // Registration function
    bool registerUser() {
        std::string username, password;

        std::cout << "\n===========================================\n";
        std::cout << "             USER REGISTRATION             \n";
        std::cout << "===========================================\n";

        std::cout << "Enter a username: ";
        std::cin >> username;

        if (username.empty()) {
            std::cout << "[ERROR] Username cannot be empty.\n";
            return false;
        }

        if (isUsernameTaken(username)) {
            std::cout << "[ERROR] Username '" << username << "' is already taken. Please try another.\n";
            return false;
        }

        std::cout << "Enter a password: ";
        std::cin >> password;

        if (password.length() < 4) {
            std::cout << "[ERROR] Password must be at least 4 characters long.\n";
            return false;
        }

        // Save credentials to file
        std::ofstream file(filename, std::ios::app);
        if (!file.is_open()) {
            std::cout << "[ERROR] Failed to open database file.\n";
            return false;
        }

        file << username << " " << password << "\n";
        file.close();

        std::cout << "[SUCCESS] Registration successful! You can now log in.\n";
        return true;
    }

    // Login function
    bool loginUser() {
        std::string username, password;

        std::cout << "\n===========================================\n";
        std::cout << "                USER LOGIN                 \n";
        std::cout << "===========================================\n";

        std::cout << "Enter username: ";
        std::cin >> username;
        std::cout << "Enter password: ";
        std::cin >> password;

        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cout << "[ERROR] No registered users found. Please register first.\n";
            return false;
        }

        std::string line, u, p;
        bool authenticated = false;

        while (std::getline(file, line)) {
            std::stringstream ss(line);
            if (ss >> u >> p) {
                if (u == username && p == password) {
                    authenticated = true;
                    break;
                }
            }
        }
        file.close();

        if (authenticated) {
            std::cout << "[SUCCESS] Welcome back, " << username << "! Login successful.\n";
            return true;
        } else {
            std::cout << "[ERROR] Invalid username or password.\n";
            return false;
        }
    }
};

int main() {
    AuthSystem auth;
    int choice;

    do {
        std::cout << "\n===========================================\n";
        std::cout << "      LOGIN & REGISTRATION SYSTEM          \n";
        std::cout << "===========================================\n";
        std::cout << "1. Register\n";
        std::cout << "2. Login\n";
        std::cout << "3. Exit\n";
        std::cout << "Choose an option (1-3): ";

        if (!(std::cin >> choice)) {
            std::cout << "Invalid choice. Please enter a number.\n";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }

        switch (choice) {
            case 1:
                auth.registerUser();
                break;
            case 2:
                auth.loginUser();
                break;
            case 3:
                std::cout << "Exiting program. Goodbye!\n";
                break;
            default:
                std::cout << "Invalid option. Select 1, 2, or 3.\n";
        }
    } while (choice != 3);

    return 0;
}
