# 核心流程

## 1. 启动流程

1. `WinMain` 加载 `msgui.dll`，调用 `MsgMainExec`。
2. `MsgMainExec` 加载 `msgcore.dll`，解析 `GetCoreEntry`。
3. 初始化 `MsgApplication`，进入 Qt 事件循环。

## 2. 解密流程

1. `DecryptDialog` 打开后进入 `LoadingPage`。
2. `LoadingPage` 调用 `StartReadWeChatProcess()`。
3. `WxProcessListPage` 展示版本、路径、wxid、数据目录等。
4. 用户点击 `begin decrypt`，`DecryptingPage` 调 `StartDecryptDataBase()`。
5. `msgcore` 并行解密多个 DB，随后合并为 `merged_db.db`。
6. 解密和合并结束后触发 `sigBeginMsgView` 进入消息浏览。

## 3. 消息浏览流程

1. `WechatMsgDialog` 订阅 `IJMsgViewManager`。
2. 启动时请求 `StartQueryAllStrTalker()`。
3. `MiddlePage` 根据会话列表创建 session 卡片。
4. 选中会话后继续查询数量、最近消息、历史消息。
5. 头像优先返回缓存；未命中时异步查库并触发头像 ready 事件。

## 4. 数据产物

- 最终合并库位置：`AppLocalDataLocation/<wxid>/merged_db.db`
- 解密临时目录：与最终库同级 `decrypted/`
