#include "Graphics/D3D12/GraphicsCore.h"
#include <stdint.h>
#include <stdio.h>
#define MY_IID_PPV_ARGS IID_PPV_ARGS
#define D3D12_GPU_VIRTUAL_ADDRESS_NULL ((D3D12_GPU_VIRTUAL_ADDRESS)0)
#define D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN ((D3D12_GPU_VIRTUAL_ADDRESS)-1)
#include <wrl.h>
#if defined(NTDDI_WIN10_RS2) && (NTDDI_VERSION >= NTDDI_WIN10_RS2)
#include <dxgi1_6.h>
#else
#include <dxgi1_4.h> // For WARP
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x) \
    if (x != nullptr)   \
    {                   \
        x->Release();   \
        x = nullptr;    \
    }
#endif
#define ASSERT(isTrue, ...) (void)(isTrue)
#define WARN_ONCE_IF(isTrue, ...) (void)(isTrue)
#define WARN_ONCE_IF_NOT(isTrue, ...) (void)(isTrue)
#define DEBUGPRINT(msg, ...) \
    do                       \
    {                        \
    } while (0)
#define ASSERT_SUCCEEDED(hr, ...) (void)(hr)

namespace Nano
{
GraphicsCoreD3D12::GraphicsCoreD3D12()
{
}

GraphicsCoreD3D12::~GraphicsCoreD3D12()
{
}

bool GraphicsCoreD3D12::Init()
{
    Microsoft::WRL::ComPtr<ID3D12Device> pDevice;
#if _DEBUG
    Microsoft::WRL::ComPtr<ID3D12Debug> debugInterface;
    if (SUCCEEDED(D3D12GetDebugInterface(MY_IID_PPV_ARGS(&debugInterface))))
        debugInterface->EnableDebugLayer();
    else
        printf("WARNING:  Unable to enable D3D12 debug validation layer\n");
#endif

    Microsoft::WRL::ComPtr<IDXGIFactory4> dxgiFactory;
    ASSERT_SUCCEEDED(CreateDXGIFactory2(0, MY_IID_PPV_ARGS(&dxgiFactory)));

    Microsoft::WRL::ComPtr<IDXGIAdapter1> pAdapter;

    constexpr bool bUseWarpDriver = false;

    if (!bUseWarpDriver)
    {
        SIZE_T MaxSize = 0;

        for (uint32_t Idx = 0; DXGI_ERROR_NOT_FOUND != dxgiFactory->EnumAdapters1(Idx, &pAdapter); ++Idx)
        {
            DXGI_ADAPTER_DESC1 desc;
            pAdapter->GetDesc1(&desc);
            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
                continue;

            if (desc.DedicatedVideoMemory > MaxSize && SUCCEEDED(D3D12CreateDevice(pAdapter.Get(), D3D_FEATURE_LEVEL_11_0, MY_IID_PPV_ARGS(&pDevice))))
            {
                pAdapter->GetDesc1(&desc);
                //printf("D3D12-capable hardware found:  %s (%u MB)\n", desc.Description, desc.DedicatedVideoMemory >> 20);
                MaxSize = desc.DedicatedVideoMemory;
            }
        }

        if (MaxSize > 0)
            _Device = pDevice.Detach();
    }

    if (_Device == nullptr)
    {
        if (bUseWarpDriver)
            printf("WARP software adapter requested.  Initializing...\n");
        else
            printf("Failed to find a hardware adapter.  Falling back to WARP.\n");
        ASSERT_SUCCEEDED(dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&pAdapter)));
        ASSERT_SUCCEEDED(D3D12CreateDevice(pAdapter.Get(), D3D_FEATURE_LEVEL_11_0, MY_IID_PPV_ARGS(&pDevice)));
        _Device = pDevice.Detach();
    }
#ifndef RELEASE
    else
    {
        bool DeveloperModeEnabled = false;

        // Look in the Windows Registry to determine if Developer Mode is enabled
        HKEY hKey;
        LSTATUS result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\AppModelUnlock"), 0, KEY_READ, &hKey);
        if (result == ERROR_SUCCESS)
        {
            DWORD keyValue, keySize = sizeof(DWORD);
            result = RegQueryValueEx(hKey, TEXT("AllowDevelopmentWithoutDevLicense"), 0, NULL, (byte *)&keyValue, &keySize);
            if (result == ERROR_SUCCESS && keyValue == 1)
                DeveloperModeEnabled = true;
            RegCloseKey(hKey);
        }

        WARN_ONCE_IF_NOT(DeveloperModeEnabled, "Enable Developer Mode on Windows 10 to get consistent profiling results");

        // Prevent the GPU from overclocking or underclocking to get consistent timings
        if (DeveloperModeEnabled)
            _Device->SetStablePowerState(TRUE);
    }
#endif

#if _DEBUG
    ID3D12InfoQueue *pInfoQueue = nullptr;
    if (SUCCEEDED(_Device->QueryInterface(MY_IID_PPV_ARGS(&pInfoQueue))))
    {
        // Suppress whole categories of messages
        //D3D12_MESSAGE_CATEGORY Categories[] = {};

        // Suppress messages based on their severity level
        D3D12_MESSAGE_SEVERITY Severities[] =
            {
                D3D12_MESSAGE_SEVERITY_INFO};

        // Suppress individual messages by their ID
        D3D12_MESSAGE_ID DenyIds[] =
            {
                // This occurs when there are uninitialized descriptors in a descriptor table, even when a
                // shader does not access the missing descriptors.  I find this is common when switching
                // shader permutations and not wanting to change much code to reorder resources.
                D3D12_MESSAGE_ID_INVALID_DESCRIPTOR_HANDLE,

                // Triggered when a shader does not export all color components of a render target, such as
                // when only writing RGB to an R10G10B10A2 buffer, ignoring alpha.
                D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_PS_OUTPUT_RT_OUTPUT_MISMATCH,

                // This occurs when a descriptor table is unbound even when a shader does not access the missing
                // descriptors.  This is common with a root signature shared between disparate shaders that
                // don't all need the same types of resources.
                D3D12_MESSAGE_ID_COMMAND_LIST_DESCRIPTOR_TABLE_NOT_SET,

                // RESOURCE_BARRIER_DUPLICATE_SUBRESOURCE_TRANSITIONS
                (D3D12_MESSAGE_ID)1008,
            };

        D3D12_INFO_QUEUE_FILTER NewFilter = {};
        //NewFilter.DenyList.NumCategories = _countof(Categories);
        //NewFilter.DenyList.pCategoryList = Categories;
        NewFilter.DenyList.NumSeverities = _countof(Severities);
        NewFilter.DenyList.pSeverityList = Severities;
        NewFilter.DenyList.NumIDs = _countof(DenyIds);
        NewFilter.DenyList.pIDList = DenyIds;

        pInfoQueue->PushStorageFilter(&NewFilter);
        pInfoQueue->Release();
    }
#endif

    // We like to do read-modify-write operations on UAVs during post processing.  To support that, we
    // need to either have the hardware do typed UAV loads of R11G11B10_FLOAT or we need to manually
    // decode an R32_UINT representation of the same buffer.  This code determines if we get the hardware
    // load support.
    D3D12_FEATURE_DATA_D3D12_OPTIONS FeatureData = {};
    if (SUCCEEDED(_Device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS, &FeatureData, sizeof(FeatureData))))
    {
        if (FeatureData.TypedUAVLoadAdditionalFormats)
        {
            D3D12_FEATURE_DATA_FORMAT_SUPPORT Support =
                {
                    DXGI_FORMAT_R11G11B10_FLOAT, D3D12_FORMAT_SUPPORT1_NONE, D3D12_FORMAT_SUPPORT2_NONE};

            if (SUCCEEDED(_Device->CheckFeatureSupport(D3D12_FEATURE_FORMAT_SUPPORT, &Support, sizeof(Support))) &&
                (Support.Support2 & D3D12_FORMAT_SUPPORT2_UAV_TYPED_LOAD) != 0)
            {
                bool g_bTypedUAVLoadSupport_R11G11B10_FLOAT = true;
            }

            Support.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;

            if (SUCCEEDED(_Device->CheckFeatureSupport(D3D12_FEATURE_FORMAT_SUPPORT, &Support, sizeof(Support))) &&
                (Support.Support2 & D3D12_FORMAT_SUPPORT2_UAV_TYPED_LOAD) != 0)
            {
                bool g_bTypedUAVLoadSupport_R16G16B16A16_FLOAT = true;
            }
        }
    }

    return _Device != nullptr;
}

void GraphicsCoreD3D12::Destroy()
{
#if defined(_DEBUG)
    ID3D12DebugDevice *debugInterface;
    if (SUCCEEDED(_Device->QueryInterface(&debugInterface)))
    {
        debugInterface->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL);
        debugInterface->Release();
    }
#endif

    SAFE_RELEASE(_Device);
}

//! Graphics Core Implement
GraphicsCore *GraphicsCore::Create()
{
    return new GraphicsCoreD3D12;
}

void GraphicsCore::Destroy(GraphicsCore *graphicsCore)
{
    GraphicsCoreD3D12 *impl = reinterpret_cast<GraphicsCoreD3D12 *>(graphicsCore);
    delete impl;
}

bool GraphicsCore::Init()
{
    GraphicsCoreD3D12 *impl = reinterpret_cast<GraphicsCoreD3D12 *>(this);
    return impl->Init();
}

void GraphicsCore::Destroy()
{
    GraphicsCoreD3D12 *impl = reinterpret_cast<GraphicsCoreD3D12 *>(this);
    impl->Destroy();
}
} // namespace Nano
