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

namespace HFL
{
	void PrintMainMenu();
	int GetValidMenuInput(int MaxOption);
	void ClearScreen();
	void WaitForInput();
	void PrintHeader(const std::string& Title);
	std::filesystem::path GetSaveDirectory(const std::string& PrincipleNamespace);

    // ------------------------------------------------------------------------
    // TIME UTILITIES
    // ------------------------------------------------------------------------

    // Simple blocking wait using std::chrono
    void Wait(float Seconds);

    // Returns time elapsed since the last time this was called
    float GetDeltaTime();
}