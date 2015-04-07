
#ifndef SATURAPREDICTOR_H_
#define SATURAPREDICTOR_H_

#include <stdio.h>
#include <string>
#include <math.h>

class saturaPredictor
{
 protected:
  int  bitWidth; // the bit width, here is 2 bit.
  int  byteSize; // the size of Precditor, here is 2K 4K and 8K. 
  long long int iTotalCount;  // the total number of branch instructers
  long long int predictFailCount;  // the total number of wrong prediction of branch
  float predictionHitRate;  // the right prediction rate of this predictor

  long long int ctoll(char s[]);  // type change
 
public:
  int entryNum;
 
  int *predictionTable; 

  int GetSize();
  void SetSize(int size);
  
  int GetBitWidth();
  void SetBitWidth(int width);

  long long int GetiTotalCount(); 
  long long int GetpredictFailCount();
  float GetpredictionHitRate();

  bool predictionProcess(int,int);
  
  // to create the predictor talbe and initilize it.
  virtual bool create();

  // the main simulation function
  virtual bool stepSimulation(char *, int);


  saturaPredictor();
  virtual ~saturaPredictor();
};

#endif /* SATURAPREDICTOR_H_ */
