#ifndef CARDS_0
#define CARDS_0

class Card;

#include "basicHeader.hpp"

/* ======================================================== */

class Card
{
protected:

  const std::string name;
    
  const uint16_t cost;
    
  bool isTapped;

public:

  Card(const std::string & name , const uint16_t & cost );
  virtual ~Card() {} 
  
  const std::string getName (void) const { return name; }
  uint16_t getCost(void) const { return cost; }
  
  void setTapped()   { isTapped = true;  }
  void setUnTapped() { isTapped = false; }

  bool checkTapped() const { return isTapped; }

  virtual void print() const = 0;
};

#endif
/* =================| E N D  O F  F I L E |================= */