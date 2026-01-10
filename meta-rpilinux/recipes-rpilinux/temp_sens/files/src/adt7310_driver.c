#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "spi.h"
#include <time.h>
#include <signal.h>

enum
{
    Reg_Status  = 0 << 3,
    Reg_Config  = 1 << 3,
    Reg_TempVal = 2 << 3,
    Reg_Id      = 3 << 3,
    Reg_Tcrit   = 4 << 3,
    Reg_Thyst   = 5 << 3,
    Reg_Thigh   = 6 << 3,
    Reg_Tlow    = 7 << 3,
} eRegs;

#define READ_REG(x) 0x40 | x
#define WRITE_REG(x) 0x00 | x

volatile uint8_t looping;

void SigTermHandler(int signal);
int8_t Init(dtSpiConf *const spi_desc);

int main(int argc, char **argv)
{
    uint8_t Tx[5] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    uint8_t Rx[5] = {0, 0, 0, 0, 0};
    int ret = 0;
    dtSpiConf mySpiConf;
    memset(&mySpiConf, 0, sizeof(mySpiConf));

    SpiOpenPort( &mySpiConf, 0, SPI_MODE_3, 8, 100000, Tx, Rx);
    ret = Init(&mySpiConf);

    if(ret == 0)
    {
        uint16_t RawTemp = 0;
        FILE *tempFile = 0;
        if(argc > 1)
        {
            looping = 1;
            tempFile = fopen(argv[1], "w");
            signal(SIGTERM, SigTermHandler);
        }

        Tx[0] = READ_REG(Reg_TempVal);

        do
        {
            Rx[1] = 0;
            Rx[2] = 0;
            SpiWriteAndRead(&mySpiConf, 3);

            RawTemp = Rx[1];
            RawTemp <<= 8;
            RawTemp |= Rx[2];

            if(tempFile != 0)
            {
                fprintf(tempFile, "%.2f\n", RawTemp*0.0078125);
                fflush(tempFile);
                fseek(tempFile, 0, SEEK_SET);
            }
            else printf("%.2f\n", RawTemp*0.0078125);

            nanosleep((const struct timespec[]){{0, 500000000L}}, NULL);
            //nanosleep((const struct timespec[]){{0, 500000000L}}, NULL);
        }
        while(looping != 0);
        if(tempFile != 0) fclose(tempFile);
    }

    SpiClosePort(&mySpiConf);
    return ret;
}

int8_t Init(dtSpiConf *const spi_desc)
{
    /* SPI reset in the sensor */
    ((uint8_t*)spi_desc->spiIocConf.tx_buf)[0] = 0xFF;
    ((uint8_t*)spi_desc->spiIocConf.tx_buf)[1] = 0xFF;
    ((uint8_t*)spi_desc->spiIocConf.tx_buf)[2] = 0xFF;
    ((uint8_t*)spi_desc->spiIocConf.tx_buf)[3] = 0xFF;
    ((uint8_t*)spi_desc->spiIocConf.tx_buf)[4] = 0xFF;
    SpiWriteAndRead(spi_desc, 5);
    nanosleep((const struct timespec[]){{0, 1000000L}}, NULL);

    /* Getting the ID */
    ((uint8_t*)spi_desc->spiIocConf.tx_buf)[0] = READ_REG(Reg_Id);
    ((uint8_t*)spi_desc->spiIocConf.tx_buf)[1] = 0xFF;
    SpiWriteAndRead(spi_desc, 2);

    if((((uint8_t*)spi_desc->spiIocConf.rx_buf)[1] & 0xF0) != 0xC0)
    {
        return -1;
    }

    /* Setting the config register */
    ((uint8_t*)spi_desc->spiIocConf.tx_buf)[0] = WRITE_REG(Reg_Config);
    ((uint8_t*)spi_desc->spiIocConf.tx_buf)[1] = 0x80;
    SpiWriteAndRead(spi_desc, 2);

    /* Reading back config register */
    ((uint8_t*)spi_desc->spiIocConf.tx_buf)[0] = READ_REG(Reg_Config);
    ((uint8_t*)spi_desc->spiIocConf.tx_buf)[1] = 0xFF;
    ((uint8_t*)spi_desc->spiIocConf.rx_buf)[1] = 0;
    SpiWriteAndRead(spi_desc, 2);

    if(((uint8_t*)spi_desc->spiIocConf.rx_buf)[1] != 0x80)
    {
        return -2;
    }

    /* Dummy read */
    nanosleep((const struct timespec[]){{0, 250000000L}}, NULL);
    ((uint8_t*)spi_desc->spiIocConf.tx_buf)[0] = READ_REG(Reg_TempVal);
    ((uint8_t*)spi_desc->spiIocConf.rx_buf)[1] = 0;
    ((uint8_t*)spi_desc->spiIocConf.rx_buf)[2] = 0;
    SpiWriteAndRead(spi_desc, 3);

    return 0;
}

void SigTermHandler(int signal)
{
    looping = 0;
    printf("Terminating temperature monitoring\n");
}
