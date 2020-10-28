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




