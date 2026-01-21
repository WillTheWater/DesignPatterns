#include "SOLID/DependencyInversionPrinciple.h"

namespace DIP
{
    // =========================================================================
    // LOW LEVEL IMPLEMENTATIONS
    // =========================================================================

    // =========================================================================
    // LOW LEVEL MODULE: TEXT FILE STORAGE
    // ROLE: Implements the contract for human-readable disk storage.
    // =========================================================================

    TextFileSave::TextFileSave()
    {
        // Use HFL to ensure path consistency across different operating systems.
        std::filesystem::path Dir = HFL::GetSaveDirectory("DIP");
        FilePath = (Dir / "score_data.txt").string();
    }

    void TextFileSave::SaveData(int Score) const
    {
        std::ofstream File(FilePath);

        if (File.is_open())
        {
            // Text mode handles formatting and readability automatically.
            File << "Player Score: " << Score << "\n";
            std::cout << ">> [TextFile] Saved score to: " << FilePath << "\n";
        }
        else
        {
            std::cerr << ">> [TextFile] Error: Could not open file for writing.\n";
        }
    }

    int TextFileSave::LoadData() const
    {
        std::ifstream File(FilePath);
        int LoadedScore = 0;

        if (File.is_open())
        {
            std::string Label1, Label2;
            // Skips labels and reads the integer directly.
            File >> Label1 >> Label2 >> LoadedScore;
            std::cout << ">> [TextFile] Loaded score: " << LoadedScore << "\n";
        }
        else
        {
            std::cout << ">> [TextFile] No save file found. Returning 0.\n";
        }

        return LoadedScore;
    }

    std::string TextFileSave::GetDescription() const
    {
        return "Text File (Human readable)";
    }

    // =========================================================================
    // LOW LEVEL MODULE: BINARY FILE STORAGE
    // ROLE: Implements the contract for raw, efficient machine-readable storage.
    // =========================================================================

    BinaryFileSave::BinaryFileSave()
    {
        std::filesystem::path Dir = HFL::GetSaveDirectory("DIP");
        FilePath = (Dir / "score_data.bin").string();
    }

    void BinaryFileSave::SaveData(int Score) const
    {
        // Using std::ios::binary avoids line-ending conversions by the OS.
        std::ofstream File(FilePath, std::ios::binary);

        if (File.is_open())
        {
            // Treat the memory of 'Score' as raw bytes (char*) for speed.
            File.write(reinterpret_cast<const char*>(&Score), sizeof(Score));
            std::cout << ">> [BinaryFile] Wrote raw bytes to: " << FilePath << "\n";
        }
    }

    int BinaryFileSave::LoadData() const
    {
        std::ifstream File(FilePath, std::ios::binary);
        int LoadedScore = 0;

        if (File.is_open())
        {
            File.read(reinterpret_cast<char*>(&LoadedScore), sizeof(LoadedScore));
            std::cout << ">> [BinaryFile] Read raw bytes: " << LoadedScore << "\n";
        }
        else
        {
            std::cout << ">> [BinaryFile] No save file found. Returning 0.\n";
        }

        return LoadedScore;
    }

    std::string BinaryFileSave::GetDescription() const
    {
        return "Binary File (Raw Data)";
    }

    // =========================================================================
    // LOW LEVEL MODULE 3: CLOUD SERVER STORAGE (SIMULATED)
    // ROLE: Implements the contract for JSON-based network storage.
    // =========================================================================

    CloudServerSave::CloudServerSave()
    {
        // Points to the data directory
        FilePath = "cloud_save.json";
    }

    void CloudServerSave::SaveData(int Score) const
    {
        // Create the JSON object
        json SaveObject;
        SaveObject["player_stats"] = {
            {"score", Score},
            {"timestamp", "2026-01-21"} // Example of adding more complex data easily
        };

        std::cout << ">> [CloudServer] Serializing to nlohmann::json...\n";

        // Write to disk (Simulating the local cache of a cloud upload)
        std::ofstream File(FilePath);
        if (File.is_open())
        {
            // The '4' inside dump() provides pretty-printing (4 spaces)
            File << SaveObject.dump(4);
            std::cout << ">> [CloudServer] Upload Successful! Cache: " << FilePath << "\n";
        }
        else
        {
            std::cerr << ">> [CloudServer] Error: Failed to write JSON cache.\n";
        }
    }

    int CloudServerSave::LoadData() const
    {
        std::ifstream File(FilePath);
        if (!File.is_open())
        {
            std::cout << ">> [CloudServer] No cache found. Returning 0.\n";
            return 0;
        }

        try
        {
            // Parse the JSON file
            json LoadedData = json::parse(File);

            // Navigate the structure: data["player_stats"]["score"]
            int Score = LoadedData.at("player_stats").at("score").get<int>();

            std::cout << ">> [CloudServer] JSON Parsed. Syncing Score: " << Score << "\n";
            return Score;
        }
        catch (json::parse_error& e)
        {
            std::cerr << ">> [CloudServer] Parse Error: " << e.what() << "\n";
            return 0;
        }
        catch (json::out_of_range& e)
        {
            std::cerr << ">> [CloudServer] Key Error: Could not find score in JSON.\n";
            return 0;
        }
    }

    std::string CloudServerSave::GetDescription() const
    {
        return "Cloud Server (nlohmann/json)";
    }

    // =========================================================================
    // HIGH LEVEL IMPLEMENTATION: GAME ENGINE
    // =========================================================================

    GameEngine::GameEngine() : Score(0), Storage(nullptr) {}

    void GameEngine::PlayGame()
    {
        HFL::PrintSection("GUESS THE NUMBER");
        std::cout << "I'm thinking of a number between 1 and 100.\n\n";

        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        int MagicNumber = (std::rand() % 100) + 1;

        int Guess = 0;
        std::cout << "Enter your guess: ";
        std::cin >> Guess;

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore();
            std::cout << "Invalid input. Score set to 0.\n";
        }
        else
        {
            if (Guess == MagicNumber)
            {
                HFL::SetColor(HFL::EColor::Green);
                std::cout << "\nCORRECT! You Win!\n";
                Score = 100;
            }
            else
            {
                HFL::SetColor(HFL::EColor::Yellow);
                std::cout << "\nWRONG! It was " << MagicNumber << ".\n";
                Score = std::max(0, 100 - (std::abs(MagicNumber - Guess)));
            }
        }
    }

    void GameEngine::SaveGame() const
    {
        if (!Storage)
        {
            HFL::SetColor(HFL::EColor::Red);
            std::cout << "[Error] No storage system injected!\n";
            return;
        }

        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "\n>> [GameEngine] Requesting save via ISaveSystem interface...\n";
        Storage->SaveData(Score);
    }

    // =========================================================================
    // DEMO IMPLEMENTATION
    // =========================================================================
    void RunDemo()
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // ======================== INTRODUCTION ========================
        HFL::ClearScreen();
        HFL::PrintHeader("DEPENDENCY INVERSION PRINCIPLE");

        HFL::PrintSection("THE DEFINITION");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "High-level modules should not depend on low-level modules.\n"
            << "Both should depend on abstractions (interfaces).\n\n";

        HFL::PrintSection("THE GOAL");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Dependency Inversion Principle (DIP) decouples the 'Policy' of the\n"
            << "program from its 'Implementation'. By depending on a shared contract\n"
            << "rather than concrete classes, the high-level logic remains isolated\n"
            << "from changes in low-level volatile details like databases or APIs.\n\n";

        HFL::PrintSection("THE EXAMPLE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "This demonstration features a decoupled Save System with three layers:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE ABSTRACTION (INTERFACE): ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "ISaveSystem defines the Save/Load contract.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE CONCRETIONS (WORKERS):   ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Text, Binary, and JSON (nlohmann) handlers.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE HIGH-LEVEL (BOSS):       ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The GameEngine that executes saves via injection.\n\n";

        HFL::PrintSection("THE BENEFIT");
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] FLEXIBILITY:   ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Swap local storage for cloud storage at runtime without re-compiling logic.\n";
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] TESTABILITY:   ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Inject 'Mock' save systems to test the engine without hitting the disk.\n";
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] DECOUPLING:    ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Third-party libraries (like nlohmann/json) are hidden from the core engine.\n";

        HFL::WaitForInput();

        // ======================== THE ARCHITECTURE ========================
        HFL::ClearScreen();
        HFL::PrintHeader("THE ARCHITECTURE");

        HFL::PrintSection("THE INVERSION");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Traditionally, the Boss (Engine) would own the Workers (Files). In DIP, this\n"
            << "is inverted. The Boss publishes a 'Job Description' (Interface), and the\n"
            << "Workers must change their shape to fit that description to be hired.\n\n";

        HFL::PrintSection("IMPLEMENTATION");

        // ======================== INTERFACE ========================
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] ISaveSystem (Interface)\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ROLE:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Abstraction. Defines how data flows, not where it goes.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    SCOPE:          ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Pure virtual SaveData() and LoadData().\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    DEPENDENCY:     ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Owns no logic. It is the bridge between Engine and Disk.\n\n";

        // ======================== LOW LEVEL ========================
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] Text / Binary / Cloud (nlohmann/json)\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ROLE:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Low-Level Details. They handle the messy work of bytes and strings.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    SCOPE:          ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Implements the Interface. Uses external libraries for serialization.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    CONSTRAINT:     ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Must follow the ISaveSystem contract exactly to be 'Injectable'.\n\n";

        // ======================== HIGH LEVEL ========================
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] GameEngine (High-Level Module)\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ROLE:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The High-Level Policy. It knows WHEN to save, but never HOW.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    SCOPE:          ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Holds an ISaveSystem pointer. Uses Dependency Injection via setters.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ADVANTAGE:      ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Engine remains 100% clean of file-system or network code.\n\n";

        HFL::SetColor(HFL::EColor::White);
        HFL::WaitForInput();

        // ======================== INITIALIZATION ========================
        TextFileSave TextStorage;
        BinaryFileSave BinaryStorage;
        CloudServerSave CloudStorage;
        GameEngine MyEngine;

        // ======================== SYSTEM LOOP ========================
        while (true)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("GUESSING GAME");

            HFL::SetColor(HFL::EColor::White);
            std::cout << "Current Score: ";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << MyEngine.GetScore() << "\n\n";

            HFL::PrintSection("MENU");
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [1] "; HFL::SetColor(HFL::EColor::White); std::cout << "PLAY\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [2] "; HFL::SetColor(HFL::EColor::White); std::cout << "SAVE\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [3] "; HFL::SetColor(HFL::EColor::White); std::cout << "LOAD\n\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [0] "; HFL::SetColor(HFL::EColor::White); std::cout << "CONTINUE\n";

            int Choice = HFL::GetValidMenuInput(3);
            if (Choice == 0) break;

            if (Choice == 1)
            {
                MyEngine.PlayGame();
                HFL::WaitForInput();
            }
            else if (Choice == 2 || Choice == 3)
            {
                HFL::PrintSection("CHOOSE PROVIDER");
                std::cout << "1. " << TextStorage.GetDescription() << "\n";
                std::cout << "2. " << BinaryStorage.GetDescription() << "\n";
                std::cout << "3. " << CloudStorage.GetDescription() << "\n";

                int Prov = HFL::GetValidMenuInput(3);
                ISaveSystem* Selected = nullptr;
                if (Prov == 1) Selected = &TextStorage;
                else if (Prov == 2) Selected = &BinaryStorage;
                else if (Prov == 3) Selected = &CloudStorage;

                if (Selected)
                {
                    MyEngine.SetSaveSystem(Selected);
                    if (Choice == 2)
                    {
                        MyEngine.SaveGame();
                    }
                    else
                    {
                        std::cout << "\n>> Engine: Pulling data through interface...\n";
                        MyEngine.SetScore(Selected->LoadData());
                    }
                }
                HFL::WaitForInput();
            }
        }

        // ======================== CONCLUSION ========================
        HFL::ClearScreen();
        HFL::PrintHeader("CONCLUSION");

        HFL::PrintSection("ARCHITECTURE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The strict adherence to Dependency Inversion confirms the following:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] ABSTRACTION OWNERSHIP: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The 'GameEngine' owns the 'ISaveSystem' interface. Low-level\n"
            << "    modules (Text/Binary/Cloud) must adapt to the engine's needs.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] PLUGGABLE INFRASTRUCTURE: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Integrate a real JSON library (nlohmann) and updated the storage\n"
            << "    logic without touching a single line of the GameEngine's core code.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] REDUCED COUPLING: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The engine remains 'blind' to specific headers like <fstream> or\n"
            << "    <nlohmann/json.hpp>, preventing 'Header Pollution' in high-level logic.\n\n";

        HFL::PrintSection("SUMMARY");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The Dependency Inversion Principle ensures that software is:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] FLEXIBLE: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The strategy for saving (Local vs Cloud) is a runtime decision.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] RESILIENT: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Changes to low-level details (like file formats) do not break the boss logic.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] SCALABLE: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Adding a new 'DatabaseSave' simply requires a new worker class,\n"
            << "    keeping the existing system open for extension but closed for modification.\n\n";

        HFL::SetColor(HFL::EColor::White);
        HFL::WaitForInput();
    }
}