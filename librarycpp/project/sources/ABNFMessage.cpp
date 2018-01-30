#include "ABNFMessage.hpp"
#include <stdio.h>

#if defined(_WIN32) || defined(WIN32) || defined (_WIN64) || defined (WIN64)
    #include <memory.h>
#else
    #include <string.h>
#endif

namespace CoreLibrary
{
	ABNFMessage::ABNFMessage()
	{
		_RequestBuffer.clear();
		_Content = nullptr;
		_HasContent = false;
		_Request.clear();
		_URL.clear();
		_Protocol.clear();
		_Version.clear();
		_ResponseText.clear();
		_ResponseCode = -1;
		_MessageType = Request;
		_KeyValueList.clear();
		_ContentSize = 0;
	}

	ABNFMessage::ABNFMessage(const char* buffer)
	{
		_RequestBuffer.clear();
		_Content = nullptr;
		_HasContent = false;
		_Request.clear();
		_URL.clear();
		_Protocol.clear();
		_Version.clear();
		_ResponseText.clear();
		_ResponseCode = -1;
		_MessageType = Request;
		_RequestBuffer = buffer;
		_KeyValueList.clear();
		_ContentSize = 0;
	}

	void ABNFMessage::reset()
	{
	}

	ABNFMessage::~ABNFMessage()
	{

		if (_Content != nullptr)
		{
			delete _Content;
			_Content = nullptr;
		}

		_RequestBuffer.clear();
		_HasContent = false;
		_Request.clear();
		_URL.clear();
		_Protocol.clear();
		_Version.clear();
		_ResponseText.clear();
		_ResponseCode = -1;
		_MessageType = Request;
		_KeyValueList.clear();
		_ContentSize = 0;
	}

	void ABNFMessage::setHeader(const char* buffer)
	{
		reset();
		_RequestBuffer = buffer;
	}

	void ABNFMessage::attachBody(const char* buffer)
	{
		if (_Content != nullptr)
		{
			delete _Content;
			_Content = nullptr;
		}

		_Content = new char[_ContentSize];
		memcpy(_Content, buffer, _ContentSize);
	}

	const char*	ABNFMessage::getRequest()
	{
		return _Request.buffer();
	}

	const char*	ABNFMessage::getProtocol()
	{
		return _Protocol.buffer();
	}

	const char*	ABNFMessage::getURL()
	{
		return _URL.buffer();
	}

	const char*	ABNFMessage::getVersion()
	{
		return _Version.buffer();
	}

	const char*	ABNFMessage::getResponseText()
	{
		return _ResponseText.buffer();
	}

	long ABNFMessage::getResponseCode()
	{
		return _ResponseCode;
	}

	long ABNFMessage::getMessageType()
	{
		return _MessageType;
	}

	const char*	ABNFMessage::getContent()
	{
		return _Content;
	}

	int ABNFMessage::getContentSize()
	{
		return _ContentSize;
	}


	void ABNFMessage::getFieldValue(const char* fieldName, GenericString &value)
	{
		char *fieldval = (char*)_KeyValueList.value(fieldName)->buffer();
		if (fieldval == nullptr)
		{
			value.clear();
		}
		else
		{
			value = fieldval;
		}

		return;
	}


	bool ABNFMessage::deSerialize()
	{
		GenericString fieldValueParams;
		GenericString field, value;

		getLine(_MessageLine);
		decodeMessageIdentificationLine(_MessageLine.buffer());

		while (true)
		{
			getLine(fieldValueParams);
			processLine(fieldValueParams.buffer(), field, value);
			if (field.length() < 1)
			{
				break;
			}

			addHeader(field.buffer(), value.buffer());

			if (memcmp(field.buffer(), "Content-Length", 14) == 0)
			{
				if (value.getInt() > 0)
				{
					_HasContent = true;
					_ContentSize = value.getInt();
				}
				else
				{
					_HasContent = false;
				}
				break;;
			}
		}
		return true;
	}

	bool ABNFMessage::hasBody()
	{
		return _HasContent;
	}

	void ABNFMessage::getLine(GenericString &line)
	{
		if (_RequestBuffer.length() < 1)
		{
			line.clear();
			return;
		}

		GenericString next;

		int pos = _RequestBuffer.indexOf("\r\n");

		if (pos == -1)
		{
			line = _RequestBuffer;
			_RequestBuffer.clear();
			return;
		}

		_RequestBuffer.getKeyValuePair(line, next, "\r\n");

		_RequestBuffer = next;
	}

	void ABNFMessage::processLine(const char *line, GenericString &field, GenericString &value)
	{
		GenericString temp(line);

		int delimeterpos = temp.indexOf(':');

		field = "";
		value = "";
		int ctr = 0;
		for (ctr = 0; line[ctr] != 0; ctr++)
		{
			if (ctr < delimeterpos)
			{
				field += line[ctr];
			}

			if (ctr > delimeterpos + 1)
			{
				value += line[ctr];
			}
			if (ctr == delimeterpos)
			{
				continue;
			}
		}
	}

	void ABNFMessage::decodeMessageIdentificationLine(const char* requestLine)
	{
		_Request.clear();
		_URL.clear();
		_Version.clear();
		_Protocol.clear();
		_ResponseCode = -1;
		_ResponseText.clear();
		_MessageType = Request;

		int ws = 0;
		GenericString token1, token2, token3;

		for (int index = 0; requestLine[index] != 0; index++)
		{
			if (requestLine[index] == ' ' || requestLine[index] == '\t')
			{
				ws++;
				continue;
			}
			if (ws > 2)
			{
				break;
			}

			if (ws == 0)
			{
				token1 += requestLine[index];
			}
			if (ws == 1)
			{
				token2 += requestLine[index];
			}
			if (ws == 2)
			{
				token3 += requestLine[index];
			}
		}

		if (token1.indexOf('/') == -1)
		{
			_Request = token1;
			_URL = token2;
			token3.getKeyValuePair(_Protocol, _Version, '/');
			_MessageType = Request;
			return;
		}
		else
		{
			token1.getKeyValuePair(_Protocol, _Version, '/');
			_ResponseCode = token2.getInt();
			_ResponseText = token3;
			_MessageType = Response;
			return;
		}
	}

	void ABNFMessage::encodeMessageIdentificationLine()
	{
		char tempBuffer[1024];
		memset(tempBuffer, 0, 1024);
		if (_MessageType == Response)
		{
			sprintf(tempBuffer, "%s/%s %d %s\r\n", _Protocol.buffer(), _Version.buffer(), _ResponseCode, _ResponseText.buffer());
		}
		else
		{
			sprintf(tempBuffer, "%s %s %s/%s\r\n", _Request.buffer(), _URL.buffer(), _Protocol.buffer(), _Version.buffer());
		}
		_MessageLine = tempBuffer;
	}

	void ABNFMessage::setProtocolInformation(const char* request, const char* URL, const char* protocol, const char* version)
	{
		_MessageType = Request;
		_KeyValueList.clear();
		_Protocol = protocol;
		_Version = version;
		_Request = request;
		_URL = URL;
	}

	void ABNFMessage::setProtocolInformation(const char* protocol, const char* version, long responsecode, const char* responsetext)
	{
		_MessageType = Response;
		_KeyValueList.clear();
		_Protocol = protocol;
		_Version = version;
		_ResponseCode = responsecode;
		_ResponseText = responsetext;
	}

	void ABNFMessage::addHeader(const char* field, const char* value)
	{
		_KeyValueList.insert(field, value);
	}

	void ABNFMessage::serialize(GenericString &abnfString)
	{
		encodeMessageIdentificationLine();

		abnfString.clear();

		int headercount = _KeyValueList.count();

		for (int index = 0; index < headercount; index++)
		{
			abnfString += _KeyValueList.getAt(index);
			abnfString += ": ";
			abnfString += *_KeyValueList.getAt(index);
			abnfString += "\r\n";
		}

		abnfString += "\r\n\r\n";
	}
}
