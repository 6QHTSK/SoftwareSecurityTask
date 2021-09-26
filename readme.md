
# 软件安全课设

已获得2021年度软件安全课程设计作品赛一等奖

## 从源文件简要安装方法

### 注射器、注入dll文件

- 下载并编译Detours库，编译使用32位；
- 下载nlohmann_json_realease库，将上述库链接到src文件夹下
- 编译dll文件，将编译出的dll文件所在目录地址及其自身绝对地址复制到src/Injector/Injector.cpp文件的26/27行相应处
- 编译注射器，将编译出的exe文件绝对地址复制到public/main.py 40行处

### 目标文件

- 直接使用Visual Studio编译即可，请记住他们的绝对路径。

### 网页文件

- 查看[src/web/README.md](src/web/README.md)，按照其指示制作

### 运行

- 用管理员权限运行public/main.py，访问[localhost:9013](http://localhost:9013)即可。注意受到浏览器限制，只能复制文件绝对路径到输入框中

## 需要自行下载

- Detours-4.0.1
- nlohmann_json_release-3.9.1

项目中还使用了base64.cpp and base64.h的内容
