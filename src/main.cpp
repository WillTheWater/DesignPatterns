#pragma once

#include "SOLID/SingleResponsibilityPrinciple.h"
#include "SOLID/OpenClosedPrinciple.h"

void RunDemo()
{
    bool IsRunning = true;
    while (IsRunning)
    {
        HFL::ClearScreen();
        HFL::PrintMainMenu();
        int Choice = HFL::GetValidMenuInput(2);

        switch (Choice)
        {
        case 1:
            SRP::RunDemo();
            break;
        case 2:
            OCP::RunDemo();
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