#include "Structural/Facade.h"

namespace FAC
{
    // =========================================================================
    // CONCRETE SUBSYSTEMS
    // =========================================================================
    bool Validator::Validate(const GameData& Data) const
    {
        std::cout << "[Validator] Checking Name... ";
        if (Data.Name.empty()) {
            std::cout << "FAILED.\n";
            return false;
        }
        std::cout << "OK.\n";
        return true;
    }

    std::string Serializer::Serialize(const GameData& Data) const
    {
        std::stringstream ss;
        ss << Data.Name << "," << Data.Level << "," << Data.XP << "," << Data.Gold;
        std::cout << "[Serializer] Data formatted to CSV.\n";
        return ss.str();
    }

    GameData Serializer::Deserialize(const std::string& Content) const
    {
        std::stringstream ss(Content);
        std::string name;
        int lvl, gold;
        float xp;

        std::getline(ss, name, ',');
        ss >> lvl;
        ss.ignore();
        ss >> xp;
        ss.ignore();
        ss >> gold;

        return GameData(name, lvl, xp, gold);
    }

    // =========================================================================
    // FACADE IMPLEMENTATION
    // =========================================================================

    bool SaveSystemFacade::Save(int SlotID, const GameData& Data)
    {
        std::cout << "\n--- FACADE: Starting Save Process (Slot " << SlotID << ") ---\n";

        // 1. Instantiate Subsystems
        IValidator* validator = new Validator();
        ISerializer* serializer = new Serializer();
        IStorage* storage = new LocalDiskStorage();

        // 2. Workflow
        if (!validator->Validate(Data))
        {
            std::cout << "Save ABORTED: Validation failed.\n";
            delete validator; delete serializer; delete storage;
            return false;
        }

        std::string dataString = serializer->Serialize(Data);
        std::string filename = "SaveSlot_" + std::to_string(SlotID) + ".sav";

        storage->Write(filename, dataString);

        // Cleanup
        delete validator;
        delete serializer;
        delete storage;

        std::cout << "--- FACADE: Save Complete ---\n\n";
        return true;
    }

    bool SaveSystemFacade::Load(int SlotID, GameData& Data)
    {
        std::cout << "\n--- FACADE: Starting Load Process (Slot " << SlotID << ") ---\n";
        bool LoadSuccessful = false;

        ISerializer* serializer = new Serializer();
        IStorage* storage = new LocalDiskStorage();

        std::string filename = "SaveSlot_" + std::to_string(SlotID) + ".sav";

        // Read data
        std::string rawContent = storage->Read(filename);

        if (!rawContent.empty())
        {
            // Deserialize
            Serializer* Ser = dynamic_cast<Serializer*>(serializer);
            if (Ser)
            {
                Data = Ser->Deserialize(rawContent);

                std::cout << "---------------------------------\n";
                std::cout << "     LOAD SUCCESSFUL!\n";
                std::cout << "---------------------------------\n";
                std::cout << " Name:  " << Data.Name << "\n";
                std::cout << " Level: " << Data.Level << "\n";
                std::cout << " XP:    " << Data.XP << "\n";
                std::cout << " Gold:  " << Data.Gold << "\n";
                std::cout << "---------------------------------\n";
                LoadSuccessful = true;
            }
        }
        else
        {
            std::cout << "Load FAILED: File empty or missing.\n";
            LoadSuccessful = false;
        }

        delete serializer;
        delete storage;

        std::cout << "--- FACADE: Load Complete ---\n\n";
        return LoadSuccessful;
    }

    // =========================================================================
    // DEMO
    // =========================================================================

    void RunDemo()
    {
        // Seed randomness
        std::srand((unsigned int)time(NULL));

        // Clear buffer
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // --- STEP 1: INTRODUCTION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Facade Pattern (Save System)");

        std::cout << "Definition:\n";
        std::cout << "Provide a simplified interface to a complex subsystem.\n";
        std::cout << "Hide complexity behind a single, easy-to-use API.\n\n";

        std::cout << "In This Demo:\n";
        std::cout << "There is a SaveSystemFacade class.\n";
        std::cout << "It hides multiple subsystems involved in saving and loading.\n";
        std::cout << "The client interacts with ONE object instead of many.\n";

        HFL::WaitForInput();

        // --- STEP 2: THE RULES ---
        HFL::ClearScreen();
        HFL::PrintHeader("Step 1: The Architecture");

        std::cout << "To recognize a Facade, there are 4 key ideas:\n\n";

        std::cout << "1. Simple Public Interface:\n";
        std::cout << "   The facade exposes a small number of clear functions.\n";
        std::cout << "   Example: SaveGame() or LoadGame().\n\n";

        std::cout << "2. Hides Subsystem Complexity:\n";
        std::cout << "   The client does NOT talk to serializers, files, or validators.\n";
        std::cout << "   All coordination happens inside the facade.\n\n";

        std::cout << "3. Subsystems Remain Independent:\n";
        std::cout << "   The facade does not replace subsystems.\n";
        std::cout << "   They still exist and can be used directly if needed.\n\n";

        std::cout << "4. Decoupling:\n";
        std::cout << "   Changes to subsystems (e.g., changing from CSV to JSON)\n";
        std::cout << "   typically don't require changes to the client code.\n";

        HFL::WaitForInput();

        // --- STEP 3: INTERACTIVE DEMO ---

        SaveSystemFacade facade;
        bool bIsRunning = true;

        while (bIsRunning)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("Save System Manager");

            std::cout << "1. Save Game\n";
            std::cout << "2. Load Game\n";
            std::cout << "3. Exit Demo\n";
            std::cout << "\n>> ";

            int choice;
            std::cin >> choice;

            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }

            if (choice == 3)
            {
                bIsRunning = false;
                break;
            }

            int slotID;
            std::cout << "Enter Save Slot ID (1-3): ";
            std::cin >> slotID;

            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }

            if (choice == 1)
            {
                std::string name;
                int lvl;

                std::cout << "Enter Character Name: ";
                std::cin >> name;
                std::cout << "Enter Level: ";
                std::cin >> lvl;

                // Generate Random XP (0-100 with 2 decimals)
                float randomXP = static_cast<float>(rand() % 10000) / 100.0f;
                // Generate Random Gold (0-500)
                int randomGold = rand() % 500;

                GameData playerData(name, lvl, randomXP, randomGold);

                std::cout << "\n-- Generating Random Stats --\n";
                std::cout << "   XP: " << randomXP << "\n";
                std::cout << "   Gold: " << randomGold << "\n";

                facade.Save(slotID, playerData);
                HFL::WaitForInput();
            }
            else if (choice == 2)
            {
                GameData loadedData;
                if (facade.Load(slotID, loadedData))
                {
                    /*std::cout << "---------------------------------\n";
                    std::cout << "     LOAD SUCCESSFUL!\n";
                    std::cout << "---------------------------------\n";
                    std::cout << " Name:  " << loadedData.Name << "\n";
                    std::cout << " Level: " << loadedData.Level << "\n";
                    std::cout << " XP:    " << std::fixed << std::setprecision(2) << loadedData.XP << "\n";
                    std::cout << " Gold:  " << loadedData.Gold << "\n";
                    std::cout << "---------------------------------\n";*/
                    HFL::WaitForInput();
                }
                else
                {
                    HFL::WaitForInput();
                }
            }
        }

        // --- STEP 4: CONCLUSION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Conclusion");

        std::cout << "Summary of Facade:\n\n";
        std::cout << "1. A interactable Simplified Interface.\n";
        std::cout << "   Only needs to call 'Save()' and 'Load()'.\n\n";

        std::cout << "2. Complexity was Hidden.\n";
        std::cout << "   The Facade handled Validation, Serialization, and File I/O.\n";
        std::cout << "   To add 'Compression', it would inside\n";
        std::cout << "   the Facade, and the Demo code would remain unchanged.\n\n";

        std::cout << "3. Subsystems are Decoupled.\n";
        std::cout << "   The Validator doesn't know about the Storage.\n";
        std::cout << "   The Serializer doesn't know about the Validator.\n";
        std::cout << "   The Facade acts as the coordinator.\n\n";

        std::cout << std::setw(40) << "End of Demo\n";
        HFL::WaitForInput();
    }
}