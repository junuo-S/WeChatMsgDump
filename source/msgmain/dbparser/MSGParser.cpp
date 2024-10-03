#include "MSGParser.h"

#include "defines.h"

#include "utils/utils.h"

static constexpr const char* const gs_appmsg = "appmsg";
static constexpr const char* const gs_title = "title";

MSGParser::MSGParser(const QVariantMap& recored)
	: m_localId(recored.value(STR_LOCALID).toLongLong())
	, m_talkerId(recored.value(STR_TALKERID).toLongLong())
	, m_msgSvrID(recored.value(STR_MSGSVRID).toLongLong())
	, m_type(recored.value(STR_TYPE).toInt())
	, m_subType(recored.value(STR_SUBTYPE).toInt())
	, m_isSender(recored.value(STR_ISSENDER).toBool())
	, m_createTime(recored.value(STR_CREATETIME).toLongLong())
	, m_sequence(recored.value(STR_SEQUENCE).toLongLong())
	, m_statusEx(recored.value(STR_STATUSEX).toLongLong())
	, m_flagEx(recored.value(STR_FLAGEX).toLongLong())
	, m_status(recored.value(STR_STATUS).toLongLong())
	, m_msgServerSeq(recored.value(STR_MSGSERVERSEQ).toLongLong())
	, m_msgSequence(recored.value(STR_MSGSEQUENCE).toLongLong())
	, m_strTalker(recored.value(STR_STRTALKER).toString())
	, m_strContent(recored.value(STR_STRCONTENT).toString())
	, m_displayContent(recored.value(STR_DISPLAYCONTENT).toString())
	, m_compressContent(recored.value(STR_COMPRESSCONTENT).toByteArray())
	, m_bytesExtra(recored.value(STR_BYTESEXTRA).toByteArray())
	, m_bytesTrans(recored.value(STR_BYTESTRANS).toByteArray())
{
	if (!m_compressContent.isEmpty())
	{
		QByteArray decompressContent;
		if (utils::DecompressLZ4(m_compressContent, decompressContent))
			m_decompressContentXML.setContent(QString::fromUtf8(decompressContent));
	}
	ensureMsgType();
}

MsgType MSGParser::getMsgType() const
{
	return m_msgType;
}

QString MSGParser::getSessionDisplay() const
{
	QString displayStr;
	switch (m_msgType)
	{
	case UnKnown:
		displayStr = QObject::tr("[Unknown msg]");
		break;
	case SystemNotice:
	case Tickle:
	case PlantText:
		displayStr = m_strContent;
		break;
	case Image:
		displayStr = QObject::tr("[Image]");
		break;
	case Voice:
		displayStr = QObject::tr("[Voice]");
		break;
	case Video:
		displayStr = QObject::tr("[Video]");
		break;
	case Sticker:
		displayStr = QObject::tr("[Sticker]");
		break;
	case MergedChatRecord:
		displayStr = QObject::tr("[Chat History]");
		break;
	case TextWithQuote:
		displayStr = getTitleFromQuoteMsg();
		break;
	case Transfer:
		displayStr = QObject::tr("[Transfer]");
		break;
	case VoiceOrVideoCall:
		displayStr = QObject::tr("[Voice/Video Call]");
		break;
	case File:
		displayStr = QObject::tr("[File]");
		break;
	default:
		break;
	}
	return displayStr;
}

bool MSGParser::getIsSender() const
{
	return m_isSender;
}

void MSGParser::ensureMsgType()
{
	if (m_type == 1 && m_subType == 0)
		m_msgType = PlantText;
	else if (m_type == 3 && m_subType == 0)
		m_msgType = Image;
	else if (m_type == 34 && m_subType == 0)
		m_msgType = Voice;
	else if (m_type == 43 && m_subType == 0)
		m_msgType = Video;
	else if (m_type == 47 && m_subType == 0)
		m_msgType = Sticker;
	else if (m_type == 49 && m_subType == 19)
		m_msgType = MergedChatRecord;
	else if (m_type == 49 && m_subType == 57)
		m_msgType = TextWithQuote;
	else if (m_type == 49 && m_subType == 2000)
		m_msgType = Transfer;
	else if (m_type == 50 && m_subType == 0)
		m_msgType = VoiceOrVideoCall;
	else if (m_type == 49 && (m_subType == 0 || m_subType == 6))
		m_msgType = File;
	else if (m_type == 10000 && (m_subType == 0 || m_subType == 8000))
		m_msgType = SystemNotice;
	else if (m_type == 10000 && m_subType == 4)
		m_msgType = Tickle;
	else
		m_msgType = UnKnown;
}

QString MSGParser::getTitleFromQuoteMsg() const
{
	QDomElement root = m_decompressContentXML.documentElement();
	QDomElement appmsg = root.firstChildElement(gs_appmsg);
	QString title = appmsg.firstChildElement(gs_title).text();
	return title;
}

