namespace UE4
{
	class APlayerController
	{
	private:
	public:
		__forceinline APawn* AcknowledgedPawn()
		{
			if (!this) return nullptr;

			return *(APawn**)(this + 0x2a8);
		}

		__forceinline UE4::FCameraCacheEntry APlayerCameraManager()
		{
			if (!this) return UE4::FCameraCacheEntry();

			auto CameraManager = *(uintptr_t*)(this + 0x2c0); // PlayerCameraManager
			if (!CameraManager)
				return UE4::FCameraCacheEntry();

			return *(UE4::FCameraCacheEntry*)(CameraManager + 0x1ab0); // CameraCachePrivate
		}
	};
}