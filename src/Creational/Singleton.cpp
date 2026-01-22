#include "Creational/Singleton.h"

namespace SGT
{
    // =========================================================================
    // SINGLETON IMPLEMENTATION: AssetManager
    // ROLE: Managing the lifecycle and access of the unique Manager instance.
    // =========================================================================

    AssetManager::AssetManager()
    {
        // Private constructor ensures no external entity can 'new' this class.
        std::cout << "[System] AssetManager Singleton Initialized.\n";
    }

    AssetManager& AssetManager::GetInstance()
    {
        // 1. STATIC LOCAL INSTANCE (Meyers' Singleton):
        //    Created the first time this function is called.
        // 2. LIFECYCLE:
        //    Lives for the duration of the program; destroyed at exit.
        // 3. THREAD SAFETY:
        //    C++11 guarantees this initialization is thread-safe.
        static AssetManager instance;
        return instance;
    }

    // =========================================================================
    // TEMPLATE SPECIALIZATION IMPLEMENTATIONS
    // ROLE: Custom logic for different asset types using the same function name.
    // =========================================================================
    // We provide specific implementations for each 'Tag' (Texture, Sound, Font).
    // This allows the manager to route requests to the correct internal cache.

    // --- SPECIALIZATION 1: TEXTURES ---
    template <>
    inline void AssetManager::LoadAsset<Texture>(const std::string& AssetPath)
    {
        std::cout << ">> [Manager] Routing request to Texture Cache...\n";

        auto It = TextureCache.find(AssetPath);
        if (It != TextureCache.end())
        {
            It->second++; // Increment Reference Count
            std::cout << "   [Cache Hit] Texture '" << AssetPath << "' found.\n";
            std::cout << "   [Status] Reference Count updated to: " << It->second << "\n";
        }
        else
        {
            std::cout << "   [Cache Miss] Loading Texture '" << AssetPath << "' from Disk...\n";
            TextureCache[AssetPath] = 1;
            std::cout << "   [Status] Asset Registered in Texture Cache.\n";
        }
    }

    // --- SPECIALIZATION 2: SOUNDS ---
    template <>
    inline void AssetManager::LoadAsset<Sound>(const std::string& AssetPath)
    {
        std::cout << ">> [Manager] Routing request to Sound Cache...\n";

        auto It = SoundCache.find(AssetPath);
        if (It != SoundCache.end())
        {
            It->second++;
            std::cout << "   [Cache Hit] Sound '" << AssetPath << "' found.\n";
            std::cout << "   [Status] Reference Count updated to: " << It->second << "\n";
        }
        else
        {
            std::cout << "   [Cache Miss] Loading Sound '" << AssetPath << "' from Disk...\n";
            SoundCache[AssetPath] = 1;
            std::cout << "   [Status] Asset Registered in Sound Cache.\n";
        }
    }

    // --- SPECIALIZATION 3: FONTS ---
    template <>
    inline void AssetManager::LoadAsset<Font>(const std::string& AssetPath)
    {
        std::cout << ">> [Manager] Routing request to Font Cache...\n";

        auto It = FontCache.find(AssetPath);
        if (It != FontCache.end())
        {
            It->second++;
            std::cout << "   [Cache Hit] Font '" << AssetPath << "' found.\n";
            std::cout << "   [Status] Reference Count updated to: " << It->second << "\n";
        }
        else
        {
            std::cout << "   [Cache Miss] Loading Font '" << AssetPath << "' from Disk...\n";
            FontCache[AssetPath] = 1;
            std::cout << "   [Status] Asset Registered in Font Cache.\n";
        }
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
        HFL::PrintHeader("SINGLETON PATTERN");

        HFL::PrintSection("THE DEFINITION");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "Ensure a class has only one instance and provide a global access point.\n\n";

        HFL::PrintSection("THE GOAL");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Singleton prevents the creation of multiple managers that could\n"
            << "cause resource conflicts. By using a single AssetManager, we ensure\n"
            << "that every system in the game shares the same cache and data.\n\n";

        HFL::PrintSection("THE EXAMPLE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "This demo uses a 'Template Asset Manager' to manage different types:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE INSTANCE:   ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "A single manager handling all global resource requests.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE SPECIALIST: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Template specialization routes files to the correct internal maps.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE CACHE:      ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Reference counting prevents loading the same file twice.\n\n";

        HFL::WaitForInput();

        // ======================== THE ARCHITECTURE ========================
        HFL::ClearScreen();
        HFL::PrintHeader("THE ARCHITECTURE");

        HFL::PrintSection("THE RULES");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "To strictly enforce a Singleton, implement the four core constraints:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] PRIVATE CONSTRUCTOR: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Prevents 'new AssetManager()' or stack allocation.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] STATIC ACCESS:       ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "GetInstance() provides the only gateway to the instance.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] DELETED COPY:        ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Prevents duplicating the instance via copy constructors.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] DELETED ASSIGNMENT:  ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Prevents overwriting the instance via the '=' operator.\n\n";

        HFL::PrintSection("THE TEMPLATE SYSTEM");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "By using Empty Structs (Texture, Sound, Font) as 'Tags', it allows\n"
            << "polymorphic behavior without the overhead of virtual functions.\n\n";

        HFL::WaitForInput();

        // ======================== INTERACTIVE SYSTEM ========================
        AssetManager& Manager = AssetManager::GetInstance();

        while (true)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("ASSET MANAGER");

            HFL::SetColor(HFL::EColor::White);
            std::cout << "Manager Instance: ";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << "[" << &Manager << "]\n";
            HFL::SetColor(HFL::EColor::Gray);
            std::cout << "Status: Ready to process requests...\n\n";

            HFL::PrintSection("MENU");
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [1] "; HFL::SetColor(HFL::EColor::White); std::cout << "LOAD: (Hero.png)\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [2] "; HFL::SetColor(HFL::EColor::White); std::cout << "LOAD: (Environment.png)\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [3] "; HFL::SetColor(HFL::EColor::White); std::cout << "LOAD: (Explosion.wav)\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [4] "; HFL::SetColor(HFL::EColor::White); std::cout << "LOAD: (Menu_Font.ttf)\n\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [0] "; HFL::SetColor(HFL::EColor::White); std::cout << "CONTINUE\n";

            int Choice = HFL::GetValidMenuInput(4);
            if (Choice == 0) break;

            HFL::PrintSection("LOADING STATUS");

            // ======================== LOADING SIMULATION ========================
            HFL::SetColor(HFL::EColor::Yellow);
            std::cout << ">> Accessing Global Singleton..."; HFL::Wait(0.8f); std::cout << " OK!\n";
            std::cout << ">> Locating Asset Path..."; HFL::Wait(0.1f); std::cout << " FOUND!\n";

            HFL::SetColor(HFL::EColor::White);
            std::cout << ">> Executing specialized LoadAsset<T>...\n\n";
            HFL::Wait(0.6f);

            // ======================== DYNAMIC RESULTS ========================
            HFL::SetColor(HFL::EColor::Cyan);
            switch (Choice)
            {
            case 1: Manager.LoadAsset<Texture>("Hero.png"); break;
            case 2: Manager.LoadAsset<Texture>("Environment.png"); break;
            case 3: Manager.LoadAsset<Sound>("Explosion.wav"); break;
            case 4: Manager.LoadAsset<Font>("Menu_Font.ttf"); break;
            }

            // ======================== CACHE ANALYSIS ========================
            HFL::Wait(0.6f);
            HFL::SetColor(HFL::EColor::Gray);
            std::cout << "\n--------------------------------------------------\n";
            std::cout << "   NOTE: If this was the first load, it will\n";
            std::cout << "  'Load from Disk'. Loading it again will use the\n";
            std::cout << "              Singleton's 'Cache'.\n";
            std::cout << "--------------------------------------------------\n";

            HFL::WaitForInput();
        }

        // ======================== CONCLUSION ========================
        HFL::ClearScreen();
        HFL::PrintHeader("CONCLUSION");

        HFL::PrintSection("ARCHITECTURE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The AssetManager Singleton implementation confirms the following:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] RESOURCE INTEGRITY: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Only one instance ever existed, ensuring a single source of truth.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] TYPE-SAFE ROUTING:  ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Template specialization allowed a unified interface (LoadAsset)\n"
            << "    to handle distinct internal data structures (Caches).\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] GLOBAL EFFICIENCY:  ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The manager was accessible from any scope without passing pointers,\n"
            << "    maintaining a clean and highly scannable codebase.\n\n";

        HFL::PrintSection("SUMMARY");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The Singleton Pattern ensures that software is:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] CENTRALIZED: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Unique resources are managed from a single, predictable location.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THREAD-SAFE: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Modern C++ static initialization prevents race conditions on startup.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] ACCESSIBLE: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Global systems can interact with the manager instantly via GetInstance().\n\n";

        HFL::SetColor(HFL::EColor::White);
        HFL::WaitForInput();
    }
}