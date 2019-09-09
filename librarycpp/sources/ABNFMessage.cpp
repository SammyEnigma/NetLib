#include "ABNFMessage.hpp"
#include "StringEx.hpp"
#include <stdio.h>

#if defined(_WIN32) || defined(WIN32) || defined (_WIN64) || defined (WIN64)
    #include <memory.h>
#else
    #include <string.h>
#endif

namespace CoreLib
{
	ABNFMessage::ABNFMessage()
	{
		_Header.clear();
		_Body.clear();
		_HasContent = false;
		_Request.clear();
		_URL.clear();
		_Protocol.clear();
		_Version.clear();
		_ResponseText.clear();
		_ResponseCode = -1;
		_MessageType = Request;
		headers.clear();
	}

	void ABNFMessage::reset()
	{
	}

	ABNFMessage::~ABNFMessage()
	{
	}

	void ABNFMessage::setHeader(const std::string& buffer)
	{
		reset();
		_Header = buffer;
	}

	void ABNFMessage::setBody(const std::string& buffer)
	{
		_Body = buffer;
	}

	const std::string&	ABNFMessage::getRequest()
	{
		return _Request;
	}

	const std::string&	ABNFMessage::getProtocol()
	{
		return _Protocol;
	}

	const std::string&	ABNFMessage::getURL()
	{
		return _URL;
	}

	const std::string&	ABNFMessage::getVersion()
	{
		return _Version;
	}

	const std::string&	ABNFMessage::getResponseText()
	{
		return _ResponseText;
	}

	long ABNFMessage::getResponseCode()
	{
		return _ResponseCode;
	}

	long ABNFMessage::getMessageType()
	{
		return _MessageType;
	}

	const std::string&	ABNFMessage::getContent()
	{
		return _Body;
	}

	size_t ABNFMessage::getContentSize()
	{
		return _Body.length();
	}


	const std::string& ABNFMessage::getHeader(const std::string& fieldName)
	{
		return headers[fieldName];
	}

	bool ABNFMessage::deSerialize()
	{
		std::string fieldValueParams;
		std::string field, value;

		std::vector<std::string> lines;

		strsplit(_Header, lines, "\r\n");

		size_t line_count = 0;

		for (auto line : lines)
		{
			if (line_count == 0)
			{
				decodeMessageIdentificationLine(line);
				continue;
			}

			strsplit(line, ':', field, value);
			stralltrim(field);
			stralltrim(value);

			headers[field] = value;

			if (field == "Content-Length")
			{
				_HasContent = true;
			}

			line_count++;
		}

		return true;
	}

	bool ABNFMessage::hasBody()
	{
		return _HasContent;
	}

	void ABNFMessage::decodeMessageIdentificationLine(const std::string& requestLine)
	{
		_Request.clear();
		_URL.clear();
		_Version.clear();
		_Protocol.clear();
		_ResponseCode = -1;
		_ResponseText.clear();
		_MessageType = Request;

		int ws = 0;
		std::string token1, token2, token3;

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

		if (token1.find('/') == std::string::npos)
		{
			_Request = token1;
			_URL = token2;
			strsplit(token3, '/', _Protocol, _Version);
			_MessageType = Request;
			return;
		}
		else
		{
			strsplit(token1, '/', _Protocol, _Version);
			_ResponseCode = strextractint(token2);
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
			sprintf(tempBuffer, "%s/%s %d %s\r\n", _Protocol.c_str(), _Version.c_str(), _ResponseCode, _ResponseText.c_str());
		}
		else
		{
			sprintf(tempBuffer, "%s %s %s/%s\r\n", _Request.c_str(), _URL.c_str(), _Protocol.c_str(), _Version.c_str());
		}
		_MessageLine = tempBuffer;
	}

	void ABNFMessage::setProtocolInformation(const std::string& request, const std::string& URL, const std::string& protocol, const std::string& version)
	{
		_MessageType = Request;
		headers.clear();
		_Protocol = protocol;
		_Version = version;
		_Request = request;
		_URL = URL;
	}

	void ABNFMessage::setProtocolInformation(const std::string& protocol, const std::string& version, long responsecode, const std::string& responsetext)
	{
		_MessageType = Response;
		headers.clear();
		_Protocol = protocol;
		_Version = version;
		_ResponseCode = responsecode;
		_ResponseText = responsetext;
	}

	void ABNFMessage::addHeader(const std::string& field, const std::string& value)
	{
		headers[field] = value;
	}

	void ABNFMessage::serialize(std::string &abnfString)
	{
		encodeMessageIdentificationLine();

		abnfString.clear();

		for (auto kv : headers)
		{
			abnfString += kv.first;
			abnfString += ": ";
			abnfString += kv.second;
			abnfString += "\r\n";
		}

		abnfString += "\r\n\r\n";
	}
}
