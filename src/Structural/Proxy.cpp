#include "Structural/Proxy.h"

namespace PRX
{
    // =========================================================================
    // LOW LEVEL MODULE: SERVER CONNECTION
    // ROLE: Simulates the network layer and high-latency asset streaming.
    // =========================================================================

    std::unique_ptr<RealTexture> ServerConnection::DownloadAsset(int AssetID)
    {
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << ">> [Network] Opening Streaming Channel";
        HFL::WaitDots(0.2);

        HFL::Wait(0.6);

        std::cout << ">> [Network] Transferring ID: " << AssetID;
        HFL::WaitDots(0.4);

        std::string Name = "HighRes_Tex_" + std::to_string(AssetID);
        auto Texture = std::make_unique<RealTexture>(Name, 4096, 4096);

        std::cout << ">> [Network] 100% Downloaded.\n";
        return Texture;
    }

    // =========================================================================
    // LOW LEVEL MODULE: THE REAL SUBJECT
    // ROLE: The heavy resource (Texture) that consumes significant RAM.
    // =========================================================================

    RealTexture::RealTexture(std::string Name, int Width, int Height)
        : Name{ Name }, Width{ Width }, Height{ Height }
    {
        HFL::SetColor(HFL::EColor::Yellow);
        std::cout << "[System] Memory Allocated: " << Name << " (" << Width << "x" << Height << ")\n";
    }

    RealTexture::~RealTexture()
    {
        HFL::SetColor(HFL::EColor::Red);
        std::cout << "[System] RealTexture '" << Name << "' released (RAM Reclaimed).\n";
    }

    // =========================================================================
    // ABSTRACTION LAYER: THE VIRTUAL PROXY
    // ROLE: Acts as a lightweight placeholder (Stub) for the Real Subject.
    // It defers the "Creation Cost" until the data is actually requested.
    // =========================================================================

    RemoteTextureProxy::RemoteTextureProxy(int AssetID) : AssetID{ AssetID }
    {
        // The Proxy owns the requester, but NOT the heavy texture.
        Requestor = std::make_unique<ServerConnection>();
    }

    int RemoteTextureProxy::GetWidth()
    {
        // Lazy Initialization: Check if the real object already exists.
        if (RealSubject) return RealSubject->GetWidth();

        HFL::SetColor(HFL::EColor::Cyan);
        std::cout << ">> [Proxy] Data not found in Local RAM. Triggering Remote Fetch...\n";

        // Materialize the subject only when needed.
        RealSubject = std::move(Requestor->DownloadAsset(AssetID));
        return RealSubject->GetWidth();
    }

    int RemoteTextureProxy::GetHeight()
    {
        // Re-use existing logic to ensure lazy-loading happens.
        if (!RealSubject) GetWidth();
        return RealSubject ? RealSubject->GetHeight() : 0;
    }

    bool RemoteTextureProxy::IsLoaded() const
    {
        // Internal state check for the UI to prove lazy loading is working.
        return RealSubject != nullptr;
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
        HFL::PrintHeader("PROXY PATTERN");

        HFL::PrintSection("THE DEFINITION");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "A Proxy provides a surrogate or placeholder for another object.\n"
            << "It controls access to the original object, allowing you to perform\n"
            << "actions before or after the request reaches the real subject.\n\n";

        HFL::PrintSection("THE GOAL");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Virtual Proxy aims to optimize resource-heavy systems by deferring\n"
            << "the cost of object creation. By using a lightweight 'stand-in', the\n"
            << "application remains responsive, loading expensive data only when\n"
            << "the user or system explicitly requests it (Lazy Loading).\n\n";

        HFL::PrintSection("THE EXAMPLE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "This demonstration features a Remote Asset Catalog with three layers:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE SUBJECT (INTERFACE): ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "ITexture defines the shared API for images.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE REAL SUBJECT:        ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The 4K Texture that requires heavy network/RAM usage.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE VIRTUAL PROXY:       ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The 'Smart Wrapper' that manages the download state.\n\n";

        HFL::PrintSection("THE BENEFIT");
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] EFFICIENCY:    ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The game starts instantly; 0kb transferred during initialization.\n";
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] SMART LOADING: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Assets materialize only when accessed, saving bandwidth.\n";
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] LIFECYCLE:     ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Proxy encapsulates the complexity of remote fetching logic.\n";

        HFL::WaitForInput();

        // ======================== THE ARCHITECTURE ========================
        HFL::ClearScreen();
        HFL::PrintHeader("THE ARCHITECTURE");

        HFL::PrintSection("THE SURROGATE");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Normally, a Client talks directly to a Heavy Object. In this pattern,\n"
            << "these is a Middleman. The Client thinks it's talking to the Texture,\n"
            << "but it's actually talking to a Proxy that isn't loaded yet.\n\n";

        HFL::PrintSection("IMPLEMENTATION");

        // ======================== INTERFACE ========================
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] ITexture (Interface)\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ROLE:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Contract. Ensures Proxy and Real Object look identical to the Client.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    SCOPE:          ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Defines GetWidth() and GetHeight().\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    MANDATE:        ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Must be used by the Client to allow for transparent substitution.\n\n";

        // ======================== REAL SUBJECT ========================
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] RealTexture (Heavy Object)\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ROLE:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Actual Data. Contains the raw pixels and memory buffers.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    SCOPE:          ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Handles actual file I/O and GPU memory allocation.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    COST:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "High latency to create; high RAM footprint while alive.\n\n";

        // ======================== PROXY ========================
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] RemoteTextureProxy (The Middleman)\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ROLE:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Lazy Loader. Holds a pointer to the Real Subject.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    SCOPE:          ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Intercepts calls. If RealSubject is null, it downloads it first.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ADVANTAGE:      ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Controls the 'When' and 'How' of resource acquisition.\n\n";

        HFL::SetColor(HFL::EColor::White);
        HFL::WaitForInput();

        // ======================== INITIALIZATION ========================
        std::vector<std::unique_ptr<ITexture>> AssetLibrary;
        const int CatalogSize = 10;

        // ======================== MAIN LOOP ========================
        while (true)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("VIRTUAL ASSET MANAGER");

            HFL::PrintSection("RAM STATUS (PROXIES VS REAL)");
            if (AssetLibrary.empty())
            {
                HFL::SetColor(HFL::EColor::Red);
                std::cout << " [!] Catalog Not Initialized. Press [1] to build library.\n";
            }
            else
            {
                int LoadedCount = 0;
                std::cout << " ";
                for (int i = 0; i < (int)AssetLibrary.size(); ++i)
                {
                    auto* P = static_cast<RemoteTextureProxy*>(AssetLibrary[i].get());
                    if (P->IsLoaded())
                    {
                        HFL::SetColor(HFL::EColor::Green);
                        std::cout << "[REAL] ";
                        LoadedCount++;
                    }
                    else
                    {
                        HFL::SetColor(HFL::EColor::Gray);
                        std::cout << "[PRXY] ";
                    }

                    if ((i + 1) % 5 == 0) std::cout << "\n ";
                }

                HFL::SetColor(HFL::EColor::White);
                std::cout << "\n Statistics: " << LoadedCount << "/" << AssetLibrary.size()
                    << " Assets fully materialized in memory.\n";
            }

            HFL::PrintSection("COMMANDS");
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [1] "; HFL::SetColor(HFL::EColor::White); std::cout << "Reset & Initialize (Instant Setup)\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [2] "; HFL::SetColor(HFL::EColor::White); std::cout << "Inspect Asset (Lazy Load specific index)\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [0] "; HFL::SetColor(HFL::EColor::White); std::cout << "CONTINUE\n";

            int Choice = HFL::GetValidMenuInput(2);
            if (Choice == 0) break;

            if (Choice == 1)
            {
                HFL::PrintSection("ACTION: FLUSHING & RE-INITIALIZING");

                if (!AssetLibrary.empty())
                {
                    std::cout << "Reclaiming RAM from existing assets...\n";
                    AssetLibrary.clear();
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                }

                std::cout << "Generating " << CatalogSize << " lightweight proxies";
                HFL::WaitDots(0.2);

                for (int i = 0; i < CatalogSize; ++i)
                {
                    AssetLibrary.push_back(std::make_unique<RemoteTextureProxy>(500 + i));
                }

                HFL::SetColor(HFL::EColor::Green);
                std::cout << " DONE!\n";
                HFL::WaitForInput();
            }
            else if (Choice == 2)
            {
                if (AssetLibrary.empty())
                {
                    HFL::SetColor(HFL::EColor::Red);
                    std::cout << "\n[Error] Catalog is empty. Initialize first!\n";
                    HFL::WaitForInput();
                    continue;
                }

                HFL::PrintSection("CHOOSE ASSET TO VIEW");
                std::cout << "Enter Asset Number (1 to " << AssetLibrary.size() << "): ";
                int UserIndex;
                if (!(std::cin >> UserIndex)) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    continue;
                }

                int VectorIndex = UserIndex - 1;

                if (VectorIndex >= 0 && VectorIndex < (int)AssetLibrary.size())
                {
                    HFL::PrintSection("DISPLAYING ASSET");

                    // Accessing data through the Proxy interface
                    int W = AssetLibrary[VectorIndex]->GetWidth();
                    int H = AssetLibrary[VectorIndex]->GetHeight();

                    HFL::SetColor(HFL::EColor::White);
                    std::cout << "\nAsset Info: " << W << "x" << H << "px\n";
                    std::cout << "Status: Success. Only this asset was fetched from the server.\n";
                }
                else
                {
                    HFL::SetColor(HFL::EColor::Red);
                    std::cout << "Invalid index selection.\n";
                }
                HFL::WaitForInput();
            }
        }

        // ======================== CONCLUSION ========================
        HFL::ClearScreen();
        HFL::PrintHeader("CONCLUSION");

        HFL::PrintSection("STRUCTURAL OBSERVATIONS");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The implementation of the Virtual Proxy confirms the following:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] TRANSPARENT SUBSTITUTION: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Client logic interacts only with 'ITexture'. It remains\n"
            << "    completely unaware whether it is talking to a Proxy or a Real Subject.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] ENCAPSULATED COMPLEXITY:  ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The 'messy' details of network latency, streaming, and caching are\n"
            << "    hidden inside the Proxy, keeping the Asset Library logic clean.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] RESOURCE MANAGEMENT:      ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "By utilizing std::unique_ptr, the Proxy ensures that when it is\n"
            << "    destroyed, the heavy RealSubject is also safely purged from RAM.\n\n";

        HFL::PrintSection("SUMMARY");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The Proxy Pattern ensures that software is:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] RESPONSIVE: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Boot times are near-instant because we defer the cost of initialization.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] EFFICIENT:  ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "System resources (RAM/Bandwidth) are only allocated for data actually used.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] PREDICTABLE: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Access to sensitive or heavy resources is centralized through a single gatekeeper.\n\n";

        HFL::SetColor(HFL::EColor::White);
        HFL::WaitForInput();
    }
}