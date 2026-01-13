#pragma once

#include "SOLID/SingleResponsibilityPrinciple.h"
#include "SOLID/OpenClosedPrinciple.h"
#include "SOLID/LiskovSubstitutionPrinciple.h"
#include "SOLID/InterfaceSegregationPrinciple.h"
#include "SOLID/DependencyInversionPrinciple.h"
#include "Creational/Singleton.h"
#include "Creational/FactoryMethod.h"
#include "Creational/AbstractFactory.h"
#include "Creational/Prototype.h"
#include "Creational/Builder.h"
#include "Structural/Facade.h"
#include "Structural/Adapter.h"
#include "Structural/Proxy.h"
#include "Structural/Bridge.h"
#include "Structural/Composite.h"
#include "Structural/Decorator.h"
#include "Structural/Flyweight.h"
#include "Behavioral/NullObject.h"
#include "Behavioral/Strategy.h"
#include "Behavioral/TemplateMethod.h"
#include "Behavioral/Iterator.h"
#include "Behavioral/Command.h"
#include "Behavioral/Observer.h"

void RunDemo()
{
    bool IsRunning = true;
    while (IsRunning)
    {
        HFL::ClearScreen();
        HFL::PrintMainMenu();
        int Choice = HFL::GetValidMenuInput(23);

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
        case 4:
            ISP::RunDemo(); // Interface Segregation Principle Demo
            break;
        case 5:
            DIP::RunDemo(); // Dependency Inversion Principle Demo
            break;
        case 6:
            SGT::RunDemo(); // Singleton Demo
            break;
        case 7:
            FTM::RunDemo(); // Factory Method Demo
            break;
        case 8:
            AFT::RunDemo(); // Abstract Factory Demo
            break;
        case 9:
            PRO::RunDemo(); // Prototype Demo
            break;
        case 10:
            BLD::RunDemo(); // Builder Demo
            break;
        case 11:
            FAC::RunDemo(); // Facade Demo
            break;
        case 12:
            ADP::RunDemo(); // Adapter Demo
            break;
        case 13:
            PRX::RunDemo(); // Proxy Demo
            break;
        case 14:
            BRG::RunDemo(); // Bridge Demo
            break;
        case 15:
            COM::RunDemo(); // Composite Demo
            break;
        case 16:
            DEC::RunDemo(); // Decorator Demo
            break;
        case 17:
            FLY::RunDemo(); // Flyweight Demo
            break;
        case 18:
            NUL::RunDemo(); // Null Object Demo
            break;
        case 19:
            STR::RunDemo(); // Strategy Demo
            break;
        case 20:
            TMP::RunDemo(); // Template Method Demo
            break;
        case 21:
            ITR::RunDemo(); // Iterator Demo
            break;
        case 22:
            CMD::RunDemo(); // Command Demo
            break;
        case 23:
            OBS::RunDemo(); // Observer Demo
            break;
        case 0:
            IsRunning = false;
            break;
        }
    }
}