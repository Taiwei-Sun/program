#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/kd.h>
int main(void)
{
    int freq[] = { /* C   D    E    F    G    A    B    C */
                    523, 587, 659, 698, 784, 880, 988, 1046 };
    int i;

    for (i=0; i<8; i++)
    {
            ioctl(STDOUT_FILENO, KIOCSOUND, 1193180/freq[i]);
            usleep(500000);
    }
    ioctl(STDOUT_FILENO, KIOCSOUND, 0); /*Stop silly sound*/
    return 0;
}
