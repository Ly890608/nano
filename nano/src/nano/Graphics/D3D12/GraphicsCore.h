#pragma once
#include "nano/Graphics/GraphicsCore.h"
#include <d3d12.h>

namespace Nano
{
class GraphicsCoreD3D12 final : public GraphicsCore
{
  public:
    GraphicsCoreD3D12();
    ~GraphicsCoreD3D12();

    bool Init();
    void Destroy();
};
} // namespace Nano
