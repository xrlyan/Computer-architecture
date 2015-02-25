#include "gsharePredictor.h"


gsharePredictor::gsharePredictor()
{
  globalHTindex=0;
}

gsharePredictor::~gsharePredictor()
{
}

// the main function to simulate the prediction
bool gsharePredictor::stepSimulation(char *address, int branchTaken)
{
  iTotalCount++;

  long long int PCAddress = ctoll(address);
  PCAddress = PCAddress >> 2;
 
 // this mask make sure we got the right bits in PCAdress
  int bitNum = int(log2(entryNum));
  globalHTindex = (globalHTindex) & (((0x1<<bitNum)-1));
  
  // xor the pc address index and the global history table index
  // the basic of gshare predictor
  int index = (PCAddress & (((0x1<<bitNum)-1)))^globalHTindex;

  if(!predictionProcess(index, branchTaken))
    predictFailCount++;

  //update the global history table index by using the current branch state
  globalHTindex = ((globalHTindex<<1) | branchTaken) ;

  return 0;
}
