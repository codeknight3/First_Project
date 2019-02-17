#include  <stdio.h>
#include  <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include<sys/types.h> 
#include<stdbool.h>
#include<signal.h>


#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

/* Splits the string by space and returns the array of tokens
*
*/
char **tokenize(char *line)
{
  char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
  char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
  int i, tokenIndex = 0, tokenNo = 0;

  for(i =0; i < strlen(line); i++){

    char readChar = line[i];

    if (readChar == ' ' || readChar == '\n' || readChar == '\t'){
      token[tokenIndex] = '\0';
      if (tokenIndex != 0){
	tokens[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
	strcpy(tokens[tokenNo++], token);
	tokenIndex = 0; 
      }
    } else {
      token[tokenIndex++] = readChar;
    }
  }
 
  free(token);
  tokens[tokenNo] = NULL ;
  return tokens;
}

void handler(int sig)
 {
   int pid,status;
 pid=waitpid(pid,&status,WNOHANG);
if(pid>0){
 printf("Shell: Background process finished\n");}
 }

int main(int argc, char* argv[]) {
	char  line[MAX_INPUT_SIZE];            
	char  **tokens;              
	int i;

	FILE* fp;
	if(argc == 2) {
		fp = fopen(argv[1],"r");
		if(fp < 0) {
			printf("File doesn't exists.");
			return -1;
		}
	}
                
	while(1) {			
		/* BEGIN: TAKING INPUT */
		bzero(line, sizeof(line));
		if(argc == 2) { // batch mode
			if(fgets(line, sizeof(line), fp) == NULL) { // file reading finished
				break;	
			}
			line[strlen(line) - 1] = '\0';
		} else { // interactive mode
			printf("$ ");
			scanf("%[^\n]", line);
			getchar();
		}
		//printf("Command entered: %s\n", line);
		/* END: TAKING INPUT */

		line[strlen(line)] = '\n'; //terminate with new line
		tokens = tokenize(line);
   
       //do whatever you want with the commands, here we just print them
              //char *argv[100];
              char *arr[100][100];
              int arr_index=0,ind2=0;
              bool amp=false,two_amp=false,three_amp=false,is_break=false;
              int master[1000],mm=0;
           

		for(i=0;tokens[i]!=NULL;i++){
                         if(strcmp(tokens[i],"&")==0)amp=true;
                                if(strcmp(tokens[i],"&&")==0)two_amp=true;
                               if(strcmp(tokens[i],"&&&")==0)three_amp=true;
			if(strcmp(tokens[i],"&")==0 || strcmp(tokens[i],"&&")==0 || strcmp(tokens[i],"&&&")==0 )
                           {arr_index++;ind2=0;continue;}
                                  arr[arr_index][ind2] = tokens[i];
                                  arr[arr_index][ind2+1]=NULL;
                                  ind2++;
                            }
                 if(tokens[0]==NULL)continue;
                 
            if(!amp && !two_amp && !three_amp)
              {
                if(strcmp(tokens[0],"exit")==0 && tokens[1]==NULL){signal(SIGINT, SIG_DFL);is_break=true;}
                 else if(strcmp(tokens[0],"cd")==0)
                    {
                      
                      int idt= fork();
                                if(idt<0){printf("fork unsuccessful");}
                                if(idt==0){int cc=chdir(arr[0][1]); if(cc<0){printf("Shell: Incorrect command\n");exit(0);}}
                                if(idt>0){master[mm++]=idt;wait(0);}
                     }
                   else{
                               int id= fork();
                                if(id<0){printf("fork unsuccessful");}
                                if(id==0)
                                {execvp(arr[0][0],arr[0]); 
                                if(execvp(arr[0][0],arr[0])<0){printf("Shell: Incorrect command\n");exit(0);}}
                                if(id>0){master[mm++]=id;wait(0);}
                                
                        }
               }
            else if(two_amp)
               { two_amp=false;
                  int i=0;
            
                 while(arr[i][0]!=NULL)
                  {
                    if(strcmp(arr[i][0],"exit")==0 && arr[i][1]==NULL){is_break=true;signal(SIGINT, SIG_DFL);break;}
                    if(strcmp(arr[i][0],"cd")==0)
                    {
                      
                      int idt= fork();
                                if(idt<0){printf("fork unsuccessful");}
                                if(idt==0){int cc=chdir(arr[i][1]); if(cc<0){printf("Shell: Incorrect command\n");exit(0);}}
                                if(idt>0){master[mm++]=idt;wait(0);}
                     }
                   else{
                               int id= fork();
                                if(id<0){printf("fork unsuccessful");}
                                if(id==0)
                                {execvp(arr[i][0],arr[i]); 
                                if(execvp(arr[i][0],arr[i])<0){printf("Shell: Incorrect command\n");exit(0);}}
                                if(id>0){master[mm++]=id;wait(0);}
                                
                        }
                    i++;
                  }

                 
               }
            
             else if(three_amp)
               {
                  int i=0;
                  three_amp=false;
                 int cpid [100];int cpid_ind=0;int a=0;
                 while(arr[i][0]!=NULL)
                  {
                    if(strcmp(arr[i][0],"exit")==0 && arr[i][1]==NULL){is_break=true;signal(SIGINT, SIG_DFL);break;}
                    if(strcmp(arr[i][0],"cd")==0)
                    {
                      
                      int idt= fork();
                       
                         
                                if(idt<0){printf("fork unsuccessful \n");}
                                if(idt==0){int cc=chdir(arr[i][1]);if(cc<0){printf("Shell: Incorrect command\n");exit(0);}}
                               if(idt>0){cpid[cpid_ind]=idt;cpid[cpid_ind+1]=0;cpid_ind++;master[mm++]=idt;}
                     }
                   else{
                               int id= fork();
                               
                                if(id<0){printf("fork unsuccessful \n");}
                                if(id==0)
                                {execvp(arr[i][0],arr[i]); setpgid(0,getpid());
                                if(execvp(arr[i][0],arr[i])<0){printf("Shell: Incorrect command\n");exit(0);}}
                                if(id>0){cpid[cpid_ind]=id;cpid[cpid_ind+1]=0;cpid_ind++;master[mm++]=id;}
                                
                        }
                           i++;
                  }

                 while(cpid[a]!=0){waitpid(cpid[a],NULL,0);a++;}
               }
                  else if(amp)
               { 
                    amp=false;
                    int i=0;
                    int status;
                 while(arr[i][0]!=NULL)
                  {
                    if(strcmp(arr[i][0],"exit")==0 && arr[i][1]==NULL){is_break=true;signal(SIGINT, SIG_DFL);break;}
                    if(strcmp(arr[i][0],"cd")==0)
                    {
                      
                      int idt= fork();
                      
                                if(idt<0){printf("fork unsuccessful");}
                                if(idt==0){int cc=chdir(arr[i][1]); if(cc<0){printf("Shell: Incorrect command\n");exit(0);}}
                                if(idt>0){signal(SIGCHLD, handler);}
                     }
                   else{
                               int id= fork();
                                if(id<0){printf("fork unsuccessful");}
                                if(id==0)
                                {execvp(arr[i][0],arr[i]); 
                                if(execvp(arr[i][0],arr[i])<0){printf("Shell: Incorrect command\n");exit(0);}}
                                if(id>0){signal(SIGCHLD, handler);}
                                
                        }
                    
                    i++;
                     }

                }
         
                if(is_break)
                 {printf("Shell: Goodbye\n");
                       for(int k=0;k<mm;k++)kill(master[k],SIGQUIT);
                     break;}
       
		// Freeing the allocated memory	
		for(i=0;tokens[i]!=NULL;i++){
			free(tokens[i]);
		}
		free(tokens);

	}
	return 0;
}
