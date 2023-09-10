# EmbeddedPrinter

这是一个Arduino库，专门针对于[歌翼科技](http://szgykjyx.sotongwang.com/)的嵌入式热敏打印机，库的测试机型为EH405，是12V，TTL电平的。库中定义了 **EmbbededPrinter** 类，可以简单认为这个库为一个中间层的库，基于串口，以串口实例指针构造打印机实例，在类中直接操作指定的串口与打印机进行交互。

## API

这里列出类中实现的功能性的方法，介绍的时候会说明`歌翼科技-通用指令集`中对应的命令。这里描述功能比较简单，如想了解更详细的内容可以参考[通用指令集](http://www.dp-geyi.com/res/soft/2017/a206cfbb9f227d28.rar)。

### nextline(void)

1. 对应命令： `LF` 和 `CR`
2. 功能：打印并换行回车
3. 参数：无
4. 返回值：无

### tab(void)

1. 对应命令： `HT`
2. 功能：跳到下一个TAB位置
3. 参数：无
4. 返回值：无

### setTab(uint8_t *d, uint8_t k)

1. 对应命令：`ESC D n`
2. 功能：设置水平定位点，也就是制表符位置
3. 参数：
  - `*d` -> 是一个数组, 以8个点为一个单位，数值范围[1, 46]
  - `k` -> 水平定位点即制表符数量，范围[1, 16]
4. 返回值：无

### printblank(uint8_t n)

1. 对应命令：`ESC j n`
2. 功能：打印缓冲区数据并进纸(n x 0.125mm)
3. 参数：n，进纸长度，范围[0, 255]，单位0.125mm
4. 返回值：无

### printlines(uint8_t n)

1. 对应命令：`ESC d n`
2. 功能：打印缓冲区数据并进纸n行
3. 参数：n，进纸行数，范围[0, 255]
4. 返回值：无

### on(void)

1. 对应命令：`ESC = n`
2. 功能：设置打印机为在线模式，接受打印数据并打印
3. 参数：无
4. 返回值：无

### off(void)

1. 对应命令：`ESC = n`
2. 功能：设置打印机为离线模式，不接受打印数据
3. 参数：无
4. 返回值：无

### linespace(void)

1. 对应命令：`ESC 2`
2. 功能：选择行间距为默认的3.75mm(30*0.125mm)
3. 参数：无
4. 返回值：无

### linespace(uint8_t n)

1. 对应命令：`ESC 3 n`
2. 功能：设置行间距为[n * 0.125mm]
3. 参数：n，单位点即0.125mm，范围[0, 255]
4. 返回值：无

### align(Alignment pos)

1. 对应命令：`ESC a n`
2. 功能：将一行数据按照指定方式对齐：居左，居中，居右
3. 参数：pos，为枚举类型`Alignment`，有三个值：Left, Center, Right
4. 返回值：无

### big(bool flag)

1. 对应命令：`ESC SO`和`ESC DC4`
2. 功能：设置或取消字符为倍增模式，即宽和高上放大为原来的两倍或恢复默认
3. 参数：flag，bool类型，为真时设置为倍增模式，假时取消倍增模式
4. 返回值：无

### margin(uint16_t n)

1. 对应命令：`GS L nL nH`
2. 功能：设置左边空白[n x 0.125mm]
3. 参数：n，单位0.125mm
4. 返回值：无

### cursor(uint16_t n)

1. 对应命令：`ESC $ nL nH`
2. 功能：设置本行内打印字符的绝对位置，有点光标的意思，所以命名为cursor
3. 参数：n，单位0.125mm
4. 返回值：无

### indent(uint8_t n)

1. 对应命令：`ESC B n`
2. 功能：设置一行内的缩进
3. 参数：n，单位为一个字符，范围[0, 47]
4. 返回值：无

### setMode(CharMode mode)

1. 对应命令：`ESC ! n`
2. 功能：设置字符打印模式
3. 参数：mode，为联合体类型，针对每一位代表的含义而定义
4. 返回值：无

### big(CharBigTimes bigTimes)

1. 对应命令：`GS ! n`
2. 功能：设置字体大小
3. 参数：bigTimes，字体放大倍数的联合体，其中包含了宽度和高度的相应增大倍数
4. 返回值：无

### big(uint8_t times)

1. 对应命令：`GS ! n`
2. 功能：设置字体大小
3. 参数：times，宽度和高度上字体放大倍数的均设置为这个数值
4. 返回值：无

### reverse(bool flag)

1. 对应命令：`GS B n`
2. 功能：设置或取消字体的反白模式
3. 参数：flag，bool类型，真时设置为泛白模式，反之取消
4. 返回值：无

### rotate(bool flag)

1. 对应命令：`ESC V n`
2. 功能：设置或取消字符的90度顺时针旋转
3. 参数：flag，bool类型，真时设置旋转，反之取消
4. 返回值：无

### status(void)

1. 对应命令：`ESC v n`
2. 功能：返回打印机的状态：机芯连接状态、有无纸、电压、温度及另外添加的超时
3. 参数：无
4. 返回值：st，类型为联合体`PrinterStatus`，包含了各个状态的位域定义

### overprint(bool flag)

1. 对应命令：`ESC G n`
2. 功能：设定或解除重叠打印模式
3. 参数：flag，bool类型，真时设置为重叠打印模式，反之取消
4. 返回值：无

### bold(bool flag)

1. 对应命令：`ESC E n`
2. 功能：设置或解除字体加粗
3. 参数：flag，bool类型，真时设置字体加粗，反之解除
4. 返回值：无

### space(uint8_t n)

1. 对应命令：`ESC SP n`
2. 功能：设置右侧字符间距
3. 参数：设置字符右侧间距[n x 0.125mm]
4. 返回值：无

### updown(bool flag)

1. 对应命令：`ESC ( n`
2. 功能：设置或取消字符上下颠倒
3. 参数：flag，bool类型，真时设置字符上下颠倒，反之取消
4. 返回值：无

### underline(uint8_t flag)

1. 对应命令：`ESC - n`
2. 功能：设置字符下划线的粗细
3. 参数：flag，只有三种值：0，1，2
4. 返回值：无

### print(char *str)

1. 对应命令：无
2. 功能：打印字符串
3. 参数：字符串
4. 返回值：无

### print(int num)

1. 对应命令：无
2. 功能：打印整数
3. 参数：整数
4. 返回值：无

### print(double num)

1. 对应命令：无
2. 功能：打印浮点数
3. 参数：浮点数
4. 返回值：无
