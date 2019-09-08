#include "String.hpp"
#include "Base64.hpp"
#include "Character.hpp"
//#include <memory.h>
//#include <stdlib.h>
//#include <stdio.h>

namespace CoreLib
{
#if defined(_WIN32) || defined(WIN32) || defined (_WIN64) || defined (WIN64)
#define strtoull(str, endptr, base) _strtoui64(str, endptr, base)
#endif

	const long PAGE_SIZE = 1024 * sizeof(void*);

	int numPages(int x);
	int stringLen(const char *ptr);
	int stringCompare(const char *dest, const char*orig, int len);
	void stringReset(char *ptr, int len);
	void stringCopy(char *dest, const char*orig, int len);
	void stringMove(char *dest, const char*orig, int len);

	void convertToString(char*ptr, long long num);
	void convertToString(char*ptr, double num);

	int convertToInt(const char* str);
	long convertToLong(const char* str);
	long long convertToLongLong(const char* str);
	double convertToLDouble(const char* str);
	
	int searchForSubstring(char *pat, char *txt);
	void computeLPSArray(char *pat, int M, int *lps);

	String::String()
	{
		_Buffer = nullptr;
		_BufferLen = 0;
		_StringLen = 0;

		_BufferLen = PAGE_SIZE;
		
		_Buffer = new char[_BufferLen];
		stringReset(_Buffer,_BufferLen);
	}

	String::String(long len)
	{
		_StringLen = len;
		_BufferLen = numPages(_StringLen + 1)*PAGE_SIZE;
		_Buffer = new char[_BufferLen];
		stringReset(_Buffer, _BufferLen);
	}

	String::String(const String &obj)
	{
		if (obj._StringLen > 0 && obj._Buffer != nullptr)
		{
			_StringLen = obj._StringLen;
			_BufferLen = obj._BufferLen;
			_Buffer = new char[_BufferLen];
			stringReset(_Buffer, _BufferLen);
			stringCopy(_Buffer, obj._Buffer, _StringLen);
		}
	}

	String::String(const String *ptr)
	{
		if (ptr != nullptr)
		{
			if (ptr->_StringLen > 0 && ptr->_Buffer != nullptr)
			{
				_StringLen = ptr->_StringLen;
				_BufferLen = ptr->_BufferLen;
				_Buffer = new char[_BufferLen];
				stringReset(_Buffer, _BufferLen);
				stringCopy(_Buffer, ptr->_Buffer, _StringLen);
			}
		}
	}

	String::String(const wchar_t* ptr)
	{
		if (ptr != nullptr)
		{
			for (_StringLen = 0; ptr[_StringLen] != '\0'; _StringLen++) {}

			_BufferLen = numPages(_StringLen + 1)*PAGE_SIZE;
			_Buffer = new char[_BufferLen];
			stringReset(_Buffer, _BufferLen);

			for (int idx = 0; idx < _StringLen; idx++)
			{
				_Buffer[idx] = ptr[idx];
			}
		}
	}

	String::String(const char* ptr)
	{
		if (ptr != nullptr)
		{
			_StringLen = stringLen(ptr);
			_BufferLen = numPages(_StringLen + 1)*PAGE_SIZE;
			_Buffer = new char[_BufferLen];
			stringReset(_Buffer, _BufferLen);
			stringCopy(_Buffer, ptr, _StringLen);
		}
	}

	String::String(const char* ptr, int startpos, int endpos)
	{
		if (ptr != nullptr)
		{
			_StringLen = endpos - startpos;
			_BufferLen = numPages(_StringLen + 1)*PAGE_SIZE;
			_Buffer = new char[_BufferLen];
			stringReset(_Buffer, _BufferLen);
			stringCopy(_Buffer, &ptr[startpos], _StringLen);
		}
	}

	String::~String()
	{
		if (_Buffer != nullptr)
		{
			delete[]_Buffer;
			_Buffer = nullptr;
		}

		_BufferLen = 0;
		_StringLen = 0;
	}

	long String::length(const char* ptr)
	{
		return stringLen(ptr);
	}

	bool String::operator!=(const String& other)
	{
		int complen = (_StringLen > other._StringLen) ? other._StringLen : _StringLen;

		if (stringCompare(_Buffer, other._Buffer, complen) != 0)
		{
			return true;
		}

		return false;
	}

	bool String::operator==(const String& other)
	{
		int complen = (_StringLen > other._StringLen) ? other._StringLen : _StringLen;

		if (stringCompare(_Buffer, other._Buffer, complen) == 0)
		{
			return true;
		}

		return false;
	}

	bool String::operator>(const String& other)
	{
		int complen = (_StringLen > other._StringLen) ? other._StringLen : _StringLen;

		if (stringCompare(_Buffer, other._Buffer, complen) > 0)
		{
			return true;
		}

		return false;
	}

	bool String::operator<(const String& other)
	{
		int complen = (_StringLen > other._StringLen) ? other._StringLen : _StringLen;

		if (stringCompare(_Buffer, other._Buffer, complen) < 0)
		{
			return true;
		}

		return false;
	}

	void String::operator=(const String& other)
	{
		clear();
		_StringLen = other._StringLen;
		_BufferLen = other._BufferLen;
		_Buffer = new char[_BufferLen];
		stringReset(_Buffer, _BufferLen);
		stringCopy(_Buffer, other._Buffer, _StringLen);
		_Buffer[_StringLen] = 0;

		return;
	}

	String& String::operator+=(const String& other)
	{
		int len = _StringLen + other._StringLen;

		if (len < _BufferLen)
		{
			stringCopy(_Buffer + _StringLen, other._Buffer, other._StringLen);
			_StringLen = len;
		}
		else
		{
			int bufferlen = numPages(len + 1)*PAGE_SIZE;
			char *buffer = new char[bufferlen];
			stringReset(buffer, bufferlen);
			stringCopy(buffer, _Buffer, _StringLen);
			stringCopy(buffer + _StringLen, other._Buffer, other._StringLen);

			_StringLen = len;
			_BufferLen = bufferlen;
			delete[]_Buffer;
			_Buffer = buffer;
		}

		return *this;
	}

	String& String::operator+=(const char& other)
	{
		int len = _StringLen + 1;

		if (len < _BufferLen)
		{
			_Buffer[_StringLen] = other;
			_StringLen++;
			_Buffer[_StringLen] = '\0';
		}
		else
		{
			int bufferlen = numPages(len + 1)*PAGE_SIZE;
			char *buffer = new char[bufferlen];
			stringReset(buffer, bufferlen);
			stringCopy(buffer, _Buffer, _StringLen);
			_Buffer[_StringLen] = other;

			_StringLen++;
			_BufferLen = bufferlen;
			delete[]_Buffer;
			_Buffer = buffer;
		}

		return *this;
	}

	String& String::operator+=(const long& other)
	{
		char tempbuffer[32] = { 0 };
		convertToString(&tempbuffer[0], (long long)other);

		int len = _StringLen + stringLen(tempbuffer);

		if (len < _BufferLen)
		{
			stringCopy(_Buffer + _StringLen, tempbuffer, stringLen(tempbuffer));
			_StringLen = len;
		}
		else
		{
			int bufferlen = numPages(len + 1)*PAGE_SIZE;
			char *buffer = new char[bufferlen];
			stringReset(buffer, bufferlen);
			stringCopy(buffer, _Buffer, _StringLen);
			stringCopy(buffer + _StringLen, tempbuffer, stringLen(tempbuffer));

			_StringLen = len;
			_BufferLen = bufferlen;
			delete[]_Buffer;
			_Buffer = buffer;
		}

		return *this;
	}

	String& String::operator+=(const int& other)
	{
		char tempbuffer[32] = { 0 };
		convertToString(&tempbuffer[0], (long long)other);

		int len = _StringLen + stringLen(tempbuffer);

		if (len < _BufferLen)
		{
			stringCopy(_Buffer + _StringLen, tempbuffer, stringLen(tempbuffer));
			_StringLen = len;
		}
		else
		{
			int bufferlen = numPages(len + 1)*PAGE_SIZE;
			char *buffer = new char[bufferlen];
			stringReset(buffer, bufferlen);
			stringCopy(buffer, _Buffer, _StringLen);
			stringCopy(buffer + _StringLen, tempbuffer, stringLen(tempbuffer));

			_StringLen = len;
			_BufferLen = bufferlen;
			delete[]_Buffer;
			_Buffer = buffer;
		}

		return *this;
	}

	String& String::operator+=(const double& other)
	{
		char tempbuffer[32] = { 0 };
		convertToString(&tempbuffer[0], (double)other);

		int len = _StringLen + stringLen(tempbuffer);

		if (len < _BufferLen)
		{
			stringCopy(_Buffer + _StringLen, tempbuffer, stringLen(tempbuffer));
			_StringLen = len;
		}
		else
		{
			int bufferlen = numPages(len + 1)*PAGE_SIZE;
			char *buffer = new char[bufferlen];
			stringReset(buffer, bufferlen);
			stringCopy(buffer, _Buffer, _StringLen);
			stringCopy(buffer + _StringLen, tempbuffer, stringLen(tempbuffer));

			_StringLen = len;
			_BufferLen = bufferlen;
			delete[]_Buffer;
			_Buffer = buffer;
		}

		return *this;
	}

	String String::operator+(const String& other)
	{
		String returnvalue;

		returnvalue._StringLen = _StringLen + other._StringLen;
		returnvalue._BufferLen = numPages(returnvalue._StringLen + 1)*PAGE_SIZE;
		returnvalue._Buffer = new char[returnvalue._BufferLen];
		stringReset(returnvalue._Buffer, returnvalue._BufferLen);
		stringCopy(returnvalue._Buffer, _Buffer, _StringLen);
		stringCopy(returnvalue._Buffer + _StringLen, other._Buffer, other._StringLen);

		return returnvalue;
	}

	String String::operator+(const char& other)
	{
		String returnvalue;

		returnvalue._StringLen = _StringLen + 1;
		returnvalue._BufferLen = numPages(returnvalue._StringLen + 1)*PAGE_SIZE;
		returnvalue._Buffer = new char[returnvalue._BufferLen];
		stringReset(returnvalue._Buffer, returnvalue._BufferLen);
		stringCopy(returnvalue._Buffer, _Buffer, _StringLen);
		returnvalue._Buffer[_StringLen] = other;

		return returnvalue;
	}

	String String::operator+(const long& other)
	{
		char tempbuffer[32] = { 0 };
		convertToString(&tempbuffer[0], (long long)other);

		String returnvalue;

		returnvalue._StringLen = _StringLen + stringLen(tempbuffer);
		returnvalue._BufferLen = numPages(returnvalue._StringLen + 1)*PAGE_SIZE;
		returnvalue._Buffer = new char[returnvalue._BufferLen];
		stringReset(returnvalue._Buffer, returnvalue._BufferLen);
		stringCopy(returnvalue._Buffer, _Buffer, _StringLen);
		stringCopy(returnvalue._Buffer + _StringLen, tempbuffer, stringLen(tempbuffer));

		return returnvalue;
	}

	String String::operator+(const int& other)
	{
		char tempbuffer[32] = { 0 };
		convertToString(&tempbuffer[0], (long long)other);

		String returnvalue;

		returnvalue._StringLen = _StringLen + stringLen(tempbuffer);
		returnvalue._BufferLen = numPages(returnvalue._StringLen + 1)*PAGE_SIZE;
		returnvalue._Buffer = new char[returnvalue._BufferLen];
		stringReset(returnvalue._Buffer, returnvalue._BufferLen);
		stringCopy(returnvalue._Buffer, _Buffer, _StringLen);
		stringCopy(returnvalue._Buffer + _StringLen, tempbuffer, stringLen(tempbuffer));

		return returnvalue;
	}

	String String::operator+(const double& other)
	{
		char tempbuffer[32] = { 0 };
		convertToString(&tempbuffer[0], (double)other);

		String returnvalue;

		returnvalue._StringLen = _StringLen + stringLen(tempbuffer);
		returnvalue._BufferLen = numPages(returnvalue._StringLen + 1)*PAGE_SIZE;
		returnvalue._Buffer = new char[returnvalue._BufferLen];
		stringReset(returnvalue._Buffer, returnvalue._BufferLen);
		stringCopy(returnvalue._Buffer, _Buffer, _StringLen);
		stringCopy(returnvalue._Buffer + _StringLen, tempbuffer, stringLen(tempbuffer));

		return returnvalue;
	}

	char String::getAt(const int atpos) const
	{
		return _Buffer[atpos];
	}

	void String::SetAt(const int atpos, const char ch)
	{
		_Buffer[atpos] = ch;
	}

	char& String::operator[](const long index)
	{
		if (index < 0 || index >(_StringLen - 1))
		{
			throw;
		}

		return _Buffer[index];
	}

	unsigned char * String::fromBase64()
	{
		Base64 b64;

		unsigned char* decodedbuffer = nullptr;
		unsigned long decodedbufferlen = 0;

		b64.decodeBase64((const char*)_Buffer, _StringLen, decodedbuffer, decodedbufferlen);

		return decodedbuffer;
	}

	char* String::toBase64()
	{
		return nullptr;
	}

	wchar_t* String::toWideCharacter()
	{
		return nullptr;
	}

	void String::clear()
	{
		if(_BufferLen > PAGE_SIZE)
		{ 
			if (_Buffer != nullptr)
			{
				_Buffer = nullptr;
			}

			_BufferLen = PAGE_SIZE;
			_Buffer = new char[_BufferLen];
		}

		_StringLen = 0;
		stringReset(_Buffer, _BufferLen);
	}

	void String::assign(const char *ptr)
	{
		if (ptr != nullptr)
		{
			clear();
			_StringLen = stringLen(ptr);
			_BufferLen = numPages(_StringLen + 1)*PAGE_SIZE;
			_Buffer = new char[_BufferLen];
			stringReset(_Buffer, _BufferLen);
			stringCopy(_Buffer, ptr, _StringLen);
		}
	}

	void String::assign(const char *ptr, int startpos, int endpos)
	{
		if (ptr != nullptr)
		{
			clear();
			_StringLen = endpos - startpos;
			_BufferLen = numPages(_StringLen + 1)*PAGE_SIZE;
			_Buffer = new char[_BufferLen];
			stringReset(_Buffer, _BufferLen);
			stringCopy(_Buffer, &ptr[startpos], _StringLen);
		}
	}

	long String::length() const
	{
		return _StringLen;
	}

	const char* String::buffer() const
	{
		return _Buffer;
	}

	int String::indexOf(const String &obj, int startpos) const
	{
		return indexOf(obj._Buffer, startpos);
	}

	int String::indexOf(const char* ptr, int startpos) const
	{
		int result = -1;

		if (ptr != nullptr)
		{

			char* pdest = (char*)searchForSubstring(_Buffer + startpos, (char*)ptr);

			if (pdest == 0)
			{
				return -1;
			}

			result = pdest - _Buffer;
		}

		return result;
	}

	int String::indexOf(const char ch, int startpos) const
	{
		for (int ctr = startpos; _Buffer[ctr] != '\0'; ctr++)
		{
			if (_Buffer[ctr] == ch)
			{
				return ctr;
			}
		}

		return -1;
	}

	void String::getSubString(int pos, int len, String &substr) const
	{
		if (_StringLen - pos < len)
		{
			len = _StringLen - pos;
		}

		char* ptr = new char[len + 1];
		stringReset(ptr, len + 1);
		stringCopy(_Buffer + pos, ptr, len);
		substr.assign(ptr);
		delete[]ptr;
	}

	int String::getInt(int pos, int len) const
	{
		if (_StringLen - pos < len)
		{
			len = _StringLen - pos;
		}

		String str;

		str.assign(_Buffer, pos, pos + len);
		int res = convertToInt(str.buffer());
		return res;
	}

	long String::getLong(int pos, int len) const
	{
		if (_StringLen - pos < len)
		{
			len = _StringLen - pos;
		}

		String str;

		str.assign(_Buffer, pos, pos + len);
		long res = convertToLong(str.buffer());
		return res;
	}

	long long String::getLongLong(int pos, int len) const
	{
		if (_StringLen - pos < len)
		{
			len = _StringLen - pos;
		}

		String str;

		str.assign(_Buffer, pos, pos + len);
		long long res = convertToLongLong(str.buffer());
		return res;
	}

	unsigned int String::getUnsignedInt(int pos, int len) const
	{
		if (_StringLen - pos < len)
		{
			len = _StringLen - pos;
		}

		String str;

		str.assign(_Buffer, pos, pos + len);
		unsigned int res = convertToInt(str.buffer());
		return res;
	}

	unsigned long String::getUnsignedLong(int pos, int len) const
	{
		if (_StringLen - pos < len)
		{
			len = _StringLen - pos;
		}

		String str;

		str.assign(_Buffer, pos, pos + len);
		unsigned long res = convertToLong(str.buffer());
		return res;
	}

	unsigned long long String::getUnsignedLongLong(int pos, int len) const
	{
		if (_StringLen - pos < len)
		{
			len = _StringLen - pos;
		}

		String str;

		str.assign(_Buffer, pos, pos + len);
		unsigned long long res = convertToLongLong(str.buffer());
		return res;
	}


	double String::getDouble(int pos, int len) const
	{
		if (_StringLen - pos < len)
		{
			len = _StringLen - pos;
		}

		String str;

		str.assign(_Buffer, pos, pos + len);
		double res = convertToLDouble(str.buffer());
		return res;
	}

	int String::getInt() const
	{
		return getInt(0, _StringLen);
	}

	long String::getLong() const
	{
		return getLong(0, _StringLen);
	}

	long long String::getLongLong() const
	{
		return getLongLong(0, _StringLen);
	}

	unsigned int String::getUnsignedInt() const
	{
		return getUnsignedInt(0, _StringLen);
	}

	unsigned long String::getUnsignedLong() const
	{
		return getUnsignedLong(0, _StringLen);
	}

	unsigned long long String::getUnsignedLongLong() const
	{
		return getUnsignedLongLong(0, _StringLen);
	}

	double String::getDouble() const
	{
		return getDouble(0, _StringLen);
	}

	int String::countOf(const String &obj) const
	{
		return countOf(obj._Buffer);
	}

	int String::countOf(const char* ptr) const
	{
		int count = 0;
		int pos = 0;
		int offset = stringLen(ptr);

		while (true)
		{
			const char* temp = &ptr[searchForSubstring(&_Buffer[pos], (char*)ptr)];
			if (temp == nullptr)
			{
				break;
			}

			count++;

			pos = (temp - _Buffer) + offset;
		}

		return count;
	}

	int String::countOf(const char ch) const
	{
		int c = 0;
		for (int i = 0; _Buffer[i] != '\0'; i++)
		{
			if (_Buffer[i] == ch)
			{
				c++;
			}
		}

		return c;
	}

	void String::toLower()
	{
		for (int ctr = 0; _Buffer[ctr] != '\0'; ctr++)
		{
			if (_Buffer[ctr] >= 65 && _Buffer[ctr] <= 90)
			{
				_Buffer[ctr] = _Buffer[ctr] + 32;
			}
		}
	}

	void String::toUpper()
	{
		for (int ctr = 0; _Buffer[ctr] != '\0'; ctr++)
		{
			if (_Buffer[ctr] >= 97 && _Buffer[ctr] <= 122)
			{
				_Buffer[ctr] = _Buffer[ctr] - 32;
			}
		}
	}

	void String::leftTrim()
	{
		char *ptr = _Buffer;

		int ctr = 0;

		int trimlen = 0;

		while (Character::isSpace(*ptr))
		{
			ptr++;
			trimlen++;
		}

		while (*ptr)
		{
			_Buffer[ctr] = *ptr;
			ctr++;
			ptr++;
		}
		_Buffer[ctr] = '\0';
		_StringLen -= trimlen;
	}

	void String::rightTrim()
	{
		for (int ctr = _StringLen - 1; ctr > -1; ctr--)
		{
			if (Character::isSpace(_Buffer[ctr]))
			{
				_Buffer[ctr] = '\0';
			}
			else
			{
				break;
			}
		}
	}

	void String::trim()
	{
		rightTrim();
		leftTrim();
	}

	void String::replace(const String &oldpattern, const String &newpattern)
	{
		int substrpos = 0;
		int diff = newpattern._StringLen - oldpattern._StringLen;

		if (diff >= 0)
		{
			int count = countOf(oldpattern);
			int oldlen = _StringLen;

			int newstringlen = count*diff + _StringLen;
			int newbufferlen = numPages(_StringLen + 1)*PAGE_SIZE;

			if (newbufferlen > _BufferLen)
			{
				_BufferLen = newbufferlen;
				char *ptr = new char[_BufferLen];
				stringReset(ptr, _BufferLen);
				stringCopy(ptr, _Buffer, _StringLen);
				delete[]_Buffer;
				_Buffer = ptr;
			}
		}

		while (true)
		{
			substrpos = indexOf(oldpattern);

			if (substrpos == -1)
			{
				break;
			}

			stringMove(_Buffer + substrpos + newpattern._StringLen, _Buffer + substrpos + oldpattern._StringLen, _StringLen + diff);
			stringCopy(_Buffer + substrpos, newpattern._Buffer, newpattern._StringLen);
		}

		_StringLen = stringLen(_Buffer);
	}

	void String::replace(const char oldchar, const char newchar)
	{
		for (int ctr = 0; _Buffer[ctr] != '\0'; ctr++)
		{
			if (_Buffer[ctr] == oldchar)
			{
				_Buffer[ctr] = newchar;
			}
		}
	}

	void String::replace(const String &oldpattern, const long npattern)
	{
		char buffer[32] = { 0 };
		convertToString(&buffer[0], (long long)npattern);
		replace(oldpattern, buffer);
	}

	void String::replace(const String &oldpattern, const double npattern)
	{
		char buffer[32] = { 0 };
		convertToString(&buffer[0], (double)npattern);
		replace(oldpattern, buffer);
	}

	void String::replace(const String &oldpattern, const int npattern)
	{
		char buffer[32] = { 0 };
		convertToString(&buffer[0], (long long)npattern);
		replace(oldpattern, buffer);
	}

	void String::remove(const String &oldpattern)
	{
		int idxToDel = -1;

		while (true)
		{
			idxToDel = indexOf(oldpattern);

			if (idxToDel < 0)
			{
				break;
			}

			stringMove(&_Buffer[idxToDel], &_Buffer[idxToDel + oldpattern._StringLen], _StringLen - idxToDel);
			_Buffer[_StringLen - oldpattern._StringLen] = 0;
			_StringLen = _StringLen - oldpattern._StringLen;
		}
	}

	void String::remove(const char oldchar)
	{
		int idxToDel = -1;

		while (true)
		{
			idxToDel = indexOf(oldchar);

			if (idxToDel < 0)
			{
				break;
			}

			stringMove(&_Buffer[idxToDel], &_Buffer[idxToDel + 1], _StringLen - idxToDel);
			_Buffer[_StringLen - 1] = 0;
			_StringLen--;
		}
	}

	bool String::removeFirst(const char oldchar)
	{
		int ctr = 0;

		bool found = false;

		for (; _Buffer[ctr] != '\0'; ctr++)
		{
			if (_Buffer[ctr] == oldchar)
			{
				found = true;
				break;
			}
		}

		if (found)
		{
			for (; _Buffer[ctr] != '\0'; ctr++)
			{
				_Buffer[ctr] = _Buffer[ctr + 1];
			}
		}

		return found;
	}

	void String::removeAll(const char oldchar)
	{
		while (removeFirst(oldchar)) {}
	}

	void String::removeAt(int pos, int len)
	{
		stringMove(&_Buffer[pos], &_Buffer[pos + len], _StringLen - len);
		_Buffer[_StringLen - len] = 0;
		_StringLen = _StringLen - len;
	}

	void String::getKeyValuePair(String &key, String &value, const char delimiter)
	{
		int pos = indexOf(delimiter);

		if (pos < 0)
			return;

		if ((pos - 0) > 0)
			key.assign(_Buffer, 0, pos);

		if (((_StringLen - 1) - (pos + 1)) > 0)
			value.assign(_Buffer, pos + 1, _StringLen - 1);
	}

	void String::getKeyValuePair(String &key, String &value, const String &delimiter)
	{
		int pos = indexOf(delimiter);

		if (pos > 0)
		{
			key.assign(_Buffer, 0, pos);
			value.assign(_Buffer, pos + delimiter._StringLen, _StringLen);
		}
	}

	void String::getSubStringList(List<String> &tokens, const char delimiter)
	{
		int delimpos = -1;
		int offset = 1;
		int startpos = 0;

		while (true)
		{
			delimpos = indexOf(delimiter, startpos);

			if (delimpos == -1)
			{
				if (_StringLen > startpos)
				{
					tokens.append(String(_Buffer, startpos, _StringLen));
				}

				break;
			}

			if ((delimpos != 0) && (delimpos >= startpos) && (delimpos != (_StringLen - 1)))
			{
				tokens.append(String(_Buffer, startpos, delimpos));
			}

			startpos = delimpos + offset;
		}
	}

	void String::getSubStringList(List<String> &tokens, const String &delimiter)
	{
		int delimpos = -1;
		int offset = delimiter._StringLen;
		int startpos = 0;

		while (true)
		{
			delimpos = indexOf(delimiter, startpos);

			if (delimpos == -1)
			{
				if (_StringLen > startpos)
				{
					tokens.append(String(_Buffer, startpos, _StringLen));
				}

				break;
			}

			if ((delimpos != 0) && (delimpos >= startpos) && (delimpos != (_StringLen - 1)))
			{
				tokens.append(String(_Buffer, startpos, delimpos));
			}

			startpos = delimpos + offset;
		}
	}

	void String::join(List<String> &tokens, String &newString, const char delimiter)
	{
		int count = tokens.count();

		for (int idx = 0; idx < count; idx++)
		{
			newString += tokens.getAt(idx);

			if (idx < count - 1)
			{
				newString += delimiter;
			}
		}
	}

	void String::join(List<String> &tokens, String &newString, const String &delimiter)
	{
		int count = tokens.count();

		for (int idx = 0; idx < count; idx++)
		{
			newString += tokens.getAt(idx);

			if (idx < count - 1)
			{
				newString += delimiter;
			}
		}
	}

	void String::reverse()
	{
		reverse(0, _StringLen);
	}

	void String::reverse(int start, int len)
	{
		int term = len - 1;

		while (start < term)
		{
			char temp = _Buffer[start];
			_Buffer[start] = _Buffer[term];
			_Buffer[term] = temp;
			start++;
			term--;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int numPages(int x)
	{
		int d = x / PAGE_SIZE;
		int m = x % PAGE_SIZE;

		if (m != 0)
		{
			m = 1;
		}

		return (d + m);
	}

	int stringLen(const char *ptr)
	{
		char *p = (char*)ptr;
		int s = 0;
		while (*p++) s++;
		return s;
	}

	int stringCompare(const char *dest, const char*orig, int len)
	{
		return 0;
	}

	void stringReset(char *ptr, int len)
	{
		for (int ctr = 0; ctr < len; ctr++)
		{
			ptr[ctr] = 0;
		}
	}

	void stringCopy(char *dest, const char*orig, int len)
	{
		for (int ctr = 0; ctr < len; ctr++)
		{
			dest[ctr] = orig[ctr];
		}
	}

	void stringMove(char *dest, const char*orig, int len)
	{
		//for (int ctr = 0; ctr < len; ctr++)
		//{
		//	dest[ctr] = orig[ctr];
		//}
	}

	void convertToString(char*ptr, long long num)
	{
		long long sign = 1;
		long long remainder = 1;
		long long dividend = num;
		long ctr = 0;

		if (num < 1)
		{
			sign = -1;
			dividend = dividend*-1;
		}

		while (dividend)
		{
			remainder = dividend % 10;
			dividend = dividend / 10;

			ptr[ctr] = remainder + 48;
			ctr++;
		}

		if (sign < 1)
		{
			ptr[ctr] = '-';
		}
		else
		{
			ctr--;
		}


		long long start = 0;

		while (start < ctr)
		{
			char temp = ptr[start];
			ptr[start] = ptr[ctr];
			ptr[ctr] = temp;
			start++;
			ctr--;
		}
	}

	void convertToString(char*ptr, double num)
	{

	}

	int convertToInt(const char* str)
	{
		int sign = 0;
		int index = 0;
		int result = 0;

		if (str == nullptr)
		{
			return 0;
		}

		if (stringLen(str) < 2)
		{
			return 0;
		}

		if (str[0] == '-')
		{
			sign = -1;
			index = 1;
		}
		else
		{
			sign = 1;
			index = 0;
		}

		for (; str[index] != '\0'; index++)
		{
			if (!Character::isNumber(str[index]))
			{
				break;
			}

			result = result * 10 + str[index] - '0';
		}

		return result*sign;
	}

	long convertToLong(const char* str)
	{
		long sign = 0;
		long index = 0;
		long result = 0;

		if (str == nullptr)
		{
			return 0;
		}

		if (stringLen(str) < 2)
		{
			return 0;
		}

		if (str[0] == '-')
		{
			sign = -1;
			index = 1;
		}
		else
		{
			sign = 1;
			index = 0;
		}

		for (; str[index] != '\0'; index++)
		{
			if (!Character::isNumber(str[index]))
			{
				break;
			}

			result = result * 10 + str[index] - '0';
		}

		return result*sign;
	}

	long long convertToLongLong(const char* str)
	{
		long long sign = 0;
		long long index = 0;
		long long result = 0;

		if (str == nullptr)
		{
			return 0;
		}

		if (stringLen(str) < 2)
		{
			return 0;
		}

		if (str[0] == '-')
		{
			sign = -1;
			index = 1;
		}
		else
		{
			sign = 1;
			index = 0;
		}

		for (; str[index] != '\0'; index++)
		{
			if (!Character::isNumber(str[index]))
			{
				break;
			}

			result = result * 10 + str[index] - '0';
		}

		return result*sign;
	}

	double convertToLDouble(const char* str)
	{
		return 0.0;
	}

	int searchForSubstring(char *pat, char *txt)
	{
		int M = stringLen(pat);
		int N = stringLen(txt);

		// create lps[] that will hold the longest prefix suffix
		// values for pattern
		int* lps = new int[M];

		// Preprocess the pattern (calculate lps[] array)
		computeLPSArray(pat, M, lps);

		int i = 0;  // index for txt[]
		int j = 0;  // index for pat[]
		while (i < N)
		{
			if (pat[j] == txt[i])
			{
				j++;
				i++;
			}

			if (j == M)
			{
				return (i - j);
				//j = lps[j - 1];
			}

			// mismatch after j matches
			else if (i < N && pat[j] != txt[i])
			{
				// Do not match lps[0..lps[j-1]] characters,
				// they will match anyway
				if (j != 0)
					j = lps[j - 1];
				else
					i = i + 1;
			}
		}
	}

	// Fills lps[] for given patttern pat[0..M-1]
	void computeLPSArray(char *pat, int M, int *lps)
	{
		// length of the previous longest prefix suffix
		int len = 0;

		lps[0] = 0; // lps[0] is always 0

					// the loop calculates lps[i] for i = 1 to M-1
		int i = 1;
		while (i < M)
		{
			if (pat[i] == pat[len])
			{
				len++;
				lps[i] = len;
				i++;
			}
			else // (pat[i] != pat[len])
			{
				// This is tricky. Consider the example.
				// AAACAAAA and i = 7. The idea is similar 
				// to search step.
				if (len != 0)
				{
					len = lps[len - 1];

					// Also, note that we do not increment
					// i here
				}
				else // if (len == 0)
				{
					lps[i] = 0;
					i++;
				}
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
