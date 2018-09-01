#include "debug.hpp"

#include "../../aim-flex.hpp"

#include <vector>

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define AT __FILE__ ":" TOSTRING(__LINE__)

struct FNV
{
	size_t operator()(std::string const& str) const
	{
		size_t ret = 2166136261U;

		for (std::string::const_iterator iterator = str.begin(); iterator != str.end(); ++iterator)
		{
			ret = (16777619 * ret) ^ static_cast<unsigned char>(*iterator);
		}

		return ret;
	}
};

void Debug::Init()
{

}

void Debug::Invoke()
{
	for (auto& items : queue)
	{
		auto& positions = items.second.positions;

		for (auto& pos : positions)
		{
			Vector screen;

			if (WorldToScreen(pos, screen))
			{
				switch (items.second.DrawType)
				{
					case BOX:
					{
						matsystemsurface->SetDrawColor(Color(255, 255, 255));
						matsystemsurface->DrawFilledRect(screen.x - 4, screen.y - 4, 8, 8);

						break;
					}
					default:
					{
						break;
					}
				}
			}
		}
	}
}

void Debug::AddBox(const Vector& pos)
{
	auto hash = FNV{}(AT);

	if (queue.find(hash) == queue.end())
	{
		queue[hash] = {};
		queue[hash].DrawType = BOX;
	}

	auto& positions = queue.at(hash).positions;

	//if (positions.size() >= 100)
	//	positions.erase(positions.begin());

	positions.clear();

	positions.emplace_back(pos);
}

void Debug::Destroy()
{

}

Debug debug;
