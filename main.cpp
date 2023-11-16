#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <map>
#include <string>
#include <regex>

class Note {
public:
    std::string title;
    std::string content;
    std::string date;
};

class Student {
public:
    std::string name;
    int studentID;
};

class DailyGoal {
public:
    std::string description;
    std::string dueDate;
    bool completed;
    void markAsCompleted() {
        completed = true;
    }
};

struct User {
    std::string username;
    std::string password;
};

const std::regex usernamePattern(R"(\d{2}\d{4}@bennett\.edu\.in)");

bool isValidUsername(const std::string& username) {
    return std::regex_match(username, usernamePattern);
}

void printWelcomeScreen() {
    std::cout << "~~~~~~~~~~~~~~     Welcome to the Student portal System      ~~~~~~~~~~~~~\n";
    std::cout << "<------------------------------------------------------------------------>\n";
}

int displayMainMenu() {
    int choice;
    std::cout << "\nMain Menu:\n";
    std::cout << "1. Register\n";
    std::cout << "2. Login\n";
    std::cout << "3. student data\n";
    std::cout << "4. Add Student\n";
    std::cout << "5. Add Note\n";
    std::cout << "6. Set Daily Goal\n";
    std::cout << "7. Display Goals\n";
    std::cout << "8.  Student Progress \n";
    std::cout << "9. Exit\n";
    std::cout << "Enter your choice: ";
    std::cin >> choice;
    return choice;
}

void registerUser() {
    User user;
    std::cout << "Enter a username (e.g., YY1234@bennett.edu.in):\n";
    std::cin >> user.username;

    if (!isValidUsername(user.username)) {
        std::cout << "Registration failed. Invalid username format. It should be in the format 'YY1234@bennett.edu.in'.\n";
        return;
    }

    std::cout << "Enter a password: ";
    std::cin >> user.password;

    std::ofstream usersFile("users.txt", std::ios::app);
    if (usersFile) {
        usersFile << user.username << ' ' << user.password << '\n';
        std::cout << "Registration successful. You can now log in.\n";
        usersFile.close();
    } else {
        std::cerr << "Error: Unable to open users.txt for writing.\n";
    }
}

bool loginUser() {
    User user;
    std::cout << "Enter your username: ";
    std::cin >> user.username;

    std::cout << "Enter your password: ";
    std::cin >> user.password;

    std::ifstream usersFile("users.txt");
    if (usersFile) {
        std::string storedUsername, storedPassword;
        bool found = false;
        while (usersFile >> storedUsername >> storedPassword) {
            if (storedUsername == user.username && storedPassword == user.password) {
                std::cout << "Login successful. Welcome, " << user.username << "!\n";
                found = true;
                break;
            }
        }
        usersFile.close();
        if (!found) {
            std::cout << "Login failed. Incorrect username or password.\n";
        }
        return found;
    } else {
        std::cerr << "Error: Unable to open users.txt for reading.\n";
        return false;
    }
}
void displayCSV(const std::string& filename) {
    std::ifstream file(filename);
    if (file) {
        std::string line;
        std::cout << std::left << std::setw(20) << "Student Report -" << std::setw(10) << "Bennett University" << std::endl;
        std::cout << "---------------------------------------------" << std::endl;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string token;
            while (std::getline(iss, token, ',')) {
                std::cout << std::left << std::setw(20) << token;
            }
            std::cout << std::endl;
        }
        file.close();
    } else {
        std::cerr << "Error: Unable to open " << filename << " for reading.\n";
    }
}
void addStudentToMainCSV(const Student& student, float overallAttendance, float progressReport) {
    std::ofstream mainCSV("main.csv", std::ios::app);
    if (mainCSV) {
        mainCSV << student.name << ',' << student.studentID << ',' << overallAttendance << ',' << progressReport << '\n';
        mainCSV.close();
    } else {
        std::cerr << "Error: Unable to open main.csv for writing.\n";
    }
}
void addStudentToCSV(const Student& student, float overallAttendance, float progressReport) {
    std::ofstream studentCSV("students.csv", std::ios::app);
    if (studentCSV) {
        studentCSV << student.name << ',' << student.studentID << ',' << overallAttendance << ',' << progressReport << '\n';
        studentCSV.close();
        addStudentToMainCSV(student, overallAttendance, progressReport);
    } else {
        std::cerr << "Error: Unable to open students.csv for writing.\n";
    }
}

void addStudentManually() {
    Student student;
    float overallAttendance, progressReport;

    std::cin.ignore();
    std::cout << "Enter student name: ";
    std::getline(std::cin, student.name);

    std::cout << "Enter Enrollment no: ";
    std::cin >> student.studentID;

    std::cout << "Enter overall attendance: ";
    std::cin >> overallAttendance;

    std::cout << "Enter progress report: ";
    std::cin >> progressReport;

    addStudentToCSV(student, overallAttendance, progressReport);
}
void displayNoteCard(const Note& note) {
    std::cout << "*********************\n";
    std::cout << "Title: " << note.title << "\n";
    std::cout << "Date: " << note.date << "\n";
    std::cout << "Content:\n" << note.content << "\n";
    std::cout << "*********************\n";
}
void saveNoteToFile(const Note& note) {
    std::ofstream file("notes.txt", std::ios::app);
    if (file) {
        file << "Title: " << note.title << "\n";
        file << "Date: " << note.date << "\n";
        file << "Content:\n" << note.content << "\n";
        file << "Added on: " << std::time(0) << "\n"; // Add timestamp
        file << "*********************\n";
        file.close();
    } else {
        std::cerr << "Error: Unable to open notes.txt for writing.\n";
    }
}
void displayGoals(const std::vector<DailyGoal>& goals) {
    std::cout << "Goals:\n";
    std::cout << "---------------------\n";
    for (const auto& goal : goals) {
            std::cout << "Description: " << goal.description << "\n";
            std::cout << "Due Date: " << goal.dueDate << "\n";
            std::cout << "Completed: "<<(goal.completed ? "Yes" : "No") << "\n";
            std::cout << "--------------------------\n";
        }
    }
void updateGoals(std::vector<DailyGoal>& goals) {
    for (auto& goal : goals) {
        std::string userInput;
        std::cout << "Is goal '" << goal.description << "' completed? (Yes/No): ";
        std::cin >> userInput;

        // Convert user input to lowercase for easier comparison
        for (char& c : userInput) {
            c = std::tolower(c);
        }

        if (userInput == "yes") {
            goal.completed = true;
        } else {
            goal.completed = false;
        }
    }
}
void saveGoalsToFile(const std::vector<DailyGoal>& goals) {
    std::ofstream file("goals.txt", std::ios::app);
    if (file) {
        for (const auto& goal : goals) {
            file << "Description: " << goal.description << "\n";
            file << "Due Date: " << goal.dueDate << "\n";
            file << "Completed: " << (goal.completed ? "Yes" : "No") << "\n";
            file << "--------------------------\n";
        }
        file.close();
    } else {
        std::cerr << "Error: Unable to open goals.txt for writing.\n";
    }
}
int main() {
    printWelcomeScreen();

    std::vector<Student> studentList;
    std::vector<Note> studentNotes;
    std::vector<DailyGoal> dailyGoals;
    std::map<int, int> studentProgress;

    int choice;
    do {
        choice = displayMainMenu();

        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                if (loginUser()) {
                }
                break;
            case 3: {
                displayCSV("main.csv");
                break;
                }
                case 4: {
                addStudentManually();
                break;
                }
            case 5: {
                        std::vector<Note> notes;
            char addAnother;

            do {
                Note newNote;
                std::cout << "Enter note title: ";
                std::getline(std::cin >> std::ws, newNote.title);

                std::cout << "Enter note content: ";
                std::getline(std::cin >> std::ws, newNote.content);

                std::cout << "Enter note date: ";
                std::getline(std::cin >> std::ws, newNote.date);

                notes.push_back(newNote);
                saveNoteToFile(newNote);
                std::cout << "Add another note? (y/n): ";
                std::cin >> addAnother;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer
                std::cout << "Notes saved to notes.txt.\n";
            } while (addAnother == 'y' || addAnother == 'Y');

            for (const auto& note : notes) {
                displayNoteCard(note);
            }
        break;
            }
            case 6: {
            DailyGoal goal;
                std::cin.ignore();
                std::cout << "Enter goal description: ";
                std::getline(std::cin, goal.description);
                std::cout << "Enter due date: ";
                std::getline(std::cin, goal.dueDate);
                goal.completed = false;  // Defaulting to not completed
                dailyGoals.push_back(goal);
                displayGoals(dailyGoals);
                saveGoalsToFile(dailyGoals);
                updateGoals(dailyGoals);
                break;
            }
            case 7: {
                displayGoals(dailyGoals);
                int option;
                std::cout << "Enter the goal number to mark as completed (0 to exit): ";
                std::cin >> option;
                if (option > 0 && option <= dailyGoals.size()) {
                    dailyGoals[option - 1].markAsCompleted();
                    std::cout << "Goal marked as completed!\n";
                } else if (option != 0) {
                    std::cout << "Invalid goal number!\n";
                }
                saveGoalsToFile(dailyGoals);
                break;
                }
        case 8: {

            }

            case 9:
                std::cout << "Goodbye!\n";
                break;
            default:
                std::cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 9);

    return 0;
}
