namespace UE4
{
	class UWorld
	{
	private:
	public:
		__forceinline UGameInstance* OwningGameInstance()
		{
			if (!this) return nullptr;

			return *( UGameInstance** )( this + 0x180 );
		}

		__forceinline ULevel* PersistentLevel()
		{
			if (!this) return nullptr;

			return *(ULevel**)(this + 0x30);
		}
	};
}