#!/bin/bash

echo "-----------> 开始打包 <-----------"
pyinstaller -F acousticModelDeployment.py --clean
cp dist/* .
rm -rf __pycache__ build dist *.spec
split --bytes=85M acousticModelDeployment acousticModelDeployment_
echo "-----------> 打包完成 <-----------"
