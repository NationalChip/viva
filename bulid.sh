#!/bin/bash

echo "-----------> 开始打包 <-----------"
pyinstaller -F main.py --clean
cp dist/* .
rm -rf __pycache__ build dist *.spec
split --bytes=85M main main_
echo "-----------> 打包完成 <-----------"
