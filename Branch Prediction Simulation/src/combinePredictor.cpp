#include "combinePredictor.h"

combinePredictor::combinePredictor()
{
}

//
combinePredictor::~combinePredictor()
{
}


bool combinePredictor::create()
{
  // create the children class first.
  // make sure both predictors are initialized
  saPredictor.SetSize(byteSize);
  saPredictor.SetBitWidth(bitWidth);
  saPredictor.create();
 
  glPredictor.SetSize(byteSize);
  glPredictor.SetBitWidth(bitWidth);
  glPredictor.globalHTindex = 0;
  glPredictor.create();
  
  gsPredictor.SetSize(byteSize);
  gsPredictor.SetBitWidth(bitWidth);
  gsPredictor.globalHTindex = 0;
  gsPredictor.create();

  entryNum = byteSize*8/bitWidth;
  predictionTable = new int[entryNum];
  for(int i=0; i< entryNum ; i++)
   {
     predictionTable[i]=0;
   }

  return 0;
}


bool combinePredictor::stepSimulation(char *address, int branchTaken)
{
  long long int PCAddress = ctoll(address); //type convertion
  iTotalCount++;

  PCAddress = PCAddress >> 2;

  // this mask make sure we got the right bits in PCAdress
  int bitNum = int(log2(entryNum));
  int index = PCAddress & (((0x1<<bitNum)-1));

  glPredictor.globalHTindex = (glPredictor.globalHTindex) & (((0x1<<bitNum)-1));
  gsPredictor.globalHTindex = (gsPredictor.globalHTindex) & (((0x1<<bitNum)-1));


  
  int predictionFromSa;
   if(saPredictor.predictionTable[index]%4 <2)
    predictionFromSa = 0;
  else
    predictionFromSa = 1;
  
       
  int predictionFromGl; 
  if(glPredictor.predictionTable[glPredictor.globalHTindex]%4 < 2)
    predictionFromGl = 0;
  else
    predictionFromGl = 1;

  int predictionFromGs; 
  if(gsPredictor.predictionTable[gsPredictor.globalHTindex]%4 < 2)
    predictionFromGs = 0;
  else
    predictionFromGs = 1;

  // update the prediction table
  saPredictor.predictionProcess(index, branchTaken);
  glPredictor.predictionProcess(glPredictor.globalHTindex, branchTaken);
  gsPredictor.predictionProcess(gsPredictor.globalHTindex, branchTaken);

  glPredictor.globalHTindex = ((glPredictor.globalHTindex<<1) | branchTaken);
  gsPredictor.globalHTindex = ((gsPredictor.globalHTindex<<1) | branchTaken);

  // we only train the selector when two predictors are different;
  // we do nothing if they are same (both correct and  wrong).
  if((predictionFromGs == predictionFromSa)&&(predictionFromGs == predictionFromGl))
  {
    if(predictionFromGs != branchTaken){
      predictFailCount++;
    }
  }else{

    switch(combineMode){
    case 0:
      if(!predictionProcess(index, branchTaken,predictionFromSa,predictionFromGl))
	predictFailCount++;
      break;
    case 1:
      if(!predictionProcess(index, branchTaken,predictionFromGl,predictionFromSa))
	predictFailCount++;
      break;
    case 2:
      if(!predictionProcess(index, branchTaken,predictionFromSa,predictionFromGs))
	predictFailCount++;
      break;
    case 3:
      if(!predictionProcess(index, branchTaken,predictionFromGs,predictionFromSa))
	predictFailCount++;
      break;
    case 4:
      if(!predictionProcess(index, branchTaken,predictionFromGl,predictionFromGs))
	predictFailCount++;
      break;
    case 5:
      if(!predictionProcess(index, branchTaken,predictionFromGs,predictionFromGl))
	predictFailCount++;
      break;
    }
  }
  return 0;
}



// We prefer to firstPredictor here. and right now, 
// saPredictor is the firstPredictor, glPredictor is secondPredictor
// 00 for strong secondPredictor 01 for weak secondPredictor
// 11 for strong firstPredictor 10 for weak firstPredicctor
bool combinePredictor::predictionProcess(int index,int branchTaken, 
					int firstPredictor, int SecondPredictor)
{
  bool correct = 1;
  int prediction = predictionTable[index]%4;
  //  printf("prediction = %d \n",prediction);
  // the prefer predictor is same with branchTaken.
  if(branchTaken == firstPredictor)
  {
    switch(prediction){
    case 0:
      predictionTable[index]++;
      correct = 0;// even the prefer predictor is right, however the selector chooses a wrong predictor
      break;
    case 1:
      predictionTable[index]+=2;
      correct = 0;// even the prefer predictor is right, however the selector chooses a wrong predictor
      break;
    case 2:
      predictionTable[index]++;
      break;
    case 3:
      break;
    }
  }else{
    switch(prediction){
    case 0:
      break;
    case 1:
      predictionTable[index]--;
      break;
    case 2:
      predictionTable[index]-=2;
      correct = 0; //  the prefer predictor is wrong, and  the selector chooses this wrong predictor
      break;
    case 3:
      predictionTable[index]--;
      correct = 0;//  the prefer predictor is wrong, and  the selector chooses this wrong predictor
      break;
    }   
  }
  return correct;
}
