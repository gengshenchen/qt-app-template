#include "task_manager.h"
namespace core {
TaskManager& TaskManager::instance() {
    static TaskManager instance;
    return instance;
}

TaskManager::TaskManager() : m_executor() {} // 默認使用硬件支持的線程數

TaskManager::~TaskManager() {
    m_executor.wait_for_all(); // 確保程序退出前所有任務已完成
}

void TaskManager::enqueue(std::function<void()> task) {
    m_executor.silent_async(task); // silent_async 用於提交無返回值的任務
}
}

