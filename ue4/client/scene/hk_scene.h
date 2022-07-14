#pragma once
#include <client/includes.hpp>
#include <string>
#include <array>
#include <vector>
#include <cwchar>
#include <client/hook/MinHook.h>

#pragma comment(lib, "vcruntime.lib")
#pragma comment(lib, "ucrt.lib")
#pragma comment(lib, "d3d11.lib")

uintptr_t Base;

Vector3 OriginalLocation;
Vector3 OriginalRotation;

Vector3 AimAngles;

#define LP ( World->OwningGameInstance()->LocalPlayers()->PendingLevelPlayerControllerClass() )

UE4::APawn* Target;
UE4::APawn* LocalPlayer;
UE4::APawn* ClosestPlayer;

void** VTable;
uint64_t GetViewPointAddress;

Vector3 AimbotPrediction(float bulletVelocity, float bulletGravity, float targetDistance, Vector3 targetPosition, Vector3 targetVelocity) {
	Vector3 recalculated = targetPosition;
	float gravity = fabs(bulletGravity);
	float time = targetDistance / fabs(bulletVelocity);
	float bulletDrop = (gravity / 250) * time * time;
	recalculated.z += bulletDrop * 120;
	recalculated.x += time * (targetVelocity.x);
	recalculated.y += time * (targetVelocity.y);
	recalculated.z += time * (targetVelocity.z);
	return recalculated;
}

namespace software
{
	void(*GetViewPoint)(UE4::APawn*, UE4::FMinimalViewInfo*, UE4::EStereoscopicPass) = nullptr;
	void(*GetPlayerViewPoint)(UE4::APlayerController*, Vector3*, Vector3*) = nullptr;
	void GetPlayerViewPointHook(UE4::APlayerController* this_PlayerController, Vector3* Location, Vector3* Rotation);

	static bool hooked_GetPlayerViewPoint = false;

	void hookGetPlayerViewPoint()
	{
		uintptr_t PlayerController = (uintptr_t)World->OwningGameInstance()->LocalPlayers()->PendingLevelPlayerControllerClass();

		if (!PlayerController) return;
	
		VTable = *reinterpret_cast<void***>(PlayerController);
		if (!VTable) return;
	
		if (MH_CreateHook(VTable[0xE2], &GetPlayerViewPointHook, reinterpret_cast<LPVOID*>(&GetPlayerViewPoint)) != MH_STATUS::MH_OK)
			MessageBoxA(0, "Failed to Create Hook", 0, 0);

		if (MH_EnableHook(VTable[0xE2]) != MH_STATUS::MH_OK)
			MessageBoxA(0, "Failed to enable hook", 0, 0);


		hooked_GetPlayerViewPoint = true;
	}

	void function()
	{
		if (!Base)
			Base = (uintptr_t)GetModuleBase(NULL);

		World = *(UE4::UWorld**)(Base + 0x573F680);

		if (!World)
			return;

		if (!World->OwningGameInstance()->LocalPlayers()->PendingLevelPlayerControllerClass()->AcknowledgedPawn())
			return;

		if (!hooked_GetPlayerViewPoint)
			hookGetPlayerViewPoint();

		int ActorsCount = World->PersistentLevel()->MaxPacket();

		for (int i = 0; i < ActorsCount; i++)
		{
			UE4::APawn* Pawn = World->PersistentLevel()->Instigator(i);
			if (!Pawn)
				continue;

			const char* ObjectNameA = UE4::GetObjectFromFName(Pawn->ID(), Base);
			wchar_t* ObjectNameW = FString::Convert(ObjectNameA);

			if (Module::StrCmp(E("TBP_ElysiumPlayer_C"), ObjectNameA, false) || Module::StrCmp(E("TBP_Player_C"), ObjectNameA, false))
			{
				Vector3 HeadPosition;

				Render::Color Box = Render::Color(255, 255, 255, 200);
				Render::Color Skeleton = Render::Color(255, 255, 255, 200);

				if (Pawn->Health() < 1 || Pawn->IsTeam())
					continue;

				if (Pawn->IsTeam())
					ObjectNameW = FString::Convert(E("Team"));

				if (Pawn->IsDowned())
					Skeleton = Render::Color(255, 0, 0, 200);

				if (Pawn == Target)
				{
					Vector3 ScreenFeetPosition;
					if (UE4::WorldToScreen(UE4::UBone::GetBoneWithRotation(LP->AcknowledgedPawn()->Mesh(), PlayerRootingBoundaries::ID::RightHand), &ScreenFeetPosition))
					{
						if (UE4::WorldToScreen(UE4::UBone::GetBoneWithRotation(Pawn->Mesh(), 28), &HeadPosition))
						{
							Render::Line(Vector2(ScreenFeetPosition.x, ScreenFeetPosition.y), Vector2(HeadPosition.x, HeadPosition.y), Render::Color(255, 246, 0, 190));
						}
					}
				}

				if (ObjectNameW == nullptr)
					ObjectNameW = FString::Convert(E("Bot"));


				if (LP->AcknowledgedPawn() != Pawn)
				{
					int Distance = (int)Math::GameDist(World->OwningGameInstance()->LocalPlayers()->PendingLevelPlayerControllerClass()->AcknowledgedPawn()->Root(), Pawn->Root());

					std::wstring DistanceW = Pawn->Username() + std::wstring(E(L" [")) + std::to_wstring(Distance) + std::wstring(E(L"m]"));

					if (UE4::WorldToScreen(UE4::UBone::GetBoneWithRotation(Pawn->Mesh(), 28), &HeadPosition))
						Render::String(Vector2(HeadPosition.x, HeadPosition.y + -14), DistanceW.c_str(), true, Render::Color(255, 255, 255, 200));

					if (!Pawn->IsDowned())
					{
						std::wstring cs;
						std::wstring nd1;

						nd1 = std::to_wstring((int)Pawn->Health());
						cs = std::wstring(E(L"HP: ")) + nd1;
						const wchar_t* str = cs.c_str();

						if (UE4::WorldToScreen(UE4::UBone::GetBoneWithRotation(Pawn->Mesh(), 28), &HeadPosition))
						{
							Render::String(Vector2(HeadPosition.x, HeadPosition.y + -25), str, true, Render::Color(255, 255, 255, 200));
						}


					}
					
				}

				UE4::DrawSkeleton(Pawn->Mesh(), Skeleton);
			}

			if (Module::StrCmp(E("TBP_Entity_C"), ObjectNameA, false))
			{
				Vector3 HeadPosition;
				PlayerRootingBoundaries::bounds_t o{};

				int Distance = (int)Math::GameDist(World->OwningGameInstance()->LocalPlayers()->PendingLevelPlayerControllerClass()->AcknowledgedPawn()->Root(), Pawn->Root());

				if (Distance < 150)
				{
					if (UE4::WorldToScreen(UE4::UBone::GetBoneWithRotation(Pawn->Mesh(), 28), &HeadPosition))
						Render::String(Vector2(HeadPosition.x, HeadPosition.y + -25), std::wstring(E(L"AI Robots")).c_str(), true, Render::Color(3, 119, 200, 200));
				}
			}

			if (Module::StrCmp(E("TBP_Player_C"), ObjectNameA, false))
			{
				if (LP->AcknowledgedPawn() == Pawn) // FTigerCharacterStatModification
				{
					*(float*)(LP->AcknowledgedPawn() + 0xb90 + 0x4c) = 15.f;//ReloadSpeedMultiplier
					*(float*)(LP->AcknowledgedPawn() + 0xb90 + 0x48) = 50.f;  //AttackSpeedMultiplier
					*(float*)(LP->AcknowledgedPawn() + 0xb90 + 0x5c) = 50.f;  //FeedSpeedMultiplier
					*(float*)(LP->AcknowledgedPawn() + 0xb90 + 0x58) = 50.f;  //AmmoCapacityMultiplier
					*(float*)(LP->AcknowledgedPawn() + 0xb90 + 0x54) = 50.f;  //AmmoFindMultiplier
					//*(float*)(LP->AcknowledgedPawn() + 0xb90 + 0x38 + 0x4) = 20.f;  //Health->FTigerCharacterSingleStatModification
					//*(float*)(LP->AcknowledgedPawn() + 0xb90 + 0x74) = 50.f;//ShootingDamageDealtMultiplier
					//*(float*)(LP->AcknowledgedPawn() + 0xb90 + 0x94) = 10.f;//ReviveSpeedMultiplier
				}
			}

			//if (Module::StrCmp(E("TBP_Item_C"), ObjectNameA, false))
			//{
			//	Vector3 ScreenPositon;
			//	auto Position = Pawn->Root();

			//	int Distance = Math::GameDist(LP->AcknowledgedPawn()->Root(), Position);

			//	if (Distance < 250)
			//	{

			//		if (UE4::WorldToScreen(Position, &ScreenPositon))
			//		{
			//			ItemInfo Information;
			//			Information.Pawn = Pawn;

			//			uint64_t ItemType = *(uint64_t*)(Information.Pawn + 0x240);
			//			DWORD_PTR WeaponName = *(DWORD_PTR*)(ItemType + 0x48);

			//			uintptr_t Ftextptr = *(uintptr_t*)(WeaponName + 0x28);

			//			if (Ftextptr)
			//			{
			//				Information.Name = UE4::ReadUnicode(Ftextptr);
			//			}

			//			Information.BoxSize = Vector2(10, 5);



			//			uint8_t ItemRarity = *(uint8_t*)(ItemType + 0x41);

			//			Information.Rarity = Render::Color(255, 255, 255, 255);

			//			switch (ItemRarity)
			//			{
			//			case 0:// common
			//				continue;
			//				Information.Rarity = Render::Color(255, 255, 255, 255);
			//				break;
			//			case 1:// uncommon
			//				continue;
			//				Information.Rarity = Render::Color(124, 252, 0, 255);
			//				break;
			//			case 2: // rare
			//				continue;
			//				Information.Rarity = Render::Color(69, 182, 254, 255);
			//				break; // epic
			//			case 3:
			//				Information.Rarity = Render::Color(188, 19, 254, 255);
			//				break;
			//			case 4:// legendary
			//				Information.Rarity = Render::Color(246, 220, 53, 255);
			//				break;

			//			}

			//			std::wstring DistanceW = Information.Name + std::wstring(E(L" [")) + std::to_wstring(Distance) + std::wstring(E(L"m]"));

			//			Render::Rectangle(Vector2(ScreenPositon.x, ScreenPositon.y), Vector2(Information.BoxSize.x, Information.BoxSize.y), Render::Color(255, 255, 255, 255));

			//			Render::String(Vector2(ScreenPositon.x, ScreenPositon.y - 14), DistanceW.c_str(), true, Information.Rarity);
			//		}
			//	}
			//}
		}

	}

	void aimbot()
	{
		if (!Base)
			Base = (uintptr_t)GetModuleBase(NULL);

		if (!World)
			return;

		float ofov = 500;
		float nfov = 0;

		Render::Circle(Vector2(information::wnd_size.x / 2.f, information::wnd_size.y / 2.f), Render::Color(255, 255, 255, 190), ofov);


		Target = 0;

		int ActorsCount = World->PersistentLevel()->MaxPacket();

		for (int i = 0; i < ActorsCount; i++)
		{
			UE4::APawn* Pawn = World->PersistentLevel()->Instigator(i);
			if (!Pawn)
				continue;

			const char* Objects = UE4::GetObjectFromFName(Pawn->ID(), Base);

			if (Module::StrCmp(E("TBP_Player_C"), Objects, false) || Module::StrCmp(E("TBP_ElysiumPlayer_C"), Objects, false))
			{

				if (Pawn->IsDowned() || Pawn == LP->AcknowledgedPawn() || Pawn->IsTeam())
					continue;

				auto Position = UE4::UBone::GetBoneWithRotation(Pawn->Mesh(), 28);

				nfov = UE4::ScreenToEnemy(Position);
				if (nfov == 0)
					continue;

				if (nfov < ofov)
				{
					ofov = nfov;
					//if (Pawn)
					//{
					//	// last "aimbot target" distance from player
					//	float fLastTargetDist = Math::GameDist(
					//		LP->AcknowledgedPawn()->Root(),
					//		Target->Root()
					//	);
					//	// new "aimbot target" distance from player		
					//	float fTargetDist = Math::GameDist(
					//		LP->AcknowledgedPawn()->Root(),
					//		Pawn->Root()
					//	);
					//	// set aimbot target to new target if closer than last target
					//	if (fTargetDist < fLastTargetDist)
					//		Target = Pawn;
					//}
					//else
						Target = Pawn;
				}
			}

			if (Module::StrCmp(E("TBP_Entity_C"), Objects, false))
			{
				auto Position = UE4::UBone::GetBoneWithRotation(Pawn->Mesh(), 28);

				nfov = UE4::ScreenToEnemy(Position);
				if (nfov == 0)
					continue;

				if (nfov < ofov)
				{
					ofov = nfov;
					//if (Pawn)
					//{
					//	// last "aimbot target" distance from player
					//	float fLastTargetDist = Math::GameDist(
					//		LP->AcknowledgedPawn()->Root(),
					//		Target->Root()
					//	);
					//	// new "aimbot target" distance from player		
					//	float fTargetDist = Math::GameDist(
					//		LP->AcknowledgedPawn()->Root(),
					//		Pawn->Root()
					//	);
					//	// set aimbot target to new target if closer than last target
					//	if (fTargetDist < fLastTargetDist)
					//		Target = Pawn;
					//}
					//else
						Target = Pawn;
				}
			}
		}

		if (!Target)
			return;

		float Distance = Math::GameDist(
					LP->AcknowledgedPawn()->Root(),
					Target->Root()
				);

		//Vector3 AimAngles;
		Vector3 HeadAngles = UE4::UBone::GetBoneWithRotation(Target->Mesh(), PlayerRootingBoundaries::ID::Head);

		auto CameraManager = *(uintptr_t*)(LP + 0x2c0); if (!CameraManager) return;
		auto CameraCache = *(UE4::FCameraCacheEntry*)(CameraManager + 0x1ab0);
		auto RootComponent = *(DWORD_PTR*)(LP->AcknowledgedPawn() + 0x138);
		if (!RootComponent)
			return;

		Vector3 vellocity = *(Vector3*)(RootComponent + 0x140);
		//AimAngles = Math::CalcAngle(CameraCache.POV.Location, HeadAngles);
		Vector3 Predicted = AimbotPrediction(30000, -504, Distance, HeadAngles, vellocity);
		AimAngles = Math::CalcAngle(CameraCache.POV.Location, Predicted);
	}


	void GetViewPointHook(UE4::APawn* this_LocalPlayer, UE4::FMinimalViewInfo* OutViewInfo, UE4::EStereoscopicPass StereoPass)
	{
		GetViewPoint(this_LocalPlayer, OutViewInfo, StereoPass);

		OutViewInfo->FOV = OutViewInfo->FOV + 40;


		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			OutViewInfo->Location = OriginalLocation;
			OutViewInfo->Rotation = OriginalRotation;
		}
		
	}

	void GetPlayerViewPointHook(UE4::APlayerController* this_PlayerController, Vector3* Location, Vector3* Rotation)
	{
		GetPlayerViewPoint(this_PlayerController, Location, Rotation);

		OriginalLocation = *Location;
		OriginalRotation = *Rotation;

		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			if (Target) {
				if (!isnan(AimAngles.x) || !isnan(AimAngles.y))
				{
					*Rotation = AimAngles;
				}
			}
		}
	}
}


namespace d3d
{
	namespace scene
	{
		static IDXGISwapChain *swapchain;
	}

	typedef HRESULT(*present)(IDXGISwapChain*, UINT, UINT);
	inline present hk_org_present_scene{  };

	LRESULT CALLBACK Wnd_Proc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{

		if ( GUI::WndProc( hWnd, msg, wParam, lParam ) )
		{
			return true;
		}
		
		return Module( user32, CallWindowProcW, GUI::NextWndProc, hWnd, msg, wParam, lParam );
	}

	HRESULT hk_present_scene( IDXGISwapChain *a1, UINT a2, UINT a3 )
	{
		
		d3d::scene::swapchain = a1;

		if ( Render::NewFrame( d3d::scene::swapchain, information::wnd_size ) )
		{

			Render::String( { 20 + 1, 20 + 1 }, E( L"Bloodhunt 1.005" ), false, Render::Color( 255, 255, 255, 255 ), 0.6 );

			software::function( );
			software::aimbot( );

			Render::EndFrame( d3d::scene::swapchain );

		}

		return hk_org_present_scene( a1, a2, a3 );

	}
}