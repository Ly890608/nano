#include "nano/Core/Application.h"
#include "Graphics/GraphicsCore.h"
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
    _GraphicsCore = GraphicsCore::Create();
    _GraphicsCore->Init();
}

void Application::OnDestroy()
{
    std::cout << "Application::OnDestroy" << std::endl;
    _GraphicsCore->Destroy();
    GraphicsCore::Destroy(_GraphicsCore);
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
