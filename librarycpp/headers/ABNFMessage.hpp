#ifndef _ABNF_MESSAGE
#define _ABNF_MESSAGE

#include <string>
#include <map>
#include <unordered_map>

namespace CoreLib
{
	typedef enum PacketType
	{
		Request,
		Response
	}PacketType;

	class ABNFMessage
	{
	public:
		ABNFMessage();
		ABNFMessage(const char* buffer);
		virtual ~ABNFMessage();

		// Incoming packet functions
		void setHeader(const char* buffer);
		bool deSerialize();

		// Outgoing packet functions
		// Request
		void setProtocolInformation(const char* request, const char* URL, const char* protocol, const char* version);
		// Response
		void setProtocolInformation(const char* protocol, const char* version, long responsecode, const char* responsetext);
		void addHeader(const char* field, const char* value);
		void serialize(std::string &abnfString);

		// Common for transmission/reception
		void	attachBody(const char* buffer);

		// Reset all internal data - useful when we reuse the packet
		void reset();

		bool	hasBody();
		const char*	getRequest();
		const char*	getProtocol();
		const char*	getURL();
		const char*	getVersion();
		const char*	getResponseText();
		const char*	getContent();
		long	getResponseCode();
		long	getMessageType();
		void	getFieldValue(const char* fieldName, std::string &value);
		int getContentSize();

	private:
		void decodeMessageIdentificationLine(const char* messageLine);
		void encodeMessageIdentificationLine();
		void processLine(const char* line, std::string &field, std::string &value);
		void getLine(std::string &line);

		std::unordered_map<std::string, std::string> _KeyValueList;

		std::string	_RequestBuffer;
		char*			_Content;
		bool			_HasContent;
		std::string	_Request;
		std::string	_URL;
		std::string	_Protocol;
		std::string	_Version;
		std::string	_ResponseText;
		std::string	_MessageLine;
		long			_ResponseCode;
		PacketType		_MessageType;
		int				_ContentSize;
	};

	typedef ABNFMessage HttpRequest;
	typedef ABNFMessage HttpResponse;
	typedef ABNFMessage SipRequest;
	typedef ABNFMessage SipResponse;
}
#endif
