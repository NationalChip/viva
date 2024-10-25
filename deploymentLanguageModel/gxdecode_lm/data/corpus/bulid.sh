#!/bin/bash

echo "-----------> 开始打包 <-----------"
pyinstaller -F word_probability_fine_tuned.py --clean
cp dist/* .
rm -rf __pycache__ build dist *.spec
split --bytes=85M word_probability_fine_tuned word_probability_fine_tuned_
echo "-----------> 打包完成 <-----------"
