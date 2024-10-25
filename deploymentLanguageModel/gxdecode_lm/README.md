
1 python 3.7或python 3.8的环境下运行

2  安装pypinyin
  pip install pypinyin==0.35.1


3 准备指令词文件corpus.txt
 3.1  目前不支持3个字以下和6个字以上的指令词
 3.2 如果指令词是6个字以上的，请根据指令词的唯一性，手动修改为6个字或5个字或4个字


4 运行
  ./run.sh corpus.txt
 
 语言模型输出位置: data/search_Graph/{gxdecoder_tlg.fst, words.txt}
