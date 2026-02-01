#include "Behavioral/Interpreter.h"

namespace INT
{
    // =========================================================================
    // THE RECURSIVE PARSER (The Tree Builder)
    // ROLE: A factory function that maps JSON 'Type' keys to C++ Classes.
    // This allows for infinite nesting without changing the core engine code.
    // =========================================================================
    std::unique_ptr<Expression> ParseCondition(const json& data)
    {
        // Identify the 'Type' of the current JSON node
        std::string type = data.at("Type").get<std::string>();

        // TERMINAL EXPRESSIONS (The Leaves)
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

        // NON-TERMINAL EXPRESSIONS (The Branches)
        // Magic happens here: ParseCondition is called RECURSIVELY on child nodes.
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

        return nullptr;
    }

    // =========================================================================
    // CONTEXT IMPLEMENTATION
    // =========================================================================

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

    // =========================================================================
    // DEMO IMPLEMENTATION
    // =========================================================================

    void RunDemo()
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // ======================== INTRODUCTION ========================
        HFL::ClearScreen();
        HFL::PrintHeader("INTERPRETER DESIGN PATTERN");

        HFL::PrintSection("THE DEFINITION");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "Given a language, define a representation for its grammar along with an\n"
            << "interpreter that uses the representation to interpret sentences in the language.\n\n";

        HFL::PrintSection("THE GOAL");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Interpreter Pattern is about 'Logic as Data'. It turns\n"
            << "external strings or JSON files into a live 'Logic Tree'. This tree can\n"
            << "poll the game state to evaluate complex, nested conditions without\n"
            << "requiring the logic to be hard-coded into the engine.\n\n";

        HFL::PrintSection("THE EXAMPLE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "This demonstration features a Data-Driven Quest System using four components:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE PLAYER CONTEXT:  ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The 'Database' containing live stats (Level, Gold, Kills).\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] TERMINAL EXPRESSIONS:";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Leaf nodes that check single facts (e.g., 'Gold > 100').\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] NON-TERMINAL EXPR:   ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Grammar (AND, OR, NOT) that joins nodes together.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE RECURSIVE PARSER:";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The 'Compiler' that reads JSON and assembles the object tree.\n\n";

        HFL::PrintSection("THE BENEFIT");
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] DESIGNER FREEDOM: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Designers can create new quest types by just editing a JSON file.\n";
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] RECURSIVE POWER:  ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Logic can be nested to any depth (e.g., A and (B or (not C))).\n";
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] READABILITY:      ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The logic tree can describe itself in plain English for the UI.\n";

        HFL::WaitForInput();

        // ======================== THE ARCHITECTURE ========================
        HFL::ClearScreen();
        HFL::PrintHeader("THE ARCHITECTURE");

        HFL::PrintSection("THE 'LOGIC TREE'");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The system works by building a recursive tree structure. It asks\n"
            << "the root node to 'Interpret', it triggers a chain reaction that filters\n"
            << "down through the branches to the leaves, returning a final boolean.\n\n";

        HFL::PrintSection("IMPLEMENTATION");

        // ======================== TERMINAL NODES ========================
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] Terminal Expressions (The Facts)\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ROLE:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The end-points of the tree. They talk directly to the Context.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    EVALUATION:     ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Simple comparisons (Level >= 10, HasItem == true).\n\n";

        // ======================== NON-TERMINAL NODES ========================
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] Non-Terminal Expressions (The Gates)\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ROLE:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The logic gates. They do not know about the Context details.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    RECURSION:      ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "They only care if their 'Child' expressions return true or false.\n\n";

        // ======================== THE PARSER ========================
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] The Parser (The Factory)\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ROLE:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Converts raw JSON 'Types' into actual C++ object instances.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ENCAPSULATION:  ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The game loop never sees the classes; it only sees the 'Expression' interface.\n\n";

        HFL::SetColor(HFL::EColor::White);
        HFL::WaitForInput();

        // ======================== INITIALIZATION ========================
        std::ifstream file("Quest_Data.json");
        if (!file.is_open()) {
            HFL::SetColor(HFL::EColor::Red);
            std::cout << "[Error] Quest_Data.json not found!\n";
            return;
        }
        json fullData = json::parse(file);

        Context PlayerCtx;
        std::unordered_map<std::string, std::unique_ptr<Expression>> QuestSystem;

        for (auto& [key, value] : fullData["Quests"].items()) {
            QuestSystem[key] = ParseCondition(value);
        }

        // ======================== GAME LOOP ========================
        while (true)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("QUEST LOG & PLAYER STATE");

            // --- PLAYER CONTEXT DISPLAY ---
            HFL::PrintSection("PLAYER STATS");
            HFL::SetColor(HFL::EColor::White);
            std::cout << "  Level:         "; HFL::SetColor(HFL::EColor::Green); std::cout << PlayerCtx.Level << "\n";
            HFL::SetColor(HFL::EColor::White);
            std::cout << "  Gold:          "; HFL::SetColor(HFL::EColor::Green); std::cout << PlayerCtx.Gold << "\n";
            HFL::SetColor(HFL::EColor::White);
            std::cout << "  Goblins Slain: "; HFL::SetColor(HFL::EColor::Green); std::cout << PlayerCtx.KillCount["Goblin"] << "\n\n";

            // --- EVALUATION SECTION ---
            HFL::PrintSection("ACTIVE QUEST EVALUATION");

            auto DisplayQuest = [&](const std::string& ID, const std::string& Name) {
                if (QuestSystem.find(ID) == QuestSystem.end()) return false;

                bool bDone = PlayerCtx.IsQuestDone(ID);
                bool bReady = QuestSystem[ID]->Interpret(PlayerCtx);

                if (bDone) HFL::SetColor(HFL::EColor::Green);
                else if (bReady) HFL::SetColor(HFL::EColor::Cyan);
                else HFL::SetColor(HFL::EColor::Gray);

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
                std::cout << "\n  You have mastered the Interpreter Pattern!\n";
                HFL::WaitForInput();
                break;
            }

            // --- INTERACTION ---
            HFL::PrintSection("COMMANDS");
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [1] "; HFL::SetColor(HFL::EColor::White); std::cout << "TRAIN\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [2] "; HFL::SetColor(HFL::EColor::White); std::cout << "FIGHT\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [3] "; HFL::SetColor(HFL::EColor::White); std::cout << "LOOT\n";

            if (bCanIntro) { HFL::SetColor(HFL::EColor::Cyan); std::cout << " [4] FINISH 'Basic Training'\n"; }
            if (bCanGoblin) { HFL::SetColor(HFL::EColor::Cyan); std::cout << " [5] FINISH 'Goblin Slayer'\n"; }
            if (bCanSecret) { HFL::SetColor(HFL::EColor::Cyan); std::cout << " [6] FINISH 'The Pacifist'\n"; }
            if (bCanLegend) { HFL::SetColor(HFL::EColor::Cyan); std::cout << " [7] FINISH 'Become a Legend'\n"; }

            HFL::SetColor(HFL::EColor::Green);
            std::cout << "\n [0] "; HFL::SetColor(HFL::EColor::White); std::cout << "CONTINUE\n\n";

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

        // ======================== CONCLUSION ========================
        HFL::ClearScreen();
        HFL::PrintHeader("CONCLUSION");

        HFL::PrintSection("ARCHITECTURE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The implementation of the Interpreter Pattern confirms the following:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] LOGICAL ABSTRACTION:   ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Game Loop only interacts with the 'Expression' base class. It has\n"
            << "    no knowledge of whether a quest is a simple check or a complex tree.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] RECURSIVE EVALUATION:  ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "By nesting Non-Terminal nodes (AND, OR, NOT), it can represent any\n"
            << "    boolean requirement without ever writing a new 'if' statement in C++.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] DATA-DRIVEN WORKFLOW:  ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The 'Grammar' is hard-coded, but the 'Sentences' (the quests) are pure\n"
            << "    JSON. This allows for live logic updates without recompiling the game.\n\n";

        HFL::PrintSection("SUMMARY");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The Interpreter Pattern ensures that software is:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] EXTENSIBLE: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Adding a new condition (e.g. TimeLimitExpression) only requires one class.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] TRANSPARENT:";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Using ToString(), the system can explain its requirements to players automatically.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] COMPLETE:   ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "With AND, OR, and NOT, the system is logically complete and can handle\n"
            << "    any combination of game state variables.\n\n";

        HFL::SetColor(HFL::EColor::White);
        HFL::WaitForInput();
    }
}