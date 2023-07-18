# SCO

## Function

系统的主功能为：内核模块通过重载系统调用函数记录日志信息，通过netlink方式将数据发往应用层，应用模块接受数据，对日志信息进行处理，展示与储存。

同时本系统也会实现灵活性与UI的扩展，其中灵活性负责实现配置程序，进行应用层与内核的通信，在应用层对内核模块进行功能的配置；UI部分将实现日志分析的可视化，完成审计浏览工具的开发。

gcc -o auditdemo auditdemo.c config.c Func.c -I ./

./auditdemo -c 进入编辑模式

在kernel文件中有可运行的脚本，编辑模式script.sh

## TODO:
...
