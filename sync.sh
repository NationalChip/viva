#!/bin/bash

# 拉取主仓库的最新代码
git pull origin main

# 初始化并更新子模块到主仓库记录的版本
git submodule update --init --recursive

# 更新子模块到最新版本
git submodule update --remote --recursive

