# Nwatch-Stm32f103

本文参考:

[WatchX-Nwatch-stm32](https://github.com/Soysauce007/WatchX-Nwatch-stm32)

[自制环境监测手表——”E-watch“，亲身开发才知道一个产品要做好有多难，怪不得科技产品卖那么贵](https:www.bilibili.com/video/BV1XJ411M7yT/?spm_id_from=333.788.videocard.0)

[自制基于stc8单片机的oled屏手表](https:www.bilibili.com/video/BV1eE411j76K/?spm_id_from=333.788.videocard.0)

# 硬件介绍

主控：stm32f103c8t6

屏幕：1.3寸oled屏 ssd1306

实时时钟：PCF8563 

锂电池充电芯片：XT4054 



# 设计缺陷

- 由于当时想将蓝牙与这个结合在一起，所以预留usart1给蓝牙模块了,所以没有设计TTL转串口芯片，需要st-link或者j-link下载程序（但是！我的蓝牙芯片烧了，肉痛！）

- 没设计电源按键和硬件复位，如果程序死机，要么选择程序刷固件，要么等待电池狗带，或者取下电源线。

- 程序下载时，保证单片机正常供电，我使用的st-link不供电。

- 板子设计的时候没有考虑外接的mpu6050和pw02的高度，所以存在结构缺陷。

- 时钟芯片的封装大了一号，但是也能焊上去。（懒得换了）

- 为了考虑手工焊接，大部分元器件使用的都是0805封装，所以相对紧凑一点。（其中包含了两个0603的贴片led）

- 在未焊接好按键的时候测试程序的时候，建议注释掉以下两句：（不然就只能用按键在亮屏时刻烧写固件，因为停机模式所以时钟停止工作！）

  ```
       WWDG_Init(0x7F,0X5F,WWDG_Prescaler_8);//窗口卡门狗，  算是弥补上文未设置硬件复位和电源键的缺陷吧     
       pwrmgr_update();  //  内部开启了停机模式
  ```

  

- 蜂鸣器带有一丝杂音，硬件缺陷啊。抄的板子，不会改，但是在开启停机模式后可弥补。

- usb插入无法检测

- 音量调节无法实现 本以为pwm可以实现该功能，但是还是太年轻，预留的PA1死活开启不了，只能折中开启一个定时器模拟pwm，但是问题是很占用资源。

- 时钟不太准，建议自己调节PCF8563 的晶振，我设计的不是很合理。

- 电量检测不准确

- 蜂鸣器的焊盘有点小了，但是能怼上去

# 源码及其PCB展示

![原理图](https://upload-images.jianshu.io/upload_images/7994561-1402356f23bdb024.png?imageMogr2/auto-orient/strip|imageView2/2/w/1194/format/webp)



![主控PCB](https://upload-images.jianshu.io/upload_images/7994561-955556b7e0086f82.png?imageMogr2/auto-orient/strip|imageView2/2/w/989/format/webp)

![源码](https://upload-images.jianshu.io/upload_images/7994561-cccff528bd0663fc.png?imageMogr2/auto-orient/strip|imageView2/2/w/1200/format/webp)

![焊接好的硬件](https://upload-images.jianshu.io/upload_images/7994561-2ca558370dc05215.jpg?imageMogr2/auto-orient/strip|imageView2/2/w/1200/format/webp)

![测试效果](https://upload-images.jianshu.io/upload_images/7994561-c03f7c26aca4a77d.jpg?imageMogr2/auto-orient/strip|imageView2/2/w/1200/format/webp)



# 购买记录

![元器件购买记录](https://upload-images.jianshu.io/upload_images/7994561-7daabe04a1ad2dca.jpg?imageMogr2/auto-orient/strip|imageView2/2/w/1080)

以上是购买元器件记录，我本身自己有两块stm32f103c8t6，所以并未记录于其内部。

另外请忽略我购买的几个络铁头！



因为还有很多设计缺陷，感觉入不了大佬们的法眼。也没啥意思去得瑟，既然你看到了这里，那么我就将它赠与有缘人，喜欢玩的就拿去玩吧。当然如果你们有其它更好的想法，请务必在下方留言，让我瞻仰一波。



能力实在有限，希望各位大佬批评指正！！！



下载链接：

github:   https://github.com/Soysauce007/Nwatch-Stm32f103.git

百度云：链接：https://pan.baidu.com/s/1QRo6dB6eNtfuYiOQnF_FOA 
               提取码：y2z9





