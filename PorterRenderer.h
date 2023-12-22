#pragma once

#include "stdafx.h"

class PorterRenderer
{
public:
	PorterRenderer() {};
	void Init(HWND hwnd);

private:
	UINT m_currentFrame;
	ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
	ComPtr<ID3D12Device> m_device;
	ComPtr<IDXGISwapChain3> m_swapChain;
	ComPtr<ID3D12CommandQueue> m_commandQueue;
};