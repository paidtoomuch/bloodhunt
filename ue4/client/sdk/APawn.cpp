#include <array>

namespace PlayerRootingBoundaries
{
	enum ID
	{
		Head = 28,
		Neck = 27,
		Chest = 158, // 108
		Waist = 25,
		Pelvis = 24,
		Leftshoulder = 99,
		Leftelbow = 65,
		LeftHand = 92,
		Rightshoulder = 144,
		Rightelbow = 110,
		RightHand = 111,
		LeftThighs = 16,
		Leftknees = 11,
		Leftleg = 191,
		LeftFoot = 13,
		RightThighs = 23,
		Rightknees = 18,
		Rightleg = 192,
		RightFoot = 20
	};

	struct bounds_t
	{
		float left, right, top, bottom;
	};

	struct bone_t
	{
		Vector3    screen;
		int index;
		bool on_screen;
	};

	std::array<bone_t, 23> bones = {
	bone_t{ Vector3{}, ID::Head, false}, bone_t{Vector3{}, ID::Neck, false},
	bone_t{ Vector3{}, ID::Waist, false },  bone_t{ Vector3{}, ID::Waist, false },
	bone_t{ Vector3{}, ID::Chest, false },  bone_t{ Vector3{}, ID::Pelvis, false },
	bone_t{ Vector3{}, ID::LeftFoot, false },  bone_t{ Vector3{}, ID::LeftFoot, false },
	bone_t{ Vector3{}, ID::Leftknees, false },  bone_t{ Vector3{}, ID::LeftThighs, false },
	bone_t{ Vector3{}, ID::LeftThighs, false },  bone_t{ Vector3{}, ID::Pelvis, false },
	bone_t{ Vector3{}, ID::RightThighs, false },  bone_t{ Vector3{}, ID::RightThighs, false },
	bone_t{ Vector3{}, ID::Rightknees, false },  bone_t{ Vector3{}, ID::RightFoot, false },
	bone_t{ Vector3{}, ID::RightFoot, false },  bone_t{ Vector3{}, ID::LeftHand, false },
	bone_t{ Vector3{}, ID::Leftelbow, false },  bone_t{ Vector3{}, ID::Leftshoulder, false },
	bone_t{ Vector3{}, ID::Rightelbow, false }, bone_t{ Vector3{}, ID::Rightshoulder, false },
	bone_t{ Vector3{}, ID::RightHand, false }
	};
}


namespace UE4
{
	wchar_t* ItemType(int Inventory_ItemType)
	{
		wchar_t* S_ItemType;

		if (Inventory_ItemType == 0)
			S_ItemType = FString::Convert("Gun");

		if (Inventory_ItemType == 1)
			S_ItemType = FString::Convert("Melee");

		if (Inventory_ItemType == 2)
			S_ItemType = FString::Convert("Unarmed");

		if (Inventory_ItemType == 3)
			S_ItemType = FString::Convert("Outfit");

		if (Inventory_ItemType == 4)
			S_ItemType = FString::Convert("Amulet");

		if (Inventory_ItemType == 5)
			S_ItemType = FString::Convert("Artifact");

		if (Inventory_ItemType == 6)
			S_ItemType = FString::Convert("Body Armor");

		if (Inventory_ItemType == 7)
			S_ItemType = FString::Convert("Relic");

		if (Inventory_ItemType == 8)
			S_ItemType = FString::Convert("Ammo");

		if (Inventory_ItemType == 9)
			S_ItemType = FString::Convert("Consumable");

		if (Inventory_ItemType == 10)
			S_ItemType = FString::Convert("Mod");

		if (Inventory_ItemType == 11)
			S_ItemType = FString::Convert("Collectible");

		return S_ItemType;
	}


	class APawn
	{
	private:
		enum ETigerOutlineMode : int
		{
			HeightenedSenses = 0,
			Lure = 1,
			EnemyPlayer = 2,
			ScoutingFamiliarsGroupMember = 3,
			ScoutingFamiliars = 4,
			BloodHuntedHSReveal = 5,
			BloodHunted = 6,
			CapturingHaven = 7,
			CharmingUs = 8,
			GroupMember = 9,
			LocalPlayer = 10,
			Count = 11,
			None = 12,
			ETigerOutlineMode_MAX = 13,
		};
	public:
		__forceinline int ID()
		{
			if (!this) return 0;

			return *(int*)(this + 0x18);
		}

		__forceinline uint64_t Mesh()
		{
			if (!this) return 0;

			return *(uint64_t*)(this + 0x288);
		}

		__forceinline Vector3 Root()
		{
			if (!this) return Vector3();

			auto RootPosition = *(DWORD_PTR*)(this + 0x138);

			return *(Vector3*)(RootPosition + 0x11c);
		}

		__forceinline uint64_t State()
		{
			if (!this) return 0;

			return *(uint64_t*)(this + 0x248);
		}

		__forceinline uint8_t Mode()
		{
			if (!this) return 0;

			auto OutlineComponent = *(uintptr_t*)(this + 0x4f0); 
			if (!OutlineComponent) return 0;

			return *(uint8_t*)(OutlineComponent + 0xb0);
		}

		__forceinline std::wstring Username()
		{
			if (!this)
				return E(L"Failed Pawn");

			uintptr_t State = this->State();

			if (!State)
				return E(L"disconnected lmao");


			DWORD_PTR PlayerName = *(DWORD_PTR*)(State + 0x308);
			if (!PlayerName)
				return E(L"Bot");

			std::wstring Name = ReadUnicode(PlayerName);

			return Name;

		}

		__forceinline bool IsDowned()
		{
			if (!this) return 0;

			return *(bool*)(this + 0xe70);
		}

		__forceinline bool IsTeam()
		{
			if (!this) return 0;

			auto OutlineComponent = *(uintptr_t*)(this + 0x4f0);
			if (!OutlineComponent) return 0;

			auto CurrentMode = *(int*)(OutlineComponent + 0xb0);
			if (!CurrentMode) return 0;

			if (CurrentMode != 9)
				return false;

			return true;
		}

		__forceinline float Health()
		{
			if (!this) return 0;

			return *(float*)(this + 0x698);
		}

		__forceinline float MaxHealth()
		{
			if (!this) return 0 ;

			return *(float*)(this + 0x58C);
		}

		__forceinline uintptr_t RangedWeaponComponent()
		{
			if (!this) return 0;

			return *(float*)(this + 0x4c8);

		}
	};
}