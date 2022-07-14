namespace UE4
{

	class ULevel
	{
	public:
		__forceinline APawn* Instigator(int idx)
		{
			if (!this) return nullptr;

			uintptr_t actor = *(uintptr_t*)(this + 0x98); 
			if (!actor) 
				return nullptr;

			uintptr_t pawn = *(uintptr_t*)(actor + idx * 0x8);
			if (pawn == 0x00 || !pawn)
				return nullptr;

			return (APawn*)(pawn);
		}

		__forceinline int32_t MaxPacket()
		{
			if (!this) return 0;

			return *(int32_t*)(this + 0xA0);
		}
	};
}