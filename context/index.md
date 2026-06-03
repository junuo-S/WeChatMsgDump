# WeChatMsgDump 知识库

本目录是当前工作区的项目知识库。处理需求时先读本文件，再按任务类型阅读对应文档。

## 必读顺序

1. 先读 [项目简介](./project-overview.md)，确认工程结构、主链路和技术栈。
2. 按任务类型读取下方“任务导航”中的文档。
3. 修改 C/C++ 代码前，继续阅读 [编码规则入口](../.codex/rules/index.md)。

## 任务导航

| 任务类型 | 必读文档 |
| --- | --- |
| 新功能、较大重构、模块职责判断 | [架构设计](./architecture-design.md)、[核心流程](./core-flows.md) |
| 解密、读取、消息解析、数据库字段 | [数据库与消息模型](./database-and-message-model.md)、[微信3.9数据库简述](./微信3.9数据库简述.md) |
| UI、窗口、事件、展示层接口 | [接口设计](./interface-design.md)、[核心流程](./core-flows.md) |
| 异步任务、回调、线程安全 | [线程与异步模型](./threading-model.md)、[接口设计](./interface-design.md) |
| CMake、依赖、构建输出、运行方式 | [构建与依赖](./build-and-dependencies.md) |
| 代码评审、缺陷定位、行为回归分析 | [项目简介](./project-overview.md)、再按涉及模块选择相关文档 |

## 文档导航

- [项目简介](./project-overview.md)
- [架构设计](./architecture-design.md)
- [线程与异步模型](./threading-model.md)
- [接口设计](./interface-design.md)
- [核心流程](./core-flows.md)
- [构建与依赖](./build-and-dependencies.md)
- [数据库与消息模型](./database-and-message-model.md)
- [微信3.9数据库简述](./微信3.9数据库简述.md)

## 维护约定

- 知识库基于当前仓库 `source/`、`design/`、`README.md` 与已有说明文档整理。
- 当源码行为、构建方式、数据库理解或模块边界发生变化时，同步更新相关文档。
- 文档优先写稳定事实和项目约束，避免记录临时猜测；不确定内容应明确标注。
