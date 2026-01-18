#include "Behavioral/Interpreter.h"

namespace INT
{
    std::unique_ptr<Expression> ParseCondition(const json& data)
    {
        // 1. Identify the 'Type' of the current JSON node
        std::string type = data.at("Type").get<std::string>();

        // 2. TERMINAL EXPRESSIONS (The Leaves)
        // These are the simple rules that don't have children.
        if (type == "Level") {
            return std::make_unique<LevelExpression>(data.at("Value").get<int>());
        }
        if (type == "Gold") {
            return std::make_unique<GoldExpression>(data.at("Value").get<int>());
        }
        if (type == "Quest") {
            return std::make_unique<QuestExpression>(data.at("Value").get<std::string>());
        }
        if (type == "Kill") {
            return std::make_unique<KillExpression>(
                data.at("Target").get<std::string>(),
                data.at("Value").get<int>()
            );
        }

        // 3. NON-TERMINAL EXPRESSIONS (The Branches)
        // This is where the magic of the Interpreter pattern happens.
        // We call ParseCondition RECURSIVELY on the child nodes.
        if (type == "AND") {
            return std::make_unique<AndExpression>(
                ParseCondition(data.at("Left")),
                ParseCondition(data.at("Right"))
            );
        }
        if (type == "OR") {
            return std::make_unique<OrExpression>(
                ParseCondition(data.at("Left")),
                ParseCondition(data.at("Right"))
            );
        }
        if (type == "NOT") {
            return std::make_unique<NotExpression>(
                ParseCondition(data.at("Bool"))
            );
        }

        return nullptr; // You could throw an error here if the Type is unknown
    }

    void Context::AddKill(const std::string& Type)
    { 
        KillCount[Type]++; 
    }

    void Context::MarkComplete(const std::string& QuestName)
    {
        CompletedQuests[QuestName] = true;
    }

    bool Context::IsQuestDone(const std::string& Name) const
    {
        auto it = CompletedQuests.find(Name);
        return (it != CompletedQuests.end()) && it->second;
    }

    void RunDemo()
    {

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // --- STEP 1: INTRODUCTION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Interpreter Pattern: Data-Driven Quest System");

        std::cout << "Definition:\n";
        std::cout << "Define a representation for its grammar along with an interpreter\n";
        std::cout << "that uses the representation to interpret sentences in the language.\n\n";

        std::cout << "In This Demo:\n";
        std::cout << "The quest logic is in a json file 'Quest_Data.json'.\n";
        std::cout << "The system functions as a mini-compiler that:\n";
        std::cout << "1. Reads raw JSON data strings.\n";
        std::cout << "2. Recursively parses them into an Expression Tree.\n";
        std::cout << "3. Interprets that tree against the live Player Context.\n";

        HFL::WaitForInput();

        // --- STEP 2: THE ARCHITECTURE ---
        HFL::ClearScreen();
        HFL::PrintHeader("THE ARCHITECTURE");

        std::cout << "1. The Context (Player State):\n";
        std::cout << "   Global state: Level, Gold, Kill-Maps, and Completion-Maps.\n\n";

        std::cout << "2. Terminal Expressions (Leaves):\n";
        std::cout << "   The basic 'Facts' (Level, Gold, Kill, Quest-Done).\n\n";

        std::cout << "3. Non-Terminal Expressions (Grammar):\n";
        std::cout << "   Recursive operators: AND, OR, and the new NOT operator.\n\n";

        std::cout << "4. The Recursive Parser:\n";
        std::cout << "   A factory function that maps JSON 'Type' keys to C++ Classes,\n";
        std::cout << "   allowing for infinite nesting without changing code.\n";

        HFL::WaitForInput();

        // --- STEP 3: THE SIMULATION ---
        // 1. Load the JSON file
        std::ifstream file("Quest_Data.json");
        if (!file.is_open()) {
            std::cout << "Error: Quest_Data.json not found!\n";
            return;
        }
        json fullData = json::parse(file);

        // 2. The Context (Player State)
        Context PlayerCtx;

        // 3. Parse all quests from the JSON into a Map for easy access
        std::unordered_map<std::string, std::unique_ptr<Expression>> QuestSystem;

        for (auto& [key, value] : fullData["Quests"].items()) {
            QuestSystem[key] = ParseCondition(value);
        }

        while (true)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("Active Quests");

            // --- PLAYER CONTEXT DISPLAY ---
            std::cout << " [ PLAYER STATS ]\n";
            std::cout << "  Level:         " << PlayerCtx.Level << "\n";
            std::cout << "  Gold:          " << PlayerCtx.Gold << "\n";
            std::cout << "  Goblins Slain: " << PlayerCtx.KillCount["Goblin"] << "\n";
            std::cout << "--------------------------------------------------\n\n";

            // --- EVALUATION SECTION ---
            auto DisplayQuest = [&](const std::string& ID, const std::string& Name) {
                if (QuestSystem.find(ID) == QuestSystem.end()) return false;

                bool bDone = PlayerCtx.IsQuestDone(ID);
                bool bReady = QuestSystem[ID]->Interpret(PlayerCtx);

                if (bDone) HFL::SetColor(HFL::EColor::BrightGreen);
                else if (bReady) HFL::SetColor(HFL::EColor::BrightCyan);

                std::cout << (bDone ? " [COMPLETED] " : (bReady ? " [READY!!]   " : " [LOCKED]    "))
                    << Name << "\n  Req: " << QuestSystem[ID]->ToString() << "\n\n";

                HFL::SetColor(HFL::EColor::White);
                return bReady && !bDone;
                };

            bool bCanIntro = DisplayQuest("Intro", "Basic Training");
            bool bCanGoblin = DisplayQuest("GoblinSlayer", "Goblin Slayer");
            bool bCanSecret = DisplayQuest("SecretQuest", "The Pacifist");
            bool bCanLegend = DisplayQuest("Legend", "Become a Legend");
            if (PlayerCtx.IsQuestDone("Legend"))
            {
                HFL::ClearScreen();
                HFL::PrintHeader("** CONGRATULATIONS **");
                HFL::PrintHeader("You Finished Design Patterns!");
                HFL::WaitForInput();
                break;
            }

            // --- INTERACTION ---
            std::cout << "Actions:\n";
            std::cout << " 1. Train\n";
            std::cout << " 2. Fight Goblin\n";
            std::cout << " 3. Loot\n";

            if (bCanIntro)  std::cout << " 4. Finish 'Basic Training'\n";
            if (bCanGoblin) std::cout << " 5. Finish 'Goblin Slayer'\n";
            if (bCanSecret) std::cout << " 6. Finish 'The Pacifist'\n";
            if (bCanLegend) std::cout << " 7. Finish 'Become a Legend'\n";

            std::cout << " 0. Continue\n\n";

            int Choice = HFL::GetValidMenuInput(7);
            if (Choice == 0) break;

            switch (Choice) {
            case 1: PlayerCtx.Level++; break;
            case 2: PlayerCtx.AddKill("Goblin"); break;
            case 3: PlayerCtx.Gold += HFL::GetRandom(20, 200); break;
            case 4: if (bCanIntro)  PlayerCtx.MarkComplete("Intro"); break;
            case 5: if (bCanGoblin) PlayerCtx.MarkComplete("GoblinSlayer"); break;
            case 6: if (bCanSecret) PlayerCtx.MarkComplete("SecretQuest"); break;
            case 7: if (bCanLegend) PlayerCtx.MarkComplete("Legend"); break;
            }
        }

        // --- STEP 4: CONCLUSION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Conclusion: The Data-Driven Interpreter");

        std::cout << "1. Separation of Concerns:\n";
        std::cout << "   Define the 'Grammar' (AND, OR, NOT),\n";
        std::cout << "   while the JSON defines the 'Content'. With the ability to\n";
        std::cout << "   change the game's rules without changing code.\n\n";

        std::cout << "2. Recursion:\n";
        std::cout << "   By using a Recursive Parser, a designer can nest logic\n";
        std::cout << "   to any depth. An 'AND' can contain an 'OR' which contains\n";
        std::cout << "   a 'NOT'. The code treats them all as simple Expressions.\n\n";

        std::cout << "3. Logical Completeness:\n";
        std::cout << "   With the NOT expression, the quest system\n";
        std::cout << "   can represent any boolean logic requirement.\n\n";

        std::cout << std::setw(40) << "End of Demo\n";
        HFL::WaitForInput();
    }
}