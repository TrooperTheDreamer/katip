#pragma once

/*
 * Image Text Processor Program
 *
 * "gui.hpp" by Caner'Trooper'Kurt
 *
 *
 * Graphical User Interface Operations
 *
 * Classes(Window, GUIElement, Button, EditBox, Label)
 * Functions(ShowInfoBox, ShowQuestionBox) 
 *
 */

#ifndef GUI_HPP
#define GUI_HPP

#include "main.hpp"
#include <string>

namespace GUI
{
	//
	// Global Definitions
	//
	enum INFO_BOX_TYPE
	{
		IBT_EMPTY,
		IBT_INFO,
		IBT_WARNING,
		IBT_ERROR,
	};

	constexpr int DEFAULT_WINDOW_CLIENT_WIDTH  = 1280;
	constexpr int DEFAULT_WINDOW_CLIENT_HEIGHT = 720;

	//
	// Classes
	//
	class Window
	{
		public:

			/*
				COTORS AND DOTOR
			*/
			Window();
			Window(std::wstring className, std::wstring title, POINT position, int clientWidth, int clientHeight);
			Window(const Window& window) = delete;
			~Window();

			/*
				Operators
			*/
			const Window& operator=(const Window& window) = delete;

			/*
				Getters and Setters
			*/
			HWND getHandle(void) const;
			void setHandle(const HWND hwnd);

			std::wstring getClassName(void) const;
			void         setClassName(std::wstring className);

			std::wstring getTitle(void) const;
			void         setTitle(const std::wstring title);

			POINT getPosition(void) const;
			void  setPosition(POINT point);
			void  setPosition(int x, int y);

			int  getWidth(void) const;
			void setWidth(int width);

			int  getHeight(void) const;
			void setHeight(int height);

			RECT  getWindowRect(void) const;
			RECT  getClientRect(void) const;

			/**
				Registers window class

				instance - instance of the application
			*/
			bool registerClass(HINSTANCE instance) const;
			/**
				Creates the window
			*/
			bool create(void);
			/**
				Shows the window
			*/
			bool show(void);
			/**
				Updates the window
			*/
			bool update(void) const;
			/**
				Destroys the window
			*/
			bool destroy(void);

		private:

			HWND         mHwnd;      //Handle of the window
			std::wstring mClassName; //Window class name to be used in registeratio
			std::wstring mTitle;     //Title text of the window's title var
			POINT        mPosition;  //Position of the window on the screen
			int          mWidth;     //Width of the window
			int          mHeight;    //Height of the window

	};

	//
	// GUI Element
	//
	class GUIElement
	{
		public:

			/*
				COTORS AND DOTOR
			*/
			GUIElement();
			GUIElement(HWND parent, int x, int y, int width, int height);
			GUIElement(const GUIElement& element) = delete;
			virtual ~GUIElement();


			/*
				Operators
			*/
			const GUIElement& operator=(const GUIElement& button) = delete;


			/*
				Getters and Setters
			*/
			virtual HWND getHwnd(void) const;

			virtual HWND getParentHwnd(void) const;
			virtual void setParentHwnd(HWND parent);

			virtual int  getXCoordinate(void) const;
			virtual void setXCoordinate(const int x);

			virtual int  getYCoordinate(void) const;
			virtual void setYCoordinate(const int y);

			virtual int  getWidth(void) const;
			virtual void setWidth(const int width);

			virtual int  getHeight(void) const;
			virtual void setHeight(const int height);

			/**
				Creates the GUI Element
			*/
			virtual bool create(void) = 0;

		protected:

			HWND mHwnd;       //Handle of the control
			HWND mParentHwnd; //Handle of the control's parent
			int  mXCoord;     //X Coordinate on the parent
			int  mYCoord;     //Y Coordinate on the parent
			int  mWidth;      //Width of the control
			int  mHeight;     //Height of the control
	};

	//
	// Button Class
	//
	class Button : public GUIElement
	{
		public:

			/*
				Cotors and Dotor
			*/
			Button();
			Button(HWND parent, int x, int y, int width, int height, const std::wstring& text);
			Button(const Button& button) = delete;
			virtual ~Button();


			/*
				Operators
			*/
			const Button& operator=(const Button& button) = delete;


			/*
				Getters and Setters
			*/
			const std::wstring& getText(void) const;
			void                setText(const std::wstring& text);

			/*
				Updates button's text
			*/
			void updateText(const std::wstring& text);

			/**
			  Creates the button control
			*/
			bool create(void);

		private:

			std::wstring mText; //Text of the button
	};

	//
	// Label Class
	//
	class Label : public GUIElement
	{
		public:

			/*
			   Cotors and Dotor
			*/
			Label();
			Label(HWND parent, int x, int y, int width, int height, const std::wstring& text);
			Label(const Label& label) = delete;
			virtual ~Label();

			/*
			   Operators
			*/
			const Label& operator=(const Label& button) = delete;

			/*
			   Getters and Setters
			*/
			const std::wstring& getText(void) const;
			void                setText(const std::wstring& text);


			/*
				Updates label's text
			*/
			void updateText(const std::wstring& text);
			
			/**
			   Creates the label control
			*/
			bool create(void);

		private:

			std::wstring mText; //Text of the label
	};

	//
	// Edit Box Class
	//
	class EditBox : public GUIElement
	{
		public:

			/*
				Cotors and Dotor
			*/
			EditBox();
			EditBox(HWND parent, int x, int y, int width, int height, const std::wstring& text);
			EditBox(const EditBox& label) = delete;
			virtual ~EditBox();


			/*
				Operators
			*/
			const EditBox& operator=(const EditBox& button) = delete;


			/*
				Getters and Setters
			*/
			const std::wstring& getText(void) const;
			void                setText(const std::wstring& text);

			/*
				Updates edit boxes text
			*/
			void updateText(const std::wstring& text);

			/**
				Creates the edit box control
			*/
			bool create(void);

		private:

			std::wstring mText; //Text of the edit control
	};


	//
	// Global Functions
	//

	/**
		Shows a message box depending on the messageType (wstring version)

		message     - the message body
		caption     - the message title
		messageType - type of the MessageBox
		hwnd        - the handle of the window to show the Info Box on
	*/
	int ShowInfoBox(std::wstring message, std::wstring caption, INFO_BOX_TYPE messageType = IBT_EMPTY, HWND hwnd = NULL);
	/**
		Shows a message box depending on the messageType (basic string version)

		message     - the message body
		caption     - the message title
		messageType - type of the MessageBox
		hwnd        - the handle of the window to show the Info Box on
	*/
	int ShowInfoBox(std::string message, std::wstring caption, INFO_BOX_TYPE messageType = IBT_EMPTY, HWND hwnd = NULL);
	/**
		Shows a question box (wstring version)

		question - question body
		caption  - question title
		hwnd     - the handle of the window to show the Info Box on
	*/
	int ShowQuestionBox(std::wstring question, std::wstring caption, HWND hwnd = NULL);
}

#endif