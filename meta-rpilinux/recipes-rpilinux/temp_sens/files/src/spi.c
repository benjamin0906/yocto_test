#include <fcntl.h>				//Needed for SPI port
#include <sys/ioctl.h>			//Needed for SPI port
#include <linux/spi/spidev.h>	//Needed for SPI port
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <spi.h>

int SpiOpenPort(dtSpiConf *const spiConf, int spi_device, const unsigned char spiMode, const unsigned char spiBits, const unsigned int spiSpeed, unsigned char *TxBuff, unsigned char *RxBuff)
{
	int status_value = 0;

    if(spiConf != 0)
    {
        if(TxBuff != 0)
        {
            if(RxBuff != 0)
            {
                spiConf->spiIocConf.tx_buf = (__u64)TxBuff;
                spiConf->spiIocConf.rx_buf = (__u64)RxBuff;
                spiConf->spiIocConf.speed_hz = spiSpeed;
                spiConf->spiIocConf.delay_usecs = 0;
                spiConf->spiIocConf.bits_per_word = spiBits;
                spiConf->spiIocConf.cs_change = 0;

                if(spi_device)
                {
                    spiConf->spiCsFd = open("/dev/spidev0.1", O_RDWR);
                }
                else
                {
                    spiConf->spiCsFd = open("/dev/spidev0.0", O_RDWR);
                }
                printf("open: %i\n", spiConf->spiCsFd);
                if(spiConf->spiCsFd >= 0)
                {
                    if(ioctl(spiConf->spiCsFd, SPI_IOC_WR_MODE, &spiMode) >= 0)
                    {
                        if(ioctl(spiConf->spiCsFd, SPI_IOC_RD_MODE, &spiMode) >= 0)
                        {
                            if(ioctl(spiConf->spiCsFd, SPI_IOC_WR_BITS_PER_WORD, &spiBits) >= 0)
                            {
                                if(ioctl(spiConf->spiCsFd, SPI_IOC_RD_BITS_PER_WORD, &spiBits) >= 0)
                                {
                                    if(ioctl(spiConf->spiCsFd, SPI_IOC_WR_MAX_SPEED_HZ, &spiSpeed) >= 0)
                                    {
                                        if(ioctl(spiConf->spiCsFd, SPI_IOC_RD_MAX_SPEED_HZ, &spiSpeed) >= 0)
                                        {
                                            printf("Success opening\n");
                                        }
                                        else status_value = -10;
                                    }
                                    else status_value = -9;
                                }
                                else status_value = -8;
                            }
                            else status_value = -7;
                        }
                        else status_value = -6;
                    }
                    else status_value = -5;
                }
                else status_value = -4;
            }
            else status_value = -3;
        }
        else status_value = -2;
    }
    else status_value = -1;

    return(status_value);
}

int SpiClosePort (const dtSpiConf *const spiConf)
{
	int status_value = -1;

    status_value = close(spiConf->spiCsFd);

    return(status_value);
}

int SpiWriteAndRead (dtSpiConf *spiConf, int Length)
{
	int retVal = -1;

    spiConf->spiIocConf.len = Length;
    retVal = ioctl(spiConf->spiCsFd, SPI_IOC_MESSAGE(1), &spiConf->spiIocConf);

	return retVal;
}
