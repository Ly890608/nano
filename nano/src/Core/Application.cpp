#include "nano/Core/Application.h"
#include "Core/Log.h"
#include "Graphics/GraphicsCore.h"

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
}

void Application::OnUpdate()
{
    Log::Flush();
}

void Application::OnRender()
{
}
} // namespace Nano
