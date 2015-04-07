#include "tournaPredictor.h"

tournaPredictor::tournaPredictor()
{
}
tournaPredictor::~tournaPredictor()
{
  if(predictionTable!=NULL)
    delete[] predictionTable;
}

bool tournaPredictor::create()
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

  entryNum = byteSize*8/bitWidth;
  predictionTable = new int[entryNum];
  for(int i=0; i< entryNum ; i++)
   {
     predictionTable[i]=0;
   }
  return 0;
}

int tournaPredictor::getCurrentPrediction()
{
  return currentPrediction;
}

bool tournaPredictor::stepSimulation(char *address, int branchTaken)
{
  iTotalCount++;

  unsigned int PCAddress = ctoll(address); //type convertion
  PCAddress = PCAddress >> 2;
  
  // this mask make sure we got the right bits in PCAdress
  int bitNum = int(log2(entryNum))+1;
  int index = PCAddress & (((0x1<<bitNum)-1));

  glPredictor.globalHTindex = (glPredictor.globalHTindex) & (((0x1<<bitNum)-1));
  
  //get the prediction value from their own prediction table. 
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

  // get the value from the tourament predictor
  int selector = predictionTable[index]%4;
  if (selector < 2) // weak select
    currentPrediction = predictionFromGl;
  else // strong select
    currentPrediction = predictionFromSa;
   
  //update their own prediction table
  saPredictor.predictionProcess(index, branchTaken);
  glPredictor.predictionProcess(glPredictor.globalHTindex, branchTaken);

  glPredictor.globalHTindex = ((glPredictor.globalHTindex<<1) | branchTaken);

  // we only train the selector when two predictors are different;
  // we do nothing if they are same (both correct and  wrong).
  if(predictionFromGl == predictionFromSa)
  {
    if(predictionFromSa != branchTaken){
      predictFailCount++;
    }
  }else{
    if(!predictionProcess(index, branchTaken,predictionFromSa,predictionFromGl))
      predictFailCount++;// save execute time
  }
  return 0;
}

// We prefer to firstPredictor here. and right now, 
// saPredictor is the firstPredictor, glPredictor is secondPredictor
// 00 for strong secondPredictor 01 for weak secondPredictor
// 11 for strong firstPredictor 10 for weak firstPredicctor
bool tournaPredictor::predictionProcess(int index,int branchTaken, 
					int firstPredictor, int SecondPredictor)
{
  bool correct = 1;
  int selector = predictionTable[index]%4;

  // the prefer predictor is same with branchTaken.
  if(branchTaken == firstPredictor)
  {
    switch(selector){
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
    switch(selector){
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
