#include "nano/Core/Application.h"
#include "nano/Core/Log.h"
#include "nano/Graphics/GraphicsCore.h"

namespace Nano
{
Application::Application()
{
}

Application::~Application()
{
}

void Application::OnInit()
{
    NANO_INFO("Application::OnInit\n");
    _GraphicsCore = GraphicsCore::Create();
    _GraphicsCore->Init();
}

void Application::OnDestroy()
{
    _GraphicsCore->Destroy();
    GraphicsCore::Destroy(_GraphicsCore);
    NANO_INFO("Application::OnDestroy\n");
}

void Application::OnUpdate()
{
    Log::Flush();
}

void Application::OnRender()
{
}
} // namespace Nano
