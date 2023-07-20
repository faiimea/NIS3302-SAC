# SAC-"System-level resource access auditing based on system call hooking"

## Function

系统的主功能为：内核模块通过重载系统调用函数记录日志信息，通过netlink方式将数据发往应用层，应用模块接受数据，对日志信息进行处理，展示与储存。

同时本系统实现了灵活性与UI的扩展，其中灵活性负责实现配置程序，进行应用层与内核的通信，在应用层对内核模块进行功能的配置；UI部分实现日志分析的可视化，数据库信息的处理与展示。

![structure1](https://github.com/faiimea/NIS3302-SAC/assets/87272252/b99a0020-62b1-4a44-b377-a06ef6dbdce8)


## Usage

### Kernel & User
后台程序编译：
```sh
gcc -o auditdemo auditdemo.c config.c Func.c -I ./
```
进入编辑模式:
```sh
./auditdemo -c
```
在kernel文件中有可运行的脚本，编辑模式script.sh

目前在脚本中定义的AuditPath为"/home/faii/Desktop/TEST"

需要更改可进入编辑模式。按照输出提示，输入0后再输入所需要路径（不需要加引号），最后输入q或Q退出即可

随后进入kernel文件夹，运行脚本scipt.sh，即可按照自定义规则insmod Module

### GUI

使用可视化界面实现用户管理，数据审计，日志整理等任务


![select1](https://github.com/faiimea/NIS3302-SAC/assets/87272252/e7f60a99-60be-4c79-a997-215b5ec5601c)

![select2](https://github.com/faiimea/NIS3302-SAC/assets/87272252/4e84da1f-34d1-42da-81ca-bbe70c1ba3b5)


