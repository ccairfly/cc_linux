1.使用VMware,虚拟机安装Ubuntu操作系统 --安装了ubuntu_V14.04版本

2.Linux中的隐藏文件,隐藏与文件属性无关,文件以.开头的都为隐藏文件,查看需要命令 ls -a 来查看 (普通ls)

3.文件的绝对路径和相对路径,在Linux中绝对路径是以'/'开始的,相对路径则相对于当前路径而言

4.Linux常用命令:
1) ls(list,列表):使用列表的方式把当前路径下的文件显示出来
ls -a显示所有,包括隐藏文件
ls -l

2) cd(change directory 更改目录)
3) pwd(print work directory 打印工作目录)

4) mkdir(make directory 创建文件夹)
mkdir -p 级联创建文件夹

5) mv(move 移动)
mv [源文件pathname] [目标文件pathname]
在目录间移动文件,修改文件名
修改pathname,可以用于移动文件和重命名文件

6) touch(创建空文件)

7) cp(copy 复制) 
cp [源文件pathname] [目标文件pathname]
cp -r 复制文件夹
cp -i 复制冲突的时候提示(建议使用)
cp -f 强制复制(建议使用,linux下已经存在文件复制可能会不成功)
cp -rf     cp -r 

8) rm(remove 移除,删除)
rm [pathname]
rm -r [pathname]

9) cat
命令用于连接文件并打印到标准输出设备上
文件内容简短的可以使用打印在终端上

10) rmdir(删除空文件夹,用处不大,建议使用rm -r)

11) ln(link 连接)
有软连接和硬连接的差别
软连接 ln -s 源文件pathname 目标文件pathname 相当于Windows中的快捷方式
ln 源文件pathname 目标文件pathname	

12) man 查询man手册,获得帮助信息
man 1 : 系统命令
man 2 : Linux的API
man 3 : c语言标准库函数

5.Linux中文件类型的标记方法
ls -l 显示详细信息的时候 开头一共10个字符 第1个是文件类型 后面9个文件权限
- 表示普通文件.
d 表示文件夹文件.
l 表示连接文件,后面会用->打印出指向.
s 表示socket文件
p 表示pipe管道文件