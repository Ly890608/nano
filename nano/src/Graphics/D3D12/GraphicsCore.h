#pragma once
#include "Graphics/GraphicsCore.h"
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

  private:
    ID3D12Device *_Device = nullptr;
};
} // namespace Nano
