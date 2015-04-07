#include "saturaPredictor.h"

saturaPredictor::saturaPredictor()
{
  iTotalCount = 0;
  predictFailCount = 0;
  predictionHitRate = 1.0;
  bitWidth=2;
  byteSize=2048;
}

// destructor function
saturaPredictor::~saturaPredictor()
{
}

void saturaPredictor::SetSize(int size)
{
  byteSize = size;
}

int saturaPredictor::GetSize()
{
  return byteSize;
}

void saturaPredictor::SetBitWidth(int width)
{
  bitWidth = width;
}

long long int saturaPredictor::GetiTotalCount()
{
  return iTotalCount;
}

long long int saturaPredictor::GetpredictFailCount()
{
  return predictFailCount;
}

float saturaPredictor::GetpredictionHitRate()
{
  predictionHitRate = (iTotalCount - predictFailCount)/(iTotalCount*1.0);
  return predictionHitRate;
}

bool saturaPredictor::create()
{
  // entryNum is based on the size of prediction table size
  entryNum = byteSize*8/bitWidth;
  predictionTable = new int[entryNum];
  for(int i=0; i< entryNum ; i++)
   {
     predictionTable[i]=0;
   }
  return 0;
}

bool saturaPredictor::stepSimulation(char *address, int branchTaken)
{
  long long int PCAddress = ctoll(address);
  iTotalCount++;
  PCAddress = PCAddress >> 2;// we do not use the first two bits.

  // this mask make sure we got the right bits in PCAdress
  int bitNum = int(log2(entryNum));
  int index = PCAddress & (((0x1<<bitNum)-1));

  // I choose wrong prediction for saving time
  // sine most of the time prediction is right
  if(!predictionProcess(index, branchTaken))
    predictFailCount++;
  return 0;
}


// train the predictor
// return true if predict is right
// also change the value of prediction table
bool saturaPredictor::predictionProcess(int index,int branchTaken)
{
  bool correct = 1;
  int prediction = predictionTable[index]%4; //store two bits in an int, use the first 4bits.
  if(branchTaken == 1)//0,1 means prediction is wrong
  {
    switch(prediction){
    case 0:
      predictionTable[index]++;
      correct = 0;
      break;
    case 1:
      predictionTable[index]+=2;
      correct = 0;
      break;
    case 2:
      predictionTable[index]++;
      break;
    case 3:
      break;
    }
  }else{// 2,3 means prediction is wrong 
    switch(prediction){
    case 0:
      break;
    case 1:
      predictionTable[index]--;
      break;
    case 2:
      predictionTable[index]-=2;
      correct = 0;
      break;
    case 3:
      predictionTable[index]--;
      correct = 0;
      break;
    }   
  }
  return correct;
}

// to convert a char[] to a long long int
// to get the integer of instructure address
long long int saturaPredictor::ctoll(char s[]) 
{ 
    long long int n = 0; 
    for (int i=0; (s[i] >= '0' && s[i] <= '9') 
        || (s[i] >= 'a' && s[i] <= 'z')
	|| (s[i] >= 'A' && s[i] <= 'Z'); ++i) 
    {         
 
        if (tolower(s[i]) > '9') 
        { 
            n = 16 * n + (10 + tolower(s[i]) - 'a'); 
        } 
        else 
        { 
            n = 16 * n + (tolower(s[i]) - '0'); 
        } 
    } 
    return n; 
} 
