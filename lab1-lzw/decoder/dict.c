
#include "dict.h"



void init_dict(dictionary_t *dict) {
  int i;

  for(i=0; i<DICT_MAX_SIZE; i++) {
    dict->entry[i].word[0] = '\0';
  }
    dict->size=0;
}

void insert_in_dictionary(
  char word[100],
  dictionary_t *dict
) {

  if(dict->size == DICT_MAX_SIZE) {
    printf("Dictionary reseted due to oversize.\n");
    if(have_dictionary==1)
    {
        init_dictionary_from_file(filename_dictionary,dict);
    }
    else
    {
        init_dictionary_with_ASCII_table(dict);
    }
  } else {
	strcpy(dict->entry[dict->size].word,word);
	dict->size++;
  }
}


void print_dictionary(
  dictionary_t *dict
)
{
    int i;
    for(i=0; i<dict->size; i++) 
    {
        printf("dic: %d: %s\n",i,dict->entry[i].word);
    }
}


char * search_in_dictionary(
  int code,
  dictionary_t *dict
)
{
        return dict->entry[code].word;
            
    return 0;
}

int size_dictionary(
  dictionary_t *dict
)
{
	return dict->size;
}


void init_dictionary_from_file(
  char *filename, 
  dictionary_t *dict
)
{
  FILE *input_file;
  char char_input, ch;
  int state,i;
  char buffer[100], aux[100];
  int first=1;

  init_dict(dict);

  
  //input_file = fopen(filename,"r"); //filename_dictionary
  input_file = fopen(filename_dictionary,"r"); //filename_dictionary
  if (input_file == NULL) {
    printf("Error opening dictionary file %s.\n",filename);
  }

  char_input = fgetc(input_file);

  state=S_CHAR; 

  while(char_input != EOF) {
  
    if(state==S_CHAR)
    {
        ch=char_input;
        state=S_COLON;
    }
    else if(state==S_COLON)
    {
        if(char_input == ':')
        {
            state=S_NUMBER;
            i=0;
        }
        else
        {
            printf("Dicionário mal formatado\n");
            exit(-1);        
        }
    }
    else if(state==S_NUMBER)
    {
        if(char_input != 10)
        {
            buffer[i++]=char_input;
        }
        else
        {
            if(first==1)
            {
                first=0;
                if(atoi(buffer)!=0)
                {
                    aux[0]='x';
                    aux[1]='\0';
                    insert_in_dictionary(aux, dict);                
                }
            }
        
            buffer[i++]='\0';
            state=S_CHAR;
            
            //printf("linha: char=%c:%s\n",ch,buffer);
            
            aux[0]=ch;
            aux[1]='\0';
            insert_in_dictionary(aux, dict);

        }
        
    }
	char_input = fgetc(input_file);
  }
}

void init_dictionary_with_ASCII_table(
  dictionary_t *dict
)
{
  int i;
  init_dict(dict);
  for(i=0; i<256; i++) {
    dict->entry[i].word[0] = i;
    dict->entry[i].word[1] = '\0';
    dict->size++;
  }
}
