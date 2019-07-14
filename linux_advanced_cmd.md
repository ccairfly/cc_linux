Linux高级指令

1. apt-get 
是Ubuntu中的一个软件 , 是用于在线安装和卸载软件的程序
安装和卸载都是在线的,所以必须能上网才能使用
sudo apt-get install vim
sudo apt-get remove vim

要解决的问题:
1)权限问题
2)上网问题

2. vi和vim的使用
vi和vim是Linux系统中的文本编辑器,vi vim gedit..等等
vi是Linux中默认的编辑器 , vim是vi的升级版
vim命令行模式和插入模式
1)命令模式的保存和退出
:wq 保存并且退出(最常用)
:q! 不保存强制退出
:w 	保存不退出

2)vim的高级使用
*查找 : 在命令模式输入/xxx 匹配到的单词会高亮显示,输入:noh取消高亮 输入n或N(shift+n)切换上下一个
*快速切换行 : 在命令模式下输入:num 定位到num行数
*显示行号 : 在命令模式下输入:set nu  取消行号输入:set nonu
*整行删除 : 将光标移动到要删除的一行,在命令模式下输入dd , 删除3行整行 输入3dd
*撤销操作 : 命令模式下输入u
*行复制 : 命令模式下输入yy 复制3行输入3yy
*粘贴 : 命令模式下输入p

3.Linux命令行中一些符号的含义
.	代表当前
..	代表上一层目录
-	代表前一个目录(不常用)
~	代表当前用户的宿主目录
/	代表根目录
$	代表普通用户的命令行提示符(起始位置)
#	代表root管理员用户的命令行提示符(起始位置)
*	万能匹配符

~宿主目录:操作系统为当前用户所设计用来存放文件和工作的默认目录,类似于Windows中的我的文档.
对于root用户管理员 输入su 和密码之后进入的管理员身份 宿主目录为/root

4.普通用户和管理员用户root
Linux中的普通用户权限也有受到限制,普通用户进入不了 cd /root 的宿主目录 , 普通用户使用不了apt-get
使用su来进行普通用户和管理员用户切换 su root(root可以省略)  和 su username切换回普通用户
使用sudo 可以让普通用户的本条命令暂时获取root用户的权限,而不必进行用户切换

5.rwx与权限表示
使用ls -l显示详细信息的时候 前面共有10个字符,第一个字符代表文件类型,剩下的9个分为三组,表示文件的权限
前三个表示此文件的属主对文件的权限(最常用看前三个就行)
中间三个表示此文件属主所在的组队文件的权限
后面三个表示其他用户对文件的权限
例如: drwxr-xr-x
权限:
r 可读
w 可写
x 可执行
- 不可

6.Linux进阶指令
1) find(查找)
sudo find ~ -name "xxx" ~目录的以下路径中查找xxx

2)grep
在一个文本文件中,搜索关键词
grep -rn "123" *
表示在当前路径的所有文件中递归搜索含有"123"的关键词的文件

3)which 
查找一个应用程序在哪里(只能查到路径)

4)whereis 
查找一个应用程序的信息及其源码和man手册的位置(比which详细)

5)uname查看系统信息
uname -a

6)关机和重启
init 0 关机
init 2 重启

7)tree/lstree(不常用,需要安装)
目录下的内容以树形结构显示出来tree

8)mount/umount 
挂载

9)磁盘空间相关
df -h	显示已挂载的分区列表
du -h	显示文件或文件夹的大小

7.多用户管理和权限管理
useradd xxx	新建xxx用户
passwd xxx 	为xxx用户添加密码

权限表示和编码规则
r	可读	4
w 	可写	2
x 	可执行	1
- 	无权限	0

文件的权限为 drwxr-xr-x	编码后为755

1)chmod 
修改权限为drwxr--r-- 则命令chmod 744 [文件名]
2)chown 
修改文件的拥有者chown xxx file.txt
3)chgrp
修改文件的所属群组 chgrp xxx file.txt

8.压缩和解压
1)压缩成tar.gz
tar -czvf xxx.tar.gz . 将此文件夹下的内容打包成xxx.tar.gz压缩包
2)解压缩tar.gz 
tar -xzvf xxx.tar.gz   解压xxx.tar.gz







