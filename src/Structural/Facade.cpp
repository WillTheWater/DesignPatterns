#include "Structural/Facade.h"

namespace FAC
{
    // =========================================================================
    // CONCRETE SUBSYSTEMS (The Low-Level Workers)
    // ROLE: Perform specific, isolated tasks. They do not know about each other.
    // =========================================================================

    bool Validator::Validate(const GameData& Data) const
    {
        std::cout << "[Validator] Checking integrity of " << Data.Name;
        HFL::WaitDots(0.6f);
        if (Data.Name.empty() || Data.Level < 0)
        {
            HFL::SetColor(HFL::EColor::Red);
            std::cout << "FAILED.\n";
            HFL::SetColor(HFL::EColor::Gray);
            return false;
        }
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "OK.\n";
        HFL::SetColor(HFL::EColor::Gray);
        return true;
    }

    std::string JsonSerializer::Serialize(const GameData& Data) const
    {
        // The library knows how to convert GameData directly to a json object
        json J = Data;

        std::cout << "[Serializer] Macro-driven serialization complete.\n";
        return J.dump(4);
    }

    GameData JsonSerializer::Deserialize(const std::string& Content) const
    {
        try
        {
            json J = json::parse(Content);

            // This implicitly calls the generated from_json()
            std::cout << "[Serializer] Macro-driven deserialization successful.\n";
            return J.get<GameData>();
        }
        catch (json::exception& e)
        {
            std::cerr << "[Serializer] Macro Parse Error: " << e.what() << "\n";
            return GameData();
        }
    }

    // =========================================================================
    // STORAGE IMPLEMENTATION
    // =========================================================================

    void LocalDiskStorage::Write(const std::string& Filename, const std::string& Content) const
    {
        std::filesystem::path Path = HFL::GetSaveDirectory("FAC") / Filename;
        std::ofstream File(Path);
        if (File.is_open())
        {
            File << Content;
            std::cout << "[Storage] IO Write successful to: " << Filename << "\n";
        }
    }

    std::string LocalDiskStorage::Read(const std::string& Filename) const
    {
        std::filesystem::path Path = HFL::GetSaveDirectory("FAC") / Filename;
        std::ifstream File(Path);
        if (!File.is_open()) return "";

        std::stringstream Buffer;
        Buffer << File.rdbuf();
        std::cout << "[Storage] IO Read successful from: " << Filename << "\n";
        return Buffer.str();
    }

    // =========================================================================
    // FACADE IMPLEMENTATION (The Master Switch)
    // ROLE: Orchestrate the subsystems into a single, cohesive workflow.
    // =========================================================================

    bool SaveSystemFacade::Save(int SlotID, const GameData& Data)
    {
        HFL::PrintSection("FACADE: INITIATING SAVE WORKFLOW");
        HFL::SetColor(HFL::EColor::Gray);

        // ======================== DEPENDENCY MANAGEMENT ========================
        Validator Check;
        JsonSerializer Format;
        LocalDiskStorage Disk;

        // ======================== LOGIC ========================
        if (!Check.Validate(Data))
        {
            HFL::SetColor(HFL::EColor::Red);
            std::cout << ">> [Facade] Aborting: Data validation failed.\n";
            return false;
        }

        std::string FormattedData = Format.Serialize(Data);
        std::string Filename = "SaveSlot_" + std::to_string(SlotID) + ".sav";

        Disk.Write(Filename, FormattedData);

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "\n>> [Facade] Save complete for Slot " << SlotID << ".\n";
        return true;
    }

    bool SaveSystemFacade::Load(int SlotID, GameData& Data)
    {
        HFL::PrintSection("FACADE: INITIATING LOAD WORKFLOW");
        HFL::SetColor(HFL::EColor::Gray);

        JsonSerializer Format;
        LocalDiskStorage Disk;

        std::string Filename = "SaveSlot_" + std::to_string(SlotID) + ".sav";
        std::string RawData = Disk.Read(Filename);

        if (RawData.empty())
        {
            HFL::SetColor(HFL::EColor::Red);
            std::cout << ">> [Facade] Aborting: No data found in slot " << SlotID << ".\n";
            return false;
        }

        Data = Format.Deserialize(RawData);

        HFL::SetColor(HFL::EColor::Green);
        std::cout << ">> [Facade] Successfully restored " << Data.Name << ".\n";
        return true;
    }

    // =========================================================================
    // DEMO IMPLEMENTATION
    // =========================================================================

    void RunDemo()
    {
        std::srand(static_cast<unsigned int>(time(NULL)));
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // ======================== INTRODUCTION ========================
        HFL::ClearScreen();
        HFL::PrintHeader("FACADE DESIGN PATTERN");

        HFL::PrintSection("THE DEFINITION");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "Provide a unified interface to a set of interfaces in a subsystem.\n"
            << "The Facade defines a higher-level interface that makes the\n"
            << "subsystem easier to use.\n\n";

        HFL::PrintSection("THE GOAL");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Facade Pattern is about 'Simplification through Encapsulation'.\n"
            << "It acts as a front-facing entry point that masks the complexity of\n"
            << "multiple low-level subsystems (Validation, Serialization, Disk I/O),\n"
            << "providing the client with a single 'Master Switch' for complex tasks.\n\n";

        HFL::PrintSection("THE EXAMPLE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "This demonstration features a complex RPG Save System with three hidden workers:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE VALIDATOR:       ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Ensures data integrity before any permanent disk changes.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE JSON SERIALIZER: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Uses nlohmann/json to map RPG stats into a data string.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE STORAGE ENGINE:  ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Handles cross-platform file paths and physical I/O streams.\n\n";

        HFL::PrintSection("THE BENEFIT");
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] LEAST KNOWLEDGE: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The client (Game Engine) has zero dependency on JSON headers or file streams.\n";
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] WORKFLOW SAFETY: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Facade enforces a strict order: Validate -> Serialize -> Write.\n";
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] MAINTENANCE:     ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Updating the JSON library or save format only requires changing one class.\n";

        HFL::WaitForInput();

        // ======================== THE ARCHITECTURE ========================
        HFL::ClearScreen();
        HFL::PrintHeader("THE ARCHITECTURE");



        HFL::PrintSection("THE 'MASTER SWITCH'");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Without a Facade, the Client must manage the lifecycles and interactions\n"
            << "of three different classes just to save the game. With a Facade, the\n"
            << "complexity is 'flattened' into a single, intuitive API call.\n\n";

        HFL::PrintSection("IMPLEMENTATION");

        // ======================== SUBSYSTEMS ========================
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] Hidden Subsystems (The Workers)\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ROLE:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Specialized specialists. They do one thing (Validate or Save) perfectly.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    VISIBILITY:     ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Completely unknown to the high-level Game Logic.\n\n";

        // ======================== THE FACADE ========================
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] SaveSystemFacade (The Unified Interface)\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ROLE:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Orchestrator. It knows which worker to call and in what order.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    SIMPLICITY:     ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Exposes only two methods: Save() and Load().\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    DECOUPLING:     ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Shields the client from the nlohmann/json macro-heavy details.\n\n";

        HFL::SetColor(HFL::EColor::White);
        HFL::WaitForInput();

        // ======================== INITIALIZATION ========================
        SaveSystemFacade SaveSystem;
        GameData ActiveHero;
        bool bIsDataLoaded = false;

        // ======================== GAME LOOP ========================
        while (true)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("CHARACTER CREATOR");

            // ======================== DATA DISPLAY ========================
            HFL::PrintSection("ACTIVE CHARACTER");

            if (!bIsDataLoaded)
            {
                HFL::SetColor(HFL::EColor::Gray);
                std::cout << " [No Character Loaded - Create or Load to begin]\n\n";
            }
            else
            {
                // ======================== IDENTITY & CORE ========================
                HFL::SetColor(HFL::EColor::White);
                std::cout << "  NAME:      "; HFL::SetColor(HFL::EColor::Green);
                std::cout << std::left << std::setw(18) << ActiveHero.Name;
                HFL::SetColor(HFL::EColor::White);
                std::cout << "  LEVEL:     "; HFL::SetColor(HFL::EColor::Green);
                std::cout << ActiveHero.Level << "\n";

                HFL::SetColor(HFL::EColor::White);
                std::cout << "  XP:        "; HFL::SetColor(HFL::EColor::Green);
                std::cout << std::left << std::setw(18) << std::fixed << std::setprecision(1) << ActiveHero.XP;
                HFL::SetColor(HFL::EColor::White);
                std::cout << "  GOLD:      "; HFL::SetColor(HFL::EColor::Green);
                std::cout << ActiveHero.Gold << "\n";

                // ======================== COMBAT ATTRIBUTES ========================
                HFL::SetColor(HFL::EColor::Gray);
                std::cout << "  --------------------------------------------------\n";
                HFL::SetColor(HFL::EColor::White);
                std::cout << "  HEALTH:    "; HFL::SetColor(HFL::EColor::Red);
                std::cout << std::left << std::setw(18) << ActiveHero.Health;
                HFL::SetColor(HFL::EColor::White);
                std::cout << "  MANA:      "; HFL::SetColor(HFL::EColor::Cyan);
                std::cout << ActiveHero.Mana << "\n";

                HFL::SetColor(HFL::EColor::White);
                std::cout << "  STR: " << ActiveHero.Strength << " | AGI: " << ActiveHero.Agility << " | INT: " << ActiveHero.Intelligence << "\n";

                // ======================== EQUIPMENT & WORLD ========================
                HFL::SetColor(HFL::EColor::Gray);
                std::cout << "  --------------------------------------------------\n";
                HFL::SetColor(HFL::EColor::White);
                std::cout << "  WEAPON:    "; HFL::SetColor(HFL::EColor::Yellow);
                std::cout << std::left << std::setw(18) << ActiveHero.PrimaryWeapon;
                HFL::SetColor(HFL::EColor::White);
                std::cout << "  ARMOR:     "; HFL::SetColor(HFL::EColor::Yellow);
                std::cout << ActiveHero.ArmorClass << "\n";

                HFL::SetColor(HFL::EColor::White);
                std::cout << "  POTIONS:   "; HFL::SetColor(HFL::EColor::White);
                std::cout << std::left << std::setw(18) << ActiveHero.PotionsCount;
                HFL::SetColor(HFL::EColor::White);
                std::cout << "  PLAYTIME:  "; HFL::SetColor(HFL::EColor::Gray);
                std::cout << (int)ActiveHero.PlayTimeSeconds << "s\n";

                // ======================== POSITION ========================
                HFL::SetColor(HFL::EColor::White);
                std::cout << "  POSITION:  "; HFL::SetColor(HFL::EColor::Gray);
                std::cout << "[" << std::fixed << std::setprecision(2)
                    << ActiveHero.Position3D[0][0] << ", "
                    << ActiveHero.Position3D[0][1] << ", "
                    << ActiveHero.Position3D[0][2] << "]\n\n";

                HFL::SetColor(HFL::EColor::Gray);
            }

            HFL::PrintSection("COMMANDS");
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [1] "; HFL::SetColor(HFL::EColor::White); std::cout << "CREATE NEW CHARACTER\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [2] "; HFL::SetColor(HFL::EColor::White); std::cout << "SAVE\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [3] "; HFL::SetColor(HFL::EColor::White); std::cout << "LOAD\n\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [0] "; HFL::SetColor(HFL::EColor::White); std::cout << "CONTINUE\n\n";

            int Choice = HFL::GetValidMenuInput(3);
            if (Choice == 0) break;

            if (Choice == 1)
            {
                HFL::PrintSection("CHARACTER CREATION");
                std::string Name;
                std::cout << "Enter Character Name: ";
                std::cin >> Name;

                std::cout << "Generating RPG Stats and World Position";
                HFL::WaitDots(0.5);

                ActiveHero.Name = Name;
                ActiveHero.Level = 1;
                ActiveHero.XP = 0.0f;
                ActiveHero.Gold = 100 + (rand() % 400);

                ActiveHero.Strength = 8 + (rand() % 12);
                ActiveHero.Agility = 8 + (rand() % 12);
                ActiveHero.Intelligence = 8 + (rand() % 12);
                ActiveHero.Health = 100 + (ActiveHero.Strength * 5);
                ActiveHero.Mana = 50 + (ActiveHero.Intelligence * 5);

                ActiveHero.PrimaryWeapon = (ActiveHero.Strength > 15) ? "Club" : "Stick";
                ActiveHero.ArmorClass = 10 + (ActiveHero.Agility / 4);
                ActiveHero.PotionsCount = 1 + (rand() % 3);

                float RandomX = (float)(rand() % 1000) / 10.0f;
                float RandomZ = (float)(rand() % 1000) / 10.0f;
                ActiveHero.Position3D[0] = { RandomX, 0.0f, RandomZ };

                ActiveHero.PlayTimeSeconds = 0.0f;

                bIsDataLoaded = true;

                HFL::SetColor(HFL::EColor::Green);
                std::cout << '\n' << Name << " has been created.\n";

                HFL::WaitForInput();
            }
            else if (Choice == 2)
            {
                if (!bIsDataLoaded) {
                    HFL::SetColor(HFL::EColor::Red);
                    std::cout << "!! Error: No active character to save.\n";
                    HFL::WaitForInput();
                    continue;
                }

                HFL::PrintSection("SLOT SELECTION");
                std::cout << "Save to Slot (1-3): ";
                int SlotID = HFL::GetValidMenuInput(3);

                HFL::SetColor(HFL::EColor::Gray);
                std::cout << "Facade: Orchestrating subsystems"; HFL::WaitDots(0.5);

                if (SaveSystem.Save(SlotID, ActiveHero)) {
                    std::cout << ">> Disk Write Successful.\n";
                }
                HFL::WaitForInput();
            }
            else if (Choice == 3)
            {
                HFL::PrintSection("SLOT SELECTION");
                std::cout << "Load from Slot (1-3): ";
                int SlotID = HFL::GetValidMenuInput(3);

                HFL::SetColor(HFL::EColor::Gray);
                std::cout << "Facade: Accessing Storage and Parsing JSON"; HFL::WaitDots(0.5);

                if (SaveSystem.Load(SlotID, ActiveHero)) {
                    bIsDataLoaded = true;
                    HFL::SetColor(HFL::EColor::Green);
                    std::cout << "\n>> Character restored successfully.\n";
                }
                else {
                    HFL::SetColor(HFL::EColor::Red);
                    std::cout << "\n>> Load Failed: Slot is empty or corrupt.\n";
                }
                HFL::WaitForInput();
            }
        }

        // ======================== CONCLUSION ========================
        HFL::ClearScreen();
        HFL::PrintHeader("CONCLUSION");

        HFL::PrintSection("ARCHITECTURE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The implementation of the Facade Pattern confirms the following:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] COMPLEXITY SHIELDING: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The client handles a single 'GameData' struct. The Facade manages\n"
            << "    the nlohmann/json macro-logic and variable mappings internally.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] SUBSYSTEM ORCHESTRATION: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The 'Validator', 'Serializer', and 'Storage' workers are kept distinct.\n"
            << "    The Facade ensures they collaborate in the correct logical sequence.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] ZERO HEADER POLLUTION: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The client logic remains pure. It has no knowledge of <fstream>,\n"
            << "    <nlohmann/json.hpp>, or cross-platform filesystem paths.\n\n";

        HFL::PrintSection("SUMMARY");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The Facade Pattern ensures that software is:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] SIMPLE:    ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "One call handles everything.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] PROTECTIVE:";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Data corruption is prevented by enforcing validation before disk I/O.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] ADAPTIVE:  ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Swapping JSON for XML or Binary only requires changing the Facade,\n"
            << "    leaving the high-level Game Loop 100% untouched.\n\n";

        HFL::SetColor(HFL::EColor::White);
        HFL::WaitForInput();
    }
}