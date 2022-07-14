namespace UE4
{
	class UGameInstance
	{
	private:
	public:
		__forceinline ULocalPlayer* LocalPlayers()
		{
			if (!this) return nullptr;

			uintptr_t LocalPlayerArray = *(uintptr_t*)(this + 0x38); if (!LocalPlayerArray) return nullptr;

			return *(ULocalPlayer**)(LocalPlayerArray);
		}
	};
}