#ifndef _BASE_64
#define _BASE_64

extern void encodebase64(const unsigned char *data, unsigned long inputlength, char *encodedString, unsigned long *outputlength);
extern void decodebase64(const char *encodedString, unsigned long inputlength, unsigned char *decodedData, unsigned long *outputlength);


#endif
