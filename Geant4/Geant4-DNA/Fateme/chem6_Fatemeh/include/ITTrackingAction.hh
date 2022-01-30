#ifndef ITTRACKINGACTION_HH
#define ITTRACKINGACTION_HH

#include "G4UserTrackingAction.hh"

class ITTrackingAction: public G4UserTrackingAction
{
public:
    ITTrackingAction();
    virtual ~ITTrackingAction();
    virtual void PreUserTrackingAction(const G4Track*);
    virtual void PostUserTrackingAction(const G4Track*);

};

#endif // ITTRACKINGACTION_HH
