#pragma once

// this is actually CCSPlayerAnimState, which inherits CPlayerAnimState
class IPlayerAnimState
{
public:
	virtual void Release() = 0;

	// Update() and DoAnimationEvent() together maintain the entire player's animation state.
	//
	// Update() maintains the the lower body animation (the player's m_nSequence)
	// and the upper body overlay based on the player's velocity and look direction.
	//
	// It also modulates these based on events triggered by DoAnimationEvent.
	virtual void Update(float eyeYaw, float eyePitch) = 0;

	// This is called by the client when a new player enters the PVS to clear any events
	// the dormant version of the entity may have been playing.
	virtual void ClearAnimationState() = 0;

	// The client uses this to figure out what angles to render the entity with (since as the guy turns,
	// it will change his body_yaw pose parameter before changing his rendered angle).
	virtual const Angle& GetRenderAngles() = 0;
};

class C_AnimationLayer
{
public:
	char  pad_0000[20];
	uint32_t m_nOrder; //0x0014
	uint32_t m_nSequence; //0x0018
	float_t m_flPrevCycle; //0x001C
	float_t m_flWeight; //0x0020
	float_t m_flWeightDeltaRate; //0x0024
	float_t m_flPlaybackRate; //0x0028
	float_t m_flCycle; //0x002C
	void *m_pOwner; //0x0030 // player's thisptr
	char  pad_0038[4]; //0x0034
};
