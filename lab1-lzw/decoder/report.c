
#include "report.h"



void report_generate(char *file_in, char *file_out)
{
    FILE *report_file;


    report_file = fopen(report_filename,"w");
    if (report_file == NULL) {
        printf("Error creating report file %s.\n",report_filename);
        return ;
    }  

    fprintf(report_file, "Relatório de decodificação de arquivo.\n\n");

    // item 1) os dados dos arquivos de entrada e de saída (nome e tamanho em bytes)
    fprintf(report_file, "Arquivo de entrada: %s\n",file_in);
    fprintf(report_file, "Arquivo de saída: %s\n",output_filename);


    // item 2) a entropia do alfabeto usado


    // item 3) a quantidade média de informação recebida por símbolo da mensagem (nesse caso, a mensagem é todo o arquivo de entrada)


    // item 4) o percentual de compactação do arquivo codificado de entrada em relação ao original

    close(report_file);


}
