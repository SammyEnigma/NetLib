#ifndef _DATA_CONNECTION
#define _DATA_CONNECTION

#include <string>
#include <chrono>
#include <variant>
#include <vector>
#include "VariantType.hpp"

namespace CoreLib
{
	typedef std::variant<void*, char, unsigned char, std::string, bool, long, unsigned long, double, std::chrono::system_clock::time_point, unsigned char*> SQLData;

	typedef enum DataBaseResponse
	{
		DB_SUCCESS,
		ERR_NULL_CONST_VIOLATION,
		ERR_UNIQ_CONST_VIOLATION,
		ERR_COLUMN_SIZE_VIOLATION,
		ERR_MISSING_COLUMN,
		ERR_INVALID_INT,
		ERR_INAVLID_SQL_STRING,
		ERR_NO_CONNECTION,
		ERR_UNKNOWN_DB_ERROR
	}DataBaseResponse;

	typedef enum DataBaseType
	{
		Oracle = 0,
		PostgreSQL = 1,
		MySql = 2,
		SQLServer = 3,
		SQLite = 4
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

		void setName(std::string& val);
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
		std::string getDatabaseTimeStamp(const std::string &timestamp, const std::string &format);

		virtual bool initialize() = 0;
		virtual bool deinitialize() = 0;
		virtual bool createConnection(const std::string &dbname, const std::string &uname, const std::string &passwd, const std::string &host, const int &port, std::string &errmsg, int &errorCode) = 0;
		virtual bool createConnection(const std::string& dbfilename, std::string& errmsg, int& errorCode) = 0;
		virtual bool closeConnection() = 0;
		virtual void enableAutoCommit() = 0;
		virtual bool executeDML(const char *sqlstr, std::string &errmsg, int &errorCode) = 0;
		virtual bool executeSQL(const char *sqlstr, std::string &errmsg, int &errorCode) = 0;
		virtual std::vector<SQLData> getRow(bool &fetchpending, std::string &errmsg, int &errorCode) = 0;
		virtual bool startTransaction() = 0;
		virtual bool commit() = 0;
		virtual bool rollback() = 0;
		virtual void getError(std::string &str, int &errorCode) = 0;
		virtual DataBaseType getDatabaseType() = 0;
		virtual int errorDBToAppType(int errorcode) = 0;
		virtual bool reConnect() = 0;
		virtual ConnectionHandle* getConnectionHandle() = 0;
		virtual StatementHandle* getStatementHandle() = 0;
		virtual VariantType translateToLocalType(unsigned int  _DbType) = 0;
		virtual bool getColumns(std::string &errmsg, int &errorCode) = 0;
		virtual bool isOpen() = 0;
		virtual std::string getNativeTimeStamp(std::chrono::system_clock::time_point& timestamp) = 0;
		virtual bool setupDateFormat(std::string errmsg, int &errorCode) = 0;
		virtual void rowLimitClause(std::string &str) = 0;
		virtual void releaseReadBuffers() = 0;
		virtual bool prepareBind(const char* sqlstring, std::string &errmsg, int &errorCode) = 0;
		virtual bool bindColumn(BindStruct &bstruct) = 0;
		virtual bool copyValue(SQLData &val, int column, int row) = 0;
		virtual void setBulkArraySize(long sz) = 0;
		virtual bool executeBulkDML(std::string &errmsg, int &errorCode) = 0;
	protected:
		DataBaseType _DbType;
		bool _AutoCommitOn;
		std::string _DbName;
		std::string _Username;
		std::string _Password;
		std::string _Host;
		std::string _DbFileName;
		int _Port;
		std::string _PortString;
		size_t _ArraySize;
	};
}
#endif

