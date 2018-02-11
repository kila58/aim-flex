#pragma once

class IVModelRender
{
public:
	void ForcedMaterialOverride(IMaterial* mat)
	{
		return getvfunc<void(__thiscall*)(void*, IMaterial*, int, int)>(this, 1)(this, mat, 0, 0);
	}
};

typedef unsigned short ModelInstanceHandle_t;

struct ModelRenderInfo_t
{
	Vector origin;
	Angle angles;
	void *pRenderable;
	const model_t *pModel;
	const matrix3x4_t* pModelToWorld;
	const matrix3x4_t* pLightingOffset;
	const Vector *pLightingOrigin;
	int flags;
	int entity_index;
	int skin;
	int body;
	int hitboxset;
	ModelInstanceHandle_t instance;
	ModelRenderInfo_t()
	{
		pModelToWorld = NULL;
		pLightingOffset = NULL;
		pLightingOrigin = NULL;
	}
};
