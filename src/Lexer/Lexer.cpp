#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "Lexer.h"

#include <iostream>
#include <sstream>

// These StyleID values from LexerTemplate.xml
const int LXX_DEFAULT = 0;
const int LXX_BRITISH = 1;
const int LXX_FRENCH = 2;
const int LXX_WORD = 3;

#define DBOUT( s )            \
{                             \
   std::wostringstream os_;    \
   os_ << s;                   \
   OutputDebugStringW( os_.str().c_str() );  \
}

int lexCounter = 0;

WordList BritishCheese;
WordList FrenchCheese;

ILexer4* LexerTemplate::LexerFactory()
{
	return new LexerTemplate();
}

LexerFactoryFunction GetLexerFactoryByIndex(int index)
{
	return (index == 0) ? LexerTemplate::LexerFactory : nullptr;
}

//
// ILexer
//

void SCI_METHOD LexerTemplate::Release()
{
	delete this;
}

int SCI_METHOD LexerTemplate::Version() const
{
	return lvRelease4;
}

Sci_Position SCI_METHOD LexerTemplate::WordListSet(int n, const char* wl) {
	WordList* wordListN = 0;
	switch (n) {
	case 0:
		wordListN = &BritishCheese;
		break;
	case 2:
		wordListN = &FrenchCheese;
		break;

	}
	Sci_Position firstModification = -1;
	if (wordListN) {
		WordList wlNew;
		wlNew.Set(wl);
		if (*wordListN != wlNew) {
			wordListN->Set(wl);
			firstModification = 0;
		}
	}
	return firstModification;
}

void SCI_METHOD LexerTemplate::Lex(Sci_PositionU startPos, Sci_Position lengthDoc, int initStyle, IDocument* pAccess)
{
	// NB: Scintilla::WordList InList functions only support unsigned char

	LexAccessor styler(pAccess);

	StyleContext sc(startPos, lengthDoc, LXX_DEFAULT, styler);

	std::string word;
	Sci_Position wordPos = startPos;

	DBOUT(lexCounter++ << "\t----- Start: " << startPos << ". Style: " << initStyle << "\n");

	// sc.More() checked at loop bottom
	for (; ; sc.Forward()) {
		 
		DBOUT(sc.ch << ": \t" << (char)sc.ch << ((sc.ch == 0) ? "\n\n" : "\n"));

		if (sc.atLineStart)
			sc.state = LXX_DEFAULT;

		if (sc.state == LXX_WORD)
		{
			if (iswalpha(sc.ch))
			{
				word += sc.ch;
			}
			else
			{
				if (sc.ch > 0)
				{
					const char* w = word.c_str();
					if (BritishCheese.InList(w))
					{
						DBOUT(wordPos << "->" << sc.currentPos << " British\n");
						sc.ChangeState(LXX_BRITISH);
					}
					else if (FrenchCheese.InList(w))
					{
						DBOUT(wordPos << "->" << sc.currentPos << " French\n");
						sc.ChangeState(LXX_FRENCH);
					}
					else
					{
						DBOUT(wordPos << "->" << sc.currentPos << " Other\n");
					}
					sc.SetState(LXX_DEFAULT);
				}
			}
		}

		if (sc.state == LXX_DEFAULT) {
			if (iswalpha(sc.ch))
			{
				wordPos = sc.currentPos;
				DBOUT("Word! Start: " << wordPos << "\n");
				sc.SetState(LXX_WORD);
				word = sc.ch;
			}
		}

		if (!sc.More())
			break;
	}

	sc.Complete();
}
