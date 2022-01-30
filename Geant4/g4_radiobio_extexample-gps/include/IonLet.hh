#ifndef IonLet_HH
#define IonLet_HH

#include <valarray>
#include "globals.hh"


// class to save and hold data for LET of different ions
class IonLet
{
public:

    // Constructor wants ion data, trackID and total voxel number
    // trackID used only to see if particle is primary
    IonLet(G4int trackID, G4int PDG, G4String fullname, G4String name, G4int Z, G4int A, G4int voxNumber);
    ~IonLet();

    // Alias for matrix type
    using array_type = std::valarray<G4double>;

    G4bool isPrimary() const     {return fIsPrimary; }
    G4int GetPDGencoding() const {return fPDGencoding; }
    G4String GetFullName() const {return fFullName; }
    G4String GetName() const     {return fName; }
    G4int GetZ() const           {return fZ; }
    G4int GetA() const           {return fA; }

    // Track and Dose LET numerator and denominator
    array_type GetLETDN() const      {return fLETDN; }
    array_type GetLETDD() const      {return fLETDD; }
    array_type GetLETTN() const      {return fLETTN; }
    array_type GetLETTD() const      {return fLETTD; }

    // Final Dose and Track LET for the ion
    array_type GetLETD() const       {return fLETD; }
    array_type GetLETT() const       {return fLETT; }

    void SetLETDN(array_type LETDN)  {fLETDN = LETDN; }
    void SetLETDD(array_type LETDD)  {fLETDD = LETDD; }
    void SetLETTN(array_type LETTN)  {fLETTN = LETTN; }
    void SetLETTD(array_type LETTD)  {fLETTD = LETTD; }


    // To update data inside this IonLet
    void update(G4int voxel, G4double DE, G4double DEELETrons, G4double Lsn, G4double DX);

    // To merge data from another IonLet object inside this one
    void merge(const IonLet* lhs);

    // To calculate LET given the numerator and denominator
    void calculate();


    // To sort by the mass number, else sort by the atomic one.
    G4bool operator<(const IonLet& a) const{return (this->fZ == a.fZ) ? this-> fA < a.fA : this->fZ < a.fZ ;}

private:
    G4bool      fIsPrimary;     // True if particle is primary
    G4int       fPDGencoding;   // Particle data group id for the particle
    G4String    fFullName;      // AZ[excitation energy]: like He3[1277.4], He4[0.0], Li7[231.4], ...
    G4String    fName;          // simple name without excitation energy: He3, He4, Li7, ...
    G4int       fZ;             // atomic number
    G4int       fA;             // mass number

    array_type fLETDN , fLETDD, fLETTN , fLETTD; // Track averaged LET and Dose averaged LET
    array_type fLETD, fLETT;
};

#endif //IonLet_HH
