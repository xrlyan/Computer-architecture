/*
    CS4431 Branch Target Buffer Simulation

    -- by Liang Yan
    Computer Science    

This file is the main execute file,
all simulation start from here.
 */

#include <stdio.h>
#include <iostream>
#include <string>
#include "btBuffer.h"
#include "tournaPredictor.h"
using namespace std;

int main()
{
    int areaSize[3];
    areaSize[0]=2048; //2Kbytes
    areaSize[1]=4096; //4Kbytes
    areaSize[2]=8192; //8Kytes
    int bitWidth =2;
    int sizePredictor=8192; 

    FILE *fp;
    int maxlen = 23;  // to make sure get whole value in a line;
    char buf[23];

    char filePath[5] = "test";
    char source[8];
    char target[8]; 
    int type=0; // branch = 1, jump =2, others = 3
    int branchState=0; 
    if( !(fp = fopen( filePath, "rt" ) ) ){		
	  printf("could not open the file %s \n",filePath);
	  return 1;
    }      

    for(int i=0;i<3;i++){
      tournaPredictor* toPredictor = new tournaPredictor();
      // to share the area with three preiction tables, >>2 is ignoring some waste here.
      toPredictor->SetSize(sizePredictor>>2);
      toPredictor->SetBitWidth(bitWidth);
      toPredictor->create();
      
      btBuffer* btb = new btBuffer();
      btb->SetSize(areaSize[i]);
      btb->create();

      rewind(fp); // back to the head of the file
      while (fgets( buf, maxlen, fp ) != NULL) {
	sscanf(buf,"%s %d %d %s",source,&type,&branchState,target);
	if(type ==1 || type == 2){
	  // since only jump and branch could reach here, we assume predition equals 1, it only
          // changes for branch 
	  int prediction = 1; 	  
	  if(type == 1){
	    toPredictor->stepSimulation(source,branchState);
	    prediction = toPredictor->getCurrentPrediction();
	  }
          btb->SetCurrentPrediction(prediction);
      	  btb->stepSimulation(source, target,branchState,type);	  
       }
      }
      // save test data
      FILE *file; 
      file = fopen("result.txt","a+"); /* add test result file  or create a file if it does not exist.*/ 
      fprintf(file,"%s,%d,%lld,%lld,%lld,%lld,%f,%f,%f\n","BTB",areaSize[i],btb->GetiTotalCount(),btb->GetMissHitCount(),
	      btb->GetWrongTargetCount(),btb->GetWrongDirectionCount(),btb->GetMissHitRate(),btb->GetWrongTargetRate(),
	      btb->GetWrongDirectionRate());
      fclose(file); 

      if(btb!=NULL)
	delete btb;
      if(toPredictor!=NULL)
	delete toPredictor;
    }
    fclose(fp);
   return 0;
}
