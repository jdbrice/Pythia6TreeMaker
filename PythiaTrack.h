#ifndef PYTHIA_TRACK_H
#define PYTHIA_TRACK_H

#include "TObject.h"

class PythiaTrack : public TObject {
	
public:
	PythiaTrack() {}
	virtual ~PythiaTrack() {}


	Int_t mId;
	Float_t mEnergy;
	Float_t mKF;
	Float_t mKS;
	Float_t mLifetime;
	Float_t mMass;
	Int_t mParent;
	Float_t mPx;
	Float_t mPy;
	Float_t mPz;
	Float_t mTime;
	Float_t mVx;
	Float_t mVy;
	Float_t mVz;

};

#endif