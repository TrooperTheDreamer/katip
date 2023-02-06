#include "gui.hpp"
#include "error.hpp"
#include "system.hpp"
#include "application.hpp"
#include <string>

extern Application::Application* App;

//
// Window Class Member Functions
//

GUI::Window::Window()
	: mHwnd(NULL), mClassName(), mTitle(), mPosition({0,0}), mWidth(0), mHeight(0)
{}

GUI::Window::Window(std::wstring className, std::wstring title, POINT position, int clientWidth, int clientHeight)
	: mHwnd(NULL), mClassName(className), mTitle(title), mPosition(position), mWidth(0), mHeight(0)
{
	if (clientWidth == 0 && clientHeight == 0) {
		clientWidth  = DEFAULT_WINDOW_CLIENT_WIDTH;
		clientHeight = DEFAULT_WINDOW_CLIENT_HEIGHT;
	}

	// determine window rect from the client area dimensions and styles
	RECT r = { 0, 0, clientWidth, clientHeight };
	AdjustWindowRectEx(&r, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE, FALSE, WS_EX_LTRREADING);

	// set width and height
	mWidth  = r.right  - r.left;
	mHeight = r.bottom - r.top;
}

GUI::Window::~Window()
{
	UnregisterClass(mClassName.c_str(), App->GetInstance());
}

HWND GUI::Window::getHandle(void) const
{
	return this->mHwnd;
}

void GUI::Window::setHandle(const HWND hwnd)
{
	this->mHwnd = hwnd;
}

std::wstring GUI::Window::getClassName(void) const
{
	return this->mClassName;
}

void GUI::Window::setClassName(std::wstring className)
{
	this->mClassName = className;
}

std::wstring GUI::Window::getTitle(void) const
{
	return this->mTitle;
}

void GUI::Window::setTitle(std::wstring title)
{
	this->mTitle = title;
}

POINT GUI::Window::getPosition(void) const
{
	return mPosition;
}

void GUI::Window::setPosition(POINT point)
{
	mPosition.x = point.x;
	mPosition.y = point.y;
}

void GUI::Window::setPosition(int x, int y)
{
	mPosition.x = x;
	mPosition.y = y;
}

int GUI::Window::getWidth(void) const
{
	return this->mWidth;
}

void GUI::Window::setWidth(const int width)
{
	mWidth = width;
}

int GUI::Window::getHeight(void) const
{
	return mHeight;
}

void GUI::Window::setHeight(const int height)
{
	mHeight = height;
}

RECT GUI::Window::getWindowRect(void) const
{
	return{ mPosition.x, mPosition.y, mWidth, mHeight };
}

RECT GUI::Window::getClientRect(void) const
{
	RECT clientRect;

	GetClientRect(mHwnd, &clientRect);

	return clientRect;
}

bool GUI::Window::registerClass(HINSTANCE instance) const
{
	try {
		WNDCLASSEX wc;
		ZeroMemory(&wc, sizeof(wc));

		//initialize window class
		wc.style         = CS_CLASSDC | CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
		wc.cbSize        = sizeof(WNDCLASSEX);
		wc.lpfnWndProc   = Application::Application::WndProc;
		wc.hInstance     = instance;
		wc.lpszClassName = mClassName.c_str();
		wc.hIcon         = LoadIcon(App->GetInstance(), NULL);
		wc.hIconSm       = LoadIcon(App->GetInstance(), NULL);
		wc.hCursor       = LoadCursor(0, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

		//register window class
		if (!RegisterClassEx(&wc)) {
			throw Error::Exception(L"Can't register the window class! : \n\n" + Error::GetLastErrorMessage(), L"Window Registeration Error");
		}

		return true;
	} catch (Error::Exception &ex) {
		Error::ShowErrorAndQuit(ex.getErrorMessage(), ex.getErrorTitle());

		return false;
	} catch (std::exception &ex) {
		Error::ShowErrorAndQuit(ex.what(), L"Window Registeration Error");

		return false;
	}
}

bool GUI::Window::create(void)
{
	try {
		//Create the window
		mHwnd = CreateWindowEx(WS_EX_LTRREADING,
							   mClassName.c_str(),
							   mTitle.c_str(),
			                   WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
							   mPosition.x,
							   mPosition.y,
							   mWidth,
							   mHeight,
							   NULL,
							   NULL,
							   App->GetInstance(),
							   NULL);

	
		if (!mHwnd) {
			throw Error::Exception(L"Can't create the window! : \n\n" + Error::GetLastErrorMessage(), L"Window Creation Error");
		}

		return true;
	} catch (Error::Exception& ex) {
		Error::ShowErrorAndQuit(ex.getErrorMessage(), ex.getErrorTitle());

		return false;
	} catch (std::exception &ex) {
		Error::ShowErrorAndQuit(ex.what(), L"Window Creation Error");

		return false;
	}
}

bool GUI::Window::show(void)
{
	try {
		if (!ShowWindow(this->mHwnd, App->GetCmdShow())) {
			throw Error::Exception(L"Can't show the window! : \n\n" + Error::GetLastErrorMessage(), L"Show Window Error");
		}

		return true;
	} catch (Error::Exception& ex) {
		Error::ShowErrorAndQuit(ex.getErrorMessage(), ex.getErrorTitle());

		return false;
	} catch (std::exception &ex) {
		Error::ShowErrorAndQuit(ex.what(), L"Show Window Error");

		return false;
	}
}

bool GUI::Window::update(void) const
{
	try {
		if (!UpdateWindow(mHwnd)) {
			throw Error::Exception(L"Can't update the window! : \n\n" + Error::GetLastErrorMessage(), L"Window Update Error");
		}

		return true;
	} catch (Error::Exception& ex) {
		Error::ShowErrorAndQuit(ex.getErrorMessage(), ex.getErrorTitle());

		return false;
	} catch (std::exception &ex) {
		Error::ShowErrorAndQuit(ex.what(), L"Window Update Error");

		return false;
	}
}

bool GUI::Window::destroy(void)
{
	try {
		if (!DestroyWindow(mHwnd)) {
			throw Error::Exception(L"Can't destroy the window! : \n" + Error::GetLastErrorMessage(), L"Destroy Window Error");
		}

		return true;
	} catch (Error::Exception& ex) {
		Error::ShowErrorAndQuit(ex.getErrorMessage(), ex.getErrorTitle());

		return false;
	} catch (std::exception &ex) {
		Error::ShowErrorAndQuit(ex.what(), L"Destroy Window Error");

		return false;
	}
}

//
// GUIElement Class Member Functions
//
GUI::GUIElement::GUIElement() :
	mHwnd(NULL), mParentHwnd(NULL), mXCoord(0), mYCoord(0), mWidth(0), mHeight(0)
{}

GUI::GUIElement::GUIElement(HWND parent, int x, int y, int width, int height) :
	mHwnd(NULL), mParentHwnd(parent), mXCoord(x), mYCoord(y), mWidth(width), mHeight(height)
{}

GUI::GUIElement::~GUIElement()
{}

HWND GUI::GUIElement::getHwnd(void) const
{
	return mHwnd;
}

HWND GUI::GUIElement::getParentHwnd(void) const
{
	return mParentHwnd;
}

void GUI::GUIElement::setParentHwnd(HWND parent)
{
	mParentHwnd = parent;
}

int GUI::GUIElement::getXCoordinate(void) const
{
	return mXCoord;
}

void GUI::GUIElement::setXCoordinate(const int x)
{
	mXCoord = x;
}

int GUI::GUIElement::getYCoordinate(void) const
{
	return mYCoord;
}

void GUI::GUIElement::setYCoordinate(const int y)
{
	mYCoord = y;
}

int GUI::GUIElement::getWidth(void) const
{
	return mWidth;
}

void GUI::GUIElement::setWidth(const int width)
{
	mWidth = width;
}

int GUI::GUIElement::getHeight(void) const
{
	return mHeight;
}

void GUI::GUIElement::setHeight(const int height)
{
	mHeight = height;
}

//
// Button Class Member Functions
//
GUI::Button::Button() :
	GUIElement(), mText()
{}

GUI::Button::Button(HWND parent, int x, int y, int width, int height, const std::wstring& text) :
	GUIElement(parent, x, y, width, height), mText(text)
{
	create();
}

GUI::Button::~Button()
{ }

const std::wstring& GUI::Button::getText(void) const
{
	return mText;
}

void GUI::Button::setText(const std::wstring& text)
{
	mText = text;
}

void GUI::Button::updateText(const std::wstring& text)
{
	mText = text;
	SetWindowText(mHwnd, mText.c_str());
}

bool GUI::Button::create(void)
{
	try {
		//Create the button
		mHwnd = CreateWindow(TEXT("BUTTON"),
			                 mText.c_str(),
			                 WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			                 mXCoord,
			                 mYCoord,
			                 mWidth,
			                 mHeight,
			                 mParentHwnd,
			                 NULL,
			                 App->GetInstance(),
			                 NULL);

		if (!mHwnd) {
			throw Error::Exception(L"Can't create the button! : \n\n" + Error::GetLastErrorMessage(), L"Button Creation Error");
		}

		return true;
	} catch (Error::Exception& ex) {
		Error::ShowErrorAndQuit(ex.getErrorMessage(), ex.getErrorTitle());

		return false;
	} catch (std::exception &ex) {
		Error::ShowErrorAndQuit(ex.what(), L"Button Creation Error");

		return false;
	}
}

//
// Label Class Member Functions
//
GUI::Label::Label() :
	GUIElement(), mText()
{}

GUI::Label::Label(HWND parent, int x, int y, int width, int height, const std::wstring& text) :
	GUIElement(parent, x, y, width, height), mText(text)
{
	create();
}

GUI::Label::~Label()
{ }

const std::wstring& GUI::Label::getText(void) const
{
	return mText;
}

void GUI::Label::setText(const std::wstring& text)
{
	mText = text;
}

void GUI::Label::updateText(const std::wstring& text)
{
	mText = text;
	SetWindowText(mHwnd, mText.c_str());
}

bool GUI::Label::create(void)
{
	try {
		//Create the label
		mHwnd = CreateWindow(TEXT("static"),
			                 mText.c_str(),
			                 WS_CHILD | WS_VISIBLE,
			                 mXCoord, mYCoord, mWidth, mHeight,
			                 mParentHwnd,
			                 NULL, App->GetInstance(), NULL);

		if (!mHwnd) {
			throw Error::Exception(L"Can't create the label! : \n\n" + Error::GetLastErrorMessage(), L"Label Creation Error");
		}

		return true;
	} catch (Error::Exception& ex) {
		Error::ShowErrorAndQuit(ex.getErrorMessage(), ex.getErrorTitle());

		return false;
	} catch (std::exception &ex) {
		Error::ShowErrorAndQuit(ex.what(), L"Label Creation Error");

		return false;
	}
}

//
// Edit Box Class Member Functions
//
GUI::EditBox::EditBox() :
	GUIElement(), mText()
{}

GUI::EditBox::EditBox(HWND parent, int x, int y, int width, int height, const std::wstring& text) :
	GUIElement(parent, x, y, width, height), mText(text)
{
	create();
}

GUI::EditBox::~EditBox()
{ }

const std::wstring& GUI::EditBox::getText(void) const
{
	return mText;
}

void GUI::EditBox::setText(const std::wstring& text)
{
	mText = text;
}

void GUI::EditBox::updateText(const std::wstring& text)
{
	mText = text;
	SetWindowText(mHwnd, mText.c_str());
}

bool GUI::EditBox::create(void)
{
	try {
		//Create the edit box
		mHwnd = CreateWindow(TEXT("EDIT"),
							 mText.c_str(),
			                 WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
			                 mXCoord, mYCoord, mWidth, mHeight,
			                 mParentHwnd, 
			                 NULL, App->GetInstance(), NULL);

		if (!mHwnd) {
			throw Error::Exception(L"Can't create the text box! : \n\n" + Error::GetLastErrorMessage(), L"Text Box Creation Error");
		}

		return true;
	} catch (Error::Exception& ex) {
		Error::ShowErrorAndQuit(ex.getErrorMessage(), ex.getErrorTitle());

		return false;
	}
	catch (std::exception &ex) {
		Error::ShowErrorAndQuit(ex.what(), L"Text Box Creation Error");

		return false;
	}
}

//
// Global Functions
//
int GUI::ShowInfoBox(std::wstring message, std::wstring caption, INFO_BOX_TYPE messageType, HWND hwnd)
{
	UINT messageBoxType = 0;

	// determine messageBoxType depending on the messageType argument
	switch (messageType) {
		case IBT_EMPTY :
			messageBoxType = 0;
		break;

		case IBT_INFO :
			messageBoxType = MB_ICONINFORMATION;
		break;

		case IBT_WARNING :
			messageBoxType = MB_ICONWARNING;
		break;
			
		case IBT_ERROR :
			messageBoxType = MB_ICONERROR;
		break;

		default :
			messageBoxType = 0;
		break;
	}

	messageBoxType |= MB_SETFOREGROUND | MB_TOPMOST;

	return MessageBox(hwnd, message.c_str(), caption.c_str(), messageBoxType);
}

int GUI::ShowInfoBox(std::string message, std::wstring caption, INFO_BOX_TYPE messageType, HWND hwnd)
{
	UINT messageBoxType = 0;

	// determine messageBoxType depending on the messageType argument
	switch (messageType) {
		case IBT_EMPTY:
			messageBoxType = 0;
			break;

		case IBT_INFO:
			messageBoxType = MB_ICONINFORMATION;
			break;

		case IBT_WARNING:
			messageBoxType = MB_ICONWARNING;
			break;

		case IBT_ERROR:
			messageBoxType = MB_ICONERROR;
			break;

		default:
			messageBoxType = 0;
			break;
	}

	messageBoxType |= MB_SETFOREGROUND | MB_TOPMOST;

	return MessageBox(hwnd, System::ConvertStringToWstring(message).c_str(), caption.c_str(), messageBoxType);
}

int GUI::ShowQuestionBox(std::wstring question, std::wstring caption, HWND hwnd)
{
	return MessageBox(hwnd, question.c_str(), caption.c_str(), MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 | MB_SETFOREGROUND | MB_TOPMOST);
}