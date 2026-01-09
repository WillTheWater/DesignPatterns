#include "Structural/Flyweight.h"

namespace FLY
{
    // =========================================================================
    // 2. THE FLYWEIGHT IMPLEMENTATION
    // =========================================================================
    LegionaryTexture::LegionaryTexture(const std::string& Filename) : Filename(Filename)
    {
        std::cout << "[Flyweight] Loading Texture: " << Filename << "...\n";
        std::cout << "   [VRAM] Allocating 1MB of Memory for Pixels...\n";
        std::cout << "[Flyweight] Texture Loaded (Heavy Intrinsic State).\n";
    }

    void LegionaryTexture::Render()
    {
        std::cout << "   [VRAM] Drawing: " << Filename << " (Intrinsic State).\n";
    }

    std::string LegionaryTexture::GetFilename() const { return Filename; }

    // =========================================================================
    // 3. THE CONTEXT IMPLEMENTATIONS
    // =========================================================================

    Legionary::Legionary(std::shared_ptr<LegionaryTexture> SharedFace) : RomanSoldierBase(SharedFace)
    {
        // Reference counting is handled by the base class constructor.
    }

    void Legionary::Draw()
    {
        std::cout << "   [Soldier] Legionary draws shield.\n";
        if (MySharedFace) MySharedFace->Render();
    }

    std::string Legionary::GetSoldierType() const { return "Legionary (Shield)"; }

    Auxilia::Auxilia(std::shared_ptr<LegionaryTexture> SharedFace) : RomanSoldierBase(SharedFace) {}

    void Auxilia::Draw()
    {
        std::cout << "   [Soldier] Auxilia throws spear.\n";
        if (MySharedFace) MySharedFace->Render();
    }

    std::string Auxilia::GetSoldierType() const { return "Auxilia (Spear)"; }

    Praetorian::Praetorian(std::shared_ptr<LegionaryTexture> SharedFace) : RomanSoldierBase(SharedFace) {}

    void Praetorian::Draw()
    {
        std::cout << "   [Soldier] Praetorian swings sword.\n";
        if (MySharedFace) MySharedFace->Render();
    }

    std::string Praetorian::GetSoldierType() const { return "Praetorian (Elite)"; }

    // =========================================================================
    // 4. THE FACTORY IMPLEMENTATION
    // =========================================================================
    std::shared_ptr<LegionaryTexture> TextureFactory::GetTexture(const std::string& Filename)
    {
        auto it = TextureCache.find(Filename);
        if (it == TextureCache.end())
        {
            auto NewTexture = std::make_shared<LegionaryTexture>(Filename);
            TextureCache[Filename] = NewTexture;
            return NewTexture;
        }
        return it->second;
    }

    // =========================================================================
    // 5. DEMO IMPLEMENTATION
    // =========================================================================
    void RunDemo()
    {
        // Clear buffer - Essential for cin stability
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // --- STEP 1: INTRODUCTION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Flyweight Pattern (Roman Army)");

        std::cout << "Definition:\n";
        std::cout << "Use sharing to support large numbers of fine-grained objects efficiently.\n";
        std::cout << "Separate Intrinsic State (Shared/Constant) from Extrinsic State (Contextual).\n\n";

        std::cout << "In This Demo:\n";
        std::cout << "We will simulate a Giant Roman Army (1000+ Soldiers).\n";
        std::cout << "Intrinsic Data: Heavy textures and meshes (Loaded once).\n";
        std::cout << "Extrinsic Data: Unit type and unique pointers (Per soldier).\n";

        HFL::WaitForInput();

        // --- STEP 2: THE PROBLEM ---
        HFL::ClearScreen();
        HFL::PrintHeader("Step 1: The Problem (Memory Exhaustion)");

        std::cout << "Scenario: Spawning 10,000 Soldiers.\n\n";

        std::cout << "THE NAIVE WAY (Deep Copying):\n";
        std::cout << "If every Soldier instance owns its own 1MB Texture object:\n";
        std::cout << "  - 10,000 Soldiers * 1.01 MB = ~10.1 GB RAM.\n";
        std::cout << "  >> RESULT: System Crash / Out of Memory.\n\n";

        std::cout << "THE FLYWEIGHT WAY (State Sharing):\n";
        std::cout << "If Texture is stored once and shared via pointers:\n";
        std::cout << "  - 1 Texture * 1 MB = 1 MB.\n";
        std::cout << "  - 10,000 Pointers * 8 bytes = 80 KB.\n";
        std::cout << "  - Army Total: ~1.08 MB.\n";
        std::cout << "  >> RESULT: 99.9% Memory Reduction.\n\n";

        HFL::WaitForInput();

        // --- STEP 3: THE ARCHITECTURE ---
        HFL::ClearScreen();
        HFL::PrintHeader("Step 2: Architecture Components");

        std::cout << "1. The Flyweight (LegionaryTexture):\n";
        std::cout << "   - Contains INTRINSIC state. Constant and immutable.\n\n";

        std::cout << "2. The Factory (TextureFactory):\n";
        std::cout << "   - The 'Gatekeeper'. Ensures unique assets are never duplicated.\n";
        std::cout << "   - Returns existing references from a Cache.\n\n";

        std::cout << "3. The Context (Soldier Subclasses):\n";
        std::cout << "   - Contains EXTRINSIC state (Unique behavior/position).\n";
        std::cout << "   - References the Flyweight to perform heavy operations (Drawing).\n";

        HFL::WaitForInput();

        // --- STEP 4: INTERACTIVE SPAWNING ---
        TextureFactory MyFactory;
        std::shared_ptr<LegionaryTexture> SharedFace = MyFactory.GetTexture("legionary_face.png");
        std::vector<std::unique_ptr<IRomanSoldier>> RomanArmy;

        bool InDemo = true;
        while (InDemo)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("Roman Army Builder");

            std::cout << "Select a Unit Type to Spawn:\n"
                << "1. Legionary (Shield)\n"
                << "2. Auxilia (Spear)\n"
                << "3. Praetorian (Elite)\n"
                << "4. Draw Army\n"
                << "0. Exit\n\n";

            // Precise memory tracking
            double ExtrinsicRAM = (RomanArmy.size() * sizeof(void*)) / 1024.0;
            std::cout << "Army Size: " << RomanArmy.size() << "\n";
            std::cout << "Intrinsic (Shared) Cost: 1.00 MB\n";
            std::cout << "Extrinsic (Unique) Cost: " << std::fixed << std::setprecision(4) << ExtrinsicRAM << " KB\n";

            int Choice = HFL::GetValidMenuInput(4);

            if (Choice == 0) break;
            if (Choice == 4)
            {
                for (const auto& S : RomanArmy) S->Draw();
                HFL::WaitForInput();
            }
            else
            {
                // Batch spawning for performance testing simulation
                int Batch = 100;
                for (int i = 0; i < Batch; ++i)
                {
                    if (Choice == 1) RomanArmy.push_back(std::make_unique<Legionary>(SharedFace));
                    else if (Choice == 2) RomanArmy.push_back(std::make_unique<Auxilia>(SharedFace));
                    else if (Choice == 3) RomanArmy.push_back(std::make_unique<Praetorian>(SharedFace));
                }
            }
        }

        // --- STEP 5: MEMORY LIFECYCLE & RAII ---
        HFL::ClearScreen();
        HFL::PrintHeader("Step 3: Lifecycle Management");

        std::cout << "The Army of " << RomanArmy.size() << " soldiers is currently in memory.\n";
        std::cout << "Each soldier holds a 'std::shared_ptr' to the Intrinsic Flyweight.\n\n";

        // IMPORTANT: Total count is (Army Size + 1) because the 'SharedFace' variable in this function also holds a reference.
        std::cout << "Current Flyweight Reference Count: " << SharedFace.use_count() << "\n";
        std::cout << "  (1 reference in Demo Scope + " << RomanArmy.size() << " references in Army vector)\n\n";

        HFL::WaitForInput();

        std::cout << ">> Action: RomanArmy.clear() triggered.\n";
        RomanArmy.clear();

        std::cout << ">> Result: All Context objects (Soldiers) destroyed.\n";
        std::cout << ">> Result: Shared references held by soldiers were released.\n\n";

        std::cout << "New Flyweight Reference Count: " << SharedFace.use_count() << "\n";
        std::cout << "  (The Texture is STILL in memory because the Factory/Demo Scope still needs it).\n";

        HFL::WaitForInput();

        // --- STEP 6: CONCLUSION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Conclusion: The Power of Flyweight");

        std::cout << "1. INTRINSIC STATE (The Flyweight):\n";
        std::cout << "   - Heavy data (1MB Texture) was stored once.\n";
        std::cout << "   - Sharing logic: TotalMemory = Intrinsic + (Count * Extrinsic)\n\n";

        std::cout << "2. EXTRINSIC STATE (The Context):\n";
        std::cout << "   - Soldiers only stored unique identifiers and pointers.\n";
        std::cout << "   - Result: Huge memory savings for massive scale.\n\n";

        std::cout << "3. DETERMINISTIC DESTRUCTION:\n";
        std::cout << "   - No manual 'delete' was required.\n";
        std::cout << "   - When the last 'std::shared_ptr' goes out of scope, the Texture is freed.\n\n";

        std::cout << std::setw(40) << "Demo Complete.\n";
        HFL::WaitForInput();
    }
}