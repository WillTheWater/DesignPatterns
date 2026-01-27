#include "Structural/Flyweight.h"

namespace FLY
{
    // =========================================================================
    // THE FLYWEIGHT IMPLEMENTATION (Intrinsic State)
    // ROLE: Manages the "Heavy" data that never changes between instances.
    // =========================================================================

    LegionaryTexture::LegionaryTexture(const std::string& Filename) : Filename(Filename)
    {
        HFL::SetColor(HFL::EColor::Cyan);
        std::cout << "[Flyweight] Loading Heavy Texture: " << Filename;
        HFL::WaitDots(0.4f);

        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "\n   [VRAM] Allocating 1.0 MB for Pixel Buffer";
        HFL::WaitDots(0.2f);

        HFL::SetColor(HFL::EColor::Green);
        std::cout << " SUCCESS.\n";
        HFL::SetColor(HFL::EColor::Gray);
    }

    void LegionaryTexture::Render()
    {
        // In a real engine, this would bind the texture ID to the GPU pipeline.
        std::cout << "   [GPU] Sampling Texture: " << Filename << " (Intrinsic Shared Data)\n";
    }

    std::string LegionaryTexture::GetFilename() const { return Filename; }

    // =========================================================================
    // THE CONTEXT IMPLEMENTATIONS (Extrinsic State)
    // ROLE: Lightweight objects that vary by behavior but share the same Flyweight.
    // =========================================================================

    // ======================== LEGIONARY ========================
    Legionary::Legionary(std::shared_ptr<LegionaryTexture> SharedFace)
        : RomanSoldierBase(SharedFace) {
    }

    void Legionary::Draw()
    {
        HFL::SetColor(HFL::EColor::White);
        std::cout << " [Unit] Legionary raises Shield.\n";
        HFL::SetColor(HFL::EColor::Gray);
        if (MySharedFace) MySharedFace->Render();
        HFL::Wait(0.02f);
    }

    std::string Legionary::GetSoldierType() const { return "Legionary"; }

    // ======================== AUXILIA ========================
    Auxilia::Auxilia(std::shared_ptr<LegionaryTexture> SharedFace)
        : RomanSoldierBase(SharedFace) {
    }

    void Auxilia::Draw()
    {
        HFL::SetColor(HFL::EColor::White);
        std::cout << " [Unit] Auxilia readies Spear.\n";
        HFL::SetColor(HFL::EColor::Gray);
        if (MySharedFace) MySharedFace->Render();
        HFL::Wait(0.02f);
    }

    std::string Auxilia::GetSoldierType() const { return "Auxilia"; }


    // ======================== PRAETORIAN ========================
    Praetorian::Praetorian(std::shared_ptr<LegionaryTexture> SharedFace)
        : RomanSoldierBase(SharedFace) {
    }

    void Praetorian::Draw()
    {
        HFL::SetColor(HFL::EColor::Yellow);
        std::cout << " [Elite] Praetorian unsheathes Sword.\n";
        HFL::SetColor(HFL::EColor::Gray);
        if (MySharedFace) MySharedFace->Render();
        HFL::Wait(0.02f);
    }

    std::string Praetorian::GetSoldierType() const { return "Praetorian"; }


    // =========================================================================
    // THE FACTORY IMPLEMENTATION (The Gatekeeper)
    // ROLE: Ensures we never load the same heavy asset twice.
    // =========================================================================

    std::shared_ptr<LegionaryTexture> TextureFactory::GetTexture(const std::string& Filename)
    {
        auto it = TextureCache.find(Filename);

        if (it == TextureCache.end())
        {
            // Case: Cache Miss. Create the heavy object for the first time.
            auto NewTexture = std::make_shared<LegionaryTexture>(Filename);
            TextureCache[Filename] = NewTexture;
            return NewTexture;
        }

        // Case: Cache Hit. Reuse the existing object.
        return it->second;
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
        HFL::PrintHeader("FLYWEIGHT DESIGN PATTERN");

        HFL::PrintSection("THE DEFINITION");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "Use sharing to support large numbers of fine-grained objects efficiently.\n"
            << "Separate Intrinsic State (Shared/Constant) from Extrinsic State (Contextual).\n\n";

        HFL::PrintSection("THE GOAL");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Flyweight Pattern is about 'Efficiency through Resource Sharing'.\n"
            << "It solves the problem of memory exhaustion when a system requires thousands\n"
            << "of similar objects by extracting heavy, identical data into a shared\n"
            << "object, leaving only the unique 'context' in the individual instances.\n\n";

        HFL::PrintSection("THE EXAMPLE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "This demonstration simulates a massive Roman Army using three components:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE FLYWEIGHT:       ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The heavy 'Intrinsic' data (A 1MB Texture) shared by all.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE FACTORY:         ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The 'Gatekeeper' that ensures assets are only loaded once.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE CONTEXT:         ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The unique 'Extrinsic' units (Soldiers) that point to the shared data.\n\n";

        HFL::PrintSection("THE MATH (10,000 SOLDIERS)");
        HFL::SetColor(HFL::EColor::Red);
        std::cout << " [*] NAIVE WAY:   10,000 x 1MB Texture = 10.0 GB (CRASH)\n";
        HFL::SetColor(HFL::EColor::Green);
        std::cout << " [*] FLYWEIGHT:   1 x 1MB + (10,000 x 8b) = 1.08 MB (OPTIMIZED)\n\n";

        HFL::WaitForInput();

        // ======================== THE ARCHITECTURE ========================
        HFL::ClearScreen();
        HFL::PrintHeader("THE ARCHITECTURE");

        HFL::PrintSection("INTRINSIC VS EXTRINSIC");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The secret to Flyweight is identifying what is 'Intrinsic' (constant across\n"
            << "all units, like a texture) and what is 'Extrinsic' (unique per unit,\n"
            << "like their current animation state or position on the battlefield).\n\n";

        HFL::PrintSection("IMPLEMENTATION");

        // ======================== THE FLYWEIGHT ========================
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] LegionaryTexture (The Shared Asset)\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    STATE:          ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Intrinsic. It contains the heavy raw pixel data.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    LIFECYCLE:      ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Managed by shared pointers; it lives as long as soldiers need it.\n\n";

        // ======================== THE CONTEXT ========================
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] RomanSoldierBase (The Context)\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    STATE:          ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Extrinsic. Contains unique logic for Legionaries vs Praetorians.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    EFFICIENCY:     ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Extremely lightweight. It only holds a pointer to the heavy data.\n\n";

        HFL::SetColor(HFL::EColor::White);
        HFL::WaitForInput();

        // ======================== INITIALIZATION ========================
        HFL::PrintSection("INITIALIZATION");
        TextureFactory MyFactory;
        std::shared_ptr<LegionaryTexture> SharedFace = MyFactory.GetTexture("roman_face.png");
        std::vector<std::unique_ptr<IRomanSoldier>> RomanArmy;
        HFL::WaitForInput();

        int Count_Legionary = 0;
        int Count_Auxilia = 0;
        int Count_Praetorian = 0;
        int TotalDrawCalls = 0;

        // ======================== GAME LOOP ========================
        while (true)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("ROMAN ARMY COMMAND");

            // ======================== ARMY COMPOSITION ========================
            HFL::PrintSection("ARMY COMPOSITION");
            HFL::SetColor(HFL::EColor::White);
            std::cout << "  LEGIONARIES:     "; HFL::SetColor(HFL::EColor::Green);
            std::cout << std::left << std::setw(18) << Count_Legionary;
            HFL::SetColor(HFL::EColor::White);
            std::cout << "  AUXILIA:         "; HFL::SetColor(HFL::EColor::Green);
            std::cout << Count_Auxilia << "\n";

            HFL::SetColor(HFL::EColor::White);
            std::cout << "  PRAETORIANS:     "; HFL::SetColor(HFL::EColor::Green);
            std::cout << std::left << std::setw(18) << Count_Praetorian;
            HFL::SetColor(HFL::EColor::White);
            std::cout << "  TOTAL UNITS:     "; HFL::SetColor(HFL::EColor::Cyan);
            std::cout << RomanArmy.size() << "\n";

            HFL::SetColor(HFL::EColor::White);
            std::cout << "  DRAW CALLS:      "; HFL::SetColor(HFL::EColor::Yellow);
            std::cout << TotalDrawCalls << " (Total Lifetime Samples)\n";

            // ======================== MEMORY STATISTICS ========================
            HFL::PrintSection("RESOURCES & MEMORY");
            double ExtrinsicRAM = (RomanArmy.size() * sizeof(void*)) / 1024.0;

            HFL::SetColor(HFL::EColor::White);
            std::cout << "  INTRINSIC COST:  "; HFL::SetColor(HFL::EColor::Cyan);
            std::cout << "1.00 MB (Shared Texture)\n";

            HFL::SetColor(HFL::EColor::White);
            std::cout << "  EXTRINSIC COST:  "; HFL::SetColor(HFL::EColor::Yellow);
            std::cout << std::fixed << std::setprecision(2) << ExtrinsicRAM << " KB (Unique Pointers)\n\n";

            HFL::PrintSection("COMMANDS");
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [1] "; HFL::SetColor(HFL::EColor::White); std::cout << "CREATE BATCH (LEGIONARIES)\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [2] "; HFL::SetColor(HFL::EColor::White); std::cout << "CREATE BATCH (AUXILIA)\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [3] "; HFL::SetColor(HFL::EColor::White); std::cout << "CREATE BATCH (PRAETORIANS)\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [4] "; HFL::SetColor(HFL::EColor::White);  std::cout << "DRAW ARMY\n\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [0] "; HFL::SetColor(HFL::EColor::White); std::cout << "CONTINUE\n\n";

            int Choice = HFL::GetValidMenuInput(4);
            if (Choice == 0) break;

            if (Choice == 4)
            {
                HFL::PrintSection("GPU RENDERING PIPELINE");
                if (RomanArmy.empty())
                {
                    HFL::SetColor(HFL::EColor::Red);
                    std::cout << " [!] The battlefield is empty. Create units first.\n";
                    HFL::Wait(2);
                }
                else
                {
                    HFL::ClearScreen();
                    int Tally = 0;
                    for (const auto& S : RomanArmy)
                    {
                        S->Draw();
                        TotalDrawCalls++;
                        Tally++;
                        if (Tally == 9)
                        {
                            HFL::ClearScreen();
                            Tally = 0;
                        }
                    }
                    HFL::Wait(1);
                }
            }
            else
            {
                int BatchSize = HFL::GetRandom(10, 90);

                HFL::SetColor(HFL::EColor::Gray);
                std::cout << " Recruiting " << BatchSize << " units"; HFL::WaitDots(0.6f);

                for (int i = 0; i < BatchSize; ++i)
                {
                    if (Choice == 1)
                    {
                        RomanArmy.push_back(std::make_unique<Legionary>(SharedFace));
                        Count_Legionary++;
                    }
                    else if (Choice == 2)
                    {
                        RomanArmy.push_back(std::make_unique<Auxilia>(SharedFace));
                        Count_Auxilia++;
                    }
                    else if (Choice == 3)
                    {
                        RomanArmy.push_back(std::make_unique<Praetorian>(SharedFace));
                        Count_Praetorian++;
                    }
                }
            }
        }

        // ======================== CONCLUSION ========================
        HFL::ClearScreen();
        HFL::PrintHeader("CONCLUSION");

        HFL::PrintSection("ARCHITECTURE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The implementation of the Flyweight Pattern confirms the following:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] RESOURCE DE-DUPLICATION: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The 'TextureFactory' acts as a cache. Thousands of soldiers\n"
            << "    point to a single VRAM allocation rather than cloning it.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] STATE SEPARATION:        ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Heavy 'Intrinsic' state is static and shared, while lightweight\n"
            << "    'Extrinsic' state (unit type/logic) remains unique to the instance.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] RAII LIFECYCLE:          ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "By using std::shared_ptr, the heavy Flyweight asset is only\n"
            << "    released when the last soldier reference is destroyed.\n\n";

        HFL::PrintSection("SUMMARY");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The Flyweight Pattern ensures that software is:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] EFFICIENT: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Reduces memory footprint by 99% in repetitive environments.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] SCALABLE:  ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Enables massive 'AAA' scale simulations on modest hardware.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] CENTRALIZED:";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Asset loading logic is consolidated within a single Factory.\n\n";

        HFL::SetColor(HFL::EColor::White);
        HFL::WaitForInput();
    }
}