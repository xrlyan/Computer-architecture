
#ifndef gsharePredictor_H_
#define gsharePredictor_H_

#include "saturaPredictor.h"

// this class is based on saturaPredictor
// add the globalHTindex and redefine the execute function
class gsharePredictor : public saturaPredictor
{
 private:
 
public:
  int globalHTindex;  // global history table

  // the main simulating function
  bool stepSimulation(char *, int);

  gsharePredictor();
  virtual ~gsharePredictor();
};

#endif /* gsharePredictor_H_ */
