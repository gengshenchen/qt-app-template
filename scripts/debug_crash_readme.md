# 工具 build from breakpad 
* 符号提取工具 dump_syms
* 分析工具 minidump_stackwalk
* 上传工具 sym_upload  上传符号库到服务器。可用可不用		

## 第一步：提取 .sym 并构建符号目录结构
* ``` symbols/
         └── MyApp/
               └── <debug_id>/
                  └── MyApp.sym

* shell切换到script 目录执行
  ```
    ./generate_symbols.sh ../out/bin/qt-app-template
  ```

## 第二步： 分析 minidump 文件
* shell切换到script 目录执行
  ``` 
    ./analyze_minidump.sh ../out/crashs/completed/xxxxxxx.dmp
  ``` 
* 输出崩溃时候的调用堆栈

## 另 minidump-2-core 
| 功能                   | 工具                      | 需要什么                       |
| -------------------- | ----------------------- | -------------------------- |
| 解析 `.dmp` 为调用栈       | `minidump_stackwalk`    | `.sym` + `.dmp`            |
| 转 `.core` + 用 GDB 调试 | `minidump-2-core` + GDB | 原始程序 + `.core`（不需要 `.sym`） |

gdb ./MyApp ./crash.core


