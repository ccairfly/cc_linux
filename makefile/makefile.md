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


条件判断
ifeq ifneq ifdef ifndef 
else
endif

使用函数
makefile中可以使用函数来完成一些变量的操作, 从而使make命令更加灵活, 函数调用之后, 函数的返回值可以当成变量使用
函数的调用使用$来标识的, 其语法是$(<function> <args>) args是函数的参数, 以","隔开, 函数名和参数之间以空格隔开

1.字符串处理函数

字符串替换函数(重点)
$(subst <from>,<to>,<text>)
功能:把字符串<text>中的<from>替换成<to>

模式字符串替换函数(重点)
$(patsubst <pattern>,<replacement>,<text>)
功能:查找<text>中的字符串中内容是否有符合<pattern>后缀的内容, 有的话替换成<replacement>中的内容

去空格函数
$(strip <string>)
例如$(strip a b c d e)则返回abcde

查找字符串
$(findstring <find>,<in>)
$(findstring a,a b c)
功能:在<in>的字符串中查找<find>字符串, 找到则返回<find>, 否则返回空字符串

过滤函数filter(重点)
$(filter <pattern...>,<text>)
功能:以<pattern>模式过滤<text>中的内容, 返回符合模式<pattern>的单词, <pattern>可以有多个

反过滤函数filter-out(重点)
$(filter-out <pattern...>,<text>)

$(sort <list>)排序函数
$(word <n>,<text>)取词函数
$(wordlist <start>,<end>,<text>)取单词串函数
$(words <text>)单词个数统计函数
$(firstword <text>)取首单词函数


2.文件名操作函数

取目录函数dir(重点)
$(dir <namelist>)
功能:在文件名序列<namelist>中取出目录部分
示例:$(dir src/dir dir2)
返回值为src/ ./

取文件名函数(重点)
$(notdir <namelist>)
功能:从文件名序列<namelist>中取出文件名部分
示例:$(notdir src/foo.c doc)
返回值为foo.c doc

取后缀函数
$(suffix <namelist>)
功能:从文件名序列<namelist>中取出各个文件名的后缀, 若文件没有后缀, 则返回空
示例:$(suffix src/foo.c src-2/foo2.c src/foo)
返回值为.c .c

$(basename <namelist>)取前缀函数
$(addsuffix <suffix>,<namelist>)加后缀函数
$(addprefix <prefix>,<names...>)加前缀函数

$(join <list1>,<list2>)连接函数
示例:$(join abbb ccc,123 45678) 返回值为abbb ccc123 45678


3.foreach函数 循环函数
$(foreach <var>,<list>,<text>)
功能:将<list>中的单词逐一取出, 放到<var>临时变量中, 然后再执行<text>表达式, 每一次<text>会返回一个字符串,
循环过程中, <text>所返回的每个字符串会以空格分隔, 循环结束时<text>则为foreach函数的返回值
示例:
names :=a b c d 
files :=$(foreach n,$(names),$(n).o)
$(files)的值为a.o b.o c.o d.o

4.if函数
功能与条件语句ifeq相类似
$(if <condition>,<then-part>,<else-part>)
若condition为真, 则返回<then-part>, 否则返回<else-part>, 若没有定义<else-part>则返回空

5.call函数
用来创建新参数化的函数
$(call <expression>,<parm1>,<parm2>,<parm3>...)
<expression>中的变量会依次被<parm>取代

6.origin函数
origin函数不操作变量的值, 指明了变量的来源
$(origin <var>)
返回值:
undefined 	-变量没有被定义过
default 	-默认的变量
environment	-环境变量
file 		-这个变量被定义在makefile中
command line-这个变量在命令行中定义
override	-override指示符重新定义的
automatic	-命令运行中的自动化变量


7.shell函数
操作系统shell的命令, shell函数把执行操作系统命令后的输出作为函数返回
$(shell <cmd>)


8.控制make的运行
$(error <text...>)		产生一个致命错误, <text>是错误信息
$(warning <text...>)	不会让make退出, 输出一段警告信息, make继续运行


>make的运行
一般是在命令行中输入make, 然后make命令会找到当前目录下的makefile来执行
有的时候你有几套编译规则, 你想在不同的时候使用不同的编译规则

1.make的退出码
退出码有三个:
0 - 表示执行成功
1 - make运行过程中出现错误
2 - 使用了make的-q选项, 并且使得make一些目标不用更新, 返回2

2.指定makefile
GNU make找寻默认的Makefile的规则是在当前目录下依次找三个文件"GNUmakefile" "makefile"和"Makefile"
我们也可以给makefile指定特殊名字的makefile, 比如common.mk, 可以使用make的"-f"或"--file"参数指定文件 make –f common.mk

3.指定目标
一般来说, make的最终目标是makefile中的第一个目标, 而其它目标一般是由这个目标连带出来的
有一个make的环境变量叫"MAKECMDGOALS"
推荐目标写法
"all"		-这个伪目标是所有目标的目标, 其功能一般是编译所有的目标
"clean" 	-这个伪目标功能是删除所有被make创建的文件
"install"	-这个伪目标功能是安装已编译好的程序, 其实就是把目标执行文件拷贝到指定的目标中去
"print"		-这个伪目标的功能是列出改变过的源文件
"tar" 		-这个伪目标功能是把源程序打包备份, 也就是一个tar文件
"dist"		-这个伪目标功能是创建一个压缩文件, 一般是把tar文件压成Z文件, 或是gz文件
"TAGS"		-这个伪目标功能是更新所有的目标, 以备完整地重编译使用
"check"
"test"		-这两个伪目标一般用来测试makefile的流程

4.检查规则
只打印命令, 不执行命令, 可以使用参数
"-n"
"--just-print" 
"--dry-run" 
"--recon"

假编译目标, 不是真正的编译, 只把目标文件的时间更新, 但不更改目标文件
"-t"
"--touch"

找目标行为, 如果目标存在, 则什么都不输出. 如果不存在则报错提示
"-q"
"--question

指定一个源文件(或依赖文件) make会根据规则, 推导出依赖这个文件的命令
"-W <file>"
"--what-if=<file>"
"--assume-new=<file>" 
"--new-file=<file>"


5.make的常用参数
GNUmake3.80版的参数定义

"-b" 
"-m"
这两个参数的作用是忽略和其他版本make的兼容性

"-B"
"--always-make" 
认为所有的目标都需要更新(重编译)

"-C <dir>"
"--directory=<dir>"
一般make会在当前目录下读取makefile, 加入-C参数指定目录读取makefile


"--debug[=<options>]"
输出make的调试信息
<options>选项
a - 也就是all, 输出所有的调试信息
b - basic, 输出简单的调试信息
v - verbose, 级别在b之上
i - implicit, 输出所有的隐含规则
j - jobs, 输出执行规则中命令的详细信息
m - makefile, 输出makefile执行更新的信息

"-d"
相当于"--debug=a"

"-e"
"--environment-overrides"
指明环境变量的值会覆盖makefile中定义的变量的值

"-f=<file>"
"--file=<file>"
"--makefile=<file>"
指定makefile文件

"-h"
"--help"
显示帮助信息

"-i" 
"--ignore-errors"
在执行时忽略所有的错误

"-I<dir>"
"--include-dir=<dir>"
指定一个被包含makefile的搜索目标, 可以使用多个"-I"参数来指定多个目录

"-j[<jobsnum>]"
"--jobs[=<jobsnum>]"
指同时运行命令的个数

"-k"
"--keep-going"
出错也不停止运行

...


>make的隐含规则

"隐含规则"也就是一种惯例, make会按照这种"惯例"心照不喧地来运行, 那怕我们的Makefile中没有书写这样的规则
例如, 把[.c]文件编译成[.o]文件这一规则, 你根本就不用写出来, make会自动推导出这种规则, 并生成我们需要的[.o]文件
系统变量"CFLAGS"可以控制编译时的编译器参数

1. 编译C程序的隐含规则
"<n>.o"的目标的依赖目标会自动推导为"<n>.c", 并且其生成命令是"$(CC) –c $(CPPFLAGS) $(CFLAGS)"

2. 编译C++程序的隐含规则
"<n>.o"的目标的依赖目标会自动推导为"<n>.cc"或是"<n>.C"，并且其生成命令是"$(CXX) –c $(CPPFLAGS) $(CFLAGS)" (建议使用".cc"作为C++源文件的后缀, 而不是".C")

3. 汇编和汇编预处理的隐含规则
"<n>.o"的目标的依赖目标会自动推导为"<n>.s", 默认使用编译器"as", 并且其生成命令是: "$(AS) $(ASFLAGS)" 
"<n>.s"的目标的依赖目标会自动推导为"<n>.S", 默认使用C预编译器"cpp", 并且其生成命令是: "$(AS) $(ASFLAGS)"

4. 链接Object文件的隐含规则
"<n>"目标依赖于"<n>.o", 通过运行C的编译器来运行链接程序生成(一般是"ld"), 其生成命令是: "$(CC) $(LDFLAGS) <n>.o $(LOADLIBES) $(LDLIBS)" 
这个规则对于只有一个源文件的工程有效, 同时也对多个Object 文件(由不同的源文件生成)的也有效

x: y.o z.o
x.c y.c z.c都存在的情况下将会执行:

cc -c x.c -o x.o 
cc -c y.c -o y.o 
cc -c z.c -o z.o 
cc x.o y.o z.o -o x 
rm -f x.o 
rm -f y.o 
rm -f z.o
如果没有一个源文件(如上例中的x.c)和你的目标名字(如上例中的x)相关联, 那么你最好写出自己的生成规则,不然隐含规则会报错的


>>隐含规则使用的变量
编译C程序的隐含规则的命令是"$(CC) –c $(CFLAGS) $(CPPFLAGS)"
$(CC)为命令变量, $(CFLAGS)为参数变量,
如果把$(CC)重定义成"gcc", 把变量$(CFLAGS)重定义成"-g", 那么隐含规则中的命令全部会以"gcc –c -g $(CPPFLAGS)"的样子来执行了
把隐含规则中使用的变量分成两种: 一种是命令相关的, 如"CC"; 一种是参数相的关, 如"CFLAGS"

1. 关于命令的变量(C语音常用)
AR		-函数库打包程序, 默认命令是"ar"
AS 		-汇编语言编译程序, 默认命令是"as"
CC 		-C语言编译程序, 默认命令是"cc"
CXX 	-C++语言编译程序, 默认命令是"g++"
CPP 	-C程序的预处理器(输出是标准输出设备) 默认命令是"$(CC) –E"
RM 		-删除文件命令, 默认命令是"rm –f"

2. 关于命令参数的变量
LDFLAGS -链接器参数(如: "ld")
ARFLAGS -函数库打包程序AR命令的参数, 默认值是"rv"
ASFLAGS -汇编语言编译器参数(当明显地调用".s"或".S"文件时)
CFLAGS 	-C语言编译器参数


>>隐含规则链
有些时候, 一个目标可能被一系列的隐含规则所作用
例如, 一个[.o]的文件生成, 可能会是先被Yacc的[.y]文件先成[.c], 然后再被C的编译器生成, 我们把这一系列的隐含规则叫做"隐含规则链"

定义模式规则
使用模式规则来定义一个隐含规则, 目标的定义需要有"%"字符, 表示一个或多个任意字符. "%"的展开发生在变量和函数的展开之后, 变量和函数的展开发生在make加载makefile的时候.
比如%.o : %.c ; <command ......>
指出了怎么从所有的.c文件生成相对应的.o文件的规则, 如果生成的目标是"a.o b.o", 那%c就是"a.c b.c"

一旦依赖目标中的%被确定,那么make会被要求去匹配当前目录下所有的文件名, 一旦找到, make就会执行规则下的命令, 所以在模式规则中, 目标可能会是多个的
下面这个例子把所有.c文件编译成.o文件
%.o:%.c 
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@
"$<"和"$@"是自动化变量 $@表示目标集合 $<表示依赖集合


自动化变量
$@ 自动化目标集合
$% 目标是函数库的时候的目标集合
$< 自动化依赖目标中的第一个目标,如果依赖目标是以模式(即"%")定义的, 那么"$<"将是符合模式的一系列的文件集 (注意, 其是一个一个取出来的)
$^ 自动化依赖目标集合
$? 所有比目标新的依赖目标的集合
$+ 自动化依赖目标集合(与$^很像, $+不去除重复项)
$* 表示目标模式中"%"及其之前的部分 如果目标是"dir/a.foo.b", 并且目标的模式是"a.%.b", 那么"$*"的值就是"dir/a.foo", 表示目标后缀前的内容
如果目标是"foo.c", 因为".c"是 make 所能识别的后缀名, 所以"$*"的值就是"foo"


模式匹配
"%"代表一个或多个字符, 所以在定义好了的模式中, 我们把"%"所匹配的内容叫做"茎", 例如"%.c"所匹配的文件"test.c"中"test"就是"茎"

重载内建隐含规则



















