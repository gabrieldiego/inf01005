#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "dict.h"
#include "report.h" 



#define C_GROUP 1
#define C_LENGTH 2
#define C_CODES 3

char *filename;



char *get_filename(char *filename)
{
    char *new_filename;
    int i,length;
    int toCopy=0;
    length=strlen(filename);
    for(i=(length-1);i>=0;i--)
    {
        if(toCopy==0)
        {
            if(filename[i]=='.')
            {
                new_filename=(char*) malloc(i+1);
                toCopy=1;
                new_filename[i]='\0';
                
            }
        }
        else
        {
            new_filename[i]=filename[i];
        }
        
    }
    
    if(toCopy==0)
    {
        new_filename=(char*) malloc(length+1);
        strcpy(new_filename,filename);
    }
    
    return new_filename;
}

int main(int argc, char **argv) {
  FILE *output_file,*code_file, *report_file;
  char char_input, ch;
  int state,i;
  int nrGroup,length,code_aux;
  char buffer[DICT_MAX_SIZE], aux[DICT_MAX_SIZE];
  dictionary_t dict;
  char *word, *previous_word;
  int first;
  


  if(argc < 2) {
    printf("Usage.\n");
    printf("  %s <coded file> [<dictionary file>]\n",argv[0]);
    exit(-1);
  }

    
    
  filename=get_filename(argv[1]);
  
  printf("filename (%s)\n",filename);
  
  strcpy(output_filename,filename);
  strcat(output_filename,".dout");

  strcpy(report_filename,filename);
  strcat(report_filename,".drel");
  
  
  init_dict(&dict);
  
  if(argc == 3) 
  {
    have_dictionary=1;
    filename_dictionary=argv[2];
    init_dictionary_from_file(argv[2],&dict);
  }
  else
  {
    have_dictionary=0; 
    init_dictionary_with_ASCII_table(&dict);
  }

  //print_dictionary(&dict);
  
  
  
  // le arquivo codificado
  code_file = fopen(argv[1],"r");
  if (code_file == NULL) {
    printf("Error opening coded file %s.\n",argv[1]);
  }
  
  
  output_file = fopen(output_filename,"w");
  if (output_file == NULL) {
    printf("Error opening coded file output.txt.\n");
  }  
  state=C_GROUP;
  char_input = fgetc(code_file);
  nrGroup=0;
  length=0;
  code_aux=0;
  i=0;  
  first=1;   
  while(char_input != EOF) 
  { 
    
    if(state==C_GROUP)
    {
        if(char_input=='1')
            nrGroup=nrGroup+ (int)pow(2,7-i);
            
        if(i==7)
        {
            state=C_LENGTH;
            i=-1;
        }
    }
    else if(state==C_LENGTH)
    {
        if(char_input=='1')
            length=length+ (int)pow(2,7-i);
            
        if(i==7)
        {
            state=C_CODES;
            i=-1;
        }
    }
    else if(state==C_CODES)
    {
        if(char_input=='1')
            code_aux=code_aux+ (int)pow(2,length-1-i);
    
        if(i==(length-1))
        {
            if(code_aux<size_dictionary(&dict))
	        {
		        word=search_in_dictionary(code_aux,&dict);
		        if(first==1)
		        { 
                    printf("(%s)",word);
                    fprintf(output_file, "%s",word);
			        first=0;
			        previous_word=word;
		        }
		        else
		        {
                    printf("(%s)",word);
                    fprintf(output_file, "%s",word);
			        buffer[0]=word[0];
			        buffer[1]='\0';
			        aux[0]='\0';
			        strcat(aux,previous_word);
			        strcat(aux, buffer);
			        insert_in_dictionary(aux,&dict);
			        previous_word=word;
		        }
            }
	        else
	        {
  
			    buffer[0]=word[0];
			    buffer[1]='\0';
			    strcat(previous_word, buffer);

        		insert_in_dictionary(previous_word,&dict);
			    previous_word=word;
                printf("(%s)",previous_word);
                fprintf(output_file, "%s",previous_word);
                }

                i=-1;
                code_aux=0;
            }
    }
    
    
    char_input = fgetc(code_file);
    i++;
  }


  printf("\ngroup number: %d\n",nrGroup);  
  printf("code length: %d\n",length); 
  close(output_file); 

  report_generate(argv[1],"output.dout");
  return 0;
  
}
