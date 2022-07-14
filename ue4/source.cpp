#include <client/scene/hk_scene.h>



auto DllMain( void *mod, int call_reason, void * ) -> bool
{
	if ( call_reason != 1 )
		return false;

	if (MH_Initialize() != MH_STATUS::MH_OK) return FALSE;

	Module::SetSpoofStub(FindPattern(E("48 83 C4 68 C3")), FindPattern(E("FF E3")));

	auto level = D3D_FEATURE_LEVEL_11_0;
	DXGI_SWAP_CHAIN_DESC sd;
	{
		ZeroMemory(&sd, sizeof sd);
		sd.BufferCount = 1;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = FindWindowA(E("UnrealWindow"), NULL);
		sd.SampleDesc.Count = 1;
		sd.Windowed = TRUE;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	}

	IDXGISwapChain* swap_chain = nullptr;
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* context = nullptr;

	D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, &level, 1, D3D11_SDK_VERSION, &sd, &swap_chain, &device, nullptr, &context);

	auto* swap_chainvtable = reinterpret_cast<uintptr_t*>(swap_chain);
	swap_chainvtable = reinterpret_cast<uintptr_t*>(swap_chainvtable[0]);

	DWORD old_protect;
	d3d::hk_org_present_scene = reinterpret_cast<d3d::present>(reinterpret_cast<DWORD_PTR*>(swap_chainvtable[8]));
	VirtualProtect(swap_chainvtable, 0x2B8, PAGE_EXECUTE_READWRITE, &old_protect);
	swap_chainvtable[8] = reinterpret_cast<DWORD_PTR>(d3d::hk_present_scene);
	VirtualProtect(swap_chainvtable, 0x2B8, old_protect, &old_protect);


	GetViewPointAddress = (uintptr_t)FindPattern(E("4C 8B DC 49 89 5B 20 55 56 57 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 24 ? ? ? ? 41 0F"));
	if (GetViewPointAddress)
	{
		if (MH_CreateHook(reinterpret_cast<LPVOID>(GetViewPointAddress), software::GetViewPointHook, reinterpret_cast<LPVOID*>(&software::GetViewPoint)) != MH_STATUS::MH_OK)
			return MessageBoxA(0, E("Failed to Create Hook"), 0, 0);

		if (MH_EnableHook(reinterpret_cast<LPVOID>(GetViewPointAddress)) != MH_STATUS::MH_OK)
			return MessageBoxA(0, E("Failed to enable hook"), 0, 0);


		//MessageBoxA(0, "GetViewPoint Hooked", 0, 0);
	}


	return true;	
}