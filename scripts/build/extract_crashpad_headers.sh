#!/bin/bash
# 提取 crashpad 头文件用于 include-only 分发

# 设置源和目标路径（你可以根据实际项目调整）
CRASHPAD_SRC=../../3rdparty/crashpad/crashpad
CRASHPAD_DEST=../../3rdparty/crashpad/include

mkdir -p "$CRASHPAD_DEST"

echo "Copying crashpad headers..."

for dir in client handler util; do
    mkdir -p "$CRASHPAD_DEST/$dir"
    rsync -av --include="*/" --include="*.h" --exclude="*" \
        "$CRASHPAD_SRC/$dir/" "$CRASHPAD_DEST/$dir/"
done

# Mini Chromium headers
MINI_SRC="$CRASHPAD_SRC/third_party/mini_chromium/mini_chromium"
MINI_DEST="$CRASHPAD_DEST/third_party/mini_chromium"

mkdir -p "$MINI_DEST"
rsync -av --include="*/" --include="*.h" --exclude="*" \
    "$MINI_SRC/" "$MINI_DEST/"

echo " Done."

