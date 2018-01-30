#ifndef _VARIANT
#define _VARIANT

#include "Buffer.hpp"
#include "GenericString.hpp"
#include "DateTime.hpp"

namespace CoreLibrary
{
	typedef enum VariantType
	{
		Void = 0,
		Char = 1,
		UnsignedChar = 2,
		String = 3,
		Boolean = 4,
		Number = 5,
		UnsignedNumber = 6,
		Decimal = 7,
		DateTimeStamp = 8,
		Raw = 9
	}VariantType;

	class Variant
	{
	public:
		Variant();
		~Variant();
		Variant(const Variant& other);
		Variant(const char val);
		Variant(const unsigned char val);
		Variant(const GenericString &val);
		Variant(const Buffer &val);
		Variant(const bool val);
		Variant(const long val);
		Variant(const unsigned long val);
		Variant(const double val);
		Variant(const DateTime &val);

		const void* getData();
		void getString(GenericString &str);
		long getSignedNumber();
		unsigned long getUnsignedNumber();
		double getReal();
		bool getBoolean();
		DateTime getTimestamp();
		char getSignedChar();

		VariantType getType();
		void setType(VariantType vtype);
		unsigned int getSize();

		void setData(const char val);
		void setData(const unsigned char val);
		void setData(const GenericString &val);
		void setData(const Buffer &val);
		void setData(const bool val);
		void setData(const long val);
		void setData(const unsigned long val);
		void setData(const double val);
		void setData(const DateTime &val);

		VariantType _DataType;
		unsigned char _RawBuffer[256];
		unsigned int _DataSize;
	};
}
#endif

