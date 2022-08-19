#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct{
   int time;
   int timeStart;
   char label[2];
   int wait;

}task;

typedef struct{
   int busyUntil;
   int size;
   char currentProcess[2];
   task history[15];
}processor;

void checkNull(FILE *ptr){
   if (ptr == NULL){
      printf("Arquivo == COCOZAO\n");
      exit(0);
   }
}

void swap(task *a, task *b){
   task temp = *a;
   *a = *b;
   *b = temp;
}

void bolhasort(task arr[], int nlinhas){
   int i, j;
   for (i = 0; i < (nlinhas-1); i++){
      for (j = 0; j < (nlinhas-i-1); j++){
         if (arr[j].time > arr[j+1].time){
            swap(&arr[j], &arr[j+1]);
        }
     }
   }
}

void reversesort(task arr[], int nlinhas){
   int i, j;
   for (i = 0; i < (nlinhas-1); i++){
      for (j = 0; j < (nlinhas-i-1); j++){
         if (arr[j].time < arr[j+1].time){
            swap(&arr[j], &arr[j+1]);
        }
     }
   }
}



void scanTask(FILE *ptr, task vetor[], int nLinhas){
   for (int i = 0; i < nLinhas; i++){
      fscanf(ptr, "%s %d\n", vetor[i].label, &vetor[i].time);
   }
}

int getLines(FILE *ptr){
   int count = 0;
   for(char c = getc(ptr); c != EOF; c = getc(ptr)){
      if(c == '\n'){
         count++;
      }
   }
   rewind(ptr);
   return count;
}

void addTask(processor *proc, task *t, int time){
   t->timeStart = time;
   t->wait = 0;
   
   proc->history[proc->size] = *t;
   proc->size++;
   proc->busyUntil = proc->busyUntil + t->time;
   strcpy(proc->currentProcess, t->label);
   
}

void printTask(task vetor[], int nLinhas){
   printf("================\n");
   for (int i = 0; i < nLinhas; i++){
      printf("%s %d\n", vetor[i].label, vetor[i].time);
   }
   printf("================\n");
}

void start(processor vetor[], task vetor2[], int nProc, int nLinhas){
   for (int i = 0; i < nProc; i++){
      vetor[i].size = 0;
      vetor[i].busyUntil = 0;
   }

   for (int j = 0; j < nLinhas; j++){
      vetor2[j].timeStart = 0;
      vetor2[j].wait = 1;
   }
}

void printProc(FILE *saida, processor proc [], int nProc){
   for(int i = 0; i < nProc; i++){
      fprintf(saida, "Processador_%d\n", i+1);
      for(int j = 0; j < (proc[i].size); j++){
         fprintf(saida, "%s;%d;%d\n", proc[i].history[j].label, (proc[i].history[j].timeStart), (proc[i].history[j].time + (proc[i].history[j].timeStart)));
      }
      fprintf(saida, "\n");  
   }
}

int contaTempo(processor proc[], int nProc){
   int aux = 0;
   for (int i = 0; i <nProc; i ++){
      if(proc[i].busyUntil > aux){
         aux = proc[i].busyUntil;
         return aux;
      }
   }
}

int main(int argc, char *argv[]){
   int time = 0;
   int nProc = atoi(argv[2]);
   int elapsedTime = 0;

   FILE *entrada;
   FILE *saida;

   processor kernels[8];

   entrada = fopen(argv[1], "r");
   saida = fopen("saida.txt", "w");
   checkNull(entrada);
   checkNull(saida);
   
   int nLinhas = getLines(entrada);
   task taskList[nLinhas];

   start(kernels, taskList, nProc, nLinhas);
   scanTask(entrada, taskList, nLinhas);
   
   printf("=======================\n");
   if (strcmp(argv[3], "SJF") == 0){
      printf("Opção escolhida: SJF ||\n");
      bolhasort(taskList, nLinhas);
   }
   else if (strcmp(argv[3], "LJF") == 0){
      printf("Opção escolhida: LJF ||\n");
      reversesort(taskList, nLinhas);
   }
   while(time < 100){//rever
      for(int i = 0; i < nLinhas; i++){
         for(int j = 0; j < nProc; j++){
            if((time >= kernels[j].busyUntil) && taskList[i].wait){
               addTask(&kernels[j], &taskList[i], time);
            }
         }
      }
      time ++;
   }
   
   elapsedTime = contaTempo(kernels, nProc);
   printf("Tempo decorrido: %d  ||\n", elapsedTime);
   printProc(saida, kernels, nProc);
   printf("=======================\n");
   fclose(entrada);
   fclose(saida);
}


