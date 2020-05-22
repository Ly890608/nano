#pragma once
#include "nano/Core/BaseDefines.h"

namespace Nano
{
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
};
} // namespace Nano
