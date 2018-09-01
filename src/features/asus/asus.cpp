#include "asus.hpp"

#include "../../aim-flex.hpp"

void Asus::Init()
{

}

inline void ModulateTextures(float opacity, bool& first)
{
	if (false)
	{

		if (first && engineclient->IsInGame())
		{
			for (MaterialHandle_t i = materialsystem->FirstMaterial(); i != materialsystem->InvalidMaterial(); i = materialsystem->NextMaterial(i))
			{
				IMaterial* mat = materialsystem->GetMaterial(i);

				//if (mat && std::string(mat->GetTextureGroupName()) == TEXTURE_GROUP_WORLD)
				//	mat->AlphaModulate(opacity);

				if (strstr(mat->GetTextureGroupName(), "World"))
					mat->AlphaModulate(0.8f); // Scale 0-1
			}

			first = false;
		}
	}
}

void Asus::Invoke()
{
	ModulateTextures(0.99f, first);
}

void Asus::Destroy()
{
	bool temp = true;

	ModulateTextures(1.f, temp);
}

Asus asus;
