#define _CRT_SECURE_NO_WARNINGS
#include "FunctionLibrary/HelperFunctionLibrary.h"

namespace HFL
{
    const int Width = 70;
    const int ColWidth = 35;

    void PrintMainMenu()
    {
        ClearScreen();

        // ======================== HEADER ========================
        PrintHeader("GAME DESIGN PATTERNS");

        auto PrintOption = [&](int Index, const std::string& Label) 
            {
                SetColor(EColor::BrightWhite);
                std::cout << std::right << std::setw(3) << Index << ". ";
                SetColor(HFL::EColor::Gray);
                std::cout << std::left << std::setw(ColWidth - 5) << Label;
            };

        // ======================== SECTION: SOLID PRINCIPLES ========================
        PrintSection("[ SOLID PRINCIPLES ]\n");
        PrintOption(1, "[SRP] Single Responsibility"); PrintOption(2, "[OCP] Open-Closed"); std::cout << "\n";
        PrintOption(3, "[LSP] Liskov Substitution");   PrintOption(4, "[ISP] Interface Segregation"); std::cout << "\n";
        PrintOption(5, "[DIP] Dependency Inversion");   std::cout << "\n\n";

        // ======================== SECTION: CREATIONAL ========================
        PrintSection("[ CREATIONAL PATTERNS ]\n");
        PrintOption(6, "[SGT] Singleton");       PrintOption(7, "[FTM] Factory Method"); std::cout << "\n";
        PrintOption(8, "[AFT] Abstract Factory"); PrintOption(9, "[PRO] Prototype"); std::cout << "\n";
        PrintOption(10, "[BLD] Builder");         std::cout << "\n\n";

        // ======================== SECTION: STRUCTURAL ========================
        PrintSection("[ STRUCTURAL PATTERNS ]\n");
        PrintOption(11, "[FAC] Facade");    PrintOption(12, "[ADP] Adapter"); std::cout << "\n";
        PrintOption(13, "[PRX] Proxy");     PrintOption(14, "[BRG] Bridge"); std::cout << "\n";
        PrintOption(15, "[COM] Composite"); PrintOption(16, "[DEC] Decorator"); std::cout << "\n";
        PrintOption(17, "[FLY] Flyweight"); std::cout << "\n\n";

        // ======================== SECTION: BEHAVIORAL ========================
        PrintSection("[ BEHAVIORAL PATTERNS ]\n");
        PrintOption(18, "[NUL] Null Object");     PrintOption(19, "[STR] Strategy"); std::cout << "\n";
        PrintOption(20, "[TMP] Template Method"); PrintOption(21, "[ITR] Iterator"); std::cout << "\n";
        PrintOption(22, "[CMD] Command");         PrintOption(23, "[OBS] Observer"); std::cout << "\n";
        PrintOption(24, "[STA] State");           PrintOption(25, "[COR] Chain of Responsibility"); std::cout << "\n";
        PrintOption(26, "[MED] Mediator");        PrintOption(27, "[MEM] Memento"); std::cout << "\n";
        PrintOption(28, "[VIS] Visitor");         PrintOption(29, "[INT] Interpreter"); std::cout << "\n\n";

        // ======================== FOOTER ========================
        SetColor(EColor::Red);
        std::cout << std::setfill('=') << std::setw(60) << "" << std::setfill(' ') << "\n";
        SetColor(EColor::BrightWhite);
        std::cout << "  0. ";
        SetColor(EColor::Gray);
        std::cout << "End Demo\n";
        SetColor(EColor::Red);
        std::cout << std::setfill('=') << std::setw(60) << "" << std::setfill(' ') << "\n";
        HFL::SetColor(HFL::EColor::White);
    }

    int GetValidMenuInput(int MaxOption)
    {
        SetColor(EColor::Magenta);
        int Choice = -1;
        while (true)
        {
            std::cout << "\nSelection: ";
            std::cin >> Choice;

            if (std::cin.fail())
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input.\n";
                continue;
            }

            if (Choice >= 0 && Choice <= MaxOption)
            {
                return Choice;
            }
            else
            {
                std::cout << "Invalid selection.\n";
            }
        }
    }

    void ClearScreen()
    {
        #ifdef _WIN32
                std::system("cls");
        #else
                std::system("clear");
        #endif
    }

    void WaitForInput()
    {
        SetColor(EColor::Magenta);
        std::cout << "\n[Press Enter to continue...]";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (std::cin.gcount() == 0 && std::cin.eof())
        {
            std::cin.clear();
        }
    }

    void PrintHeader(const std::string& Title)
    {
        SetColor(EColor::Blue);
        std::cout << std::setfill('=') << std::setw(60) << "" << std::setfill(' ') << "\n";
        SetColor(EColor::BrightWhite);
        std::cout << std::setw((60 + Title.length()) / 2) << std::right << Title << "\n";
        SetColor(EColor::Blue);
        std::cout << std::setfill('=') << std::setw(60) << "" << std::setfill(' ') << "\n\n";
        SetColor(EColor::White);
    }

    void PrintSection(const std::string& Title, EColor Color)
    {
        SetColor(Color);
        std::cout << std::setw((60 + Title.length()) / 2) << std::right << Title << "\n";
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

    // =========================================================================
    // CROSS-PLATFORM SAVING
    // =========================================================================
    // Windows: C:/Users/Name/AppData/Roaming/GameDesignPatterns/SRP/
    // Linux/Mac: ~/.local/share/GameDesignPatterns/SRP/
    // =========================================================================
    std::filesystem::path GetSaveDirectory(const std::string& PrincipleNamespace)
    {
        std::filesystem::path SavePath;

        // Determines the OS-specific base directory
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

        // Appends Project Root and Subfolder
        SavePath /= "GameDesignPatterns";
        SavePath /= PrincipleNamespace;

        // Creates the directory structure
        if (!std::filesystem::exists(SavePath))
        {
            std::filesystem::create_directories(SavePath);
            std::cout << "[System] Created save directory at: " << SavePath << "\n";
        }

        return SavePath;
    }

    void Wait(float Seconds)
    {
        // Converts float seconds to a chrono duration and sleeps
        std::this_thread::sleep_for(std::chrono::duration<float>(Seconds));
    }
    void WaitDots(float Seconds)
    {
        for (int i = 0; i < 3; ++i) { HFL::Wait(Seconds); std::cout << "."; }
        std::cout << '\n';
    }
}