#include "system.hpp"
#include "graphics.hpp"
#include "error.hpp"
#include "gui.hpp"
#include <algorithm>
#include <cmath>

//
// Global variables
//
FT_Library      Graphics::FontLibrary;
Graphics::Font* Graphics::MainFont = nullptr;

//
// Font class member functions
//

Graphics::Font::Font() : mFace()
{}

Graphics::Font::Font(const std::string& fileName, const std::string& path) :
	mFace()
{
	load(fileName, path);
}

Graphics::Font::Font(const Font& font) :
	mFace(font.mFace)
{ }

Graphics::Font::~Font()
{
	reset();
}

Graphics::Font& Graphics::Font::operator=(const Font& font)
{
	reset();

	mFace = font.mFace;

	return *this;
}

BYTE* Graphics::Font::createGlyphBits(wchar_t chr, int size, int &x, int &y, int &width, int &height, int& xAdvance)
{
	BYTE* bits{ nullptr };

	try {
		UINT dpi = GetDpiForWindow(GetDesktopWindow());
		if (FT_Set_Char_Size(mFace, 0, size * 64, dpi, dpi)) {
			throw Error::Exception(L"Can't sent the font char size!", L"Create font char bitmap error");
		}

		if (FT_Set_Pixel_Sizes(mFace, 0, size)) {
			throw Error::Exception(L"Can't set font pixel size!", L"Create font char bitmap error");
		}

		if (FT_Load_Char(mFace, (FT_ULong)chr, FT_LOAD_RENDER)) {
			return nullptr;
		}

		//set fill color
		Pixel fillColor;

		fillColor.mAlpha = 255;
		fillColor.mRed   = 255;
		fillColor.mGreen = 0;
		fillColor.mBlue  = 0;

		// check if char is space, determine xAdvance using TEXT_SPACE_FACTOR
		if (chr == L' ') {
			width    = 1;
			height   = 1;
			x        = 0;
			y        = 0;
			xAdvance = size / TEXT_SPACE_FACTOR;

			bits = new BYTE[width * height * 4]{ 0 };

			return bits;
		}

		//determine dimensions and characteristics
		width    = mFace->glyph->bitmap.width;
		height   = mFace->glyph->bitmap.rows;
		x        = mFace->glyph->bitmap_left;
		y        = mFace->glyph->bitmap_top;
		xAdvance = mFace->glyph->metrics.horiAdvance / 64;

		if (width == 0 || height == 0) {
			return nullptr;
		}

		// create glpyh bits
		bits = new BYTE[width * height * 4]{ 0 };
		int index{ 0 };
		for (int j = 0; j < height; ++j) {
			for (int k = 0; k < width; ++k) {
				index = j * width * 4 + k * 4;
				if (mFace->glyph->bitmap.buffer[j * width + k] < 128) {
					bits[index + 0] = 0;
					bits[index + 1] = 0;
					bits[index + 2] = 0;
					bits[index + 3] = 0;
				}
				else {
					bits[index + 0] = fillColor.mBlue;
					bits[index + 1] = fillColor.mGreen;
					bits[index + 2] = fillColor.mRed;
					bits[index + 3] = fillColor.mAlpha;
				}
			}
		}

		// all good! return bits
		return bits;
	} catch (Error::Exception& ex) {
		Error::ShowError(ex.getErrorMessage(), ex.getErrorTitle());

		if (bits) {
			delete bits;
			bits = nullptr;
		}

		return bits;
	} catch (std::exception& ex) {
		Error::ShowError(ex.what(), L"Create font char bitmap error");

		if (bits) {
			delete bits;
			bits = nullptr;
		}

		return bits;
	}
}

bool Graphics::Font::load(const std::string& fileName, const std::string& path)
{
	try {
		if (!fileName.empty()) {
			//get appdir and load font
			std::string appDir = System::ConvertWstringToString(System::GetApplicationDirectory());
			std::string file = path.empty() ? appDir + '\\' + fileName : (path + '\\' + fileName); //if path is empty font is in the application directory

			FT_Error error = FT_New_Face(Graphics::FontLibrary, file.c_str(), 0, &mFace);

			if (error == FT_Err_Unknown_File_Format) {
				throw Error::Exception(L"Font file format can't be recognized!", L"Font Load Error");
			} else if (error) {
				throw Error::Exception(L"Can't open the font file!", L"Font Load Error");
			}

			return true;
		} else {
			throw Error::Exception(L"Font filename is empty!", L"Font Load Error");
		}
	} catch (Error::Exception& ex) {
		Error::ShowError(ex.getErrorMessage(), ex.getErrorTitle());

		return false;
	} catch (std::exception& ex) {
		Error::ShowError(ex.what(), L"Font Load Error");

		return false;
	}
}

void Graphics::Font::reset(void)
{
	//FT_Done_Face(mFace);
}

//
// Graphics Element Class Member Functions
//
Graphics::GraphicsElement::GraphicsElement() : 
	mWidth(0), mHeight(0), mBits(nullptr)
{}

Graphics::GraphicsElement::GraphicsElement(const BYTE* bits, const int width, const int height) :
	mWidth(width), mHeight(height), mBits(nullptr)
{
	if (bits) {
		if (!setBits(bits)) {
			reset();
		}
	}
}

Graphics::GraphicsElement::GraphicsElement(const GraphicsElement& element) :
	mWidth(element.mWidth), mHeight(element.mHeight), mBits(nullptr)
{
	if (!setBits(element.mBits)) {
		reset();
	}
}

Graphics::GraphicsElement::~GraphicsElement()
{
	reset();
}

const Graphics::GraphicsElement& Graphics::GraphicsElement::operator=(const GraphicsElement& element)
{
	mWidth  = element.mWidth;
	mHeight = element.mHeight;
	mBits   = nullptr;

	if (!setBits(element.mBits)) {
		reset();
	}

	return *this;
}

bool Graphics::GraphicsElement::operator==(const GraphicsElement& element) const
{
	if (element.mWidth == mWidth && element.mHeight == mHeight) {
		for (int i = 0; i < mWidth * mHeight * 4; ++i) {
			if (element.mBits[i] != mBits[i]) {
				return false;
			}
		}
	} else {
		return false;
	}

	return true;
}

bool Graphics::GraphicsElement::operator!=(const GraphicsElement& element) const
{
	return !(*this == element);
}

int Graphics::GraphicsElement::getWidth(void) const
{
	return mWidth;
}

void Graphics::GraphicsElement::setWidth(const int width)
{
	mWidth = width;
}

int Graphics::GraphicsElement::getHeight(void) const
{
	return mHeight;
}

void Graphics::GraphicsElement::setHeight(const int height)
{
	mHeight = height;
}

BYTE* Graphics::GraphicsElement::getBits(void) const
{
	return mBits;
}

bool Graphics::GraphicsElement::setBits(const BYTE* bits)
{
	mBits = SetBits(bits, mWidth, mHeight);

	if (mBits == nullptr) {
		reset();

		return false;
	} else {
		return true;
	}
}

void Graphics::GraphicsElement::reset(void)
{
	mWidth  = 0;
	mHeight = 0;

	if (mBits) {
		delete[] mBits;
		mBits = nullptr;
	}
}

//
// Text Class Member Functions
//

Graphics::Text::Text() :
	GraphicsElement(), mText(), mSize(0), mFont(nullptr)
{}

Graphics::Text::Text(const std::wstring& text, const int size, Graphics::Font* font) :
	GraphicsElement(nullptr, 0, 0), mText(text), mSize(size), mFont(font)
{}

Graphics::Text::Text(const Text& text) :
	GraphicsElement(text.mBits, text.mWidth, text.mHeight), mText(text.mText), mSize(text.mSize), mFont(text.mFont)
{}

Graphics::Text::~Text()
{
	reset();
}

Graphics::Text::operator std::wstring()
{
	return std::wstring(mText);
}

const Graphics::Text& Graphics::Text::operator=(const Text& text)
{
	mText   = text.mText;
	mWidth  = text.mWidth;
	mHeight = text.mHeight;
	mSize   = text.mSize;

	if (!setBits(text.mBits)) {
		reset();
	}

	return *this;
}

Graphics::Text Graphics::Text::operator+(const std::wstring& text) const
{
	return Graphics::Text(mText + text, mHeight, mFont);
}

const Graphics::Text& Graphics::Text::operator+=(const std::wstring& text)
{
	mText += text;

	if (!compose()) {
		reset();
	}

	return *this;
}

const std::wstring& Graphics::Text::getText(void) const
{
	return mText;
}

void Graphics::Text::setText(const std::wstring& text)
{
	mText = text;
}

int Graphics::Text::getSize(void) const
{
	return mSize;
}

void Graphics::Text::setSize(const int size)
{
	mSize = size;
}

bool Graphics::Text::compose(void)
{
	BYTE* finalBits{ nullptr };

	try {
		std::vector<BYTE*> bitsVec;      // each characters bits vector
		std::vector<RECT>  bitsRectsVec; // each characters x, y coords and width, height dimensions vector
		std::vector<int>   bitsRowsVec;  // each characters row number vector (row that char belongs)
		std::vector<int>   bitsXAdvVec;  // each characters x advance vector

		//
		// calculate each character coords and dimensions, set bits, determine the row it belongs
		//
		int   width{ 0 };      // width of the char
		int   height{ 0 };     // height of the char
		int   x{ 0 };          // x coord of the char
		int   y{ 0 };          // y coord of the char
		int   xAdvance{ 0 };   // horizontal advance of the char
		BYTE* bits{ nullptr }; // bits of the char
		int   curRow{ 0 };     // current row number
		int   rowWidth{ 0 };   // width of the current row
		int   maxWidth{ 0 };   // width of the widest row
		for (size_t i = 0; i < mText.size(); ++i) {
			if (mText[i] != L'\n') { // same line
				bits = mFont->createGlyphBits(mText[i], mSize, x, y, width, height, xAdvance);

				rowWidth += xAdvance;
				if (rowWidth > maxWidth) {
					maxWidth = rowWidth;
				}
			} else { // new line
				++curRow;
				rowWidth = 0;
				continue;
			}

			if (bits) { // if bits are set
				bitsVec.push_back(bits);
				bitsRectsVec.push_back({ x, y, x + width, y + height });
				bitsRowsVec.push_back(curRow);
				bitsXAdvVec.push_back(xAdvance);
			} else { // if bits are not set (skip character)
				continue;
			}
		}

		//
		// Compose lines
		//
		int lineHeight = (int)std::roundf((float)mSize * TEXT_LINE_HEIGHT_FACTOR);   // height of the each line
		int baseLine   = (int)std::roundf((float)mSize * TEXT_LINE_BASELINE_FACTOR); // base line of the current line

		mWidth  = maxWidth;
		mHeight = (curRow + 1) * (lineHeight);

		finalBits = new BYTE[mWidth * mHeight * 4]{ 0 };

		int curX{ 0 };                       // current x coord on the current line
		int curY{ 0 };                       // current y coord on the current line
		int xSrc{ 0 }, ySrc{ 0 };            // x coord of the source, y coord of the source
		int xDes{ 0 }, yDes{ 0 };            // x coord on the destination, y coord on the destination 
		int bitsWidth{ 0 }, bitsHeight{ 0 }; // width and height of the char
		int indexSrc{ 0 }, indexDes{ 0 };    // index of the source, index of the destination
		for (size_t i = 0; i < bitsVec.size(); ++i) {
			if (i != 0 && bitsRowsVec[i] > bitsRowsVec[i - 1]) { //jump to new line
				curX = 0;
			}
			curY       = (bitsRowsVec[i] * lineHeight) + lineHeight - baseLine; //determine current y on the line
			bitsWidth  = bitsRectsVec[i].right  - bitsRectsVec[i].left;
			bitsHeight = bitsRectsVec[i].bottom - bitsRectsVec[i].top;
			for (yDes = curY - bitsRectsVec[i].top, ySrc = 0; ySrc < bitsHeight; ++yDes, ++ySrc) {
				for (xSrc = 0, xDes = curX + bitsRectsVec[i].left; xSrc < bitsWidth; ++xSrc, ++xDes) {
					indexSrc = ySrc * bitsWidth * 4 + xSrc * 4; // determine source index
					indexDes = yDes * mWidth * 4 + xDes * 4;    // determine destination index(relative to current line)

					if (indexDes >= 0 && indexDes <= mWidth * mHeight * 4 - 4) { // check boundaries
						finalBits[indexDes + 0] = bitsVec[i][indexSrc + 0]; 
						finalBits[indexDes + 1] = bitsVec[i][indexSrc + 1]; 
						finalBits[indexDes + 2] = bitsVec[i][indexSrc + 2]; 
						finalBits[indexDes + 3] = bitsVec[i][indexSrc + 3];
					}
				}
			}
			curX += bitsXAdvVec[i];

			delete[] bitsVec[i];
		}

		//
		// Set bits of composed lines
		//
		if (!setBits(finalBits)) {
			delete[] finalBits;

			reset();

			return false;
		}

		// it's all good! return true
		return true;
	} catch (Error::Exception& ex) {
		Error::ShowError(ex.getErrorMessage(), ex.getErrorTitle());

		if (finalBits) {
			delete[] finalBits;
		}

		return false;
	} catch (std::exception& ex) {
		Error::ShowError(ex.what(), L"Text Generation Error");

		if (finalBits) {
			delete[] finalBits;
		}

		return false;
	}
}

void Graphics::Text::reset(void)
{
	GraphicsElement::reset();

	mText.clear();
	mSize = 0;
}

BYTE* Graphics::SetBits(const BYTE* bits, const int width, const int height)
{
	BYTE* newBits{ nullptr };

	try {
		//check if bits are valid
		if (bits == nullptr) {
			throw Error::Exception(L"Can't set the bits!", L"Set Bits Error");
		}

		if (width == 0 || height == 0) {
			throw Error::Exception(L"Width and Height is not set!", L"Set Bits Error");
		}

		// create new bits and copy input bits
		newBits = new BYTE[width * height * 4]{ 0 };
		std::copy(bits, bits + width * height * 4, newBits);

		// return bits
		return newBits;
	} catch (Error::Exception& ex) {
		if (newBits) {
			delete[] newBits;
			newBits = nullptr;
		}

		Error::ShowError(ex.getErrorMessage(), ex.getErrorTitle());

		return newBits;
	} catch (std::exception& ex) {
		if (newBits) {
			delete[] newBits;
			newBits = nullptr;
		}

		Error::ShowError(std::string(ex.what()), L"Set Bits Error");

		return newBits;
	}
}