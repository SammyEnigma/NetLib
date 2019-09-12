#ifndef _ABNF_MESSAGE
#define _ABNF_MESSAGE

#include <string>
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
		virtual ~ABNFMessage();

		// Incoming packet functions
		void setHeader(const std::string& buffer);
		bool deSerialize();

		// Outgoing packet functions
		// Request
		void setProtocolInformation(const std::string& request, const std::string& URL, const std::string& protocol, const std::string& version);
		// Response
		void setProtocolInformation(const std::string& protocol, const std::string& version, long responsecode, const std::string& responsetext);
		void addHeader(const std::string& field, const std::string& value);
		void serialize(std::string &abnfString);

		// Common for transmission/reception
		void	setBody(const std::string& buffer);

		// Reset all internal data - useful when we reuse the packet
		void reset();

		bool	hasBody();
		const std::string&	getRequest();
		const std::string&	getProtocol();
		const std::string&	getURL();
		const std::string&	getVersion();
		const std::string&	getResponseText();
		const std::string&	getContent();
		const std::string&  getHeader(const std::string& fieldName);
		long	getResponseCode();
		long	getMessageType();
		size_t getContentSize();

	private:
		void decodeMessageIdentificationLine(const std::string& messageLine);
		void encodeMessageIdentificationLine();

		std::unordered_map<std::string, std::string> headers;

		std::string	_Header;
		std::string	_Body;
		bool		_HasContent;
		std::string	_Request;
		std::string	_URL;
		std::string	_Protocol;
		std::string	_Version;
		std::string	_ResponseText;
		std::string	_MessageLine;
		long		_ResponseCode;
		PacketType	_MessageType;
	};

	typedef ABNFMessage HttpRequest;
	typedef ABNFMessage HttpResponse;
	typedef ABNFMessage SipRequest;
	typedef ABNFMessage SipResponse;
}
#endif
