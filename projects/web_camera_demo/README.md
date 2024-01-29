# 简介

本示例演示网口网页摄像头功能

## 示例展示的功能

本例程使用OV5640摄像头并内置简易网页服务器，PC浏览器通过访问网页服务器获取jpeg图像流进行显示

## 硬件设置
* 使用USB Type-C线缆连接PC USB端口和PWR DEBUG端口
* 使用以太网线缆将开发板RGMII或RMII端口接入路由器中，路由器需要开启DHCP功能。
* 所在的局域网需要能访问互联网。
* OV5640摄像头插入到对应的24P fpc座子上

## 完整的测试步骤

- 等待lwip和网口、网页服务器相关初始化成功，若成功会出现以下Log

```console
 \ | /
- RT -     Thread Operating System
 / | \     5.0.1 build Dec 14 2023 15:51:02
 2006 - 2022 Copyright by RT-Thread team
lwIP-2.1.2 initialized!
[15] I/I2C: I2C bus [i2c0] registered
[20] I/sal.skt: Socket Abstraction Layer initialize success.
I2C bus is ready
msh />http_mjpeg_server...
[3040] I/NO_TAG: PHY Status: Link up
[3044] I/NO_TAG: PHY Speed: 100Mbps
[3048] I/NO_TAG: PHY Duplex: full duplex

web IP address has find....
WebServer Waiting for client on port 80...

```

- 完成后，查询当前节点的网络信息，记住本IP地址

```console
msh />ifconfig
network interface device: w0 (Default)
MTU: 1500
MAC: fc 58 4a 2c 48 17
FLAGS: UP LINK_UP INTERNET_UP DHCP_ENABLE ETHARP BROADCAST IGMP
ip address: 192.168.0.101
gw address: 192.168.0.1
net mask  : 255.255.255.0
dns server #0: 192.168.1.1
dns server #1: 192.168.0.1
```

- 若以上已就绪，在PC浏览器上，输入本IP:80，比如本机: 192.168.0.101:80， 就可以在网页上看到图像流，同时会有相关Log打印

```console
msh />
StreamConnected=4,192.168.0.107 , 58737

uri=GET / HTTP/1.1

bytes_received <= 0,lwip close...

StreamConnected=4,192.168.0.107 , 58738

uri=GET /?hpm6750evk2=mjpeg HTTP/1.1

 Now Ok!!

```


