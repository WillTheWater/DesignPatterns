#include "Creational/Singleton.h"

namespace SGT
{
    // ------------------------------------------------------------------------
    // SINGLETON IMPLEMENTATION
    // ------------------------------------------------------------------------
    AssetManager::AssetManager()
    {
        std::cout << "[System] AssetManager Singleton Initialized.\n";
    }

    AssetManager& AssetManager::GetInstance()
    {
        // 'instance' is a static variable inside a function.
        // 1. It is created the first time this function is called.
        // 2. It is destroyed automatically when the program exits.
        // 3. It is Thread-Safe: The compiler ensures only one thread runs the constructor.
        static AssetManager instance;
        return instance;
    }

    // ------------------------------------------------------------------------
    // TEMPLATE SPECIALIZATION IMPLEMENTATIONS
    // ------------------------------------------------------------------------
    // We provide a custom implementation of 'LoadAsset' for EVERY specific type.
    // LoadAsset<Texture> -> Use function A (Texture Cache).
    // LoadAsset<Sound>   -> Use function B (Sound Cache).
    // LoadAsset<Font>    -> Use function C (Font Cache).

    // --- VERSION 1: TEXTURE ---
    template <>
    inline void AssetManager::LoadAsset<Texture>(const std::string& AssetPath)
    {
        std::cout << ">> [Manager] Routing request to Texture Cache...\n";

        auto It = TextureCache.find(AssetPath);
        if (It != TextureCache.end())
        {
            // This has been loaded this before.
            It->second++;
            std::cout << "   [Found Asset] Texture '" << AssetPath << "' found.\n";
            std::cout << "   [Status] New Reference Count: " << It->second << "\n";
        }
        else
        {
            //  Load from "Disk" (Simulated)
            std::cout << "   Loading Texture '" << AssetPath << "' from disk...\n";
            TextureCache[AssetPath] = 1;
            std::cout << "   [Status] Texture Loaded.\n";
        }
    }

    // --- VERSION 2: SOUND (NEW) ---
    template <>
    inline void AssetManager::LoadAsset<Sound>(const std::string& AssetPath)
    {
        std::cout << ">> [Manager] Routing request to Sound Cache...\n";

        auto It = SoundCache.find(AssetPath);
        if (It != SoundCache.end())
        {
            // This has been loaded this before.
            It->second++;
            std::cout << "   [Found Asset] Sound '" << AssetPath << "' found.\n";
            std::cout << "   [Status] New Reference Count: " << It->second << "\n";
        }
        else
        {
            //  Load from "Disk" (Simulated)
            std::cout << "   Loading Sound '" << AssetPath << "' from disk...\n";
            SoundCache[AssetPath] = 1;
            std::cout << "   [Status] Sound Loaded.\n";
        }
    }

    // --- VERSION 3: FONT ---
    template <>
    inline void AssetManager::LoadAsset<Font>(const std::string& AssetPath)
    {
        std::cout << ">> [Manager] Routing request to Font Cache...\n";

        auto It = FontCache.find(AssetPath);
        if (It != FontCache.end())
        {
            // This has been loaded this before.
            It->second++;
            std::cout << "   [Found Asset] Font '" << AssetPath << "' found.\n";
            std::cout << "   [Status] New Reference Count: " << It->second << "\n";
        }
        else
        {
            //  Load from "Disk" (Simulated)
            std::cout << "   Loading Font '" << AssetPath << "' from disk...\n";
            FontCache[AssetPath] = 1;
            std::cout << "   [Status] Font Loaded.\n";
        }
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
        HFL::PrintHeader("Singleton Pattern (Asset Manager)");

        std::cout << "Definition:\n";
        std::cout << "Ensure a class has only one instance.\n";
        std::cout << "Provide a global point of access to it.\n\n";

        std::cout << "In This Demo:\n";
        std::cout << "There is an AssetManager Singleton.\n";
        std::cout << "It uses TEMPLATE SPECIALIZATION to handle Textures, Sounds, and Fonts.\n";
        std::cout << "It has three separate 'LoadAsset' functions.\n";

        HFL::WaitForInput();

        // --- STEP 2: THE RULES ---
        HFL::ClearScreen();
        HFL::PrintHeader("Step 1: The Rules");

        std::cout << "To enforce a Singleton, there are 4 rules:\n\n";

        std::cout << "1. Private Constructor:\n";
        std::cout << "   private:\n";
        std::cout << "       AssetManager();\n";
        std::cout << "   };\n";
        std::cout << "   Result: You cannot type 'new AssetManager()'.\n\n";

        std::cout << "2. Static GetInstance():\n";
        std::cout << "   Checks if instance exists. If yes, return it.\n";
        std::cout << "   If no, create it (Lazy Initialization).\n\n";

        std::cout << "3. Deleted Copy Constructor:\n";
        std::cout << "   AssetManager(const AssetManager&) = delete;\n";
        std::cout << "   Prevents accidentally making a copy of the Singleton.\n\n";

        std::cout << "4. Deleted Assignment:\n";
        std::cout << "   operator=(const AssetManager&) = delete;\n";
        std::cout << "   Prevents overwriting the single instance.\n\n";

        std::cout << "The Template System:\n";
        std::cout << "   We have 'struct Texture', 'struct Sound', 'struct Font'.\n";
        std::cout << "   They act as 'Tags' to tell the function which Cache to use.\n";

        HFL::WaitForInput();

        // --- STEP 3: INTERACTIVE SYSTEM ---

        // Access the Singleton
        AssetManager& Manager = AssetManager::GetInstance();

        bool InDemo = true;
        while (InDemo)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("Asset Manager");

            std::cout << "Select an Asset to Load:\n";
            std::cout << "1. Load Texture 'Hero.png'\n";
            std::cout << "2. Load Texture 'Wall.jpg'\n";
            std::cout << "3. Load Sound 'Explosion.wav'\n";
            std::cout << "4. Load Sound 'BGM.mp3'\n";
            std::cout << "5. Reload Texture 'Hero.png' (Test Singleton Cache)\n";
            std::cout << "6. Reload Sound 'Explosion.wav' (Test Singleton Cache)\n";
            std::cout << "0. Exit Demo\n";
            std::cout << "\nChoice: ";

            int Choice;
            std::cin >> Choice;

            if (std::cin.fail()) { std::cin.clear(); std::cin.ignore(); continue; }

            if (Choice == 0) break;

            if (Choice == 1)
            {
                // Generates 'LoadAsset<Texture>' version
                Manager.LoadAsset<Texture>("Hero.png");
                HFL::WaitForInput();
            }
            else if (Choice == 2)
            {
                // Generates 'LoadAsset<Texture>' version (Different Asset)
                Manager.LoadAsset<Texture>("Wall.jpg");
                HFL::WaitForInput();
            }
            else if (Choice == 3)
            {
                // Generates 'LoadAsset<Sound>' version
                Manager.LoadAsset<Sound>("Explosion.wav");
                HFL::WaitForInput();
            }
            else if (Choice == 4)
            {
                // Generates 'LoadAsset<Sound>' version (Different Asset)
                Manager.LoadAsset<Sound>("BGM.mp3");
                HFL::WaitForInput();
            }
            else if (Choice == 5)
            {
                // This tests the Singleton "Cache" logic for Textures
                std::cout << "\nAttempting to load 'Hero.png' again...\n";
                Manager.LoadAsset<Texture>("Hero.png");
                HFL::WaitForInput();
            }
            else if (Choice == 6)
            {
                // This tests the Singleton "Cache" logic for Sounds
                std::cout << "\nAttempting to load 'Explosion.wav' again...\n";
                Manager.LoadAsset<Sound>("Explosion.wav");
                HFL::WaitForInput();
            }
        }

        // --- STEP 4: GLOBAL ACCESS ---
        HFL::ClearScreen();
        HFL::PrintHeader("Step 2: Global Access");

        std::cout << "Because GetInstance() is static and public,\n";
        std::cout << "we can access the manager from ANYWHERE.\n\n";
        std::cout << "Accessing from a totally different context...\n";

        // We can just call GetInstance again directly
        SGT::AssetManager::GetInstance().LoadAsset<Texture>("Floor.png");

        HFL::WaitForInput();

        // --- STEP 5: CONCLUSION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Conclusion");

        std::cout << "Summary:\n\n";
        std::cout << "1. Singleton Rule:\n";
        std::cout << "   Only ONE AssetManager existed.\n";
        std::cout << "   It remembered the reference counts for Textures, Sounds, AND Fonts.\n\n";

        std::cout << "2. Clean Templates:\n";
        std::cout << "   We used 'template <> void LoadAsset<Texture>'\n";
        std::cout << "   And 'template <> void LoadAsset<Sound>'.\n";
        std::cout << "   And 'template <> void LoadAsset<Font>'.\n";

        std::cout << "3. Efficiency:\n";
        std::cout << "   By caching, we avoided loading 'Hero.png' and 'Explosion.wav' twice.\n\n";

        std::cout << std::setw(40) << "End of Demo\n";
        HFL::WaitForInput();
    }
}