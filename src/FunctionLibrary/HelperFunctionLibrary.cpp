#include "FunctionLibrary/HelperFunctionLibrary.h"

namespace HFL
{
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