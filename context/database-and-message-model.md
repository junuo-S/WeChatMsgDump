# 数据库与消息模型

## 解密库选择规则

- 配置文件：`source/msgcore/resource/configs/databasesv3.json`
- `needDecrypt=true` 的库会进入解密队列。
- 支持固定文件名和正则匹配（`fuzzy=true`）。

当前 v3 配置里典型参与解密的库：

- `MicroMsg.db`
- `OpenIMContact.db`
- `OpenIMMedia.db`
- `OpenIMMsg.db`
- `MSG*.db`
- `MediaMSG*.db`

## 查询主表

- 会话统计与聊天记录主要读取 `MSG` 表。
- 联系人及头像主要读取 `Contact`、`ContactHeadImgUrl`。

## 消息类型映射

- 统一枚举：`JMsgType`。
- 工厂：`JMessageFactory::create(...)` 按 Type/SubType 生成子类。
- 子类示例：文本、图片、语音、视频、文件、引用、转账、系统消息等。

## 联系人缓存策略

- `JMsgViewManager` 使用 `QCache<QString, ContactInfo>`（容量 1024）。
- `GetContactInfo(wxid)`：
  - 命中缓存：立即返回。
  - 未命中：返回 `nullopt` 并异步查库；结果到达后再通知 UI。

## 已识别的实现现状

- `JWeChatDBDecryptManager` 代码会尝试读取 `:/configs/databasesv4.json`，但当前资源目录仅看到 `databasesv3.json`；若走 v4 逻辑需补齐配置与资源打包。
