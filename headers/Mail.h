#ifndef _MAIL
#define _MAIL

#include "GenericString.h"
#include "DateTime.h"
#include "Base64.h"
#include "List.h"
#include "Buffer.h"
#include "File.h"

namespace CoreLibrary
{
	typedef enum MailTextEncoding
	{
		QuotedPrintable,
		Base64
	}MailTextEncoding;

	typedef enum MultiPartType
	{
		PlainText,
		Html,
		InlineImage,
		Attachment
	}MultiPartType;

	class MimeNode
	{
	public:
		GenericString NodeName;
		GenericString Data;
		MultiPartType NodeType;
		MailTextEncoding TextEncoding;
	};

	class MailBody
	{
	public:

		MailBody();
		MailBody(const MailBody &other);
		~MailBody();

		void operator=(const MailBody& other);

		void setMessage(GenericString message);
		void setMessageId(GenericString msgid);

		void attachFile(GenericString filename);

		GenericString* message();
		List<GenericString>* attachments();
		GenericString* messageId();

		void serialize(GenericString &str);
		void deSerialize();

		bool hasMimeData();

		List<MimeNode>* mimeNodes();

	private:
		void parseAttachments();
		void parseTexts();
		void extractAttachment(int &ctr);
		void extractText(int &ctr);

		List<GenericString> _AttachementList;
		GenericString _Message;
		GenericString _MessageId;

		List<GenericString> _MimeData;
		List<MimeNode> _ParsedMimeData;

	};

	/*------------------------------------------------------*/

	class MailHeader
	{
	public:

		MailHeader();
		MailHeader(const MailHeader &other);
		~MailHeader();

		void operator=(const MailHeader& other);

		void addtoToList(GenericString receipient, bool overwrite = false);
		void addtoCcList(GenericString receipient, bool overwrite = false);
		void addtoBccList(GenericString receipient, bool overwrite = false);

		void setSubject(GenericString subject);
		void setFrom(GenericString from);
		void setMessageId(GenericString msgid);
		void setTimeStamp(GenericString dt);
		void setDirection(GenericString dir);
		void setStatus(GenericString st);

		void toList(List<GenericString> &lst);
		void ccList(List<GenericString> &lst);
		void bccList(List<GenericString> &lst);

		GenericString* to();
		GenericString* cc();
		GenericString* bcc();

		GenericString* subject();
		GenericString* from();
		GenericString* messageId();
		GenericString* timeStamp();

		GenericString* direction();
		GenericString* status();

		void serialize(GenericString &str, MailBody &body);
		void deSerialize();

		void normalizeAddresses();

	private:
		void normalizeAddress(GenericString &addressstr);

		GenericString _Subject;
		GenericString _From;
		GenericString _ToList;
		GenericString _CcList;
		GenericString _BccList;
		GenericString _MessageId;
		GenericString _TimeStamp;
		GenericString _Direction;
		GenericString _Status;
	};
}
#endif
