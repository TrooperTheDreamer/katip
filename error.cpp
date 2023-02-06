#include "error.hpp"
#include "gui.hpp"
#include "application.hpp"

extern Application::Application* App;

//
// Member Functions
//
Error::Exception::Exception(std::wstring message, std::wstring title) : errorMessage(message), errorTitle(title)
{}

std::wstring Error::Exception::getErrorMessage(void) const
{
	return errorMessage;
}

std::wstring Error::Exception::getErrorTitle(void) const
{
	return this->errorTitle;
}

//
// Global Functions
//
std::wstring Error::GetLastErrorMessage(void)
{
	//Get the error message, if any.
	DWORD errorMessageID = GetLastError();

	if (errorMessageID == 0) {
		return std::wstring(); //No error message has been recorded, return empty string
	}

	LPWSTR messageBuffer = nullptr;
	size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		                        NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&messageBuffer,
		                        0, NULL);

	std::wstring message(messageBuffer, size);

	//Free the buffer.
	LocalFree(messageBuffer);

	return message;
}

int Error::ShowError(std::wstring message, std::wstring caption, HWND hwnd)
{
	HWND handle = NULL;

	if (hwnd != NULL) {
		handle = hwnd;
	}

	return GUI::ShowInfoBox(message, caption, GUI::IBT_ERROR, handle);
}

int Error::ShowError(std::string message, std::wstring caption, HWND hwnd)
{
	HWND handle = NULL;

	if (hwnd != NULL) {
		handle = hwnd;
	}

	return GUI::ShowInfoBox(message, caption, GUI::IBT_ERROR, handle);
}

void Error::ShowErrorAndQuit(std::wstring message, std::wstring caption, int exitCode, HWND hwnd)
{
	HWND handle = NULL;

	if (hwnd != NULL) {
		handle = hwnd;
	}

	Error::ShowError(message, caption, handle);	

	PostQuitMessage(0);
}

void Error::ShowErrorAndQuit(std::string message, std::wstring caption, int exitCode, HWND hwnd)
{
	HWND handle = NULL;

	if (hwnd != NULL) {
		handle = hwnd;
	}

	Error::ShowError(message, caption, handle);

	PostQuitMessage(0);
}