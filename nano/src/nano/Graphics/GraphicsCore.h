#pragma once
#include "nano/Core/BaseDefines.h"

namespace Nano
{
template <typename T>
struct GraphicsAPI
{
    GraphicsAPI() = default;
    ~GraphicsAPI() = default;
    GraphicsAPI(const GraphicsAPI<T> &) = delete;
    GraphicsAPI<T> &operator=(const GraphicsAPI<T> &) = delete;
};

struct GraphicsCore : public GraphicsAPI<GraphicsCore>
{
    static GraphicsCore *Create();
    static void Destroy(GraphicsCore *graphicsCore);

    bool Init();
    void Destroy();
};
} // namespace Nano
