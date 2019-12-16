#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <math.h>
#include <stdio.h>                            //***Bilgehan Geçici 150117072*** 
#include <string.h>
#include <ctype.h>
#include <locale.h>

 struct node { 
 
	char *word;
	struct node *next;
	char *docId;
	int counter;
};

typedef struct node node;
typedef struct node* nodePtr;
typedef struct node** nodePtrPtr;

void insert(nodePtrPtr header, char *string, char *documentId){ // Insert the different words from all .txt files to MLL.

	nodePtr newNode, temp;
	newNode=malloc(sizeof(node));
	char *str;
	char *docID;
	str= (char *)malloc(60*sizeof(char));
    docID = (char *)malloc(60*sizeof(char));
		
    strcpy(str,string);
    strcpy(docID,documentId);
	newNode->next=NULL;
	newNode->word = str;
	newNode->docId = docID;
	newNode->counter = 1;
    
	// if LL empty
	if (*header == NULL){
		*header=newNode;
	}else{
		temp=*header;
		
		if(strcmp(temp->word,newNode->word)==0 && strcmp(temp->docId , newNode->docId) == 0){
			newNode->counter++;
			return;
		}
		
		while (temp->next != NULL){
			temp=temp->next;
			
			if(strcmp(temp->word,newNode->word)==0 && strcmp(temp->docId , newNode->docId) == 0){
				newNode->counter++;
				return;
			}	
		}
				
		temp->next = newNode;
		temp->next->next = NULL;
	}						
}

void insertFrequent(nodePtrPtr header, char *string){ // Insert the different words to the list. It also keeps the word's frequencies.
	nodePtr newNode, temp;
	newNode=malloc(sizeof(node));
	char *str;
	
	str= (char *)malloc(60*sizeof(char));
    strcpy(str,string);
	newNode->next=NULL;
	
	newNode->word = str;
	newNode->counter = 1;
	
	if (*header == NULL){
		*header=newNode;
	}else{
		
		temp=*header;
		
		while (temp->next != NULL){
			temp=temp->next;
			if(strcmp(temp->word,newNode->word)==0){
				
				temp->counter++;
				return;			
			}	
		}		
		temp->next = newNode;
		temp->next->next = NULL;
	}						
}

nodePtr sortList(nodePtr head) { //Sort the list in descending order.

    nodePtr tmpPtr = head;
    nodePtr tmpNext = head->next;

    int temp;
    char *string;

    while(tmpNext != NULL){
           while(tmpNext != tmpPtr){
                    if(tmpNext->counter > tmpPtr->counter){
                    	
                            temp = tmpPtr->counter;
                            string = tmpPtr->word;
                            tmpPtr->counter = tmpNext->counter;
                            tmpPtr->word = tmpNext->word;
                            tmpNext->counter = temp;
                            tmpNext->word = string;
                    }
                    tmpPtr = tmpPtr->next;
            }
            tmpPtr = head;
            tmpNext = tmpNext->next;
    }
         return tmpPtr ; // Place holder
}  

int numOfFiles(char *name){ //Get the number of files in that given directory.

	DIR *folder;
	struct dirent *entry;
	folder = opendir(name); // Open the given directory.
	int numOfFiles = 0;
    if(folder == NULL){ // Check the directory is null.
    	
        perror("File cannot be opened.");
        return(1);
    }

    while( (entry=readdir(folder)) ){ // Get the number of .txt files in that given directory.
    
        numOfFiles++;   
    }
    closedir(folder); // Folder closed.
    return numOfFiles - 2;
}
  	  
void printList(node *currentPtr){ // Print the basic MLL with their frequencies.
	
     if(currentPtr->next ==  NULL){
     	printf("The list is empty");
	 }
	 
      while (currentPtr != NULL) { 
         printf(" %d %s --> ",currentPtr->counter,currentPtr->word);
         currentPtr = currentPtr->next;   
      } 
   } 
   
void printFirstOrder(node *currentPtr){ // Print the first order sequence between the words.

	nodePtr tempNode = NULL ;
    nodePtr temp = currentPtr;
    
	while(temp != NULL) {
		for(tempNode = temp->next ; tempNode != NULL ; tempNode = tempNode->next) {
			if(strcmp(tempNode->docId , temp->docId) == 0) {
			printf("(%s,%s)" , temp->word , tempNode->word ) ;
			}	
		}
	temp = temp->next ;	
	}	
}

void printSecondOrder(node* currentPtr){ // Print the second order sequence between the words.
	
	nodePtr mainLink = currentPtr;
	nodePtr secondLink = currentPtr;
	nodePtr checker = NULL;
	nodePtr checker2 = NULL;
	
	while(mainLink != NULL) {
		for(checker = mainLink->next ; checker!= NULL ; checker = checker->next) {
			if(strcmp(mainLink->docId , checker->docId) != 0 && 
			   strcmp(mainLink->word , checker->word) == 0  ) {
				 while(secondLink != NULL) {	
					  if(strcmp(secondLink->docId , mainLink->docId) == 0 && strcmp(secondLink->word , mainLink->word) != 0 ) {
						  for(checker2 = secondLink->next ; checker2 != NULL ; checker2 = checker2->next) {
							   if(strcmp(checker2->docId , checker->docId) == 0 && 
								  strcmp(checker2->word , checker->word) != 0 && 
								  strcmp(checker2->word , secondLink->word) != 0) {
							  	  printf("(%s,%s)",secondLink->word,checker2->word);
								}
							}	
						}
				secondLink = secondLink->next;
				}							
			}
		}
	mainLink = mainLink->next;	
    }	     
 } 
 
void printThirdOrder(node *currentPtr){  // Print the third order sequence between the words.
	
	nodePtr mainLink = currentPtr;
	nodePtr secondLink = currentPtr;
	nodePtr ThirdLink = currentPtr;
	nodePtr FourthLink = currentPtr;
	nodePtr FifthLink = currentPtr;
	nodePtr checker = currentPtr;
	
	while(mainLink != NULL){
		while(checker != NULL){
			if(strcmp(mainLink->docId, checker->docId) != 0 && strcmp(mainLink->word, checker->word) == 0){
				for(secondLink; secondLink != NULL; secondLink = secondLink->next){
					if(strcmp(secondLink->docId, checker->docId) == 0){
						for(ThirdLink; ThirdLink!= NULL; ThirdLink = ThirdLink->next){
					        if(strcmp(ThirdLink->docId, mainLink->docId) != 0 && strcmp(ThirdLink->word, secondLink->word) == 0 && strcmp(ThirdLink->docId, secondLink->docId) != 0){
								for(FourthLink; FourthLink != NULL; FourthLink = FourthLink->next){
									if(strcmp(FourthLink->docId, ThirdLink->docId) == 0 && strcmp(FourthLink->word, ThirdLink->word) != 0){
										for(FifthLink; FifthLink!=NULL; FifthLink = FifthLink->next){
											if(strcmp(FifthLink->docId, mainLink->docId) == 0 && strcmp(FifthLink->word, mainLink->word) != 0){
												printf("(%s,%s)", FifthLink->word, FourthLink->word);
											}
										}
										FifthLink = currentPtr;
									}
								}
								FourthLink = currentPtr;			
							}
						}
						ThirdLink = currentPtr;	
					}
				   secondLink = secondLink->next;
				}
			}
		   checker = checker->next;
	   }
		mainLink = mainLink->next;	
	}
}
 
void printEconFrequencyList(node *currentPtr,int EconFiles){ // Print the Econ documents word's frequencies and their Idf values.
	printf("---------------------ECON----------------------\n");
	printf("%s %15s %5s\n","Most Frequent Words   ","Number of Times   ","IDF");
  	printf("\n");
	int count = 0;
	float Idf = 0;
  	while (currentPtr != NULL) { 
  	     Idf = log((float)EconFiles/currentPtr->counter);
  	     Idf = Idf * currentPtr->counter;
         printf("%s\t  %21d %16.2lf\n",currentPtr->word,currentPtr->counter,Idf);
         count++; 
         currentPtr = currentPtr->next;   
         if(count == 5)
         break;
      } 	
}

void printHealthFrequencyList(node *currentPtr,int HealthFiles){ // Print the Health documents word's frequencies and their Idf values.
	
	printf("-------------------HEALTH----------------------\n");
	printf("%s %15s %5s\n","Most Frequent Words   ","Number of Times   ","IDF");
  	printf("\n");
	int count = 0;
	float Idf = 0;
  	while (currentPtr != NULL) { 
  	     Idf = log((float)HealthFiles/currentPtr->counter);
  	     Idf = Idf * currentPtr->counter;
         printf("%s\t  %21d %16.2lf\n",currentPtr->word,currentPtr->counter,Idf);
         count++; 
         currentPtr = currentPtr->next;   
         if(count == 5)
         break;
      } 	
}

void printMagFrequencyList(node *currentPtr,int MagFiles){ // Print the Magazin documents word's frequencies and their Idf values.
	
	printf("-------------------MAGAZÝN---------------------\n");
	printf("%s %15s %5s\n","Most Frequent Words   ","Number of Times   ","IDF");
  	printf("\n");
	int count = 0;
	float Idf = 0;
  	while (currentPtr != NULL) { 
  	     Idf = log((float)MagFiles/currentPtr->counter);
  	     Idf = Idf * currentPtr->counter;
         printf("%s\t  %21d %16.2lf\n",currentPtr->word,currentPtr->counter,Idf);
         count++; 
         currentPtr = currentPtr->next;   
         if(count == 5)
         break;
      } 
}

int main () {
	
	setlocale(LC_ALL, "Turkish"); // This is for turkish letters. 
    nodePtr root = NULL; // Master Link list.
	nodePtr EconLink = NULL; // Econ document's linkedList.
	nodePtr HealthLink = NULL ; // Health document's LinkedList.
	nodePtr MagazinLink = NULL ; // Magazin document's LinkedList.
	
	int EconTxtId;
	int HealthTxtId;
	int MagTxtId;
	
    FILE *fp;
	DIR *EconFolder;
    int EconFiles = 0;
    int HealthFiles = 0;
    int MagFiles = 0;
    int totalNumberOfDocuments = 0;
	  
    char *econ = "dataset/econ/";
    char *health = "dataset/health/";
    char *mag = "dataset/magazin/";
    char *DocID = NULL; 
    
    EconFiles = numOfFiles(econ); // Returns number of econ's folder.txt files.
    HealthFiles = numOfFiles(health); // Returns number of health's folder.txt files.
    MagFiles = numOfFiles(mag); // Returns number of magazin's folder.txt files.
    
    int i;
    char lineEcon[500];
    char *tokenEcon = NULL;
    char bufferEcon[2];
    char delim[] = " ?\n";
   
    for(i = 1; i <= EconFiles; i++){ // Reading econ document's .txt files 
      	char econ[50];
      	strcpy(econ, "dataset/econ/");
      	itoa(i,bufferEcon,10);
      	char *filename= NULL;
      	char *filenameExtension2 = NULL;
      	filename = econ;
      	filenameExtension2 = strcat(bufferEcon,".txt");
      	strcat(filename, filenameExtension2); 
        DocID = filename;
      	fp = fopen(filename, "r");
      	if (fp == NULL) return 1;
	    while(fgets(lineEcon,sizeof lineEcon,fp)!= NULL) { // Take the words from econ document's .txt's files.
	    	
		     tokenEcon = strtok(lineEcon, delim);
		     
		     while (tokenEcon != NULL){
		     	
			   insert(&root,tokenEcon, DocID); // Add the given word to the MLL.
			   insertFrequent(&EconLink,tokenEcon); // Add the given word to the EconLink List.
               tokenEcon = strtok(NULL, delim);  
          } 
	   }
	}
	 
	fclose(fp);
	
	   int j ;
       char lineHealth[500];
       char *tokenHealth = NULL;
       char bufferHealth[2];
       
    for(j = 1; j <= HealthFiles; j++){ // Reading health document's .txt files 
      	char health[50];
      	strcpy(health, "dataset/health/");
      	itoa(j,bufferHealth,10);
      	char *filename= NULL;
      	char *filenameExtension2 = NULL;
      	filename = health;
      	filenameExtension2 = strcat(bufferHealth,".txt");
      	strcat(filename, filenameExtension2);
		int result; 
        DocID = filename;
      	fp = fopen(filename, "r");
      	if (fp == NULL) return 1;
      	
	    while(fgets(lineHealth,sizeof lineHealth,fp)!= NULL) {  // Take the words from health document's .txt's files.
	    	
		   tokenHealth=strtok(lineHealth, delim);
		   
		   while (tokenHealth != NULL){
		   		
               insert(&root,tokenHealth,DocID); // Add the given word to the MLL.
			   insertFrequent(&HealthLink,tokenHealth); // Add the given word to the HealthLink List.
               tokenHealth = strtok(NULL, delim); 
			  }
           }  
        }
     
	 fclose(fp);
	 
	  int k ;
	  char lineMag[500];
      char *tokenMag = NULL; 
      char bufferMag[2];
      
    for(k = 1; k <= MagFiles; k++){ // Reading magazin document's .txt files 
      	char Mag[50];
      	strcpy(Mag, "dataset/magazin/");
      	itoa(k,bufferMag,10);
      	char *filename= NULL;
      	char *filenameExtension2 = NULL;
      	filename = Mag;
      	filenameExtension2 = strcat(bufferMag,".txt");
      	strcat(filename, filenameExtension2); 
        DocID = filename;
      	fp = fopen(filename, "r");
      	if (fp == NULL) return 1;
	    
	    while(fgets(lineMag,sizeof lineMag,fp)!= NULL) {  // Take the words from magazin document's .txt's files.
	    	
		    tokenMag=strtok(lineMag,delim);
		    
		    while (tokenMag != NULL){
   
             insert(&root,tokenMag, DocID); // Add the given word to the MLL. 
			 insertFrequent(&MagazinLink,tokenMag); // Add the given word to the EconLink List.
             tokenMag = strtok(NULL, delim);  
          }
      }
   }
   
   fclose(fp);
   
    printf("1st term co-occurance order: \n") ,
    printFirstOrder(root);
    printf("\n");
    printf("\n");
    printf("2nd term co-occurance order: \n") ,
    printSecondOrder(root);
    printf("\n");
    printf("\n");
    printf("3rd term co-occurance order: \n") ,
    printThirdOrder(root);
	printf("\n");
	printf("\n");
	printf("\n");
	printf("\n");
	
	EconLink = sortList(EconLink); // Sort the econ link.
	HealthLink = sortList(HealthLink); // Sort the health link.
	MagazinLink = sortList(MagazinLink); // Sort the magazin link.
	
	printEconFrequencyList(EconLink,EconFiles);
	printf("\n");
	printHealthFrequencyList(HealthLink,HealthFiles);
	printf("\n");
    printMagFrequencyList(MagazinLink,MagFiles);			
}
