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

int main(int argc, char **argv)
{
    uint8_t Tx[5] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    uint8_t Rx[5] = {0, 0, 0, 0, 0};
    dtSpiConf mySpiConf;
    memset(&mySpiConf, 0, sizeof(mySpiConf));

    SpiOpenPort( &mySpiConf, 0, SPI_MODE_3, 8, 100000, Tx, Rx);

    /* SPI reset in the sensor */
    SpiWriteAndRead(&mySpiConf, 5);
    nanosleep((const struct timespec[]){{0, 1000000L}}, NULL);

    /* Getting the ID */
    Tx[0] = READ_REG(Reg_Id);
    SpiWriteAndRead(&mySpiConf, 2);

    if((Rx[1] & 0xF0) == 0xC0)
    {
        /* Setting the config register */
        Tx[0] = WRITE_REG(Reg_Config);
        Tx[1] = 0x80;
        SpiWriteAndRead(&mySpiConf, 2);

        /* Reading back config register */
        Tx[0] = READ_REG(Reg_Config);
        Rx[1] = 0;
        SpiWriteAndRead(&mySpiConf, 2);

        if(Rx[1] == Tx[1])
        {
            printf("Configured 0x%x\n", Rx[1]);
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
    }
    else printf("ID is wrong %x\n", Rx[1]);
    SpiClosePort(&mySpiConf);
    return 0;
}

void SigTermHandler(int signal)
{
    looping = 0;
    printf("Terminating temperature monitoring\n");
}
