/*
 * qpid_sample.c
 *
 * Change Logs:
 * Date           Author            Notes
 * 2021-09-10     qiyongzhong       first version
 */

#include <qpid.h>
#include <rtthread.h>

#ifdef QPID_USING_TEST

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define QPID_CTRL_MUL           5   //控制量计算输出量的倍率系数
#define QPID_CTRL_OFFSET        10  //控制量计算输出量的偏移系数

static qpid_t qpid_datas;
static float qpid_ctrl_val = 0;
static int   qpid_vals_idx = 0;
static float qpid_vals[4] = {0};

static void qpdi_vals_init(void)
{
    qpid_ctrl_val = 0;
    qpid_vals_idx = 0;
    memset(qpid_vals, 0, sizeof(qpid_vals));
}

static float qpid_get_out_val(void)
{
    int total = sizeof(qpid_vals)/sizeof(qpid_vals[0]);
    
    qpid_vals[qpid_vals_idx] = (qpid_ctrl_val * QPID_CTRL_MUL + QPID_CTRL_OFFSET);
    qpid_vals_idx++;
    qpid_vals_idx %= total;

    float rst = 0;
    for (int i=0; i<total; i++)
    {
        rst += qpid_vals[i];
    }
    rst /= total;

    return(rst);
}

static const char *cmd_info[] =
{
    "Usages:\n",
    "qpid init              - initialize qpid datas.\n",
    "qpid dst val           - set destination value.\n",
    "qpid ratio kp ki kd    - set ratio - kp, ki, kd.\n",
    "qpid lmt min max       - set out limit - min, max.\n",
    "qpid cal_inc times     - calculate incremental type pid.\n",
    "qpid cal_pos times     - calculate positional type pid.\n",
    "\n"
};    
static void show_cmd_info(void)
{
    for(int i=0; i<sizeof(cmd_info)/sizeof(char*); i++)
    {
        rt_kprintf(cmd_info[i]);
    }
}
static void qpid_test(int argc, char **argv)  
{  
    if (argc < 2)
    {
        show_cmd_info();
        return;
    }
    
    if (strcmp(argv[1], "init") == 0)
    {
        qpdi_vals_init();
        qpid_init(&qpid_datas);
        rt_kprintf("qpid init ok.\n");
        return;
    }
    
    if (strcmp(argv[1], "dst") == 0)
    {
        float dst;
        char str[32];
        if (argc < 3)
        {
            rt_kprintf(cmd_info[2]);
            return;
        }
        sscanf(argv[2], "%f", &dst);
        qpid_set_dst(&qpid_datas, dst);
        sprintf(str, "%f", dst);
        rt_kprintf("qpid set dst = %s.\n", str);
        return;
    }
    
    if (strcmp(argv[1], "ratio") == 0)
    {
        float kp, ki, kd;
        char str[32];
        if (argc < 5)
        {
            rt_kprintf(cmd_info[3]);
            return;
        }
        sscanf(argv[2], "%f", &kp);
        sscanf(argv[3], "%f", &ki);
        sscanf(argv[4], "%f", &kd);
        qpid_set_ratio(&qpid_datas, kp, ki, kd);
        sprintf(str, "%f, %f, %f", kp, ki, kd);
        rt_kprintf("qpid set ratio kp, ki, kp = %s.\n", str);
        return;
    }
    
    if (strcmp(argv[1], "lmt") == 0)
    {
        float min, max;
        char str[32];
        if (argc < 4)
        {
            rt_kprintf(cmd_info[4]);
            return;
        }
        sscanf(argv[2], "%f", &min);
        sscanf(argv[3], "%f", &max);
        qpid_set_lmt(&qpid_datas, min, max);
        sprintf(str, "%f, %f", min, max);
        rt_kprintf("qpid set limit min, max = %s.\n", str);
        return;
    }
    
    if (strcmp(argv[1], "cal_inc") == 0)
    {
        int times;
        char str[32];
        if (argc < 3)
        {
            rt_kprintf(cmd_info[5]);
            return;
        }
        times = atoi(argv[2]);
        rt_kprintf("qpid calculate times = %d.\n", times);
        for (int i=0; i<times; i++)
        {
            float out_val = qpid_get_out_val();
            sprintf(str, "%f", out_val);
            rt_kprintf("out value = %s.\n", str);
            qpid_ctrl_val += qpid_cal_inc(&qpid_datas, out_val);
        }
        return;
    }
    
    if (strcmp(argv[1], "cal_pos") == 0)
    {
        int times;
        char str[32];
        if (argc < 3)
        {
            rt_kprintf(cmd_info[6]);
            return;
        }
        times = atoi(argv[2]);
        rt_kprintf("qpid calculate times = %d.\n", times);
        for (int i=0; i<times; i++)
        {
            float out_val = qpid_get_out_val();
            sprintf(str, "%f", out_val);
            rt_kprintf("out value = %s.\n", str);
            qpid_ctrl_val = qpid_cal_pos(&qpid_datas, out_val);
        }
        return;
    }
    
    rt_kprintf("No supported command.\n");
} 
MSH_CMD_EXPORT_ALIAS(qpid_test, qpid, test qpid functions);

#endif

