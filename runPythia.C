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


TPythia6 *pythia6 = NULL;
TTree * tree = NULL;
TClonesArray *pyTracks = NULL;

void setupPythia( int iseed = 0 ){
	 pythia6 = new TPythia6();

	 // high pt QCD processes
	 const int MSEL_MINBIAS = 1;
	 const int MSEL_CCBAR_TRIG = 4;
	 pythia6->SetMSEL(MSEL_MINBIAS);

	 // STAR Tune with modified primordial <kt>
    // pythia6->SetMSTP(51,7);
    // pythia6->SetMSTP(82,4);
    // pythia6->SetPARP(82,2.0);
    // pythia6->SetPARP(83,0.5);
    // pythia6->SetPARP(84,0.4);
    // pythia6->SetPARP(85,0.9);
    // pythia6->SetPARP(86,0.95);
    // pythia6->SetPARP(89,1800);
    // pythia6->SetPARP(90,0.25);
    // pythia6->SetPARP(91,0.3);// primordial <kt> distribution width (based on gaussian)
    // pythia6->SetPARP(67,1.0);//mstp32*4 high pT tuned parameter

	// Tune from JOEY BUTTERWORTH
	pythia->SetPARP(91,1.0); //<kT>
	pythia->SetPARP(67,1.0);  //mstp*4

	// TURN ON relavent ccbar decays
	for(int i=684; i<=735; i++) pythia->SetMDME(i,1,0); //D+
	for(int i=755; i<=807; i++) pythia->SetMDME(i,1,0); //D0
	pythia->SetMDME(818,1,0); //Ds
	for(int i=824; i<=850; i++) pythia->SetMDME(i,1,0); //Ds
	for(int i=857; i<=862; i++) pythia->SetMDME(i,1,0); //eta_c,J/psi,chi_2c
	for(int i=1097; i<=1165; i++) pythia->SetMDME(i,1,0); //Lc


    TRandom3 rndm;
    rndm.SetSeed( iseed );
    // int rSeed = rndm.Integer( 100000000 );
    pythia6->SetMRPY(1, iseed); 
	pythia6->Initialize("CMS","p","p",200);

	pythia->Pylist(12);
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

void runPythia( string out_file = "test.root", int n_events = 10000, int seed = 2, int seedStart = 0 ) {


	gSystem->Load("libPhysics");
	gSystem->Load("libEG.so");
	gSystem->Load("libEGPythia6.so");
	gSystem->Load( "PythiaTrack_h.so" );



	setupPythia( seed + seedStart );

	TFile * fout = new TFile( out_file.c_str(), "RECREATE" );
	makeTree();


	int nevent = 0;
	for(int i = 1; i<=n_events; i++ ){
		pythia6->GenerateEvent();	

		if ( i % 1000 == 0 )
			cout << "." << std::flush;
		TObjArray *particles = pythia6->GetListOfParticles();

		pyTracks->Clear();
		// vector<int> pos, neg;
		bool hasMuon = false;
		int nTracks = particles->GetEntries();
		for ( int iTrack = 0; iTrack < nTracks; iTrack++ ){
			TMCParticle * plc = (TMCParticle*)particles->At( iTrack );
			addTrack( plc, iTrack );
			if ( abs( plc->GetKF() ) == 13 ) {
				hasMuon = true;
				// cout << "Muon with parent = " << plc->GetParent() << endl;
			}

			// if ( plc->GetKF() == 13 ) neg.push_back( iTrack );
			// if ( plc->GetKF() == -13 ) pos.push_back( iTrack );

		}

		// for ( int iPos = 0; iPos < pos.size(); iPos++ ){
		// 	for ( int iNeg = 0; iNeg < neg.size(); iNeg++ ){
		// 		TMCParticle * pPos = (TMCParticle*)particles->At( pos[iPos] );
		// 		TMCParticle * pNeg = (TMCParticle*)particles->At( neg[iNeg] );

		// 		if ( pPos->GetParent() == pNeg->GetParent() && pNeg->GetParent() > 0 ){

		// 			cout << "parent = " << pPos->GetParent() << endl ;
		// 			TMCParticle * parent = (TMCParticle*)particles->At( pNeg->GetParent() - 1 );
		// 			// if ( parent->GetKF() == 221 ) continue;

		// 			cout << "parent kf = " << parent->GetKF() << endl;
		// 			cout << "children = " << parent->GetFirstChild() << " -> " << parent->GetLastChild() << endl;
		// 			cout << "pos = " << pos[iPos] << ", neg = " << neg[iNeg] << endl;
		// 			cout << "pos kf = " << pPos->GetKF() << endl;

		// 			TLorentzVector lv1, lv2, lv, lvp;
		// 			TVector3 mom1( pPos->GetPx(), pPos->GetPy(), pPos->GetPz() );
		// 			TVector3 mom2( pNeg->GetPx(), pNeg->GetPy(), pNeg->GetPz() );

		// 			lv1.SetPtEtaPhiM( mom1.Perp(), mom1.Eta(), mom1.Phi(), pPos->GetMass() );
		// 			lv2.SetPtEtaPhiM( mom2.Perp(), mom2.Eta(), mom2.Phi(), pNeg->GetMass() );
		// 			lv = lv1 + lv2;

		// 			cout << "parent.mMass = " << parent->GetMass() << " vs. " << lv.M() << endl;

		// 		}
				
		// 	}
		// }


		if ( hasMuon )
			tree->Fill();
	}


	tree->Write();
	cout << "Closing " << out_file << endl;
	fout->Close();

}