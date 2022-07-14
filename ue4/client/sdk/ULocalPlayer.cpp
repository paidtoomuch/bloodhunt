namespace UE4
{
	class ULocalPlayer
	{
	private:
	public:
		__forceinline APlayerController* PendingLevelPlayerControllerClass()
		{
			if (!this) return nullptr;

			return *(APlayerController**)( this + 0x30);
		}
	};
}