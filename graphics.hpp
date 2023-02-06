#pragma once

/*
* Image Text Processor Program
*
* "graphics.hpp" by Caner'Trooper'Kurt
*
*
* Graphics Operations
*
* Classes (Pixel, Font, GraphicsElement, Text)
*
* Functions (SetBits)
* 
*/

#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "main.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <vector>

namespace Graphics
{
	//
	// Global Definitions
	//
	constexpr float TEXT_LINE_HEIGHT_FACTOR   = 1.2f;
	constexpr float TEXT_LINE_BASELINE_FACTOR = 0.3f;
	constexpr int   TEXT_SPACE_FACTOR         = 3;
	
	//
	// Classes
	//
	struct Pixel
	{
		BYTE mRed{ 0 };
		BYTE mGreen{ 0 };
		BYTE mBlue{ 0 };
		BYTE mAlpha{ 0 };
	};

	class Font
	{
		public:

			Font();
			Font(const std::string& fileName, const std::string& path = std::string{});
			Font(const Font& font);
			~Font();


			Font& operator=(const Font& font);

			/**
				Creates glyph bits of the char (RGBA)

				[in]  chr      - the wide char to create the glyph(make sure font supports that character)
				[in]  size     - size of the glyph in pixels
				[out] x        - x coordinate of the character on the glyph
				[out] y        - y coordinate of the character on the glpyh
				[out] width    - width of the glyph
				[out] height   - height of the glyph
				[out] xAdvance - advance on the horizontal direction of the glyph

				returns RGBA BYTEs of the glyph
			*/
			BYTE* createGlyphBits(wchar_t chr, int size, int& x, int& y, int& width, int& height, int& xAdvance);
			/**
			   Loads font file from the hard drive

			   [in] fileName - name of the font file
			   [in] path     - full path of the folder that contains the font file

			   returns true on success
			*/
			bool load(const std::string& fileName, const std::string& path = std::string{});
			/**
				Resets font
			*/
			void  reset(void);

		private:

			FT_Face mFace; // Font face
	};

	class GraphicsElement
	{
		public:

			GraphicsElement();
			GraphicsElement(const BYTE* bits, const int width, const int height);
			GraphicsElement(const GraphicsElement& element);
			virtual ~GraphicsElement();


			virtual const GraphicsElement& operator=(const GraphicsElement& element);
			virtual bool                   operator==(const GraphicsElement& element) const; //compares by the bits
			virtual bool                   operator!=(const GraphicsElement& element) const; //compares by the bits


			virtual int  getWidth(void) const;
			virtual void setWidth(const int width);

			virtual int  getHeight(void) const;
			virtual void setHeight(const int height);

			virtual BYTE* getBits(void) const;
			virtual bool  setBits(const BYTE* bits);

			/**
				Resets the graphics element
			*/
			virtual void reset(void);

		protected:

			int   mWidth;  // Width of the graphics element
			int   mHeight; // Height of the graphics element
			BYTE* mBits;   // BYTEs of the graphics element
	};

	class Text : public GraphicsElement
	{
		public:

			Text();
			Text(const std::wstring& text, const int size, Font* font);
			Text(const Text& text);
			virtual ~Text();


			operator std::wstring();
			const Text& operator=(const Text& text);
			Text        operator+(const std::wstring& text) const;
			const Text& operator+=(const std::wstring& text);


			const std::wstring& getText(void) const;
			void                setText(const std::wstring& text);

			int  getSize(void) const;
			void setSize(const int size);

			/**
				Composes text element with the text, size and font 
			*/
			bool compose(void);
			/**
				Resets all members of the text element
			*/
			void reset(void);

		private:

			std::wstring mText; //Wide string of the text
			int          mSize; //Char size
			Font*        mFont; //Font of the text

	};

	/**
		Sets new bits and return them (this function is shared with all graphics elements)

		[in] bits   - bits to copy
		[in] width  - width of the input matrix
		[in] height - height of the input matrix

		returna copied bits (new)
	*/
	BYTE* SetBits(const BYTE* bits, const int width, const int height);

	//
	// Global variables
	//
	extern FT_Library      FontLibrary; //Font library of the application
	extern Graphics::Font* MainFont;    //Main Font of the application
}

#endif