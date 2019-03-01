#include "iic_soft.h"
#include "gpio.h"

/*!
 *  @brief      初始化模拟IIC
 *  @return      void
 *  Sample usage:       IIC_init()
 */
void IIC_init()
{
    gpio_init(IIC_SCL, GPO, 0);
    gpio_init(IIC_SDA, GPO, 0);
}

/*!
 *  @brief      模拟IIC延时
 *  @return     void
 *  Sample usage:       IIC_delay()    如果IIC通讯失败可以尝试增加n的值
 */
static void IIC_delay()
{
    uint8_t n = 2;

    while(n--)
    {
        asm("nop");asm("nop");asm("nop");asm("nop");
    }
}

/*!
 *  @brief      启动IIC
 *  @return     void
 *  Sample usage:       IIC_start()
 */
static void IIC_start()
{
    DIR_OUT();
    SDA_SET(1);
    IIC_delay();
    SCL_SET(1);
    IIC_delay();
    SDA_SET(0);
    IIC_delay();
}

/*!
 *  @brief      停止IIC
 *  @return     void
 *  Sample usage:       IIC_start()
 */
static void IIC_stop()
{
    DIR_OUT();
    SDA_SET(0);
    IIC_delay();
    SCL_SET(1);
    IIC_delay();
    SDA_SET(1);
    IIC_delay();
}

/*!
 *  @brief      IIC应答
 *  @param      ack_data    应答信号，  ACK：主应答； NO_ACK： 从应答
 *  @return     void
 *  Sample usage:       IIC_ack(ACK); IIC_ack(NO_ACK)
 */
static void IIC_ack(uint8_t ack_data)
{
    SCL_SET(0);
	IIC_delay();

    if(ack_data)
    {
        SDA_SET(0);     //主应答
    }
    else
    {
        SDA_SET(1);     //从应答
    }

    SCL_SET(1);
    IIC_delay();
    SCL_SET(0);
    IIC_delay();
}

/*!
 *  @brief      IIC等待应答
 *  @return     void
 *  Sample usage:       IIC_wait_ack()
 */
static uint8_t IIC_wait_ack()
{
    SCL_SET(0);
	DIR_IN();
	IIC_delay();

	SCL_SET(1);
    IIC_delay();

    if(SDA())           //应答为高电平，异常，通信失败
    {
        DIR_OUT();
        SCL_SET(0);
        return 0;
    }
    DIR_OUT();
    SCL_SET(0);
    return 1;
}

/*!
 *  @brief      IIC读取
 *  @return     uint8   读取的数据
 *  Sample usage:       IIC_read()
 */
uint8_t IIC_read()
{
    uint8_t i = 0;
    uint8_t c = 0;

    SCL_SET(0);
    IIC_delay();
    SDA_SET(1);
    DIR_IN();  //置数据线为输入方式

    for(i=0;i<8;i++)
    {
        IIC_delay();
        SCL_SET(0);         //置时钟线为低，准备接收数据位
        IIC_delay();
        SCL_SET(1);        //置时钟线为高，使数据线上数据有效
        IIC_delay();
        c<<=1;         //数据位移位
        if(SDA())
        {
            c += 1;   //读数据位（先读取高位）
        }
    }
    DIR_OUT();
	SCL_SET(0);
	IIC_delay();
	IIC_ack(NO_ACK);

    return c;
}

/*!
 *  @brief      IIC写入
 *  @param      c   写入的数据
 *  @return     void
 *  Sample usage:       IIC_write(8)
 */
void IIC_write(uint8_t c)
{
    uint8_t i=8;

    while(i--)
    {
        if(c & 0x80)    //取数据高位
        {
            SDA_SET(1); //发送逻辑1
        }
        else
        {
            SDA_SET(0); //发送逻辑0
        }
        c <<= 1;
        IIC_delay();
        SCL_SET(1); //SCL时钟拉高， 维持一段时间
        IIC_delay();
        SCL_SET(0); //SCL时钟拉低
    }
    IIC_wait_ack(); //等待应答
}

/*!
 *  @brief      模拟IIC写数据到设备寄存器函数
 *  @param      dev_add			设备地址(低七位地址)
 *  @param      reg				寄存器地址
 *  @param      dat				写入的数据
 *  @return     void
 */
void IIC_write_reg(uint8_t dev_add, uint8_t reg, uint8_t data)
{
	IIC_start();
    IIC_write((dev_add<<1) | 0x00);  //发送器件地址加写位
	IIC_write(reg);   				 //发送从机寄存器地址
	IIC_write(data);   				 //发送需要写入的数据
	IIC_stop();
}

/*!
 *  @brief      模拟IIC从设备寄存器读取数据
 *  @param      dev_add			设备地址(低七位地址)
 *  @param      reg				寄存器地址
 *  @param
 *  @return     uint8			返回寄存器的数据
 */
uint8_t IIC_read_reg(uint8_t dev_add, uint8_t reg)
{
	uint8_t data;

	IIC_start();
    IIC_write((dev_add<<1) | 0x00);  //发送器件地址加写位
	IIC_write(reg);   				  //发送从机寄存器地址

	IIC_start();                       //IIC重启（由发送变为接收需要重启IIC）
	IIC_write((dev_add<<1) | 0x01);    //发送器件地址加读位
	data = IIC_read();   				//发送需要写入的数据
	IIC_stop();

	return data;
}
