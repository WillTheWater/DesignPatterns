#pragma once

#include "SOLID/SingleResponsibilityPrinciple.h"
#include "SOLID/OpenClosedPrinciple.h"
#include "SOLID/LiskovSubstitutionPrinciple.h"

void RunDemo()
{
    bool IsRunning = true;
    while (IsRunning)
    {
        HFL::ClearScreen();
        HFL::PrintMainMenu();
        int Choice = HFL::GetValidMenuInput(3);

        switch (Choice)
        {
        case 1:
            SRP::RunDemo(); // Single Responsibility Principle Demo
            break;
        case 2:
            OCP::RunDemo(); // Open-Closed Principle Demo
            break;
        case 3:
            LSP::RunDemo(); // Liskov Substitution Principle Demo
            break;
        case 0:
            IsRunning = false;
            break;
        }
    }
}


int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    RunDemo();
    return 0;
}