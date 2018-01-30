#ifndef _CHARACTER
#define _CHARACTER

namespace CoreLibrary
{
	class Character
	{
	public:
		Character();
		Character(char ch);

		bool isSpace();
		bool isNumber();
		bool isAlphabet();
		bool isUpper();
		bool isLower();
		bool isSpecial();

		static bool isSpace(char ch);
		static bool isNumber(char ch);
		static bool isAlphabet(char ch);
		static bool isUpper(char ch);
		static bool isLower(char ch);
		static bool isSpecial(char ch);

	private:
		char _Char;
	};
}

#endif