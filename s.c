#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int numparametros = 0;

int cuentaEspacios(char comand[]){
    int aux = 0;
    for(int i =0; i< strlen(comand); i++){
        if((comand[i]==' ')||(comand[i]=='\n'))
            aux++;
    }
    return aux;
}

void imprimePalabras(char **sp, char *archivo){
    char **temp=sp;
    printf("Aqui deberian mostrar los parametros\n");
    printf("%s\n",archivo);
    
    // for(int i =0 ; i < numparametros; i++){
    //     printf("%s\n",*temp);
    //     temp++;
    // }
    for(int i =0 ; i < numparametros; i++){
        for(int j=0 ; j<strlen(*temp); j++)
            printf("%c",temp[i][j]);
        printf("\n");
    }
    printf("Acaba pa\n");
}

char ** separaCadenas(char archivo[20],char *comand){
    int p=0, q=0, aux = 0;
    int n = strlen(comand);
    char temp[250];
    char **ite, **sp;
    numparametros = (cuentaEspacios(comand));
    //printf("numero de para %d\n",numparametros);
    if(numparametros>0)
        ite = (char **) malloc(sizeof(char *)*numparametros);
    else
        ite = NULL;
    sp = ite;
    //printf("%d\n",cuentaEspacios(comand)-1);
    for (int i =0; i <n ; i++){
        if((comand[i]==' ')||(comand[i]=='\t')||(comand[i]=='\n')){
            if (aux==0){
                memcpy(archivo, &comand[p], q);
            }
                memcpy(temp, &comand[p], q);
                temp[q-p]='\0';
                *ite = (char *) malloc(sizeof(char )*(strlen(temp)));
                strcpy(*ite,temp);
                ite++;
            p=q=i+1;
            aux++;
        }else{
            q++;
        }
    }
    return sp;
}

void limpiacadenas(char archivo[20],char **sp){
    char **aux=sp;
    for(int i =0 ; i < numparametros; i++){
        free(aux[i]);
    }
    free(sp);
    sp = NULL;
    memset(archivo,0,20);
}

int main(){
    // gcc -Wall -std=c11 shell.c -o shell
	char comand[250];
    char archivo[20];
    char **sp = NULL;
	while(strcmp(archivo,"exit")){
        limpiacadenas(archivo,sp);
    	printf("Myshell>");
        fflush(stdin);
		fgets(comand,250,stdin);
        fflush(stdin);
        sp = separaCadenas(archivo,comand);		
		int pid=fork();
		if(pid==0){
			execvp(archivo,sp);	
			exit(1);
		}else{
			wait(NULL);
			//exit(1);
		}	
	}
}
