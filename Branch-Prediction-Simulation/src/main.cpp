/*
    CS4431 Branch Preddiction

    -- by Liang Yan
    Computer Science    

This file is the main execute file,
all simulation start from here.
 */

#include <stdio.h>
#include <iostream>
#include <string>
#include "saturaPredictor.h"
#include "globalPredictor.h"
#include "gsharePredictor.h"
#include "tournaPredictor.h"
using namespace std;

int main()
{
    int areaSize[3];
    areaSize[0]=2048; //2Kbytes
    areaSize[1]=4096; //4Kbytes
    areaSize[2]=8192; //8Kytes
    int bitWidth =2;

    FILE *file; 
    file = fopen("result.txt","a+"); /* add test result file  or create a file if it does not exist.*/ 
    for(int i=0;i<3;i++){

      class saturaPredictor saPredictor;
      saPredictor.SetSize(areaSize[i]);
      saPredictor.SetBitWidth(bitWidth);
      saPredictor.create();
    
      class globalPredictor glPredictor;
      glPredictor.SetSize(areaSize[i]);
      glPredictor.SetBitWidth(bitWidth);
      glPredictor.create();
    
      class gsharePredictor gsPredictor;
      gsPredictor.SetSize(areaSize[i]);
      gsPredictor.SetBitWidth(bitWidth);
      gsPredictor.create();
   
      class tournaPredictor toPredictor;
      // to share the area with three preiction tables, >>2 is ignoring some waste here.
      int area = areaSize[i]>>2; 
      toPredictor.SetSize(area);
      toPredictor.SetBitWidth(bitWidth);
      toPredictor.create();
      
      int maxlen = 23;  // to make sure get whole value in a line;
    
      FILE *fp;
      char filePath[5] = "test";
      char source[8];
      char target[8]; 
      char buf[23];
      int type=0; // branch = 1, jump =2, others = 3
      int branchState=0; 
      if( !(fp = fopen( filePath, "rt" ) ) ){		
	    printf("could not open the file %s",filePath);
	    return 1;
      }
      while (fgets( buf, maxlen, fp ) != NULL) {
	sscanf(buf,"%s %d %d %s",source,&type,&branchState,target);
	if(type ==1){
	  // printf("%s %d %d %s \n",source,type,branchState,target);
	  saPredictor.stepSimulation(source,branchState);
	  glPredictor.stepSimulation(branchState);
	  gsPredictor.stepSimulation(source,branchState);
	  toPredictor.stepSimulation(source,branchState);
       }
      }
      fclose(fp);
      fprintf(file,"%s,%d,%lld,%lld,%f\n","saturaPredictor",areaSize[i],saPredictor.GetiTotalCount(),saPredictor.GetpredictFailCount(),saPredictor.GetpredictionHitRate());
      fprintf(file,"%s,%d,%lld,%lld,%f\n","globalPredictor",areaSize[i],glPredictor.GetiTotalCount(),glPredictor.GetpredictFailCount(),glPredictor.GetpredictionHitRate());
      fprintf(file,"%s,%d,%lld,%lld,%f\n","gsharePredictor",areaSize[i],gsPredictor.GetiTotalCount(),gsPredictor.GetpredictFailCount(),gsPredictor.GetpredictionHitRate());
      fprintf(file,"%s,%d,%lld,%lld,%f\n","tournaPredictor",areaSize[i],toPredictor.GetiTotalCount(),toPredictor.GetpredictFailCount(),toPredictor.GetpredictionHitRate());
    }
    fclose(file); 

    return 0;
}
