#!/bin/bash

echo "-----------> 开始打包 <-----------"
pyinstaller -F createTTS.py --clean
cp dist/* .
rm -rf __pycache__ build dist *.spec
split --bytes=85M createTTS createTTS_
echo "-----------> 打包完成 <-----------"
