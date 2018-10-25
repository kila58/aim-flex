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
						matsystemsurface->SetDrawColor(Color(255, 0, 0));
						matsystemsurface->DrawFilledRect(screen.x - 2, screen.y - 2, 4, 4);

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

void Debug::AddBox(const Vector& pos, bool called_multiple_times)
{
	auto hash = FNV{}(AT);

	if (queue.find(hash) == queue.end())
	{
		queue[hash] = {};
		queue[hash].DrawType = BOX;
	}

	auto& positions = queue.at(hash).positions;

	if (called_multiple_times)
	{
		if (positions.size() >= 256)
			positions.erase(positions.begin());
	}
	else
		positions.clear();

	positions.emplace_back(pos);
}

void Debug::Destroy()
{

}

Debug debug;
