#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <cstdlib>
#include <limits>
#include <ctime>
#include <memory>
#include <type_traits>
#include <chrono>
#include <thread>
#include <algorithm>
#include <stack>
#include <functional>
#include <random>
#include <type_traits>
#include <optional>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace HFL
{
    enum class EColor 
    {
        Black,      Red,    Green, 
        Yellow,     Blue,   Magenta, 
        Cyan,       White,  Gray, 

        BrightRed,      BrightGreen, 
        BrightYellow,   BrightBlue, 
        BrightMagenta,  BrightCyan, 
        BrightWhite
    };

	void PrintMainMenu();
	int GetValidMenuInput(int MaxOption);
	void ClearScreen();
	void WaitForInput();
	void PrintHeader(const std::string& Title);
    void PrintSection(const std::string& Title, EColor Color = EColor::Cyan);
    void SetColor(EColor Color);
	std::filesystem::path GetSaveDirectory(const std::string& PrincipleNamespace);


    // Simple wait using std::chrono
    void Wait(float Seconds);
    void WaitDots(float Seconds);

    // Random number generator std::random
    static std::random_device rd;
    static std::mt19937 gen(rd());
    
    template <typename T>
    T GetRandom(T Min, T Max)
    {
        if constexpr (std::is_floating_point_v<T>) 
        {
            std::uniform_real_distribution<T> dis(Min, Max);
            return dis(gen);
        }
        else 
        {
            std::uniform_int_distribution<T> dis(Min, Max);
            return dis(gen);
        }
    }
}