# 调试子进程
set follow-fork-mode [parent|child]

parent: fork之后继续调试父进程，子进程不受影响。
child: fork之后调试子进程，父进程不受影响。

# 打开单步调试程序汇编的命令
set disassemble-next-line on

# 批量打断点
rbreak rb_*指令，找到了程序中所有以 tb_* 开头的函数

# 函数地址打断点
当调试汇编程序，或者没有调试信息的程序时，经常需要在程序地址上打断点，方法为b *address。例如：
(gdb) b *0x400522

# 开启日志将gdb的每条调试日志输出日志
set logging file [日志文件名字]
set logging on

# 自动反汇编接下来要执行的代码
set disassemble-next-line on

# 单步调试指令
si命令类似于s命令，ni命令类似于n命令。所不同的是，这两个命令（si/ni）所针对的是汇编指令，而s/n针对的是源代码。	

# 设置多次进入断点
ignore [断点ID] [断点次数]

# 打印指定地址的二进制
x/[前多少位]x [地址]

# 查看当前指令前的数据
x/200i $pc-200

# gdb带代码调试
gdb [可执行程序] --tui
# 打条件断点
b [断点ID|断点位置] if [c语言的condition]
# 启用断点
enable [breakpoints] [range...]
enable 1-5 # 启用1-5断点
# 禁用断点
disable [breakpoints] [range...]
例子：disable 1-5  # 禁用1到5的断点

# 线程锁
set scheduler-locking off|on
off:不锁定任何线程，如果在当前模式使用continue的话会启动所有线程
on:打开线程锁，锁定其他线程，只有当前线程执行

# 根据地址查看函数符号
info symbol [地址]

# 例如: 监视一个4byte 的整形地址. 书写格式举例如下.
多线程一定要加线程ID
watch [-l|-location] expr [thread threadnum] [mask maskvalue]
watch *(int*)0x12345678
# 多线程时候的watch
watch *(int*)0x12345678 thread [id号]
# 监控读某个地址断点
rwatch [地址]
# 监控读或者写某个地址断点
awatch [地址]

# 查看硬件断点的数量
show can-use-hw-watchpoints

# 执行很多次的命令
[命令] [次数]

# 如果有关实时事件 34 (SIG34) 的输出干扰使用 gdb 或 ddd 在 Linux 上进行调试，则可以将 GDB 配置为忽略这些事件：
handle SIG34 pass
handle SIG34 noprint

# gdb调试脚本
gdb [可执行程序] -command=[*.sh]

# 将下面默认值设为y的命令
Make breakpoint pending on future shared library load? (y or [n]) y
set breakpoint pending on

# 关闭pagequit
set pagination off

# gdb带参数命令
gdb -args [可执行程序] [命令参数]

# gdb自定义命令
http://ifeve.com/gdb-script/
# 完整*.gdb格式
define commandName  
    statement  
    ......  
end

document commandName
    search a specified byte value(0 ~ 255) during a memory
    usage: search_byte begin_addr end_addr byte
end

# 上述命令直接在gdb命令行中运行source xxx.gdb文件生效
例子：
https://blog.csdn.net/hejinjing_tom_com/article/details/50350865

# 打印a0寄存器后的地址
define print_a0
    set $a0_addr = $a0
    x/16x $a0_addr
end

# 自动单步调试的脚本
define print_instr
    while 1
        i r
        ni
    end
end

# gdb的find一个int的值
find [起始地址], [终止地址], (int)[需要查找的值]

# 只将日志输出到文件
set logging redirect on

# gdb不打印信息的命令
gdb -quiet [二进制]

# gdb官方手册
https://sourceware.org/gdb/current/onlinedocs/gdb.pdf

# gdb查看编译配置参数
show configuration

# python扩展gdb
https://blog.mengy.org/extend-gdb-with-python/
## 注册命令
source xxx.py

例子：
import gdb

class SearchPtr(gdb.Command):
    start_addr = ""
    end_addr = ""
    b_num = 1
    command = ""

    def __init__(self):
        super(self.__class__, self).__init__("search_ptr", gdb.COMMAND_USER)

    def invoke(self, args, from_tty):
        gdb.execute('set breakpoint pending on')
        gdb.execute('set pagination off')
        gdb.execute('set logging on')
        gdb.execute('handle SIG34 pass')
        gdb.execute('handle SIG34 noprint')

        gdb.execute('b compiler.cpp:4435')
        gdb.execute('r ~/tanglin/LoalCalculator/LoalCalculator/bin/Debug/net5.0/LoalCalculator.dll')

        while True:
            if self.is_compiler_b():
                # 打断点
                self.command = 'b *methodCodePtr'
                print(self.command)
                gdb.execute(self.command)
                self.b_num = self.b_num + 1
                self.command = 'disable {}'.format(self.b_num)
                print(self.command)
                gdb.execute(self.command)

                self.command = 'b *(end_ptr - 8)'
                print(self.command)
                gdb.execute(self.command)
                self.b_num = self.b_num + 1
                self.command = 'disable {}'.format(self.b_num)
                print(self.command)
                gdb.execute('disable {}'.format(self.b_num))

                # 获取start_ptr, end_ptr
                self.command = 'p/x start_ptr'
                print(self.command)
                self.start_addr = gdb.execute(self.command, False, True)
                self.start_addr = self.start_addr[self.start_addr.find('=') + 1: -1]

                self.command = 'p/x end_ptr'
                print(self.command)
                self.end_addr = gdb.execute(self.command, False, True)
                self.end_addr = self.end_addr[self.end_addr.find('=') + 1: -1]

                gdb.execute('c')
            else:
                self.command = 'find {}, {}, (int)0x688898b2'.format(self.start_addr, self.end_addr)
                print(self.command)
                find_result = gdb.execute('find {}, {}, (int)0x688898b2'.format(self.start_addr, self.end_addr), False, True)
                if -1 == find_result.find(' no '):
                    gdb.execute('c')
                else:
                    return

    def is_compiler_b(self):
        line_info = gdb.execute('info line', False, True)
        line_len = len("Line 4435")
        if "Line 4435" == line_info[:line_len]:
            return True
        return False

SearchPtr()
