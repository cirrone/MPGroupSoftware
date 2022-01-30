
#ifndef Run_h
#define Run_h 1


#include "G4Run.hh"
#include "globals.hh"


class Run : public G4Run
{
public:

  Run();
  ~Run();
 
   
  void Energy (G4double);
   
  virtual void Merge(const G4Run*);
  
  void EndOfRun ();

 
 private: 
  G4double Totalenergy;
  
};

#endif

