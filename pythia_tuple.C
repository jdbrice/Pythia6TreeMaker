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
//#include "histogram.h"
//#include "TPythia6/TPythia6.h"
//#include "TPythia6/TMCParticle6.h"

#define eMass 0.000511


Double_t momRes(Double_t *x, Double_t *par)
{
	double pt = x[0];
	double m = par[0];
	const Double_t a = par[1];
	const Double_t b = par[2];
	return TMath::Sqrt(a*a*pt*pt+b*b*m*m/pt/pt+b*b);
}



void pythia_tuple(int irun=1, int Nevt=100000, int iseed=789456){
	TStopwatch*   stopWatch = new TStopwatch();
	stopWatch->Start();
	gSystem->Load("libPhysics");
	gSystem->Load("libEG.so");
//    gSystem->Load("/star/u/yfzhang/lbl/mc/pythia/pythia6/libPythia6.so"); // will load the default from the library
	gSystem->Load("libEGPythia6.so");
    //    gSystem->Load("TPythia6/TPythia6.so");
	TH1D *hEventCounter = new TH1D("eventCounter","",10,0.,10.);
    TH2D *hPtYkaon = new TH2D("PtYkaon","",200,0.,20.,100,-5.,5.);
    TH2D *hPtYpion = new TH2D("PtYpion","",200,0.,20.,100,-5.,5.);
    TH2D *hPtYproton = new TH2D("PtYproton","",200,0.,20.,100,-5.,5.);
    TH2D *hPtYd0 = new TH2D("PtYd0","",200,0.,20.,100,-5.,5.);
    TH2D *hPtYdstar = new TH2D("PtYdstar","",200,0.,20.,100,-5.,5.);
    TH2D *hPtYc = new TH2D("PtYc","",200,0.,20.,100,-5.,5.);

    //========================= initial trees ==========================
//    hTree = new TTree("mhTree","hTree");
//    hTree->SetAutoSave(1000000);
//    
//    cout << "Initialize the hTree ... " << endl;
//    
//	hTree->Branch("NFill",&mhTree.NFill,"NFill/I");
//    hTree->Branch("pTK",mhTree.pTK,"pTK[NFill]/F");
//    hTree->Branch("pTDK",mhTree.pTDK,"pTDK[NFill]/F");
//	hTree->Branch("pTPi",mhTree.pTPi,"pTPi[NFill]/F");
//	hTree->Branch("pTDPi",mhTree.pTDPi,"pTDPi[NFill]/F");
	


    //======================== initial parameters ==========================

    // Initialize Pythia
	TPythia6 *myPythia6 = new TPythia6();

    myPythia6->SetMSEL(1); //call PYGIVE('msel = 1')   ! pp min. bias. 
//    myPythia6->SetMSEL(4);//c trigger
/*
    myPythia6->SetMSEL(0);  // PHENIX mb
    myPythia6->SetMSUB(11,1);
    myPythia6->SetMSUB(12,1);
    myPythia6->SetMSUB(13,1);
    myPythia6->SetMSUB(28,1);
    myPythia6->SetMSUB(53,1);
    myPythia6->SetMSUB(68,1);
*/
    //    myPythia6->SetMSEL(5);//b trigger
    //myPythia6->SetMSTP(81,1);//call PYGIVE('mstp(81) = 1') ! multiple interaction on.
    //myPythia6->SetMSTP(82,4);//call PYGIVE('mstp(82) = 4') ! impact parameter choice.
    //myPythia6->SetMSTP(2, 2);//call PYGIVE('mstp(2) = 2') ! 2nd order running of alpha_s.
    //myPythia6->SetMSTP(33,3);//call PYGIVE('mstp(33) = 3') ! K-factors.

    //double gauss distribution
    //PARP(83)=0.2
    //PARP(84)=0.5
    //myPythia6->SetPARP(83,0.2);
    //myPythia6->SetPARP(84,0.5);

    //myPythia6->SetPARP(82,3.2);//call PYGIVE('parp(82) = 3.2') ! PT0 multiple distribution tail.
    //myPythia6->SetCKIN(3,5);//Set the high pT trigger 4GeV/c    

    //tuned parameters: - Xiaoyan Lin
    //    myPythia6->SetMSTJ(11,3);//Peterson FF - don't change these any more
    //    myPythia6->SetPARJ(54,-1e-05);//mstj11 epsilon, make spectrum hard D=-0.05
    //    myPythia6->SetPARJ(55,-1e-05);
/*
    myPythia6->SetMSTP(33,1);//common kfactor stored in parp31
    myPythia6->SetMSTP(32,4);//Q^2 scale D:8
    myPythia6->SetMSTP(51,7);//CTEQ5L PDF
    myPythia6->SetPARP(31,3.5); //D:1.5
    myPythia6->SetPARP(91,1.5);//<kt>
    //    myPythia6->SetPARP(93,10);//maximum pt broadening D:5 - unknown
    myPythia6->SetPMAS(4,1,1.25);//c-quark mass D=1.5
    //    myPythia6->SetPMAS(5,1,4.8);//b-quark mass  D=4.8
    myPythia6->SetPARP(67,4);//mstp32*4 high pT tuned parameter
*/
    // PHENIX tune parameters (but for v6.319, use this first anyway)
/*
    myPythia6->SetPARP(91,1.5);
    myPythia6->SetMSTP(32,4);
    myPythia6->SetCKIN(3,2.);
*/
// STAR Tune with modified primordial <kt>
    myPythia6->SetMSTP(51,7);
    myPythia6->SetMSTP(82,4);
    myPythia6->SetPARP(82,2.0);
    myPythia6->SetPARP(83,0.5);
    myPythia6->SetPARP(84,0.4);
    myPythia6->SetPARP(85,0.9);
    myPythia6->SetPARP(86,0.95);
    myPythia6->SetPARP(89,1800);
    myPythia6->SetPARP(90,0.25);
    myPythia6->SetPARP(91,0.3);// primordial <kt> distribution width (based on gaussian)
    myPythia6->SetPARP(67,1.0);//mstp32*4 high pT tuned parameter
  

	//CDF TuneA
    	
    /*     myPythia6->SetMSTP(51,7);
        myPythia6->SetMSTP(82,4);
        myPythia6->SetPARP(82,2.0);
    	myPythia6->SetPARP(83,0.5);
    	myPythia6->SetPARP(84,0.4);
    	myPythia6->SetPARP(85,0.9);
	myPythia6->SetPARP(86,0.95);
	myPythia6->SetPARP(89,1800);
	myPythia6->SetPARP(90,0.25);
	myPythia6->SetPARP(91,1.0);
	myPythia6->SetPARP(67,4.0);*/
    
	//    myPythia6->SetMSTP(33,1);//common kfactor stored in parp31
	//    myPythia6->SetMSTP(32,8);//Q^2 scale definition D:8
	//    myPythia6->SetMSTP(51,7);//CTEQ5L PDF
	//    myPythia6->SetPARP(31,3.5); //D:1.5
	//    myPythia6->SetPARP(91,1.0);// primordial <kt> distribution width (based on gaussian)
	//    myPythia6->SetPARP(67,1.0);//mstp32*4 high pT tuned parameter
	
    //====================== particle decay mode ==========================
	
    //switch off non-electron decays
    //    for(int i=684; i<=735; i++) myPythia6->SetMDME(i,1,0); //D+
    //for(int i=755; i<=807; i++) myPythia6->SetMDME(i,1,0); //D0
    //myPythia6->SetMDME(818,1,0); //Ds
    //for(int i=824; i<=850; i++) myPythia6->SetMDME(i,1,0); //Ds
    //for(int i=857; i<=862; i++) myPythia6->SetMDME(i,1,0); //eta_c,J/psi,chi_2c
    //for(int i=1097; i<=1165; i++) myPythia6->SetMDME(i,1,0); //Lc
    /*	myPythia6->SetMDCY(102,1,0);
	myPythia6->SetMDCY(106,1,0);
	myPythia6->SetMDCY(109,1,0);
	myPythia6->SetMDCY(116,1,0);
	myPythia6->SetMDCY(112,1,0);
	myPythia6->SetMDCY(105,1,0);
	myPythia6->SetMDCY(164,1,0);
	
	myPythia6->SetMDCY(167,1,0);
	myPythia6->SetMDCY(162,1,0);
	myPythia6->SetMDCY(169,1,0);
	myPythia6->SetMDCY(172,1,0);
	myPythia6->SetMDCY(174,1,0);
	myPythia6->SetMDCY(176,1,0);
	myPythia6->SetMDCY(122,1,0);
	myPythia6->SetMDCY(125,1,0);
	myPythia6->SetMDCY(128,1,0);
	myPythia6->SetMDCY(181,1,0); */
	
    //============================ initial run =============================
	
	myPythia6->SetMRPY(1,iseed); 
	myPythia6->Initialize("CMS","p","p",200);
	
	myPythia6->Pystat(1);
	myPythia6->Pystat(4);
	myPythia6->Pystat(5);
	
    //============================ run events ==============================
	

    
	const float PDGmassPion = 0.13957; // in GeV
	const float PDGmassKaon = 0.49368; // in GeV
	const float PDGmassD0 = 1.86484; // in GeV


	int nevent = 0;
	for(int i = 1;i<=Nevt;i++){
		myPythia6->GenerateEvent();
		hEventCounter->Fill(0); //all events

		//      if(i<10) myPythia6->Pylist(1);
		
		TObjArray *particles = myPythia6->GetListOfParticles();
		int nParticles = particles->GetEntries();
		if(i%10000==0) {
			cout<<"on event #"<<i<<endl;
		}


		for(int l=0;l<nParticles;l++) {
			TMCParticle *mParticle = (TMCParticle*)particles->At(l);
			if(!mParticle) continue;
            float px = mParticle->GetPx();
            float py = mParticle->GetPy();
            float pz = mParticle->GetPz();
            float energy = mParticle->GetEnergy();
            TLorentzVector fourMom(px,py,pz,energy);
	    if(fourMom.Pt()>0.0000001){
	      float eta = fourMom.Eta();
	      if(fabs(mParticle->GetKF())==321 && mParticle->GetKS()==1 && fabs(eta)<1) hPtYkaon->Fill(fourMom.Pt(), fourMom.Rapidity());
	      if(fabs(mParticle->GetKF())==211 && mParticle->GetKS()==1 && fabs(eta)<1) hPtYpion->Fill(fourMom.Pt(), fourMom.Rapidity());
	      if(fabs(mParticle->GetKF())==2212 && mParticle->GetKS()==1 && fabs(eta)<1) hPtYproton->Fill(fourMom.Pt(), fourMom.Rapidity());
	      if(fabs(mParticle->GetKF())==413 /*&& mParticle->GetKS()==1*/) {hPtYdstar->Fill(fourMom.Pt(), fourMom.Rapidity()); /*cout<<"dstar"<<endl;*/}
	      if(fabs(mParticle->GetKF())==421 /*&& mParticle->GetKS()==1*/) {hPtYd0->Fill(fourMom.Pt(), fourMom.Rapidity()); /*cout<<"d0"<<endl;*/}
	      if(fabs(mParticle->GetKF())==4 /*&& mParticle->GetKS()==1*/) {hPtYc->Fill(fourMom.Pt(), fourMom.Rapidity()); /*cout<<"cquark"<<endl;*/}
	    }
        }
		nevent ++;
	}//event

    //============================= output ==============================OA

    char rootfilename[100];
    sprintf(rootfilename,"out/pythiaevent_%d.root",irun);
    TFile* file = new TFile(rootfilename,"RECREATE");
    //    file->SetCompressionLevel(9);
    file->cd();

//    hTree->Write();
    hEventCounter->Write();
    hPtYkaon->Write();
    hPtYpion->Write();
    hPtYproton->Write();
    hPtYd0->Write();
    hPtYdstar->Write();
    hPtYc->Write();

    file->Close();

    myPythia6->Pystat(1);

    stopWatch->Stop();
    stopWatch->Print();
}
