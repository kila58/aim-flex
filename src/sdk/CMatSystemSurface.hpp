#pragma once

#undef CreateFont

class CMatSystemSurface
{
public:
	void SetDrawColor(Color col)
	{
		return getvfunc<void(__thiscall*)(void*, Color)>(this, 14)(this, col);
	}
	void DrawFilledRect(int x, int y, int w, int h)
	{
		return getvfunc<void(__thiscall*)(void*, int, int, int, int)>(this, 16)(this, x, y, x + w, y + h);
	}
	void DrawOutlinedRect(int x, int y, int w, int h)
	{
		return getvfunc<void(__thiscall*)(void*, int, int, int, int)>(this, 18)(this, x, y, x + w, y + h);
	}
	void DrawLine(int x1, int y1, int x2, int y2)
	{
		return getvfunc<void(__thiscall*)(void *, int, int, int, int)>(this, 19)(this, x1, y1, x2, y2);
	}
	void SetFont(unsigned long font)
	{
		return getvfunc<void(__thiscall*)(void*, unsigned long)>(this, 23)(this, font);
	}
	void SetTextColor(Color col)
	{
		return getvfunc<void(__thiscall*)(void*, Color)>(this, 24)(this, col);
	}
	void SetTextPos(int x, int y)
	{
		return getvfunc<void(__thiscall*)(void*, int, int)>(this, 26)(this, x, y);
	}
	void SetTextPos(float x, float y)
	{
		return SetTextPos((int)x, (int)y);
	}
	void DrawPrintText(const wchar_t* text, int textLen = -1, int drawType = 0)
	{
		if (textLen == -1)
			textLen = std::wcslen(text);

		return getvfunc<void(__thiscall*)(void*, const wchar_t*, int, int)>(this, 28)(this, text, textLen, drawType);
	}
	void DrawPrintText(const std::string& text)
	{
		int len = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, text.c_str(), text.size(), NULL, 0);
		
		if (!len)
			return;

		std::wstring str(len, 0);
		MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, text.c_str(), text.size(), str.data(), str.size());

		DrawPrintText(str.c_str());
	}
	unsigned int CreateFont()
	{
		return getvfunc<unsigned int(__thiscall*)(void*)>(this, 71)(this);
	}
	bool SetFontGlyphSet(unsigned long font, const char *windowsFontName, int tall, int weight, int blur, int scanlines, int flags)
	{
		return getvfunc<bool(__thiscall*)(void*, unsigned int, const char*, int, int, int, int, int, int, int)>(this, 72)(this, font, windowsFontName, tall, weight, blur, scanlines, flags, 0, 0);
	}
	void GetTextSize(unsigned long font, const wchar_t* text, int& wide, int& tall)
	{
		return getvfunc<void(__thiscall*)(void*, unsigned long, const wchar_t*, int&, int&)>(this, 79)(this, font, text, wide, tall);
	}
	void GetTextSize(unsigned long font, const std::string& text, int& wide, int& tall)
	{
		int len = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, text.c_str(), text.size(), NULL, 0);

		if (!len)
			return;

		std::wstring str(len, 0);
		MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, text.c_str(), text.size(), str.data(), str.size());

		GetTextSize(font, str.c_str(), wide, tall);
	}
};

enum FontFlags
{
	FONTFLAG_NONE,
	FONTFLAG_ITALIC = 0x001,
	FONTFLAG_UNDERLINE = 0x002,
	FONTFLAG_STRIKEOUT = 0x004,
	FONTFLAG_SYMBOL = 0x008,
	FONTFLAG_ANTIALIAS = 0x010,
	FONTFLAG_GAUSSIANBLUR = 0x020,
	FONTFLAG_ROTARY = 0x040,
	FONTFLAG_DROPSHADOW = 0x080,
	FONTFLAG_ADDITIVE = 0x100,
	FONTFLAG_OUTLINE = 0x200,
	FONTFLAG_CUSTOM = 0x400,
	FONTFLAG_BITMAP = 0x800,
};
