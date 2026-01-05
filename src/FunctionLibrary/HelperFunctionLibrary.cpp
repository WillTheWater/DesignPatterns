#define _CRT_SECURE_NO_WARNINGS
#include "FunctionLibrary/HelperFunctionLibrary.h"

namespace HFL
{
    void PrintMainMenu()
    {
        std::cout << std::setfill('=') << std::setw(60) << "" << std::setfill(' ') << "\n";
        std::cout << std::setw(40) << "Game Design Patterns\n";
        std::cout << std::setfill('=') << std::setw(60) << "" << std::setfill(' ') << "\n\n";

        std::cout << "Select a Design Pattern to Demo:\n\n";
        std::cout << "1. Single Responsibility Principle (SRP)\n";
        std::cout << "2. Open-Closed Principle (OCP)\n";
        std::cout << "3. Liskov Substitution Principle (LSP)\n";
        std::cout << "4. Interface Segregation Principle (ISP)\n";
        std::cout << "5. Dependency Inversion Principle (DIP)\n";
        std::cout << "6. Singleton (SGT)\n";
        std::cout << "7. Factory Method (FTM)\n";
        std::cout << "8. Abstract Factory (AFT)\n";
        std::cout << "9. Prototype (PRO)\n";
        std::cout << "10. Builder (BLD)\n";
        std::cout << "11. Facade (FAC)\n";
        std::cout << "12. Adapter (ADP)\n";
        std::cout << "0. End Demo\n";
    }

    int GetValidMenuInput(int MaxOption)
    {
        int Choice = -1;
        while (true)
        {
            std::cout << "\nSelection: ";
            std::cin >> Choice;

            if (std::cin.fail())
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Please enter a number.\n";
                continue;
            }

            if (Choice >= 0 && Choice <= MaxOption)
            {
                return Choice;
            }
            else
            {
                std::cout << "Invalid selection. Please choose between 0 and " << MaxOption << ".\n";
            }
        }
    }

    void ClearScreen()
    {
        // Cross-platform clear screen
#ifdef _WIN32
        std::system("cls");
#else
        std::system("clear");
#endif
    }

    void WaitForInput()
    {
        std::cout << "\n[Press Enter to continue...]";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (std::cin.peek() == '\n') std::cin.get();
    }

    void PrintHeader(const std::string& Title)
    {
        std::cout << std::setfill('=') << std::setw(60) << "" << std::setfill(' ') << "\n";
        std::cout << std::setw(40) << Title << "\n";
        std::cout << std::setfill('=') << std::setw(60) << "" << std::setfill(' ') << "\n\n";
    }

    // ------------------------------------------------------------------------
    // CROSS-PLATFORM SAVE DIRECTORY
    // ------------------------------------------------------------------------
    // Windows: C:\Users\Name\AppData\Roaming\GameDesignPatterns\SRP\
    // Linux/Mac: ~/.local/share/GameDesignPatterns/SRP/ (or equiv)
    // ------------------------------------------------------------------------
    std::filesystem::path GetSaveDirectory(const std::string& PrincipleNamespace)
    {
        std::filesystem::path SavePath;

        // 1. Determine the OS-specific base directory
        #ifdef _WIN32
                const char* AppData = std::getenv("APPDATA");
                if (AppData) SavePath = AppData;
        #elif __APPLE__ || __linux__
                const char* Home = std::getenv("HOME");
                if (Home) {
                    SavePath = Home;
        #ifdef __APPLE__
                    SavePath /= "Library/Application Support";
        #else
                    SavePath /= ".local/share";
        #endif
                }
        #endif

        // 2. Append Project Root and Specific Subfolder
        SavePath /= "GameDesignPatterns";
        SavePath /= PrincipleNamespace;

        // 3. Create the directory structure if it doesn't exist
        if (!std::filesystem::exists(SavePath))
        {
            std::filesystem::create_directories(SavePath);
            std::cout << "[System] Created save directory at: " << SavePath << "\n";
        }

        return SavePath;
    }
}