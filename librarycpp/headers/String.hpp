#ifndef __STRING
#define __STRING

#include "List.hpp"

namespace CoreLib
{
	class StringList;

	class String
	{
	public:
		String();
		String(long len);
		String(const String &obj);
		String(const String *ptr);
		String(const char* ptr);
		String(const wchar_t* ptr);
		String(const char* ptr, int startpos, int endpos);
		~String();

		long length() const;
		void clear();
		void assign(const char *ptr);
		void assign(const char *ptr, int startpos, int endpos);
		const char* buffer() const;
		int indexOf(const String &obj, int startpos = 0) const;
		int indexOf(const char* ptr, int startpos = 0) const;
		int indexOf(const char ch, int startpos = 0) const;
		void getSubString(int pos, int len, String &substr) const;

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

		int countOf(const String &obj) const;
		int countOf(const char* ptr) const;
		int countOf(const char ch) const;

		void toLower();
		void toUpper();
		void leftTrim();
		void rightTrim();
		void trim();
		void reverse();
		void reverse(int start, int len);

		void replace(const String &oldpattern, const String &newpattern);
		void replace(const char oldchar, const char newchar);
		void replace(const String &oldpattern, const long npattern);
		void replace(const String &oldpattern, const double npattern);
		void replace(const String &oldpattern, const int npattern);

		void remove(const String &oldpattern);
		void remove(const char oldchar);
		void removeAll(const char oldchar);
		bool removeFirst(const char oldchar);
		void removeAt(int pos, int len);

		void getKeyValuePair(String &key, String &value, const char delimiter);
		void getKeyValuePair(String &key, String &value, const String &delimiter);

		void getSubStringList(List<String> &tokens, const char delimiter = ' ');
		void getSubStringList(List<String> &tokens, const String &delimiter = " ");

		char getAt(const int atpos) const;
		void SetAt(const int atpos, const char ch);

		unsigned char* fromBase64();
		char* toBase64();
		wchar_t* toWideCharacter();
		static long length(const char* ptr);

		static void join(List<String> &tokens, String &newString, const char delimiter = ' ');
		static void join(List<String> &tokens, String &newString, const String &delimiter = " ");

		void operator=(const String& other);
		bool operator!=(const String& other);
		bool operator==(const String& other);
		bool operator>(const String& other);
		bool operator<(const String& other);

		String& operator+=(const String& other);
		String& operator+=(const char& other);
		String& operator+=(const int& other);
		String& operator+=(const long& other);
		String& operator+=(const double& other);

		String operator+(const String& other);
		String operator+(const char& other);
		String operator+(const int& other);
		String operator+(const long& other);
		String operator+(const double& other);

		char& operator[](const long index);
	private:
		char*	_Buffer;
		long	_BufferLen;
		long	_StringLen;
	};
}

#endif
