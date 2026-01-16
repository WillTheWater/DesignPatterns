#define _CRT_SECURE_NO_WARNINGS
#include "FunctionLibrary/HelperFunctionLibrary.h"

namespace HFL
{
    const int Width = 70;
    const int ColWidth = 35;

    void PrintMainMenu()
    {
        ClearScreen();

        // Header
        SetColor(EColor::Blue);
        std::cout << std::setfill('=') << std::setw(Width) << "" << std::setfill(' ') << "\n";
        SetColor(EColor::BrightWhite);
        std::cout << std::right << std::setw((Width + 20) / 2) << "GAME DESIGN PATTERNS\n";
        SetColor(EColor::Blue);
        std::cout << std::setfill('=') << std::setw(Width) << "" << std::setfill(' ') << "\n\n";

        auto PrintOption = [&](int Index, const std::string& Label) 
            {
                SetColor(EColor::Yellow);
                std::cout << std::right << std::setw(3) << Index << ". ";
                SetColor(HFL::EColor::White);
                std::cout << std::left << std::setw(ColWidth - 5) << Label;
            };

        // --- SECTION: SOLID PRINCIPLES ---
        SetColor(EColor::Green);
        std::cout << std::string((Width - 22) / 2, ' ') << "[ SOLID PRINCIPLES ]\n";
        PrintOption(1, "[SRP] Single Responsibility"); PrintOption(2, "[OCP] Open-Closed"); std::cout << "\n";
        PrintOption(3, "[LSP] Liskov Substitution");   PrintOption(4, "[ISP] Interface Segregation"); std::cout << "\n";
        PrintOption(5, "[DIP] Dependency Inversion");   std::cout << "\n\n";

        // --- SECTION: CREATIONAL ---
        SetColor(EColor::Green);
        std::cout << std::string((Width - 24) / 2, ' ') << "[ CREATIONAL PATTERNS ]\n";
        PrintOption(6, "[SGT] Singleton");       PrintOption(7, "[FTM] Factory Method"); std::cout << "\n";
        PrintOption(8, "[AFT] Abstract Factory"); PrintOption(9, "[PRO] Prototype"); std::cout << "\n";
        PrintOption(10, "[BLD] Builder");         std::cout << "\n\n";

        // --- SECTION: STRUCTURAL ---
        SetColor(EColor::Green);
        std::cout << std::string((Width - 24) / 2, ' ') << "[ STRUCTURAL PATTERNS ]\n";
        PrintOption(11, "[FAC] Facade");    PrintOption(12, "[ADP] Adapter"); std::cout << "\n";
        PrintOption(13, "[PRX] Proxy");     PrintOption(14, "[BRG] Bridge"); std::cout << "\n";
        PrintOption(15, "[COM] Composite"); PrintOption(16, "[DEC] Decorator"); std::cout << "\n";
        PrintOption(17, "[FLY] Flyweight"); std::cout << "\n\n";

        // --- SECTION: BEHAVIORAL ---
        SetColor(EColor::Green);
        std::cout << std::string((Width - 24) / 2, ' ') << "[ BEHAVIORAL PATTERNS ]\n";
        PrintOption(18, "[NUL] Null Object");     PrintOption(19, "[STR] Strategy"); std::cout << "\n";
        PrintOption(20, "[TMP] Template Method"); PrintOption(21, "[ITR] Iterator"); std::cout << "\n";
        PrintOption(22, "[CMD] Command");         PrintOption(23, "[OBS] Observer"); std::cout << "\n";
        PrintOption(24, "[STA] State");           PrintOption(25, "[COR] Chain of Responsibility"); std::cout << "\n";
        PrintOption(26, "[MED] Mediator");        PrintOption(27, "[MEM] Memento"); std::cout << "\n";
        PrintOption(28, "[VIS] Visitor");         PrintOption(29, "[INT] Interpreter"); std::cout << "\n\n";

        // Footer
        SetColor(EColor::BrightRed);
        std::cout << " " << std::setfill('-') << std::setw(Width - 2) << "" << std::setfill(' ') << "\n";
        SetColor(EColor::Yellow);
        std::cout << "  0. ";
        SetColor(EColor::BrightWhite);
        std::cout << "End Demo\n";
        SetColor(EColor::BrightRed);
        std::cout << " " << std::setfill('-') << std::setw(Width - 2) << "" << std::setfill(' ') << "\n";

        HFL::SetColor(HFL::EColor::White);
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
        SetColor(EColor::Blue);
        std::cout << std::setfill('=') << std::setw(60) << "" << std::setfill(' ') << "\n";
        SetColor(EColor::BrightWhite);
        std::cout << std::string(18, ' ') << "" << Title << "\n";
        SetColor(EColor::Blue);
        std::cout << std::setfill('=') << std::setw(60) << "" << std::setfill(' ') << "\n\n";
        SetColor(EColor::White);
    }

    void SetColor(EColor Color)
    {
        switch (Color)
        {
        case EColor::Black:         std::cout << "\033[30m"; break;
        case EColor::Red:           std::cout << "\033[31m"; break;
        case EColor::Green:         std::cout << "\033[32m"; break;
        case EColor::Yellow:        std::cout << "\033[33m"; break;
        case EColor::Blue:          std::cout << "\033[34m"; break;
        case EColor::Magenta:       std::cout << "\033[35m"; break;
        case EColor::Cyan:          std::cout << "\033[36m"; break;
        case EColor::White:         std::cout << "\033[37m"; break;
        case EColor::Gray:          std::cout << "\033[90m"; break;
        case EColor::BrightRed:     std::cout << "\033[91m"; break;
        case EColor::BrightGreen:   std::cout << "\033[92m"; break;
        case EColor::BrightYellow:  std::cout << "\033[93m"; break;
        case EColor::BrightBlue:    std::cout << "\033[94m"; break;
        case EColor::BrightMagenta: std::cout << "\033[95m"; break;
        case EColor::BrightCyan:    std::cout << "\033[96m"; break;
        case EColor::BrightWhite:   std::cout << "\033[97m"; break;
        }
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

    void Wait(float Seconds)
    {
        // Convert float seconds to a chrono duration and sleep
        std::this_thread::sleep_for(std::chrono::duration<float>(Seconds));
    }
}