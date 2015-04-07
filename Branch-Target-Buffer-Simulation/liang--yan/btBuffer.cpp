#include "btBuffer.h"

btBuffer::btBuffer()
{
  iTotalCount = 0;
  missHitCount = 0;
  wrongTargetCount = 0;
  wrongDirectionCount = 0;
  byteSize=2048;
}
// destructor function
btBuffer::~btBuffer()
{
  if(btb!=NULL)
   delete[] btb;
}

// size of btb
void btBuffer::SetSize(int size)
{
  byteSize = size;
}
int btBuffer::GetSize()
{
  return byteSize;
}

// set current prediction from tournament predictor
void btBuffer::SetCurrentPrediction(int value)
{
  currentPrediction = value;
}
// get current prediction from tournament predictor
int btBuffer::GetCurrentPrediction()
{
  return currentPrediction;
}

// the total count of jump and branch instructions
long long int btBuffer::GetiTotalCount()
{
  return iTotalCount;
}
long long int btBuffer::GetMissHitCount()
{
  return missHitCount;
}
float btBuffer::GetMissHitRate()
{
  missHitRate = (missHitCount)/(iTotalCount*1.0);
  return missHitRate;
}
long long int btBuffer::GetWrongTargetCount()
{
  return wrongTargetCount;
}
float btBuffer::GetWrongTargetRate()
{
  wrongTargetRate = (wrongTargetCount)/(iTotalCount*1.0);
  return wrongTargetRate;
}
long long int btBuffer::GetWrongDirectionCount()
{
  return wrongDirectionCount;
}
float btBuffer::GetWrongDirectionRate()
{
  wrongDirectionRate = (wrongDirectionCount)/(iTotalCount*1.0);
  return wrongDirectionRate;
}

/*-----
update the value of entry line in btb
entry line structure:
0000 0000 0000 0000 0000 0000 0000 0000
   tag (22,23,24 bits)             |->

0000 0000 0000 0000 0000 0000 0000     000         0
32bit for target address       ->|     type    valid
------*/

unsigned long long  btBuffer::SetBTBEntry(btbData btbEntry)
{
  unsigned long long btbLine = 0;
  // add pc tag to btb entry line
  btbLine += btbEntry.pcTag;
  btbLine = btbLine << 36;
  
  // add target address to btb entry line
  unsigned long long temp = 0;
  temp += btbEntry.targetAddress;
  temp = temp << 4;
  btbLine += temp;

  // add type
  temp=0;
  temp += btbEntry.type;
  temp = temp << 1;
  btbLine += temp;

  //add valid 
  btbLine += btbEntry.valid;
  
  return btbLine;
}

/*-----
get the value from the btb entry line
entry line structure:
0000 0000 0000 0000 0000 0000 0000 0000
   tag (22,23,24 bits)             |->

0000 0000 0000 0000 0000 0000 0000     000         0
32bit for target address       ->|     type    valid
------*/
btbData btBuffer::GetBTBEntry(unsigned long long btbLine)
{

  btbData btbEntry;
  unsigned long long temp = btbLine;
  
  //get the valid
  btbEntry.valid= temp & 0x1; // get the last 1 bit for 
  
  //get the type
  temp=btbLine;
  temp = temp >> 1 ;
  btbEntry.type=temp & (0x7);

  //get the target address
  temp=btbLine;
  temp = temp >> 4 ;
  btbEntry.targetAddress=temp & (0xFFFFFFFF);

  // get the tag 
  temp=btbLine;
  btbEntry.pcTag = temp >> 36;
 
  return btbEntry;
}

// construct the branch target buffer
bool btBuffer::create()
{
  // entryNum is based on the size of BTB
  entryNum = byteSize >> 3;
  bitNum  = int(log2(entryNum)) ;
  btb = new unsigned long long[entryNum];
  for(int i=0; i< entryNum ; i++)
   {
     btb[i] = 0;
   }
  return 0;
}

bool btBuffer::stepSimulation(char *address, char *targetAddress, int branchTaken, int type)
{
  iTotalCount++;

  unsigned int PCAddress = ctoll(address);
  unsigned int target = ctoll(targetAddress);

  PCAddress = PCAddress >> 2;// we do not use the first two bits.
  unsigned int temp = PCAddress;

  int index = temp & ((0x1 << bitNum)-1);
  int tag = PCAddress >> bitNum;
  
  currentBTBEntry = GetBTBEntry(btb[index]);  
  btbData realEntry;
  realEntry.valid = 1;
  realEntry.type = type;
  realEntry.targetAddress = target;
  realEntry.pcTag = tag;  
  
  if(!currentBTBEntry.valid){
    btb[index] = SetBTBEntry(realEntry);
  }else{    
    // miss hit in BTB
    if((type == 2 && currentBTBEntry.pcTag != realEntry.pcTag)|| 
       (type == 1 && branchTaken == 1 && currentPrediction ==1 && currentBTBEntry.pcTag != realEntry.pcTag))
          missHitCount++; 

    // wrong target address
    if((type == 2 && currentBTBEntry.pcTag == realEntry.pcTag)
       || (type == 1 && currentPrediction ==1 && branchTaken == 1 && currentBTBEntry.pcTag == realEntry.pcTag) )
      if(currentBTBEntry.targetAddress != target)
	wrongTargetCount++;

    // wrong direction 
    if(type == 1 && currentBTBEntry.targetAddress == target && currentBTBEntry.pcTag == realEntry.pcTag)
      if(currentPrediction == 0 && branchTaken == 1)
	wrongDirectionCount++;
    
    // update the btb entry line
    if(type == 2 || (type == 1 && branchTaken == 1))
      btb[index] = SetBTBEntry(realEntry);     
  }
  return 0;
}

// to convert a char[] to a long long int
// to get the integer of instructure address
unsigned int btBuffer::ctoll(char s[]) 
{ 
    unsigned int n = 0; 
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
