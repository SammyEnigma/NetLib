#include "Variant.hpp"
#include <memory.h>

namespace CoreLibrary
{
	Variant::Variant()
	{
		_DataType = Void;
		memset((void*)&_RawBuffer[0], 0, 256);
		_DataSize = 0;
	}

	Variant::Variant(const Variant& other)
	{
		_DataType = other._DataType;
		memset((void*)&_RawBuffer[0], 0, 256);
		memcpy(_RawBuffer, other._RawBuffer, other._DataSize);
		_DataSize = other._DataSize;
	}

	Variant::~Variant()
	{
	}

	Variant::Variant(const char val)
	{
		_DataType = Char;
		_RawBuffer[0] = val;
		_RawBuffer[1] = 0;
		_DataSize = sizeof(char);
	}

	Variant::Variant(const unsigned char val)
	{
		_DataType = UnsignedChar;
		_RawBuffer[0] = val;
		_RawBuffer[1] = 0;
		_DataSize = sizeof(unsigned char);
	}

	Variant::Variant(const GenericString &val)
	{
		int sz = val.length();

		if (sz > 255)
		{
			sz = 255;
		}

		_DataType = String;
		memset((void*)&_RawBuffer[0], 0, 256);
		memcpy((char*)&_RawBuffer[0], val.buffer(), sz);
		_DataSize = sz;
	}

	Variant::Variant(const Buffer &val)
	{
		int sz = val.length();

		if (sz > 255)
		{
			sz = 255;
		}
		_DataType = Raw;
		memset((void*)&_RawBuffer[0], 0, 256);
		memcpy(_RawBuffer, val.buffer(), sz);
		_DataSize = sz;
	}

	Variant::Variant(const bool val)
	{
		_DataType = Boolean;
		memset((void*)&_RawBuffer[0], 0, 256);
		memcpy(_RawBuffer, &val, sizeof(bool));
		_DataSize = sizeof(bool);
	}

	Variant::Variant(const long val)
	{
		_DataType = Number;
		memset((void*)&_RawBuffer[0], 0, 256);
		memcpy(_RawBuffer, &val, sizeof(long));
		_DataSize = sizeof(long);
	}

	Variant::Variant(const unsigned long val)
	{
		_DataType = UnsignedNumber;
		memset((void*)&_RawBuffer[0], 0, 256);
		memcpy(_RawBuffer, &val, sizeof(unsigned long));
		_DataSize = sizeof(unsigned long);
	}

	Variant::Variant(const double val)
	{
		_DataType = Decimal;
		memset((void*)&_RawBuffer[0], 0, 256);
		memcpy(_RawBuffer, &val, sizeof(double));
		_DataSize = sizeof(double);
	}

	Variant::Variant(const DateTime &val)
	{
		long long t = val.getTime();
		_DataType = DateTimeStamp;
		_DataSize = sizeof(long long);
		memset((void*)&_RawBuffer[0], 0, 256);
		memcpy(_RawBuffer, &t, _DataSize);
	}

	VariantType Variant::getType()
	{
		return _DataType;
	}

	void Variant::setType(VariantType vtype)
	{
		_DataType = vtype;
	}

	unsigned int Variant::getSize()
	{
		return _DataSize;
	}

	const void *Variant::getData()
	{
		return &_RawBuffer[0];
	}

	void Variant::setData(const char val)
	{
		_DataType = Char;
		_RawBuffer[0] = val;
		_RawBuffer[1] = 0;
		_DataSize = sizeof(char);
	}

	void Variant::setData(const unsigned char val)
	{
		_DataType = UnsignedChar;
		_RawBuffer[0] = val;
		_RawBuffer[1] = 0;
		_DataSize = sizeof(unsigned char);
	}

	void Variant::setData(const GenericString &val)
	{
		int sz = val.length();

		if (sz > 255)
		{
			sz = 255;
		}

		_DataType = String;
		memset((void*)&_RawBuffer[0], 0, 256);
		memcpy((char*)&_RawBuffer[0], val.buffer(), sz);
		_DataSize = sz;
	}

	void Variant::setData(const Buffer &val)
	{
		int sz = val.length();

		if (sz > 255)
		{
			sz = 255;
		}
		_DataType = Raw;
		memset((void*)&_RawBuffer[0], 0, 256);
		memcpy(_RawBuffer, val.buffer(), sz);
		_DataSize = sz;
	}

	void Variant::setData(const bool val)
	{
		_DataType = Boolean;
		memset((void*)&_RawBuffer[0], 0, 256);
		memcpy(_RawBuffer, &val, sizeof(bool));
		_DataSize = sizeof(bool);
	}

	void Variant::setData(const long val)
	{
		_DataType = Number;
		memset((void*)&_RawBuffer[0], 0, 256);
		memcpy(_RawBuffer, &val, sizeof(long));
		_DataSize = sizeof(long);
	}

	void Variant::setData(const unsigned long val)
	{
		_DataType = UnsignedNumber;
		memset((void*)&_RawBuffer[0], 0, 256);
		memcpy(_RawBuffer, &val, sizeof(unsigned long));
		_DataSize = sizeof(unsigned long);
	}

	void Variant::setData(const double val)
	{
		_DataType = Decimal;
		memset((void*)&_RawBuffer[0], 0, 256);
		memcpy(_RawBuffer, &val, sizeof(double));
		_DataSize = sizeof(double);
	}

	void Variant::setData(const DateTime &val)
	{
		long long t = val.getTime();
		_DataType = DateTimeStamp;
		_DataSize = sizeof(long long);
		memset((void*)&_RawBuffer[0], 0, 256);
		memcpy(_RawBuffer, &t, _DataSize);
	}

	void Variant::getString(GenericString &str)
	{
		str.clear();

		char ptr[255];
		memset((void*)&ptr[0], 0, 255);

		switch (_DataType)
		{
		case Char:
		{
			str += (char)_RawBuffer[0];
			break;
		}
		case UnsignedChar:
		{
			str += (char)_RawBuffer[0];
			break;
		}
		case String:
		{
			str += (char*)&_RawBuffer[0];
			break;
		}
		case Boolean:
		{
			if (getBoolean())
			{
				str = "true";
			}
			else
			{
				str = "false";
			}
			break;
		}
		case Number:
		{
			str += getSignedNumber();
			break;
		}
		case UnsignedNumber:
		{
			// Check !
			str += getSignedNumber();
			break;
		}
		case Decimal:
		{
			str += getReal();
			break;
		}
		case DateTimeStamp:
		{
			DateTime dt = getTimestamp();
			str = dt.getDateString("yyyy/MM/dd hh:mm:ss");
			break;
		}
		case Raw:
		{
			str = (char*)&_RawBuffer[0];
			break;
		}
		case Void:
		{
			str = "";
			break;
		}
		default:
		{
			str = "";
			break;
		}
		}
	}

	long Variant::getSignedNumber()
	{
		if (_DataType == Number)
		{
			long temp;
			memcpy((void*)&temp, (void*)&_RawBuffer[0], sizeof(long));
			return temp;
		}

		return 0;
	}

	unsigned long Variant::getUnsignedNumber()
	{
		if (_DataType == Number)
		{
			unsigned long temp;
			memcpy((void*)&temp, (void*)&_RawBuffer[0], sizeof(unsigned long));
			return temp;
		}

		return 0;
	}

	double Variant::getReal()
	{
		if (_DataType == Decimal)
		{
			double temp;
			memcpy((void*)&temp, (void*)&_RawBuffer[0], sizeof(double));
			return temp;
		}

		return (double)0.0;
	}

	bool Variant::getBoolean()
	{
		if (_DataType == Boolean)
		{
			bool temp;
			memcpy((void*)&temp, (void*)&_RawBuffer[0], sizeof(bool));
			return temp;
		}
		return false;
	}


	DateTime Variant::getTimestamp()
	{
		if (_DataType != DateTimeStamp)
		{
			DateTime ts;
			return ts;
		}

		long long temp;
		memcpy((void*)&temp, (void*)&_RawBuffer[0], sizeof(long long));

		DateTime ts(temp);
		return ts;
	}

	char Variant::getSignedChar()
	{
		if (_DataType == Char || _DataType == String)
		{
			return _RawBuffer[0];
		}

		return '\0';
	}
}
