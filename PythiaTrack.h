#ifndef PYTHIA_TRACK_H
#define PYTHIA_TRACK_H

#include "TObject.h"

class PythiaTrack : public TObject {
	
public:
	PythiaTrack() {
		Int_t   mId = 0.0;
		Float_t mEnergy = 0.0;
		Int_t   mKF = 0.0;
		Int_t   mKS = 0.0;
		Float_t mLifetime = 0.0;
		Float_t mMass = 0.0;
		Int_t   mParent = 0.0;
		Float_t mPx = 0.0;
		Float_t mPy = 0.0;
		Float_t mPz = 0.0;
		Float_t mTime = 0.0;
		Float_t mVx = 0.0;
		Float_t mVy = 0.0;
		Float_t mVz = 0.0;
	}
	virtual ~PythiaTrack() {}


	Int_t mId;
	Float_t mEnergy;
	Int_t mKF;
	Int_t mKS;
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

	ClassDef( PythiaTrack, 2 )
};

#endif