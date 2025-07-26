#!/usr/bin/env python3
# -*- coding: utf-8 -*-
 
# python3 gen_and_upload_symbols.py ../out/

import os
import sys
import argparse
import subprocess
import tempfile
import platform
import shutil
import requests

# --- 配置区 ---
# 请在此处修改为您的项目配置
CONFIG = {
    "SUBDOMAIN": "cgs",  # 您的 Backtrace 子域名
    "TOKEN": "e461fe30fdad99925439a62c599edf7d66b7e007d1379d2359a7e2380af62130",  # 具有 'symbol:post' 权限的符号访问令牌
    "TAG": "v1.2.7",  # 为本次上传指定的标签 (例如应用版本号), 如果不需要则设为 None
    "DUMP_SYMS_PATH": "../3rdparty/crashpad/tools/linux/dump_syms"  # dump_syms 工具的路径
}
# --- 配置区结束 ---


# 定义不同平台下需要查找的调试符号文件扩展名或目录名
PLATFORM_TARGETS = {
    "Windows": [".pdb"],
    "Darwin": [".dSYM"],  # macOS
    "Linux": []  # 在Linux上, 调试信息通常在未剥离的二进制文件中
}

def find_debug_files(search_path):
    """
    在指定路径下递归查找与当前平台匹配的调试文件或目录。

    Args:
        search_path (str): 要搜索的根目录。

    Returns:
        list: 包含所有找到的目标绝对路径的列表。
    """
    system_platform = platform.system()
    targets = PLATFORM_TARGETS.get(system_platform, [])
    
    if not targets and system_platform == "Linux":
        print(f"警告：在 Linux 平台上，请确保 '{search_path}' 包含带调试信息的未剥离二进制文件。")
        # 在Linux上，我们可能需要查找没有扩展名的可执行文件或.so文件
        # 为简化起见，这里假设用户会直接提供包含这些文件的目录
        targets.extend(['.so']) # 添加.so作为常见目标
    elif not targets:
        print(f"警告：当前平台 '{system_platform}' 没有预设的调试文件目标。")
        return []


    print(f"平台: {system_platform}。将在 '{search_path}' 目录下查找 {', '.join(targets)} ...")
    
    found_files = []
    for root, dirs, files in os.walk(search_path):
        # 优先匹配目录 (如 .dSYM)
        for d in dirs:
            if any(d.endswith(ext) for ext in targets):
                full_path = os.path.join(root, d)
                found_files.append(full_path)
        # 再匹配文件 (如 .pdb 和 .so)
        for f in files:
            if any(f.endswith(ext) for ext in targets) or (system_platform == "Linux" and '.' not in f):
                 full_path = os.path.join(root, f)
                 found_files.append(full_path)
                
    print(f"找到了 {len(found_files)} 个匹配的目标。")
    return list(set(found_files)) # 去重

def generate_symbol(dump_syms_path, debug_file_path, output_dir):
    """
    使用 dump_syms 为给定的文件生成符号文件。

    Args:
        dump_syms_path (str): dump_syms 工具的路径。
        debug_file_path (str): 调试符号文件的路径。
        output_dir (str): 用于存放生成的 .sym 文件的目录。

    Returns:
        str: 生成的 .sym 文件的路径，如果失败则返回 None。
    """
    print(f"  -> 正在为 '{os.path.basename(debug_file_path)}' 生成符号...")
    
    try:
        # dump_syms 将符号数据输出到 stdout
        result = subprocess.run(
            [dump_syms_path, "-r", debug_file_path], # -r 表示递归，对.dSYM等目录友好
            capture_output=True, text=True, check=True, encoding='utf-8', errors='ignore'
        )
        
        stdout_lines = result.stdout.splitlines()
        if not stdout_lines:
            print(f"     错误：dump_syms 没有为 '{debug_file_path}' 生成任何输出。")
            return None

        # 第一行包含了符号文件的正确名称信息: MODULE os arch id name
        first_line = stdout_lines[0]
        parts = first_line.split()
        if parts[0] != 'MODULE':
             print(f"     错误：dump_syms 输出格式不正确。第一行不是 'MODULE'。")
             return None
        
        # 使用模块名作为 .sym 文件名
        sym_filename = f"{parts[4]}.sym"
        sym_filepath = os.path.join(output_dir, sym_filename)

        # 将完整的符号输出写入文件
        with open(sym_filepath, "w", encoding='utf-8') as f:
            f.write(result.stdout)

        print(f"     成功生成符号文件：'{sym_filename}'")
        return sym_filepath

    except FileNotFoundError:
        print(f"     致命错误：找不到 dump_syms 工具。请检查配置中的 DUMP_SYMS_PATH：'{dump_syms_path}'")
        sys.exit(1)
    except subprocess.CalledProcessError as e:
        print(f"     错误：执行 dump_syms 失败。返回码: {e.returncode}")
        print(f"     STDERR: {e.stderr.strip()}")
        return None
    except Exception as e:
        print(f"     生成符号时发生未知错误: {e}")
        return None

def create_archive(source_dir, archive_path_without_ext):
    """
    将一个目录的内容打包成 .zip 文件。

    Args:
        source_dir (str): 要打包的源目录。
        archive_path_without_ext (str): 归档文件的路径（不含扩展名）。

    Returns:
        str: 生成的 .zip 文件的完整路径。
    """
    print(f"-> 正在将符号文件打包成 ZIP 归档...")
    zip_path = shutil.make_archive(archive_path_without_ext, 'zip', source_dir)
    print(f"   成功创建归档：'{zip_path}'")
    return zip_path


def upload_archive(archive_path, subdomain, token, tag=None):
    """
    将符号归档上传到 Backtrace 服务器。

    Args:
        archive_path (str): 要上传的归档文件路径。
        subdomain (str): Backtrace 的子域名。
        token (str): 符号上传访问令牌。
        tag (str, optional): 为本次上传关联的标签。
    """
    
    # 构建上传 URL
    url = f"https://submit.backtrace.io/{subdomain}/{token}/symbols"
    params = {}
    if tag:
        params['tag'] = tag
        print(f"-> 准备上传到服务器，标签为: '{tag}'")
    else:
        print("-> 准备上传到服务器...")

    try:
        with open(archive_path, 'rb') as f:
            files = {'upload_file': (os.path.basename(archive_path), f, 'application/zip')}
            response = requests.post(url, params=params, files=files, timeout=300) # 5分钟超时

        print(f"   服务器响应状态码: {response.status_code}")
        
        # 检查响应状态
        response.raise_for_status()
        
        print("   上传成功！")
        # 打印服务器返回的具体信息
        try:
            print(f"   服务器消息: {response.json()}")
        except requests.exceptions.JSONDecodeError:
            print(f"   服务器消息: {response.text}")

    except requests.exceptions.HTTPError as e:
        print(f"   上传失败！HTTP 错误: {e.response.status_code}")
        print(f"   错误详情: {e.response.text}")
    except requests.exceptions.RequestException as e:
        print(f"   上传失败！发生网络错误: {e}")
    except Exception as e:
        print(f"   上传时发生未知错误: {e}")


def main():
    """
    主函数，解析参数并协调整个流程。
    """
    # 检查配置是否已填写
    if "your-company" in CONFIG["SUBDOMAIN"] or "your-symbol-post-token-here" in CONFIG["TOKEN"]:
        print("错误：请先在脚本顶部的 CONFIG 部分填写您的项目信息。")
        sys.exit(1)

    parser = argparse.ArgumentParser(
        description="为 Backtrace/Sauce Labs 生成、打包并上传调试符号（内置配置版）。"
    )
    parser.add_argument(
        "search_path",
        help="包含调试符号文件（如 .pdb, .dSYM）的根目录。"
    )
    args = parser.parse_args()

    # 1. 查找所有相关的调试文件
    debug_files = find_debug_files(args.search_path)
    if not debug_files:
        print("未找到任何可处理的调试文件。脚本退出。")
        sys.exit(0)

    # 使用临时目录来存放生成的 .sym 文件和最终的 .zip 文件
    with tempfile.TemporaryDirectory() as temp_dir:
        print(f"\n使用临时目录：'{temp_dir}'")
        sym_dir = os.path.join(temp_dir, 'symbols')
        os.makedirs(sym_dir)
        
        generated_sym_files = []
        
        # 2. 为每个找到的文件生成符号
        for i, file_path in enumerate(debug_files):
            print(f"\n--- 处理文件 {i+1}/{len(debug_files)}: {os.path.basename(file_path)} ---")
            sym_file = generate_symbol(CONFIG["DUMP_SYMS_PATH"], file_path, sym_dir)
            if sym_file:
                generated_sym_files.append(sym_file)

        if not generated_sym_files:
                        print("\n未能成功生成任何符号文件。脚本退出。")
                        sys.exit(1)
            
        print(f"\n--- 总共生成了 {len(generated_sym_files)} 个 .sym 文件 ---")

        # 3. 打包所有 .sym 文件
        archive_base_path = os.path.join(temp_dir, 'symbol_archive')
        archive_path = create_archive(sym_dir, archive_base_path)
        
        # 4. 上传归档
        upload_archive(
            archive_path,
            CONFIG["SUBDOMAIN"],
            CONFIG["TOKEN"],
            CONFIG["TAG"]
        )

    print("\n--- 任务完成 ---")

if __name__ == "__main__":
    main()

