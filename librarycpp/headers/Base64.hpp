#ifndef _BASE_64
#define _BASE_64

namespace CoreLib
{
	class Base64
	{
	public:
		Base64();
		void encodeBase64(const unsigned char *data, unsigned long inputlength, char *encodedString, unsigned long &outputlength);
		void decodeBase64(const char *encodedString, unsigned long inputlength, unsigned char *decodedData, unsigned long &outputlength);
	};
}
#endif
