#pragma once

class JAbstractWeChatDBDecryptor;
enum class WeChatMajorVersion : short;
class JWeChatDBDecryptorFactoryItemBase;
class JWeChatDBDecryptorFactory
{
public:
	static JAbstractWeChatDBDecryptor* createInstance(const WeChatMajorVersion majorVersion);

private:
	static void registerItem(JWeChatDBDecryptorFactoryItemBase* item);
	static void unregisterItem(JWeChatDBDecryptorFactoryItemBase* item);
	friend class JWeChatDBDecryptorFactoryItemBase;
};

class JWeChatDBDecryptorFactoryItemBase
{
public:
	JWeChatDBDecryptorFactoryItemBase(const WeChatMajorVersion majorVersion);
	virtual ~JWeChatDBDecryptorFactoryItemBase();
	virtual JAbstractWeChatDBDecryptor* createInstance() = 0;
	WeChatMajorVersion m_majorVersion;
};

template<class T>
class JWeChatDBDecryptorFactoryItem : public JWeChatDBDecryptorFactoryItemBase
{
public:
	JWeChatDBDecryptorFactoryItem(const WeChatMajorVersion majorVersion) : JWeChatDBDecryptorFactoryItemBase(majorVersion) {}
	virtual JAbstractWeChatDBDecryptor* createInstance() override
	{
		return new T;
	}
};

#define DECLAREFACTORY_DECRYPTOR(type, Class) \
	JWeChatDBDecryptorFactoryItem<Class> gs_##Class##_factory(type);
