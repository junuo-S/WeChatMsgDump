# 架构设计

## 分层

- 启动层：`wechatmsgdump`
  - 仅负责加载 UI 动态库并转发执行。
- 表现层：`msgui`
  - 界面渲染、交互、事件订阅、用户流程驱动。
- 领域层：`msgcore`
  - 解密、合并、消息查询、消息对象构建。
- 基础设施层：`junuobase/junuoui`
  - 通用工具与控件。

## 模块边界

- `msgui` 不直接操作数据库与解密实现，只通过 `global_interface.h` 中接口交互。
- `msgcore` 通过 `IJCoreSubject/IJCoreObserver` 向 UI 推送异步结果。
- `msgcore` 内部仍保持实现细节可替换：进程读取器、解密器均由工厂创建。

## 关键对象关系

- `IJCoreEntry -> IJCoreApplication`
- `IJCoreApplication -> IJWeChatDBDecryptManager / IJMsgViewManager`
- `IJWeChatDBDecryptManager`：负责读取进程信息、解密并合并数据库。
- `IJMsgViewManager`：负责联系人/会话/消息查询。

## 设计要点

- 通过接口隔离 UI 与核心逻辑，降低耦合。
- 通过事件总线（Observer）统一异步回传形态。
- 通过工厂 + 抽象基类，为微信版本差异预留扩展点。
