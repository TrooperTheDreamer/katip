#pragma once

/*
 * Image Text Processor Program
 *
 * "system.hpp" by Caner'Trooper'Kurt
 *
 *
 * System Operations
 *
 *
 *
 */

#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "main.hpp"
#include <string>

namespace System
{
	//
	// Global Functions
	//

	/**
		Gets screen resolution for the primary monitor
	*/
	RECT GetScreenResolution(HWND hwnd = GetDesktopWindow()); 
	/** 
		Converts string to wstring
	*/
	std::wstring ConvertStringToWstring(const std::string& string);
	/**
		Converts wstring to string
	*/
	std::string ConvertWstringToString(const std::wstring& wstring);
	/**
		Gets Application directory
	*/
	std::wstring GetApplicationDirectory(void);
	/**
		Trims wide string from both ends
	*/
	std::wstring TrimWideString(const std::wstring& wstring);
}

#endif