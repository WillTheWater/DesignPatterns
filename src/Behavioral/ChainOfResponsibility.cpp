#include "Behavioral/ChainOfResponsibility.h"

namespace COR
{
    // =========================================================================
    // DATA IMPLEMENTATION
    // ROLE: Handles the visual representation of the processed request.
    // =========================================================================

    void LootItem::Display() const
    {
        // 1. Determine Color based on Rarity
        HFL::EColor RarityColor = HFL::EColor::BrightWhite;
        if (Rarity == "Magic") RarityColor = HFL::EColor::Blue;
        if (Rarity == "Rare")  RarityColor = HFL::EColor::Yellow;

        // 2. Construct Title: [Prefix] [BaseType] [Suffix]
        std::string TitlePrefix = Prefixes.empty() ? "" : Prefixes[0].Name + " ";
        std::string TitleSuffix = Suffixes.empty() ? "" : " " + Suffixes[0].Name;
        std::string FullName = TitlePrefix + BaseType + TitleSuffix;

        // 3. Render Tooltip
        HFL::SetColor(RarityColor);
        std::cout << "    " << FullName << "\n";

        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "    " << Quality << " " << BaseType << " (" << Rarity << ")\n";

        // Main Stat Display
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    - " << MainStatName << ": " << MainStatValue << "\n";

        // Affix Attributes (Magical Properties)
        HFL::SetColor(HFL::EColor::Cyan);
        for (const auto& p : Prefixes) std::cout << "    - " << p.Attribute << "\n";
        for (const auto& s : Suffixes) std::cout << "    - " << s.Attribute << "\n";

        HFL::SetColor(HFL::EColor::White);
    }

    // =========================================================================
    // STATIC POOL INITIALIZATION
    // =========================================================================

    std::unordered_map<std::string, std::string> AffixHandler::PrePool = {
         {"Fiery", "Adds 5-10 Fire Damage"},
         {"Glacial", "Chills enemies on hit"},
         {"Sturdy", "Increases Block Chance by 5%"},
         {"Ancient", "All stats increased by 10%"},
         {"Glinting", "15% Extra Gold from Monsters"},
         {"Titanic", "+20 to Strength"},
         {"Vexing", "5% chance to confuse targets"},
         {"Godly", "Increases all damage by 20%"},
         {"Volcanic", "Creates a fire nova on kill"},
         {"Ethereal", "Cannot be repaired, massive stats"}
    };

    std::unordered_map<std::string, std::string> AffixHandler::SufPool = {
        {"of the Bear", "+15 to Maximum Health"},
        {"of the Fox", "+10 to Agility"},
        {"of Speed", "10% Faster Move Speed"},
        {"of Ages", "Reduces requirements by 20%"},
        {"of the Moon", "Regenerate Mana over time"},
        {"of Thorns", "Reflects 10 damage to attackers"},
        {"of the Whale", "+50 to Maximum Health"},
        {"of Carnage", "Increases Critical Hit chance"},
        {"of the Deep", "Adds Frost Nova to attacks"},
        {"of Blight", "Deals Poison damage over 5s"}
    };

    // =========================================================================
    // HANDLER LOGIC (The Assembly Line Workers)
    // ROLE: Each class modifies the item and delegates to the next link.
    // =========================================================================

    void BaseTypeHandler::Handle(LootItem& Item)
    {
        std::vector<std::string> Types = { "Sword", "Armor", "Shield", "Gloves", "Ring" };

        // Pick Type using template random
        int TypeIndex = HFL::GetRandom<int>(0, static_cast<int>(Types.size()) - 1);
        Item.BaseType = Types[TypeIndex];

        // Initial Stat Baseline
        if (Item.BaseType == "Sword") { Item.MainStatName = "Damage"; Item.MainStatValue = 10; }
        else if (Item.BaseType == "Armor" || Item.BaseType == "Shield") { Item.MainStatName = "Defense"; Item.MainStatValue = 15; }
        else { Item.MainStatName = "Resistance"; Item.MainStatValue = 5; }

        ItemHandler::Handle(Item);
    }

    void QualityHandler::Handle(LootItem& Item)
    {
        float Roll = HFL::GetRandom<float>(0.0f, 100.0f);

        if (Roll <= 15.0f)
        {
            Item.Quality = "Elite";
            Item.MainStatValue += 10;
        }
        else if (Roll <= 40.0f)
        {
            Item.Quality = "Superior";
            Item.MainStatValue += 5;
        }
        else
        {
            Item.Quality = "Normal";
        }

        ItemHandler::Handle(Item);
    }

    void RarityHandler::Handle(LootItem& Item)
    {
        float Roll = HFL::GetRandom<float>(0.0f, 100.0f);

        if (Roll <= 20.0f)      Item.Rarity = "Rare";
        else if (Roll <= 55.0f) Item.Rarity = "Magic";
        else                    Item.Rarity = "Common";

        ItemHandler::Handle(Item);
    }

    void AffixHandler::Handle(LootItem& Item)
    {
        // Determine how many magical properties to roll based on previous Rarity handler
        int Count = (Item.Rarity == "Magic") ? 1 : (Item.Rarity == "Rare") ? 2 : 0;

        for (int i = 0; i < Count; ++i)
        {
            // Pick random Prefix from Map
            auto itPre = PrePool.begin();
            std::advance(itPre, HFL::GetRandom<int>(0, static_cast<int>(PrePool.size()) - 1));
            Item.Prefixes.push_back({ itPre->first, itPre->second });

            // Pick random Suffix from Map
            auto itSuf = SufPool.begin();
            std::advance(itSuf, HFL::GetRandom<int>(0, static_cast<int>(SufPool.size()) - 1));
            Item.Suffixes.push_back({ itSuf->first, itSuf->second });
        }

        ItemHandler::Handle(Item);
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
        HFL::PrintHeader("CHAIN OF RESPONSIBILITY");

        HFL::PrintSection("THE DEFINITION");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "Avoid coupling the sender of a request to its receiver by giving more\n"
            << "than one object a chance to handle the request. Chain the receiving\n"
            << "objects and pass the request along the chain.\n\n";

        HFL::PrintSection("THE GOAL");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Chain of Responsibility is about 'Decoupled Processing Chains'.\n"
            << "It transforms a massive conditional block into a modular assembly line,\n"
            << "where each link performs a specific transformation on a shared object\n"
            << "before delegating to the next handler in the sequence.\n\n";

        HFL::PrintSection("THE EXAMPLE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "This demonstration features an ARPG Loot Factory with four specialized links:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] BASETYPE HANDLER:   ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Selects the core item (Sword, Ring) and base stats.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] QUALITY HANDLER:    ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Rolls for 'Superior' or 'Elite' stat multipliers.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] RARITY HANDLER:     ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Determines the item tier (Common, Magic, or Rare).\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] AFFIX HANDLER:      ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Adds magical prefixes/suffixes based on the rolled rarity.\n\n";

        HFL::PrintSection("THE BENEFIT");
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] SINGLE RESPONSIBILITY: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Each class handles exactly one part of the loot generation math.\n";
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] OPEN/CLOSED:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Add new loot rules (e.g. Set Items) by adding a link, not changing old code.\n";
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] DYNAMIC PIPELINES:     ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The order of operations can be swapped or truncated at runtime.\n";

        HFL::WaitForInput();

        // ======================== THE ARCHITECTURE ========================
        HFL::ClearScreen();
        HFL::PrintHeader("THE ARCHITECTURE");

        HFL::PrintSection("THE 'ASSEMBLY LINE'");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "In a standard approach, loot generation is a fragile web of if-else logic.\n"
            << "In this pattern, the 'LootItem' request travels through a pipeline where\n"
            << "each handler can either process and pass, or terminate the chain.\n\n";

        HFL::PrintSection("IMPLEMENTATION");

        // ======================== THE CONTEXT ========================
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] The Context (LootItem)\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ROLE:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "A passive data structure representing the 'Request' being handled.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    LIFECYCLE:      ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Modified cumulatively as it passes through each concrete link.\n\n";

        // ======================== THE HANDLERS ========================
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] Concrete Handlers (The Links)\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ROLE:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Autonomous processing units that manage their own logic and delegation.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    DECOUPLING:     ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Links only know about the next interface, not the specific implementation.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ABSTRACTION:    ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The 'ItemHandler' base class automates the pointer delegation logic.\n\n";

        HFL::SetColor(HFL::EColor::White);
        HFL::WaitForInput();

        // ======================== INITIALIZATION ========================
        auto Base = std::make_shared<BaseTypeHandler>();
        auto Quality = std::make_shared<QualityHandler>();
        auto Rarity = std::make_shared<RarityHandler>();
        auto Affixes = std::make_shared<AffixHandler>();

        // Assembly Line: Base -> Quality -> Rarity -> Affixes
        Base->SetNext(Quality);
        Quality->SetNext(Rarity);
        Rarity->SetNext(Affixes);

        // ======================== GAME LOOP ========================
        while (true)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("LOOT FACTORY");

            HFL::PrintSection("MENU");
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [1] "; HFL::SetColor(HFL::EColor::White); std::cout << "KILL MONSTER\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [0] "; HFL::SetColor(HFL::EColor::White); std::cout << "CONTINUE\n\n";

            int Choice = HFL::GetValidMenuInput(1);
            if (Choice == 0) break;

            HFL::ClearScreen();
            HFL::PrintHeader("MONSTER SLAIN! DROPPING ITEMS...");

            for (int i = 0; i < 3; ++i)
            {
                // 1. Create request
                LootItem NewItem;

                // 2. Start the chain
                Base->Handle(NewItem);

                // 3. Display
                NewItem.Display();
                std::cout << "\n";
                HFL::Wait(0.4f);
            }

            HFL::WaitForInput();
        }

        // ======================== CONCLUSION ========================
        HFL::ClearScreen();
        HFL::PrintHeader("CONCLUSION");

        HFL::PrintSection("ARCHITECTURE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The implementation of the Chain of Responsibility confirms the following:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] DECOUPLED INVOCATION:    ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Client only interacts with the first link in the chain.\n"
            << "    It remains oblivious to the existence or count of subsequent handlers.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] LOGICAL ISOLATION:       ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Each handler is a self-contained unit. 'Rarity' logic is physically\n"
            << "    separated from 'Affix' logic, preventing 'God Function' bloat.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] EXTENSIBLE PIPELINES:    ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Adding a 'SeasonPassHandler' or 'LegendaryRollHandler' requires zero\n"
            << "    modification to the existing BaseType or Quality classes.\n\n";

        HFL::PrintSection("SUMMARY");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The Chain of Responsibility Pattern ensures that software is:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] MODULAR:   ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Complex branching is replaced by a linear sequence of objects.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] ADAPTIVE:  ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The 'Assembly Line' can be re-wired at runtime to suit different game modes.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] MAINTAINABLE:";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Bugs are easily isolated to a specific handler class rather than a 500-line switch.\n\n";

        HFL::SetColor(HFL::EColor::White);
        HFL::WaitForInput();
    }
}