#include "main.hpp"
#include "application.hpp"
#include "graphics.hpp"
#include <string>
#include "gui.hpp"
#include "error.hpp"
#include "system.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/dnn.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <algorithm>
#include <locale>
#include <codecvt>
#include <cmath>
#include <fstream>


//
// Member Variables
//
HINSTANCE     Application::Application::mInstance;
std::wstring  Application::Application::mCmdLine;
int           Application::Application::mCmdShow;
std::wstring  Application::Application::mAppName;
GUI::Window*  Application::Application::mWindow;
GUI::Label*   Application::Application::mFontSizeLabel;
GUI::EditBox* Application::Application::mFontSizeEditBox;
GUI::Label*   Application::Application::mAlgoCalcInputLabel;
GUI::EditBox* Application::Application::mAlgoCalcInputEditBox;
GUI::Button*  Application::Application::mOpenImageBtn;
GUI::Button*  Application::Application::mCloseWindowBtn;
cv::Mat       Application::Application::mImage;
std::wstring  Application::Application::mImageFileFullPath;

//
// Member Functions
//
Application::Application::Application()
{}

Application::Application::~Application()
{
	Deinitialize();
}

HINSTANCE Application::Application::GetInstance(void)
{
	return mInstance;
}

void Application::Application::SetInstance(HINSTANCE instance)
{
	mInstance = instance;
}

std::wstring Application::Application::GetCmdLine(void)
{
	return mCmdLine;
}

void Application::Application::SetCmdLine(std::wstring cmdLine)
{
	mCmdLine = cmdLine;
}

int Application::Application::GetCmdShow(void)
{
	return mCmdShow;
}

void Application::Application::SetCmdShow(int cmdShow)
{
	mCmdShow = cmdShow;
}

std::wstring Application::Application::GetAppName(void)
{
	return mAppName;
}

void Application::Application::SetAppName(std::wstring appName)
{
	mAppName = appName;
}

GUI::Window* Application::Application::GetWindow(void)
{
	return mWindow;
}

void Application::Application::Initialize()
{
	// Create the window
	mWindow = new GUI::Window(L"Trooper Was Here", mAppName.c_str(), { 100, 100 }, 312, 110);

	//register the window
	if (mWindow->registerClass(mInstance)) {
		//create the window
		if (mWindow->create()) {
			//update the window
			if (mWindow->update()) {
				//show the window
				if (!mWindow->show()) {
					Quit();
				}
			} else {
				Quit();
			}
		} else {
			Quit();
		}
	} else {
		Quit();
	}


	// Create the algorithm scaling input label
	mAlgoCalcInputLabel = new GUI::Label(mWindow->getHandle(), 6, 6, 225, 20, L"Algorithm Slace Input : ");

	// Create the algorithm scaling input edit box
	mAlgoCalcInputEditBox = new GUI::EditBox(mWindow->getHandle(), 230, 6, 75, 20, L"1280");

	// Create font size label
	mFontSizeLabel = new GUI::Label(mWindow->getHandle(), 6, 32, 225, 20, L"Font Size (in pixels) : ");

	// Create font size edit box
	mFontSizeEditBox = new GUI::EditBox(mWindow->getHandle(), 230, 32, 75, 20, L"16");

	// Create the open image button
	mOpenImageBtn = new GUI::Button(mWindow->getHandle(), 6, 58, 300, 20, L"Open Image File");

	// Create the close application button
	mCloseWindowBtn = new GUI::Button(mWindow->getHandle(), 6, 84, 300, 20, L"Exit Program");

	// Initialize font library
	if (FT_Init_FreeType(&Graphics::FontLibrary)) {
		Error::ShowErrorAndQuit(L"Can't init FreeType!", L"Application Start Error");
	}

	// Create font
	Graphics::MainFont = new Graphics::Font();

	if (!Graphics::MainFont->load("font.ttf")) {
		Error::ShowErrorAndQuit(L"Can't load the main font!", L"Application Start Error");
	}
}

void Application::Application::Deinitialize()
{
	if (mWindow) {
		mWindow->destroy();
		delete mWindow;
		mWindow = nullptr;
	}

	if (!mImage.empty()) { 
		mImage.release();
	}

	if (mAlgoCalcInputLabel) {
		delete mAlgoCalcInputLabel;
		mAlgoCalcInputLabel = nullptr;
	}

	if (mAlgoCalcInputEditBox) {
		delete mAlgoCalcInputEditBox;
		mAlgoCalcInputEditBox = nullptr;
	}

	if (mFontSizeLabel) {
		delete mFontSizeLabel;
		mFontSizeLabel = nullptr;
	}

	if (mFontSizeEditBox) {
		delete mFontSizeEditBox;
		mFontSizeEditBox = nullptr;
	}

	if (mOpenImageBtn) {
		delete mOpenImageBtn;
		mOpenImageBtn = nullptr;
	}

	if (mCloseWindowBtn) {
		delete mCloseWindowBtn;
		mCloseWindowBtn = nullptr;
	}

	//FT_Done_FreeType(Graphics::FontLibrary); 

	if (Graphics::MainFont) {
		delete Graphics::MainFont;
		Graphics::MainFont = nullptr;
	}
}

void Application::Application::Run(void)
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (GetMessage(&msg, NULL, 0, 0)) { //get message from window's messages list
		TranslateMessage(&msg); // translate message
		DispatchMessage(&msg);  // dispatch message		
	}
}

void Application::Application::Quit(void)
{
	Deinitialize();
	PostQuitMessage(0);
}

int Application::Application::CheckAndReturnScaleInput(void)
{
	try {
		std::wstring inputText = System::TrimWideString(mAlgoCalcInputEditBox->getText());

		//check if string is empty
		if (inputText.empty()) {
			throw Error::Exception(L"You did not enter the algorithm scale input!", L"Scale Input Error");
		}

		//check if string is consisted of only digits
		for (int i = 0; i < inputText.size(); ++i) {
			if (!iswdigit(inputText[i])) { //is the wchar not digit?
				throw Error::Exception(L"Algorithm scale input must be a number!", L"Scale Input Error");
			}
		}

		//check if input variable is multiply of 32
		int algCalcInput = std::stoi(inputText);
		if (algCalcInput % 32) {
			throw Error::Exception(L"Algorithm scale input must be multiples of 32!", L"Scale Input Error");
		}

		return algCalcInput;
	} catch (Error::Exception& ex) {
		Error::ShowError(ex.getErrorMessage(), ex.getErrorTitle());

		return 0;
	} catch (std::exception& ex) {
		Error::ShowError(ex.what(), L"Scale Input Error");

		return 0;
	}
}

int Application::Application::CheckAndReturnFontSize(void)
{
	try {
		std::wstring inputText = System::TrimWideString(mFontSizeEditBox->getText());

		//check if string is empty
		if (inputText.empty()) {
			throw Error::Exception(L"You did not enter the font size!", L"Font Size Input Error");
		}

		//check if string is consisted of only digits
		for (int i = 0; i < inputText.size(); ++i) {
			if (!iswdigit(inputText[i])) { //is the wchar not digit?
				throw Error::Exception(L"Font size must be a number!", L"Font Size Input Error");
			}
		}

		//check if font size is bigger than 0
		int fontSize = std::stoi(inputText);
		if (fontSize <= 0) {
			throw Error::Exception(L"Font size must be bigger then zero!", L"Font Size Input Error");
		}

		return fontSize;
	}
	catch (Error::Exception& ex) {
		Error::ShowError(ex.getErrorMessage(), ex.getErrorTitle());

		return 0;
	}
	catch (std::exception& ex) {
		Error::ShowError(ex.what(), L"Font Size Input Error");

		return 0;
	}
}

bool Application::Application::OpenAndDecodeImageFile(void)
{
	try {
		OPENFILENAME ofn;
		ZeroMemory(&ofn, sizeof(OPENFILENAME));

		//
		// Initialize fields of OPENFILENAME structure
		//
		wchar_t szFile[260] = { 0 };
		ofn.lStructSize     = sizeof(ofn);
		ofn.hwndOwner       = mWindow->getHandle();
		ofn.lpstrFile       = szFile;
		ofn.nMaxFile        = sizeof(szFile);
		ofn.lpstrFilter     = L"JPG\0*.JPG\0PNG\0*.PNG\0";
		ofn.nFilterIndex    = 1;
		ofn.lpstrFileTitle  = NULL;
		ofn.nMaxFileTitle   = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags           = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (GetOpenFileName(&ofn)) { //open the picture dialog and select the image
		   mImageFileFullPath = ofn.lpstrFile;

			//
			// Encode unicode filepath image data (OpenCV doesn't accept unicode filepaths)
			//
			FILE* fp = _wfopen(mImageFileFullPath.c_str(), L"rb");

			//determine the size of the file
			fseek(fp, 0, SEEK_END);
			long sz = ftell(fp);

			//read data
			char* buf = new char[sz];
			fseek(fp, 0, SEEK_SET);
			size_t n = fread(buf, 1, sz, fp);
			cv::_InputArray arr(buf, sz);

			//release image if previously loaded
			if (!mImage.empty()) {
				mImage.release();
			}

			//decode from data
			mImage = imdecode(arr, cv::IMREAD_COLOR);

			//delete buffer and close file
			delete[] buf;
			fclose(fp);

			return true;
		}

		return false;
	} catch (Error::Exception& ex) {
		Error::ShowError(ex.getErrorMessage(), ex.getErrorTitle());

		return false;
	} catch (std::exception& ex) {
		Error::ShowError(ex.what(), L"Open Image Error");

		return false;
	}
}

bool Application::Application::ProcessImageFile(const int inputScale, const int fontSize)
{
	try {
		/*
			Algorithm is based on the article on https://learnopencv.com/deep-learning-based-text-detection-using-opencv-c-python/

			I got better results than using Tesseract OCR
			inputScale bigger the better results but more processing time
		*/

		constexpr float confThreshold{ 0.5f };
		constexpr float nonMaxThreshold{ 0.4f };

		//Load the network
		cv::dnn::Net net = cv::dnn::readNet(System::ConvertWstringToString(System::GetApplicationDirectory()) + "\\frozen_east_text_detection.pb");

		//prefer GPU and CUDA cores
		net.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
		net.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA);

		//prepare the input image 
		cv::Mat blob;
		cv::dnn::blobFromImage(mImage, blob, 1.0f, cv::Size(inputScale, inputScale), cv::Scalar(123.68, 116.78, 103.94), true, false);

		//pass input through the netwok
		std::vector<cv::String> outputLayers(2);
		outputLayers[0] = "feature_fusion/Conv_7/Sigmoid";
		outputLayers[1] = "feature_fusion/concat_3";

		std::vector<cv::Mat> output;
		net.setInput(blob);
		net.forward(output, outputLayers);
		cv::Mat scores = output[0];
		cv::Mat geometry = output[1];

		//process the output
		std::vector<cv::RotatedRect> boxes;
		std::vector<float>           confidences;

		CV_Assert(scores.dims == 4); CV_Assert(geometry.dims == 4); CV_Assert(scores.size[0] == 1);
		CV_Assert(geometry.size[0] == 1); CV_Assert(scores.size[1] == 1); CV_Assert(geometry.size[1] == 5);
		CV_Assert(scores.size[2] == geometry.size[2]); CV_Assert(scores.size[3] == geometry.size[3]);

		const int height = scores.size[2];
		const int width = scores.size[3];
		for (int y = 0; y < height; ++y) {
			const float* scoresData = scores.ptr<float>(0, 0, y);
			const float* x0_data    = geometry.ptr<float>(0, 0, y);
			const float* x1_data    = geometry.ptr<float>(0, 1, y);
			const float* x2_data    = geometry.ptr<float>(0, 2, y);
			const float* x3_data    = geometry.ptr<float>(0, 3, y);
			const float* anglesData = geometry.ptr<float>(0, 4, y);
			for (int x = 0; x < width; ++x) {
				float score = scoresData[x];
				if (score < confThreshold)
					continue;

				// Decode a prediction.
				// Multiple by 4 because feature maps are 4 time less than input image.
				float offsetX = x * 4.0f, offsetY = y * 4.0f;
				float angle   = anglesData[x];
				float cosA    = std::cos(angle);
				float sinA    = std::sin(angle);
				float h       = x0_data[x] + x2_data[x];
				float w       = x1_data[x] + x3_data[x];

				cv::Point2f offset(offsetX + cosA * x1_data[x] + sinA * x2_data[x], offsetY - sinA * x1_data[x] + cosA * x2_data[x]);
				cv::Point2f p1 = cv::Point2f(-sinA * h, -cosA * h) + offset;
				cv::Point2f p3 = cv::Point2f(-cosA * w, sinA * w) + offset;
				cv::RotatedRect r(0.5f * (p1 + p3), cv::Size2f(w, h), -angle * 180.0f / (float)CV_PI);
				boxes.push_back(r);
				confidences.push_back(score);
			}
		}

		// filter out the false positives
		std::vector<int> indices;
		cv::dnn::NMSBoxes(boxes, confidences, confThreshold, nonMaxThreshold, indices); 

		//
		// Render detections
		//

		// initialize tesseract
		tesseract::TessBaseAPI* ocr = new tesseract::TessBaseAPI();

		std::string path = System::ConvertWstringToString(System::GetApplicationDirectory()) + "\\tessdata";
		if (ocr->Init(path.c_str(), "tur")) {
			throw Error::Exception(L"Can't initialize Tesserract!", L"Image Processing Error");
		}
		
		// set page seg mode
		ocr->SetPageSegMode(tesseract::PSM_SINGLE_WORD);
		
		// convert image to gray scale for proper text recognition
		cv::Mat greyImage;
		cv::cvtColor(mImage, greyImage, cv::COLOR_BGR2GRAY);

		// set image
		ocr->SetImage(greyImage.data, (int)greyImage.cols, (int)greyImage.rows, 1, (int)greyImage.step);

		//convert BGR to ABGR
		cv::cvtColor(mImage, mImage, cv::COLOR_BGR2BGRA);

		std::wfstream file{ mImageFileFullPath+L"_words.txt", std::ios::out | std::ios::trunc};
		file.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>()));

		cv::Point2f ratio((float)mImage.cols / inputScale, (float)mImage.rows / inputScale);
		for (size_t i = 0; i < indices.size(); ++i) {
			cv::RotatedRect& box = boxes[indices[i]];

			// set box 
			int minX{ std::numeric_limits<int>::max() }, minY{ std::numeric_limits<int>::max() };
			int maxX{ std::numeric_limits<int>::min() }, maxY{ std::numeric_limits<int>::min() };
			cv::Point2f vertices[4];
			box.points(vertices);
			for (int j = 0; j < 4; ++j) {
				vertices[j].x *= ratio.x;
				vertices[j].y *= ratio.y;

				//calculate bounding box of the rotated rect
				if (vertices[j].x < minX) {
					minX = (int)vertices[j].x;
				}

				if (vertices[j].x > maxX) {
					maxX = (int)std::roundf(vertices[j].x);
				}

				if (vertices[j].y < minY) {
					minY = (int)vertices[j].y;
				}

				if (vertices[j].y > maxY) {
					maxY = (int)std::roundf(vertices[j].y);
				}
			}

			//draw line connecting the vertices
			for (int j = 0; j < 4; ++j) {
				cv::line(mImage, vertices[j], vertices[(j + 1) % 4], { 0 ,255, 0 }, 1, cv::LINE_AA);
			}

			//get text on the rectangle 
			if (minX >= 0 && maxX <= mImage.cols && minY>= 0 && maxY <= mImage.rows) {
				//set rectangle to recognize text
				ocr->SetRectangle(minX, minY, maxX - minX, maxY - minY);
				
				if (ocr->MeanTextConf()) {
					//get text
					char*       text    = ocr->GetUTF8Text();
					std::string wordStr = text;
					delete[] text;
					//convert UTF8 string to wstring
					std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> converter;
					std::wstring word = converter.from_bytes(wordStr);

					file << word + L'\n';
					
					// create text element
					Graphics::Text textElement{ word, fontSize, Graphics::MainFont };

					if (!textElement.compose()) {
						continue;
					}
					
					//create text mat					
					cv::Mat textImage(textElement.getHeight(), textElement.getWidth(), CV_8UC4, textElement.getBits());
					
					// create mask mat (unfortunately somehow OpenCV doesn't process Alpha channel)
					BYTE* maskBits = new BYTE[textElement.getHeight() * textElement.getWidth()];
					BYTE* textBits = textElement.getBits();

					for (int i = 0;i < textElement.getHeight() * textElement.getWidth() * 4;i += 4) {
						if (textBits[i + 3] == 0) { //transparent
							maskBits[i / 4] = 0;
						} else { //not transparent
							maskBits[i / 4] = 1;
						}
					}

					cv::Mat maskMat(textElement.getHeight(), textElement.getWidth(), CV_8UC1, maskBits);

					delete[] maskBits;

					//draw text mat on image 
					int x1{ minX }, y1{ minY - fontSize };
					int x2{ minX + textImage.cols }, y2{ minY - fontSize + textImage.rows };

					//check bounds (make sure textImage fits in the processed Image)
					if (x1 < 0) {
						x1 = 0;
					}

					if (x2 > mImage.cols) {
						x2 = mImage.cols;
					}

					if (y1 < 0) {
						y1 = 0;
					}

					if (y2 > mImage.rows) {
						y2 = mImage.rows;
					}

					//crop text image in case bounds are changed
					textImage = textImage(cv::Range(0, y2 - y1), cv::Range(0, x2 - x1));

					//crop mask Mat in case bounds are changed
					maskMat = maskMat(cv::Range(0, y2 - y1), cv::Range(0, x2 - x1));

					// copy text image to processed image using the mask
					textImage.copyTo(mImage.rowRange(y1, y2).colRange(x1, x2), maskMat);

					textImage.release();
				}
			}
		}
		
		file.close();

		// Destroy tesseract object and release memory
		greyImage.release();
		ocr->End();
		delete ocr;

		return true;
	} catch (Error::Exception& ex) {
		Error::ShowError(ex.getErrorMessage(), ex.getErrorTitle());

		return false;
	} catch (std::exception& ex) {
		Error::ShowError(ex.what(), L"Image Processing Error");

		return false;
	}
}

bool Application::Application::ShowImageFile(void)
{
	try {
		//
		// Resize Image to fit screen
		//

		// get screen resolution
		RECT resolution = System::GetScreenResolution();

		// check if scaling is necessary
		if (mImage.cols > resolution.right || mImage.rows > resolution.bottom) {
			// get ratios
			float imageRatio  = (float)mImage.cols / (float)mImage.rows;
			float screenRatio = (float)resolution.right / (float)resolution.bottom;

			//calculate new width and height
			int newWidth{ 0 }, newHeight{ 0 };

			if (screenRatio > imageRatio) {
				newWidth  = (int)((float)mImage.cols * (float)resolution.bottom / (float)mImage.rows);
				newHeight = resolution.bottom;
			} else {
				newWidth  = resolution.right;
				newHeight = (int)((float)mImage.rows * (float)resolution.right / (float)mImage.cols);
			}

			// resize image
			cv::resize(mImage, mImage, { newWidth, newHeight });
		}

		//
		// Show image
		//
		cv::destroyAllWindows();
		cv::imshow("Katip", mImage);

		return true;
	} catch (Error::Exception& ex) {
		Error::ShowError(ex.getErrorMessage(), ex.getErrorTitle());

		return false;
	} catch (std::exception& ex) {
		Error::ShowError(ex.what(), L"Image Processing Error");

		return false;
	}
}

LRESULT CALLBACK Application::Application::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
		case WM_KEYUP: //key pressed and released?
			if (wParam == VK_ESCAPE) { //escape key pressed and released?
				SendMessage(hwnd, WM_CLOSE, 0, 0); //send close window message
			}
		break;

		case WM_COMMAND:
			if (HIWORD(wParam) == BN_CLICKED) { //button clicked?
				if ((HWND)lParam == mCloseWindowBtn->getHwnd()) { // is this close button?
					SendMessage(mCloseWindowBtn->getParentHwnd(), WM_CLOSE, 0, 0);
				} else if ((HWND)lParam == mOpenImageBtn->getHwnd()) { //is this open image button?
					int inputScale = CheckAndReturnScaleInput();
					if (inputScale) {
						int fontSize = CheckAndReturnFontSize();
						if (fontSize) {
							if (OpenAndDecodeImageFile()) {
								if (ProcessImageFile(inputScale, fontSize)) {
									ShowImageFile();
								}
							}
						}
						
					}
				}
			}

			if (HIWORD(wParam) == EN_UPDATE) { //control text changed?
				if (mAlgoCalcInputEditBox != nullptr && (HWND)lParam == mAlgoCalcInputEditBox->getHwnd()) { //is this algorithm calculation input edit box
					wchar_t buff[25];
					GetWindowText(mAlgoCalcInputEditBox->getHwnd(), buff, 25);
						
					mAlgoCalcInputEditBox->setText(buff);
				}

				if (mFontSizeEditBox != nullptr && (HWND)lParam == mFontSizeEditBox->getHwnd()) { //is this font size edit box
					wchar_t buff[25];
					GetWindowText(mFontSizeEditBox->getHwnd(), buff, 25);

					mFontSizeEditBox->setText(buff);
				}
			}
		break;

		case WM_CLOSE: //close message sent?
			//ask if user wants to close the window?
			if (GUI::ShowQuestionBox(L"Are you sure you want to exit?", L"Exit Program") == IDYES) {
				Quit();
			}

			return 0;
		break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}