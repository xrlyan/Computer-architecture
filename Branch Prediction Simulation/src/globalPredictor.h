
#ifndef GLOBALPREDICTOR_H_
#define GLOBALPREDICTOR_H_

#include "saturaPredictor.h"

//derived class based on saturaPredictor
//add globalHTindex and redefine the function of stepSimulation

class globalPredictor : public saturaPredictor
{
 private:
 
public:
  int  globalHTindex;
  bool stepSimulation(int);


  globalPredictor();
  virtual ~globalPredictor();
};

#endif /* GLOBALPREDICTOR_H_ */
