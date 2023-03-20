#include <stdint.h>
#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

static int fd = 0;
static const char *device = "/dev/spidev0.0";
static uint8_t mode = SPI_MODE_0;
static uint8_t lsb_first = 0;
static uint8_t bits_per_word = 8;
static uint32_t speed = 10000;

static void pabort(int fd, const char *s)
{
        perror(s);
        if (fd >= 0) close(fd);
        abort();
}

static void transfer(int fd, uint8_t addr, uint8_t val)
{
  printf("0x%0x 0x%0x\n", addr, val);
        int ret;
  uint8_t tx[2];
  tx[0] = addr;
  tx[1] = val;
  uint8_t rx[2];

        struct spi_ioc_transfer tr = {
                .tx_buf = (unsigned long)tx,
                .rx_buf = (unsigned long)rx,
                .len = 2,
                .delay_usecs = 0,
                .speed_hz = speed,
                .bits_per_word = bits_per_word,
        };

        ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);

        if (ret < 1)
                pabort(fd, "can't send spi message");

}

int comp (const void * elem1, const void * elem2)
{
    double f = *((double*)elem1);
    double s = *((double*)elem2);
    if (f > s) return  1;
    if (f < s) return -1;
    return 0;
}

int setup_spi(const char* devstr)
{
        int ret = 0;

        int fd = open(devstr, O_RDWR);

        if (fd < 0)
                pabort(-1, "can't open device");

        ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
        if (ret == -1)
                pabort(fd, "can't set write spi mode");

        ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
        if (ret == -1)
                pabort(fd, "can't set read spi mode");

  ret = ioctl(fd, SPI_IOC_WR_LSB_FIRST, &lsb_first);
        if (ret == -1)
                pabort(fd, "can't set write lsb first");

        ret = ioctl(fd, SPI_IOC_RD_LSB_FIRST, &lsb_first);
        if (ret == -1)
                pabort(fd, "can't set read lsb first");

        ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits_per_word);
        if (ret == -1)
                pabort(fd, "can't set bits per write word");

        ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits_per_word);
        if (ret == -1)
                pabort(fd, "can't get bits per read word");

        ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
        if (ret == -1)
                pabort(fd, "can't set max write speed hz");

        ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
        if (ret == -1)
                pabort(fd, "can't get max read speed hz");

  return fd;
}

int main(int argc, char *argv[])
{

  if (argc < 3) {
    pabort(0, "ad8402 rdac1 rdac2");
  }

  uint8_t rdac1 = atoi(argv[1]);
  uint8_t rdac2 = atoi(argv[2]);

  int fd = setup_spi(device);

  if (fd <= 0) {
    pabort(0, "cannot open spi device");
  }

        transfer(fd, 0x0, rdac1);
        transfer(fd, 0x1, rdac2);

        close(fd);

        return 0;
}
