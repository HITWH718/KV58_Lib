#ifndef _SPI_H_
#define _SPI_H_

#include "MKV58F24.h"
#include "clock.h"
#include "port.h"

// SPI�˿ں궨��
#define SPI0_SCK_PIN    PTB1
#define SPI0_SOUT_PIN   PTB2
#define SPI0_SIN_PIN    PTB3
#define SPI0_PCS0_PIN   PTB4
#define SPI0_PCS1_PIN   PTB5
#define SPI0_PCS2_PIN   PTB6
#define SPI0_PCS3_PIN   PTB7
#define SPI0_PCS4_PIN   PTB8
#define SPI0_PCS5_PIN   PTB9

#define SPI1_SCK_PIN    PTB1
#define SPI1_SOUT_PIN   PTB2
#define SPI1_SIN_PIN    PTB3
#define SPI1_PCS0_PIN   PTB4
#define SPI1_PCS1_PIN   PTB5
#define SPI1_PCS2_PIN   PTB6
#define SPI1_PCS3_PIN   PTB7

#define SPI2_SCK_PIN    PTB1
#define SPI2_SOUT_PIN   PTB2
#define SPI2_SIN_PIN    PTB3
#define SPI2_PCS0_PIN   PTB4
#define SPI2_PCS1_PIN   PTB5


/**
 *  @brief ���ӻ�ģʽ
 */
typedef enum
{
    MASTER,    //����ģʽ
    SLAVE      //�ӻ�ģʽ
} SPI_CFG;

/**
 *  @brief SPIģ���
 */
typedef enum
{
    SPI_0,
    SPI_1,
    SPI_2
} SPIn_e;

/**
 *  @brief SPIģ��Ƭѡ��
 */
typedef enum
{
    SPI_PCS0 = 1 << 0,
    SPI_PCS1 = 1 << 1,
    SPI_PCS2 = 1 << 2,
    SPI_PCS3 = 1 << 3,
    SPI_PCS4 = 1 << 4,
    SPI_PCS5 = 1 << 5,
} SPI_PCSn_e;


extern uint32_t spi_init(SPIn_e, SPI_PCSn_e , SPI_CFG,uint32_t baud);                                        //SPI��ʼ����ѡ��Ƭѡ�źţ�����ģʽ��������
uint32_t spi_set_baud(SPIn_e, uint32_t baud);

//�������շ��ͺ���
extern void spi_mosi(SPIn_e, SPI_PCSn_e pcs, uint8_t *modata, uint8_t *midata, uint32_t len);    //SPI���ͽ��պ���,����databuff���ݣ����ѽ��յ������ݴ����databuff��(ע�⣬�Ḳ��ԭ����databuff)
extern void spi_mosi_cmd(SPIn_e, SPI_PCSn_e pcs, uint8_t *mocmd , uint8_t *micmd , uint8_t *modata, uint8_t *midata, uint32_t cmdlen , uint32_t len); //SPI���ͽ��պ���,��spi_mosi��ȣ������ȷ���cmd �������Ĳ��裬���ֿ������ַ���

#endif
