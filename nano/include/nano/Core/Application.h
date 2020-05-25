#pragma once
#include "nano/Core/BaseDefines.h"

namespace Nano
{
struct GraphicsCore;
class NANO_API Application
{
  public:
    Application();
    ~Application();

    void OnInit();
    void OnDestroy();
    void OnUpdate();
    void OnRender();

  private:
    GraphicsCore *_GraphicsCore = nullptr;
};
} // namespace Nano
