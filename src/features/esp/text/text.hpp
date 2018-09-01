#pragma once

// todo:: big ez func for the largeeeeeeeeeeeeee wchar DrAwPrIntTeXt
void ESP::PlayerText()
{
	matsystemsurface->SetFont(font);

	wchar_t wname[128];
	MultiByteToWideChar(CP_UTF8, 0, info.name, -1, wname, 128);

	int tw, th;
	matsystemsurface->GetTextSize(font, wname, tw, th);

	matsystemsurface->SetTextPos(x + w / 2 - tw / 2, y - th - 2);
	matsystemsurface->DrawPrintText(wname);
}
