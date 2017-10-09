#ifndef _STRING_EX
#define _STRING_EX

//#include <memory.h>
//#include <stdlib.h>
//#include <stdio.h>
#include "List.h"

namespace CoreLibrary
{
	class GenericStringList;

	class GenericString
	{
	public:
		GenericString();
		GenericString(long len);
		GenericString(const GenericString &obj);
		GenericString(const GenericString *ptr);
		GenericString(const char* ptr);
		GenericString(const wchar_t* ptr);
		GenericString(const char* ptr, int startpos, int endpos);
		~GenericString();

		long length() const;
		void clear();
		void assign(const char *ptr);
		void assign(const char *ptr, int startpos, int endpos);
		const char* buffer() const;
		int indexOf(const GenericString &obj, int startpos = 0) const;
		int indexOf(const char* ptr, int startpos = 0) const;
		int indexOf(const char ch, int startpos = 0) const;
		void getSubString(int pos, int len, GenericString &substr) const;

		int getInt(int pos, int len) const;
		long getLong(int pos, int len) const;
		long long getLongLong(int pos, int len) const;
		unsigned int getUnsignedInt(int pos, int len) const;
		unsigned long getUnsignedLong(int pos, int len) const;
		unsigned long long getUnsignedLongLong(int pos, int len) const;
		double getDouble(int pos, int len) const;

		int getInt() const;
		long getLong() const;
		long long getLongLong() const;
		unsigned int getUnsignedInt() const;
		unsigned long getUnsignedLong() const;
		unsigned long long getUnsignedLongLong() const;
		double getDouble() const;

		int countOf(const GenericString &obj) const;
		int countOf(const char* ptr) const;
		int countOf(const char ch) const;

		void toLower();
		void toUpper();
		void leftTrim();
		void rightTrim();
		void trim();
		void reverse();
		void reverse(int start, int len);

		void replace(const GenericString &oldpattern, const GenericString &newpattern);
		void replace(const char oldchar, const char newchar);
		void replace(const GenericString &oldpattern, const long npattern);
		void replace(const GenericString &oldpattern, const double npattern);
		void replace(const GenericString &oldpattern, const int npattern);

		void remove(const GenericString &oldpattern);
		void remove(const char oldchar);
		void removeAll(const char oldchar);
		bool removeFirst(const char oldchar);
		void removeAt(int pos, int len);

		void getKeyValuePair(GenericString &key, GenericString &value, const char delimiter);
		void getKeyValuePair(GenericString &key, GenericString &value, const GenericString &delimiter);

		void getSubStringList(List &tokens, const char delimiter = ' ');
		void getSubStringList(List &tokens, const GenericString &delimiter = " ");

		char getAt(const int atpos) const;
		void SetAt(const int atpos, const char ch);

		unsigned char* fromBase64();
		char* toBase64();
		wchar_t* toWideCharacter();
		static long length(const char* ptr);

		static void join(List &tokens, GenericString &newString, const char delimiter = ' ');
		static void join(List &tokens, GenericString &newString, const GenericString &delimiter = " ");

		void operator=(const GenericString& other);
		bool operator!=(const GenericString& other);
		bool operator==(const GenericString& other);
		bool operator>(const GenericString& other);
		bool operator<(const GenericString& other);

		GenericString& operator+=(const GenericString& other);
		GenericString& operator+=(const char& other);
		GenericString& operator+=(const int& other);
		GenericString& operator+=(const long& other);
		GenericString& operator+=(const double& other);

		GenericString operator+(const GenericString& other);
		GenericString operator+(const char& other);
		GenericString operator+(const int& other);
		GenericString operator+(const long& other);
		GenericString operator+(const double& other);

		char& operator[](const long index);
	private:
		char*	_Buffer;
		long	_BufferLen;
		long	_StringLen;
	};
}

#endif
