
#ifndef combinePredictor_H_
#define combinePredictor_H_

#include "saturaPredictor.h"
#include "globalPredictor.h"
#include "gsharePredictor.h"


class combinePredictor : public saturaPredictor
{
 private:
  class saturaPredictor saPredictor;
  class globalPredictor glPredictor;
  class gsharePredictor gsPredictor;

  bool predictionProcess(int,int,int,int);
 
public:
  int combineMode; // two combine two.
  bool create();
  bool stepSimulation(char *, int);
  

  combinePredictor();
  virtual ~combinePredictor();
};

#endif /* combinePredictor_H_ */
