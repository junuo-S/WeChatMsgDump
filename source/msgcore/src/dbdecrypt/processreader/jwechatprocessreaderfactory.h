#pragma once

class JAbstractWeChatProcessReader;
enum class WeChatMajorVersion : short;
class JWeChatProcessReaderFactoryItemBase;

class JWeChatProcessReaderFactory
{
public:
	static JAbstractWeChatProcessReader* createInstance(const WeChatMajorVersion majorVersion);

private:
	static void registerItem(JWeChatProcessReaderFactoryItemBase* item);
	static void unregisterItem(JWeChatProcessReaderFactoryItemBase* item);
	friend class JWeChatProcessReaderFactoryItemBase;
};

class JWeChatProcessReaderFactoryItemBase
{
public:
	JWeChatProcessReaderFactoryItemBase(const WeChatMajorVersion majorVersion);
	virtual ~JWeChatProcessReaderFactoryItemBase();
	virtual JAbstractWeChatProcessReader* createInstance() = 0;

	WeChatMajorVersion m_majorVersion;
};

template<class T>
class JWeChatProcessReaderFactoryItem : public JWeChatProcessReaderFactoryItemBase
{
public:
	JWeChatProcessReaderFactoryItem(const WeChatMajorVersion majorVersion) : JWeChatProcessReaderFactoryItemBase(majorVersion) {}
	virtual JAbstractWeChatProcessReader* createInstance() override
	{
		return new T;
	}
};

#define DECLAREFACTORY_PROCESS_READER(type, Class) \
	JWeChatProcessReaderFactoryItem<Class> gs_##Class##_factory(type);
