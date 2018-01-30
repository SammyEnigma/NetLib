#include "Mail.hpp"
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>

namespace CoreLibrary
{
	MailBody::MailBody()
	{
	}

	MailBody::~MailBody()
	{
	}

	MailBody::MailBody(const MailBody &other)
	{
		_Message = other._Message;
		_MessageId = other._MessageId;
		_AttachementList.append(other._AttachementList);
	}

	void MailBody::operator=(const MailBody& other)
	{
		_Message = other._Message;
		_MessageId = other._MessageId;
		_AttachementList.append(other._AttachementList);
	}

	void MailBody::setMessage(GenericString message)
	{
		_Message = message;
	}

	void MailBody::attachFile(GenericString filename)
	{
		_AttachementList.append(filename);
	}

	void MailBody::setMessageId(GenericString msgid)
	{
		_MessageId = msgid;
	}

	GenericString* MailBody::message()
	{
		return &_Message;
	}

	List<GenericString>* MailBody::attachments()
	{
		return &_AttachementList;
	}

	GenericString* MailBody::messageId()
	{
		return &_MessageId;
	}

	void MailBody::serialize(GenericString &str)
	{
		str.clear();

		if (_AttachementList.count() < 1)
		{
			str += "\n";
			str += _Message;
		}
		else
		{
			char linebuffer[1024] = { 0 };
			GenericString attachmentBoundary = "";
			DateTime ts;

			attachmentBoundary = "--";
			attachmentBoundary += ts.getDateString("yyyyMMddhhmmss");
			attachmentBoundary += ts.getDateString("yyyyMMddhhmmss");
			attachmentBoundary += "--";

			memset(linebuffer, 0, 1024);
			sprintf(linebuffer, "MIME-Version: 1.0\n");
			str += linebuffer;
			memset(linebuffer, 0, 1024);
			sprintf(linebuffer, "Content-Type:multipart/mixed;boundary=\"%s\"\n", attachmentBoundary.buffer());
			str += linebuffer;

			memset(linebuffer, 0, 1024);
			sprintf(linebuffer, "--%s\n", attachmentBoundary.buffer());
			str += linebuffer;

			str += "Content-type: text/plain; charset=iso-8859-1\n";
			str += "Content-Transfer-Encoding: quoted-printable\n";

			memset(linebuffer, 0, 1024);
			sprintf(linebuffer, "\n%s\n", _Message.buffer());
			str += linebuffer;

			for (int idx = 0; idx < _AttachementList.count(); idx++)
			{
				GenericString fname = _AttachementList.getAt(idx);

				File fl(fname);

				if (!fl.fileExists())
				{
					continue;
				}

				if (!fl.openFile(ReadOnly, Binary))
				{
					continue;
				}

				Buffer filebuff;
				fl.read(filebuff);

				GenericString b64content = filebuff.toBase64();

				memset(linebuffer, 0, 1024);
				sprintf(linebuffer, "--%s\n", attachmentBoundary.buffer());
				str += linebuffer;

				memset(linebuffer, 0, 1024);
				sprintf(linebuffer, "Content-Type:application/octet-stream;name=\"%s\"\n", fname.buffer());
				str += linebuffer;

				memset(linebuffer, 0, 1024);
				sprintf(linebuffer, "Content-Transfer-Encoding:base64\n");
				str += linebuffer;

				memset(linebuffer, 0, 1024);
				sprintf(linebuffer, "Content-Disposition:attachment;filename=\"%s\"\n\n", fname.buffer());
				str += linebuffer;

				str += b64content;
				str += "\n\n";

			}

			memset(linebuffer, 0, 1024);
			sprintf(linebuffer, "--%s--\n", attachmentBoundary.buffer());
			str += linebuffer;
		}
	}

	void MailBody::deSerialize()
	{
		parseAttachments();
		parseTexts();
	}

	bool MailBody::hasMimeData()
	{
		_Message.getSubStringList(_MimeData, "\r\n");

		while (true)
		{
			if (_MimeData.getAt(_MimeData.count() - 1)->length() < 1)
			{
				_MimeData.truncate();
			}
			else
			{
				break;
			}
		}

		GenericString firstline = _MimeData.getAt(0);
		GenericString lastline = _MimeData.getAt(_MimeData.count() - 1);
		bool hasMimeBoundary = false;
		bool hasContentTypeTag = false;

		if (firstline + "--" == lastline)
		{
			hasMimeBoundary = true;
			GenericString str = _MimeData.getAt(1);
			str.toLower();
			if (str.indexOf("content-type") > -1)
			{
				hasContentTypeTag = true;
			}
		}

		if (hasMimeBoundary && hasContentTypeTag)
		{
			return true;
		}

		return false;
	}


	void MailBody::parseAttachments()
	{
		List<GenericString> displaydata;

		int count = _MimeData.count();

		for (int idx = 0; idx < count; idx++)
		{
			GenericString line = _MimeData.getAt(idx);

			GenericString str = line;

			str.trim();
			str.toLower();

			if (str.indexOf("content-type:") > -1 && str.indexOf("name=") > -1)
			{
				idx--;
				extractAttachment(idx);
				continue;
			}

			displaydata.append(line);
		}

		_MimeData.clear();
		_MimeData.append(displaydata);
	}

	void MailBody::extractAttachment(int &ctr)
	{
		int startline = ctr;
		GenericString buffer = "";
		GenericString start = _MimeData.getAt(ctr);
		GenericString inlinecontentid;

		bool startaccumulating = false;
		MimeNode value;

		value.NodeType = Attachment;

		ctr++;

		while (true)
		{
			buffer = _MimeData.getAt(ctr);

			buffer.trim();

			if (start == buffer || start + "--" == buffer)
			{
				break;
			}

			if (buffer.length() < 1)
			{
				if (startaccumulating == false)
				{
					startaccumulating = true;
				}
				else
				{
					startaccumulating = false;
				}
				ctr++;
				continue;
			}

			if (startaccumulating)
			{
				if (buffer.length() > 0)
				{
					value.Data += buffer;
				}

				if (buffer.indexOf("==") > -1)
				{
					startaccumulating = false;
				}
			}
			else
			{
				GenericString temp = buffer;
				temp.toLower();

				if (temp.indexOf("content-disposition") > -1 && temp.indexOf("inline") > -1)
				{
					value.NodeType = InlineImage;
				}

				if (temp.indexOf("content-id") > -1)
				{
					int cpos = buffer.indexOf(':');
                    buffer.removeAt(0, cpos + 1);
					buffer.remove('<');
					buffer.remove('>');
					inlinecontentid = buffer;
					inlinecontentid.trim();
				}
			}

			ctr++;
		}

		GenericString name = _MimeData.getAt(startline + 1);
		int pos = name.indexOf("name=");

		GenericString key, val;

		name.getKeyValuePair(key, val, "name=");

		if (value.NodeType == Attachment)
		{
			value.NodeName = val;
			value.NodeName.replace("\"", "");
		}
		else
		{
			value.NodeName = inlinecontentid;
		}

		_ParsedMimeData.append(value);
		_AttachementList.append(value.NodeName);
	}

	void MailBody::parseTexts()
	{
		List<GenericString> displaydata;

		int count = _MimeData.count();

		for (int idx = 0; idx < count; idx++)
		{
			GenericString line = _MimeData.getAt(idx);

			GenericString str = line;

			str.trim();
			str.toLower();

			if (str.indexOf("content-type:") > -1
				&& (str.indexOf("text/plain") > -1 || str.indexOf("text/html") > -1))
			{
				idx--;
				extractText(idx);
				continue;
			}

			displaydata.append(line);
		}

		_MimeData.clear();
	}

	void MailBody::extractText(int &ctr)
	{
		GenericString buffer = "";
		GenericString start = _MimeData.getAt(ctr);

		bool startaccumulating = false;
		MimeNode value;

		ctr++;

		while (true)
		{
			buffer = _MimeData.getAt(ctr);
			buffer.trim();

			if (start == buffer || start + "--" == buffer)
			{
				break;
			}

			if (!startaccumulating)
			{
				if (buffer.length() < 1)
				{
					startaccumulating = true;
					ctr++;
					continue;
				}
			}

			if (startaccumulating)
			{
				if (buffer.length() < 1)
				{
					value.Data += "\r\n";
				}
				else
				{
					if (buffer.getAt(buffer.length() - 1) == '=' && value.TextEncoding == MailTextEncoding::QuotedPrintable)
					{
                        buffer.removeAt(buffer.length() - 1, 1);
					}

					value.Data += buffer;
				}
			}
			else
			{
				buffer.toLower();

				if (buffer.indexOf("content-type") > -1 && buffer.indexOf("text/plain") > -1)
				{
					value.NodeType = PlainText;
				}

				if (buffer.indexOf("content-type") > -1 && buffer.indexOf("text/html") > -1)
				{
					value.NodeType = Html;
				}

				if (buffer.indexOf("content-transfer-encoding") > -1 && buffer.indexOf("base64") > -1)
				{
					value.TextEncoding = MailTextEncoding::Base64;
				}

				if (buffer.indexOf("content-transfer-encoding") > -1 && buffer.indexOf("quoted-printable") > -1)
				{
					value.TextEncoding = MailTextEncoding::QuotedPrintable;
				}
			}

			ctr++;
		}

		if (value.TextEncoding == MailTextEncoding::Base64)
		{
			char* ptr = (char*)value.Data.fromBase64();
			value.Data.assign(ptr);
		}

		_ParsedMimeData.append(value);
	}

	List<MimeNode>* MailBody::mimeNodes()
	{
		return &_ParsedMimeData;
	}

	/*------------------------------------------------------*/

	MailHeader::MailHeader()
	{

	}

	MailHeader::~MailHeader()
	{

	}

	MailHeader::MailHeader(const MailHeader &other)
	{
		_Subject = other._Subject;
		_From = other._From;
		_MessageId = other._MessageId;
		_TimeStamp = other._TimeStamp;
		_ToList = other._ToList;
		_CcList = other._CcList;
		_BccList = other._BccList;
		_Direction = other._Direction;
		_Status = other._Status;

	}

	void MailHeader::operator=(const MailHeader& other)
	{
		_Subject = other._Subject;
		_From = other._From;
		_MessageId = other._MessageId;
		_TimeStamp = other._TimeStamp;
		_ToList = other._ToList;
		_CcList = other._CcList;
		_BccList = other._BccList;
		_Direction = other._Direction;
		_Status = other._Status;
	}

	void MailHeader::addtoToList(GenericString receipient, bool overwrite)
	{
		if (_ToList.length() < 1)
		{
			receipient.trim();
			_ToList = receipient;
		}
		else
		{
			if (overwrite)
			{
				_ToList = receipient;
			}
			else
			{
				_ToList += ",";
				receipient.trim();
				_ToList += receipient;
			}
		}
	}

	void MailHeader::addtoCcList(GenericString receipient, bool overwrite)
	{
		if (_CcList.length() < 1)
		{
			receipient.trim();
			_CcList = receipient;
		}
		else
		{
			if (overwrite)
			{
				_CcList = receipient;
			}
			else
			{
				_CcList += ",";
				receipient.trim();
				_CcList += receipient;
			}
		}
	}

	void MailHeader::addtoBccList(GenericString receipient, bool overwrite)
	{
		if (_BccList.length() < 1)
		{
			receipient.trim();
			_BccList = receipient;
		}
		else
		{
			if (overwrite)
			{
				_BccList = receipient;
			}
			else
			{
				_BccList += ",";
				receipient.trim();
				_BccList += receipient;
			}
		}
	}

	void MailHeader::setSubject(GenericString subject)
	{
		_Subject = subject;
	}

	void MailHeader::setFrom(GenericString from)
	{
		_From = from;
	}

	void MailHeader::setMessageId(GenericString msgid)
	{
		_MessageId = msgid;
	}

	void MailHeader::setTimeStamp(GenericString dt)
	{
		_TimeStamp = dt;
	}

	void MailHeader::setDirection(GenericString dir)
	{
		_Direction = dir;
	}

	void MailHeader::setStatus(GenericString st)
	{
		_Status = st;
	}

	void MailHeader::toList(List<GenericString> &lst)
	{
		_ToList.getSubStringList(lst, ',');
	}

	void MailHeader::ccList(List<GenericString> &lst)
	{
		_CcList.getSubStringList(lst, ',');
	}

	void MailHeader::bccList(List<GenericString> &lst)
	{
		_BccList.getSubStringList(lst, ',');
	}

	GenericString* MailHeader::to()
	{
		return &_ToList;
	}

	GenericString* MailHeader::cc()
	{
		return &_CcList;
	}

	GenericString* MailHeader::bcc()
	{
		return &_BccList;
	}

	GenericString* MailHeader::subject()
	{
		return &_Subject;
	}

	GenericString* MailHeader::from()
	{
		return &_From;
	}

	GenericString* MailHeader::messageId()
	{
		return &_MessageId;
	}

	GenericString* MailHeader::timeStamp()
	{
		return &_TimeStamp;
	}

	GenericString* MailHeader::direction()
	{
		return &_Direction;
	}

	GenericString* MailHeader::status()
	{
		return &_Status;
	}

	void MailHeader::serialize(GenericString &str, MailBody &body)
	{
		str.clear();

		str += "To: ";
		str += _ToList;
		str += "\r\n";

		str += "From: ";
		str += _From;
		str += "\r\n";

		str += "Subject: ";
		str += _Subject;
		str += "\r\n";

		if (_CcList.length() > 0)
		{
			str += "cc: ";
			str += _CcList;
			str += "\r\n";
		}

		if (_BccList.length() > 0)
		{
			str += "bcc: ";
			str += _BccList;
			str += "\r\n";
		}


		GenericString bodystr;

		body.serialize(bodystr);

		str += bodystr;
	}

	void MailHeader::deSerialize()
	{

	}

	void MailHeader::normalizeAddresses()
	{
		normalizeAddress(_From);
		normalizeAddress(_ToList);
		normalizeAddress(_CcList);
		normalizeAddress(_BccList);
	}

	void MailHeader::normalizeAddress(GenericString &addressstr)
	{
		List<GenericString> pass1;
		List<GenericString> pass2;
		GenericString str;
		GenericString line;
		int idx = 0;

		pass1.clear();
		pass2.clear();

		str = addressstr;
		str.replace('<', ',');
		str.replace('>', ',');

		str.getSubStringList(pass1, ',');

		for (idx = 0; idx < pass1.count(); idx++)
		{
			line = pass1.getAt(idx);
			line.trim();
			if (line.indexOf('@') > -1 && line.getAt(0) != '"')
			{
				pass2.append(line);
			}
		}

		addressstr.clear();
		GenericString::join(pass2, addressstr, ',');
	}
}
