#include "zgui.h"

namespace zgui
{

#pragma warning (push)
#pragma warning (disable: 4514 4996)

zgui_wchar CharacterFunctions::toUpperCase (const zgui_wchar character) throw()
{
    return (zgui_wchar)fn_CharUpperW((LPWSTR)character);
}

zgui_wchar CharacterFunctions::toLowerCase(const zgui_wchar character) throw()
{
    return (zgui_wchar)fn_CharLowerW((LPWSTR)character);
}

bool CharacterFunctions::isUpperCase (const zgui_wchar character) throw()
{
    WORD cType;
    return (bool)(fn_GetStringTypeW(CT_CTYPE1, (LPCWSTR)&character, 1, &cType) && (cType & C1_UPPER));
    //return iswupper((wchar_t)character) != 0;
}

bool CharacterFunctions::isLowerCase (const zgui_wchar character) throw()
{
    WORD cType;
    return (bool)(fn_GetStringTypeW(CT_CTYPE1, (LPCWSTR)&character, 1, &cType) && (cType & C1_LOWER));
    //return iswlower((wchar_t)character) != 0;
}

#pragma warning (pop)

bool CharacterFunctions::isWhitespace (const char character) throw()
{
    return character == ' ' || (character <= 13 && character >= 9);
}

bool CharacterFunctions::isWhitespace (const zgui_wchar character) throw()
{
    //return iswspace ((wchar_t) character) != 0;
    WORD cType;
    return (bool)(fn_GetStringTypeW(CT_CTYPE1, (LPCWSTR)&character, 1, &cType) && (cType & C1_SPACE));
}

bool CharacterFunctions::isDigit (const char character) throw()
{
    return (character >= '0' && character <= '9');
}

bool CharacterFunctions::isDigit (const zgui_wchar character) throw()
{
    //return iswdigit ((wchar_t) character) != 0;
    WORD cType;
    return (bool)(fn_GetStringTypeW(CT_CTYPE1, (LPCWSTR)&character, 1, &cType) && (cType & C1_DIGIT));
}

bool CharacterFunctions::isLetter (const char character) throw()
{
    return (character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z');
}

bool CharacterFunctions::isLetter (const zgui_wchar character) throw()
{
    WORD cType;
    return (bool)(fn_GetStringTypeW(CT_CTYPE1, (LPCWSTR)&character, 1, &cType) && (cType & C1_ALPHA));
    //return iswalpha ((wchar_t) character) != 0;
}

bool CharacterFunctions::isLetterOrDigit(const char character) throw()
{
    return (character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z') || (character >= '0' && character <= '9');
}

bool CharacterFunctions::isLetterOrDigit (const zgui_wchar character) throw()
{
    WORD cType;
    return (bool)(fn_GetStringTypeW(CT_CTYPE1, (LPCWSTR)&character, 1, &cType) && (cType & (C1_DIGIT | C1_ALPHA)));
    //return iswalnum ((wchar_t) character) != 0;
}

int CharacterFunctions::getHexDigitValue (const zgui_wchar digit) throw()
{
    unsigned int d = digit - '0';
    if (d < (unsigned int) 10)
        return (int) d;

    d += (unsigned int) ('0' - 'a');
    if (d < (unsigned int) 6)
        return (int) d + 10;

    d += (unsigned int) ('a' - 'A');
    if (d < (unsigned int) 6)
        return (int) d + 10;

    return -1;
}

}
