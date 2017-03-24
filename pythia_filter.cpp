
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

#define LOGURU_IMPLEMENTATION 1
#include "loguru.h"

#include "PythiaTrack.h"
#include "TMCParticle.h"


using namespace std;
TFile * tFile    = nullptr;
TTree * tree     = NULL;
TPythia6 *pythia = NULL;
int pidFilter    = 0;
int nParticles   = 0;

TClonesArray *pyTracks = NULL;

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

void setupPythia( string output_path, int trigger, int iseed = 0 ){
	LOG_SCOPE_FUNCTION( INFO );

	pythia = new TPythia6();

	const int MSEL_MINBIAS = 1;
	const int MSEL_CCBAR_TRIG = 4;

	if ( MSEL_MINBIAS != trigger && MSEL_CCBAR_TRIG != trigger ){
		LOG_F( ERROR, "WARNING: trigger must be (1=mb) or (4=ccbar), got %d", trigger );
	}

	LOG_F( INFO, "pythia->SetMSEL(%d);", trigger );
	pythia->SetMSEL(trigger);

	// Tune from JOEY BUTTERWORTH
	pythia->SetPARP(91,1.0); //<kT>
	pythia->SetPARP(67,1.0);  //mstp*4

	LOG_F( INFO, "pythia->SetMRPY(1, %d); \\ rndSeed=%d", iseed, iseed );
	LOG_F( INFO, "pythia->Initialize(\"CMS\",\"p\",\"p\",200);" );

	pythia->SetMRPY(1, iseed); 
	pythia->Initialize("CMS","p","p",200);


	pythia->Pylist(12);
	pythia->Pystat(1);
	pythia->Pystat(4);
	pythia->Pystat(5);


	string name = output_path + to_string( iseed ) +".root" ;
	LOG_F( INFO, "writing to: %s", name.c_str() );

	tFile = new TFile( name.c_str(), "RECREATE" );
	tFile->cd();
}

void genEvents( ULong_t _nEvents ){

	ULong_t iEvent = 0;
	while ( iEvent < _nEvents ){

		nParticles = 0;
		pythia->GenerateEvent();
		nParticles = pythia->GetNumberOfParticles();

		TObjArray *particles = pythia->GetListOfParticles();

		pyTracks->Clear();
		
		int nMuons = 0;
		for ( int iTrack = 0; iTrack < nParticles; iTrack++ ){
			TMCParticle * plc = (TMCParticle*)particles->At( iTrack );
			addTrack( plc, iTrack );
			if ( abs( plc->GetKF() ) == pidFilter || 0 == pidFilter ) {
				nMuons++;
			}
		} // loop tracks

		
		if ( iEvent % 1000 == 0 )
			cout << "." << std::flush;

		iEvent++;

		if ( nMuons > 1 )
			tree->Fill();
	}
	cout << endl;
}



Int_t main( Int_t argc, Char_t **argv){
	cout << "argc = " << argc << endl;

	if ( argc < 5 ) {
		cout << "USAGE\n GENERATOR trig(1=mb,4=ccbar) output_path nEvents rndSeed filterPID( 13=mu+/- ) (OPTIONAL) -v[LOG_LEVEL]" << endl;
		return 1;
	}

	loguru::init(argc, argv);
	// loguru::g_stderr_verbosity = loguru::Verbosity_OFF;
	// loguru::add_file("debug.log", loguru::Truncate, loguru::Verbosity_MAX);

	int trigger = atoi( argv[1] );
	string output_path = string( argv[2] );
	int nEvents = atoi( argv[3] );
	int seed = atoi( argv[4] );

	if ( argc >= 6 ){
		pidFilter = atoi( argv[5] );
		LOG_F( INFO, "PID Filter = %d", pidFilter );
	}

	LOG_F( INFO, "trigger=%d, nEvents=%d, seed=%d", trigger, nEvents, seed  );
	LOG_F( INFO, "output_path=%s", output_path.c_str() );
	
	setupPythia( output_path, trigger, seed );

	tree 		= new TTree( "pythia", "Pythia pp 200 GeV", 99 );
	pyTracks 	= new TClonesArray( "PythiaTrack" );
	tree->Branch( "Tracks", &pyTracks, 256000, 99 );


	genEvents( nEvents );

	cout<<"::::::::Printing out stats:::::::"<<endl;
	pythia->Pystat(1);
	pythia->Pystat(4);
	pythia->Pystat(5);

	tree->Write();
	tFile->Close();

	return 0;
}