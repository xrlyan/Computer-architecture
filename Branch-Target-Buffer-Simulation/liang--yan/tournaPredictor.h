
#ifndef tournaPredictor_H_
#define tournaPredictor_H_

#include "saturaPredictor.h"
#include "globalPredictor.h"


// this class is based on saturaPredictor
// add two new predictor class
// redefine the initial function create(), 
//the execute function stepSimulation()
// and the train function predictionProcess() 
class tournaPredictor : public saturaPredictor
{
 private:
  class saturaPredictor saPredictor;
  class globalPredictor glPredictor;
  int currentPrediction;

  bool predictionProcess(int,int,int,int);
 
public:
  bool create();
  bool stepSimulation(char *, int);
  
  int getCurrentPrediction();  

  tournaPredictor();
  virtual ~tournaPredictor();
};

#endif /* tournaPredictor_H_ */
