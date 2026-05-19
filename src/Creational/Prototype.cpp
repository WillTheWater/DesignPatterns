#include "Creational/Prototype.h"

namespace PRO
{
    // =========================================================================
    // BASE CLASS IMPLEMENTATION
    // =========================================================================
    Zombie::Zombie(std::string Name, int Health)
        : Name(Name), Health(Health)
    {
        // Simulates an expensive operation (Loading Mesh/Texture/Sounds)
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << ">> [System] Disk I/O: Loading '" << Name << "' assets into RAM...\n";
    }

    void Zombie::Attack() const
    {
        std::cout << "   *Generic shambling sounds*\n";
    }

    // =========================================================================
    // CONCRETE PROTOTYPES IMPLEMENTATION
    // =========================================================================

    // ======================== CRAWLER ========================
    Crawler::Crawler() : Zombie("Crawler", 20) {}

    std::unique_ptr<Zombie> Crawler::Clone() const
    {
        // DEEP COPY via Copy Constructor:
        // std::make_unique allocates new memory and stamps the Prototype's
        // current state (Name/Health) into the new object instantly.
        return std::make_unique<Crawler>(*this);
    }

    void Crawler::Attack() const { std::cout << "   *Sprints on all fours and bites!*\n"; }

    // ======================== WALKER ========================
    Walker::Walker() : Zombie("Walker", 100) {}

    std::unique_ptr<Zombie> Walker::Clone() const { return std::make_unique<Walker>(*this); }

    void Walker::Attack() const { std::cout << "   *Walks slowly but relentlessly...*\n"; }

    // ======================== BLOATER ========================
    Bloater::Bloater() : Zombie("Bloater", 300) {}

    std::unique_ptr<Zombie> Bloater::Clone() const { return std::make_unique<Bloater>(*this); }

    void Bloater::Attack() const { std::cout << "   *Slams into the ground, causing a gas cloud!*\n"; }

    // =========================================================================
    // DEMO IMPLEMENTATION
    // =========================================================================
    void RunDemo()
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // ======================== INTRODUCTION ========================
        HFL::ClearScreen();
        HFL::PrintHeader("PROTOTYPE PATTERN");

        HFL::PrintSection("THE DEFINITION");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "Specify the kinds of objects to create using a prototype instance,\n"
            << "and create new objects by copying this prototype.\n\n";

        HFL::PrintSection("THE GOAL");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "In games, initializing an actor is slow. It involves disk access,\n"
            << "mesh parsing, and texture binding. The Prototype pattern creates one\n"
            << "'Master' actor. Every subsequent spawn is a simple RAM-to-RAM copy,\n"
            << "bypassing the expensive 'initialization' phase entirely.\n\n";

        HFL::PrintSection("THE EXAMPLE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "Initialize three 'Master Prototypes' and then use them to\n"
            << "instantly clone a massive horde of unique objects.\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE PROTOTYPE (INTERFACE): ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Zombie defines the 'Clone()' contract.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE MASTERS (PROTOTYPES):  ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The unique instances loaded from 'Disk'.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE HORDE (CLONES):        ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Instances created via 'Clone()' in RAM.\n\n";

        HFL::WaitForInput();

        // ======================== THE ARCHITECTURE ========================
        HFL::ClearScreen();
        HFL::PrintHeader("THE ARCHITECTURE");

        HFL::PrintSection("COVARIANT RETURNS");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "A derived Clone() returns its own specific type rather\n"
            << "than the base type. This means Crawler::Clone() returns Crawler*,\n"
            << "preserving type safety and avoiding unnecessary casting.\n\n";

        HFL::PrintSection("IMPLEMENTATION");

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] Clone() vs New\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    NEW:            ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Runs full constructor logic (Disk access, Heavy logic).\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    CLONE:          ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Runs Copy Constructor (Bitwise/Deep copy of existing RAM).\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "\n[*] Prototype Management\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ADVANTAGE:      ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "You can modify a Prototype at runtime (e.g., 'Enraged' state)\n"
            << "                    and all subsequent clones will inherit that state.\n";

        HFL::SetColor(HFL::EColor::White);
        HFL::WaitForInput();



        // ======================== INITIALIZATION ========================
        HFL::ClearScreen();
        HFL::PrintHeader("ZOMBIE ARMY");

        // ======================== MASTER COPY ========================
        auto CrawlerMaster = std::make_unique<Crawler>();
        auto WalkerMaster = std::make_unique<Walker>();
        auto BloaterMaster = std::make_unique<Bloater>();

        // ======================== CLONES ========================
        std::vector<std::unique_ptr<Zombie>> Horde;
        int CrawlerCount = 0;
        int WalkerCount = 0;
        int BloaterCount = 0;

        HFL::PrintSection("HORDE SPAWNER");

        while (true)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("ZOMBIE HORDE SPAWNER");

            // ======================== ZOMBIE STATS ========================
            HFL::PrintSection("HORDE COMPOSITION");
            HFL::SetColor(HFL::EColor::White);
            std::cout << "  [CRAWLERS]: "; HFL::SetColor(HFL::EColor::Green); std::cout << CrawlerCount << "\n";
            HFL::SetColor(HFL::EColor::White);
            std::cout << "  [WALKERS]:  "; HFL::SetColor(HFL::EColor::Green); std::cout << WalkerCount << "\n";
            HFL::SetColor(HFL::EColor::White);
            std::cout << "  [BLOATERS]: "; HFL::SetColor(HFL::EColor::Green); std::cout << BloaterCount << "\n";

            HFL::SetColor(HFL::EColor::Gray);
            std::cout << "  ------------------------\n";
            HFL::SetColor(HFL::EColor::White);
            std::cout << "  TOTAL ZOMBIES: ";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << Horde.size() << "\n\n";

            HFL::PrintSection("SPAWN MENU");
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [1] "; HFL::SetColor(HFL::EColor::White); std::cout << "CRAWLER\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [2] "; HFL::SetColor(HFL::EColor::White); std::cout << "WALKER\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [3] "; HFL::SetColor(HFL::EColor::White); std::cout << "BLOATER\n\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [0] "; HFL::SetColor(HFL::EColor::White); std::cout << "CONTINUE\n";

            int Choice = HFL::GetValidMenuInput(3);
            if (Choice == 0) break;

            Zombie* Prototype = nullptr;
            std::string TypeName = "";

            if (Choice == 1) { Prototype = CrawlerMaster.get(); TypeName = "Crawler"; }
            else if (Choice == 2) { Prototype = WalkerMaster.get(); TypeName = "Walker"; }
            else if (Choice == 3) { Prototype = BloaterMaster.get(); TypeName = "Bloater"; }

            if (Prototype)
            {
                std::cout << "\nHow many " << TypeName << "s to clone? (Max 100,000): ";

                long long RawInput;
                std::cin >> RawInput;

                unsigned int FinalAmount = 0;

                if (std::cin.fail())
                {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << ">> [Error] Invalid input. No units spawned.\n";
                }
                else if (RawInput < 0)
                {
                    FinalAmount = 0;
                    std::cout << ">> [System] Negative value detected. Defaulting to 0.\n";
                }
                else if (RawInput > 100000)
                {
                    FinalAmount = 100000;
                    std::cout << ">> [System] Input exceeds safety limit. Capping at 100,000.\n";
                }
                else
                {
                    FinalAmount = static_cast<unsigned int>(RawInput);
                }

                HFL::SetColor(HFL::EColor::Gray);
                std::cout << ">> [Spawner] Performing RAM-to-RAM memory stamp for " << FinalAmount << " zombies...\n";

                for (int i = 0; i < FinalAmount; ++i)
                {
                    // THE CORE ACTION: Copying the RAM of the Master Prototype.
                    // Clone() returns a unique_ptr, so ownership transfers cleanly
                    // into the vector without any raw pointer intermediates.
                    Horde.push_back(Prototype->Clone());
                }

                if (Choice == 1) CrawlerCount += FinalAmount;
                else if (Choice == 2) WalkerCount += FinalAmount;
                else if (Choice == 3) BloaterCount += FinalAmount;

                HFL::SetColor(HFL::EColor::Green);
                std::cout << ">> [Success] " << FinalAmount << " " << TypeName << "(s) added to the horde!\n";

                HFL::SetColor(HFL::EColor::Gray);
                std::cout << ">> [Action] The last clone screams: ";
                HFL::SetColor(HFL::EColor::White);
                Horde.back()->Attack();
            }
            HFL::WaitForInput();
        }

        // ======================== CONCLUSION ========================
        HFL::ClearScreen();
        HFL::PrintHeader("CONCLUSION");

        HFL::PrintSection("ARCHITECTURE & SOLID");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The implementation of the Prototype Pattern confirms the following:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] DEPENDENCY INVERSION:  ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Spawner depends on the 'Zombie' abstraction. It never\n"
            << "    calls 'new Crawler()', inverting the dependency flow.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] OPEN/CLOSED BOUNDARY:  ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Easy to add new Zombie types (Runners, Spitters) by creating\n"
            << "    new classes without modifying the core Spawner logic.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] INTERFACE COVARIANCE:  ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Specific types are preserved during cloning, ensuring that\n"
            << "    object-specific data is never lost during the copy.\n\n";

        HFL::PrintSection("SUMMARY");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The Prototype Pattern ensures that software is:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] PERFORMANT: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "RAM-to-RAM cloning bypasses expensive Disk I/O and constructors.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] DECOUPLED:  ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The high-level Spawner remains 'blind' to concrete zombie types.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] SCALABLE:   ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Complex initialization is localized to the Prototype's creation,\n"
            << "    keeping the runtime spawning logic clean and predictable.\n\n";

        HFL::SetColor(HFL::EColor::White);
        HFL::WaitForInput();
    }
}