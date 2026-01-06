#include "Structural/Proxy.h"

namespace PRX
{
    // =========================================================================
    // SERVER CONNECTION (The Requestor / The Network)
    // =========================================================================
    // This class represents the link to the "Asset Server".
    // In a real app, this would use TCP/IP or HTTP.
    // For this demo, we simulate network latency with 'std::this_thread::sleep_for'.

    std::unique_ptr<RealTexture> ServerConnection::DownloadAsset(int AssetID)
    {
        std::cout << ">> [Network] Connecting to Server...\n";
        // Simulate Network Latency (Slow!)
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    
        std::cout << ">> [Network] Downloading Asset ID: " << AssetID << "...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
        // Create the Heavy RealTexture (This is the expensive part).
        std::string Name = "Texture_" + std::to_string(AssetID);
        auto Texture = std::make_unique<RealTexture>(Name, 512, 512);
    
        std::cout << ">> [Network] Download Complete.\n";
        return Texture;
    }

    // =========================================================================
    // THE REAL SUBJECT (The Heavy Object - The Server)
    // =========================================================================

    RealTexture::RealTexture(std::string Name, int Width, int Height)
        : Name{Name}
        , Width{Width}
        , Height{Height}
    {
        std::cout << "[System] RealTexture Created (Heavy Object).\n";
    }

    // =========================================================================
    // PROXY IMPLEMENTATION (The Logic)
    // =========================================================================

    RemoteTextureProxy::RemoteTextureProxy(int AssetID)
        : AssetID{AssetID}
    {
        std::cout << "[System] RemoteTextureProxy Created (Lightweight).\n";

        // 1. Create the Requestor (The Network Link)
        Requestor = std::make_unique<ServerConnection>();

        // 2. RealSubject is NULL (Lazy Initialization)
        // The Proxy starts empty. It does not call 'DownloadAsset()' yet.
        std::cout << "[System] Proxy is empty. (Lazy Initialization)\n";
    }

    // This is where the "Proxy Logic" happens.
    int RemoteTextureProxy::GetWidth()
    {
        // The Game Logic calls GetWidth().

        if (RealSubject)
        {
            // CASE 1: CACHED HIT
            // The RealTexture is already loaded.
            // We return data instantly. No network latency.
            std::cout << ">> [Proxy] Returning Cached Width (Instant).\n";
            return RealSubject->GetWidth();
        }
        else
        {
            // CASE 2: CACHE MISS
            // The RealTexture is NULL. We need to get it.
            std::cout << ">> [Proxy] Cache Miss! No RealTexture loaded.\n";

            // The Proxy acts as a go-between. It asks the Requestor (Server).
            if (Requestor)
            {
                // This call is slow (Network Latency).
                // But it only happens ONCE.
                std::cout << ">> [Proxy] Delegating to ServerConnection...\n";
                RealSubject = std::move(Requestor->DownloadAsset(123));

                std::cout << ">> [Proxy] RealTexture received. Caching it...\n";
            }
            else
            {
                std::cout << "[Error] No ServerConnection attached to Proxy.\n";
                return 0;
            }

            // Now we have the data, we can return it.
            return RealSubject->GetWidth();
        }
    }

    int RemoteTextureProxy::GetHeight()
    {
        // Same Lazy Logic as GetWidth()
        if (RealSubject) return RealSubject->GetHeight();
        else
        {
            if (Requestor)
            {
                RealSubject = std::move(Requestor->DownloadAsset(123));
            }
            return 0;
        }
    }

    // =========================================================================
    // DEMO IMPLEMENTATION
    // =========================================================================
    void RunDemo()
    {
        // Clear buffer
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // --- STEP 1: INTRODUCTION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Proxy Pattern (Remote Assets)");

        std::cout << "Definition:\n";
        std::cout << "Provide a surrogate or placeholder for another object to control access to it.\n";
        std::cout << "Control access to an expensive object.\n\n";

        std::cout << "In This Demo:\n";
        std::cout << "Simulate a 'Game Engine' downloading textures from a 'Server'.\n";
        std::cout << "1. RealTexture: The heavy image that lives on the Server.\n";
        std::cout << "2. RemoteTextureProxy: The lightweight stub in the Game.\n";
        std::cout << "3. Lazy Loading: The Proxy only downloads the texture when looked at.\n";

        HFL::WaitForInput();

        // --- STEP 2: THE ARCHITECTURE ---
        HFL::ClearScreen();
        HFL::PrintHeader("The Architecture");

        std::cout << "3 Key Roles:\n\n";

        std::cout << "1. ITexture (The Target Interface):\n";
        std::cout << "   - Defines 'GetWidth', 'GetHeight'.\n";
        std::cout << "   - The Game Logic uses this.\n";
        std::cout << "   - It doesn't know if it's talking to a RealTexture or a Proxy.\n\n";

        std::cout << "2. RealTexture (The RealSubject):\n";
        std::cout << "   - The heavy image data.\n";
        std::cout << "   - Loading it takes 'Network Time' (1.5s simulated).\n\n";

        std::cout << "3. RemoteTextureProxy (The Proxy):\n";
        std::cout << "   - Implements ITexture (Looks like a texture to the Game).\n";
        std::cout << "   - Is 'Lightweight'. It holds a pointer to a ServerConnection.\n";
        std::cout << "   - RealSubject is NULL initially (Lazy).\n\n";

        std::cout << "THE FLOW:\n";
        std::cout << "1. Game creates RemoteTextureProxies (Instant).\n";
        std::cout << "2. Game calls Proxy->GetWidth().\n";
        std::cout << "3. Proxy checks: Do I have RealTexture? No.\n";
        std::cout << "4. Proxy asks ServerConnection to Download(RealTexture).\n";
        std::cout << "5. ServerConnection returns RealTexture (Wait 1.5s).\n";
        std::cout << "6. Proxy Caches RealTexture. Game gets Width.\n";
        std::cout << "7. Next time Game calls Proxy->GetWidth(), it returns instantly.\n\n";

        HFL::WaitForInput();

        // --- STEP 3: INTERACTIVE SYSTEM ---

        bool InDemo = true;

        // Persistent proxy
        std::unique_ptr<ITexture> TextureProxy;

        while (InDemo)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("Remote Asset Manager (Proxy Demo)");

            std::cout << "Proxy Status: ";
            if (!TextureProxy)
                std::cout << "NONE (No Proxy Created)\n";
            else
                std::cout << "ACTIVE (May Be Loaded or Cached)\n";

            std::cout << "\nSelect an Action:\n";
            std::cout << "1. Create Texture Proxy (Instant, Lightweight)\n";
            std::cout << "2. Access Texture Width (Triggers Lazy Download)\n";
            std::cout << "3. Access Texture Width Again (Cache Hit)\n";
            std::cout << "0. Exit Demo\n";
            std::cout << "\nChoice: ";

            int Choice;
            std::cin >> Choice;

            if (std::cin.fail())
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }

            if (Choice == 0)
                break;

            switch (Choice)
            {
            case 1:
            {
                std::cout << "\n[Action] Creating RemoteTextureProxy...\n";
                TextureProxy = std::make_unique<RemoteTextureProxy>(999);
                std::cout << ">> Proxy created instantly. No network call made.\n";
                break;
            }

            case 2:
            {
                if (!TextureProxy)
                {
                    std::cout << "\n[Error] No proxy exists. Create one first.\n";
                    break;
                }

                std::cout << "\n[Action] First access to texture data.\n";
                std::cout << ">> Expect network latency...\n";

                int Width = TextureProxy->GetWidth();
                std::cout << ">> Texture Width: " << Width << "\n";
                break;
            }

            case 3:
            {
                if (!TextureProxy)
                {
                    std::cout << "\n[Error] No proxy exists. Create one first.\n";
                    break;
                }

                std::cout << "\n[Action] Second access to texture data.\n";
                std::cout << ">> Expect instant response (cached).\n";

                int Width = TextureProxy->GetWidth();
                std::cout << ">> Texture Width: " << Width << " (Cached)\n";
                break;
            }

            default:
                std::cout << "\n[Error] Invalid choice.\n";
                break;
            }

            HFL::WaitForInput();
        }


        // --- STEP 4: CONCLUSION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Conclusion");

        std::cout << "Summary:\n\n";
        std::cout << "1. Lazy Initialization:\n";
        std::cout << "   Created 1 Proxy instantly. The RealTexture was NOT created.\n";
        std::cout << "   Only downloaded the RealTexture when GetWidth() was called.\n\n";

        std::cout << "2. Virtual Proxy:\n";
        std::cout << "   The Game Logic didn't know it was talking to a Server.\n";
        std::cout << "   The Proxy handled the network request transparently.\n\n";

        std::cout << "3. Performance:\n";
        std::cout << "   The first call took 1.5s (Simulated Network).\n";
        std::cout << "   The second call took <1ms (Cached).\n";
        std::cout << "   This allows instant Game Start times.\n";

        std::cout << std::setw(40) << "End of Demo\n";
        HFL::WaitForInput();
    }
}