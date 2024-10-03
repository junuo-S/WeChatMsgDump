#pragma once

#include <QVariantMap>
#include <QDomDocument>

#include "global.h"

class MSGParser
{
public:
	MSGParser(const QVariantMap& recored);
	MsgType getMsgType() const;
	QString getSessionDisplay() const;
	bool getIsSender() const;

private:
	void ensureMsgType();
	QString getTitleFromQuoteMsg() const;

	qint64 m_localId;
	qint64 m_talkerId;
	qint64 m_msgSvrID;
	short m_type;
	short m_subType;
	bool m_isSender;
	qint64 m_createTime;
	qint64 m_sequence;
	qint64 m_statusEx;
	qint64 m_flagEx;
	qint64 m_status;
	qint64 m_msgServerSeq;
	qint64 m_msgSequence;
	QString m_strTalker;
	QString m_strContent;
	QString m_displayContent;
	QByteArray m_compressContent;
	QDomDocument m_decompressContentXML;
	QByteArray m_bytesExtra;
	QByteArray m_bytesTrans;
	MsgType m_msgType = UnKnown;
};
