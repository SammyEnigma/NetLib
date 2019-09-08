#ifndef _DATE_TIME
#define _DATE_TIME

#include <string>
#include <time.h>

namespace CoreLib
{
	class DateTime
	{
	public:
		DateTime();
		DateTime(const DateTime& other);
		DateTime(const std::string &str, const std::string &format);
		DateTime(long long tinfo);
		~DateTime();

		void operator=(const DateTime& other);
		bool operator!=(const DateTime& other);
		bool operator==(const DateTime& other);
		bool operator>=(const DateTime& other);
		bool operator<=(const DateTime& other);
		bool operator>(const DateTime& other);
		bool operator<(const DateTime& other);
		DateTime& operator+=(const DateTime& other);
		DateTime operator+(const DateTime& other);
		DateTime& operator-=(const DateTime& other);
		DateTime operator-(const DateTime& other);

		void buildFromString(const std::string &str, const std::string &format);
		void buildFromTime(const time_t &tinfo);

		std::string getDateString(const std::string& format);
		std::string getDateString();
		time_t getTime();

		void addDays(int val);
		void addHours(int val);
		void addMinutes(int val);
		void addSeconds(int val);

		int getDays();
		int getMonths();
		int getYears();
		int getHours();
		int getMinutes();
		int getSeconds();

		void setDay(int val);
		void setMonth(int val);
		void setYear(int val);
		void setHour(int val);
		void setMinute(int val);
		void setSecond(int val);

	private:
		struct tm timeinfo;;
	};
}
#endif

