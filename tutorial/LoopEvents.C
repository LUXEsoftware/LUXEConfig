#include <podio/Reader.h>      // makeReader
#include <podio/ROOTReader.h>  // for ROOTReader (similar for the others)
#include <edm4hep/MCParticleCollection.h>
#include <podio/Frame.h>
#include <DD4hep/BitFieldCoder.h>
#include <edm4hep/Constants.h>

auto rootReader = podio::ROOTReader(); //Selected low level reader to read root EDM4hep file
void init(){//Initialize file to read and setup decoder for detector segmentation CellID
	rootReader.openFile("../reconstruction/luxe_reco.edm4hep.root"); //File to read, change path to read any other EDM4hep file
	const auto decoder = dd4hep::DDSegmentation::BitFieldCoder("system:0:1,side:1:1,layer:2:2,module:4:1,sensor:5:5,x:32:-16,y:48:-16"); //Decoder of CellID from string in detector collection

	//const auto cats = rootReader.getAvailableCategories();
}
// const auto metadata = podio::Frame(rootReader.readEntry(podio::Category::Metadata, 0));//Locate metadata collection
// const auto siHitsEncoding = metadata.getParameter<std::string>(podio::collMetadataParamName("SiHits", edm4hep::labels::CellIDEncoding));//Get bit encoding string for SiHits collection CellID

//Example function to read an EDM4hep file, to execute this function call it from LoopEvents function
void ReadEDM4hepFile(){
	for (size_t i = 0; i < rootReader.getEntries(podio::Category::Event); ++i) {//For loop to loop through events from event 0 to event=Number of entries in EDM4hep file
		auto event = podio::Frame(rootReader.readEntry(podio::Category::Event, i));//Get entry data for event i
		const auto& siHit = event.get<edm4hep::SimTrackerHitCollection>("SiHits");//In event i get a SimTrackerHitCollection type collection from branch "SiHits" we will call this collection "siHit"
		//siHit containes all hits in the tracker in one event and to get their properties we have to loop through them
		for(auto trackerHit : siHit){//This if a for loop to loop through the hits in the tracker for event i
			auto simParticle = trackerHit.getParticle();//With this we get the MCParticle that generated this hit
			double x = trackerHit.getPosition().x; //Here we get the x coordinate (mm) of the hit and assign the value to variable x, we could do the same for y and z
			//Check EDM4hep page https://edm4hep.web.cern.ch/ for all available information in collection types
		}
		//Now lets read another collection for the same event i
		const auto& mcParticles = event.get<edm4hep::MCParticleCollection>("MCParticles");//MCParticle collection has all info about the particles generated during simulation
		for(auto particle : mcParticles){//Loop through particles in MCParticles for one event i
		cout << "MASS " << particle.getMass() <<"\n";//Example on how to get the mass of the particle and print it out to the terminal
		if(particle.getGeneratorStatus()==1){//Check if this MCParticle is primary or secondary
			cout << "This is secondary particle \n";
		}
		double x_vertex = particle.getVertex().x; //Read vertex coordinate x of MCParticle
		}
	}
	return ;

}


template<typename Collection>
double SumEnergyCollection(const Collection& collection){
	double SumEnergy =0;
	for(const auto& hit : collection){
		SumEnergy+=hit.getEnergy();		
		//cout << "POSITION Y " << PixelCaloHit.getEnergy() <<"\n";
	}
	return SumEnergy;
}
template<typename Collection>
double SumHitsCollection(const Collection& collection){
	int SumHits = 0;
	for(const auto& hit : collection){
		SumHits++;		
		//cout << "POSITION Y " << PixelCaloHit.getEnergy() <<"\n";
	}
	return SumHits;
}

template <typename CollectionT>
void GetEnergyInECAL(string collectionName){
	int MCParticles=0;
	TCanvas *c1 = new TCanvas("c1","c1",2000,1000);
	TH1D *hMCParticleEnergyDist = new TH1D("hMCParticleEnergyDist","Generated particle energy; [GeV]; Entries",100,0,15);
	TH1D *hMCParticleEnergyDistMissCalo = new TH1D("hMCParticleEnergyDistMissCalo","Generated particle energy of events that miss the calorimeter; [GeV]; Entries",100,0,15);
	TH1D *hHitsInECAL = new TH1D("hHitsInECAL","Total hits in event", 200, -0.5, 199.5);
	/*//Reco ranges
	TH1D *hSumEnergyECALDist = new TH1D("hSumEnergyECALDist","Total EDep per event ECAL; Total EDep [GeV]; Entries",100,0,5);
	TH1D *hSumEnergyECALNormDist = new TH1D("hSumEnergyECALNormDist","Total EDep/E per event ECAL ;Total EDep/ Particle energy ; Entries",100,0,1);
	TH1D *hSumEnergyECALNormDist_0_to_3 = new TH1D("hSumEnergyECALNormDist_0_to_3","Total EDep/E per event ECAL for particles from 0 to 3 GeV ;Total EDep/ Particle energy ; Entries",100,0,1);
	TH1D *hSumEnergyECALNormDist_3_to_6 = new TH1D("hSumEnergyECALNormDist_3_to_6","Total EDep/E per event ECAL for particles from 3 to 6 GeV ;Total EDep/ Particle energy ; Entries",100,0,1);
	TH1D *hSumEnergyECALNormDist_6_to_10 = new TH1D("hSumEnergyECALNormDist_6_to_10","Total EDep/E per event ECAL for particles from 6 to 10 GeV ;Total EDep/ Particle energy ; Entries",100,0,1);
	*/
	//Original output ranges
	TH1D *hSumEnergyECALDist = new TH1D("hSumEnergyECALDist","Total EDep per event ECAL; Total EDep [GeV]; Entries",100,-0.002,0.1);
	TH1D *hSumEnergyECALNormDist = new TH1D("hSumEnergyECALNormDist","Total EDep/E per event ECAL ;Total EDep/ Particle energy ; Entries",100,-0.002,0.02);
	TH1D *hSumEnergyECALNormDist_0_to_2 = new TH1D("hSumEnergyECALNormDist_0_to_2","Total EDep/E per event ECAL for particles from 0 to 2 GeV ;Total EDep/ Particle energy ; Entries",100,-0.002,0.02);
	TH1D *hSumEnergyECALNormDist_2_to_8 = new TH1D("hSumEnergyECALNormDist_2_to_8","Total EDep/E per event ECAL for particles from 2 to 8 GeV ;Total EDep/ Particle energy ; Entries",100,-0.002,0.02);
	TH1D *hSumEnergyECALNormDist_8_to_10 = new TH1D("hSumEnergyECALNormDist_8_to_10","Total EDep/E per event ECAL for particles from 8 to 10 GeV ;Total EDep/ Particle energy ; Entries",100,-0.002,0.02);
	
	/*//Digi ranges
	TH1D *hSumEnergyECALDist = new TH1D("hSumEnergyECALDist","Total EDep per event ECAL; Total EDep [GeV]; Entries",100,0,1000);
	TH1D *hSumEnergyECALNormDist = new TH1D("hSumEnergyECALNormDist","Total EDep/E per event ECAL ;Total EDep/ Particle energy ; Entries",100,0,200);
	TH1D *hSumEnergyECALNormDist_0_to_3 = new TH1D("hSumEnergyECALNormDist_0_to_3","Total EDep/E per event ECAL for particles from 0 to 3 GeV ;Total EDep/ Particle energy ; Entries",100,0,200);
	TH1D *hSumEnergyECALNormDist_3_to_6 = new TH1D("hSumEnergyECALNormDist_3_to_6","Total EDep/E per event ECAL for particles from 3 to 6 GeV ;Total EDep/ Particle energy ; Entries",100,0,200);
	TH1D *hSumEnergyECALNormDist_6_to_10 = new TH1D("hSumEnergyECALNormDist_6_to_10","Total EDep/E per event ECAL for particles from 6 to 10 GeV ;Total EDep/ Particle energy ; Entries",100,0,200);
	*/
	
	int TotalHits = 0;
	for (size_t i = 0; i < rootReader.getEntries(podio::Category::Event); ++i) {
		double SumEnergyECAL=0;
		auto event = podio::Frame(rootReader.readEntry(podio::Category::Event, i));
		const auto& PixelSiEcal = event.get<CollectionT>(collectionName);
//		for(auto PixelCaloHit : PixelSiEcal){
//			SumEnergyECAL=SumEnergyECAL+PixelCaloHit.getEnergy();		
//			//cout << "POSITION Y " << PixelCaloHit.getEnergy() <<"\n";
//		}
		SumEnergyECAL=SumEnergyCollection(PixelSiEcal);
		hHitsInECAL->Fill(SumHitsCollection(PixelSiEcal));
		TotalHits+=SumHitsCollection(PixelSiEcal);

		const auto& mcParticles = event.get<edm4hep::MCParticleCollection>("MCParticles");
		for(auto particle : mcParticles){
			MCParticles++;
			if(particle.getGeneratorStatus()==1){
				hSumEnergyECALNormDist->Fill(SumEnergyECAL/particle.getEnergy());
				hMCParticleEnergyDist->Fill(particle.getEnergy());
				if(SumEnergyECAL==0){
					hMCParticleEnergyDistMissCalo->Fill(particle.getEnergy());
				}
				if(particle.getEnergy()<2){
					hSumEnergyECALNormDist_0_to_2->Fill(SumEnergyECAL/particle.getEnergy());
				}
				else if(particle.getEnergy()<8){
					hSumEnergyECALNormDist_2_to_8->Fill(SumEnergyECAL/particle.getEnergy());
				}
				else{
					hSumEnergyECALNormDist_8_to_10->Fill(SumEnergyECAL/particle.getEnergy());
				}
			}
		}
		hSumEnergyECALDist->Fill(SumEnergyECAL);
	}
	cout << "Number of MCParticles " << MCParticles <<"\n";
	cout << "Number of hits in ecal " << TotalHits <<"\n";
	c1->cd()->SetLogy();
	hHitsInECAL->Draw();
	c1->Print("HitsECAL.png");
	hMCParticleEnergyDistMissCalo->Draw();
	c1->Print("MCParticleEnergyMissCalo.png");
	c1->cd();
	hMCParticleEnergyDist->Draw();
	c1->Print("MCParticleEnergy.png");
	c1->cd()->SetLogy();
	hSumEnergyECALDist->Draw();
	c1->Print("totalEnergyDepECAL.png");
	c1->cd()->SetLogy();
	hSumEnergyECALNormDist->Draw();
	c1->Print("totalEnergyDepECALparticleEnergy.png");
	c1->cd()->SetLogy();
	hSumEnergyECALNormDist_0_to_2->Draw();
	c1->Print("totalEnergyDepECALparticleEnergy_0-2.png");
	c1->cd()->SetLogy();
	hSumEnergyECALNormDist_2_to_8->Draw();
	c1->Print("totalEnergyDepECALparticleEnergy_2-8.png");
	c1->cd()->SetLogy();
	hSumEnergyECALNormDist_8_to_10->Draw();
	c1->Print("totalEnergyDepECALparticleEnergy_8-10.png");
	c1->Close();
}

void PlotTrackerHitsNonFromSecParticle(){
	//TH1D *hmass= new TH1D("hmass","mass",10,0,0.001);
	// low level reader
	for (size_t i = 0; i < rootReader.getEntries(podio::Category::Event); ++i) {
		TCanvas *c1 = new TCanvas("c1","c1",2000,1000);
		TGraph2D *gPositionXZETracker= new TGraph2D();//("hPositionXY","XY Position",1000,0,500,100,-5,5);
		TGraph2D *gPositionXYETracker= new TGraph2D();//("hPositionXY","XY Position",1000,0,500,100,-5,5);
		gPositionXYETracker->SetMarkerStyle(34);
		gPositionXYETracker->SetMarkerSize(2);
		gPositionXZETracker->SetMarkerSize(2);
		gPositionXZETracker->SetMarkerStyle(34);
		auto event = podio::Frame(rootReader.readEntry(podio::Category::Event, i));
		const auto& siHit = event.get<edm4hep::SimTrackerHitCollection>("SiHits");
		for(auto trackerHit : siHit){
			//auto hitLayer  = decoder.get(trackerHit.getCellID(), "layer");
			auto simParticle = trackerHit.getParticle();
			if(trackerHit.isProducedBySecondary()){ 
				gPositionXYETracker->AddPoint(trackerHit.getPosition().x,trackerHit.getPosition().y,simParticle.getEnergy());
				gPositionXZETracker->AddPoint(trackerHit.getPosition().x,trackerHit.getPosition().z,simParticle.getEnergy());
			}
		}
		c1->cd();
		c1->SetTheta(89.99511);
		c1->SetPhi(3.184653e-11);
		gStyle->SetOptStat("e");
		gStyle->SetPalette(109);
		gPositionXZETracker->Draw("pcolz");
		c1->Print(Form("XZEMCParticleEnergyTracker_%d.png",1));
		gPositionXYETracker->Draw("pcolz");
		c1->Print(Form("XYEMCParticleEnergyTracker_%d.png",1));
	}
	return ;

}

void GetEventsAboveThreshold(){
	//TH1D *hmass= new TH1D("hmass","mass",10,0,0.001);
	int particles=0;
	int EventsInECAL=0;
	double MCParticleEnergyThr=5;
	double xTrackerThr=400;
	int eventsList[7] = {1283,2253,2290,2674,7789,7903,9113};
	// low level reader
	//for (size_t i = 0; i < rootReader.getEntries(podio::Category::Event); ++i) {
	for (int i : eventsList){
		int backscatterParticles=0;
		int backscatterParticlesHits=0;
		int hitsInTracker=0;
		int hitsInECAL=0;
		int secondaryParticlesHits=0;
		TCanvas *c1 = new TCanvas("c1","c1",2000,1000);
		TGraph2D *gPositionXYZ= new TGraph2D();//("hPositionXY","XY Position",1000,0,500,100,-5,5);
		TGraph2D *gPositionXZETrackerSec= new TGraph2D();//("hPositionXY","XY Position",1000,0,500,100,-5,5);
		TGraph2D *gPositionXYETrackerSec= new TGraph2D();//("hPositionXY","XY Position",1000,0,500,100,-5,5);
		TGraph2D *gPositionXZETracker= new TGraph2D();//("hPositionXY","XY Position",1000,0,500,100,-5,5);
		TGraph2D *gPositionXYETracker= new TGraph2D();//("hPositionXY","XY Position",1000,0,500,100,-5,5);
		TGraph2D *gPositionXYEMCParticle= new TGraph2D();//("hPositionXY","XY Position",1000,0,500,100,-5,5);
		gPositionXYZ->SetMarkerStyle(8);
		gPositionXYZ->SetMarkerSize(6);
		gPositionXYETrackerSec->SetMarkerStyle(34);
		gPositionXYETrackerSec->SetMarkerSize(6);
		gPositionXZETrackerSec->SetMarkerSize(6);
		gPositionXZETrackerSec->SetMarkerStyle(34);
		gPositionXYETracker->SetMarkerStyle(8);
		gPositionXYETracker->SetMarkerSize(4);
		gPositionXZETracker->SetMarkerSize(4);
		gPositionXZETracker->SetMarkerStyle(8);
		gPositionXYEMCParticle->SetMarkerStyle(8);
		gPositionXYEMCParticle->SetMarkerSize(6);
		bool xTrackerAboveThr=false;
		bool MCParticleEnergyAboveThr=false;
	  auto event = podio::Frame(rootReader.readEntry(podio::Category::Event, i));
	  const auto& siHit = event.get<edm4hep::SimTrackerHitCollection>("SiHits");
	  for(auto trackerHit : siHit){
		auto simParticle = trackerHit.getParticle();
		//gPositionXYZ->AddPoint(trackerHit.getPosition().x,trackerHit.getPosition().y,trackerHit.getPosition().z);
		//if(simParticle.isBackscatter()){
		//if(simParticle.getGeneratorStatus()==1){
			//gPositionXYETracker->AddPoint(trackerHit.getPosition().x,trackerHit.getPosition().y,simParticle.getEnergy());
		//}
			//cout << "POSITION Y " << hit.getPosition().y <<"\n";
		hitsInTracker++;
		if(simParticle.isBackscatter()){ backscatterParticlesHits++;}
		if(trackerHit.isProducedBySecondary()){ 
			secondaryParticlesHits++;
			gPositionXYETrackerSec->AddPoint(trackerHit.getPosition().x,trackerHit.getPosition().y,simParticle.getEnergy());
			gPositionXZETrackerSec->AddPoint(trackerHit.getPosition().x,trackerHit.getPosition().z,simParticle.getEnergy());
		}
		if(simParticle.getEnergy()>MCParticleEnergyThr)
		{
			MCParticleEnergyAboveThr=true;
		}
		if(trackerHit.getPosition().x>xTrackerThr)
		{
			xTrackerAboveThr=true;
		}
	    //if(MCParticleEnergyAboveThr&&xTrackerAboveThr){
		//if(!trackerHit.isProducedBySecondary()){ 
			gPositionXYETracker->AddPoint(trackerHit.getPosition().x,trackerHit.getPosition().y,simParticle.getEnergy());
			gPositionXZETracker->AddPoint(trackerHit.getPosition().x,trackerHit.getPosition().z,simParticle.getEnergy());
		//}
	  }
	  const auto& PixelSiEcal = event.get<edm4hep::SimCalorimeterHitCollection>("PixelSiEcalCollection");
	  if(MCParticleEnergyAboveThr&&xTrackerAboveThr){
		EventsInECAL++;
		cout << i << "\n";
		for(auto PixelCaloHit : PixelSiEcal){
			gPositionXYZ->AddPoint(PixelCaloHit.getPosition().x,PixelCaloHit.getPosition().y,PixelCaloHit.getPosition().z);
			//cout << "POSITION Y " << hit.getPosition().y <<"\n";
			hitsInECAL++;
		}
		}
	  const auto& mcParticles = event.get<edm4hep::MCParticleCollection>("MCParticles");
	  for(auto particle : mcParticles){
		//cout << "MASS " << particle.getMass() <<"\n";
		//hmass->Fill(particle.getMass());
		if(particle.isBackscatter()){
			backscatterParticles++;
	  	}
		if(particle.getGeneratorStatus()==1){
			gPositionXYEMCParticle->AddPoint(particle.getVertex().x,particle.getVertex().y,particle.getVertex().z);
		}
			particles++;
	  }
	  // ...
		c1->cd();
		gPositionXYZ->Draw("pcolz");
		c1->Print(Form("XYZECAL_%d.png",i));
		c1->SetTheta(89.99511);
		c1->SetPhi(3.184653e-11);
		gStyle->SetOptStat("e");
		gStyle->SetPalette(109);
		gPositionXZETracker->Draw("pcolz");
		gPositionXZETrackerSec->Draw("pcolz same");
		c1->Print(Form("XZEMCParticleEnergyTracker_%d.png",i));
		gPositionXYETracker->Draw("pcolz");
		gPositionXYETrackerSec->Draw("pcolz same");
		c1->Print(Form("XYEMCParticleEnergyTracker_%d.png",i));
		gPositionXYEMCParticle->Draw("pcolz");
		c1->Print(Form("XYEMCParticleEnergy_%d.png",i));
		c1->Close();
		cout << "number of Secondary particles hits in tracker " << secondaryParticlesHits <<"\n";
		cout << "number of hits in ECAL " << hitsInECAL <<"\n";
		cout << "number of EVENTS w hits in ECAL " << EventsInECAL <<"\n";
		cout << "number of hits in tracker " << hitsInTracker <<"\n";
		cout << "number of backscattered particles " << backscatterParticles <<"\n";
		cout << "number of backscattered particles hits in tracker " << backscatterParticlesHits <<"\n";
		cout << "number of particles " << particles <<"\n";
	}
	return ;

}

void LoopEvents(){
	init();
	ReadEDM4hepFile();
	//Other function examples
	//GetEventsAboveThreshold();
	//GetEnergyInECAL<edm4hep::CalorimeterHitCollection>("PixelSiEcalCollectionDigi");
	//GetEnergyInECAL<edm4hep::SimCalorimeterHitCollection>("PixelSiEcalCollection");
}
