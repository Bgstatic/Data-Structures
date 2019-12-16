#include <stdio.h>
#include <stdlib.h>     /* Bilgehan Geçici 150117072 */
#include <string.h>
#include <math.h>

struct bin_tree { // Binary Search Tree
	
   int data;
   struct bin_tree * right, * left;

};
typedef struct bin_tree node;

void insert(node ** tree, int val){ // Insert the given value to the BST 

    node *temp = NULL;
    
    if(!(*tree)){
    
        temp = (node *)malloc(sizeof(node));
        temp->left = temp->right = NULL;
        temp->data = val;
        *tree = temp;
        return;
    }

    if(val < (*tree)->data){
    
        insert(&(*tree)->left, val);
    }
    
    else if(val > (*tree)->data){
    
        insert(&(*tree)->right, val);
    }

}

void deltree(node * tree) { // Delete the tree.

    if (tree){
    
        deltree(tree->left);
        deltree(tree->right);
        free(tree);
    }
}

void print_preorder(node * tree, FILE *fp){ // Print the pre-order version of tree.

    if (tree){
    
        fprintf(fp,"%d ",tree->data);
        print_preorder(tree->left, fp);
        print_preorder(tree->right, fp);
    }
}

void print_postorder(node * tree, FILE *fp){ // Print the post-order version of tree.

    if (tree){
    
        print_preorder(tree->left, fp);
        print_preorder(tree->right, fp);
        fprintf(fp,"%d ",tree->data);
    }
}

void printArray(int arr[], int size, FILE *fp){ // Function to print the possible topologies to the output file.
      
    if (fp == NULL){

      printf("Error opening file!\n");
   }
    int i;
    int number;
    fprintf(fp, "[");
    for(i=0; i<size; i++){
        
        fprintf(fp ,"%d ",arr[i]);
        //printf("%d ",arr[i]);
    }
     fprintf(fp,"]");
     fprintf(fp,"\n");
     //printf("]");
	 //printf("\n"); 
}

void swap(int *number1, int *number2){ // Function to swap the variables of numbers array.

    int temp;
    temp = *number1;
    *number1 = *number2;
    *number2 = temp;
}

void leftSubTree(node* root, int depth, int* max) {
    // Base Case 
    if (root == NULL) 
        return; 
  
    // If this is the first node of its level 
    if (*max < depth) { 
        printf("%d\t", root->left->data); 
        *max = depth; 
    } 
  
    // Recur for left and right subtrees 
    leftSubTree(root->left, depth + 1, max); 
    leftSubTree(root->right, depth + 1, max); 
} 

void RightSubTree(node* root, int depth, int* max) {
    // Base Case 
    if (root == NULL) 
        return; 
  
    // If this is the first node of its level 
    if (*max < depth) { 
        printf("%d\t", root->right->data); 
        *max = depth; 
    } 
  
    // Recur for left and right subtrees 
    RightSubTree(root->right, depth + 1, max); 
    RightSubTree(root->left, depth + 1, max); 
} 

void leftSide(node* root) {
    int max = 0; 
    leftSubTree(root, 1, &max); 
} 

void rightSide(node *root) {

    int max_level = 0; 
    RightSubTree(root, 1, &max_level); 
} 

int IsidenticalBST( node* root, node* checker) {
     
    if (root==NULL && checker==NULL) 
        return 1; 
  
    if (root!=NULL && checker!=NULL) {
        return
        ( 
            root->data == checker->data && 
            IsidenticalBST(root->left, checker->left) && 
            IsidenticalBST(root->right, checker->right) 
        ); 
    } 
      
    return 0; 
}  

int checkIdenticalBST(node *root , node *checker, int *array, int length, FILE *fp){ /* Check the checker tree with respect to root tree which is from input sequence if checker has same topology with root tree,then print checker tree.*/ 
																												
	    int j;    
	    for(j = 0; j <length; j++){ 
	     	
	     	insert(&checker,array[j]);	
		 }
		 
		 if(root->data != checker->data){
		 	
		 	return;
		 }
		 if(root->left->data != checker->left->data){
		 	
		 	return;
		 }
		 else if(root->right->data != checker->right->data){
		 	
		 	return;
		 }
		 if(IsidenticalBST(root, checker)){
		 	
		 	printArray(array,length,fp);
		 }
		 else{
		 	
		 	return;
		 }		
}

void possiblePoints(int *array, int i, int length, node *root, node *checker, FILE *fp) { /* Get all the possibilities of the numbers array and send it the checkIdenticalBST function.*/
                                                                               
  if (length == i){
  	
  	      node * temp = NULL;
  	      temp = checker;  
		  checkIdenticalBST(root, temp, array, length, fp);
		  deltree(temp); 
     return;
  }
  
  int j = i;
  
  for (j = i; j < length; j++) { 
  
     swap(array+i, array+j);
     possiblePoints(array, i+1, length, root, checker, fp);
     swap(array+i, array+j);
     
  }
  return;
}

int main() {
	
	FILE *fp;
	node *root; // Main tree.
	node *checker; // Checker tree for Checking the  main tree whether both of them are in the same topology.
	root = NULL;
	checker = NULL;
	char *input = NULL; // Input sequence.
	char *inputTmp = NULL; 
	input = (char*)malloc(sizeof(int)*1024); 
	inputTmp = (char*)malloc(sizeof(char)*1024);
	fp = fopen("output.txt","w+");
	
	char *token = NULL;
	char *tokenArr = NULL; 
	char delim[] = " \n";
	
    printf("Please enter the sequence: ");
    scanf("%[^\n]",input); // Take input from the user.
    
    memcpy(inputTmp, input, strlen(input)+1); // Copy the content of input to the inputTmp.
   
    int size = 0; // Array size for the numbers from the user's input. 
    token = strtok(input, delim); // Tokenizing the input.
    while(token != NULL){
    	
    	size++;
    	insert(&root, atoi(token)); // Convert the token from string to number and add it to the binary search tree.
    	token = strtok(NULL, delim);
    	
	}
	int numbers[size];
	int arr[size]; //Input number array.
	
	int i = 0;
	int j = 0;
	tokenArr = strtok(inputTmp,delim);

	while(tokenArr != NULL){
		
		numbers[i] = atoi(tokenArr); // Get the numbers from string and add it to the numbers array.
	    i++;
		tokenArr = strtok(NULL, delim);
	}

	int start = 0;
	fprintf(fp, "The pre-order traversal of input is:");
	fprintf(fp, "\n");
	print_preorder(root,fp);
	fprintf(fp, "\n");
	fprintf(fp, "The post-order traversal of input is:");
	fprintf(fp, "\n");
    print_postorder(root,fp);
    fprintf(fp,"\n");
    fprintf(fp, "The all possible topologies with respect to input tree are: ");
    fprintf(fp,"\n");
	possiblePoints(numbers, start, size, root, checker, fp);
	fclose(fp);
	printf("All possible topologies have sent to output.txt file.");
	printf("\n");
}

