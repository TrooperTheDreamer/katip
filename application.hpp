#pragma once

/*
 * Image Text Processor Program
 *
 * "application.hpp" by Caner'Trooper'Kurt
 *
 *
 * Application Logic Operations
 *
 * Classes(Application)
 *
 */

#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "main.hpp"
#include "gui.hpp"
#include <opencv2\opencv.hpp>

namespace Application
{
	//
	// Application Class
	//
	class Application
	{
		public :

			Application();
			~Application();

			static HINSTANCE    GetInstance(void);
			static void         SetInstance(HINSTANCE instance);

			static std::wstring GetCmdLine(void);
			static void         SetCmdLine(std::wstring cmdLine);

			static int          GetCmdShow(void);
			static void         SetCmdShow(int cmdShow);

			static std::wstring GetAppName(void);
			static void         SetAppName(std::wstring appName);

			static GUI::Window* GetWindow(void);

			/**
				Initializes the application

				Creates necessary elements for the application
			*/
			static void Initialize(void);
			/**
				Deinitializes the application

				Releases the resources back to the system
			*/
			static void Deinitialize(void);
			/**
				Runs the application

				Programs main loop is inside that function
			*/
			static void Run(void);	
			/**
				Quits the application

				Deinitializes the application and posts quit message
			*/
			static void Quit(void);
			/**
				Checks if image scale input is valid and returns it (returns zero on failure)
			*/
			static int CheckAndReturnScaleInput(void);
			/**
				Checks if font size is valid and returns it (returns zero on failure)
			*/
			static int CheckAndReturnFontSize(void);
			/**
				Opens the image using image selector and decodes it(returns true on success)
			*/
			static bool OpenAndDecodeImageFile(void);
			/**
				Processes the image file detecting text on the image(returns true on success)

				inputScale - scale of the input image 
				fontSize   - size of the font in pixels
			*/
			static bool ProcessImageFile(const int inputScale, const int fontSize);
			/**
				Renders the Image file in a seperate window after scaling it
			*/
			static bool ShowImageFile(void);
			/**
				Window Procedure of the application

				Handles the application messages sent by the OS
			*/
			static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		private:

			static HINSTANCE    mInstance;
			static std::wstring mCmdLine;
			static int          mCmdShow;
			static std::wstring mAppName;

			static GUI::Window*   mWindow;               //Application's main window
			static GUI::Label*    mFontSizeLabel;        //Main Window's font size label
			static GUI::EditBox*  mFontSizeEditBox;      //Main Window's font size edit box
			static GUI::Label*    mAlgoCalcInputLabel;   //Main Window's Algorithm Calculation Input Label
			static GUI::EditBox*  mAlgoCalcInputEditBox; //Main Window's Algorith Calculation Input Edit Box
			static GUI::Button*   mOpenImageBtn;         //Main Window's Open Image Button
			static GUI::Button*   mCloseWindowBtn;       //Main Window's Close Window Button
			static cv::Mat        mImage;                //Image file to be processed
			static std::wstring   mImageFileFullPath;    //Full path of the image file
	};
}

#endif