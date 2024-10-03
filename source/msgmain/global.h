#pragma once

enum WechatPage
{
	ChatPage,
	FriendPage,
};

enum MsgType
{
	UnKnown = -1,
	PlantText,
	Image,
	Voice,
	Video,
	Sticker,
	MergedChatRecord,
	TextWithQuote,
	Transfer,
	VoiceOrVideoCall,
	File,
	SystemNotice,
	Tickle,
};
