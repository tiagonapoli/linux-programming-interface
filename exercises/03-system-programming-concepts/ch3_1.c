#include <stdio.h>
#include <unistd.h>
#include <linux/reboot.h>

int main() {
    printf("MAGIC1: 0x%08X (%d)\n", LINUX_REBOOT_MAGIC1, LINUX_REBOOT_MAGIC1);
    printf("MAGIC2: 0x%08X (%d)\n", LINUX_REBOOT_MAGIC2, LINUX_REBOOT_MAGIC2);
}