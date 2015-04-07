#ifndef BTBUFFER_H_
#define BTBUFFER_H_

#include <stdio.h>
#include <string>
#include <math.h>

struct btbData
{
  int  valid;
  unsigned int pcTag;
  unsigned int targetAddress;
  int  type;
} ; 

class btBuffer
{
 private:
  int currentPrediction; // current prediction from predictor;
  btbData currentBTBEntry;
  
  int  byteSize; // the size of btb, here is 2K 4K and 8K. 
  long long int iTotalCount;  // the total number of jump and branch instructers
   
  long long int missHitCount;
  float missHitRate;  // the miss rate hit of this btb
  long long int wrongTargetCount;
  float wrongTargetRate;  // the wrong target rate of this btb
  long long int wrongDirectionCount;
  float wrongDirectionRate;  // the wrong direction rate of this btb

  unsigned int ctoll(char s[]);  // type change
 
public:
  int entryNum;
  int bitNum;
 

  btbData GetBTBEntry(unsigned long long); 
  unsigned long long SetBTBEntry(btbData);
  
  unsigned long long* btb;

  int GetSize();
  void SetSize(int size);
 
  int GetCurrentPrediction();
  void SetCurrentPrediction(int value);


  long long int GetiTotalCount(); 
  long long int GetMissHitCount();
  float GetMissHitRate();
  long long int GetWrongTargetCount();
  float GetWrongTargetRate();
  long long int GetWrongDirectionCount();
  float GetWrongDirectionRate();

  
  // to create the predictor talbe and initilize it.
  bool create();

  // the main simulation function
  bool stepSimulation(char *, char *, int,int);


  btBuffer();
  ~btBuffer();
};

#endif /* BTBUFFER_H_ */
