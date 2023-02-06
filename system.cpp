#include "main.hpp"
#include "application.hpp"
#include "system.hpp"
#include "gui.hpp"
#include "error.hpp"
#include <clocale>
#include <locale>

//
// Global Functions
//
RECT System::GetScreenResolution(HWND hwnd)
{
	//get resolution
	DEVMODE devMode;
	ZeroMemory(&devMode, sizeof(DEVMODE));

	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode);
	
	//return the resolution RECT
	return { 0, 0, (LONG)devMode.dmPelsWidth, (LONG)devMode.dmPelsHeight };
}

std::wstring System::ConvertStringToWstring(const std::string& string)
{
	std::wstring temp(string.length(), L' ');
	std::copy(string.begin(), string.end(), temp.begin());

	return temp;
}

std::string System::ConvertWstringToString(const std::wstring& wstring)
{
	std::setlocale(LC_ALL, "");

	const std::locale                                   locale("");
	typedef std::codecvt<wchar_t, char, std::mbstate_t> converter_type;
	const converter_type&                               converter = std::use_facet<converter_type>(locale);
	std::vector<char>                                   to(wstring.length() * converter.max_length());
	std::mbstate_t                                      state;
	const wchar_t*                                      from_next;
	char*                                               to_next;
	const converter_type::result                        result = converter.out(state, wstring.data(), wstring.data() + wstring.length(),
		                                                                      from_next, &to[0], &to[0] + to.size(), to_next);
	if (result == converter_type::ok || result == converter_type::noconv) {
		return std::string(&to[0], to_next);
	}
	else {
		return std::string{};
	}
}

std::wstring System::GetApplicationDirectory(void)
{
	WCHAR path[MAX_PATH];
	GetModuleFileNameW(NULL, path, MAX_PATH);

	std::wstring directory = path;

	directory = directory.substr(0, directory.find_last_of(L"\\"));

	return directory;
}

std::wstring System::TrimWideString(const std::wstring& wstring)
{
	size_t frontTrimEnd{ 0 }, endTrimEnd{ wstring.length() };
	bool   empty{ true };

	// get front end position
	for (size_t i = 0; i < wstring.length(); ++i) {
		if (wstring[i] == L' ') {
			++frontTrimEnd;
		} else {
			empty = false;

			break;
		}
	}

	// get back end position
	for (size_t i = wstring.length() - 1; i > 0; --i) {
		if (wstring[i] == L' ') {
			--endTrimEnd;
		} else {
			empty = false;

			break;
		}
	}

	if (empty) {
		return L"";
	}

	//return sub string
	return wstring.substr(frontTrimEnd, endTrimEnd - frontTrimEnd);
}


