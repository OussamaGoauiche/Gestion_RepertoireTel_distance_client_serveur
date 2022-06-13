# include  <stdio.h>
# include  <string.h>
# include <unistd.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <netinet/in.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <sys/types.h>
#include <signal.h>
#include<time.h>
#include <ctype.h>
#include<dirent.h>
#include<sys/stat.h>
#include<sys/param.h>
#include <fcntl.h>
struct personne
{
	char * nom;
	char *prenom;		
	char *numero;	
};

typedef struct personne personne;


void Affichage_Arbitraire(const char *repertoire)
{
	DIR* dir;
	struct dirent *entree;
	dir=opendir(repertoire);
	if(dir==NULL)
	return;
	fprintf(stdout,"%s : \n",repertoire);
	while((entree=readdir(dir))!=NULL)
	{
	fprintf(stdout,">Inode %ld|",entree->d_ino);
	fprintf(stdout,"nom %-25s|",entree->d_name);
	fprintf(stdout,"longueur %d|",entree->d_reclen);
	fprintf(stdout,"type: %u\n",entree->d_type);
	}
	closedir(dir);

}

void Affichage_trie(const char *repertoire)
{

struct dirent ** nameList;
int n,i;
n=scandir(repertoire,&nameList,NULL,alphasort);
if (n < 0) {
		fprintf(stderr,"Echec\n");
		exit(EXIT_FAILURE);
	}


for (i = 0; i < n; i++) {
		struct dirent *entree= nameList[i];
		 fprintf(stdout,">nom %-25s|",entree->d_name);
		fprintf(stdout,"Inode %ld|",entree->d_ino);
	        fprintf(stdout,"longueur %d|",entree->d_reclen);
	        fprintf(stdout,"type: %u\n",entree->d_type);
		}
for (i = 0; i < n; i++) {
		free(nameList[i]);
	}
free(nameList);
}
void List_contenue_repertoire(const char *repertoire,int choix)
{	
	if(choix==0)
	Affichage_Arbitraire(repertoire);
	else 
	Affichage_trie(repertoire);
}
void Ajouter_sRepertoire(const char *repertoire)
{
	if(mkdir(repertoire,0777)!=0)
	{
	perror("");
	return;
	}
	else
	{
	FILE *files[2];
       for (int i = 0; i < 2; i++)
       {
       char filename[20];
       sprintf(filename, "%s/Fichier%d.txt",repertoire,i);
       files[i] = fopen(filename, "a");
       }
       }
}
       
void supprimer(const char *nom)
{

char cmd[32] = { 0 };

    int ret = 0;

    sprintf(cmd, "rm -rf %s", nom);

    ret = system(cmd);

    if (ret == 0)
        printf("succe\n");
    else
        printf("Echec  %s\n", nom);

}

void Ajouter(const char *file,personne p)
{
	FILE *fp;
        fp= fopen(file, "a");
	fputs(p.nom,fp);
	fputs(";",fp);
	fputs(p.prenom,fp);
	fputs(";",fp);
	fputs(p.numero,fp);
	fputs("\0",fp);
	fputs("\n",fp);
	fclose(fp);

}
void Recherche(const char *file,const char *wrd)
{
	char  buffer[256],tmp[20];
	int n, m, i, j, k,d=0,line=0;
	
	FILE* fp;
	fp = fopen(file, "r");

	m = strlen(wrd); 

	
	line = 0;
	
	while (fgets(buffer, 256, fp) != NULL) {

		i = 0;
		n = strlen(buffer);
	
		while (i < n) {
                       j = 0;
			while (i < n && j < m && buffer[i] == wrd[j]) {
				++i, ++j;
			}

			
			if ((buffer[i] ==';') && j == m) {
				printf("%s;",wrd);
				k=0;
				i++;
				while(buffer[i]!='\n'){
				tmp[k]=buffer[i];
				i++;
				k++;
				}
				printf("%s\n",tmp);
				
			}

			
			while (i < n && buffer[i] !=';') {
				++i;
			}
			++i;

		}
		++line;

	}

         fclose(fp);

}
int RechercheLine(const char *file,const char *wrd)
{
	char  buffer[256],tmp[20];
	int n, m, i, j, k,d=0,line=0;
	
	FILE* fp;
	fp = fopen(file, "r");

	m = strlen(wrd); 

	
	line = 0;
	
	while (fgets(buffer, 256, fp) != NULL) {

		i = 0;
		n = strlen(buffer);
	
		while (i < n) {
                       j = 0;
			while (i < n && j < m && buffer[i] == wrd[j]) {
				++i, ++j;
			}

			
			if ((buffer[i] ==';') && j == m) {
				        return line;
			}

			
			while (i < n && buffer[i] !=';') {
				++i;
			}
			++i;

		}
		++line;

	}

         fclose(fp);

}



int Modifier(const char file,const char *wrd,char newln)
{
        int MAX=256;
        FILE *fptr1, *fptr2;
        int lno, linectr = 0;
        char str[MAX],fname[MAX];        
        char temp[] = "temp.txt";
		
        fptr1 = fopen(file, "r");
        if (!fptr1) 
        {
                printf("Echec ouverture fichier!!\n");
                return 0;
        }
        fptr2 = fopen(temp, "w");
        if (!fptr2) 
        {
                printf("Echec ouverture fichier temporaire!!\n");
                fclose(fptr1);
                return 0;
        }
        lno=RechercheLine(file,wrd);
        lno++;
        while (!feof(fptr1)) 
        {
            strcpy(str, "\0");
            fgets(str, MAX, fptr1);
            if (!feof(fptr1)) 
            {
                linectr++;
                if (linectr != lno) 
                    {
                        fprintf(fptr2, "%s", str);
                    } 
                    else 
                    {
                        fprintf(fptr2, "%s\n", newln);
                    }
                }
        }
        strcpy(str, "\n");
        fclose(fptr1);
        fclose(fptr2);
        remove(file);
        rename(temp,file);
        printf(" succée!! \n");
}

 void supprimer_p(char *file, const char *wrd)
{
    Modifier(file,wrd,"");
}

int main(int argc,char *argv[]){

	int ServerSock=socket(AF_INET,SOCK_STREAM,0);
	if(ServerSock<0){
	printf("error");
	exit(1);
	}
	
	int option =1;
	
	setsockopt(ServerSock,SOL_SOCKET,SO_REUSEADDR,(void*)&option,sizeof(option));
	
	struct sockaddr_in serverAdd;
	
	memset(&serverAdd,0,sizeof(struct sockaddr_in));
	
	serverAdd.sin_family=AF_INET;
	
	serverAdd.sin_port=htons(4441);
	
	serverAdd.sin_addr.s_addr=htonl(INADDR_ANY);
	
	if(bind(ServerSock,(struct sockaddr *)&serverAdd,sizeof(struct sockaddr_in))==0)
	
	printf("Connexion est bien fait sur le port 4440 \n");
	
	else 
	{
	printf("echec de bind bind \n");
	}
	if(listen(ServerSock,4)==0)
	printf("au cours de listing\n");
	else
	 {
	 printf("echec de lsiting ");
	 exit(1);
	 }
	 
	 struct sockaddr_in connectedClientAddress;
	 int clientaddrlength=0;
	 while(1){
	 memset(&connectedClientAddress,0,sizeof(connectedClientAddress));
	 int connectServerSock=accept(ServerSock,(struct sockaddr *)&connectedClientAddress,&clientaddrlength);
	 if(clientaddrlength==-1)
	 {
	 printf("Failed to accept a connection request\n");
	 exit(1);
	 }
	 else
	 printf("new client with is connected \n");
	 
	 pid_t pid=fork();
	 
	 if(pid==-1)
	 {
	 printf("error \n");
	 continue;
	 }
	 if(pid==0){
	 close(ServerSock);
	 send(connectServerSock,"Bienvenue dans le repertoire d'universit� sultan moulay sliman",1024,0);
	 send(connectServerSock,">Taper 1 pour Affichage arbitraire",1024,0);
	 send(connectServerSock,">Taper 2 pour Affichage tri�",1024,0);
	 send(connectServerSock,">Taper 3 pour Lister le contenu du r�pertoire",1024,0);
	 send(connectServerSock,">Taper 4 pour Ajouter un sous r�pertoire contenant deux fichiers txt",1024,0);
	 send(connectServerSock,">Taper 5 pour Supprimer un r�pertoire ou un fichier donn�",1024,0);
	 send(connectServerSock,">Taper 6 pour ajouter une personne � un fichier donn�",1024,0);
	 send(connectServerSock,">Taper 7 pour Rechercher les informations d�une entr�e donn�e dans un fichier sp�cifique",1024,0);
	 send(connectServerSock,">Taper 8 pour modifier les informations d�une entr�e donn�e dans un fichier sp�cifique",1024,0);
	  send(connectServerSock,">Taper 9 pour supprimer une personne",1024,0);
	 send(connectServerSock,">entrer votre choix",1024,0);
	 char bufferrec[1024];
	 char choix[1024];
	 char nom[1024];
	 char prenom[1024];
	 char num[1024];
	 char mot[1024];
	 char new[1024];
	 while(1){
	 memset(bufferrec,0,sizeof(bufferrec));
	 recv(connectServerSock,bufferrec,1024,0);
	 printf("%s\n",bufferrec);
	 if(atoi(bufferrec)==1) {
	 	send(connectServerSock,"saisir le nom du repertoire :)",1024,0);
	 	recv(connectServerSock,bufferrec,1024,0);
	 	Affichage_Arbitraire(bufferrec);
	 }
	  if(atoi(bufferrec)==2) {
	 	send(connectServerSock,"saisir le nom du repertoire :)",1024,0);
	 	recv(connectServerSock,bufferrec,1024,0);
	 	Affichage_trie(bufferrec);
	 }
	 if(atoi(bufferrec)==3) {
	 	send(connectServerSock,"saisir le nom du repertoire :)",1024,0);
	 	recv(connectServerSock,bufferrec,1024,0);
	 	send(connectServerSock,"saisir le choix 1 ou 0 :)",1024,0);
	 	recv(connectServerSock,choix,1024,0);
	 	List_contenue_repertoire(bufferrec,atoi(choix));
	 }
	  if(atoi(bufferrec)==4) {
	 	send(connectServerSock,"saisir le nom du nouveau repertoire :)",1024,0);
	 	recv(connectServerSock,bufferrec,1024,0);
	 	Ajouter_sRepertoire(bufferrec);
	 	send(connectServerSock,"Bien crée",1024,0);
	 }
	   if(atoi(bufferrec)==5) {
	 	send(connectServerSock,"saisir le nom du  repertoire qui tu veux supprimer :)  Rq : il doit spéciefier le chemin :)",1024,0);
	 	recv(connectServerSock,bufferrec,1024,0);
	 	supprimer(bufferrec);
	 	send(connectServerSock,"Bien supprimer",1024,0);
	 }
	 if(atoi(bufferrec)==6) {
	 	personne p;
	 	send(connectServerSock,"saisir le chemin du fichier",1024,0);
	 	recv(connectServerSock,bufferrec,1024,0);
	 	send(connectServerSock,"saisir le nom du personne",1024,0);
	 	recv(connectServerSock,nom,1024,0);
	 	p.nom=nom;
	 	send(connectServerSock,"saisir le pernom du personne",1024,0);
	 	recv(connectServerSock,prenom,1024,0);
	 	p.prenom=prenom;
	 	send(connectServerSock,"saisir le numéro du personne",1024,0);
	 	recv(connectServerSock,num,1024,0);
	 	p.numero=num;
	 	Ajouter(bufferrec,p);
	 	send(connectServerSock,"Bien ajouter",1024,0);
	 }
	  if(atoi(bufferrec)==7) {
	 	
	 	send(connectServerSock,"saisir le chemin du fichier",1024,0);
	 	recv(connectServerSock,bufferrec,1024,0);
	 	send(connectServerSock,"saisir le mot a chercher",1024,0);
	 	recv(connectServerSock,mot,1024,0);
	 	 Recherche(bufferrec,mot);
	 	send(connectServerSock,"Bien trouver",1024,0);
	 }
	 
	   if(atoi(bufferrec)==8) {
	 	
	 	send(connectServerSock,"saisir le chemin du fichier",1024,0);
	 	recv(connectServerSock,bufferrec,1024,0);
	 	send(connectServerSock,"saisir le mot a modifier",1024,0);
	 	recv(connectServerSock,mot,1024,0);
	 	send(connectServerSock,"saisir le nouveau mot ",1024,0);
	 	recv(connectServerSock,new,1024,0);
	 	Modifier(bufferrec,mot,new);
	 	send(connectServerSock,"Bien modifier",1024,0);
	 }
	  if(atoi(bufferrec)==9) {
	 	personne *p;
	 	send(connectServerSock,"saisir le chemin du fichier",1024,0);
	 	recv(connectServerSock,bufferrec,1024,0);
	 	
	 	send(connectServerSock,"saisir le nom du personne",1024,0);
	 	recv(connectServerSock,nom,1024,0);
	 	
	 	supprimer_p(bufferrec,nom);
	 	send(connectServerSock,"Bien supprimer",1024,0);
	 }
	 
	 
	 if(bufferrec[0]=='?')
	 {
	 printf("Client is déconnecté\n");
	 close(connectServerSock);
	 exit(15);
	 }
	 else{
	 send(connectServerSock,bufferrec,1024,0);
	 }
	 }
	 }
	 else
	 close(connectServerSock);
	 }
	return 0;
}