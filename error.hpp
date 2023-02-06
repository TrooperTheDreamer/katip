#pragma once

/*
 * Image Text Processor Program
 *
 * "error.hpp" by Caner'Trooper'Kurt
 *
 *
 * Error Handling Operations
 *
 * Classes (Exception)
 * Functions(ShowError, ShowErrorAndQuit, GetLastErrorMessage)
 */

#ifndef ERROR_HPP
#define ERROR_HPP

#include "main.hpp"
#include <string>

namespace Error
{
	class Exception
	{
		private:

			std::wstring errorMessage;
			std::wstring errorTitle;

		public:

			Exception(std::wstring message, std::wstring title);

			std::wstring getErrorMessage(void) const;
			std::wstring getErrorTitle(void) const;
	};

	/**
		Shows an error box (wstring version)

		message     - the message body
		caption     - the message title
		messageType - type of the MessageBox
		hwnd        - handle of the window to show the message on
	*/
	int ShowError(std::wstring message, std::wstring caption, HWND hwnd = NULL);
	/**
		Shows an error box (wstring version)

		message     - the message body
		caption     - the message title
		messageType - type of the MessageBox
		hwnd        - handle of the window to show the message on
	*/
	int ShowError(std::string message, std::wstring caption, HWND hwnd = NULL);
	/**
		Shows an error box and quits the application(wstring version)

		message     - the message body
		caption     - the message title
		messageType - type of the MessageBox
		exitCode    - exit code to return to the operating system
		hwnd        - handle of the window to show the message on
	*/
	void ShowErrorAndQuit(std::wstring message, std::wstring caption, int exitCode = 0, HWND hwnd = NULL);
	/**
		Shows an error box and quits the application(basic string version)

		message     - the message body
		caption     - the message title
		messageType - type of the MessageBox
		exitCode    - exit code to return to the operating system
		hwnd        - handle of the window to show the message on
	*/
	void ShowErrorAndQuit(std::string message, std::wstring caption, int exitCode = 0, HWND hwnd = NULL);
	/**
		Grabs and returns the last error message of the system
	*/
	std::wstring GetLastErrorMessage(void);
}

#endif