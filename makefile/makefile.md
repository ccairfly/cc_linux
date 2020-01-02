编译(compile): 使源文件生成出目标文件.obj, 再由中间目标文件生成可执行文件. 编译只关心函数与变量的声明是否正确
链接(link): 把大量的.obj或.o文件合成执行文件的动作, 叫做链接. 需要告诉编译器头文件的位置

Makefile 里主要包含了五个东西: 显式规则 隐晦规则 变量定义 文件指示 注释

1. 显式规则: 显式规则说明了如何生成一个或多的的目标文件. 这是由Makefile的书写者明显指出, 要生成的文件, 文件的依赖文件, 生成的命令.
2. 隐晦规则: 由于我们的make有自动推导的功能, 所以隐晦的规则可以让我们比较粗糙地简略地书写Makefile, 这是由make所支持的.
3. 变量的定义: 在Makefile中我们要定义一系列的变量, 变量一般都是字符串, 这个有点像C语言中的宏, 当Makefile被执行时, 其中的变量都会被扩展到相应的引用位置上.
4. 文件指示: 其中包含了三个部分, 一个是在一个Makefile中引用另一个Makefile,就像C语言中的include一样; 另一个是指根据某些情况指定Makefile中的有效部分, 就像C语言中的预编译#if一样; 还有就是定义一个多行的命令.
5. 注释: Makefile中只有行注释, 和UNIX的Shell脚本一样, 其注释是用"#"字符, 如果你要在你的Makefile中使用"#"字符, 可以用反斜框进行转义,如: "\#"

Makefile中的命令必须要以[Tab]键开始

文件指示:
包含文件 使用include可以把别的makefile包含进来
inlcude <fileName> 	(fileName可以包含路径和通配符, 但include后不允许出现[Tab])

比如有a.mk b.mk c.mk hello.make 以及变量$(bar)  其中变量 bar = d.mk e.mk, 则include的写法为
include hello.make *.mk $(bar) 这等价于include hello.make a.mk b.mk c.mk d.mk e.mk

make命令开始时,会找寻include所指出的其它Makefile, 并把其内容安置在当前的位置. make首先会在当前目录下寻找, 如果当前目录没有找到的时候, 还会在以下情况查找:
1. 如果 make 执行时, 有"-I"或"--include-dir"参数, 那么make就会在这个参数所指定的目录下去寻找.
2. 如果目录<prefix>/include（一般是： /usr/local/bin 或/usr/include）存在的话, make也会去找. (Windows下没有)
在include前面增加一个"-"符号, 作用是make没有找到这些文件或无法读取的时候, 不报错而继续执行

make的工作方式:
1. 读入所有的Makefile
2. 读入被include的其它Makefile
3. 初始化文件中的变量
4. 推导隐晦规则, 并分析所有规则
5. 为所有的目标文件创建依赖关系链
6. 根据依赖关系, 决定哪些目标要重新生成
7. 执行生成命令

文件搜索功能:
"VPATH"变量和vpath关键字, make会在当前目录中寻找目标文件和依赖文件, 找不到的话会根据VPATH目录去寻找, 
方法1. VPATH = src:../headers
方法2. vpath关键字 
vpath <pattern> <directories>	为符合模式pattern的文件指定搜索目录directories
vpath <pattern>清除符合模式<pattern>的文件的搜索目录
vpath 清除所有已被设置好了的文件搜索目录

<pattern>需要包含%字符比如%.h 
例如: vpath %.h ../src 在src目录下搜索所有.h

伪目标:
例如clean就是一个伪目标
伪目标标记.PHONY 伪目标可以没有依赖文件, 直接写命令

多目标:
bigoutput littleoutput: text.g 
	generate text.g -$(subst output,,$@) > $@ 
上述规则等价于: 
bigoutput: text.g 
	generate text.g -big > bigoutput 
littleoutput: text.g 
	generate text.g -little > littleoutput
其中, $@是自动化变量, 这里表示目标的集合, 类似于数组, 依次取出目标, 并执行命令

静态模式:
<targets ...>: <target-pattern>: <prereq-patterns ...> 
<commands> 
... 
targets 定义了一系列的目标文件, 可以有通配符. 是目标的一个集合
target-parrtern 是指明了targets的模式, 也就是的目标集模式
prereq-parrterns 是目标的依赖模式, 它对target-parrtern形成的模式再进行一次依赖目标的定义
例如:
objects = foo.o bar.o 
all: $(objects) 
$(objects): %.o: %.c 
	$(CC) -c $(CFLAGS) $< -o $@
等价于
foo.o: foo.c 
	$(CC) -c $(CFLAGS) foo.c -o foo.o 
bar.o: bar.c 
	$(CC) -c $(CFLAGS) bar.c -o bar.o
其中自动化变量$<表示依赖集合(这里指foo.c bar.c) $@表示目标集合(这里指foo.o bar.o)
