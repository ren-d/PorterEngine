#pragma once

#include "stdafx.h"

class PorterRenderer
{
public:
	PorterRenderer() {};

	void Initialize(HWND hwnd);

private:
	void SetupPipeline();

	// EnableDebugLayer
	void EnableDebugLayer();

	// Create Device
	void SetupDevices();

	// Create Command Queue
	void CreateCommandQueue();

	// Create Swap Chain
	void CreateSwapChain();

	// Create Render Target View Descriptor Heap
	void CreateDescriptorHeaps();

	// Create Command allocator
	void CreateCommandAllocator();

	void SetupAssets();

	HWND m_hwnd;
	UINT m_rtvHandleIncrementSize;;
	UINT m_currentFrame;
	ComPtr<IDXGIFactory6> m_factory;
	ComPtr<ID3D12CommandAllocator> m_commandAllocator;
	ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
	ComPtr<ID3D12Device> m_device;
	ComPtr<IDXGISwapChain3> m_swapChain;
	ComPtr<ID3D12CommandQueue> m_commandQueue;
};