# 网络连通性及吞吐性能测试

## 概述

本示例演示了基于RT-Thread 网络连通性测试和吞吐性能测试

## 硬件设置

* 使用USB Type-C线缆连接PC USB端口和PWR DEBUG端口
* 使用以太网线缆连接PC以太网端口和开发板RGMII或RMII端口

## 运行示例

* 编译下载程序
* 串口终端显示

```console
 \ | /
- RT -     Thread Operating System
 / | \     5.0.2 build Apr 19 2025 10:18:07
 2006 - 2022 Copyright by RT-Thread team
lwIP-2.1.2 initialized!
[27] I/sal.skt: Socket Abstraction Layer initialize success.
msh />[4067] I/NO_TAG: ENET0
[4070] I/NO_TAG: PHY Status: Link up
[4074] I/NO_TAG: PHY Speed: 1000Mbps
[4078] I/NO_TAG: PHY Duplex: full duplex
```

## 功能验证

### 1. IP分配查询及DHCP状态确认

```console
msh />ifconfig
network interface device: ET (Default)
MTU: 1500
MAC: 98 2c bc b1 9f 17
FLAGS: UP LINK_UP INTERNET_DOWN DHCP_ENABLE ETHARP BROADCAST
ip address: 192.168.100.6
gw address: 192.168.100.1
net mask  : 255.255.255.0
dns server #0: 192.168.100.1
dns server #1: 0.0.0.0

```

**注： 若DHCP开启，则DHCP状态为“DHCP_ENABLE”，需要将网口连接至路由器或具有DHCP服务的PC  **   

### 2. PING测试

  （1）Windows系统中，打开cmd, 运行ping

```console
C:\Users>ping 192.168.100.6

正在 Ping 192.168.100.6 具有 32 字节的数据:
来自 192.168.100.6 的回复: 字节=32 时间<1ms TTL=255
来自 192.168.100.6 的回复: 字节=32 时间<1ms TTL=255
来自 192.168.100.6 的回复: 字节=32 时间<1ms TTL=255
来自 192.168.100.6 的回复: 字节=32 时间<1ms TTL=255

192.168.100.6 的 Ping 统计信息:
    数据包: 已发送 = 4，已接收 = 4，丢失 = 0 (0% 丢失)，
往返行程的估计时间(以毫秒为单位):
    最短 = 0ms，最长 = 0ms，平均 = 0ms
```

  （2）开发板Ping PC

```console
msh />ping 192.168.100.5
ping: not found specified netif, using default netdev ET.
60 bytes from 192.168.100.5 icmp_seq=0 ttl=64 time=0 ms
60 bytes from 192.168.100.5 icmp_seq=1 ttl=64 time=0 ms
60 bytes from 192.168.100.5 icmp_seq=2 ttl=64 time=0 ms
60 bytes from 192.168.100.5 icmp_seq=3 ttl=64 time=0 ms

```

### 3. **iperf测试**

- **TCP服务端模式**

  - MCU端输入命令

    ```console
    msh /> iperf -s
    ```

  -  PC端输入命令

    ```console
    C:\Users>iperf -c 192.168.100.6 -i 1
    ```

  - 观察PC端结果

    ```console
    ------------------------------------------------------------
    Client connecting to 192.168.100.6, TCP port 5001
    TCP window size: 64.0 KByte (default)
    ------------------------------------------------------------
    [360] local 192.168.100.5 port 55344 connected with 192.168.100.6 port 5001
    [ ID] Interval       Transfer     Bandwidth
    [360]  0.0- 1.0 sec  58.2 MBytes   488 Mbits/sec
    [360]  1.0- 2.0 sec  58.7 MBytes   492 Mbits/sec
    [360]  2.0- 3.0 sec  58.1 MBytes   487 Mbits/sec
    [360]  3.0- 4.0 sec  57.9 MBytes   486 Mbits/sec
    [360]  4.0- 5.0 sec  58.0 MBytes   486 Mbits/sec
    [360]  5.0- 6.0 sec  57.0 MBytes   478 Mbits/sec
    [360]  6.0- 7.0 sec  57.8 MBytes   485 Mbits/sec
    [360]  7.0- 8.0 sec  57.6 MBytes   483 Mbits/sec
    [360]  8.0- 9.0 sec  57.2 MBytes   480 Mbits/sec
    [360]  9.0-10.0 sec  57.4 MBytes   482 Mbits/sec
    [360]  0.0-10.0 sec   578 MBytes   484 Mbits/sec
    ```

  - 观察MCU端结果

    ```console
    msh />[555987] I/iperf: new client connected from (192.168.100.5, 55344)
    [560994] I/iperf: iperfd01: 488.6140 Mbps!
    [565984] W/iperf: client disconnected (192.168.100.5, 55344)
    ```

    

- **TCP客户端模式**

  - PC端输入命令

    ```console
    C:\Users>iperf -s -i 1
    ```

  - MCU端输入命令

    ```console
    msh />iperf -c 192.168.100.5
    ```

  - 观察PC端结果

    ```console
    ------------------------------------------------------------
    Server listening on TCP port 5001
    TCP window size: 64.0 KByte (default)
    ------------------------------------------------------------
    [412] local 192.168.100.5 port 5001 connected with 192.168.100.6 port 52681
    [ ID] Interval       Transfer     Bandwidth
    [412]  0.0- 1.0 sec  45.6 MBytes   382 Mbits/sec
    [412]  1.0- 2.0 sec  48.3 MBytes   405 Mbits/sec
    [412]  2.0- 3.0 sec  48.9 MBytes   410 Mbits/sec
    [412]  3.0- 4.0 sec  49.0 MBytes   411 Mbits/sec
    [412]  4.0- 5.0 sec  48.6 MBytes   408 Mbits/sec
    [412]  5.0- 6.0 sec  49.7 MBytes   417 Mbits/sec
    [412]  6.0- 7.0 sec  49.7 MBytes   417 Mbits/sec
    [412]  7.0- 8.0 sec  49.3 MBytes   414 Mbits/sec
    [412]  8.0- 9.0 sec  49.7 MBytes   417 Mbits/sec
    [412]  9.0-10.0 sec  49.3 MBytes   414 Mbits/sec
    [412] 10.0-11.0 sec  49.8 MBytes   418 Mbits/sec
    [412] 11.0-12.0 sec  48.4 MBytes   406 Mbits/sec
    [412] 12.0-13.0 sec  48.1 MBytes   404 Mbits/sec
    [412] 13.0-14.0 sec  47.9 MBytes   402 Mbits/sec
    [412] 14.0-15.0 sec  48.3 MBytes   405 Mbits/sec
    [412] 15.0-16.0 sec  48.2 MBytes   404 Mbits/sec
    [412] 16.0-17.0 sec  47.5 MBytes   398 Mbits/sec
    [412] 17.0-18.0 sec  48.6 MBytes   408 Mbits/sec
    [412] 18.0-19.0 sec  48.7 MBytes   408 Mbits/sec
    [412] 19.0-20.0 sec  48.0 MBytes   402 Mbits/sec
    ```

  - 观察MCU端结果

    ```console
    msh />[1799569] I/iperf: Connect to iperf server successful!
    [1804574] I/iperf: iperfc01: 403.3670 Mbps!
    [1809574] I/iperf: iperfc01: 415.7140 Mbps!
    [1814574] I/iperf: iperfc01: 407.0830 Mbps!
    [1819574] I/iperf: iperfc01: 404.4160 Mbps!
    ```

- **UDP服务端模式**

  - MCU端输入命令

    ```console
    msh />iperf -u -s
    ```

  - PC端输入命令

    ```console
    C:\Users>iperf -u -c 192.168.100.6 -i 1 -b 1000M
    ```

  - 观察PC端结果

    ```console
    ------------------------------------------------------------
    Client connecting to 192.168.100.6, UDP port 5001
    Sending 1470 byte datagrams
    UDP buffer size: 64.0 KByte (default)
    ------------------------------------------------------------
    [360] local 192.168.100.5 port 51565 connected with 192.168.100.6 port 5001
    [ ID] Interval       Transfer     Bandwidth
    [360]  0.0- 1.0 sec  80.3 MBytes   674 Mbits/sec
    [360]  1.0- 2.0 sec  77.7 MBytes   652 Mbits/sec
    [360]  2.0- 3.0 sec  80.3 MBytes   673 Mbits/sec
    [360]  3.0- 4.0 sec  80.7 MBytes   677 Mbits/sec
    [360]  4.0- 5.0 sec  78.5 MBytes   659 Mbits/sec
    [360]  5.0- 6.0 sec  76.8 MBytes   644 Mbits/sec
    [360]  6.0- 7.0 sec  75.1 MBytes   630 Mbits/sec
    [360]  7.0- 8.0 sec  79.1 MBytes   663 Mbits/sec
    [360]  8.0- 9.0 sec  79.8 MBytes   670 Mbits/sec
    [360]  9.0-10.0 sec  80.4 MBytes   675 Mbits/sec
    [360] 10.0-11.0 sec  78.4 MBytes   658 Mbits/sec
    [360] 11.0-12.0 sec  79.0 MBytes   663 Mbits/sec
    [360] 12.0-13.0 sec  76.2 MBytes   639 Mbits/sec
    [360] 13.0-14.0 sec  79.3 MBytes   665 Mbits/sec
    [360] 14.0-15.0 sec  79.8 MBytes   670 Mbits/sec
    [360] 15.0-16.0 sec  77.7 MBytes   652 Mbits/sec
    [360] 16.0-17.0 sec  76.8 MBytes   644 Mbits/sec
    [360] 17.0-18.0 sec  76.4 MBytes   641 Mbits/sec
    [360] 18.0-19.0 sec  78.8 MBytes   661 Mbits/sec
    [360] 19.0-20.0 sec  79.0 MBytes   663 Mbits/sec
    ```

  - 观察MCU端结果

    ```console
    msh />[110791] I/iperf: iperfd01: 455.7140 Mbps! lost:24074 total:217829
    
    [115798] I/iperf: iperfd01: 616.4700 Mbps! lost:16601 total:278706
    
    [120805] I/iperf: iperfd01: 632.0850 Mbps! lost:11873 total:280617
    
    [125812] I/iperf: iperfd01: 623.3640 Mbps! lost:13284 total:278320
    
    [131282] I/iperf: iperfd01: 132.7410 Mbps! lost:-2237084 total:-2175429
    ```

    

- UDP客户端模式

  - PC端输入命令

    ```console
    C:\Users>iperf -u -s -i 1
    ```

  - MCU端输入命令

    ```console
    msh />iperf -u -c 192.168.100.5
    ```

  - 观察MCU端结果

    ```console
    ------------------------------------------------------------
    Server listening on UDP port 5001
    Receiving 1470 byte datagrams
    UDP buffer size: 64.0 KByte (default)
    ------------------------------------------------------------
    [344] local 192.168.100.5 port 5001 connected with 192.168.100.6 port 62510
    [ ID] Interval       Transfer     Bandwidth       Jitter   Lost/Total Datagrams
    [344]  0.0- 1.0 sec   100 MBytes   841 Mbits/sec  0.004 ms 1392772550/320199 (4.3e+005%)
    [344]  1.0- 2.0 sec  97.9 MBytes   822 Mbits/sec  0.030 ms 3015/72883 (4.1%)
    [344]  2.0- 3.0 sec   100 MBytes   841 Mbits/sec  0.003 ms 1338/72880 (1.8%)
    [344]  3.0- 4.0 sec   101 MBytes   845 Mbits/sec  0.001 ms 1028/72854 (1.4%)
    [344]  4.0- 5.0 sec  97.7 MBytes   819 Mbits/sec  0.064 ms 3162/72846 (4.3%)
    [344]  5.0- 6.0 sec   100 MBytes   843 Mbits/sec  0.002 ms 1323/72971 (1.8%)
    [344]  6.0- 7.0 sec   102 MBytes   854 Mbits/sec  0.015 ms  172/72827 (0.24%)
    [344]  7.0- 8.0 sec   102 MBytes   857 Mbits/sec  0.004 ms    0/72912 (0%)
    [344]  8.0- 9.0 sec  99.4 MBytes   834 Mbits/sec  0.003 ms 1926/72843 (2.6%)
    [344]  9.0-10.0 sec   102 MBytes   853 Mbits/sec  0.006 ms  338/72902 (0.46%)
    [344] 10.0-11.0 sec   101 MBytes   846 Mbits/sec  0.003 ms  929/72855 (1.3%)
    [344] 11.0-12.0 sec  99.7 MBytes   836 Mbits/sec  0.002 ms 1816/72923 (2.5%)
    [344] 12.0-13.0 sec   102 MBytes   856 Mbits/sec  0.002 ms   50/72839 (0.069%)
    [344] 13.0-14.0 sec   102 MBytes   858 Mbits/sec  0.034 ms    0/72934 (0%)
    [344] 14.0-15.0 sec  90.6 MBytes   760 Mbits/sec  0.005 ms 8240/72874 (11%)
    [344] 15.0-16.0 sec   102 MBytes   857 Mbits/sec  0.002 ms    2/72852 (0.0027%)
    [344] 16.0-17.0 sec   102 MBytes   857 Mbits/sec  0.003 ms    0/72909 (0%)
    [344] 17.0-18.0 sec   102 MBytes   857 Mbits/sec  0.003 ms    0/72841 (0%)
    [344] 18.0-19.0 sec   102 MBytes   858 Mbits/sec  0.002 ms    0/72918 (0%)
    [344] 19.0-20.0 sec   101 MBytes   844 Mbits/sec  0.002 ms 1084/72843 (1.5%)
    ```

    

  - 观察MCU端结果

    ```console
    [66131] I/iperf: iperf udp mode run...
    ```

    **注：此模式下，MCU端无统计信息输出，且无退出机制，需要按reset键重启MCU。**

  

  

