
#include "TPythia6.h"
#include "TTree.h"
#include "TClonesArray.h"
#include "TRandom3.h"
#include "TNtuple.h"
#include "TFile.h"
#include "TLorentzVector.h"

// STL
#include <iostream>
#include <string> 

// #include <cstdio>
// #include <iostream>
// #include <fstream>
// #include <vector>

// #include "TMath.h"
// #include "TString.h"
// #include "TH1.h"
// #include "TH2.h"
// #include "TFile.h"
// #include "TCanvas.h"
// #include "TStyle.h"
// #include "TVector3.h"
// #include "TTree.h"
// #include "TGraphErrors.h"
// #include "TF1.h"
// #include "TH2.h"
// #include "TLorentzVector.h"
// #include "TH1.h"
// #include "TList.h"
// #include "TMultiGraph.h"

// 
// #include "TParticle.h"
// #include "TParticlePDG.h"


using namespace std;


TPythia6 *pythia = NULL;
TTree * tree = NULL;
TClonesArray *pyTracks = NULL;
TFile * tFile = nullptr;
TNtuple * ntuple = nullptr;
int nParticles = 0;

#define PID_c 4
#define PID_mu 13
#define PID_string 92

#define MASS_mu 0.1056583745 // GeV/c^2

#define K_ID 2
#define K_PARENT 3

void setupPythia( int trigger, int iseed = 0 ){
	pythia = new TPythia6();

	const int MSEL_MINBIAS = 1;
	const int MSEL_CCBAR_TRIG = 4;
	if ( MSEL_MINBIAS != trigger && MSEL_CCBAR_TRIG != trigger ){
		cout << "WARNING: trigger must be (1=mb) or (4=ccbar), got " << trigger << endl;
	}

	pythia->SetMSEL(trigger);

	// Tune from JOEY BUTTERWORTH
	pythia->SetPARP(91,1.0); //<kT>
	pythia->SetPARP(67,1.0);  //mstp*4


	// // TURN ON relavent ccbar decays
	for(int i=673; i<=683; i++) pythia->SetMDME(i,1,0); //D+ ->e
	for(int i=684; i<=694; i++) pythia->SetMDME(i,1,1); //D+ -> mu
	for(int i=695; i<=735; i++) pythia->SetMDME(i,1,0); //D+ -> other

	// 	// D0
	for(int i=747; i<=754; i++) pythia->SetMDME(i,1,0); //D0 -> e
	for(int i=755; i<=762; i++) pythia->SetMDME(i,1,1); //D0 -> mu
	for(int i=763; i<=807; i++) pythia->SetMDME(i,1,0); //D0 -> other

	for(int i=818; i<=823; i++) pythia->SetMDME(i,1,0); //Ds -> e
	for(int i=824; i<=828; i++) pythia->SetMDME(i,1,1); //Ds -> mu
	for(int i=829; i<=850; i++) pythia->SetMDME(i,1,0); //Ds -> other

	for(int i=857; i<=862; i++) pythia->SetMDME(i,1,0); //eta_c,J/psi,chi_2c
	




	// for(int i=755; i<=807; i++) pythia->SetMDME(i,1,0); //D0
	// pythia->SetMDME(818,1,0); //Ds
	// for(int i=824; i<=850; i++) pythia->SetMDME(i,1,0); //Ds
	// for(int i=857; i<=862; i++) pythia->SetMDME(i,1,0); //eta_c,J/psi,chi_2c
	// for(int i=1097; i<=1165; i++) pythia->SetMDME(i,1,0); //Lc
	// 
	// 
	
	// TURN OFF non mu decay channels
	// for ( int i = 673; i <= 683; i++ ) pythia->SetMDME(i,1,0); //D+->e
	// for ( int i = 695; i <= 735; i++ ) pythia->SetMDME(i,1,0); //D+-> other



	TRandom3 rndm;
	rndm.SetSeed( iseed );
	pythia->SetMRPY(1, iseed); 
	pythia->Initialize("CMS","p","p",200);

	pythia->Pylist(12);
	pythia->Pystat(1);
	pythia->Pystat(4);
	pythia->Pystat(5);


	string name = "/data/star1/jdb12/dilepton/Run15_PP_200_Mtd/ccbar/pythia_ccbar_" + to_string( iseed ) +".root" ;
	tFile = new TFile( name.c_str(), "RECREATE" );
	tFile->cd();
	ntuple = new TNtuple("ccbar_mumu", "ccbar to mumu","nPt:nEta:nPhi:nMass:nParentId:pPt:pEta:pPhi:pMass:pParentId:pairPt:pairEta:pairPhi:pairMass:pairY");
}

void printPlc( int i ){
	cout << "K(1)=" << pythia->GetK(i, 1) << ", K(2)=" << pythia->GetK(i, 2) << ", K(3)=" << pythia->GetK(i, 3) << ", K(4)=" << pythia->GetK(i, 4) << ", K(5)=" << pythia->GetK(i, 5) << endl;
}

int state( int i ){
	if ( i>0 && i<= nParticles )
		return pythia->GetK( i, 1 );
	return -1;
}
int plcId( int i ){
	if ( i>0 && i<= nParticles )
		return pythia->GetK( i, 2 );
	return -1;
}
int parentIndex( int i ){
	if ( i>0 && i<= nParticles )
		return pythia->GetK( i, 3 );
	return -1;
}
int posX( int i ){
	if ( i>0 && i<= nParticles )
		return pythia->GetK( i, 4 );
	return -1;
}
int posY( int i ){
	if ( i>0 && i<= nParticles )
		return pythia->GetK( i, 5 );
	return -1;
}
TLorentzVector lvec( int i ){
	TLorentzVector lv;
	if ( i>0 && i<= nParticles ){
		lv.SetPxPyPzE(  pythia->GetP(i,1),
						pythia->GetP(i,2),
						pythia->GetP(i,3),
						pythia->GetP(i,4));
	}
	return lv;
}



int findStrings(){
	int nStrings = 0;
	for(Int_t i = 0; i < nParticles; i++){
		int id = abs(pythia->GetK(i+1, K_ID ) );
		if(id == PID_string ){  
			int parentIdIndex = pythia->GetK( i+1, K_PARENT);
			
			if(abs(pythia->GetK(parentIdIndex,2)) == PID_c){
				nStrings++;
				if ( nStrings >= 2 ) break;
			}//charm		
		}//string
	}
	return nStrings;
}

bool isMuon( int i ){

	if ( abs( plcId( i ) ) == PID_mu ){
		

		if ( (posX(i) != 0 || posY(i) != 0) && state( i ) != 1 ){
			cout << "ERROR state pos mismatch" << endl;
		}

		int pIndex = parentIndex( i );
		if ( pIndex >= 1 ){
			// printPlc( pIndex );

			int parentId = abs(plcId( pIndex ));
			if(parentId==411||parentId==421||parentId==431||parentId==4122){
				// printPlc( i );
				// cout << "\tPARENT: "; printPlc( pIndex );
				return true;
			}

		} else {
			cout << "\t reject no parent" << endl;
		}
	}
	return false;
}

void findMuons(){
	
	TLorentzVector plv, nlv, lv;
	int pParentId = -1, nParentId = -1;
	bool foundPos = false; 
	bool foundNeg = false;

	for(Int_t i = 0; i < nParticles; i++){
		int iPlc = i+1;
		bool isMu = isMuon( iPlc );
		int pId = plcId( iPlc );
		if (  isMu && pId == PID_mu ){
			// set pos
			nlv = lvec( iPlc );
			nParentId = parentIndex( iPlc );
			foundNeg = true;
		} else if ( isMu && pId == -PID_mu ){
			plv = lvec( iPlc );
			pParentId = parentIndex( iPlc );
			foundPos = true;
		}

		if ( foundPos && foundNeg ) break;

	}

	if ( foundPos && foundNeg ){
		lv = plv + nlv;

		float data[15] = {
				(float)nlv.Pt(), (float)nlv.Eta(), (float)nlv.Phi(), (float)nlv.M(), (float)nParentId,
				(float)plv.Pt(), (float)plv.Eta(), (float)plv.Phi(), (float)plv.M(), (float)pParentId,
				(float)lv.Pt(), (float)lv.Eta(), (float)lv.Phi(), (float)lv.M(), (float)lv.Rapidity()
		};
		ntuple->Fill( data );
		// ntuple->Fill(
		// 		(float)nlv.Pt(), (float)nlv.Eta(), (float)nlv.Phi(), (float)nlv.M(), (float)nParentId,
		// 		(float)plv.Pt(), (float)plv.Eta(), (float)plv.Phi(), (float)plv.M(), (float)pParentId,
		// 		(float)lv.Pt(), (float)lv.Eta(), (float)lv.Phi(), (float)lv.M(), (float)lv.Rapidity()
		// 	);
	}
}


void genEvents( ULong_t _nEvents ){

	ULong_t iEvent = 0;
	while ( iEvent < _nEvents ){

		pythia->GenerateEvent();
		nParticles = pythia->GetNumberOfParticles();

		// cout << "nString = " <<  << endl;
		int nStrings = findStrings();
		if ( nStrings >= 2 )
			findMuons( );

		if ( iEvent % 1000 == 0 )
			cout << "." << std::flush;

		iEvent++;
	}
	cout << endl;

}


// NOTE: at first i thought it looked wrong to assume only 1 ccbar -> mu mu but it is so rare that this is fine

Int_t main( Int_t argc, Char_t **argv){
	cout << "argc = " << argc << endl;
	if ( argc < 4 ) {
		cout  << "USAGE\n GENERATOR trig(1=mb,4=ccbar) nEvents rndSeed" << endl;
		return 1;
	}

	int trigger = atoi( argv[1] );
	int nEvents = atoi( argv[2] );
	int seed = atoi( argv[3] );
	setupPythia( trigger, seed );
	genEvents( nEvents );

	cout<<"::::::::Printing out stats:::::::"<<endl;
	pythia->Pystat(1);
	pythia->Pystat(4);
	pythia->Pystat(5);

	cout << "SAVE THEN END" << endl;
	ntuple->Write();
	tFile->Write();
	tFile->Close();
	delete tFile;
	cout << "END" << endl;



	return 0;
}