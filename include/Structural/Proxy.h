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
// [ITexture]: The interface both the "Real Texture" and "Proxy" implement.
// [RealTexture]: The heavy object (Server-side) holding raw pixel data.
// [RemoteTextureProxy]: The lightweight placeholder the Game Logic holds.
// [ServerConnection]: The network handler the Proxy uses to fetch data.
//
// THE BENEFIT:
// * Performance: Create 100 Texture Proxies instantly without memory overhead.
// * Efficiency: Pay the cost of downloading/loading only when actually used.
// * Control: The Proxy can handle cache logic or "Loading..." states.
// =========================================================================

namespace PRX
{
    // =========================================================================
    // THE ABSTRACTION (The Interface)
    // Defines the contract for both the Real Subject and the Proxy.
    // The Game Logic only knows about this interface.
    // =========================================================================
    class ITexture
    {
    public:
        virtual ~ITexture() = default;

        virtual int GetWidth() = 0;
        virtual int GetHeight() = 0;
    };

    // =========================================================================
    // THE REAL SUBJECT (The Heavy Object)
    // Represents a high-resolution image. In a real scenario, this lives
    // on a server or deep in the disk. Loading this is "Expensive".
    // =========================================================================
    class RealTexture : public ITexture
    {
    public:
        RealTexture(std::string Name, int Width, int Height);
        ~RealTexture();

        int GetWidth() override { return Width; }
        int GetHeight() override { return Height; }

    private:
        std::string Name;
        int Width;
        int Height;
    };

    // =========================================================================
    // THE REQUESTOR (The Network Link)
    // This class represents the connection to the asset server.
    // It is used by the Proxy to "download" the heavy RealSubject.
    // =========================================================================
    class ServerConnection
    {
    public:
        // The heavy method that simulates network latency and asset creation.
        std::unique_ptr<RealTexture> DownloadAsset(int AssetID);
    };

    // =========================================================================
    // THE PROXY (The Placeholder)
    // Implements the interface so the Game Logic can treat it like a real texture.
    // It delegates to the ServerConnection only when data is requested.
    // =========================================================================
    class RemoteTextureProxy : public ITexture
    {
    public:
        // The Proxy is created with a "Ticket" (ID/GUID) rather than raw data.
        RemoteTextureProxy(int AssetID);

        // LAZY LOADING: Checks if RealSubject exists; if not, triggers download.
        bool IsLoaded() const;
        int GetWidth() override;
        int GetHeight() override;

    private:
        int AssetID;

        // The "Lazy" pointer. Null until the first request.
        std::unique_ptr<RealTexture> RealSubject;

        // The helper used to fetch the real object.
        std::unique_ptr<ServerConnection> Requestor;
    };

    // =========================================================================
    // DEMO
    // =========================================================================
    void RunDemo();
}