#pragma once

#include "../features.hpp"

#include <sstream>
#include <unordered_map>

class Vector;

enum DebugDrawTypes
{
	BOX = 1
};

struct DrawQueueItem
{
	DebugDrawTypes DrawType;
	// only using std::vector cause I can forward intialize it...
	std::vector<Vector> positions;
};

class Debug : public BaseFeature
{
private:
	std::ostringstream stream;
	std::unordered_map<int, DrawQueueItem> queue;
public:
	Debug() : BaseFeature(PAINT) {}

	void Init();
	void Invoke();
	void AddBox(const Vector& pos);
	template<typename T> Debug& operator<<(T val)
	{
		stream << val;

		cvar->ConsoleColorPrintf(stream.str().c_str());

		stream = std::ostringstream();

		return *this;
	}
	void Destroy();
};

extern Debug debug;
