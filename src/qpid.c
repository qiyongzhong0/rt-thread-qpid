/*
 * qpid.c
 *
 * Change Logs:
 * Date           Author            Notes
 * 2021-09-09     qiyongzhong       first version
 */

#include <qpid.h>
#include <rtthread.h>
#include <string.h>

#define QPID_ASSERT(x)      RT_ASSERT(x)

void qpid_init(qpid_t *qpid)//初始化qpid
{
    QPID_ASSERT(qpid != RT_NULL);
    memset(qpid, 0, sizeof(qpid_t));
}

void qpid_set_dst(qpid_t *qpid, float dst)//设置目标值
{
    QPID_ASSERT(qpid != RT_NULL);
    qpid->dst = dst;
}

void qpid_set_ratio(qpid_t *qpid, float kp, float ki, float kd)//设置各项比例
{
    QPID_ASSERT(qpid != RT_NULL);
    qpid->kp = kp;
    qpid->ki = ki;
    qpid->kd = kd;
}

void qpid_set_lmt(qpid_t *qpid, float min, float max)//设置输出限值
{
    QPID_ASSERT(qpid != RT_NULL);
    qpid->min = min;
    qpid->max = max;
}

float qpid_cal_inc(qpid_t *qpid, float cur)//计算增量型pid, 输出增量值
{
    float rst = 0;
    
    QPID_ASSERT(qpid != RT_NULL);

    qpid->err[2] = qpid->err[1];//转移上上次偏差到err[2]
    qpid->err[1] = qpid->err[0];//转移上次偏差到err[1]
    qpid->err[0] = qpid->dst - cur;//计算本次偏差到err[0]

    rst = qpid->kp * (qpid->err[0] - qpid->err[1]);//计算比例项
    rst += qpid->ki * qpid->err[0];//计算累加积分项
    rst += qpid->kd * (qpid->err[0] - (qpid->err[1] * 2) + qpid->err[2]);//计算累加微分项

    if ((qpid->min * qpid->max >= 0) || (qpid->min >= qpid->max))//限值参数不可用
    {
        return(rst);
    }
    
    if (rst < qpid->min)
    {
        return(qpid->min);
    }

    if (rst > qpid->max)
    {
        return(qpid->max);
    }

    return(rst);
}

float qpid_cal_pos(qpid_t *qpid, float cur)//计算位置型pid, 输出位置值
{
    float rst = 0;
    
    QPID_ASSERT(qpid != RT_NULL);

    qpid->err[2] += qpid->err[0];//计算偏差积分到err[2], 将积分滞后1个周期
    qpid->err[1] = qpid->err[0];//转移上次偏差到err[1]
    qpid->err[0] = qpid->dst - cur;//计算本次偏差到err[0]

    rst = qpid->kp * qpid->err[0];//计算比例项
    rst += qpid->ki * qpid->err[2];//计算累加积分项
    rst += qpid->kd * (qpid->err[0] - qpid->err[1]);//计算累加微分项

    if (qpid->min >= qpid->max)//限值参数不可用
    {
        return(rst);
    }
    
    if (rst < qpid->min)
    {
        return(qpid->min);
    }

    if (rst > qpid->max)
    {
        return(qpid->max);
    }

    return(rst);
}

