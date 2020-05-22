#include "nano/Core/Application.h"
#include <iostream>

namespace Nano
{
Application::Application()
{
    std::cout << "Application::Application" << std::endl;
}

Application::~Application()
{
    std::cout << "Application::~Application" << std::endl;
}

void Application::OnInit()
{
    std::cout << "Application::OnInit" << std::endl;
}

void Application::OnDestroy()
{
    std::cout << "Application::OnDestroy" << std::endl;
}

void Application::OnUpdate()
{
    std::cout << "Application::OnUpdate" << std::endl;
}

void Application::OnRender()
{
    std::cout << "Application::OnRender" << std::endl;
}
} // namespace Nano
