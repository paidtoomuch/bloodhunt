#pragma once
#include <random>
#include <client/module/utilities.h>
#include <client/module/math.h>
#include <client/d3d/render.h>
#include <client/d3d/gui/gui.h>

#include <client/sdk/APawn.cpp>
#include <client/sdk/APlayerController.cpp>
#include <client/sdk/ULocalPlayer.cpp>
#include <client/sdk/ULevel.cpp>
#include <client/sdk/UGameInstance.cpp>
#include <client/sdk/UWorld.cpp>

UE4::UWorld* World;

struct ItemInfo
{
public:
	UE4::APawn* Pawn;
	Render::Color Rarity;
	Vector2 BoxSize;
	std::wstring Name;
};

namespace UE4 // post-lib
{
	ItemInfo ItemCache(ItemInfo Information)
	{
		uint64_t ItemType = *(uint64_t*)(Information.Pawn + 0x240);
		if (ItemType)
		{
			uint8_t Inventory_ItemType = *(uint8_t*)(ItemType + 0x40);
			if (Inventory_ItemType)
			{

				Information.BoxSize = Vector2(10, 5);


				uint8_t ItemRarity = *(uint8_t*)(ItemType + 0x41);

				Information.Rarity = Render::Color(255, 255, 255, 255);

				switch (ItemRarity)
				{
				case 0:
					Information.Rarity = Render::Color(255, 255, 255, 255);
					break;
				case 1:
					Information.Rarity = Render::Color(123, 244, 78, 255);
					break;
				case 2:
					Information.Rarity = Render::Color(69, 182, 254, 255);
					break;
				case 3:
					Information.Rarity = Render::Color(148, 118, 152, 255);
					break;
				case 4:
					Information.Rarity = Render::Color(246, 220, 53, 255);
					break;

				}
			}

			DWORD_PTR WeaponName = *(DWORD_PTR*)(ItemType + 0x48);
			if (WeaponName)
			{
				uintptr_t Ftextptr = *(uintptr_t*)(WeaponName + 0x28);

				Information.Name = UE4::ReadUnicode(Ftextptr);
			}
		}

		return Information;
	}

	enum EStereoscopicPass;

	static bool WorldToScreen(Vector3 WorldPos, Vector3* vScreenPos)
	{
		if (!World->OwningGameInstance()->LocalPlayers()->PendingLevelPlayerControllerClass())
			return false;

		static uintptr_t WorldToScreen_addr = 0;

		if (!WorldToScreen_addr)
		{
			WorldToScreen_addr = (uintptr_t)FindPattern(E("E8 ? ? ? ? 84 C0 75 08 84 DB 0F 85 ? ? ? ? 48 8B 4D D0 48 8D 45 68"));
			WorldToScreen_addr = RVA(WorldToScreen_addr, 5);

			if (!WorldToScreen_addr)
				return false;
		}

		auto WorldToScreenVT = reinterpret_cast<bool(__fastcall*)(uintptr_t pPlayerController, Vector3 vWorldPos, Vector3 * vScreenPosOut, char)>(WorldToScreen_addr);
		return WorldToScreenVT((uintptr_t)World->OwningGameInstance()->LocalPlayers()->PendingLevelPlayerControllerClass(), WorldPos, vScreenPos, (char)0);
	}

	bool IsBoundingAreaParameters(UE4::APawn* Pawn, PlayerRootingBoundaries::bounds_t* out_bounds)
	{
		// USceneComponent::ATigerApartmentArchetype::Min (0x230)
		// USceneComponent::ATigerApartmentArchetype::Max (0x238)

	}

	void DrawBone(Vector3 from, Vector3 to, Render::Color color)
	{
		Vector3 W2S_from = from;
		Vector3 oW2S_from;
		if (!WorldToScreen(W2S_from, &oW2S_from)) return;
		Vector3  W2S_to = to;
		Vector3 oW2S_to;


		if (!WorldToScreen(W2S_to, &oW2S_to)) return;
		Render::Line(Vector2(oW2S_from.x, oW2S_from.y), Vector2(oW2S_to.x, oW2S_to.y), color);
	}

	void DrawSkeleton(uintptr_t pEnemySoldier, Render::Color color)
	{
		DrawBone(UBone::GetBoneWithRotation(pEnemySoldier, PlayerRootingBoundaries::ID::Head), UBone::GetBoneWithRotation(pEnemySoldier, PlayerRootingBoundaries::ID::Neck), color);
		DrawBone(UBone::GetBoneWithRotation(pEnemySoldier, PlayerRootingBoundaries::ID::Neck), UBone::GetBoneWithRotation(pEnemySoldier, PlayerRootingBoundaries::ID::Chest), color);
		DrawBone(UBone::GetBoneWithRotation(pEnemySoldier, PlayerRootingBoundaries::ID::Chest), UBone::GetBoneWithRotation(pEnemySoldier, PlayerRootingBoundaries::ID::Waist), color);
		DrawBone(UBone::GetBoneWithRotation(pEnemySoldier, PlayerRootingBoundaries::ID::Waist), UBone::GetBoneWithRotation(pEnemySoldier, PlayerRootingBoundaries::ID::Pelvis), color);

		DrawBone(UBone::GetBoneWithRotation(pEnemySoldier, PlayerRootingBoundaries::ID::Neck), UBone::GetBoneWithRotation(pEnemySoldier, PlayerRootingBoundaries::ID::Leftshoulder), color);
		DrawBone(UBone::GetBoneWithRotation(pEnemySoldier, PlayerRootingBoundaries::ID::Leftshoulder), UBone::GetBoneWithRotation(pEnemySoldier, PlayerRootingBoundaries::ID::Leftelbow), color);
		DrawBone(UBone::GetBoneWithRotation(pEnemySoldier, PlayerRootingBoundaries::ID::Leftelbow), UBone::GetBoneWithRotation(pEnemySoldier, PlayerRootingBoundaries::ID::LeftHand), color);

		DrawBone(UBone::GetBoneWithRotation(pEnemySoldier, PlayerRootingBoundaries::ID::Neck), UBone::GetBoneWithRotation(pEnemySoldier, PlayerRootingBoundaries::ID::Rightshoulder), color);
		DrawBone(UBone::GetBoneWithRotation(pEnemySoldier, PlayerRootingBoundaries::ID::Rightshoulder), UBone::GetBoneWithRotation(pEnemySoldier, PlayerRootingBoundaries::ID::Rightelbow), color);
		DrawBone(UBone::GetBoneWithRotation(pEnemySoldier, PlayerRootingBoundaries::ID::Rightelbow), UBone::GetBoneWithRotation(pEnemySoldier, PlayerRootingBoundaries::ID::RightHand), color);

		DrawBone(UBone::GetBoneWithRotation(pEnemySoldier, PlayerRootingBoundaries::ID::Pelvis), UBone::GetBoneWithRotation(pEnemySoldier, PlayerRootingBoundaries::ID::LeftThighs), color);
		DrawBone(UBone::GetBoneWithRotation(pEnemySoldier, PlayerRootingBoundaries::ID::LeftThighs), UBone::GetBoneWithRotation(pEnemySoldier, PlayerRootingBoundaries::ID::Leftknees), color);
		DrawBone(UBone::GetBoneWithRotation(pEnemySoldier, PlayerRootingBoundaries::ID::Leftknees), UBone::GetBoneWithRotation(pEnemySoldier, PlayerRootingBoundaries::ID::Leftleg), color);
		//DrawBone(UBone::GetBoneWithRotation(pEnemySoldier, PlayerRootingBoundaries::ID::Leftleg), UBone::GetBoneWithRotation(pEnemySoldier, PlayerRootingBoundaries::ID::LeftFoot), color);

		DrawBone(UBone::GetBoneWithRotation(pEnemySoldier, PlayerRootingBoundaries::ID::Pelvis), UBone::GetBoneWithRotation(pEnemySoldier, PlayerRootingBoundaries::ID::RightThighs), color);
		DrawBone(UBone::GetBoneWithRotation(pEnemySoldier, PlayerRootingBoundaries::ID::RightThighs), UBone::GetBoneWithRotation(pEnemySoldier, PlayerRootingBoundaries::ID::Rightknees), color);
		DrawBone(UBone::GetBoneWithRotation(pEnemySoldier, PlayerRootingBoundaries::ID::Rightknees), UBone::GetBoneWithRotation(pEnemySoldier, PlayerRootingBoundaries::ID::Rightleg), color);
		//DrawBone(UBone::GetBoneWithRotation(pEnemySoldier, PlayerRootingBoundaries::ID::Rightleg), UBone::GetBoneWithRotation(pEnemySoldier, PlayerRootingBoundaries::ID::RightFoot), color);

	}

	float ScreenToEnemy(Vector3 position)
	{
		Vector3 out;
		if (WorldToScreen(position, &out))
		{
			return (fabs(out.x - (information::wnd_size.x / 2)) + fabs(out.y - (information::wnd_size.y / 2)));
		}
		return 0;
	}
}