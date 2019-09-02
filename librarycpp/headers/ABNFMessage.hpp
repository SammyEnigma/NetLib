#ifndef _ABNF_MESSAGE
#define _ABNF_MESSAGE

#include "GenericString.hpp"
#include "Map.hpp"

namespace CoreLibrary
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
		void serialize(GenericString &abnfString);

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
		void	getFieldValue(const char* fieldName, GenericString &value);
		int getContentSize();

	private:
		void decodeMessageIdentificationLine(const char* messageLine);
		void encodeMessageIdentificationLine();
		void processLine(const char* line, GenericString &field, GenericString &value);
		void getLine(GenericString &line);

		Map<GenericString, GenericString> _KeyValueList;

		GenericString	_RequestBuffer;
		char*			_Content;
		bool			_HasContent;
		GenericString	_Request;
		GenericString	_URL;
		GenericString	_Protocol;
		GenericString	_Version;
		GenericString	_ResponseText;
		GenericString	_MessageLine;
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
