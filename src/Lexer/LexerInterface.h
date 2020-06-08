#pragma once
#include <string.h>
#include <wtypes.h>

#ifndef LEXERINTERFACE_H
#define LEXERINTERFACE_H

#ifndef ILEXER_H
#include "ILexer.h"
#endif


#ifndef LEXERMODULE_H
#include "LexerModule.h"
#endif

using namespace Scintilla;

extern LexerFactoryFunction GetLexerFactoryByIndex(int index);

extern "C" __declspec(dllexport) inline int __stdcall GetLexerCount()
{
	return 1;
}

extern "C" __declspec(dllexport) inline void __stdcall GetLexerName(unsigned int index, char* name, int buflength)
{
	if (index == 0)
	{
		strncpy_s(name, buflength, "LexerTemplate", _TRUNCATE);
		name[buflength - 1] = '\0';
	}
}

extern "C" __declspec(dllexport) inline void __stdcall GetLexerStatusText(unsigned int index, WCHAR * desc, int buflength)
{
	if (index == 0)
	{
		wcsncpy_s(desc, buflength, L"LexerTemplate file", _TRUNCATE);
		desc[buflength - 1] = L'\0';
	}
}

extern "C" __declspec(dllexport) inline LexerFactoryFunction __stdcall GetLexerFactory(unsigned int index)
{
	return GetLexerFactoryByIndex(index);
}

#endif