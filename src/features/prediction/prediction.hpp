#pragma once

#include "../features.hpp"

#include "../../aim-flex.hpp"

class IMoveHelper;

class Prediction : public BaseFeature
{
private:
	float old_time;
	float old_frame;
public:
	Prediction() : BaseFeature(CREATEMOVE, 2u) {}

	void Init();
	void Invoke();
	void End();
	void Destroy();

	IMoveHelper* movehelper;
};


#define	COORD_INTEGER_BITS			14
#define COORD_FRACTIONAL_BITS		5
#define COORD_DENOMINATOR			(1<<(COORD_FRACTIONAL_BITS))
#define COORD_RESOLUTION	(1.0/(COORD_DENOMINATOR))

#define DIST_EPSILON (0.03125)

#define	MAX_CLIP_PLANES		5

#define PLAYER_MAX_SAFE_FALL_SPEED	580 // approx 20 feet

class GameMove
{
private:
	//player
	Vector m_vecVelocity;
	Vector m_vecBaseVelocity;
	Vector m_vecAbsOrigin;
	Vector m_outWishVel;
	float m_surfaceFriction = 1;
	float m_outStepHeight = 0;
	Vector m_outJumpVel;
	float m_flStepSize = 16;

	//cvars
	float sv_gravity = 800;
	float sv_friction = 4;
	float sv_stopspeed = 100;
	float sv_accelerate = 10;
	float sv_airaccelerate = 10;
	float sv_bounce = 0;

	//meta
	float frametime;
	C_BaseEntity* groundentity;
	Vector vec3_origin;

	//settings
	bool debug = false;

public:
	void InitalState(C_BaseEntity* ply);
	void Spew(std::string text, bool fordebug = true);
	trace_t UTIL_TraceRay(const Ray_t &ray, unsigned int mask, C_BaseEntity* ignore, int collisionGroup);
	trace_t TracePlayerBBox(C_BaseEntity* ply, Vector start, Vector end, unsigned int fmask, int collisiongroup);
	trace_t TryTouchGround(C_BaseEntity* ply, Vector start, Vector end, Vector mins, Vector maxs, unsigned int fmask, int collisiongroup);
	void TryTouchGroundInQuadrants(C_BaseEntity* ply, Vector start, Vector end, unsigned int fmask, int collisiongroup, trace_t& pm);
	void CategorizePosition(C_BaseEntity* ply);
	bool IsOnGround(C_BaseEntity* ply);
	void StartGravity(C_BaseEntity* ply);
	void FinishGravity(C_BaseEntity* ply);
	void Friction(C_BaseEntity* ply);
	void Accelerate(C_BaseEntity* ply, Vector wishdir, float wishspeed, float accel);
	void AirAccelerate(C_BaseEntity* ply, Vector wishdir, float wishspeed, float accel);
	void StayOnGround(C_BaseEntity* ply);
	int TryPlayerMove(C_BaseEntity* ply, Vector* firstdest = nullptr, trace_t* firsttrace = nullptr);
	void StepMove(C_BaseEntity* ply, Vector destination, trace_t &trace);
	void WalkMove(C_BaseEntity* ply, CUserCmd* cmd);
	void AirMove(C_BaseEntity* ply, CUserCmd* cmd);
	void FullWalkMove(C_BaseEntity* ply, CUserCmd* cmd);
	void PlayerMove(C_BaseEntity* ply, CUserCmd* cmd);
	void RenderMove(C_BaseEntity* ply, CUserCmd* cmd, int ticks);
};

extern GameMove gamemove;
extern Prediction predict;
