#pragma once

// todo: https://www.unknowncheats.me/forum/1829808-post14.html (rewrite so it's not half sdk/half trash)
class CInput
{
private:
	void* pvftable;
public:
	bool m_pad_something;
	bool m_fMouseInitialized;
	bool m_fMouseActive;
	bool pad_something01;
	char pad_0x08[0x2C];
	void* m_keys;
	char pad_0x38[0x64];
	__int32 pad_0x41;
	__int32 pad_0x42;

	// Is the 3rd person camera using the mouse?
	bool		m_fCameraInterceptingMouse;
	// Are we in 3rd person view?
	bool		m_fCameraInThirdPerson;
	// Should we move view along with mouse?
	bool		m_fCameraMovingWithMouse;


	// Is the camera in distance moving mode?
	bool		m_fCameraDistanceMove;
	// Old and current mouse position readings.
	int			m_nCameraOldX;
	int			m_nCameraOldY;
	int			m_nCameraX;
	int			m_nCameraY;

	// orthographic camera settings
	bool		m_CameraIsOrthographic;

	Angle		m_angPreviousViewAngles;

	float		m_flLastForwardMove;

	float m_flPreviousJoystickForward;
	float m_flPreviousJoystickSide;
	float m_flPreviousJoystickPitch;
	float m_flPreviousJoystickYaw;

	class CVerifiedUserCmd
	{
	public:
		CUserCmd			m_cmd;
		unsigned long		m_crc;
	};

	CUserCmd	*m_pCommands;
	CVerifiedUserCmd *m_pVerifiedCommands;
};
