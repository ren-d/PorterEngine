#include "PorterRenderer.h"

namespace RendererGlobals {
	const UINT frameCount = 3;
}
void PorterRenderer::Initialize(HWND hwnd)
{
	m_hwnd = hwnd;
	SetupPipeline();
}

void PorterRenderer::SetupPipeline()
{

#if DEBUG
	EnableDebugLayer();
#endif
	SetupDevices();
	CreateCommandQueue();
	CreateSwapChain();
	CreateCommandAllocator();





}

void PorterRenderer::EnableDebugLayer()
{
	// Enable Debug Layer
	ComPtr<ID3D12Debug> debugController;

	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}
}

void PorterRenderer::SetupDevices()
{
	// Init Factory 

	UINT factoryFlags = 0;

	CreateDXGIFactory2(factoryFlags, IID_PPV_ARGS(&factory));

	// Find Device 

	// TODO: clean this up

	ComPtr<IDXGIAdapter1> adapter;
	ComPtr<IDXGIAdapter1> adapter1;
	for (
		UINT adapterIndex = 0;
		SUCCEEDED(factory->EnumAdapterByGpuPreference(
			adapterIndex,
			DXGI_GPU_PREFERENCE_UNSPECIFIED,
			IID_PPV_ARGS(&adapter1)));
		++adapterIndex)
	{
		if (adapter1)
		{
			DXGI_ADAPTER_DESC desc;
			adapter1->GetDesc(&desc);
			adapter = adapter1.Detach();
			break;
		}

	}

	SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device)));

}

void PorterRenderer::CreateCommandQueue()
{
	// Create Command Queue

	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue));
}

void PorterRenderer::CreateSwapChain()
{
	// Create Swap Chain

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.BufferCount = RendererGlobals::frameCount;
	swapChainDesc.Width = 1366;
	swapChainDesc.Height = 768;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.SampleDesc.Count = 1;

	ComPtr<IDXGISwapChain1> swapChain;
	factory->CreateSwapChainForHwnd(m_commandQueue.Get(), m_hwnd, &swapChainDesc, nullptr, nullptr, &swapChain);

	factory->MakeWindowAssociation(m_hwnd, DXGI_MWA_NO_ALT_ENTER);

	swapChain.As(&m_swapChain);
	m_currentFrame = m_swapChain->GetCurrentBackBufferIndex();

	CreateDescriptorHeaps();

	// Create Frame Resources
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());
	ComPtr<ID3D12Resource> renderTargets[RendererGlobals::frameCount];

	for (UINT i = 0; i < RendererGlobals::frameCount; ++i)
	{
		m_swapChain->GetBuffer(m_currentFrame, IID_PPV_ARGS(&renderTargets[i]));
		m_device->CreateRenderTargetView(renderTargets[i].Get(), nullptr, rtvHandle);
		rtvHandle.Offset(1, m_rtvHandleIncrementSize);
	}

}

void PorterRenderer::CreateDescriptorHeaps()
{

	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc = {};
	descriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	descriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	descriptorHeapDesc.NumDescriptors = RendererGlobals::frameCount;

	m_device->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&m_rtvHeap));

	m_rtvHandleIncrementSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
}

void PorterRenderer::CreateCommandAllocator()
{
	m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator));
}