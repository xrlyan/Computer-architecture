#include "globalPredictor.h"

//construct function
globalPredictor::globalPredictor()
{
  globalHTindex = 0;
}

//destructor function
globalPredictor::~globalPredictor()
{
}

//  To simulating the branch Prediction by line.
//  for a global Predictor, we only need the current 
//  branch state.
bool globalPredictor::stepSimulation(int branchTaken)
{

  iTotalCount++;

  // this mask make sure we got the right bits in PCAdress
  int bitNum = int(log2(entryNum));
  globalHTindex = (globalHTindex) & (((0x1<<bitNum)-1));

  if(!predictionProcess(globalHTindex, branchTaken))
    predictFailCount++;

  // update the globalHTindex with the new current branch state
  globalHTindex = ((globalHTindex<<1) | branchTaken);

  return 0;
}

