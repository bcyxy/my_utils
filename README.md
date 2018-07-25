My Utils

## 使用
- 编译
    - 完全编译
        - 描述：编译所有的模块
        - 命令：`make`
    - 指定编译
        - 描述：编译指定的模块
        - 命令：`make <指定的模块>`
            - e.g. `make str_aux`
        - 模块名称即`src/`目录下的文件夹名称
- 加入项目
    - 将`output/include/`路径下的对应模块的`.h`文件`include`到项目中；
    - 将`output/lib/`路径下的对应模块的库连接入项目；
