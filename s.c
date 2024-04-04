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

void ejecutarComando(char archivo[20], char **sp, int entrada, int salida) {
    // Redirección de entrada/salida
    if (entrada != STDIN_FILENO) {
        dup2(entrada, STDIN_FILENO);
        close(entrada);
    }
    if (salida != STDOUT_FILENO) {
        dup2(salida, STDOUT_FILENO);
        close(salida);
    }

    execvp(archivo, sp);
    perror("Error al ejecutar el comando");
    exit(EXIT_FAILURE);
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
		int numComandos = 1;
        for (int i = 0; i < strlen(comand); i++) {
            if (comand[i] == '|') {
                numComandos++;
            }
        }

        int pipes[numComandos - 1][2];
        int entrada = STDIN_FILENO;

        for (int i = 0; i < numComandos; i++) {
            pipe(pipes[i]);
            int salida = (i == numComandos - 1) ? STDOUT_FILENO : pipes[i][1];

            int pid = fork();
            if (pid == 0) {
                ejecutarComando(archivo, sp, entrada, salida);
            } else {
                wait(NULL);
                if (entrada != STDIN_FILENO) {
                    close(entrada);
                }
                if (salida != STDOUT_FILENO) {
                    close(salida);
                }
                entrada = pipes[i][0];
            }
        }

        // Cerrar los descriptores de archivo de las tuberías en el proceso padre
        for (int i = 0; i < numComandos - 1; i++) {
            close(pipes[i][0]);
            close(pipes[i][1]);
        }
    }
}
