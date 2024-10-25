#!/bin/bash

# 函数运行启动脚本或Python文件
run_main() {
    if [ -e main ]; then
        echo "============== 使用 main 执行文件启动 ============"
        chmod +x main
        ./main "$@"
    elif ls main_* 1> /dev/null 2>&1; then
        cat main_* > main
        chmod +x main
        echo "============== 使用 main_* 合并后的执行文件启动 ============"
        ./main "$@"
    else
        echo "============== 使用 main.py 源代码文件启动 ============"
        python3 main.py "$@"
    fi
}

# 记录脚本开始时间
#start_time=$(date +%s)


run_main $@


## 记录脚本结束时间
#end_time=$(date +%s)

# 计算脚本运行时长
#duration=$((end_time - start_time))

#echo "Script execution time: $duration seconds"

