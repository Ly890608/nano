#include "Graphics/GraphicsCore.h"

namespace Nano
{
GraphicsCore *GraphicsCore::Create()
{
    return new GraphicsCore;
}

void GraphicsCore::Destroy(GraphicsCore *graphicsCore)
{
    delete graphicsCore;
}

bool GraphicsCore::Init()
{
    return true;
}

void GraphicsCore::Destroy()
{
}
} // namespace Nano
