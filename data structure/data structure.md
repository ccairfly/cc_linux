数据结构与算法(C语言版)

基本概念:
数据:所有能输入到计算机中并被计算机程序处理得符号的总称
数据元素:是数据的基本单位,数据的个体
数据项:是数据结构的最小单位, 组合项
数据对象:是性质相同的数据元素集合
数据结构:是相互之间存在一种或多种特定关系的数据元素的集合

数据的逻辑结构(关系结构):
1.线性结构
2.树型结构
3.图型结构(网状结构)
4.集合

数据的存储结构: 逻辑结构的存储映象 数据结构在计算机中的表示
数据元素的映象方法:
最小单位是二进制数的一位(bit)
位串构成元素或结点
关系的映象方法:
顺序映象:顺序存储(数组等)
非顺序映象:链式存储(指针)
可借用高级语言中的"数据类型"来描述存储结构

数据类型
在高级程序语言编写的程序中 数据类型是一个值的集合和定义 在此集合上的一组操作的总称

抽象数据类型(ADT)
是指一个数学模型以及定义在此数学模型上的一组操作
重要特征:
1.数据抽象:强调是其本质的特征 其所能完成的功能以及它和外部用户的接口(即外界使用它的方法)
2.数据封装:

抽象数据类型的表示与实现
(1)预定义常量和类型
#define TRUE 		1 
#define FALSE		0
#define OK			1
#define ERROR		0
#define INFEASIBLE	-1
#define OVERFLOW	-2
typedef int Status;

(2)结构的表示 用类型定义typedef描述
(3)基本操作的算法 











