# 线程与异步模型

## 总体策略

- UI 主线程只做界面与事件分发。
- 耗时任务（读进程、解密、DB 查询）全部异步执行。

## 解密链路线程模型

- `StartReadWeChatProcess()`：投递到 `QThreadPool::globalInstance()`，完成后发 `Event_ReadProcess`。
- `StartDecryptDataBase()`：
  - 先创建独立 `QThread` 承载总流程。
  - 对每个待解密 DB 文件，继续投递到全局线程池并行解密。
  - `waitForDone()` 等待所有解密任务结束后，串行执行合并。
  - 通过 `Event_Decrypt` / `Event_Combine` 进度事件回推 UI。

## 查询链路线程模型

- `WechatDbReader` 持有 `JunuoDbThreadPool`。
- 线程池内部为每个 worker 绑定独立 `QSqlDatabase` 连接。
- 查询通过 `invokeMethod(..., Qt::QueuedConnection)` 下发到 worker 所在线程执行。
- 查询结果再通过 queued invoke 回到接收者线程。

## 事件与线程安全约束

- `IJCoreSubject::Notify` 同步遍历观察者，观察者内部通常再 `invokeMethod` 回 UI 线程。
- `JunuoDbThreadPool` 通过 `QMutex + QWaitCondition` 管控 worker 借还。

## 当前实现中的注意点

- `StartDecryptDataBase()` 使用 `QThreadPool::globalInstance()->waitForDone()`，会等待全局线程池中的全部任务；若未来新增无关全局任务，可能互相影响。
- `QThreadPool::start([&decryptedCount])` 里捕获原子引用，生命周期依赖外层线程函数，不应提前退出。
