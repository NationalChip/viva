# VIVA
* Viva 是一款功能强大的配置工具，专为简化 lvp_aiot(https://github.com/NationalChip/lvp_aiot) 低功耗离线语音识别 SDK 的开发而设计。通过 Viva，用户可以轻松定义和配置语音指令词、串口协议、TTS 播报以及方言自学习，实现真正的“0代码”开发。


# 使用指南
* 需要将 viva 跟 lvp_aiot sdk放在同级目录下。
* viva 仅支持在 Ubuntu 系统中运行。
* 第一次使用，请先完成以下两步：
    * 运行脚本`./sync.sh`拉取最新的声学模型
    * 确保安装了 ffmpeg。如果未安装，可以运行以下命令进行安装：
    ```
    sudo apt-get install ffmpeg

    ```

## 1.配置文件 config.yml 说明：
* config.yml 的对不同字段进行设置，将会有不同的运行结果，对各个字段的含义都应该理解清楚。
    * **如果想要在 lvp_aiot 工程目录下就可以直接 make 编译，rebuild_auto_config字段 应设置为Y。**
    * acoustics_model_version字段 应设置与gxmodel文件夹下发布的模型包一致的版本号，即与文件夹同名
* **工程中 config.yml 配置均有详细的注释说明，理解注释说明的意思后，方可正确配置**

## 2.结合viva和lvp_aiot工程制作Demo
* 阅读[1.配置文件 config.yml 说明](#1配置文件-configyml-说明)，熟悉viva的配置文件，接下来，就开始制作Demo。

### 2.1.配置viva
#### 2.1.1.必须设置的字段
* 设置芯片类型:
    * **chip_name:** 选 `GX8002D`
* 设置模型/项目包名字:
    * **project_name:** 请根据产品类型定义名称。比如 `general_dryer_v503_set38_20240529`。
* 设置声学模型版本:
    * **acoustics_model_version:** 请选 dual_model_mode，按如下格式填写。
        ```
            dual_model_mode:
                small_model:
                    V1.0.3
                large_model:
                    v0.5.5
        ```
* 设置工程目录:
    * **sdk_root_path:** sdk 工程的根路径，这里请填写：**../lvp_aiot/**
* 设置唤醒词、免唤醒词和指令词的灵敏度:
    * **major_kws_misactive_number:** 表示唤醒词的灵敏度，一般填 1~3 ，数值越大，灵敏度越高。推荐填 3。也可以填高、中、低进行设置，推荐填 中。
    * **immediate_command_word_misactive_number:** 表示免唤醒词的灵敏度，一般填 1~5 ，数值越大，灵敏度越高。推荐填 3。也可以填高、中、低进行设置，推荐填 中。
    * **command_word_misactive_number:** 表示指令词的灵敏度，一般填 3~12 ，数值越大，灵敏度越高。推荐填 8。也可以填高、中、低进行设置，推荐填 中。
    * **需要注意：主唤醒词 或 免唤醒词肯定会有一个，若主唤醒词 或 免唤醒词中那个没有，可以在该字段冒号后填null，或删除该字段。**
* 设置唤醒词:
    * **major_list:** 字段用来设置唤醒词列表，多个唤醒词运行使用 **|** 隔开，也允许换行书写。
* 设置免唤醒词:
    * **immediate_command_word_list:** 字段用来设置免唤醒词列表，多个免唤醒词运行使用 **|** 隔开，也允许换行书写。
* 设置所有命令词的ID，TTS回复:
    * ** words_list:** 字段包含将所有唤醒词、指令词的 ID 和 TTS回复
        * **语法格式：指令词#id@tts回复内容**
            * #id : 可以不需要填写，系统支持自动分配。
            * 多个指令词的id如果一样，请用 | 隔开，比如 指令词1|指令词2
            * 唤醒词的tts播报数目最多支持3条，lvp_aiot工程软件会随机选一条tts进行回复。
            * 指令词的tts播报数目只支持1条。

#### 2.1.2.非必须设置字段
* 其它 tts、串口、自学习等相关配置

### 2.2.运行 viva
* 在 `viva` 目录下运行 `run.sh`，即可生成模型包。
* 使用 `./run.sh -h` 可以查看帮助信息。

### 2.3.编译 lvp_aiot
* 回到 lvp_aiot 工程目录，直接 make 编译成功后，在 output 目录下会生成一个 mcu_nor.bin。
    ```shell
    ~/lvp_aiot/tools/viva
    $ cd ../../         # 回到 lvp_aiot 工程目录

    ~/lvp_aiot
    $ make clean;make   # make 进行编译

    ~/lvp_aiot
    $ ll output
    ... # 省略一亿行
    -rw-rw-r--  1 jindg jindg  130060 Apr 25 15:07 mcu_nor.bin
    ... # 省略一亿行
    ```
### 2.4.烧录
* 请阅读：[串口升级](https://nationalchip.gitlab.io/ai_audio_docs/software/lvp/SDK%E5%BC%80%E5%8F%91%E6%8C%87%E5%8D%97/SDK%E5%BF%AB%E9%80%9F%E5%85%A5%E9%97%A8/%E4%B8%B2%E5%8F%A3%E5%8D%87%E7%BA%A7/) 
