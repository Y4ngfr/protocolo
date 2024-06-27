#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>

int main()
{
    struct stat fileStatus;
    FILE* fp;
    int ret;
    long posicao;

    fp = fopen("recordings/audio 25-06-2024 às 19:32:26:45.wav", "r");

    fseek(fp, 0, SEEK_END);

    posicao = ftell(fp);      // Método para captar tamanho do arquivo
    printf("%ld\n", posicao); //

    fileStatus.st_size = 0;

    ret = stat("recordings/audio 25-06-2024 às 19:32:35:19.wav", &fileStatus);

    if(ret == -1){
        printf("erro\n");
        perror("");
        return 1;
    }

    // tamanho em bytes do arquivo
    printf("%ld\n", (__intmax_t)fileStatus.st_size);

    return 0;
}