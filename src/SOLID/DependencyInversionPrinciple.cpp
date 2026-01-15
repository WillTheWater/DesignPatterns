#include "SOLID/DependencyInversionPrinciple.h"

namespace DIP
{
    // =========================================================================
    // LOW LEVEL IMPLEMENTATIONS
    // =========================================================================

    // =========================================================================
    // LOW LEVEL MODULE 1: TEXT FILE STORAGE
    // =========================================================================
    // This class implements the ISaveSystem interface to save data
    // as a human-readable text file.
    // =========================================================================

    TextFileSave::TextFileSave()
    {
        // Uses HelperFunctionLibrary to find a cross-platform save directory.
        // This ensures it works on Windows, Mac, and Linux without hardcoding paths.
        std::filesystem::path Dir = HFL::GetSaveDirectory("DIP");

        // This defines the filename here.
        FilePath = (Dir / "score_data.txt").string();
    }

    void TextFileSave::SaveData(int Score) const
    {
        // std::ofstream opens a file for writing.
        // If the file doesn't exist, it creates it.
        // If it does exist, it overwrites it.
        std::ofstream File(FilePath);

        if (File.is_open())
        {
            // Text mode handles formatting automatically.
            // It write a label so we konw what the "data" means.
            File << "Player Score: " << Score << "\n";

            std::cout << ">> [TextFile] Saved score to: " << FilePath << "\n";
        }
        else
        {
            // In real context, it might throw an exception.
            // This helps pinpoint future errors
            std::cerr << ">> [TextFile] Error: Could not open file for writing.\n";
        }
    }

    int TextFileSave::LoadData() const
    {
        // std::ifstream opens a file for reading.
        std::ifstream File(FilePath);
        int LoadedScore = 0;

        if (File.is_open())
        {
            std::string Label1;
            std::string Label2;

            // The >> operator reads data based on the type.
            // Since LoadedScore is an int, it reads the number automatically.
            // It skips over the text label automatically (until it hits whitespace/newline).
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
    // LOW LEVEL MODULE 2: BINARY FILE STORAGE
    // =========================================================================
    // This class implements the ISaveSystem interface to save data
    // as raw binary (machine-readable only).
    // =========================================================================

    BinaryFileSave::BinaryFileSave()
    {
        // Uses HelperFunctionLibrary to find a cross-platform save directory.
        std::filesystem::path Dir = HFL::GetSaveDirectory("DIP");

        // Binary files often use .bin or .dat extension.
        FilePath = (Dir / "score_data.bin").string();
    }

    void BinaryFileSave::SaveData(int Score) const
    {
        // IMPORTANT: std::ios::binary flag.
        // This prevents the OS from converting newlines (\n) to OS-specific formats (\r\n).
        // It ensures it writes the raw bytes of the integer.
        std::ofstream File(FilePath, std::ios::binary);

        if (File.is_open())
        {
            // reinterpret_cast<const char*>
            // A C++ specific cast that treats the memory of 'Score' (an int)
            // as a block of raw bytes (char*).
            // sizeof(Score) tells the function to write exactly 4 bytes (on a 32-bit int).
            // This is much faster than text conversion.
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
            // We read exactly sizeof(int) bytes directly into the memory address of LoadedScore.
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
    // =========================================================================
    // This class implements the ISaveSystem interface to simulate sending
    // data over a network. In a real engine, this would use HTTP (libcurl).
    // =========================================================================

    CloudServerSave::CloudServerSave()
    {
        // How to save a JSON file locally to demonstrate serialization.
        // In a real case, this file would be uploaded via HTTP.
        std::filesystem::path Dir = HFL::GetSaveDirectory("DIP");
        FilePath = (Dir / "cloud_save.json").string();
    }

    void CloudServerSave::SaveData(int Score) const
    {
        // 1. CONSTRUCT JSON STRING
        // In a real case, use a library like 'JsonCpp' or 'RapidJSON'.
        // For this demo, it's just a string using stringstream.
        std::stringstream ss;
        ss << "{\n";                             //
        ss << "  \"score\": " << Score << "\n";  // This Is a FAKE JSON Do not use
        ss << "}";                               //

        std::string JSONString = ss.str();

        // 2. SIMULATE NETWORK PROCESS
        std::cout << ">> [CloudServer] Serializing game state to JSON...\n";
        std::cout << ">> [CloudServer] Connecting to api.game-server.com...\n";
        std::cout << ">> [CloudServer] Uploading data...\n";

        // 3. WRITE TO DISK
        // Even though it's a 'Cloud' save, we save the JSON file locally.
        // To allow the 'Load' function to work.
        // In the real case it would be uploaded to cloud.
        std::ofstream File(FilePath);
        if (File.is_open())
        {
            File << JSONString;
            std::cout << ">> [CloudServer] Upload Successful!\n";
            std::cout << ">> [CloudServer] Local cache saved to: " << FilePath << "\n";
        }
        else
        {
            std::cerr << ">> [CloudServer] Error: Failed to save local cache.\n";
        }
    }

    int CloudServerSave::LoadData() const
    {
        std::ifstream File(FilePath);
        if (!File.is_open())
        {
            std::cout << ">> [CloudServer] No local cache found. Fetching from remote server...\n";
            // In a real case, this is where you would use 'libcurl' to download.
            // For demo, we just return 0.
            return 0;
        }

        std::cout << ">> [CloudServer] Loading from local cache (" << FilePath << ")...\n";

        // 4. PARSE JSON
        // Manually parse the string to extract the number.
        // Format: {"score": 100}

        std::string Line;
        int Score = 0;
        bool bFoundScore = false;

        while (std::getline(File, Line))
        {
            // Looks for the line containing '"score":'
            if (Line.find("\"score\"") != std::string::npos)
            {
                // Find the colon ':'
                size_t ColonPos = Line.find(':');

                if (ColonPos != std::string::npos)
                {
                    // Get the substring after the colon and convert it to an int
                    std::string ValueStr = Line.substr(ColonPos + 1);
                    try
                    {
                        Score = std::stoi(ValueStr);
                        bFoundScore = true;
                    }
                    catch (...)
                    {
                        // Handle bad format
                        std::cout << ">> [CloudServer] Error: Invalid JSON format.\n";
                    }
                }
            }
        }

        if (bFoundScore)
        {
            std::cout << ">> [CloudServer] Sync complete. Score: " << Score << "\n";
        }

        return Score;
    }

    std::string CloudServerSave::GetDescription() const
    {
        return "Cloud Server (JSON Format)";
    }

    // =========================================================================
    // HIGH LEVEL IMPLEMENTATION
    // =========================================================================
    GameEngine::GameEngine() : Score(0), Storage(nullptr) {}

    void GameEngine::PlayGame()
    {
        std::cout << "\n----------------------------------------\n";
        std::cout << "           Guess the Number!\n";
        std::cout << "----------------------------------------\n";
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
                std::cout << "\nCORRECT! You Win!\n";
                Score = 100;
            }
            else
            {
                std::cout << "\nWRONG! It was " << MagicNumber << ".\n";
                Score = 100 - (std::abs(MagicNumber - Guess));
            }
        }
        std::cout << "----------------------------------------\n";
    }

    void GameEngine::SaveGame() const
    {
        if (Storage == nullptr)
        {
            std::cout << "[Error] No storage system selected!\n";
            return;
        }

        std::cout << "\n>> GameEngine: Saving...\n";
        // The High Level Engine calls SaveGame. But it doesn't know
        // how writing a file or sending a network packet works.
        // That belongs only to the Modules.
        Storage->SaveData(Score);
    }

    // =========================================================================
    // DEMO IMPLEMENTATION
    // =========================================================================
    void RunDemo()
    {
        // Clear buffer
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // --- STEP 1: INTRODUCTION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Dependency Inversion Principle (DIP)");

        std::cout << "Definition:\n";
        std::cout << "Depend on abstractions, not on concretions.\n";
        std::cout << "Invert the dependency arrow to point towards the interface.\n\n";

        std::cout << "In This Demo:\n";
        std::cout << "You will play a mini-game to generate a score.\n";
        std::cout << "Then, you will dynamically select HOW to save that score.\n";
        std::cout << "The Game Engine never knows how it's saved.\n";

        HFL::WaitForInput();

        // --- STEP 2: THE SETUP ---
        HFL::ClearScreen();
        HFL::PrintHeader("Step 1: The Architecture");

        std::cout << "We have defined the (Interface) in the High Level.\n\n";

        std::cout << "1. ISaveSystem (The Interface):\n";
        std::cout << "   - Defines 'SaveData' and 'LoadData'.\n";
        std::cout << "   - Created by the GameEngine.\n\n";

        std::cout << "2. Low Level Modules:\n";
        std::cout << "   - TextFile: Implements Interface to write .txt files.\n";
        std::cout << "   - BinaryFile: Implements Interface to write .bin bytes.\n";
        std::cout << "   - CloudServer: Implements Interface to send .JSON packets.\n\n";

        std::cout << "The Inversion:\n";
        std::cout << "   The Engine creates the 'Job'.\n";
        std::cout << "   The Modules 'apply' for the job.\n";

        HFL::WaitForInput();

        // --- STEP 3: INTERACTIVE SYSTEM ---

        // Create the Low Level Modules (The Workers)
        TextFileSave TextStorage;
        BinaryFileSave BinaryStorage;
        CloudServerSave CloudStorage;

        // Create the High Level Module (The Boss)
        GameEngine MyEngine;

        bool InDemo = true;
        while (InDemo)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("DIP Engine System");

            std::cout << "Current Score: " << MyEngine.GetScore() << "\n\n";

            std::cout << "Select an Action:\n";
            std::cout << "1. Play Game (Generate Score)\n";
            std::cout << "2. Save Game (Choose Storage)\n";
            std::cout << "3. Load Game (Choose Storage)\n";
            std::cout << "0. Exit Demo\n";
            std::cout << "\nChoice: ";

            int Choice;
            std::cin >> Choice;

            if (std::cin.fail()) { std::cin.clear(); std::cin.ignore(); continue; }

            if (Choice == 0) break;

            // --- PLAY GAME ---
            if (Choice == 1)
            {
                MyEngine.PlayGame();
                HFL::WaitForInput();
            }

            // --- SAVE GAME ---
            else if (Choice == 2)
            {
                std::cout << "\nSelect Storage Provider:\n";
                std::cout << "1. " << TextStorage.GetDescription() << "\n";
                std::cout << "2. " << BinaryStorage.GetDescription() << "\n";
                std::cout << "3. " << CloudStorage.GetDescription() << "\n";
                std::cout << "\nStorage Choice: ";

                int StorageChoice;
                std::cin >> StorageChoice;

                if (std::cin.fail()) { std::cin.clear(); std::cin.ignore(); continue; }

                ISaveSystem* SelectedSaver = nullptr;

                if (StorageChoice == 1) SelectedSaver = &TextStorage;
                else if (StorageChoice == 2) SelectedSaver = &BinaryStorage;
                else if (StorageChoice == 3) SelectedSaver = &CloudStorage;

                if (SelectedSaver)
                {
                    // INJECTION POINT
                    MyEngine.SetSaveSystem(SelectedSaver);
                    MyEngine.SaveGame();
                }
                HFL::WaitForInput();
            }

            // --- LOAD GAME ---
            else if (Choice == 3)
            {
                std::cout << "\nSelect Storage Provider:\n";
                std::cout << "1. " << TextStorage.GetDescription() << "\n";
                std::cout << "2. " << BinaryStorage.GetDescription() << "\n";
                std::cout << "3. " << CloudStorage.GetDescription() << "\n";
                std::cout << "\nStorage Choice: ";

                int StorageChoice;
                std::cin >> StorageChoice;

                if (std::cin.fail()) { std::cin.clear(); std::cin.ignore(); continue; }

                ISaveSystem* SelectedSaver = nullptr;

                if (StorageChoice == 1) SelectedSaver = &TextStorage;
                else if (StorageChoice == 2) SelectedSaver = &BinaryStorage;
                else if (StorageChoice == 3) SelectedSaver = &CloudStorage;

                if (SelectedSaver)
                {
                    MyEngine.SetSaveSystem(SelectedSaver);
                    // Simple demo of load (updates score)
                    std::cout << ">> Loading data into Engine...\n";
                    int LoadedScore = SelectedSaver->LoadData();
                    MyEngine.SetScore(LoadedScore);
                    MyEngine.SetSaveSystem(nullptr);
                }
                HFL::WaitForInput();
            }
        }

        // --- STEP 4: CONCLUSION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Conclusion");

        std::cout << "Summary of DIP:\n\n";
        std::cout << "1. The GameEngine (High Level) is never affected.\n";
        std::cout << "   It didn't need 'SaveTextFile' or 'UploadToCloud'.\n\n";

        std::cout << "2. We Inverted the Dependency.\n";
        std::cout << "   The Engine defined 'ISaveSystem'.\n";
        std::cout << "   The Files and Cloud implemented it.\n\n";

        std::cout << "3. Constructor/Setter Injection.\n";
        std::cout << "   We 'injected' the storage choice at runtime.\n";
        std::cout << "   The Engine was flexible and open to change.\n\n";

        std::cout << std::setw(40) << "End of Demo\n";
        HFL::WaitForInput();
    }
}