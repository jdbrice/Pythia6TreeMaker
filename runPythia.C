#include "PythiaTrack.h"

#include "TROOT.h"
#include "TFile.h"
#include "TF1.h"
#include "TH1.h"
#include "TH2.h"
#include "TChain.h"
#include "TSystem.h"
#include "TMath.h"
#include "TTree.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include "TObjArray.h"
#include "TBenchmark.h"
#include "TRandom.h"
#include "TRandom3.h"
#include <iostream>
#include <vector>
#include <string>

#include "TRandom3.h"
#include "TClonesArray.h"
#include "TPythia6.h"
#include "TMCParticle.h"

TPythia6 *pythia6 = nullptr;
TTree * tree = nullptr;
TClonesArray *pyTracks = nullptr;

void setupPythia( int iseed = 0 ){
	 pythia6 = new TPythia6();

	 // high pt QCD processes
	 pythia6->SetMSEL(1);

	 // STAR Tune with modified primordial <kt>
    pythia6->SetMSTP(51,7);
    pythia6->SetMSTP(82,4);
    pythia6->SetPARP(82,2.0);
    pythia6->SetPARP(83,0.5);
    pythia6->SetPARP(84,0.4);
    pythia6->SetPARP(85,0.9);
    pythia6->SetPARP(86,0.95);
    pythia6->SetPARP(89,1800);
    pythia6->SetPARP(90,0.25);
    pythia6->SetPARP(91,0.3);// primordial <kt> distribution width (based on gaussian)
    pythia6->SetPARP(67,1.0);//mstp32*4 high pT tuned parameter

    TRandom3 rndm;
    rndm.SetSeed( 0 );
    int rSeed = rndm.Integer( 100000000 );
    pythia6->SetMRPY(1, iseed); 
	pythia6->Initialize("CMS","p","p",200);

	pythia6->Pystat(1);
	pythia6->Pystat(4);
	pythia6->Pystat(5);
}


void makeTree() {

	tree = new TTree( "pythia", "Pythia pp 200 GeV", 99 );
	pyTracks = new TClonesArray( "PythiaTrack" );
	tree->Branch( "Tracks", &pyTracks, 256000, 99 );

}

void addTrack( TMCParticle * plc, int iTrack ){

	PythiaTrack * ptr = new ((*pyTracks)[iTrack]) PythiaTrack( );

	ptr->mId       = iTrack;
	ptr->mEnergy   = plc->GetEnergy();
	ptr->mKF       = plc->GetKF();
	ptr->mKS       = plc->GetKS();
	ptr->mLifetime = plc->GetLifetime();
	ptr->mMass     = plc->GetMass();
	ptr->mParent   = plc->GetParent();
	ptr->mPx       = plc->GetPx();
	ptr->mPy       = plc->GetPy();
	ptr->mPz       = plc->GetPz();
	ptr->mTime     = plc->GetTime();
	ptr->mVx       = plc->GetVx();
	ptr->mVy       = plc->GetVy();
	ptr->mVz       = plc->GetVz();


}

void runPythia( string out_file = "out.root", int n_events = 1000, int seed = 0 ) {

	gSystem->Load("libPhysics");
	gSystem->Load("libEG.so");
	gSystem->Load("libEGPythia6.so");
	gSystem->Load( "PythiaTrack_h.so" );


	setupPythia( seed );

	TFile * fout = new TFile( out_file.c_str(), "RECREATE" );
	makeTree();


	int nevent = 0;
	for(int i = 1; i<=n_events; i++ ){
		pythia6->GenerateEvent();	

		if ( i % 1000 == 0 )
			cout << "." << std::flush;
		TObjArray *particles = pythia6->GetListOfParticles();

		pyTracks->Clear();
		
		int nTracks = particles->GetEntries();
		for ( int iTrack = 0; iTrack < nTracks; iTrack++ ){
			addTrack( (TMCParticle*)particles->At( iTrack ), iTrack );
		}
		tree->Fill();
	}


	tree->Write();
	cout << "Closing " << out_file << endl;
	fout->Close();

}