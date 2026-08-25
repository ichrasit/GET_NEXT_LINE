#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>


void    test_et(char *dosya_adi, int fd_override)
{
    int fd;
    char    *line;
    int i;
    if(fd_override != 0)
        fd = fd_override;
    else
        fd = open(dosya_adi, O_RDONLY);
    printf("\n ------ TEST %s ----------\n", dosya_adi);
    if(fd == -1)
        printf("Uyari: FD gecersiz veya dosya yok");
    i = 1;
    while((line = get_next_line(fd)) != NULL)
    {
        printf("satir %d : %s", i, line);
        free(line);
        i++;
    }
    printf("\n--> OKUMA BİTTİ VEYA NULL DONDU\n");
    if(fd != -1)
        close(fd);

}
int main()
{
    test_et("Gecersiz fd testi", -1);
    test_et("Sacma fd testi", 42);
    test_et("bos_dosya.txt", 0);
    test_et("tek_enter.txt", 0);
    test_et("enter_yok.txt", 0);
    test_et("test.txt", 0);
}