1. HCI Commands: HCI commands packet的长度不超过255bytes
Opcode(包含OGF+OCF 2Bytes) + Parameter Total Length(1Byte) + Parameter...

OGF(Opcode Group Field): 操作码组字段 占用高6bit
OCF(Opcode Command Field): 操作码命令字段 占用低10bit

每个OGF下的OCF不同, 常用的OGF:
1.1 linkControlCommands				0x01 链路控制命令, inquiry, connect, disconnect...
1.2 linkPolicyCommands				0x02 链路策略命令, hold mode, sniff, switch role...
1.3 HCI ControlAndBasebandCommands	0x03 控制本地芯片跟基带命令, reset
1.4 informationParameterCommands	0x04 读取信息命令, 读LMP版本, read addr...
1.5 statusParameterCommands			0x05 读状态参数命令, 如读取rssi
1.6 testingCommands					0x06 测试命令, 让芯片进入DUT模式
1.7 LE Commands						0x07 BLE的命令
1.8 vendorSpecificCommands			0x3F 厂商扩展的命令

2. HCI Event
Event Code(1Byte) + Parameter Total Length(1Byte) + Parameter...
controller响应回host的状态
比如: 
0x01 Inquiry Complete Event 搜索完成
0x04 Connection Request Event 连接请求
0x15 Return Link Keys event 返回linkkey

3. 蓝牙HCI流控(flow control)
流控用于host和controller之间, 分为:
host->controller的流控
controller->host的流控

3.1 host到controller的数据流控
分为两种Packet-based Data Flow Control 和 Data-Block-Based Data Flow Control
通过HCI_Write_Flow_Control_Mode命令来切换 
Packet-based data flow control在BR/EDR芯片默认, 可以不下这条命令

command								OCF			CommandsParameter	returnParameter
HCI_Write_Flow_Control_Mode			0x0067		flowControlMode		status

flowControlMode:
0x00	Packet-based data flow control(BR/EDR芯片默认)
0x01	Data-Block-Based Data Flow Control(AMP芯片默认)

步骤1: 在初始化时, host会向controller发送Read_Buffer_Size的command, 获取到ACL和SCO分组的最大长度(封包个数和每包的size)
步骤2: host发送一包ACL之后, buffer_size-1
步骤3: controller收到包之后会返回Num of complete packet event给host, host收到后更新buffer_size, 这里buffer_size+1
步骤4: 当host收到controller发来的HCI_Disconnection_Complete断开完成事件, 
packetbuffer被释放, 而且controller也不会再发送Num of complete packet event事件了

3.2 controller到host的流控
...

4. 蓝牙协议栈初始化流程
4.1 硬件连接 H4的transport, 则需要连接: host和controller TX/RX/CTS/RTS相连
H5和BCSP的transport只需要接入TX/RX
4.2 对应transport的初始化, 对于H4的transport, 初始化TX/RX/CTS/RTS
4.3 持续发送HCI_RESET直到芯片回应, 接收到command complete with command opcode
4.4 发送Vendor command, 芯片原厂自定义的commands
4.5 发送Read Buffer Size command, 接收到command complete with command opcode
4.6 发送Read BT address command, 接收到command complete with command opcode
4.7 发送Write code command, 接收到command complete with comand opcode
4.8 发送Change Local Name command, 接收到command complete with comand opcode
4.9 发送Write page timeout command, 接收到command complete with comand opcode
4.10 发送Write set Event Mask command, 接收到command complete with comand opcode
芯片产生的事件掩码:
inquiry complete event 
inquiry result event
connection complete event 
等等... 选择conntroller回复host的HCI Event
4.11 发送Write Write Simple Pairing command, 接收到command complete with comand
此部分如果配置为1, 那么就默认SSP配对; 如果是0, 那么就是PINCODE配对
4.12 发送Write Scan Enable command, 接收到command complete with comand opcode
打开inquiry scan就可以搜索到蓝牙了

5. 传统蓝牙HCI搜索流程 搜索command以及产生的event
蓝牙搜索周围设备, 也叫做inquiry, 整体流程如下:
5.1 host给controller发送inquiry命令
inquiry的command parameter参数中包含LAP, Inquiry_Lenght, Num_Responses
LAP: GIAC和LIAC普通可搜索和限制可搜索, 
Inquiry_Length:搜索时间1.28~61.44s
Num_Responses: 最多返回的搜索到的设备个数0x01~0xFF
Inquiry_Length和Num_Responses只要有一个条件满足则停止搜索
5.2 controller收到命令后给host返回command status的HCI event
5.3 controller通过HCI event上报inquiry result
此部分搜索上来的inquiry result中获取不到bluetoothName, 需要额外去调用Remote Name Request command来获取
5.4 最终controller上报inquiry complete

6. 传统蓝牙HCI连接流程 HCI connection command以及产生的event
此部分指的是hci的连接, 不是上层profile的连接

6.1 HCI连接的流程(对应master):
1) host向controller发送HCI_Create_Connection的命令
command parameter参数包含BD_ADDR, Packet_Tpye, Page_Scan_Repetition_Mode, Clock_Offset, Allow_Role_Switch
BD_ADDR:要连接的remote设备的蓝牙地址
Packet_Type:支持的数据封包类型2-DH1,3-DH1,DH1,2-DH3,3-DH3,DH3,2-DH5,3-DH5,DH5...
Page_Scan_Repetition_Mode:page scan重复模式
Clock_Offset:时钟偏移
Allow_Role_Switch:是否允许角色转换

2) controller向host上报command status with create connection opcode
HCI Command Status event参数:status, Num_HCI_Command_Packets, commandOpcode
status:0x00 command pending; 0x01~0xFF: command failed 具体看controller error code
Num_HCI_Command_Packets:用于标示command status是返回哪个command的, 这里是command的标号
commandOpcode:用于标示command status是返回哪个cmmand的

3) controller向host上报连接完成
connection complete event参数包含status,Connection_Handle,BD_ADDR,Link_Type,Encryption_Enable
status:0x00表示connection completed
Link_Type:连接的类型 0x00-SCO 0x01-ACL
Encryption_Enabled:是否允许加密, 0x00禁止 0x01允许

6.2 HCI接受连接请求(对应slave):
1)host收到controller发来的连接请求事件connect request event
参数:BD_ADDR, Class_of_Device, Link_Type
Class_of_Device:可以看出对方是手机, 平板电脑还是其他
Link_Type:连接的类型 0x00-SCO 0x01-ACL 0x02-eSCO

2)host向controller发送接受连接的command
command parameter: BD_ADDR,Role.
Role:选择当前连接的角色0x00-master 0x01-slave

3)host收到controller发来的event: command status with accept connection req的opcode

4)host收到controller发来的event: connect complete event


7. 传统蓝牙pincode配对和安全简单配对(SSP: security simple pairing)流程
pincode配对是最古老的配对方式, 配对的时候需要用户自己输入pincode来实现配对
7.1 pincode的HCI流程:
1)controller向houst发送pincode请求事件 PIN code request event	HCI_PIN_Code_Request (参数BD_ADDR)
2)host向controller发送pincode请求回应的command	HCI_PIN_Code_Request_Reply(参数BD_ADDR, pincodeLenght, pincode)
pincodeLenght范围0x01~0x10
3)controller发送command complete(status BD_ADDR)

7.2 蓝牙SSP配对方式(简单安全配对 simple secure pairing)
就是点蓝牙连接的时候, 手机直接弹出配对码, 对方输入正确的配对码来进行配对
1)host先向controller发送Write enable SSP的command, 
2)Set Event Mask, 允许controller向host发送相关的event
3)host接收到controller发来的HCI_IO_Capability_Response event(参数BD_ADDR, IO_Capability, OOB_Data_Present, Authentication_Requirements)
IO_Capability: IO能力, displayOnly, displayYesNO, keyboardOnly, noInputNoOutput
OOB_Data_Present: 是否需要OOB data, 使用带外(out of band)机制来发现设备以及交换或传送将在配对过程中使用的加密信息等场景
Authentication_Requirements: 是否需要auth(0x00不需要, 0x01~0x05:MITM)
4)host接收到controller发来的IO Capability Request请求事件(参数BD_ADDR)
5)host发送IO Capability Request Reply的command: HCI_IO_Capability_Request_Reply(参数BD_ADDR, IO_Capability, OOB_Data_Present, Authentication_Requirements)
并接收到controller发来的command complete with opcode
参数与HCI_IO_Capability_Response_Event的参数类似
6)host接收到用户确认请求事件 HCI_User_Confirmation_Request(参数:BD_ADDR, Numeric_Value随机数)
7)host发送User Confirmation Request Reply command, 并接收到command complete with opcode
8)host接收到controller发来的Simple Pairing Complete event(前提是步骤2已经设置好)
DisplayOnly只是需要显示随机数字就好了
DisplayYesNo让user来决定是否要配对或者不配对
KeyBoardonly让用户通过键盘来输入配对码
NoInputNoOutput啥也不需要显示
(手机表现不太一样)

8. L2CAP概念 
逻辑链路控制和适配层协议(Logical Link Control and Adaptation Layer Protocol), 缩写为L2CAP.
通过协议多路复用 分段重组操作和组概念, 向高层提供面向连接的和无连接的数据服务, L2CAP还屏蔽了低层传输协议中的很多特性, 
使得高层协议应用开发人员可以不必了解基层协议而进行开发, L2CAP在整个协议栈中的位置是架在上层协议和HCI中间
Resource Manager有分包/组包, 重传/流控, 封装/调度
Channel Manager有连接/断开/交互频道参数等

Upper layer:
Lower layer:
L2CAP channel:
SDU: Services Data Unit
SDU Segment: 
Segmentation: 
Reassemble: 
PDU: Protocol Data Unit
Basic L2CAP Header: 
Basic information frame(B-frame): 
Information frame(I-frame):
Supervisory frame(S-frame):
Control frame(C-frame):
Gproup frame(G-frame):
Credit-based frame(K-frame):
Fragment:
Fragmentation:
Recombination:
MTU(Maximum Transmission Unit):
MPS(Maximum PDU payload Size):
Signaling MTU:
Connectionless MTU:
MaxTransmit:

8.1 信道标识符(Channel Identifier CID)
CID表示逻辑信道本地端设备的名字
经典蓝牙使用到的CID:
0x0000 Null identifier
0x0001 L2CAP Signaling channel 
0x0002 Connectionless channel
0x0003 AMP Manager protocol
0x0007 BR/EDR Security Manager
0x003F AMP Test Manager
0x0040~0xFFFF Dynamically allocated

8.2 L2CAP操作模式
L2CAP一共有以下几个操作模式
Basic L2CAP Mode, 基本模式(默认模式)
Flow Control Mode, 此模式下不会进行重传, 但丢失的数据能被检测到, 并报告丢失
Retransmission Mode, 此模式确保数据包都能成功的传输给对端设备
Enhanced Retransmission Mode, 此模式和重传模式类似, 加入了Poll-bit等提高恢复效率
Streaming Mode, 此模式是为了真实的实时传输, 数据包被编号但是不需要ACK确认 设定一个超时定时器, 一旦定时器超时就将超时数据冲掉
LE Credit Based Flow Control Mode, 被用于LE设备通讯
Enhanced Credit BasedFlow Control Mode

9. L2CAP数据格式
没有特殊说明的情况, 一般使用小端模式
9.1 面向连接的基本模式数据格式(CONNECTION-ORIENTEDCHANNELS IN BASIC L2CAP MODE)称为B-frame
Length + channel ID + information payload
(Length + channel ID: basic L2CAP header)
Length: Information payload的长度(16bit)
channel ID: The channel ID (CID) identifies the destination channel endpoint of the packet(16bit)
Infomation payload：就是后面的数据, 有可能是L2CAP本身的(比如signal通道), 也有可能是上层协议

9.2 无连接的基本模式数据格式(CONNECTIONLESS DATACHANNEL IN BASIC L2CAP MODE)称为G-frame
Length + 0x0002 + PSM + information pyload
Length:(InFormation payload + PSM)的长度(16bit)
CID固定为0x0002
PSM(Protocol/Service Multiplexer): 主要就是用于标识协议的(>=16bit)

Protocol				PSM				Reference
SDP						0x0001			See Bluetooth Service Discovery Protocol (SDP), Bluetooth SIG
RFCOMM					0x0003			See RFCOMM with TS 07.10, Bluetooth SIG
TCS-BIN					0x0005			See Bluetooth Telephony Control Specification / TCS Binary, Bluetooth SIG
TCS-BIN-CORDLESS		0x0007			See Bluetooth Telephony Control Specification / TCS Binary, Bluetooth SIG
BNEP					0x000F			See Bluetooth Network Encapsulation Protocol, Bluetooth SIG
HID_Control				0x0011			See Human Interface Device, Bluetooth SIG
HID_Interrupt			0x0013			See Human Interface Device, Bluetooth SIG
UPnP					0x0015			See [ESDP], Bluetooth SIG
AVCTP					0x0017			See Audio/Video Control Transport Protocol, Bluetooth SIG
AVDTP					0x0019			See Audio/Video Distribution Transport Protocol, Bluetooth SIG
AVCTP_Browsing			0x001B			See Audio/Video Remote Control Profile, Bluetooth SIG
UDI_C-Plane				0x001D			See the Unrestricted Digital Information Profile [UDI], Bluetooth SIG
ATT						0x001F			See Bluetooth Core Specification​
​3DSP					0x0021​	​​		See 3D Synchronization Profile, Bluetooth SIG.
​LE_PSM_IPSP	​			0x0023			​See Internet Protocol Support Profile (IPSP), Bluetooth SIG
OTS						0x0025			See Object Transfer Service (OTS), Bluetooth SIG
EATT					0x0027			See Bluetooth Core Specification

InFormation payload：上层协议的数据

9.3 信号封包的数据格式(SIGNALING PACKET FORMATS)
L2CAP的命令叫做signaling command(简称C-frame), 所有的命令都是通过signaling channel来发送, 
传统蓝牙的CID是固定的0x0001, 低功耗的CID是固定的0x0005
命令格式为:
Lenght + channel ID + Information payload

channel固定0x0001或0x0005
在signaling command的information payload的格式为:
Code(1Byte) + identifier(1Byte) + length(2Bytes) + data

Identifier(1octet): 用于标示command的发送序列, response必须跟request相同
Length(2octets): 用于标示后续的data长度
Data(0 or more octets): 针对不同的signaling command, 后续的data是不同的, 具体command具体分析

9.3.1 信号封包介绍
针对于每个SIGNALING来说明
1) L2CAP_COMMAND_REJECT_RSP(CODE 0x01)
Code(0x01 1Byte) + identifier(1Byte) + length(2Bytes) + Reason + Data(option)
Reason(2 octets): 告知remote端为什么发送拒绝封包, reason有以下值:
如果拒绝reason是0x0000, 那么是remote端发送的command id是错误的, data部分则是0byte, 什么都不需要填写
如果拒绝reason是0x0001, 那么就是MTU有问题, data就是2byte的MTU
如果拒绝reason是0x0002, 那么会有4byte的data, 分别来表示local(first) and remote(second) channel endpoints

2) L2CAP_CONNECTION_REQ(CODE 0x02)
发起连接请求
Code(0x02 1Byte) + identifier(1Byte) + length(2Bytes) + PSM + Source CID
PSM: (Protocol/Service Multiplexer)用于标识协议的(同G-frame)
Source CID:连接的放用source ID, 也就是本地的CID 动态申请的0x0040~0xFFFF

3) L2CAP_CONNECTION_RSP(CODE 0x03)
响应发起连接请求
Code(0x03 1Byte) + identifier(1Byte) + length(2Bytes) + Destination CID + Source CID + Result(2Bytes) + Status(2Bytes)
Destination CID: 可以认为是发送response的本地CID
Source CID：发送L2CAP_CONNECTION_REQ的CID
Result: 连接的结果
0x0000 Connection successful
0x0001 Connection pending
0x0002~0x0007 Connection refuse...
Status:只有当Result = 0x0001 connection pending会用到
0x0000 no further information available
0x0001 authentication pending
0x0002 authorization pending

4) L2CAP_CONFIGURATION_REQ(CODE 0x04)
发起配置请求
Code(0x03 1Byte) + identifier(1Byte) + length(2Bytes) + Destination CID + Flags(2Bytes) + Configuration option

5) L2CAP_CONFIGURATION_RSP(CODE 0x05)
发起配置回应

6) L2CAP_DISCONNECTION_REQ(CODE 0x06)
L2CAP channel断开请求

7) L2CAP_DISCONNECTION_RSP(CODE 0x07)
断开请求的reponse

8) L2CAP_ECHO_REQ(CODE 0x08)
用来请求一个L2CAP的ECHO response

9) L2CAP_ECHO_RSP(CODE 0x09)
回复一个echo request

10) L2CAP_INFORMATION_REQ(CODE 0x0A)
信息请求, 用来请求一些信息

11) L2CAP_INFORMATION_RSP(CODE 0x0B)
Information请求的response


9.4 传统L2CAP的连接流程
前面使用signaling command C-frame 后面使用基本模式B-frame
9.4.1 主机发送L2CAP Connection Request with RFCOMM PSM
9.4.2 从机回复L2CAP Connection Response
9.4.3 主机发送L2CAP Configure Request配置请求 with MTU
9.4.4 从机回复L2CAP Configure Response
9.4.5 若不成功 从机可以发送L2CAP Configure Request配置请求 with MTU重新配置
9.4.6 后续SDU的交互 B-frame格式:
Length + channel ID + information payload

