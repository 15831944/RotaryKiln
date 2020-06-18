# 回转窑RotaryKiln软件安装激活步骤

## 安装

1. 选择目标位置

  ![image-20200618111147192](回转窑RotaryKiln软件安装激活步骤.assets/image-20200618111147192.png)

2. 执行安装

  ![image-20200618111211615](回转窑RotaryKiln软件安装激活步骤.assets/image-20200618111211615.png)

3. 安装Microsoft Visual C++ 2015 Redistributable (x86) 运行库

  ![image-20200618111224975](回转窑RotaryKiln软件安装激活步骤.assets/image-20200618111224975.png)

  ![image-20200618111347999](回转窑RotaryKiln软件安装激活步骤.assets/image-20200618111347999.png)

4. 注意：请检查电脑安装的Office版本

  - 系统未安装Office，则需要执行安装目录下（默认C:\Program Files (x86)\RotaryKiln）Engine_install.bat;
  - 系统安装了Office-x64，则需要执行安装目录下（默认C:\Program Files (x86)\RotaryKiln）Engine_install_passive.bat;
  - 系统安装了Office-x86，则不需要安装额外驱动。

  ![image-20200618111358672](回转窑RotaryKiln软件安装激活步骤.assets/image-20200618111358672.png)

- 如何检测Office是x64还是x86？

以Office2016为例，可在Excel中，点击【文件】→【账户】→【关于Excel】看到。

  ![image-20200618111554294](回转窑RotaryKiln软件安装激活步骤.assets/image-20200618111554294.png)

  ![image-20200618111447748](回转窑RotaryKiln软件安装激活步骤.assets/image-20200618111447748.png)

- 如何安装驱动？

请执行对应的批处理文件，步骤如下

  ![image-20200618111702776](回转窑RotaryKiln软件安装激活步骤.assets/image-20200618111702776.png)

  ![image-20200618111634327](回转窑RotaryKiln软件安装激活步骤.assets/image-20200618111634327.png)

  ![image-20200618111728970](回转窑RotaryKiln软件安装激活步骤.assets/image-20200618111728970.png)

  ![image-20200618111736706](回转窑RotaryKiln软件安装激活步骤.assets/image-20200618111736706.png)

  ![image-20200618111749594](回转窑RotaryKiln软件安装激活步骤.assets/image-20200618111749594.png)

  ![image-20200618111808553](回转窑RotaryKiln软件安装激活步骤.assets/image-20200618111808553.png)

  ![image-20200618111821592](回转窑RotaryKiln软件安装激活步骤.assets/image-20200618111821592.png)

5. 安装完成

  ![image-20200618111844714](回转窑RotaryKiln软件安装激活步骤.assets/image-20200618111844714.png)


## 激活

执行软件，复制机器注册窗口中的本机序列号，将对应的注册码填入并确定即完成注册，或点击试用，可试用30min。

  ![image-20200618111945705](回转窑RotaryKiln软件安装激活步骤.assets/image-20200618111945705.png)

## 问题


1.   ![image-20200617094731428](回转窑RotaryKiln软件安装激活步骤.assets/image-20200617094731428.png)

  此问题时由于Microsoft Visual C++ 2015 Redistributable (x86) 运行库未安装导致的，请执行安装目录下的vc_redist.x86.exe来修复问题。

2.   ![image-20200617090324259](回转窑RotaryKiln软件安装激活步骤.assets/image-20200617090324259.png)

  此问题是由于缺少可用的32位Access驱动，请参照安装步骤4来安装驱动程序。

3.   ![image-20200617091407859](回转窑RotaryKiln软件安装激活步骤.assets/image-20200617091407859.png)

  此问题是由于数据库丢失或被移动（默认db/RotaryKiln.accdb），如果数据库文件丢失，可以备份文件后重新安装软件，并用备份文件覆盖安装目录。

