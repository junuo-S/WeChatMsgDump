#pragma once

#include <QStringList>

class JWeChatDBCombiner
{
public:
	~JWeChatDBCombiner();
	bool beginCombine(const QString& outputFilePath);
	bool combineFile(const QString& inputFilePath);
	bool endCombine();
};
