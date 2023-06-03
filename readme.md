# qpid软件包

## 1.简介

**qpid** 软件包是PID自动控制算法的C语言实现，支持增量型和位置型两种算法。

### 1.1目录结构

`qpid` 软件包目录结构如下所示：

``` 
qpid
├───inc                     // 头文件目录
│   └───qpid.h           	// API 接口头文件
├───src                     // 源码目录
│   |   qpid.c              // 算法源文件
│   └───qpid_test.c         // 测试源文件
│   license                 // 软件包许可证
│   readme.md               // 软件包使用说明
└───SConscript              // RT-Thread 默认的构建脚本
```

### 1.2许可证

qpid package 遵循 LGPLv2.1 许可，详见 `LICENSE` 文件。

### 1.3依赖

- RT_Thread 4.0

## 2.使用

### 2.1接口函数说明

#### void qpid_init(qpid_t *qpid);
- 功能 ：初始化PID控制数据
- 参数 ：qpid--控制数据指针
- 返回 ：无

#### void qpid_set_dst(qpid_t *qpid, float dst);
- 功能 ：设置被控制量的目标值
- 参数 ：qpid--控制数据指针
- 参数 ：dst--控制目标值
- 返回 ：无

#### void qpid_set_ratio(qpid_t *qpid, float kp, float ki, float kd);
- 功能 ：设置控制比率系数
- 参数 ：qpid--控制数据指针
- 参数 ：kp--比例项系数值
- 参数 ：ki--积分项系数值
- 参数 ：kd--微分项系数值
- 返回 ：无

#### void qpid_set_lmt(qpid_t *qpid, float min, float max);
- 功能 ：设置计算输出的限值, 当限值不合法时将不会被使用
- 参数 ：qpid--控制数据指针
- 参数 ：min--输出最小限值
- 参数 ：max--输出最大限值
- 返回 ：无

#### float qpid_cal_inc(qpid_t *qpid, float cur);
- 功能 ：计算增量型控制值
- 参数 ：qpid--控制数据指针
- 参数 ：cur--被控制量的当前值
- 返回 ：控制量的增量值

#### float qpid_cal_pos(qpid_t *qpid, float cur);
- 功能 ：计算位置型控制值
- 参数 ：qpid--控制数据指针
- 参数 ：cur--被控制量的当前值
- 返回 ：控制量的位置值

### 2.3获取组件

- **方式1：**
通过 *Env配置工具* 或 *RT-Thread studio* 开启软件包，根据需要配置各项参数；配置路径为 *RT-Thread online packages -> signalprocess packages -> qpid* 


### 2.4配置参数说明

| 参数宏 | 说明 |
| ---- | ---- |
| QPID_USING_TEST       | 使用测试功能

## 3. 联系方式

* 维护：qiyongzhong
* 主页：https://gitee.com/qiyongzhong0/rt-thread-qpid
* 邮箱：917768104@qq.com
