#include <sys/ioctl.h>

int main() {
    int fd;
    struct ifreq ifr;

    char *str = "new line";
    // Create a socket (needed for network-related ioctls)
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1) {
        perror("socket failed");
        return 1;
    }

    // Specify the network interface (e.g., "eth0")
    strncpy(ifr.ifr_name, "eth0", IFNAMSIZ - 1);

    // Use ioctl with SIOCGIFADDR to get the IP address
    if (ioctl(fd, SIOCGIFADDR, &ifr) == -1) {
        perror("ioctl failed");
        close(fd);
        return 1;
    }

    // Extract and print the IP address
    char *ip = inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
    printf("IP address of eth0: %s\n", ip);

    close(fd);
    return 0;
}
