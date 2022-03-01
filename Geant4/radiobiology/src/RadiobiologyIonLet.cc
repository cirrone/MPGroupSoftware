#include "RadiobiologyIonLet.hh"

RadiobiologyIonLet::RadiobiologyIonLet(G4int trackID, G4int PDG, G4String fullname, G4String name, G4int Z, G4int A, G4int voxNumber):
    fIsPrimary(trackID ==1), fPDGencoding(PDG), fFullName(fullname), fName(name),
    fZ(Z), fA(A)
{
    fLETDN = array_type(0.0, voxNumber);
    fLETDD = array_type(0.0, voxNumber);
    fLETTN = array_type(0.0, voxNumber);
    fLETTD = array_type(0.0, voxNumber);

    fLETD = array_type(0.0, voxNumber);
    fLETT = array_type(0.0, voxNumber);
}

RadiobiologyIonLet::~RadiobiologyIonLet()
{

}

void RadiobiologyIonLet::update(G4int voxel, G4double DE, G4double DEELETrons, G4double Lsn, G4double DX)
{
    fLETDN[voxel] += (DE + DEELETrons)* Lsn; // ions dose LET Numerator, including secondary electrons energy deposit
    fLETDD[voxel] += DE + DEELETrons;        // ions dose LET Denominator, including secondary electrons energy deposit
    fLETTN[voxel] += DX * Lsn;                // ions track LET Numerator
    fLETTD[voxel] += DX;                     // ions track LET Denominator
}

void RadiobiologyIonLet::merge(const RadiobiologyIonLet* rhs)
{
    // If programmed correctly, this exception should never appear
    if (rhs->GetPDGencoding() != fPDGencoding ||
            rhs->isPrimary() != fIsPrimary)
        G4Exception("IonLet::merge", "mergingdifferentions", FatalException, "Cannotmerge ions, probably merging of data from different ions");

    fLETDN += rhs->GetLETDN();
    fLETDD += rhs->GetLETDD();
    fLETTN += rhs->GetLETTN();
    fLETTD += rhs->GetLETTD();
}

void RadiobiologyIonLet::calculate()
{
    for(unsigned int  v=0; v < fLETD.size(); v++)
    {

        if (fLETDD[v] >0.) fLETD[v] = fLETDN[v] / fLETDD[v];
        if (fLETTD[v] >0.) fLETT[v] = fLETTN[v] / fLETTD[v];
    }
}
