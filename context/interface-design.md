# 接口设计

## 公共接口位置

- 文件：`source/include/global_interface.h`

## 核心接口

- `IJCoreEntry`
  - `GetCoreApplication()`：获取核心应用对象。
- `IJCoreApplication`
  - `GetDecryptManager()`：解密管理器。
  - `GetMsgViewManager()`：消息浏览管理器。
- `IJWeChatDBDecryptManager`
  - `StartReadWeChatProcess()`
  - `StartDecryptDataBase()`
- `IJMsgViewManager`
  - `GetContactInfo()`（同步缓存命中 + 异步补齐）
  - `StartQueryAllStrTalker()`
  - `StartQueryMessageCount()`
  - `StartQuerySessionLastMessage()`
  - `StartQueryChatHistory()`

## 事件模型

- 主题接口：`IJCoreSubject`（Attach/Detach/Notify）
- 观察者接口：`IJCoreObserver`（`OnCoreEvent`）
- 事件基类：`IJCoreEvent`
- 通用异步事件：`JCommonAsyncEvent`
- 消息视图事件：`JMsgViewAsyncEvent`

## 事件分类

- `Event_ReadProcess`：进程信息读取。
- `Event_Decrypt`：解密阶段进度。
- `Event_Combine`：合并阶段进度。
- `Event_MsgView`：消息查询结果。

## 消息抽象

- `IJMessage` 统一消息读取接口：类型、会话、时间、文本、原始字段。
- `MessagePtr` 为 `std::shared_ptr<IJMessage>`，用于 UI/核心间传递。
- `JMessageFactory` 根据 DB 记录生成具体消息对象。
