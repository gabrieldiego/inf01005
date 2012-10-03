
#include "report.h"



void report_generate(char *file_in, char *file_out)
{
    FILE *report_file, *code_file, *output_file;
    float cont_in, cont_out;
    float length_in, length_out, compactacao;
    char char_input;
    double entropia, q_media;


    report_file = fopen(report_filename,"w");
    if (report_file == NULL) {
        printf("Error creating report file %s.\n",report_filename);
        return ;
    }  

    fprintf(report_file, "Relatório de decodificação de arquivo.\n\n");

    // item 1) os dados dos arquivos de entrada e de saída (nome e tamanho em bytes)
    
    // le arquivo codificado
    code_file = fopen(input_filename,"r");
    cont_in=0;
    if (code_file != NULL) {
        cont_in=0;
        char_input = fgetc(code_file);
        while(char_input != EOF) 
        {
            cont_in++;
            char_input = fgetc(code_file);
        } 
    }    
    fclose(code_file);
    
    length_in=cont_in/8.0;

    // le arquivo saida
    output_file = fopen(output_filename,"r");
    cont_in=0;
    char_input = fgetc(output_file);
    if (output_file != NULL) {
        cont_in=0;
        while(char_input != EOF) 
        {
            cont_in++; 
            char_input = fgetc(output_file);
        } 
    }    
    
    fclose(output_file);
    
    length_out=cont_in;

    
    fprintf(report_file, "Arquivo de entrada: %s (%.2f B)\n",file_in,length_in);
    fprintf(report_file, "Arquivo de saída: %s (%.2f B)\n\n",output_filename,cont_in);


    // item 2) a entropia do alfabeto usado
    entropia=0;
    entropia=(-log2(cont_0/(cont_0+cont_1))*cont_0)+(-log2(cont_1/(cont_0+cont_1))*cont_1);
    
    fprintf(report_file, "Entropia do alfabeto: %.3f \n\n",entropia);
    

    // item 3) a quantidade média de informação recebida por símbolo da mensagem (nesse caso, a mensagem é todo o arquivo de entrada)
    q_media=entropia/(cont_0+cont_1);
    
    fprintf(report_file, "Quantidade média de informação recebida por símbolo: %.3f \n\n",q_media);

    // item 4) o percentual de compactação do arquivo codificado de entrada em relação ao original

    fprintf(report_file, "Compressão de arquivo: %.2f%%\n\n",(1.0-length_in/cont_in)*100.0);
    
    
    fclose(report_file);

}
