       #include "binomial.h"
       #include "fatal.h"
       
	                                             
        typedef struct BinNode *Position;

        struct BinNode{ // Properties of BinNode
            
            int TotalWaitingTime;
		    int ExecutionTime;
		    int ArrivalTime;
		    int WaitingTime;
		    double PriorityValue;
		    int position;
            Position LeftChild;
            Position NextSibling;
        };

        struct Collection
        {
            int CurrentSize;
            BinTree TheTrees[ MaxTrees ];
        };

        BinQueue Initialize( void )
        {
            BinQueue H;
            int i;

            H = malloc( sizeof( struct Collection ) );
            if( H == NULL )
                FatalError( "Out of space!!!" );
            H->CurrentSize = 0;
            for( i = 0; i < MaxTrees; i++ )
                H->TheTrees[ i ] = NULL;
            return H;
        }

        static void DestroyTree( BinTree T )
        {
            if( T != NULL )
            {
                DestroyTree( T->LeftChild );
                DestroyTree( T->NextSibling );
                free( T );
            }
        }

        void Destroy( BinQueue H )
        {
            int i;

            for( i = 0; i < MaxTrees; i++ )
                DestroyTree( H->TheTrees[ i ] );
        }

        BinQueue MakeEmpty( BinQueue H )
        {
            int i;

            Destroy( H );
            for( i = 0; i < MaxTrees; i++ )
                H->TheTrees[ i ] = NULL;
            H->CurrentSize = 0;

            return H;
        }
        /* Not optimized for O(1) amortized performance */
        BinQueue Insert(int ExecutionTime , double PriorityValue ,     // Insert all properties of the node.
		 int ArrivalTime , int WaitingTime ,int position, BinQueue H )
        {
            BinTree NewNode;
            BinQueue OneItem;
			int i;

            NewNode = malloc( sizeof( struct BinNode ) );
            if( NewNode == NULL )
                FatalError( "Out of space!!!" );
                
            NewNode->LeftChild = NewNode->NextSibling = NULL;
            
			NewNode->ExecutionTime = ExecutionTime;
			NewNode->ArrivalTime = ArrivalTime;
			NewNode->PriorityValue = PriorityValue;
			NewNode->WaitingTime = WaitingTime;
			NewNode->position = position; 

            OneItem = Initialize( );
            OneItem->CurrentSize = 1;
            OneItem->TheTrees[ 0 ] = NewNode;

            return Merge( H, OneItem );
        }

        ElementType DeleteMin( BinQueue H )
        {
            int i, j;
            int MinTree;   /* The tree with the minimum item */
            BinQueue DeletedQueue;
            Position DeletedTree, OldRoot;
            double MinItem;

            if( IsEmpty( H ) )
            {
                Error( "Empty binomial queue" );
				MinItem=-Infinity;
                return MinItem;
            }

            MinItem = Infinity;
            for( i = 0; i < MaxTrees; i++ )
            {
                if( H->TheTrees[ i ] && H->TheTrees[ i ]->PriorityValue < MinItem )
                {
                    /* Update minimum */
                    MinItem = H->TheTrees[ i ]->PriorityValue;
					
                    MinTree = i;
                }
            }

            DeletedTree = H->TheTrees[ MinTree ];
            OldRoot = DeletedTree;
            DeletedTree = DeletedTree->LeftChild;
            free( OldRoot );

            DeletedQueue = Initialize( );
            DeletedQueue->CurrentSize = ( 1 << MinTree ) - 1;
            for( j = MinTree - 1; j >= 0; j-- )
            {
                DeletedQueue->TheTrees[ j ] = DeletedTree;
                DeletedTree = DeletedTree->NextSibling;
                DeletedQueue->TheTrees[ j ]->NextSibling = NULL;
            }

            H->TheTrees[ MinTree ] = NULL;
            H->CurrentSize -= DeletedQueue->CurrentSize + 1;

            Merge( H, DeletedQueue );
            return MinItem;
        }

        BinTree FindMin( BinQueue H ,int position) // Find the minumum priorityValue from binomial tree.
        {
            int i;
            BinTree MinNode;
            MinNode = malloc( sizeof( struct BinNode ) );

            if( IsEmpty( H ) )
            {
                Error( "Empty binomial queue" );
				MinNode->PriorityValue = 0;
                return MinNode;
            }

            MinNode->PriorityValue = Infinity;
            	
			
            for( i = 0; i < MaxTrees; i++ )
            {
                if( H->TheTrees[ i ] && (H->TheTrees[ i ]->PriorityValue < MinNode->PriorityValue)){
		             
		            MinNode->position = position;
                    MinNode = H->TheTrees[i];
                    
				}	
            }
               
            return MinNode;
        }

        int IsEmpty( BinQueue H )
        {
            return H->CurrentSize == 0;
        }

        int IsFull( BinQueue H )
        {
            return H->CurrentSize == Capacity;
        }

        /* Return the result of merging equal-sized T1 and T2 */
        BinTree CombineTrees( BinTree T1, BinTree T2 )
        {
            if( T1->PriorityValue > T2->PriorityValue )
                return CombineTrees( T2, T1 );
            if(T1->PriorityValue == T2->PriorityValue){
            	if(T1->ArrivalTime > T2->ArrivalTime){
            		return CombineTrees( T2 , T1 );
				}           	
			}
            T2->NextSibling = T1->LeftChild;
            T1->LeftChild = T2;
            return T1;
        }
        
        /* Merge two binomial queues */
        /* Not optimized for early termination */
        /* H1 contains merged result */
        BinQueue Merge( BinQueue H1, BinQueue H2 )
        {
            BinTree T1, T2, Carry = NULL;
            int i, j;

            if( H1->CurrentSize + H2->CurrentSize > Capacity )
                Error( "Merge would exceed capacity" );

            H1->CurrentSize += H2->CurrentSize;
            for( i = 0, j = 1; j <= H1->CurrentSize; i++, j *= 2 )
            {
                T1 = H1->TheTrees[ i ]; T2 = H2->TheTrees[ i ];

                switch( !!T1 + 2 * !!T2 + 4 * !!Carry )
                {
                    case 0: /* No trees */
                    case 1: /* Only H1 */
                        break;
                    case 2: /* Only H2 */
                        H1->TheTrees[ i ] = T2;
                        H2->TheTrees[ i ] = NULL;
                        break;
                    case 4: /* Only Carry */
                        H1->TheTrees[ i ] = Carry;
                        Carry = NULL;
                        break;
                    case 3: /* H1 and H2 */
                        Carry = CombineTrees( T1, T2 );
                        H1->TheTrees[ i ] = H2->TheTrees[ i ] = NULL;
                        break;
                    case 5: /* H1 and Carry */
                        Carry = CombineTrees( T1, Carry );
                        H1->TheTrees[ i ] = NULL;
                        break;
                    case 6: /* H2 and Carry */
                        Carry = CombineTrees( T2, Carry );
                        H2->TheTrees[ i ] = NULL;
                        break;
                    case 7: /* All three */
                        H1->TheTrees[ i ] = Carry;
                        Carry = CombineTrees( T1, T2 );
                        H2->TheTrees[ i ] = NULL;
                        break;
                }
            }
            return H1;
        }

        int ProcessTime;     
        void UpdateWaitingTime(int WaitingTime , BinTree BinomialTree ){ // Updates the waiting time from Binomial Tree.
		
			if( BinomialTree != NULL){
				
				BinomialTree->WaitingTime += WaitingTime;
				UpdateWaitingTime(WaitingTime , BinomialTree->LeftChild);
				UpdateWaitingTime(WaitingTime , BinomialTree->NextSibling);
			}
		}
		
		void UpdateWaitingTimeHeap(int waitingtime , BinQueue H ){ // Updates the waiting time form Binomial Heap vie Binomial Trees.
			int i;
			for(i = 0; i < MaxTrees; i++ ){
				
				UpdateWaitingTime(waitingtime , H->TheTrees[ i ] );
			}
		}
		
		int CalculatePriorityValue(int eMax , int QuantumTime, int position, int WaitingTime, BinQueue H){ // Calculate the priority value for the new insertion.
		
			int TotalWaitingTime = 0 ;
			
			if(!IsEmpty(H)){
				 
				int exeTime;
				BinTree Minumum = FindMin(H,position);
				exeTime= Minumum->ExecutionTime;
				double MainformulaPart;
				double priorityValue = 0;
				
						if(exeTime > QuantumTime){
							eMax = 3 * eMax * (-1);
							exeTime -= QuantumTime;
							exeTime *= 2;
							MainformulaPart =  1 / exp(pow((exeTime/(double)eMax),3.0));
							exeTime /= 2;
							
							priorityValue = MainformulaPart * exeTime;
							ProcessTime += QuantumTime;
							int ArrivalTime = Minumum->ArrivalTime;
							int WaitingTime = Minumum->WaitingTime;
							DeleteMin(H);
							UpdateWaitingTimeHeap(QuantumTime , H);
							Insert(exeTime , priorityValue , ArrivalTime , WaitingTime ,position, H);
							
						}
						else{ // Find the total waiting time.
							
							if(exeTime <= QuantumTime){
							 
							TotalWaitingTime += Minumum->WaitingTime;
							Minumum->TotalWaitingTime = TotalWaitingTime;
							
							DeleteMin(H);
								
						 if(exeTime == QuantumTime){
							
							ProcessTime += QuantumTime;
							UpdateWaitingTimeHeap(QuantumTime , H);
							
							}
						 else if(QuantumTime > exeTime){
							
							ProcessTime += exeTime;
							UpdateWaitingTimeHeap(exeTime , H);
							
							}
								
						}
			}
			return TotalWaitingTime;
		  }
		}
 
		main(){
			
			int time;
			BinQueue H1;
			int TotalWaitingTime = 0; 	
			int position;
			int ExecutionTime;			
			int ArrivalTime;
			int WaitingTime;
			double AWT;
			int q;
			int eMax;
			int ProcessCounter = 0;
			H1 = Initialize();
			FILE *fp;
			
			fp = fopen("input.txt" , "r");
			while(!feof(fp)){ // Find the eMax from given input.
				
				fscanf(fp , "%*1s%d %d %d" , &position , &ExecutionTime , &ArrivalTime);
				
				if(ExecutionTime > eMax){
					eMax = ExecutionTime;
				}
			}
			fclose(fp);
			fp = NULL;
			for(q=1; q <= 10; q++){ // Quantum time loop.
		        time = 0; 
			    ProcessCounter = 0; 
			    ProcessTime = 0;
			    fp = fopen("input.txt" , "r");
				fscanf(fp , "%*1s%d %d %d" , &position , &ExecutionTime , &ArrivalTime); // Get all the variables from input.
					while(!feof(fp)){
						
						if(ArrivalTime == time){ // Find the waiting time of the given process for insertion (Calculations will be done later).
							
							if(ProcessTime != time){
								
								WaitingTime = ProcessTime - ArrivalTime;
							}
							else{
								
								WaitingTime = 0;
							}
							Insert(ExecutionTime , ExecutionTime , ArrivalTime ,WaitingTime ,position, H1); // Insert the given process with its properties.
							ProcessCounter++; // Increment counter after every insertion.
							
						
							if(ProcessTime == time){
									
								TotalWaitingTime += CalculatePriorityValue(eMax , q ,position, WaitingTime, H1); //Get the total waiting time while insertion are on the way.
								
							}
							fscanf(fp , "%*1s%d %d %d" , &position , &ExecutionTime , &ArrivalTime);
							
							time += 1;	//Increment the pyhsical time by one.				
						}
						else{
							
							if(!IsEmpty(H1)){ 
								
								if(ProcessTime == time){
										
								TotalWaitingTime +=	CalculatePriorityValue(eMax , q , position, WaitingTime, H1);
							
								}
							}					
							time += 1;
						}
					}
					fclose(fp);
					
					while(!IsEmpty(H1)){ //Get the total waiting time after the insertion ends.
						
						if(ArrivalTime == time){
							
							if(ProcessTime == time){
								
								TotalWaitingTime += CalculatePriorityValue(eMax , q , position ,WaitingTime, H1);
							   
							}
							if(ProcessTime != time){
								
								WaitingTime = ProcessTime - ArrivalTime;
							}
							
							Insert(ExecutionTime , ExecutionTime , ArrivalTime , WaitingTime ,position, H1);
							ProcessCounter++;
						}
						else{
							
							if(ProcessTime == time){
								
							  TotalWaitingTime += CalculatePriorityValue(eMax , q , position, WaitingTime,  H1);
							    
							}
                           
						}
						time+=1;
					}
					
					if(IsEmpty(H1)){ // Printing results.
						
						AWT = ((double)TotalWaitingTime / ProcessCounter);
						printf("Quantum Time =  %d --> AWT = %.2lf\n" , q, AWT);
						printf("\n");								
					}
					 
			        TotalWaitingTime = 0;					
			}
			MakeEmpty(H1);
			
		}
