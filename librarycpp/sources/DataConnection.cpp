#include "DataConnection.hpp"

#if defined(_WIN32) || defined(WIN32) || defined (_WIN64) || defined (WIN64)
    #include <memory.h>
#else
    #include <string.h>
#endif

namespace CoreLibrary
{
	Database *dataConnection = nullptr;

	BindStruct::BindStruct()
	{
		memset((void*)&_ColumnName[0], 0, 33);
		_ColumnType = Void;
		_Precision = 6;
	}

	void BindStruct::setColumnName(const char* ptr)
	{
		memset((void*)&_ColumnName[0], 0, 33);
		memcpy((char*)&_ColumnName[0], ptr, 32);
	}

	void BindStruct::setColumnType(const VariantType vtype)
	{
		_ColumnType = vtype;
	}

	void BindStruct::setColumnPrecision(const unsigned int prec)
	{
		_Precision = prec;
	}


	const char* BindStruct::getColumnName()
	{
		return (const char*)&_ColumnName[0];
	}

	VariantType BindStruct::getColumnType()
	{
		return _ColumnType;
	}

	unsigned short BindStruct::getColumnPrecision()
	{
		return _Precision;
	}

	ConnectionHandle::ConnectionHandle()
	{

	}

	ConnectionHandle::~ConnectionHandle()
	{

	}

	StatementHandle::StatementHandle()
	{

	}

	StatementHandle::~StatementHandle()
	{

	}

	Column::Column()
	{
		memset((void*)&_ColumnName[0], 0, 64);
	}

	Column::~Column()
	{

	}

	const char *Column::getName()
	{
		return (const char*)&_ColumnName[0];
	}

	long Column::getLength()
	{
		return _ColumnLength;
	}

	long Column::getPosition()
	{
		return _ColumnPosition;
	}

	VariantType Column::getType()
	{
		return _ColumnType;
	}

	unsigned long Column::getNativeType()
	{
		return _NativeType;
	}


	void Column::setName(GenericString val)
	{
		memset((void*)&_ColumnName[0], 0, 64);
		memcpy((void*)&_ColumnName[0], val.buffer(), 63);
	}

	void Column::setLength(long val)
	{
		_ColumnLength = val;
	}

	void Column::setType(VariantType val)
	{
		_ColumnType = val;
	}

	void Column::setPosition(long val)
	{
		_ColumnPosition = val;
	}

	void Column::setNativeType(unsigned long val)
	{
		_NativeType = val;
	}

	void Column::setByteUse(bool buse)
	{
		_ByteUsed = buse;
	}

	bool Column::getByteUse()
	{
		return _ByteUsed;
	}

	int Column::getScale()
	{
		return _Scale;
	}

	int Column::getPrecision()
	{
		return _Precision;
	}

	void Column::setScale(int inscl)
	{
		_Scale = inscl;
	}

	void Column::setPrecision(int prec)
	{
		_Precision = prec;
	}

	void* Column::data()
	{
		return (void*)&_Data[0];
	}


	Database::Database()
	{
		_AutoCommitOn = true;
		_ArraySize = 64;
	}

	Database::~Database()
	{
	}

	GenericString Database::getDatabaseTimeStamp(const GenericString &timestamp, const GenericString &format)
	{
		DateTime tsval;
		tsval.buildFromString(timestamp, format);
		return getNativeTimeStamp(tsval);
	}
}
