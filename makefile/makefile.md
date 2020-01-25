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

自动生成依赖性:
可以使用gcc编译器自动生成依赖性, 比如在main.c中包含了#include "defs.h", 那依赖关系应该是 main.o : main.c defs.h
大多数C/C++编译器都支持"-M"选项 gcc -MM main.c 的输出则是：main.o: main.c defs.h
如果源码有很多.c文件 makefile也对应这源文件进行修改
GNU 组织建议把编译器为每一个源文件的自动生成的依赖关系放到一个文件中, 
为每一个"name.c"的文件都生成一个"name.d"的Makefile文件,[.d]文件中就存放对应[.c]文件的依赖关系



命令的书写
1. 显示命令
make会把执行的命令在执行前会输出打印到屏幕上, 在命令前加入@则这条命令不会显示出来
比如 @ echo 正在编译XXX模块	和 echo 正在编译XXX模块 这两个命令 分别会输出:正在编译XXX模块(一句) 和 echo 正在编译XXX模块 正在编译XXX模块(两句)
make 参数"-s"或"--slient"则是全面禁止命令的显示

2. 命令执行
当依赖目标新于目标时, 也就是当规则的目标需要被更新时, make会一条一条的执行其后的命令
如果要让上一条命令的结果应用在下一条命令的时候, 应该使用分号分隔这两条命令, 比如使用cd 然后后续做一些事情的时候

3. 命令出错
命令运行完成之后, make会检测命令返回的返回码, 如果命令返回成功, 则make会执行下一条命令
如果一个规则中的某个命令出错了(命令退出码非零), 那么make就会终止执行当前规则, 这将有可能终止所有规则的执行
在命令行前面加"-" 比如:	-rm *.o
全局的方法, 在make命令加上"-i"参数, 那么make执行的所有命令都会忽略错误
make的"-k"参数, "--keep going"如果某规则中的命令出错了, 则终止该命令的执行, 但是继续执行其他的命令

4. 嵌套执行make
一些大的工程中, 我们会把我们不同模块或是不同功能的源文件放在不同的目录中, 我们可以在每个目录中都书写一个该目录的Makefile, 模块编译, 分段编译, 便于维护
比如有个subdir的子目录, 子目录下有个makefile文件
subsystem: 
	cd subdir && $(MAKE)
等价于
subsystem: 
	$(MAKE) -C subdir
$(MAKE)是宏变量的意思, 给出make一些需要的参数, 便于维护
这里意思是cd进入subdir目录, 然后执行make命令, 这个makefile是总控makefile, 总控makefile的变量可以传递到下级的makefile中



使用变量
makefile中的变量可以包含数字,字符和下划线. 但是不能包含":" "#" "="和空字符(空格回车等) $< $@ 自动化变量
1. 变量的基础
变量在声明的时候需要给予初值, 在使用的时候要加上$
2. 变量中的变量
变量中"="右侧的值也可以为变量, 有可能会变量递归定义, 
A = $(B)
B = $(A)
所以可以使用":="操作符来定义变量 
x := foo
y := $(x) bar
x := later
结果为
y = foo bar
x = later
这种定义方法的好处是前面的变量不能使用后面的变量

变量的高级用法
变量值的替换:替换变量中的共有部分, 其格式是$(var:a=b)或${var:a=b}, 将变量var中所有以a结尾的字符串替换成b 结尾指的是空格和结束符
例如:
foo := a.o b.o c.o
bar := $(foo:.o =.c)

追加变量值+=

override指示符
有变量是在命令行中设置的, 可以使用override指示符定义
override <var> := <value>
override <var> = <value>

多行定义define关键字(命令包技术)
define two-lines 
echo foo 
echo $(bar) 
endef

环境变量
CFLAGS, 可以在make开始运行的时候载入到makefile文件中, 这个变量如果由命令行带入, 则环境变量被覆盖(除了在make指定-e参数之外)

目标变量
相当于局部变量
target:变量声明
target:override 变量声明
例如
prog : CFLAGS = -g 
prog : prog.o foo.o bar.o 
	$(CC) $(CFLAGS) prog.o foo.o bar.o 
prog.o : prog.c 
	$(CC) $(CFLAGS) prog.c 
foo.o : foo.c 
	$(CC) $(CFLAGS) foo.c 
bar.o : bar.c 
	$(CC) $(CFLAGS) bar.c
这个示例中不管环境变量CFLAGS是什么在这个target规则中都是-g

模式变量
模式变量的使用方法和目标变量类似
pattern:变量声明
pattern:override 变量声明
























