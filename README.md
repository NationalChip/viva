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
** 如果想要在 lvp_aiot 工程目录下就可以直接 make 编译，rebuild_auto_config字段 应设置为Y。
** acoustics_model_version字段 应设置与gxmodel文件夹下发布的模型包一致的版本号，即与文件夹同名
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
                        #通用模型_V055
        ```
## 如何 “0代码” 开发 lvp_aiot
* 阅读 config.yml 中的注释