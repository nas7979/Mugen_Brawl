#include "DXUT.h"
#include "Utility.h"

void Utility::SplitString(std::vector<std::string>* _result, std::string* _string, char _separator)
{
    int startIndex = 0;
    int length = _string->size();
    for (int i = 0; i < length; i++)
    {
        if ((*_string)[i] == _separator)
        {
            _result->push_back(std::string(&(*_string)[startIndex], i - startIndex));
            startIndex = i + 1;
        }
    }

    _result->push_back(std::string(&(*_string)[startIndex], length - startIndex));
}

int Utility::FindIndexInString(std::string* _string, char _find, int _startIndex, int _searchDir)
{
    for (int i = min(_startIndex, _string->size() - 1); i < _string->size(); i += _searchDir)
    {
        if ((*_string)[i] == _find && (i == 0 || i == _string->size() - 1 || (*_string)[i + _searchDir] != _find))
        {
            return i;
        }
    }

    return -1;
}

std::string Utility::WToA(const std::wstring& _w)
{
    return std::string(_w.begin(), _w.end());
}

std::wstring Utility::AToW(const std::string& _a)
{
    return std::wstring(_a.begin(), _a.end());
}

int Utility::HexToInt(const std::string& _hex)
{
    int result = 0;
    for (int i = 0; i < _hex.size(); i++)
    {
        BYTE hex = _hex[_hex.size() - 1 - i];
        int value = 0;
        if ('0' <= hex && hex <= '9')
            value = hex - '0';
        if ('a' <= hex && hex <= 'f')
            value = hex - 'a' + 10;
        if ('A' <= hex && hex <= 'F')
            value = hex - 'A' + 10;

        result |= value << i * 4;
    }

    return result;
}

static char intToHexMap[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
std::string Utility::IntToHex(int _int)
{
    char result[8];
    for (int i = 0; i < 8; i++)
        result[7 - i] = intToHexMap[_int >> i * 4 & 0xf];

    return std::string(result, 8);
}
