#include "Creational/Prototype.h"

namespace PRO
{
    // ------------------------------------------------------------------------
    // BASE CLASS IMPLEMENTATION
    // ------------------------------------------------------------------------
    Zombie::Zombie(std::string Name, int Health)
        : Name(Name), Health(Health)
    {
        // Simulates loading Mesh/Texture from disk.
        std::cout << ">> [System] Loading '" << Name << "' assets from disk...\n";
    }

    void Zombie::Attack() const
    {
        std::cout << "   *Shambling around*\n";
    }

    // ------------------------------------------------------------------------
    // CRAWLER IMPLEMENTATION
    // ------------------------------------------------------------------------
    Crawler::Crawler() : Zombie("Crawler", 20) {}

    // COVARIANCE: Returns the specific type (Crawler*) instead of generic Zombie*.
    // Keeping the exact type.
    Crawler* Crawler::Clone() const
    {
        // DEEP COPY:
        // 'new Crawler(*this)' creates a NEW Crawler and copies the Name/Health
        // from 'this' object (The Prototype).
        // 
        // 1. Allocates memory for a NEW Crawler.
        // 2. Looks at 'this' (Prototype).
        // 3. Copies Name="Crawler" and Health=20 into the NEW Crawler.
        // 4. Returns pointer to the NEW Crawler.
        return new Crawler(*this);
        // *this is the difference between calling the Constructor and calling
        // the Copy Constructor.
    }

    void Crawler::Attack() const
    {
        std::cout << "   *Crawls fast towards you!*\n";
    }

    // ------------------------------------------------------------------------
    // WALKER IMPLEMENTATION
    // ------------------------------------------------------------------------
    Walker::Walker() : Zombie("Walker", 100) {}

    Walker* Walker::Clone() const
    {
        return new Walker(*this);
    }

    void Walker::Attack() const
    {
        std::cout << "   *Walks slowly towards you...*\n";
    }

    // ------------------------------------------------------------------------
    // BLOATER IMPLEMENTATION
    // ------------------------------------------------------------------------
    Bloater::Bloater() : Zombie("Bloater", 300) {}

    Bloater* Bloater::Clone() const
    {
        return new Bloater(*this);
    }

    void Bloater::Attack() const
    {
        std::cout << "   *slams into you!*\n";
    }

    // ------------------------------------------------------------------------
    // DEMO IMPLEMENTATION
    // ------------------------------------------------------------------------
    void RunDemo()
    {
        // Clear buffer
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // --- STEP 1: INTRODUCTION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Prototype Pattern");

        std::cout << "Definition:\n";
        std::cout << "Specify the kinds of objects to create using a prototype instance,\n";
        std::cout << "and create new objects by copying this prototype.\n\n";

        std::cout << "In This Demo:\n";
        std::cout << "You can create a 'Zombie Horde'.\n";
        std::cout << "By defining 'Prototypes' of zombies types.\n";
        std::cout << "When they spawn it just CLONES the Prototype, it doesn't 'Create' new ones.\n";
        std::cout << "This avoids re-loading assets for every instance.\n";

        HFL::WaitForInput();

        // --- STEP 2: THE PROTOTYPES ---
        HFL::ClearScreen();
        HFL::PrintHeader("Step 1: The Prototypes");

        std::cout << "There are 3 types of Zombies.\n";
        std::cout << "Each has its own 'Clone()' method that copies its data.\n\n";

        std::cout << "1. Crawler (Fast, Low HP)\n";
        std::cout << "   'Attack()': Crawl fast.\n";
        std::cout << "   'Clone()': Returns Crawler* (Covariance)\n\n";

        std::cout << "2. Walker (Slow, High HP)\n";
        std::cout << "   'Attack()': Walk slow.\n";
        std::cout << "   'Clone()': Returns Walker* (Covariance)\n\n";

        std::cout << "3. Bloater (Tank)\n";
        std::cout << "   'Attack()': Slam.\n";
        std::cout << "   'Clone()': Returns Bloater* (Covariance)\n\n";

        HFL::WaitForInput();

        // --- STEP 3: INTERACTIVE HORDE ---

        // 1. Create the Masters (Load their assets once)
        std::cout << "[System] Initializing Prototypes...\n";
        // We use unique_ptr to manage the Masters automatically
        std::unique_ptr<Crawler> CrawlerPrototype = std::make_unique<Crawler>();
        std::unique_ptr<Walker> WalkerPrototype = std::make_unique<Walker>();
        std::unique_ptr<Bloater> BloaterPrototype = std::make_unique<Bloater>();

        // The Horde holds copies (raw pointers to clones)
        std::vector<Zombie*> Horde;
        int TotalCount = 0;

        bool InDemo = true;
        while (InDemo)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("Zombie Horde Spawner");

            std::cout << "\nCurrent Horde Size: " << TotalCount << "\n";

            std::cout << "Select a Prototype to Clone:\n\n";
            std::cout << "1. Crawler\n";
            std::cout << "2. Walker\n";
            std::cout << "3. Bloater\n";
            std::cout << "0. Exit Demo\n";

            std::cout << "\nChoice: ";
            int Choice;
            std::cin >> Choice;

            if (std::cin.fail()) { std::cin.clear(); std::cin.ignore(); continue; }

            if (Choice == 0) break;

            Zombie* SelectedPrototype = nullptr;
            std::string TypeName = "";

            // Select the Master
            if (Choice == 1) { SelectedPrototype = CrawlerPrototype.get(); TypeName = "Crawler"; }
            else if (Choice == 2) { SelectedPrototype = WalkerPrototype.get(); TypeName = "Walker"; }
            else if (Choice == 3) { SelectedPrototype = BloaterPrototype.get(); TypeName = "Bloater"; }

            if (SelectedPrototype)
            {
                std::cout << "\nHow many " << TypeName << "s to spawn? ";
                int Amount;
                std::cin >> Amount;

                if (std::cin.fail() || Amount <= 0 || Amount > 100000) { std::cin.clear(); std::cin.ignore(); continue; }

                // ----------------------------------------------------------
                // THE CLONING PROCESS
                // ----------------------------------------------------------
                // Note: We DO NOT call "new Crawler()" here.
                // We call "Clone()". The specific class handles the copy.
                // This is the difference between the Constructor and Copy Constructor
                std::cout << "\n>> [Spawner] Cloning " << Amount << " " << TypeName << "s...\n";

                for (int i = 0; i < Amount; ++i)
                {
                    Zombie* NewZombie = SelectedPrototype->Clone();
                    Horde.push_back(NewZombie);
                }

                TotalCount += Amount;
                std::cout << ">> [Success] Horde Size is now: " << TotalCount << "\n";

                // Let them attack to prove they are real objects
                std::cout << ">> [Action] The horde attacks!\n";
                Horde.back()->Attack();

                HFL::WaitForInput();
            }
        }

        // --- STEP 4: CONCLUSION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Conclusion");

        std::cout << "Why 'Cloning'?\n\n";
        std::cout << "1. Construction is EXPENSIVE:\n";
        std::cout << "   When you use 'new Crawler()':\n";
        std::cout << "   - Calls the Constructor.\n";
        std::cout << "   - Loads 'Crawler.png' from Disk.\n";
        std::cout << "   - Allocates RAM.\n";
        std::cout << "   Doing this 100 times is VERY slow.\n\n";

        std::cout << "2. Cloning is INSTANT:\n";
        std::cout << "   When you use 'Crawler.Clone()':\n";
        std::cout << "   - Calls the Copy Constructor.\n";
        std::cout << "   - Uses the 'Prototype's' RAM data.\n";
        std::cout << "   - Stamps it into a new object.\n";
        std::cout << "   - NO disk access. NO loading.\n\n";

        std::cout << "Summary of Prototype:\n\n";
        std::cout << "1. Efficiency:\n";
        std::cout << "   Used 'Clone()' instead of 'New'.\n";
        std::cout << "   This avoids 'Loading Assets' for every zombie.\n";
        std::cout << "   It copies RAM instead of reloading from Disk.\n\n";

        std::cout << "2. Covariant Return Types:\n";
        std::cout << "   Crawler* Clone() returned a Crawler*, not a generic Zombie*.\n";
        std::cout << "   Preserving the specific type.\n\n";

        std::cout << "3. The Pattern:\n";
        std::cout << "   Define 'Prototypes' and create copies.\n";
        std::cout << "   Efficient spawning without repetitive initialization.\n";

        std::cout << std::setw(40) << "End of Demo\n";
        HFL::WaitForInput();
    }
}