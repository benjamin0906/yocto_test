#ifndef SPI_H

#include <linux/spi/spidev.h>

typedef struct
{
    struct spi_ioc_transfer spiIocConf;
    int spiCsFd;
} dtSpiConf;

int SpiOpenPort(dtSpiConf *const spiConf, int spi_device, const unsigned char spiMode, const unsigned char spiBits, const unsigned int spiSpeed, unsigned char *TxBuff, unsigned char *RxBuff);
int SpiClosePort (const dtSpiConf *const spiConf);
int SpiWriteAndRead (dtSpiConf *spiConf, int Length);

#endif /* SPI_H */
