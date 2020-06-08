#pragma once

//#define WIN32_LEAN_AND_MEAN
//#include <windows.h>
#include <assert.h>

#include "Scintilla.h"
#include "ILexer.h"
#include "LexAccessor.h"
#include "LexerModule.h"
#include "StyleContext.h"
#include "CharacterSet.h"
#include "DefaultLexer.h"
#include "WordList.h"

using namespace Scintilla;

class LexerTemplate : public DefaultLexer
{

public:
	LexerTemplate() {}
	virtual ~LexerTemplate() {}

	static ILexer4* LexerFactory();

	// ILexer
	void	SCI_METHOD Release();
	int 	SCI_METHOD Version() const;
	void	SCI_METHOD Lex(Sci_PositionU startPos, Sci_Position lengthDoc, int initStyle, IDocument* pAccess);
	//void	SCI_METHOD Fold(Sci_PositionU startPos, Sci_Position lengthDoc, int initStyle, IDocument* pAccess);
	//void*	SCI_METHOD PrivateCall(int operation, void* pointer);
	Sci_Position SCI_METHOD WordListSet(int n, const char* wl) override;
};