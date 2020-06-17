# 回转窑RotaryKiln软件安装激活步骤

## 安装

1. 选择目标位置

  ![image-20200617084516858](C:\Users\Admin\AppData\Roaming\Typora\typora-user-images\image-20200617084516858.png)

2. 执行安装

  ![image-20200617084708387](C:\Users\Admin\AppData\Roaming\Typora\typora-user-images\image-20200617084708387.png)

3. 安装Microsoft Visual C++ 2015 Redistributable (x86) 运行库

  ![image-20200617084759455](C:\Users\Admin\AppData\Roaming\Typora\typora-user-images\image-20200617084759455.png)

  ![image-20200617084939810](C:\Users\Admin\AppData\Roaming\Typora\typora-user-images\image-20200617084939810.png)

4. 注意：请检查电脑安装的Office版本

  - 系统未安装Office，则需要执行安装目录下（默认C:\Program Files (x86)\RotaryKiln）Engine_install.bat;
  - 系统安装了Office-x64，则需要执行安装目录下（默认C:\Program Files (x86)\RotaryKiln）Engine_install_passive.bat;
  - 系统安装了Office-x86，则不需要安装额外驱动。

  ![image-20200617084958563](C:\Users\Admin\AppData\Roaming\Typora\typora-user-images\image-20200617084958563.png)

- 如何检测Office是x64还是x86？

可在Excel中，点击【文件】→【账户】→【关于Excel】看到。

![image-20200617112935410](回转窑RotaryKiln软件安装激活步骤.assets/image-20200617112935410.png)

- 如何安装驱动？

请执行对应的批处理文件，步骤如下

  ![image-20200617090928503](C:\Users\Admin\AppData\Roaming\Typora\typora-user-images\image-20200617090928503.png)

  ![image-20200617090947963](C:\Users\Admin\AppData\Roaming\Typora\typora-user-images\image-20200617090947963.png)

  ![image-20200617091005915](C:\Users\Admin\AppData\Roaming\Typora\typora-user-images\image-20200617091005915.png)

  ![image-20200617091027835](C:\Users\Admin\AppData\Roaming\Typora\typora-user-images\image-20200617091027835.png)

  ![image-20200617091105796](C:\Users\Admin\AppData\Roaming\Typora\typora-user-images\image-20200617091105796.png)

  ![image-20200617091119195](C:\Users\Admin\AppData\Roaming\Typora\typora-user-images\image-20200617091119195.png)

5. 安装完成

  ![image-20200617085725277](C:\Users\Admin\AppData\Roaming\Typora\typora-user-images\image-20200617085725277.png)


## 激活

执行软件，复制机器注册窗口中的本机序列号，将对应的注册码填入并确定即完成注册。

![image-20200617085930547](C:\Users\Admin\AppData\Roaming\Typora\typora-user-images\image-20200617085930547.png)

## 问题


1. ![image-20200617094731428](回转窑RotaryKiln软件安装激活步骤.assets/image-20200617094731428.png)

  此问题时由于Microsoft Visual C++ 2015 Redistributable (x86) 运行库未安装导致的，请执行安装目录下的vc_redist.x86.exe来修复问题。

2. ![image-20200617090324259](C:\Users\Admin\AppData\Roaming\Typora\typora-user-images\image-20200617090324259.png)

  此问题是由于缺少可用的32位Access驱动，请参照安装步骤4来安装驱动程序。

3. ![image-20200617091407859](C:\Users\Admin\AppData\Roaming\Typora\typora-user-images\image-20200617091407859.png)

  此问题是由于数据库丢失或与配置文件路径不一致，配置文件application.cfg的DatabasePath保存着数据库路径（默认db/RotaryKiln.accdb），请移动数据库文件或修改配置文件以保持一致，如果数据库文件丢失，可以备份文件后重新安装软件，并用备份文件覆盖安装目录。

