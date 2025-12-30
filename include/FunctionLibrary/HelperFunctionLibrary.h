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

namespace HFL
{
	void ClearScreen();
	void WaitForInput();
	void PrintHeader(const std::string& Title);

	// Takes the Design Principle Namespace as a String.
	std::filesystem::path GetSaveDirectory(const std::string& PrincipleNamespace);
}