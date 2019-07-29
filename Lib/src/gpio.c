#include "gpio.h"
#include "assert.h"

static GPIO_Type * const GPIOX[] = GPIO_BASE_PTRS;

/*!
 *  @brief      初始化gpio
 *  @param      PTxn    端口
 *  @param      cfg     引脚方向,0=输入,1=输出
 *  @param      data    输出初始状态,0=低电平,1=高电平 （对输入无效）
 *  Sample usage:       gpio_init (PTA8, GPI,0);    //初始化 PTA8 管脚为输入
 */
void gpio_init(PTXn_e ptxn, GPIO_CFG cfg, uint8_t data)
{
    port_init(ptxn, AsGpio);

    uint8_t ptx, ptn;

    ptx = PTX(ptxn); //GPIO模块号
    ptn = PTn(ptxn); //GPIO端口号

    if(cfg == GPI)// GPIO输入
    {
        GPIOX[ptx]->PDDR &= ~(1U << ptn);
    }
    else // GPIO输出
    {
        GPIOX[ptx]->PDDR |= (1U << ptn);

        if(data == 0U) //输出逻辑0
        {
            GPIOX[ptx]->PCOR = 1U << ptn;
        }
        else //输出逻辑1
        {
            GPIOX[ptx]->PSOR = 1U << ptn;
        }
    }
}

/*!
 *  @brief      设置引脚数据方向
 *  @param      PTxn    端口
 *  @param      cfg     引脚方向,0=输入,1=输出
 *  @since      v5.0
 *  Sample usage:       gpio_ddr (PTA8, GPI);    //设置 PTA8 管脚为输入
 */
void gpio_ddr (PTXn_e ptxn, GPIO_CFG cfg)
{
    //端口方向控制输入还是输出
    if(cfg == GPI)
    {
        //设置端口方向为输入
        GPIOX[PTX(ptxn)]->PDDR &= ~(1U << PTn(ptxn)); // GPIO PDDR 管脚号 清0，即对应管脚配置为端口方向输入
    }
    else
    {
        //设置端口方向为输出
       GPIOX[PTX(ptxn)]->PDDR |= (1U << PTn(ptxn)); // GPIO PDDR 管脚号 置1，即对应管脚配置为端口方向输出
    }
}

/*!
 *  @brief      设置引脚状态
 *  @param      PTxn    端口
 *  @param      data    输出初始状态,0=低电平,1=高电平 （对输入无效）
 *  @since      v5.0
 *  @warning    务必保证数据方向为输出（DEBUG模式下，有断言进行检测）
 *  Sample usage:       gpio_set (PTA8, 1);    // PTA8 管脚 输出 1
 */
void gpio_set (PTXn_e ptxn, uint8_t data)
{
    // 断言，检测 输出方向是否为输出
    assert(BIT_GET(GPIOX[PTX(ptxn)]->PDDR, PTn(ptxn)) == GPO);

    //端口输出数据
    if(data == 0)
    {
        GPIOX[PTX(ptxn)]->PCOR = 1U << PTn(ptxn);   // GPIO PDOR 管脚号 清0，即对应管脚配置为端口输出低电平
    }
    else
    {
        GPIOX[PTX(ptxn)]->PSOR = 1U << PTn(ptxn);  // GPIO PDOR 管脚号 置1，即对应管脚配置为端口输出高电平
    }
}

/*!
 *  @brief      反转引脚状态
 *  @param      PTxn    端口
 *  @since      v5.0
 *  @warning    务必保证数据方向为输出（DEBUG模式下，有断言进行检测）
 *  Sample usage:       gpio_turn (PTA8);    // PTA8 管脚 输出 反转
 */
void gpio_turn (PTXn_e ptxn)
{
    // 断言，检测 输出方向是否为输出
    assert(BIT_GET(GPIOX[PTX(ptxn)]->PDDR, PTn(ptxn)) == GPO);

    GPIOX[PTX(ptxn)]->PTOR = 1U << PTn(ptxn);  // GPIO PTOR ptxn 置1，其他清0，即对应管脚配置为端口输出反转，其他位不变
}

/*!
 *  @brief      读取引脚输入状态
 *  @param      PTxn    端口
 *  @return     管脚的状态，1为高电平，0为低电平
 *  @since      v5.0
 *  @warning    务必保证数据方向为输入（DEBUG模式下，有断言进行检测）
 *  Sample usage:       uint8 pta8_data = gpio_get (PTA8);    // 获取 PTA8 管脚 输入电平
 */
uint8_t gpio_get(PTXn_e ptxn)
{
    // 断言，检测 输出方向是否为输出
    assert(BIT_GET(GPIOX[PTX(ptxn)]->PDDR, PTn(ptxn)) == GPI);
    return (GPIOX[PTX(ptxn)]->PDIR >> PTn(ptxn) & 0x01);  // 获取 GPIO PDIR ptxn 状态，即读取管脚输入电平
}
