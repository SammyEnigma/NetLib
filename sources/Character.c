#include "Character.h"

namespace CoreLibrary
{
	Character::Character()
	{
		_Char = 0;
	}

	Character::Character(char ch)
	{
		_Char = ch;
	}

	bool Character::isSpace()
	{
		if (_Char == ' ' || _Char == '\t' || _Char == '\n' || _Char == '\v' || _Char == '\f' || _Char == '\r')
			return true;
		else
			return false;
	}

	bool Character::isNumber()
	{
		if (_Char >= 48 && _Char <= 57)
			return true;
		else
			return false;
	}

	bool Character::isAlphabet()
	{
		return true;
	}

	bool Character::isUpper()
	{
		return true;
	}

	bool Character::isLower()
	{
		return true;
	}

	bool Character::isSpecial()
	{
		return true;
	}

	bool Character::isSpace(char ch)
	{
		if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\v' || ch == '\f' || ch == '\r')
		{
			return true;
		}

		return false;
	}

	bool Character::isNumber(char ch)
	{
		return true;
	}

	bool Character::isAlphabet(char ch)
	{
		return true;
	}

	bool Character::isUpper(char ch)
	{
		return true;
	}

	bool Character::isLower(char ch)
	{
		return true;
	}

	bool Character::isSpecial(char ch)
	{
		return true;
	}
}
