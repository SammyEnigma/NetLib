#ifndef _DATA_CONNECTION
#define _DATA_CONNECTION

#include "GenericString.h"
#include "DateTime.h"
#include "Variant.h"

namespace CoreLibrary
{
	const int DB_SUCCESS = 0;
	const int ERR_NULL_CONST_VIOLATION = 901;
	const int ERR_UNIQ_CONST_VIOLATION = 902;
	const int ERR_COLUMN_SIZE_VIOLATION = 903;
	const int ERR_MISSING_COLUMN = 904;
	const int ERR_INVALID_INT = 905;
	const int ERR_INAVLID_SQL_STRING = 906;
	const int ERR_NO_CONNECTION = 907;
	const int ERR_UNKNOWN_DB_ERROR = 999;

	typedef enum DataBaseType
	{
		Oracle = 0,
		PostgreSQL = 1,
		MySql = 2,
		SQLServer = 3,
		DB2 = 4
	}DataBaseType;

	const unsigned int numcolumns = 32;

	const unsigned int charsize = sizeof(char);
	const unsigned int strsize = 256 * sizeof(char);
	const unsigned int datesize = 7 * sizeof(unsigned char);
	const unsigned int intsize = sizeof(long);
	const unsigned int uintsize = sizeof(unsigned long);
	const unsigned int realsize = sizeof(double);

	class BindStruct
	{
	public:
		BindStruct();
		void setColumnName(const char* ptr);
		void setColumnType(const VariantType vtype);
		void setColumnPrecision(const unsigned int prec);

		const char* getColumnName();
		VariantType getColumnType();
		unsigned short getColumnPrecision();
	private:
		char _ColumnName[33];
		VariantType _ColumnType;
		unsigned short _Precision;
	};


	//Forward declarations

	//Local to this file;
	class Database;

	class ConnectionHandle
	{
	public:
		ConnectionHandle();
		virtual ~ConnectionHandle();
		virtual void acquire() = 0;
		virtual void release() = 0;
	};

	class StatementHandle
	{
	public:
		StatementHandle();
		virtual ~StatementHandle();
	};

	class Column
	{
	public:
		Column();
		~Column();

		const char* getName();
		long getLength();
		long getPosition();
		VariantType getType();
		unsigned long getNativeType();
		bool getByteUse();
		int getScale();
		int getPrecision();

		void setName(GenericString val);
		void setLength(long val);
		void setPosition(long val);
		void setType(VariantType val);
		void setNativeType(unsigned long val);
		void setByteUse(bool buse);
		void setScale(int inscl);
		void setPrecision(int prec);
		void* data();

	private:
		VariantType _ColumnType;
		unsigned long _NativeType;
		long    _ColumnLength;
		char _ColumnName[64];
		long    _ColumnPosition;
		bool    _ByteUsed;
		int     _Scale;
		int     _Precision;
		unsigned char _Data[256];
	};

	class Database
	{
	public:
		Database();
		~Database();
		GenericString getDatabaseTimeStamp(const GenericString &timestamp, const GenericString &format);

		virtual bool initialize() = 0;
		virtual bool deinitialize() = 0;
		virtual bool createConnection(const GenericString &dbname, const GenericString &uname, const GenericString &passwd, const GenericString &host, const int &port, GenericString &errmsg, int &errorCode) = 0;
		virtual bool closeConnection() = 0;
		virtual void enableAutoCommit() = 0;
		virtual bool executeDML(const char *sqlstr, GenericString &errmsg, int &errorCode) = 0;
		virtual bool executeSQL(const char *sqlstr, GenericString &errmsg, int &errorCode) = 0;
		virtual Variant* getRow(bool &fetchpending, GenericString &errmsg, int &errorCode) = 0;
		virtual bool startTransaction() = 0;
		virtual bool commit() = 0;
		virtual bool rollback() = 0;
		virtual void getError(GenericString &str, int &errorCode) = 0;
		virtual DataBaseType getDatabaseType() = 0;
		virtual int errorDBToAppType(int errorcode) = 0;
		virtual bool reConnect() = 0;
		virtual ConnectionHandle* getConnectionHandle() = 0;
		virtual StatementHandle* getStatementHandle() = 0;
		virtual VariantType translateToLocalType(unsigned int  _DbType) = 0;
		virtual bool getColumns(GenericString &errmsg, int &errorCode) = 0;
		virtual bool isOpen() = 0;
		virtual GenericString getNativeTimeStamp(DateTime &timestamp) = 0;
		virtual bool setupDateFormat(GenericString errmsg, int &errorCode) = 0;
		virtual void rowLimitClause(GenericString &str) = 0;
		virtual void releaseReadBuffers() = 0;
		virtual bool prepareBind(const char* sqlstring, GenericString &errmsg, int &errorCode) = 0;
		virtual bool bindColumn(BindStruct &bstruct) = 0;
		virtual bool copyValue(Variant &val, int column, int row) = 0;
		virtual void setBulkArraySize(long sz) = 0;
		virtual bool executeBulkDML(GenericString &errmsg, int &errorCode) = 0;
	protected:
		DataBaseType _DbType;
		bool _AutoCommitOn;
		GenericString _DbName;
		GenericString _Username;
		GenericString _Password;
		GenericString _Host;
		int _Port;
		GenericString _PortString;
		unsigned int _ArraySize;
	};
}
#endif

