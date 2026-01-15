#include "Behavioral/ChainOfResponsibility.h"

namespace COR
{
    // =========================================================================
    // DATA IMPLEMENTATION
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
        std::cout << "   " << FullName << "\n";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "   " << Quality << " " << BaseType << " (" << Rarity << ")\n";

        // Main Stat
        HFL::SetColor(HFL::EColor::White);
        std::cout << "   - " << MainStatName << ": " << MainStatValue << "\n";

        // Affix Attributes
        HFL::SetColor(HFL::EColor::White);
        for (const auto& p : Prefixes) std::cout << "   - " << p.Attribute << "\n";
        for (const auto& s : Suffixes) std::cout << "   - " << s.Attribute << "\n";

        HFL::SetColor(HFL::EColor::White);
    }

    // Affix Pools
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
    // HANDLER LOGIC
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
        // Using float for more granular percentage rolls
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
        int Count = (Item.Rarity == "Magic") ? 1 : (Item.Rarity == "Rare") ? 2 : 0;

        for (int i = 0; i < Count; ++i)
        {
            // Pick random Prefix from Map using modern random
            auto itPre = PrePool.begin();
            std::advance(itPre, HFL::GetRandom<int>(0, static_cast<int>(PrePool.size()) - 1));
            Item.Prefixes.push_back({ itPre->first, itPre->second });

            // Pick random Suffix from Map using modern random
            auto itSuf = SufPool.begin();
            std::advance(itSuf, HFL::GetRandom<int>(0, static_cast<int>(SufPool.size()) - 1));
            Item.Suffixes.push_back({ itSuf->first, itSuf->second });
        }

        ItemHandler::Handle(Item);
    }

    // =========================================================================
    // DEMO IMPLEMENTATION: Chain of Responsibility
    // =========================================================================
    void RunDemo()
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // --- STEP 1: INTRODUCTION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Chain of Responsibility Pattern");

        HFL::SetColor(HFL::EColor::BrightYellow);
        std::cout << "Definition:\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "Avoid coupling the sender of a request to its receiver by giving more\n";
        std::cout << "than one object a chance to handle the request. Chain the receiving\n";
        std::cout << "objects and pass the request along the chain.\n\n";

        std::cout << "In This Demo:\n";
        std::cout << "It simulates an 'Item Assembly Line' (ARPG loot factory).\n";
        std::cout << "A blank 'LootItem' object is passed through a sequence of handlers.\n";
        std::cout << "Each handler modifies the item before passing it to the next link.\n";

        HFL::WaitForInput();

        // --- STEP 2: THE ROLES ---
        HFL::ClearScreen();
        HFL::PrintHeader("Step 1: The Roles");

        HFL::SetColor(HFL::EColor::BrightYellow);
        std::cout << "1. The Context (LootItem):\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "   - The object being built. It contains data but no logic.\n\n";

        HFL::SetColor(HFL::EColor::BrightYellow);
        std::cout << "2. The Base Handler (ItemHandler):\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "   - An interface that defines 'SetNext()' and 'Handle()'.\n";
        std::cout << "   - It ensures every link in the chain knows who follows it.\n\n";

        HFL::SetColor(HFL::EColor::BrightYellow);
        std::cout << "3. Concrete Handlers (Base, Quality, Rarity, Affix):\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "   - Specialized workers. One picks the item type, one rolls rarity,\n";
        std::cout << "     and one adds magical properties based on that rarity.\n";

        HFL::WaitForInput();



        // ------------------------------------------------------------------------
        // CONSTRUCTING THE CHAIN (The Assembly Line)
        // ------------------------------------------------------------------------
        // A shared_ptr to manage the lifetime of the handlers.
        // By linking them together, it creates a unidirectional flow of data.
        // ------------------------------------------------------------------------
        auto Base = std::make_shared<BaseTypeHandler>();
        auto Quality = std::make_shared<QualityHandler>();
        auto Rarity = std::make_shared<RarityHandler>();
        auto Affixes = std::make_shared<AffixHandler>();

        // Set the order of operations:
        // Base -> Quality -> Rarity -> Affixes -> Null (End)
        Base->SetNext(Quality);
        Quality->SetNext(Rarity);
        Rarity->SetNext(Affixes);
        // ------------------------------------------------------------------------

        // --- STEP 3: INTERACTIVE LOOP ---
        bool bRunning = true;
        while (bRunning)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("Loot Factory: Assembly Line");

            std::cout << "1. Kill Monster\n";
            std::cout << "0. Exit Demo\n\n";

            int Choice = HFL::GetValidMenuInput(1);
            if (Choice == 0) break;

            HFL::ClearScreen();
            HFL::PrintHeader("Loot Drop:");

            for (int i = 0; i < 3; ++i)
            {
                // Create a blank request (The "Context")
                LootItem NewItem;

                // Start the chain at the first link.
                // The item will 'bubble' through the handlers automatically.
                Base->Handle(NewItem);

                // Resulting item is now fully formed
                NewItem.Display();
                std::cout << "\n";
                HFL::Wait(0.5f);
            }

            HFL::WaitForInput();
        }

        // --- STEP 4: CONCLUSION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Conclusion");

        std::cout << "Key Takeaways:\n\n";

        HFL::SetColor(HFL::EColor::BrightYellow);
        std::cout << "1. Decoupling:\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "   The Main loop only knows about the 'Base' handler.\n";
        std::cout << "   It doesn't need to know that 'Affixes' or 'Rarity' even exist.\n\n";

        HFL::SetColor(HFL::EColor::BrightYellow);
        std::cout << "2. Single Responsibility (SRP):\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "   If we want to change the math for 'Quality', we only touch\n";
        std::cout << "   the QualityHandler class. The rest of the chain remains safe.\n\n";

        HFL::SetColor(HFL::EColor::BrightYellow);
        std::cout << "3. The Power of Order:\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "   We can re-order the chain at runtime. Want to roll Rarity BEFORE\n";
        std::cout << "   Quality? Just change the 'SetNext()' calls.\n\n";

        std::cout << std::setw(45) << "End of Behavioral Demo\n";
        HFL::WaitForInput();
    }
}