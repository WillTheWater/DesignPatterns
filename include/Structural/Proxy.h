#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"

// =========================================================================
// STRUCTURAL DESIGN PATTERNS: Proxy Pattern
// =========================================================================
// "Provide a surrogate or placeholder for another object to control access to it."
//
// THE GOAL:
// 1. Lazy Initialization: Don't create a heavy object until needed.
// 2. Access Control: Validate permissions before accessing the object.
// 3. Remote Access: Act as a local representative for a remote object (Server).
//
// THE EXAMPLE:
// A Remote Texture Asset System.
// 1. RealTexture: The heavy object that holds pixel data.
//    Loading it is slow (Simulated).
// 2. RemoteTextureProxy (The Proxy): The lightweight object the Game holds.
//    It looks like a Texture, but it's empty until needed.
// 3. ServerConnection: The network interface.
//    The Proxy uses this to download the heavy RealTexture from the "Server".
//
// BENEFIT:
// 1. Performance: Create 100 Texture Proxies instantly.
//    Pay the cost of downloading only when actually used.
// =========================================================================

namespace PRX
{
    // ------------------------------------------------------------------------
    // 1. THE TARGET INTERFACE (The Contract)
    // ------------------------------------------------------------------------
    // This is the interface both the "Real Texture" (Server) and "Proxy" (Client) implement.
    // The Game Logic only knows about this interface.
    class ITexture
    {
    public:
        virtual ~ITexture() = default;

        virtual int GetWidth() = 0;
        virtual int GetHeight() = 0;
    };

    // ------------------------------------------------------------------------
    // 2. THE REAL SUBJECT (The Heavy Object - The Server)
    // ------------------------------------------------------------------------
    // In a real engine, this class lives on the Server machine.
    // It represents a high-resolution image.
    // Loading this data is "Expensive".
    class RealTexture : public ITexture
    {
    public:
        RealTexture(std::string Name, int Width, int Height);

        // The RealSubject implements the interface.
        int GetWidth() override { return Width; }
        int GetHeight() override { return Height; }

    private:
        std::string Name;
        int Width;
        int Height;
    };

    // ------------------------------------------------------------------------
    // 3. THE PROXY (The Placeholder / The Client)
    // ------------------------------------------------------------------------
    // This class represents the "Catalog Entry" or "Stub".
    // It implements the interface so the Game can treat it like a real texture.
    // However, it does NOT load the heavy RealTexture immediately.
    // It delegates to a "Requestor" (ServerConnection) to get the data.
    class RemoteTextureProxy : public ITexture
    {
    public:
        // The Proxy is created with a "Ticket" (An ID to request from server).
        // In a real app, this might be a file path or a GUID.
        RemoteTextureProxy(int AssetID);

        // The Game Logic calls this method.
        // The Proxy checks: "Do I have the real object? If not, ask Server."
        int GetWidth() override;
        int GetHeight() override;

    private:
        int AssetID;
        // The "Lazy" pointer. It is null until the first time data is requested.
        std::unique_ptr<RealTexture> RealSubject;

        // The Requestor. The Proxy uses this to "download" the RealSubject.
        std::unique_ptr<class ServerConnection> Requestor;
    };

    // ------------------------------------------------------------------------
    // 4. THE REQUESTOR (The Server / The Heavy Machinery)
    // ------------------------------------------------------------------------
    // This class represents the network connection to the asset server.
    // It is the "Adaptee" that the Proxy (RemoteTextureProxy) uses.
    class ServerConnection
    {
    public:
        ServerConnection()
        {
            std::cout << "[System] ServerConnection (Network Link) Established.\n";
        }

        // This is the heavy method that actually does the work.
        // The Proxy calls this to get the 'RealTexture' data.
        std::unique_ptr<RealTexture> DownloadAsset(int AssetID);
    };

    // ------------------------------------------------------------------------
    // 4. DEMO
    // ------------------------------------------------------------------------
    void RunDemo();
}