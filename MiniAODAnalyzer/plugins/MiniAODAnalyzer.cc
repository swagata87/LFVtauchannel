// -*- C++ -*-
//
// Package:    WPrimeToTau/MiniAODAnalyzer
// Class:      MiniAODAnalyzer
//
/**\class MiniAODAnalyzer MiniAODAnalyzer.cc WPrimeToTau/MiniAODAnalyzer/plugins/MiniAODAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Swagata Mukherjee
//         Created:  Tue, 05 Jul 2016 09:41:37 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/global/EDFilter.h"
#include "FWCore/Framework/interface/EDProducer.h"
//#include "PhysicsTools/HepMCCandAlgos/interface/PDFWeightsHelper.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHERunInfoProduct.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "DataFormats/Common/interface/RefToPtr.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Particle.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "DataFormats/PatCandidates/interface/PackedTriggerPrescales.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/PatCandidates/interface/VIDCutFlowResult.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include <vector>
#include "TTree.h"
#include "TFile.h"
#include <string>
#include "TH1.h"
#include "TLorentzVector.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <fstream>
#include "TProfile.h"

//#include "stdlib.h"
// cant set lorentzvetor branch without this
#ifdef __CINT__
#pragma link C++ class std::vector<TLorentzVector>+;
#endif
#ifdef __CINT__
#pragma link C++ class std::vector<float>+;
#endif

// new includes
#include <unordered_map>
#include <unordered_set>
#include "helper.hh"
#include "TString.h"
//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.

//class MiniAODAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
class MiniAODAnalyzer : public edm::EDAnalyzer {
public:
  explicit MiniAODAnalyzer(const edm::ParameterSet&);
  ~MiniAODAnalyzer();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  virtual void beginJob() override;
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override;
  virtual void beginRun( edm::Run const &iRun, edm::EventSetup const &iSetup ) override;

  float DRTauMu(TLorentzVector, std::vector<TLorentzVector>);

  bool PassTauID(const pat::Tau &tau);
  bool PassTauID_noEleRej(const pat::Tau &tau);
  bool PassTauID_noMuRej(const pat::Tau &tau);
  bool PassTauID_Old_VLoose(const pat::Tau &tau);
  bool PassTauID_Old_Loose(const pat::Tau &tau);
  bool PassTauID_Old_Medium(const pat::Tau &tau);
  bool PassTauID_Old_VTight(const pat::Tau &tau);
  bool PassTauID_Old_Tight(const pat::Tau &tau);

  bool PassTauID_New_VLoose(const pat::Tau &tau);
  bool PassTauID_New_Loose(const pat::Tau &tau);
  bool PassTauID_New_Medium(const pat::Tau &tau);
  bool PassTauID_New_VTight(const pat::Tau &tau);
  bool PassTauID_New_Tight(const pat::Tau &tau);
 
  bool PassTauID_decay(const pat::Tau &tau);
  bool PassTauID_decay_iso(const pat::Tau &tau);
  bool PassTauID_decay_iso_muon(const pat::Tau &tau);
  bool PassTauID_NonIsolated(const pat::Tau &tau);
  bool PassTauID_NonIsolated(const pat::Tau &tau, std::string idcheck);
  bool PassTauAcceptance(TLorentzVector tau);
  bool PassTauAcceptanceForCR(TLorentzVector tau);
  bool PassTauAcceptanceForTrigEff(TLorentzVector tau); 
  bool FindTauIDEfficiency(const edm::Event&,TLorentzVector gen_p4);
  bool FindTauTrigEfficiency(bool passRefTrigger, bool passAnaTrigger, int nTau, double met, double tauPt, double tauEta);
  bool FindTauTrigEfficiency_Signal(bool passAnaTrigger, int nTau, double met, double tauPt, double tauEta, double MT);
  bool PassFinalCuts(int nGoodTau_, double met_val_, double met_phi_, double tau_pt_, double tau_phi_);
  bool PassFinalCuts(TLorentzVector part1, const pat::MET part2, pat::MET::METUncertainty metUncert);
  bool PassFinalCuts(TLorentzVector part1, const pat::MET part2);
  bool PassFinalCuts(int nGoodTau_,TLorentzVector part1, const pat::MET part2);
  bool PassFinalCuts_Except_dphiTauMET(int nGoodTau_, double met_val_, double met_phi_, double tau_pt_, double tau_phi_);
  bool PassFinalCuts_Except_pToverEtMiss(int nGoodTau_, double met_val_, double met_phi_, double tau_pt_, double tau_phi_);
  bool PassFinalCuts_noMET(int nGoodTau_, double met_val_, double met_phi_, double tau_pt_, double tau_phi_);
  double GetTauIDScaleFactor(double tau_pt, std::string mode);
  double GetTauIDScaleFactorUncert(double SF, double tau_pt, std::string mode, std::string up, std::string down);
  bool Overlap(edm::Handle<edm::View<reco::GenParticle>>, double, double);

  std::vector<int> *pdf_indices = new std::vector<int>;
  std::vector<int> alpha_indices;

  std::vector<double> *inpdfweights = new std::vector<double>;
  std::vector<double> *alpha_s_container = new std::vector<double>;

  //new additions
  virtual void Create_Trees();
  virtual void Fill_Tree(TLorentzVector sel_lepton, const pat::MET sel_met,double weight, std::string systematic);
  //virtual void Fill_QCD_Tree(bool iso,TLorentzVector sel_lepton, const pat::MET sel_met, double weight);
  virtual void Fill_QCD_Tree(bool iso,TLorentzVector sel_lepton, int lepton_n, edm::Handle<std::vector<pat::Jet>> JetList,const pat::MET sel_met, double weight,int ifake, int tauDM, int nTRK, double chiso, double neuiso);

  std::unordered_map< std::string,float > mLeptonTree;
  //  std::unordered_map< std::string,float > *mLeptonTree = new std::unordered_map< std::string,float >;
  std::unordered_map< std::string,float > mReweightTree;
  std::unordered_map< std::string,float > mQCDTree;
  std::unordered_map< std::string,float > mFakeTree;
  std::unordered_map< std::string,std::vector<float> > mFakeVectorTree;
  //std::unordered_map< std::string,float > mFakeVectorTree;
  edm::Service<TFileService> fs;
  Helper* helper =new Helper(fs);

  //MT addition
  double calcMT(TLorentzVector part1, TLorentzVector part2);
  double calcMT(TLorentzVector part1, const pat::MET part2);
  double calcMT(TLorentzVector part1, const pat::MET part2, pat::MET::METUncertainty metUncert);
  double calcMT(const pat::Electron part1, const pat::MET part2);
  double calcMT(const pat::Muon part1, const pat::MET part2);
  double calcMT(const pat::Tau part1, const pat::MET part2);

  //  std::unordered_map< std::string,pat::MET::METUncertainty > mSyst;
  // std::unordered_map< std::string,TH1F* > mSystHist;
  // std::unordered_map< std::string,std::string > mSystName;
  // virtual void SetSystMap();

  //deltaR calculation
  double DeltaR(auto part ,auto tau);
  double DeltaR(pat::Electron part ,pat::Tau tau);
  double DeltaR(pat::Muon part,pat::Tau tau);

  //deltaPhi calculation
  double DeltaPhi(pat::Electron part1, pat::MET part2);
  double DeltaPhi(pat::Muon part1, pat::MET part2);
  double DeltaPhi(pat::Tau part1, pat::MET part2);

  //kfactor
  double applyWKfactor(int mode,edm::Handle<edm::View<reco::GenParticle>> genPart);
  std::string KFactorE_;
  std::string KFactorMu_;
  std::string KFactorTau_;
  TFile* m_kfactorFile_ele;
  TFile* m_kfactorFile_muo;
  TFile* m_kfactorFile_tau;
  TH1F* m_kfactorHist_ele[3];
  TH1F* m_kfactorHist_muo[3];
  TH1F* m_kfactorHist_tau[3];
  std::string sourceFileString;
  double WtoInt;
  double WJetsInt;
  int getWdecay(edm::Handle<edm::View<reco::GenParticle>> genPart);
  double getWmass(edm::Handle<edm::View<reco::GenParticle>> genPart);
  double wmass_stored;
  double k_fak_stored;
  double k_fak=1.0;
  double k_fak_up=1.0;
  double k_fak_down=1.0;
  //
  double tauID_SF =1.0;
  double tauID_SF_flat_syst_up =1.0 ;
  double tauID_SF_flat_syst_down =1.0;

  double tauID_SF_other =1.0;
  double tauID_SF_flat_syst_up_other =1.0 ;
  double tauID_SF_flat_syst_down_other =1.0;

  double tauID_SF_ptDep_syst_up =1.0 ;
  double tauID_SF_ptDep_syst_down =1.0;

  double tauID_SF_ptDep_syst_up5 =1.0 ;
  double tauID_SF_ptDep_syst_up50 =1.0 ;
  double tauID_SF_ptDep_syst_down50 =1.0;

  double tauID_SF_ptDep_syst_up50_other =1.0 ;
  double tauID_SF_ptDep_syst_down50_other =1.0;
  //
  double tauELE_SF =1.0;
  double tauELE_SF_syst_up =1.0 ;
  double tauELE_SF_syst_down =1.0;
  //
  double trig_SF =1.0;
  double trig_SF_syst_up =1.0 ;
  double trig_SF_syst_down =1.0;
  //
  double genHT = 0.0;
  //  double genHT_fake = 0.0;
  double genMTT = 0.0;

  float alpha_s_wt_down = 1.0;
  float alpha_s_wt_up = 1.0;
  //
  int tauDM=100;
  //  double genMTT2 = 0.0;
  //discriminators
  std::vector<std::string> *d_mydisc = new std::vector<std::string> ;
  std::vector<std::string> *d_mydisc_emu = new std::vector<std::string> ;
  std::vector<std::string> *d_mydisc_mu = new std::vector<std::string> ;
  void SetUpDisc();

  //QCD stuff
  double minDphiMET(std::vector<pat::Jet> JetList, const pat::MET sel_met);
  void QCDAnalyse(const pat::MET sel_met);
  bool check_single_tau_kinematics(pat::Tau lepton, const pat::MET sel_met);

  edm::Handle<pat::MuonCollection> muons;
  // pat::Electron objects can be recast as reco::GsfElectron objects //
  edm::Handle<edm::View<reco::GsfElectron> > electrons;
  //edm::Handle<pat::ElectronCollection> electrons;
  edm::Handle<pat::TauCollection> taus;
  edm::Handle<pat::JetCollection> jets;
  edm::Handle<edm::View<reco::GenParticle> > pruned; ///might not work for data
  //reco::GenParticle* GetTruthMatch(std::string name, pat::Tau lepton);
  reco::GenParticle* GetTruthMatch(std::string name, auto lepton);
  reco::GenParticle* GetTruthMatchAllFlavor(auto lepton);
  std::map< pat::Tau*,reco::GenParticle* > tauGenMatchMapAllFlav;
  std::map< pat::Tau*,reco::GenParticle* > tauGenMatchMap;

  ///rest qcd stuff
  void QCDAnalyseTau(const pat::MET sel_met,double weight,edm::Handle<edm::View<reco::GenParticle>> genPart);
  int vetoNumberEle(double ptTreshold,double vetoConeSize);
  int vetoNumberMuon(double ptTreshold,double vetoConeSize);
  int vetoNumberTau(double ptTreshold);
  bool m_do_complicated_tau_stuff;
  //std::map<int,std::vector<pat::Particle>>qcd_leptons;
  std::vector<pat::Tau> *qcd_lepton_tau = new std::vector<pat::Tau>;
  std::vector<double> *qcd_weight_tau = new std::vector<double>;
  std::vector<pat::Electron> *qcd_lepton_ele = new std::vector<pat::Electron>;
  std::vector<double> *qcd_weight_ele = new std::vector<double>;
  std::vector<pat::Muon> *qcd_lepton_mu = new std::vector<pat::Muon>;
  std::vector<double> *qcd_weight_mu = new std::vector<double>;

  //std::string QCDWeightE_;
  //std::string QCDWeightMu_;
  std::string QCDWeightTau_;
  //TFile* m_qcdweightFile_ele;
  //TFile* m_qcdweightFile_muo;
  TFile* m_qcdweightFile_tau;
  //TH1D* hist_qcd_weight_ele;
  //TH1D* hist_qcd_weight_muo;
  TH2D* hist_qcd_weight_tau;

  std::vector<int> *EleIDPassed = new std::vector<int> ;
  std::vector<int> *MuonIDPassed = new std::vector<int>;
  std::vector<float> *FakeCandPt = new std::vector<float>;
  std::vector<float> *FakeCandPhi = new std::vector<float>;
  std::vector<float> *FakeCandEta = new std::vector<float>;
  std::vector<float> *FakeCandMt = new std::vector<float>;
  std::vector<float> *FakeCandDeltaPhi = new std::vector<float>;
  std::vector<float> *FakeCandMET = new std::vector<float>;
  std::vector<float> *FakeCandMETPhi = new std::vector<float>;
  std::vector<float> *FakeCandIso = new std::vector<float>;
  std::vector<float> *FakeCandNoIso = new std::vector<float>;
  std::vector<float> *FakeCandVetoNum = new std::vector<float>;
  std::vector<float> *FakeCandDecayMode = new std::vector<float>;
  std::vector<float> *FakeCandLeptonType = new std::vector<float>;
  std::vector<float> *FakeCandLeptonMt = new std::vector<float>;
  std::vector<float> *FakeCandLeptonDeltaPhi = new std::vector<float>;
  std::vector<float> *FakeCandLeptonPt = new std::vector<float>;
  std::vector<float> *FakeCandLeptonEta = new std::vector<float>;
  std::vector<float> *FakeCandLeptonPhi = new std::vector<float>;
  std::vector<float> *FakeCandLeptonTrigger = new std::vector<float>;
  std::vector<TLorentzVector> *FakeCandLorentz = new std::vector<TLorentzVector>;

  //trigger for ele + muon
  bool passEleTrig=false;
  bool passMuonTrig;
  bool passMuonTrig_ForEff=false;

  //reweighting stuff
  bool useReweighting;
  void setShiftedTree(TLorentzVector sel_lepton, const pat::MET sel_met,double weight, pat::MET::METUncertainty metUncert, std::string name);
  void setShiftedTree(TLorentzVector sel_lepton, const pat::MET sel_met, double weight, std::string name);

  // ----------member data ---------------------------
  edm::LumiReWeighting LumiWeights_;
  edm::LumiReWeighting LumiWeights_UP_;
  edm::LumiReWeighting LumiWeights_DOWN_;
  edm::EDGetTokenT<reco::VertexCollection> vtxToken_;
  edm::EDGetTokenT<pat::TauCollection> tauToken_;
  edm::EDGetTokenT<pat::MuonCollection> muonToken_;
  edm::EDGetTokenT<pat::JetCollection> jetToken_;
  edm::EDGetToken  electronToken_;
  //edm::EDGetTokenT<pat::ElectronCollection> electronToken_;
  edm::EDGetTokenT<pat::METCollection> metToken_;
  edm::EDGetTokenT<pat::METCollection> metToken_reminiaod_;
  edm::EDGetTokenT<edm::TriggerResults> triggerBits_;
  edm::EDGetTokenT<edm::TriggerResults> triggerBits_MET_;
  edm::EDGetTokenT<pat::TriggerObjectStandAloneCollection> triggerObjects_;
  edm::EDGetTokenT<pat::PackedTriggerPrescales> triggerPrescales_;
  // edm::EDGetTokenT<edm::ValueMap<bool> > eleLooseIdMapToken_;
  edm::EDGetTokenT<edm::View<reco::GenParticle> > prunedGenToken_;
  edm::EDGetTokenT<edm::View<pat::PackedGenParticle> > packedGenToken_;
  edm::EDGetTokenT<GenEventInfoProduct> genEventInfoProductTagToken_;
  edm::EDGetTokenT<std::vector<PileupSummaryInfo> > puCollection_;
  edm::EDGetTokenT<bool> BadChCandFilterToken_;
  edm::EDGetTokenT<bool> BadPFMuonFilterToken_;
  edm::EDGetTokenT<edm::ValueMap<bool> > eleIdMapToken_;
  edm::EDGetTokenT<LHEEventProduct> LHEEventToken_;
  edm::EDGetTokenT<LHERunInfoProduct> LHERunInfoToken_;

  //------//
  std::string pdfName_;
  //  std::string lheString = "source" ; //  "externalLHEProducer" ;
  std::string lheString =  "externalLHEProducer" ;
  std::string pileupMC_ ;
  std::string pileupData_ ;
  std::string pileupData_UP_ ;
  std::string pileupData_DOWN_ ;
  std::string pdfid_1;
  std::string pdfid_2;
  std::string alphas_id_1;
  std::string alphas_id_2;

  std::string tag_;
  bool RunOnData;
  bool doTrees;
  bool doFakeHist;
  bool doPDFuncertainty;
  std::string generatorName_;
  int debugLevel;
  //
  //CONTROL region 
  //
  TProfile* hprof_taueta_dRtaumu; 
  TProfile* hprof_taueta_dRtaumu_pT_less_80; 
  TProfile* hprof_taueta_dRtaumu_pT_80_140; 
  TProfile* hprof_taueta_dRtaumu_pT_above_140; 

  TH2F* h2_taueta_dRtaumu;
  TH2F* h2_taueta_dRtaumu_pT_less_80;
  TH2F* h2_taueta_dRtaumu_pT_80_140;
  TH2F* h2_taueta_dRtaumu_pT_above_140;
  //
  TH1F* h1_recoil_CR;
  TH1F* h1_tauPtOverMet_CR;
  TH1F* h1_dPhiTauMet_CR;
  TH1F* h1_MT_CR;
  TH1F* h1_tauPt_CR;
  TH1F* h1_tauPhi_CR;
  TH1F* h1_tauEta_CR;
  TH1F* h1_dimuonMass_CR;
  TH1F* h1_dimuonPt_CR;
  TH1F* h1_original_MET_CR;
    TH1F* h1_TauCharge_CR;
   TH1F* h1_TauDxy_CR;
  TH1F* h1_TauChIso_CR;
   TH1F* h1_TauNeuIso_CR;
    TH1F* h1_TauNTrack_CR;
  //
  //CR2
  //
  TH1F* h1_recoil_CR2;
  TH1F* h1_tauPtOverMet_CR2;
  TH1F* h1_dPhiTauMet_CR2;
  TH1F* h1_MT_CR2;
  TH1F* h1_tauPt_CR2;
  TH1F* h1_tauPhi_CR2;
  TH1F* h1_tauEta_CR2;
  TH1F* h1_dimuonMass_CR2;
  TH1F* h1_dimuonPt_CR2;
  TH1F* h1_original_MET_CR2;
  TH1F* h1_TauCharge_CR2;
  TH1F* h1_TauDxy_CR2;
  TH1F* h1_TauChIso_CR2;
  TH1F* h1_TauNeuIso_CR2;
  TH1F* h1_TauNTrack_CR2;
  //
  //CR3
  //
  TH1F* h1_recoil_CR3;
  TH1F* h1_tauPtOverMet_CR3;
  TH1F* h1_dPhiTauMet_CR3;
  TH1F* h1_MT_CR3;
  TH1F* h1_tauPt_CR3;
  TH1F* h1_tauPhi_CR3;
  TH1F* h1_tauEta_CR3;
  TH1F* h1_dimuonMass_CR3;
  TH1F* h1_dimuonPt_CR3;
  TH1F* h1_original_MET_CR3;
  TH1F* h1_TauCharge_CR3;
  TH1F* h1_TauDxy_CR3;
  TH1F* h1_TauChIso_CR3;
  TH1F* h1_TauNeuIso_CR3;
  TH1F* h1_TauNTrack_CR3;
  //
  //CR4
  //
  TH1F* h1_recoil_CR4;
  TH1F* h1_tauPtOverMet_CR4;
  TH1F* h1_dPhiTauMet_CR4;
  TH1F* h1_MT_CR4;
  TH1F* h1_tauPt_CR4;
  TH1F* h1_tauPhi_CR4;
  TH1F* h1_tauEta_CR4;
  TH1F* h1_dimuonMass_CR4;
  TH1F* h1_dimuonPt_CR4;
  TH1F* h1_original_MET_CR4;
  TH1F* h1_TauCharge_CR4;
  TH1F* h1_TauDxy_CR4;
  TH1F* h1_TauChIso_CR4;
  TH1F* h1_TauNeuIso_CR4;
  TH1F* h1_TauNTrack_CR4;
  //
  //CR5
  //
  TH1F* h1_recoil_CR5;
  TH1F* h1_tauPtOverMet_CR5;
  TH1F* h1_dPhiTauMet_CR5;
  TH1F* h1_MT_CR5;
  TH1F* h1_tauPt_CR5;
  TH1F* h1_tauPhi_CR5;
  TH1F* h1_tauEta_CR5;
  TH1F* h1_dimuonMass_CR5;
  TH1F* h1_dimuonPt_CR5;
  TH1F* h1_original_MET_CR5;
  TH1F* h1_TauCharge_CR5;
  TH1F* h1_TauDxy_CR5;
  TH1F* h1_TauChIso_CR5;
  TH1F* h1_TauNeuIso_CR5;
  TH1F* h1_TauNTrack_CR5;

  //
  //CR6
  //
  TH1F* h1_recoil_CR6;
  TH1F* h1_tauPtOverMet_CR6;
  TH1F* h1_dPhiTauMet_CR6;
  TH1F* h1_MT_CR6;
  TH1F* h1_tauPt_CR6;
  TH1F* h1_tauPhi_CR6;
  TH1F* h1_tauEta_CR6;
  TH1F* h1_dimuonMass_CR6;
  TH1F* h1_dimuonPt_CR6;
  TH1F* h1_original_MET_CR6;
  TH1F* h1_TauCharge_CR6;
  TH1F* h1_TauDxy_CR6;
  TH1F* h1_TauChIso_CR6;
  TH1F* h1_TauNeuIso_CR6;
  TH1F* h1_TauNTrack_CR6;


  //
  TH1I *h1_EventCount;
  TH1I *h1_EventCount2;
  TH1I *h1_EventCount_Stage1;
  TH1I *h1_EventCount_Stage1_needEleSF;

  TH1F* h1_dr_tau_ele;
  TH1F* h1_dr_tau_mu;

  TH1F *h1_genMTT;
  TH1F *h1_TauPt_Gen_den;
  TH1F *h1_TauPt_Gen_den_reco;
  TH1F *h1_TauPt_Gen_den_decay;
  TH1F *h1_TauPt_Gen_den_decay_iso;
  TH1F *h1_TauPt_Gen_den_decay_iso_muon;
  //
  TH1F *h1_TauPt_Gen_num_reco;
  TH1F *h1_TauPt_Gen_num_tot;
  TH1F *h1_TauPt_Gen_num_decay;
  TH1F *h1_TauPt_Gen_num_decay_iso;
  TH1F *h1_TauPt_Gen_num_decay_iso_muon;
  TH1F *h1_TauPt_Gen_num_decay_iso_muon_ele;
  //
  TH1I *h1_nGoodTau_Reco;
  TH1I *h1_nGenTau;
  //  TH1F *h1_TauPt_reco;
  // TH1F *h1_TauEta_reco;
  TH1F *h1_TauPt_TrigEff_Deno;
  TH1F *h1_TauPt_TrigEff_Num;
  TH1F *h1_TauEta_TrigEff_Deno;
  TH1F *h1_TauEta_TrigEff_Num;
  TH1F *h1_met_TrigEff_Deno;
  TH1F *h1_met_TrigEff_Num;
  //
  TH1F *h1_TauPt_TrigEff_Sig_Deno;
  TH1F *h1_TauPt_TrigEff_Sig_Num;
  TH1F *h1_TauEta_TrigEff_Sig_Deno;
  TH1F *h1_TauEta_TrigEff_Sig_Num;

  TH1F *h1_met_TrigEff_Sig_Deno;
  TH1F *h1_met_TrigEff_Sig_Num;

  TH1F *h1_MT_TrigEff_Sig_Deno;
  TH1F *h1_MT_TrigEff_Sig_Num;


  // TH1F *h1_TauPt_goodreco;
  //  TH1F *h1_TauEta_goodreco;
  TH2F *h2_IDfailEle_eta_pt_Stage1;

  TH1F *h1_IDfailEle_eta_Stage1;
  TH1F *h1_IDfailEle_pt_Stage1;

  TH1F *h1_TauPt_Stage1;
  TH1F *h1_TauCharge_Stage1;
  TH1F *h1_TauDxy_Stage1;
  TH1F *h1_TauChIso_Stage1;
  TH1F *h1_TauNeuIso_Stage1;
  TH1F *h1_TauNTrack_Stage1;
  TH1F *h1_TauEta_Stage1;
  TH1F *h1_TauPhi_Stage1;
  TH1F *h1_pToverEtMiss_Stage1;
  TH1F *h1_dPhi_tau_MET_Stage1;
  //
  TH1F *h1_MT_Stage1_NullDM;
  TH1F *h1_MT_Stage1_OneProngDM;
  TH1F *h1_MT_Stage1_TwoProngDM;
  TH1F *h1_MT_Stage1_ThreeProngDM;
  TH1F *h1_MT_Stage1_RareDM;
  //
 TH1F *h1_TauPhi_Stage1_NullDM;
 TH1F *h1_TauPhi_Stage1_OneProngDM;
 TH1F *h1_TauPhi_Stage1_TwoProngDM;
 TH1F *h1_TauPhi_Stage1_ThreeProngDM;
 TH1F *h1_TauPhi_Stage1_RareDM;
 
  //
  TH1F *h1_TauEta_Stage1_NullDM;
  TH1F *h1_TauEta_Stage1_OneProngDM;
  TH1F *h1_TauEta_Stage1_TwoProngDM;
  TH1F *h1_TauEta_Stage1_ThreeProngDM;
  TH1F *h1_TauEta_Stage1_RareDM;
  //
  TH1F *h1_MET_Stage1_NullDM;
  TH1F *h1_MET_Stage1_OneProngDM;
  TH1F *h1_MET_Stage1_TwoProngDM;
  TH1F *h1_MET_Stage1_ThreeProngDM;
  TH1F *h1_MET_Stage1_RareDM;
  //
  TH1F *h1_TauPt_Stage1_NullDM;
  TH1F *h1_TauPt_Stage1_OneProngDM;
  TH1F *h1_TauPt_Stage1_TwoProngDM;
  TH1F *h1_TauPt_Stage1_ThreeProngDM;
  TH1F *h1_TauPt_Stage1_RareDM;
  //
  TH1F *h1_MT_Stage1;
  TH1F *h1_MT_Stage1_tauID_SF_other;

  TH1F *h1_MT_New_VLoose_Stage1;
  TH1F *h1_MT_New_Loose_Stage1;
  TH1F *h1_MT_New_Medium_Stage1;
  TH1F *h1_MT_New_Tight_Stage1;
  TH1F *h1_MT_New_VTight_Stage1;

  TH1F *h1_MT_noEleRej_Stage1;
  TH1F *h1_MT_noMuRej_Stage1;
  TH1F *h1_MT_Old_VLoose_Stage1;
  TH1F *h1_MT_Old_Medium_Stage1;
  TH1F *h1_MT_Old_Loose_Stage1;
  TH1F *h1_MT_SR2;
  TH1F *h1_MT_SR2_noEleRej;
  TH1F *h1_MT_SR2_noMuRej;
  TH1F *h1_MT_Old_Tight_Stage1;
  TH1F *h1_MT_Old_VTight_Stage1;

  TH1F *h1_MET_Stage1;
  TH1F *h1_MET_Stage4;
  TH1F *h1_MET_Stage1_tauID_SF_other;
  TH1F *h1_MET_phi_Stage1;
  TH1F *h1_MT_Stage1_metUncert_JetEnUp;
  TH1F *h1_MT_Stage1_metUncert_JetEnDown;
  TH1F *h1_MT_Stage1_metUncert_JetResUp;
  TH1F *h1_MT_Stage1_metUncert_JetResDown;
  TH1F *h1_MT_Stage1_metUncert_MuonEnUp;
  TH1F *h1_MT_Stage1_metUncert_MuonEnDown;
  TH1F *h1_MT_Stage1_metUncert_ElectronEnUp;
  TH1F *h1_MT_Stage1_metUncert_ElectronEnDown;
  TH1F *h1_MT_Stage1_metUncert_TauEnUp;
  TH1F *h1_MT_Stage1_metUncert_TauEnDown;
  TH1F *h1_MT_Stage1_metUncert_PhotonEnUp;
  TH1F *h1_MT_Stage1_metUncert_PhotonEnDown;
  TH1F *h1_MT_Stage1_metUncert_UnclusteredEnUp;
  TH1F *h1_MT_Stage1_metUncert_UnclusteredEnDown;
  TH1F *h1_MT_Stage1_TauScaleUp;
  TH1F *h1_MT_Stage1_TauScaleDown;
  TH1F *h1_MT_Stage1_pileupUncertUp;
  TH1F *h1_MT_Stage1_pileupUncertDown;
  TH1F *h1_MT_Stage1_pdfUncertUp;
  TH1F *h1_MT_Stage1_pdfUncertDown;
  TH1F *h1_MT_Stage1_kFactorUp;
  TH1F *h1_MT_Stage1_kFactorDown;

  TH1F *h1_MT_Stage1_TauIDSF_flat_Up;
  TH1F *h1_MT_Stage1_TauIDSF_flat_Down;

  TH1F *h1_MT_Stage1_TauIDSF_flat_Up_other;
  TH1F *h1_MT_Stage1_TauIDSF_flat_Down_other;
  TH1F *h1_MT_Stage1_TauIDSF_ptDep_Up;
  TH1F *h1_MT_Stage1_TauIDSF_ptDep_Down;

  TH1F *h1_MT_Stage1_TauIDSF_ptDep_Up5;
  TH1F *h1_MT_Stage1_TauIDSF_ptDep_Up50;
  TH1F *h1_MT_Stage1_TauIDSF_ptDep_Down50;

  TH1F *h1_MT_Stage1_TauIDSF_ptDep_Up50_other;
  TH1F *h1_MT_Stage1_TauIDSF_ptDep_Down50_other;

  TH1F *h1_MT_Stage1_TauELESFUp;
  TH1F *h1_MT_Stage1_TauELESFDown;
  TH1F *h1_MT_Stage1_trigSFUp;
  TH1F *h1_MT_Stage1_trigSFDown;
  //
  TH1F *h1_MT_Stage1_alphaUp;
  TH1F *h1_MT_Stage1_alphaDown;
  //
  // TH1F *h1_MET_Stage2;
  TH1F *h1_dphiTauMET_Stage2;
  TH1F *h1_pToverEtMiss_Stage3;
  TH1F *h1_recoVtx_NoPUWt;
  TH1F *h1_recoVtx_WithPUWt;
  //TauID
  //  TH1F *h1_TauID_decayModeFinding;
  //  TH1F *h1_TauID_byMediumIsolationMVArun2v1DBoldDMwLT;
  //  TH1F *h1_TauID_againstElectronLooseMVA6;
  //  TH1F *h1_TauID_againstMuonLoose3;
  
  //N-1 
  TH1F *h1_MT_passTauTrig;
  TH1F *h1_MT_passAllMETFilters;
  TH1F *h1_MT_leptonVeto;
  TH1F *h1_MT_nGoodTau;
  TH1F *h1_MT_met_val;
  TH1F *h1_MT_pTbyET;
  TH1F *h1_MT_dphi;

  //-- These 100 histograms needed for PDF uncertainty --//
  TH1F *h1_MT_Stage1_pdfWt[100];
  char *histname_MT = new char[60];
  int nbinMT=8000;
  int xlowMT=0;
  int xupMT=8000;
  //
  int Run;
  int LumiSc;
  double  final_wt_NOPU=1;
  double final_weight=1;
  double final_weight_tauID_SF_other=1;
  double final_weight_PUweight_UP=1;
  double final_weight_PUweight_DOWN=1;
  double final_weight_kfact_UP=1;
  double final_weight_kfact_DOWN=1;

  double final_weight_tauIDSF_flat_UP=1;
  double final_weight_tauIDSF_flat_DOWN=1;

  double final_weight_tauIDSF_flat_UP_other=1;
  double final_weight_tauIDSF_flat_DOWN_other=1;

  double final_weight_tauIDSF_ptDep_UP=1;
  double final_weight_tauIDSF_ptDep_DOWN=1;

  double final_weight_tauIDSF_ptDep_UP5=1;
  double final_weight_tauIDSF_ptDep_UP50=1;
  double final_weight_tauIDSF_ptDep_DOWN50=1;

  double final_weight_tauIDSF_ptDep_UP50_other=1;
  double final_weight_tauIDSF_ptDep_DOWN50_other=1;

  double final_weight_tauELESF_UP=1;
  double final_weight_tauELESF_DOWN=1;
  double final_weight_alpha_UP=1;
  double final_weight_alpha_DOWN=1;
  double final_weight_trigSF_UP=1;
  double final_weight_trigSF_DOWN=1;
  unsigned long Event;
  double dphi_tau_met;
  double pToverEtMiss;

  //  int num_PU_vertices;

  std::ofstream myfile;

};

//
// constructors and destructor
//
MiniAODAnalyzer::MiniAODAnalyzer(const edm::ParameterSet& iConfig):
  vtxToken_(consumes<reco::VertexCollection>(iConfig.getParameter<edm::InputTag>("vertices"))),
  tauToken_(consumes<pat::TauCollection>(iConfig.getParameter<edm::InputTag>("taus"))),
  muonToken_(consumes<pat::MuonCollection>(iConfig.getParameter<edm::InputTag>("muons"))),
  jetToken_(consumes<pat::JetCollection>(iConfig.getParameter<edm::InputTag>("jets"))),
  // electronsMiniAODToken_    = mayConsume<edm::View<reco::GsfElectron> >
  // (iConfig.getParameter<edm::InputTag>
  // ("electronsMiniAOD"));
  electronToken_(mayConsume<edm::View<reco::GsfElectron>>(iConfig.getParameter<edm::InputTag>("electrons"))),
  metToken_(consumes<pat::METCollection>(iConfig.getParameter<edm::InputTag>("met"))),
  metToken_reminiaod_(consumes<pat::METCollection>(iConfig.getParameter<edm::InputTag>("met_reminiaod"))),
  triggerBits_(consumes<edm::TriggerResults>(iConfig.getParameter<edm::InputTag>("bits"))),
  triggerBits_MET_(consumes<edm::TriggerResults>(iConfig.getParameter<edm::InputTag>("bits_MET"))),
  triggerObjects_(consumes<pat::TriggerObjectStandAloneCollection>(iConfig.getParameter<edm::InputTag>("objects"))),
  triggerPrescales_(consumes<pat::PackedTriggerPrescales>(iConfig.getParameter<edm::InputTag>("prescales"))),
  //  eleLooseIdMapToken_(consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("eleLooseIdMap"))),
  prunedGenToken_(consumes<edm::View<reco::GenParticle> >(iConfig.getParameter<edm::InputTag>("pruned"))),
  packedGenToken_(consumes<edm::View<pat::PackedGenParticle> >(iConfig.getParameter<edm::InputTag>("packed"))),
  genEventInfoProductTagToken_(consumes<GenEventInfoProduct>(iConfig.getParameter<edm::InputTag>("genEventInfoProductMiniAOD"))),
  puCollection_(consumes<std::vector<PileupSummaryInfo> >(iConfig.getParameter<edm::InputTag>("pileupCollection"))),
  BadChCandFilterToken_(consumes<bool>(iConfig.getParameter<edm::InputTag>("BadChargedCandidateFilter"))),
  BadPFMuonFilterToken_(consumes<bool>(iConfig.getParameter<edm::InputTag>("BadPFMuonFilter"))),
  eleIdMapToken_(consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("eleIdMap"))),
  LHEEventToken_( consumes<LHEEventProduct>( iConfig.getParameter<edm::InputTag>( "LHEEventTag" ))),
  //  LHERunInfoToken_( consumes<LHERunInfoProduct,edm::InRun> ( iConfig.getParameter<edm::InputTag>( "LHEEventTag" ))),
  LHERunInfoToken_( consumes<LHERunInfoProduct,edm::InRun> (edm::InputTag("externalLHEProducer"))),
  //  LHERunInfoToken_( consumes<LHERunInfoProduct,edm::InRun> (edm::InputTag("source"))),
  pdfName_(iConfig.getParameter<std::string>("pdfName")),
  tag_(iConfig.getUntrackedParameter<std::string>( "tag", "initrwgt" )),
  RunOnData(iConfig.getParameter<bool>("RunOnData_")),
  doTrees(iConfig.getParameter<bool>("doTrees_")),
  doFakeHist(iConfig.getParameter<bool>("doFakeHist_")),
  doPDFuncertainty(iConfig.getParameter<bool>("doPDFuncertainty_")),
  generatorName_(iConfig.getParameter<std::string>("generatorName")),
  debugLevel(iConfig.getParameter<int>("debugLevel_"))
{
   //now do what ever initialization is needed
  //usesResource("TFileService");
  pileupMC_ = iConfig.getParameter<std::string>("PileupMCFile") ;
  pileupData_ = iConfig.getParameter<std::string>("PileupDataFile") ;
  pileupData_UP_ = iConfig.getParameter<std::string>("PileupDataFile_UP") ;
  pileupData_DOWN_ = iConfig.getParameter<std::string>("PileupDataFile_DOWN") ;
  KFactorE_ = iConfig.getParameter<std::string>("KFactorE") ;
  KFactorMu_ = iConfig.getParameter<std::string>("KFactorMu") ;
  KFactorTau_ = iConfig.getParameter<std::string>("KFactorTau") ;
  sourceFileString=iConfig.getParameter<std::string>("sourceFileString");
  //QCDWeightE_ = iConfig.getParameter<std::string>("QCDWeightE") ;
  //QCDWeightMu_ = iConfig.getParameter<std::string>("QCDWeightMu") ;
  QCDWeightTau_ = iConfig.getParameter<std::string>("QCDWeightTau") ;
  sourceFileString=iConfig.getParameter<std::string>("sourceFileString");
  useReweighting=iConfig.getParameter<bool>("useReweighting");
  TFileDirectory histoDir = fs->mkdir("histoDir");
  TFileDirectory controlDir = fs->mkdir("controlDir");
  /////  if (isPowheg) lheString = "source" ;

  h1_EventCount = histoDir.make<TH1I>("eventCount", "EventCount", 10, 0, 10);
  h1_EventCount2 = histoDir.make<TH1I>("eventCount2", "EventCount2", 10, 0, 10);

  h1_EventCount_Stage1 = histoDir.make<TH1I>("eventCount_Stage1", "EventCount_Stage1", 10, 0, 10);
  h1_EventCount_Stage1_needEleSF = histoDir.make<TH1I>("eventCount_Stage1_needEleSF", "EventCount_Stage1_needEleSF", 10, 0, 10);
 
  h1_dr_tau_ele = histoDir.make<TH1F>("deltaR_tau_ele", "dR_tau_ele", 100, 0, 2);
  h1_dr_tau_mu = histoDir.make<TH1F>("deltaR_tau_mu", "dR_tau_mu", 100, 0, 2);

  h1_nGenTau = histoDir.make<TH1I>("nGenTau", "nGenTau", 5, -0.5, 4.5);
  h1_nGoodTau_Reco = histoDir.make<TH1I>("nGoodTauReco", "nGoodTauReco", 5, -0.5, 4.5);
  //h1_genMTT
  h1_genMTT = histoDir.make<TH1F>("Gen_MTT", "MTT_Gen", 2000, 0, 2000);
  //
  h1_TauPt_Gen_den = histoDir.make<TH1F>("tauPt_Gen_den", "TauPt_Gen_den", 1000, 0, 8000);
  h1_TauPt_Gen_den_reco = histoDir.make<TH1F>("tauPt_Gen_den_reco", "TauPt_Gen_den_reco", 1000, 0, 8000);
  h1_TauPt_Gen_den_decay = histoDir.make<TH1F>("tauPt_Gen_den_decay", "TauPt_Gen_den_decay", 1000, 0, 8000);
  h1_TauPt_Gen_den_decay_iso = histoDir.make<TH1F>("tauPt_Gen_den_decay_iso", "TauPt_Gen_den_decay_iso", 1000, 0, 8000);
  h1_TauPt_Gen_den_decay_iso_muon = histoDir.make<TH1F>("tauPt_Gen_den_decay_iso_muon", "TauPt_Gen_den_decay_iso_muon", 1000, 0, 8000);
  //
  h1_TauPt_Gen_num_tot = histoDir.make<TH1F>("tauPt_Gen_num_tot", "TauPt_Gen_num_tot", 1000, 0, 8000);
  h1_TauPt_Gen_num_reco = histoDir.make<TH1F>("tauPt_Gen_num_reco", "TauPt_Gen_num_reco", 1000, 0, 8000);
  h1_TauPt_Gen_num_decay = histoDir.make<TH1F>("tauPt_Gen_num_decay", "TauPt_Gen_num_decay", 1000, 0, 8000);
  h1_TauPt_Gen_num_decay_iso = histoDir.make<TH1F>("tauPt_Gen_num_decay_iso", "TauPt_Gen_num_decay_iso", 1000, 0, 8000);
  h1_TauPt_Gen_num_decay_iso_muon = histoDir.make<TH1F>("tauPt_Gen_num_decay_iso_muon", "TauPt_Gen_num_decay_iso_muon", 1000, 0, 8000);
  h1_TauPt_Gen_num_decay_iso_muon_ele = histoDir.make<TH1F>("tauPt_Gen_num_decay_iso_muon_ele", "TauPt_Gen_num_decay_iso_muon_ele", 1000, 0, 8000);
  //
  //  h1_TauPt_reco = histoDir.make<TH1F>("tauPt_reco", "TauPt_reco", 1000, 0, 4000);
  // h1_TauPt_goodreco = histoDir.make<TH1F>("tauPt_goodreco", "TauPt_goodreco", 1000, 0, 4000);
  h1_TauPt_TrigEff_Deno = histoDir.make<TH1F>("tauPt_TrigEff_Deno", "TauPt_TrigEff_Deno", 2000, 0, 2000);
  h1_TauPt_TrigEff_Num  = histoDir.make<TH1F>("tauPt_TrigEff_Num", "TauPt_TrigEff_Num", 2000, 0, 2000);
  h1_TauEta_TrigEff_Deno = histoDir.make<TH1F>("tauEta_TrigEff_Deno", "TauEta_TrigEff_Deno", 48, -2.4, 2.4);
  h1_TauEta_TrigEff_Num  = histoDir.make<TH1F>("tauEta_TrigEff_Num", "TauEta_TrigEff_Num", 48, -2.4, 2.4);
  h1_met_TrigEff_Deno = histoDir.make<TH1F>("met_TrigEff_Deno", "MET_TrigEff_Deno", 2000, 0, 2000);
  h1_met_TrigEff_Num  = histoDir.make<TH1F>("met_TrigEff_Num", "MET_TrigEff_Num", 2000, 0, 2000);
  //
  h1_TauPt_TrigEff_Sig_Deno = histoDir.make<TH1F>("tauPt_TrigEff_Sig_Deno", "TauPt_TrigEff_Sig_Deno", 6000, 0, 6000);
  h1_TauPt_TrigEff_Sig_Num  = histoDir.make<TH1F>("tauPt_TrigEff_Sig_Num", "TauPt_TrigEff_Sig_Num", 6000, 0, 6000);

  h1_TauEta_TrigEff_Sig_Deno = histoDir.make<TH1F>("tauEta_TrigEff_Sig_Deno", "TauEta_TrigEff_Sig_Deno", 48, -2.4, 2.4);
  h1_TauEta_TrigEff_Sig_Num  = histoDir.make<TH1F>("tauEta_TrigEff_Sig_Num", "TauEta_TrigEff_Sig_Num", 48, -2.4, 2.4);

  h1_met_TrigEff_Sig_Deno = histoDir.make<TH1F>("met_TrigEff_Sig_Deno", "MET_TrigEff_Sig_Deno", 6000, 0, 6000);
  h1_met_TrigEff_Sig_Num  = histoDir.make<TH1F>("met_TrigEff_Sig_Num", "MET_TrigEff_Sig_Num", 6000, 0, 6000);

  h1_MT_TrigEff_Sig_Deno = histoDir.make<TH1F>("MT_TrigEff_Sig_Deno", "mT_TrigEff_Sig_Deno", 8000, 0, 8000);
  h1_MT_TrigEff_Sig_Num  = histoDir.make<TH1F>("MT_TrigEff_Sig_Num",  "mT_TrigEff_Sig_Num", 8000, 0, 8000);

  //
  // h1_TauEta_reco = histoDir.make<TH1F>("tauEta_reco", "TauEta_reco", 48, -2.4, 2.4);
  // h1_TauEta_goodreco = histoDir.make<TH1F>("tauEta_goodreco", "TauEta_goodreco", 48, -2.4, 2.4);
  h1_TauPt_Stage1 = histoDir.make<TH1F>("tauPt_Stage1", "TauPt_Stage1", 1000, 0, 4000);
  h1_tauPt_CR = controlDir.make<TH1F>("tauPt_CR", "TauPt_CR", 1000, 0, 4000);
  h1_dimuonPt_CR = controlDir.make<TH1F>("dimuonPt_CR", "dimuonPt_CR", 1000, 0, 4000);
  h1_tauPtOverMet_CR = controlDir.make<TH1F>("tauPtOverMet_CR", "TauPtOverMet_CR", 1000, 0, 10);
  h1_dPhiTauMet_CR = controlDir.make<TH1F>("dPhiTauMet_CR", "DPhiTauMet_CR", 800, -4.0, 4.0);
  //
  h1_tauPt_CR2 = controlDir.make<TH1F>("tauPt_CR2", "TauPt_CR2", 1000, 0, 4000);
  h1_dimuonPt_CR2 = controlDir.make<TH1F>("dimuonPt_CR2", "dimuonPt_CR2", 1000, 0, 4000);
  h1_tauPtOverMet_CR2 = controlDir.make<TH1F>("tauPtOverMet_CR2", "TauPtOverMet_CR2", 1000, 0, 10);
  h1_dPhiTauMet_CR2 = controlDir.make<TH1F>("dPhiTauMet_CR2", "DPhiTauMet_CR2", 800, -4.0, 4.0);

  h1_tauPt_CR3 = controlDir.make<TH1F>("tauPt_CR3", "TauPt_CR3", 1000, 0, 4000);
  h1_dimuonPt_CR3 = controlDir.make<TH1F>("dimuonPt_CR3", "dimuonPt_CR3", 1000, 0, 4000);
  h1_tauPtOverMet_CR3 = controlDir.make<TH1F>("tauPtOverMet_CR3", "TauPtOverMet_CR3", 1000, 0, 10);
  h1_dPhiTauMet_CR3 = controlDir.make<TH1F>("dPhiTauMet_CR3", "DPhiTauMet_CR3", 800, -4.0, 4.0);

  h1_tauPt_CR4 = controlDir.make<TH1F>("tauPt_CR4", "TauPt_CR4", 1000, 0, 4000);
  h1_dimuonPt_CR4 = controlDir.make<TH1F>("dimuonPt_CR4", "dimuonPt_CR4", 1000, 0, 4000);
  h1_tauPtOverMet_CR4 = controlDir.make<TH1F>("tauPtOverMet_CR4", "TauPtOverMet_CR4", 1000, 0, 10);
  h1_dPhiTauMet_CR4 = controlDir.make<TH1F>("dPhiTauMet_CR4", "DPhiTauMet_CR4", 800, -4.0, 4.0);

  h1_tauPt_CR5 = controlDir.make<TH1F>("tauPt_CR5", "TauPt_CR5", 1000, 0, 4000);
  h1_dimuonPt_CR5 = controlDir.make<TH1F>("dimuonPt_CR5", "dimuonPt_CR5", 1000, 0, 4000);
  h1_tauPtOverMet_CR5 = controlDir.make<TH1F>("tauPtOverMet_CR5", "TauPtOverMet_CR5", 1000, 0, 10);
  h1_dPhiTauMet_CR5 = controlDir.make<TH1F>("dPhiTauMet_CR5", "DPhiTauMet_CR5", 800, -4.0, 4.0);

  h1_tauPt_CR6 = controlDir.make<TH1F>("tauPt_CR6", "TauPt_CR6", 1000, 0, 4000);
  h1_dimuonPt_CR6 = controlDir.make<TH1F>("dimuonPt_CR6", "dimuonPt_CR6", 1000, 0, 4000);
  h1_tauPtOverMet_CR6 = controlDir.make<TH1F>("tauPtOverMet_CR6", "TauPtOverMet_CR6", 1000, 0, 10);
  h1_dPhiTauMet_CR6 = controlDir.make<TH1F>("dPhiTauMet_CR6", "DPhiTauMet_CR6", 800, -4.0, 4.0);

  h1_IDfailEle_pt_Stage1  = histoDir.make<TH1F>("IDfailEle_pt_Stage1",  "IDfailEle_pt_Stage1",  400, 0, 4000);
  h1_IDfailEle_eta_Stage1 = histoDir.make<TH1F>("IDfailEle_eta_Stage1", "IDfailEle_eta_Stage1", 100,  -2.5, 2.5);

  h2_IDfailEle_eta_pt_Stage1 = histoDir.make<TH2F>("IDfailEle_eta_pt", "IDfailEle_eta_pt", 10, -2.5, 2.5, 400, 0, 4000) ;

  h2_taueta_dRtaumu = controlDir.make<TH2F>("tauEta_dRtaumu",   "tauEta_dRtaumu", 44, -2.2, 2.2, 500, 0, 5)  ;
  h2_taueta_dRtaumu_pT_less_80 = controlDir.make<TH2F>("tauEta_dRtaumu_pT_less_80",   "tauEta_dRtaumu_pT_less_80", 44, -2.2, 2.2, 500, 0, 5)  ;
  h2_taueta_dRtaumu_pT_80_140 = controlDir.make<TH2F>("tauEta_dRtaumu_pT_80_140",   "tauEta_dRtaumu_pT_80_140", 44, -2.2, 2.2, 500, 0, 5)  ;
  h2_taueta_dRtaumu_pT_above_140 = controlDir.make<TH2F>("tauEta_dRtaumu_pT_above_140",   "tauEta_dRtaumu_pT_above_140", 44, -2.2, 2.2, 500, 0, 5)  ;

  hprof_taueta_dRtaumu = controlDir.make<TProfile>("profile_tauEta_dRtaumu",   "prof_tauEta_dRtaumu", 44, -2.2, 2.2, 0, 5)  ;
  hprof_taueta_dRtaumu_pT_less_80 = controlDir.make<TProfile>("profile_tauEta_dRtaumu_pT_less_80",   "prof_tauEta_dRtaumu_pT_less_80", 44, -2.2, 2.2, 0, 5)  ;
  hprof_taueta_dRtaumu_pT_80_140 = controlDir.make<TProfile>("profile_tauEta_dRtaumu_pT_80_140",   "prof_tauEta_dRtaumu_pT_80_140", 44, -2.2, 2.2, 0, 5)  ;
  hprof_taueta_dRtaumu_pT_above_140 = controlDir.make<TProfile>("profile_tauEta_dRtaumu_pT_above_140",   "prof_tauEta_dRtaumu_pT_above_140", 44, -2.2, 2.2, 0, 5)  ;

  //
  h1_TauPt_Stage1_NullDM = histoDir.make<TH1F>("tauPt_Stage1_null", "TauPt_Stage1_null", 1000, 0, 4000);
  h1_TauPt_Stage1_OneProngDM = histoDir.make<TH1F>("tauPt_Stage1_1prong", "TauPt_Stage1_1prong", 1000, 0, 4000);
  h1_TauPt_Stage1_TwoProngDM = histoDir.make<TH1F>("tauPt_Stage1_2prong", "TauPt_Stage1_2prong", 1000, 0, 4000);
  h1_TauPt_Stage1_ThreeProngDM = histoDir.make<TH1F>("tauPt_Stage1_3prong", "TauPt_Stage1_3prong", 1000, 0, 4000);
  h1_TauPt_Stage1_RareDM = histoDir.make<TH1F>("tauPt_Stage1_rare", "TauPt_Stage1_rare", 1000, 0, 4000);
  //
  h1_TauCharge_Stage1 = histoDir.make<TH1F>("tauCharge_Stage1", "TauCharge_Stage1", 5, -2.5, 2.5);

  h1_TauCharge_CR = controlDir.make<TH1F>("tauCharge_CR", "TauCharge_CR", 5, -2.5, 2.5);
  h1_TauCharge_CR2 = controlDir.make<TH1F>("tauCharge_CR2", "TauCharge_CR2", 5, -2.5, 2.5);
  h1_TauCharge_CR3 = controlDir.make<TH1F>("tauCharge_CR3", "TauCharge_CR3", 5, -2.5, 2.5);
  h1_TauCharge_CR4 = controlDir.make<TH1F>("tauCharge_CR4", "TauCharge_CR4", 5, -2.5, 2.5);
  h1_TauCharge_CR5 = controlDir.make<TH1F>("tauCharge_CR5", "TauCharge_CR5", 5, -2.5, 2.5);
  h1_TauCharge_CR6 = controlDir.make<TH1F>("tauCharge_CR6", "TauCharge_CR6", 5, -2.5, 2.5);

  h1_TauDxy_Stage1 = histoDir.make<TH1F>("tauDxy_Stage1", "TauDxy_Stage1", 1000, 0, 1);
  h1_TauChIso_Stage1 = histoDir.make<TH1F>("tauChIso_Stage1", "TauChIso_Stage1", 200, 0, 20);
  h1_TauNeuIso_Stage1 = histoDir.make<TH1F>("tauNeuIso_Stage1", "TauNeuIso_Stage1", 500, 0, 50);
  h1_TauNTrack_Stage1 = histoDir.make<TH1F>("tauNTrack_Stage1", "TauNTrack_Stage1", 50, -0.5, 49.5);
  //
   h1_TauDxy_CR = controlDir.make<TH1F>("tauDxy_CR", "TauDxy_CR", 1000, 0, 1);
  h1_TauChIso_CR = controlDir.make<TH1F>("tauChIso_CR", "TauChIso_CR", 200, 0, 20);
  h1_TauNeuIso_CR = controlDir.make<TH1F>("tauNeuIso_CR", "TauNeuIso_CR", 500, 0, 50);
   h1_TauNTrack_CR = controlDir.make<TH1F>("tauNTrack_CR", "TauNTrack_CR", 50, -0.5, 49.5);

   h1_TauDxy_CR2 = controlDir.make<TH1F>("tauDxy_CR2", "TauDxy_CR2", 1000, 0, 1);
   h1_TauChIso_CR2 = controlDir.make<TH1F>("tauChIso_CR2", "TauChIso_CR2", 200, 0, 20);
   h1_TauNeuIso_CR2 = controlDir.make<TH1F>("tauNeuIso_CR2", "TauNeuIso_CR2", 500, 0, 50);
   h1_TauNTrack_CR2 = controlDir.make<TH1F>("tauNTrack_CR2", "TauNTrack_CR2", 50, -0.5, 49.5);

   h1_TauDxy_CR3 = controlDir.make<TH1F>("tauDxy_CR3", "TauDxy_CR3", 1000, 0, 1);
   h1_TauChIso_CR3 = controlDir.make<TH1F>("tauChIso_CR3", "TauChIso_CR3", 200, 0, 20);
   h1_TauNeuIso_CR3 = controlDir.make<TH1F>("tauNeuIso_CR3", "TauNeuIso_CR3", 500, 0, 50);
   h1_TauNTrack_CR3 = controlDir.make<TH1F>("tauNTrack_CR3", "TauNTrack_CR3", 50, -0.5, 49.5);

   h1_TauDxy_CR4 = controlDir.make<TH1F>("tauDxy_CR4", "TauDxy_CR4", 1000, 0, 1);
   h1_TauChIso_CR4 = controlDir.make<TH1F>("tauChIso_CR4", "TauChIso_CR4", 200, 0, 20);
   h1_TauNeuIso_CR4 = controlDir.make<TH1F>("tauNeuIso_CR4", "TauNeuIso_CR4", 500, 0, 50);
   h1_TauNTrack_CR4 = controlDir.make<TH1F>("tauNTrack_CR4", "TauNTrack_CR4", 50, -0.5, 49.5);

   h1_TauDxy_CR5 = controlDir.make<TH1F>("tauDxy_CR5", "TauDxy_CR5", 1000, 0, 1);
   h1_TauChIso_CR5 = controlDir.make<TH1F>("tauChIso_CR5", "TauChIso_CR5", 200, 0, 20);
   h1_TauNeuIso_CR5 = controlDir.make<TH1F>("tauNeuIso_CR5", "TauNeuIso_CR5", 500, 0, 50);
   h1_TauNTrack_CR5 = controlDir.make<TH1F>("tauNTrack_CR5", "TauNTrack_CR5", 50, -0.5, 49.5);

   h1_TauDxy_CR6 = controlDir.make<TH1F>("tauDxy_CR6", "TauDxy_CR6", 1000, 0, 1);
   h1_TauChIso_CR6 = controlDir.make<TH1F>("tauChIso_CR6", "TauChIso_CR6", 200, 0, 20);
   h1_TauNeuIso_CR6 = controlDir.make<TH1F>("tauNeuIso_CR6", "TauNeuIso_CR6", 500, 0, 50);
   h1_TauNTrack_CR6 = controlDir.make<TH1F>("tauNTrack_CR6", "TauNTrack_CR6", 50, -0.5, 49.5);

  //
  h1_TauEta_Stage1 = histoDir.make<TH1F>("tauEta_Stage1", "TauEta_Stage1", 480, -2.4, 2.4);
  h1_tauEta_CR = controlDir.make<TH1F>("tauEta_CR", "TauEta_CR", 480, -2.4, 2.4);
  h1_tauEta_CR2 = controlDir.make<TH1F>("tauEta_CR2", "TauEta_CR2", 480, -2.4, 2.4);
  h1_tauEta_CR3 = controlDir.make<TH1F>("tauEta_CR3", "TauEta_CR3", 480, -2.4, 2.4);
  h1_tauEta_CR4 = controlDir.make<TH1F>("tauEta_CR4", "TauEta_CR4", 480, -2.4, 2.4);
  h1_tauEta_CR5 = controlDir.make<TH1F>("tauEta_CR5", "TauEta_CR5", 480, -2.4, 2.4);
  h1_tauEta_CR6 = controlDir.make<TH1F>("tauEta_CR6", "TauEta_CR6", 480, -2.4, 2.4);

  h1_TauPhi_Stage1 = histoDir.make<TH1F>("tauPhi_Stage1", "TauPhi_Stage1", 800, -4.0, 4.0);
  h1_tauPhi_CR = controlDir.make<TH1F>("tauPhi_CR", "TauPhi_CR", 800, -4.0, 4.0);
  h1_tauPhi_CR2 = controlDir.make<TH1F>("tauPhi_CR2", "TauPhi_CR2", 800, -4.0, 4.0);
  h1_tauPhi_CR3 = controlDir.make<TH1F>("tauPhi_CR3", "TauPhi_CR3", 800, -4.0, 4.0);
  h1_tauPhi_CR4 = controlDir.make<TH1F>("tauPhi_CR4", "TauPhi_CR4", 800, -4.0, 4.0);
  h1_tauPhi_CR5 = controlDir.make<TH1F>("tauPhi_CR5", "TauPhi_CR5", 800, -4.0, 4.0);
  h1_tauPhi_CR6 = controlDir.make<TH1F>("tauPhi_CR6", "TauPhi_CR6", 800, -4.0, 4.0);

  //
  h1_TauPhi_Stage1_NullDM = histoDir.make<TH1F>("tauPhi_Stage1_null", "TauPhi_Stage1_null", 800, -4.0, 4.0);
  h1_TauPhi_Stage1_OneProngDM = histoDir.make<TH1F>("tauPhi_Stage1_1prong", "TauPhi_Stage1_1prong", 800, -4.0, 4.0);
  h1_TauPhi_Stage1_TwoProngDM = histoDir.make<TH1F>("tauPhi_Stage1_2prong", "TauPhi_Stage1_2prong", 800, -4.0, 4.0);
  h1_TauPhi_Stage1_ThreeProngDM = histoDir.make<TH1F>("tauPhi_Stage1_3prong", "TauPhi_Stage1_3prong", 800, -4.0, 4.0);
  h1_TauPhi_Stage1_RareDM = histoDir.make<TH1F>("tauPhi_Stage1_rare", "TauPhi_Stage1_rare", 800, -4.0, 4.0);
 
  /// N-1
  h1_MT_passTauTrig       = histoDir.make<TH1F>("MT_passTauTrig_1",       "MT_passTauTrig",       1600, 0, 8000); // 1
  h1_MT_passAllMETFilters = histoDir.make<TH1F>("MT_passAllMETFilters_2", "MT_passAllMETFilters", 1600, 0, 8000); // 2
  h1_MT_leptonVeto        = histoDir.make<TH1F>("MT_leptonVeto_3",        "MT_leptonVeto",        1600, 0, 8000); // 3
  h1_MT_nGoodTau          = histoDir.make<TH1F>("MT_nGoodTau_4",          "MT_nGoodTau",          1600, 0, 8000); // 4
  h1_MT_met_val           = histoDir.make<TH1F>("MT_met_val_5",           "MT_met_val",           1600, 0, 8000); // 5
  h1_MT_pTbyET            = histoDir.make<TH1F>("MT_pTbyET_6",            "MT_pTbyET",            1600, 0, 8000); // 6
  h1_MT_dphi              = histoDir.make<TH1F>("MT_dphi_7",              "MT_dphi",              1600, 0, 8000); // 7
  //
  h1_TauEta_Stage1_NullDM = histoDir.make<TH1F>("tauEta_Stage1_null", "TauEta_Stage1_null", 480, -2.4, 2.4);
  h1_TauEta_Stage1_OneProngDM = histoDir.make<TH1F>("tauEta_Stage1_1prong", "TauEta_Stage1_1prong", 480, -2.4, 2.4);
  h1_TauEta_Stage1_TwoProngDM = histoDir.make<TH1F>("tauEta_Stage1_2prong", "TauEta_Stage1_2prong", 480, -2.4, 2.4);
  h1_TauEta_Stage1_ThreeProngDM = histoDir.make<TH1F>("tauEta_Stage1_3prong", "TauEta_Stage1_3prong", 480, -2.4, 2.4);
  h1_TauEta_Stage1_RareDM = histoDir.make<TH1F>("tauEta_Stage1_rare", "TauEta_Stage1_rare", 480, -2.4, 2.4);
  ///
  h1_MT_Stage1_NullDM = histoDir.make<TH1F>("mT_Stage1_null", "MT_Stage1_null", nbinMT, xlowMT, xupMT);
  h1_MT_Stage1_OneProngDM = histoDir.make<TH1F>("mT_Stage1_1prong", "MT_Stage1_1prong", nbinMT, xlowMT, xupMT);
  h1_MT_Stage1_TwoProngDM = histoDir.make<TH1F>("mT_Stage1_2prong", "MT_Stage1_2prong", nbinMT, xlowMT, xupMT);
  h1_MT_Stage1_ThreeProngDM = histoDir.make<TH1F>("mT_Stage1_3prong", "MT_Stage1_3prong", nbinMT, xlowMT, xupMT);
  h1_MT_Stage1_RareDM = histoDir.make<TH1F>("mT_Stage1_rare", "MT_Stage1_rare", nbinMT, xlowMT, xupMT);
  ///
  h1_MET_Stage1_NullDM = histoDir.make<TH1F>("met_Stage1_null", "MET_Stage1_null", nbinMT, xlowMT, xupMT);
  h1_MET_Stage1_OneProngDM = histoDir.make<TH1F>("met_Stage1_1prong", "MET_Stage1_1prong", nbinMT, xlowMT, xupMT);
  h1_MET_Stage1_TwoProngDM = histoDir.make<TH1F>("met_Stage1_2prong", "MET_Stage1_2prong", nbinMT, xlowMT, xupMT);
  h1_MET_Stage1_ThreeProngDM = histoDir.make<TH1F>("met_Stage1_3prong", "MET_Stage1_3prong", nbinMT, xlowMT, xupMT);
  h1_MET_Stage1_RareDM = histoDir.make<TH1F>("met_Stage1_rare", "MET_Stage1_rare", nbinMT, xlowMT, xupMT);
  //
  h1_MT_CR = controlDir.make<TH1F>("mT_CR", "MT_CR", nbinMT, xlowMT, xupMT);
  h1_MT_CR2 = controlDir.make<TH1F>("mT_CR2", "MT_CR2", nbinMT, xlowMT, xupMT);
  h1_MT_CR3 = controlDir.make<TH1F>("mT_CR3", "MT_CR3", nbinMT, xlowMT, xupMT);
  h1_MT_CR4 = controlDir.make<TH1F>("mT_CR4", "MT_CR4", nbinMT, xlowMT, xupMT);
  h1_MT_CR5 = controlDir.make<TH1F>("mT_CR5", "MT_CR5", nbinMT, xlowMT, xupMT);
  h1_MT_CR6 = controlDir.make<TH1F>("mT_CR6", "MT_CR6", nbinMT, xlowMT, xupMT);
  h1_MT_Stage1 = histoDir.make<TH1F>("mT_Stage1", "MT_Stage1", nbinMT, xlowMT, xupMT);
  h1_MT_Stage1_tauID_SF_other = histoDir.make<TH1F>("mT_Stage1_tauID_SF_other", "MT_Stage1_tauID_SF_other", nbinMT, xlowMT, xupMT);
  //
  h1_MT_New_VLoose_Stage1 = histoDir.make<TH1F>("mT_New_VLoose_Stage1", "MT_Stage1_New_VLoose", nbinMT, xlowMT, xupMT);
  h1_MT_New_Loose_Stage1 = histoDir.make<TH1F>("mT_New_Loose_Stage1", "MT_Stage1_New_Loose", nbinMT, xlowMT, xupMT);
  h1_MT_New_Medium_Stage1 = histoDir.make<TH1F>("mT_New_Medium_Stage1", "MT_Stage1_New_Medium", nbinMT, xlowMT, xupMT);
  h1_MT_New_Tight_Stage1 = histoDir.make<TH1F>("mT_New_Tight_Stage1", "MT_Stage1_New_Tight", nbinMT, xlowMT, xupMT);
  h1_MT_New_VTight_Stage1 = histoDir.make<TH1F>("mT_New_VTight_Stage1", "MT_Stage1_New_VTight", nbinMT, xlowMT, xupMT);
  //
  //
  h1_MT_Old_Medium_Stage1 = histoDir.make<TH1F>("mT_Old_Medium_Stage1", "MT_Stage1_Old_Medium", nbinMT, xlowMT, xupMT);
  h1_MT_Old_VLoose_Stage1 = histoDir.make<TH1F>("mT_Old_VLoose_Stage1", "MT_Stage1_Old_VLoose", nbinMT, xlowMT, xupMT);
  h1_MT_noEleRej_Stage1 = histoDir.make<TH1F>("mT_noEleRej_Stage1", "MT_Stage1_noEleRej", nbinMT, xlowMT, xupMT);
  h1_MT_noMuRej_Stage1 = histoDir.make<TH1F>("mT_noMuRej_Stage1", "MT_Stage1_noMuRej", nbinMT, xlowMT, xupMT);
  h1_MT_Old_Loose_Stage1 = histoDir.make<TH1F>("mT_Old_Loose_Stage1", "MT_Stage1_Old_Loose", nbinMT, xlowMT, xupMT);
  h1_MT_SR2 = histoDir.make<TH1F>("mT_SR2", "MT_SR2", nbinMT, xlowMT, xupMT);
  h1_MT_SR2_noEleRej = histoDir.make<TH1F>("mT_SR2_noEleRej", "MT_SR2_noEleRej", nbinMT, xlowMT, xupMT);
  h1_MT_SR2_noMuRej = histoDir.make<TH1F>("mT_SR2_noMuRej", "MT_SR2_noMuRej", nbinMT, xlowMT, xupMT);
  h1_MT_Old_Tight_Stage1 = histoDir.make<TH1F>("mT_Old_Tight_Stage1", "MT_Stage1_Old_Tight", nbinMT, xlowMT, xupMT);
  h1_MT_Old_VTight_Stage1 = histoDir.make<TH1F>("mT_Old_VTight_Stage1", "MT_Stage1_Old_VTight", nbinMT, xlowMT, xupMT);
  //
  h1_dimuonMass_CR = controlDir.make<TH1F>("diMuMass", "mumu_mass", 400, 0, 200);
  h1_recoil_CR=controlDir.make<TH1F>("recoil_CR", "hist_recoil_CR", nbinMT, xlowMT, xupMT);
  h1_original_MET_CR = controlDir.make<TH1F>("original_MET_CR", "MET_original_CR", nbinMT, xlowMT, xupMT);
  //
  h1_dimuonMass_CR2 = controlDir.make<TH1F>("diMuMass2", "mumu_mass2", 400, 0, 200);
  h1_recoil_CR2 = controlDir.make<TH1F>("recoil_CR2", "hist_recoil_CR2", nbinMT, xlowMT, xupMT);
  h1_original_MET_CR2 = controlDir.make<TH1F>("original_MET_CR2", "MET_original_CR2", nbinMT, xlowMT, xupMT);

  h1_dimuonMass_CR3 = controlDir.make<TH1F>("diMuMass3", "mumu_mass3", 400, 0, 200);
  h1_recoil_CR3 = controlDir.make<TH1F>("recoil_CR3", "hist_recoil_CR3", nbinMT, xlowMT, xupMT);
  h1_original_MET_CR3 = controlDir.make<TH1F>("original_MET_CR3", "MET_original_CR3", nbinMT, xlowMT, xupMT);

  h1_dimuonMass_CR4 = controlDir.make<TH1F>("diMuMass4", "mumu_mass4", 400, 0, 200);
  h1_recoil_CR4 = controlDir.make<TH1F>("recoil_CR4", "hist_recoil_CR4", nbinMT, xlowMT, xupMT);
  h1_original_MET_CR4 = controlDir.make<TH1F>("original_MET_CR4", "MET_original_CR4", nbinMT, xlowMT, xupMT);

  h1_dimuonMass_CR5 = controlDir.make<TH1F>("diMuMass5", "mumu_mass5", 400, 0, 200);
  h1_recoil_CR5 = controlDir.make<TH1F>("recoil_CR5", "hist_recoil_CR5", nbinMT, xlowMT, xupMT);
  h1_original_MET_CR5 = controlDir.make<TH1F>("original_MET_CR5", "MET_original_CR5", nbinMT, xlowMT, xupMT);

  h1_dimuonMass_CR6 = controlDir.make<TH1F>("diMuMass6", "mumu_mass6", 400, 0, 200);
  h1_recoil_CR6 = controlDir.make<TH1F>("recoil_CR6", "hist_recoil_CR6", nbinMT, xlowMT, xupMT);
  h1_original_MET_CR6 = controlDir.make<TH1F>("original_MET_CR6", "MET_original_CR6", nbinMT, xlowMT, xupMT);

  //
  h1_MET_Stage1 = histoDir.make<TH1F>("met_Stage1", "MET_Stage1", nbinMT, xlowMT, xupMT);
 h1_MET_Stage4 = histoDir.make<TH1F>("met_Stage4", "MET_Stage4", nbinMT, xlowMT, xupMT);
  h1_MET_phi_Stage1 = histoDir.make<TH1F>("met_phi_stage1", "MET_phi_Stage1", 800, -4.0, 4.0);
  //h1_MET_Stage1

  h1_MT_Stage1_metUncert_JetEnUp = histoDir.make<TH1F>("mT_Stage1_metUncert_JetEnUp", "MT_Stage1_metUncert_JetEnUp", nbinMT, xlowMT, xupMT);
  h1_MT_Stage1_metUncert_JetEnDown = histoDir.make<TH1F>("mT_Stage1_metUncert_JetEnDown", "MT_Stage1_metUncert_JetEnDown", nbinMT, xlowMT, xupMT);
  h1_MT_Stage1_metUncert_JetResUp = histoDir.make<TH1F>("mT_Stage1_metUncert_JetResUp", "MT_Stage1_metUncert_JetResUp", nbinMT, xlowMT, xupMT);
  h1_MT_Stage1_metUncert_JetResDown = histoDir.make<TH1F>("mT_Stage1_metUncert_JetResDown", "MT_Stage1_metUncert_JetResDown", nbinMT, xlowMT, xupMT);
  h1_MT_Stage1_metUncert_MuonEnUp = histoDir.make<TH1F>("mT_Stage1_metUncert_MuonEnUp", "MT_Stage1_metUncert_MuonEnUp", nbinMT, xlowMT, xupMT);
  h1_MT_Stage1_metUncert_MuonEnDown = histoDir.make<TH1F>("mT_Stage1_metUncert_MuonEnDown", "MT_Stage1_metUncert_MuonEnDown", nbinMT, xlowMT, xupMT);
  h1_MT_Stage1_metUncert_ElectronEnUp = histoDir.make<TH1F>("mT_Stage1_metUncert_ElectronEnUp", "MT_Stage1_metUncert_ElectronEnUp", nbinMT, xlowMT, xupMT);
  h1_MT_Stage1_metUncert_ElectronEnDown = histoDir.make<TH1F>("mT_Stage1_metUncert_ElectronEnDown", "MT_Stage1_metUncert_ElectronEnDown", nbinMT, xlowMT, xupMT);
  h1_MT_Stage1_metUncert_TauEnUp = histoDir.make<TH1F>("mT_Stage1_metUncert_TauEnUp", "MT_Stage1_metUncert_TauEnUp", nbinMT, xlowMT, xupMT);
  h1_MT_Stage1_metUncert_TauEnDown = histoDir.make<TH1F>("mT_Stage1_metUncert_TauEnDown", "MT_Stage1_metUncert_TauEnDown", nbinMT, xlowMT, xupMT);
  h1_MT_Stage1_metUncert_PhotonEnUp = histoDir.make<TH1F>("mT_Stage1_metUncert_PhotonEnUp", "MT_Stage1_metUncert_PhotonEnUp", nbinMT, xlowMT, xupMT);
  h1_MT_Stage1_metUncert_PhotonEnDown = histoDir.make<TH1F>("mT_Stage1_metUncert_PhotonEnDown", "MT_Stage1_metUncert_PhotonEnDown", nbinMT, xlowMT, xupMT);
  h1_MT_Stage1_metUncert_UnclusteredEnUp = histoDir.make<TH1F>("mT_Stage1_metUncert_UnclusteredEnUp", "MT_Stage1_metUncert_UnclusteredEnUp", nbinMT, xlowMT, xupMT);
  h1_MT_Stage1_metUncert_UnclusteredEnDown = histoDir.make<TH1F>("mT_Stage1_metUncert_UnclusteredEnDown", "MT_Stage1_metUncert_UnclusteredEnDown", nbinMT, xlowMT, xupMT);
  h1_MT_Stage1_TauScaleUp = histoDir.make<TH1F>("mT_Stage1_TauScaleUp", "MT_Stage1_TauScaleUp", nbinMT, xlowMT, xupMT);
  h1_MT_Stage1_TauScaleDown = histoDir.make<TH1F>("mT_Stage1_TauScaleDown", "MT_Stage1_TauScaleDown", nbinMT, xlowMT, xupMT);
  h1_MT_Stage1_pileupUncertUp = histoDir.make<TH1F>("mT_Stage1_pileupUncertUp", "MT_Stage1_pileupUncertUp", nbinMT, xlowMT, xupMT);
  h1_MT_Stage1_pileupUncertDown =histoDir.make<TH1F>("mT_Stage1_pileupUncertDown", "MT_Stage1_pileupUncertDown", nbinMT, xlowMT, xupMT);
  h1_MT_Stage1_kFactorUp =histoDir.make<TH1F>("mT_Stage1_kFactorUp", "MT_Stage1_kFactorUp", nbinMT, xlowMT, xupMT);
  h1_MT_Stage1_kFactorDown =histoDir.make<TH1F>("mT_Stage1_kFactorDown", "MT_Stage1_kFactorDown", nbinMT, xlowMT, xupMT);
  //
  h1_MT_Stage1_TauIDSF_flat_Up =histoDir.make<TH1F>("mT_Stage1_TauIDSF_flat_Up", "MT_Stage1_TauIDSF_flat_Up", nbinMT, xlowMT, xupMT);
  h1_MT_Stage1_TauIDSF_flat_Down =histoDir.make<TH1F>("mT_Stage1_TauIDSF_flat_Down", "MT_Stage1_TauIDSF_flat_Down", nbinMT, xlowMT, xupMT);
  //
  h1_MT_Stage1_TauIDSF_flat_Up_other =histoDir.make<TH1F>("mT_Stage1_TauIDSF_flat_Up_other", "MT_Stage1_TauIDSF_flat_Up_other", nbinMT, xlowMT, xupMT);
  h1_MT_Stage1_TauIDSF_flat_Down_other =histoDir.make<TH1F>("mT_Stage1_TauIDSF_flat_Down_other", "MT_Stage1_TauIDSF_flat_Down_other", nbinMT, xlowMT, xupMT);

  h1_MT_Stage1_TauIDSF_ptDep_Up =histoDir.make<TH1F>("mT_Stage1_TauIDSF_ptDep_Up", "MT_Stage1_TauIDSF_ptDep_Up", nbinMT, xlowMT, xupMT);
  h1_MT_Stage1_TauIDSF_ptDep_Down =histoDir.make<TH1F>("mT_Stage1_TauIDSF_ptDep_Down", "MT_Stage1_TauIDSF_ptDep_Down", nbinMT, xlowMT, xupMT);

  h1_MT_Stage1_TauIDSF_ptDep_Up5 =histoDir.make<TH1F>("mT_Stage1_TauIDSF_ptDep_Up5", "MT_Stage1_TauIDSF_ptDep_Up5", nbinMT, xlowMT, xupMT);
  h1_MT_Stage1_TauIDSF_ptDep_Up50 =histoDir.make<TH1F>("mT_Stage1_TauIDSF_ptDep_Up50", "MT_Stage1_TauIDSF_ptDep_Up50", nbinMT, xlowMT, xupMT);
  h1_MT_Stage1_TauIDSF_ptDep_Down50 =histoDir.make<TH1F>("mT_Stage1_TauIDSF_ptDep_Down50", "MT_Stage1_TauIDSF_ptDep_Down50", nbinMT, xlowMT, xupMT);
  //
  h1_MT_Stage1_TauIDSF_ptDep_Up50_other =histoDir.make<TH1F>("mT_Stage1_TauIDSF_ptDep_Up50_other", "MT_Stage1_TauIDSF_ptDep_Up50_other", nbinMT, xlowMT, xupMT);
  h1_MT_Stage1_TauIDSF_ptDep_Down50_other =histoDir.make<TH1F>("mT_Stage1_TauIDSF_ptDep_Down50_other", "MT_Stage1_TauIDSF_ptDep_Down50_other", nbinMT, xlowMT, xupMT);
  //

  h1_MT_Stage1_TauELESFUp =histoDir.make<TH1F>("mT_Stage1_TauELESFUp", "MT_Stage1_TauELESFUp", nbinMT, xlowMT, xupMT);
  h1_MT_Stage1_TauELESFDown =histoDir.make<TH1F>("mT_Stage1_TauELESFDown", "MT_Stage1_TauELESFDown", nbinMT, xlowMT, xupMT);
  //  TH1F *h1_MT_Stage1_trigSFUp;
  //   TH1F *h1_MT_Stage1_trigSFDown;
  h1_MT_Stage1_trigSFUp =histoDir.make<TH1F>("mT_Stage1_trigSFUp", "MT_Stage1_trigSFUp", nbinMT, xlowMT, xupMT);
  h1_MT_Stage1_trigSFDown =histoDir.make<TH1F>("mT_Stage1_trigSFDown", "MT_Stage1_trigSFDown", nbinMT, xlowMT, xupMT);
  //
  //  h1_TauID_decayModeFinding = histoDir.make<TH1F>("decayModeFinding", "TauID_decayModeFinding", 30, -1, 2);
  //  h1_TauID_byMediumIsolationMVArun2v1DBoldDMwLT = histoDir.make<TH1F>("byMediumIsolationMVArun2v1DBoldDMwLT", "TauID_byMediumIsolationMVArun2v1DBoldDMwLT", 30, -1, 2);
  //  h1_TauID_againstElectronLooseMVA6 = histoDir.make<TH1F>("againstElectronLooseMVA6", "TauID_againstElectronLooseMVA6", 30, -1, 2);
  //  h1_TauID_againstMuonLoose3 = histoDir.make<TH1F>("againstMuonLoose3", "TauID_againstMuonLoose3", 30, -1, 2);
 
  //
  h1_MT_Stage1_alphaUp   = histoDir.make<TH1F>("mT_Stage1_alphaUp",   "MT_Stage1_alphaUp",   nbinMT, xlowMT, xupMT);
  h1_MT_Stage1_alphaDown = histoDir.make<TH1F>("mT_Stage1_alphaDown", "MT_Stage1_alphaDown", nbinMT, xlowMT, xupMT);

  h1_dphiTauMET_Stage2   = histoDir.make<TH1F>("dphi_tau_MET_Stage2", "DPhi_Tau_MET_Stage2", 800, -4.0, 4.0);
  h1_pToverEtMiss_Stage3 = histoDir.make<TH1F>("pToverEtMiss_Stage3", "PToverEtMiss_Stage3", 300, 0.0, 3.0);

  h1_dPhi_tau_MET_Stage1 = histoDir.make<TH1F>("dPhi_tau_MET_Stage1", "DPhi_Tau_MET_Stage1", 800, -4.0, 4.0);
  h1_pToverEtMiss_Stage1 = histoDir.make<TH1F>("pToverEtMiss_Stage1", "PToverEtMiss_Stage1", 300, 0, 3.0);

  if ( doPDFuncertainty ) {
    h1_MT_Stage1_pdfUncertUp = histoDir.make<TH1F>("mT_Stage1_pdfUncertUp", "MT_Stage1_pdfUncertUp", nbinMT, xlowMT, xupMT);
    h1_MT_Stage1_pdfUncertDown =histoDir.make<TH1F>("mT_Stage1_pdfUncertDown", "MT_Stage1_pdfUncertDown", nbinMT, xlowMT, xupMT);
  }

  ///qcd histos
  int nstages=8;
  if (doFakeHist) {
    helper->CreateHisto(nstages,"Tau_fake_pt",  8000, 0, 8000, "p_{T} [GeV]");
    helper->CreateHisto(nstages,"Tau_nofake_pt",  8000, 0, 8000, "p_{T} [GeV]");
    helper->CreateHisto("Tau_fake_pt_decay", 8000, 0, 8000, 20, 0, 20, "p_{T} [GeV]", "decay mode");
    helper->CreateHisto("Tau_nofake_pt_decay", 8000, 0, 8000, 20, 0, 20, "p_{T} [GeV]", "decay mode");
    helper->CreateHisto("Tau_fake_pt_eta", 8000, 0, 8000, 20, -5, 5, "p_{T} [GeV]", "eta");
    helper->CreateHisto("Tau_nofake_pt_eta", 8000, 0, 8000, 20, -5, 5, "p_{T} [GeV]", "eta");
    //Tau_nofake_pt_met = testDir.make<TH2D>("Tau_nofake_pt_met", "Tau_nofake_pt_met", 8000, 0, 8000, 8000, 0, 8000 );
    //Tau_fake_pt_met = testDir.make<TH2D>("Tau_fake_pt_met", "Tau_fake_pt_met", 8000, 0, 8000, 8000, 0, 8000 );
    helper->CreateHisto("Tau_fake_pt_met", 800, 0, 8000, 800, 0, 8000, "p_{T} [GeV]", "MET");
    helper->CreateHisto("Tau_nofake_pt_met", 800, 0, 8000, 800, 0, 8000, "p_{T} [GeV]", "MET");
    helper->CreateHisto(nstages,"Tau_fake_pt_true",  8000, 0, 8000, "p_{T} [GeV]");
    helper->CreateHisto(nstages,"Tau_nofake_pt_true",  8000, 0, 8000, "p_{T} [GeV]");
    helper->CreateHisto("Tau_fake_pt_decay_true", 8000, 0, 8000, 20, 0, 20, "p_{T} [GeV]", "decay mode");
    helper->CreateHisto("Tau_nofake_pt_decay_true", 8000, 0, 8000, 20, 0, 20, "p_{T} [GeV]", "decay mode");
    helper->CreateHisto("Tau_fake_pt_eta_true", 8000, 0, 8000, 20, -5, 5, "p_{T} [GeV]", "eta");
    helper->CreateHisto("Tau_nofake_pt_eta_true", 8000, 0, 8000, 20, -5, 5, "p_{T} [GeV]", "eta");
    //Tau_fake_pt_met_true = testDir.make<TH2D>("Tau_fake_pt_met_true", "Tau_fake_pt_met_true", 8000, 0, 8000, 8000, 0, 8000);
    //Tau_nofake_pt_met_true = testDir.make<TH2D>("Tau_nofake_pt_met_true", "Tau_nofake_pt_met_true", 8000, 0, 8000, 8000, 0, 8000);
    helper->CreateHisto("Tau_fake_pt_met_true", 800, 0, 8000, 800, 0, 8000, "p_{T} [GeV]", "MET");
    helper->CreateHisto("Tau_nofake_pt_met_true", 800, 0, 8000, 800, 0, 8000, "p_{T} [GeV]", "MET");
    if(not RunOnData){
      for(std::string gen : {"lightquark","gluon","bquark","lightlepton","unmatched"}){
    helper->CreateHisto(nstages,Form("Tau_fake_pt_%s",gen.c_str()),  8000, 0, 8000, "p_{T} [GeV]");
    helper->CreateHisto(nstages,Form("Tau_nofake_pt_%s",gen.c_str()),  8000, 0, 8000, "p_{T} [GeV]");
      }
    }
  }
  m_do_complicated_tau_stuff=false;


  //m_qcdweightFile_ele = new TFile( ,"READ");
  //qcd_weight_ele=(TH1D*) qcd_weight_ele.Get("qcdFake_pt");
  //qcd_weight_ele->SetName("eleFake");
  //m_qcdweightFile_muo = new TFile( ,"READ");
  //qcd_weight_muo=(TH1D*) qcd_weight_muo.Get("qcdFake_pt");
  //qcd_weight_muo->SetName("muoFake");
  m_qcdweightFile_tau = new TFile(QCDWeightTau_.c_str(),"READ");
  hist_qcd_weight_tau=(TH2D*) m_qcdweightFile_tau->Get("qcdFake_pt");
  hist_qcd_weight_tau->SetName("tauFake");

  ///k-factor
  m_kfactorFile_ele= new TFile(KFactorE_.c_str(),"READ");
  m_kfactorHist_ele[0] = (TH1F*) m_kfactorFile_ele->Get("k_fac_p");
  m_kfactorHist_ele[1] = (TH1F*) m_kfactorFile_ele->Get("k_fac_m");
  m_kfactorHist_ele[2] = (TH1F*) m_kfactorFile_ele->Get("k_fac_mean");

  m_kfactorFile_muo= new TFile(KFactorMu_.c_str(),"READ");
  m_kfactorHist_muo[0] = (TH1F*) m_kfactorFile_muo->Get("k_fac_p");
  m_kfactorHist_muo[1] = (TH1F*) m_kfactorFile_muo->Get("k_fac_m");
  m_kfactorHist_muo[2] = (TH1F*) m_kfactorFile_muo->Get("k_fac_mean");

  m_kfactorFile_tau= new TFile(KFactorTau_.c_str(),"READ");
  m_kfactorHist_tau[0] = (TH1F*) m_kfactorFile_tau->Get("k_fac_p");
  m_kfactorHist_tau[1] = (TH1F*) m_kfactorFile_tau->Get("k_fac_m");
  m_kfactorHist_tau[2] = (TH1F*) m_kfactorFile_tau->Get("k_fac_mean");

  // discriminators
  d_mydisc->clear();
  SetUpDisc();

  h1_recoVtx_NoPUWt = histoDir.make<TH1F>("recoVtx_NoPUWt", "RecoVtx_NoPUWt", 100, 0, 100);
  h1_recoVtx_WithPUWt = histoDir.make<TH1F>("recoVtx_WithPUWt", "RecoVtx_WithPUWt", 100, 0, 100);

  //
  if ( doPDFuncertainty ) {
    for (int i=0; i<100; i++) {
      sprintf(histname_MT,"mT_Stage1_pdfWt_%d",i);
      h1_MT_Stage1_pdfWt[i] = new TH1F(histname_MT,"",nbinMT, xlowMT, xupMT);
    }
  }
  if (!RunOnData) {
    LumiWeights_ = edm::LumiReWeighting(pileupMC_, pileupData_, "pileup", "pileup");
    LumiWeights_UP_ = edm::LumiReWeighting(pileupMC_, pileupData_UP_, "pileup", "pileup");
    LumiWeights_DOWN_ = edm::LumiReWeighting(pileupMC_, pileupData_DOWN_, "pileup", "pileup");
  }
}


MiniAODAnalyzer::~MiniAODAnalyzer()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
  delete  histname_MT;
  delete  m_kfactorFile_ele;
  delete  m_kfactorFile_tau;
  delete  m_kfactorFile_muo;
  delete  m_qcdweightFile_tau;
  delete  pdf_indices;
  delete  inpdfweights;
  delete  alpha_s_container;
  delete  d_mydisc;
  delete  d_mydisc_emu;
  delete  d_mydisc_mu;
  delete  qcd_lepton_ele;
  delete  qcd_weight_ele;
  delete  qcd_lepton_mu;
  delete  qcd_lepton_mu;
  delete  qcd_lepton_tau;
  delete  qcd_weight_tau;
  delete  EleIDPassed;
  delete  MuonIDPassed;
  delete  FakeCandPt;
  delete  FakeCandPhi;
  delete  FakeCandEta;
  delete  FakeCandMt;
  delete  FakeCandDeltaPhi;
  delete  FakeCandMET;
  delete  FakeCandMETPhi;
  delete  FakeCandIso;
  delete  FakeCandNoIso;
  delete  FakeCandVetoNum;
  delete  FakeCandDecayMode;
  delete  FakeCandLeptonType;
  delete  FakeCandLeptonMt;
  delete  FakeCandLeptonDeltaPhi;
  delete  FakeCandLeptonPt;
  delete  FakeCandLeptonEta;
  delete  FakeCandLeptonPhi;
  delete  FakeCandLeptonTrigger;
  delete  FakeCandLorentz;

  if ( doPDFuncertainty ) {
    for (unsigned int i=0; i<100; i++) {
      delete h1_MT_Stage1_pdfWt[i];
    }
  }
}

//
// member functions
//

void MiniAODAnalyzer::beginRun( edm::Run const &iRun, edm::EventSetup const &iSetup ) {
  pdf_indices->clear();
  alpha_indices.clear();
  
  if (!RunOnData ) {
    if ( doPDFuncertainty) {
      edm::Handle<LHERunInfoProduct> run;
      typedef std::vector<LHERunInfoProduct::Header>::const_iterator headers_const_iterator;
      iRun.getByLabel( lheString , run );
      std::cout << "LHERunInfoProduct available ?  " << run.isValid() << std::endl;
      if ( run.isValid()) {
	LHERunInfoProduct myLHERunInfoProduct = *(run.product());
        std::vector<std::string> weight_lines;
	for (headers_const_iterator iter=myLHERunInfoProduct.headers_begin(); iter!=myLHERunInfoProduct.headers_end(); iter++){
	  std::vector<std::string> lines = iter->lines();
	  if( ( iter->tag() ).compare( tag_ ) == 0 ) {
	    weight_lines = iter->lines();
	  }
	   if (debugLevel>3) {
	    for (unsigned int iLine = 0; iLine<lines.size(); iLine++) {
	      std::cout   << "LINE " << lines.at(iLine);
	    }
	    }
	}
	int pdfidx = 0;
	pdfidx = run->heprup().PDFSUP.first;
	if (generatorName_=="powheg" && pdfidx==-1) pdfidx=260000;
	std::cout << "This sample was generated with the following PDFs : "   << pdfidx <<   std::endl;
	pdfid_1 = boost::lexical_cast<std::string>(pdfidx + 1);
	pdfid_2 = boost::lexical_cast<std::string>(pdfidx + 100);
	std::cout << "PDFs min and max id for MC replicas: " << pdfid_1 << "   " << pdfid_2 << std::endl;
	
	//alpha_s
	// --- Get alphas id
	//pdf 263000 -> No alpha_s uncertainty
	if (pdfidx == 292200){
	  alphas_id_1 = "292301";
	  alphas_id_2 = "292302";
	}
	
	else if (pdfidx == 292000){
	  alphas_id_1 = "292101";
	  alphas_id_2 = "292102";
	}
	
	else if (pdfidx == 260000){
	  alphas_id_1 = "265000";
	  alphas_id_2 = "266000";
	}
	
	else if (pdfidx == 260400){
	  alphas_id_1 = "265400";
	  alphas_id_2 = "266400";
	}
	/*
	else {
	  alphas_id_1 = "0";
          alphas_id_2 = "0";
	  }*/
       	std::cout << "alpha_S min and max id  : " << alphas_id_1 << "   " << alphas_id_2 << std::endl;
	
	
	std::stringstream ss;
	std::copy(weight_lines.begin(), weight_lines.end(),std::ostream_iterator<std::string>(ss,""));
	boost::property_tree::ptree pt;
	read_xml( ss , pt);
	
	// --- Name of the weightgroup
	//  string scalevar = "scale_variation";
	std::string pdfvar="";
	if (generatorName_=="powheg") {
	  pdfvar = "PDF_variation";
	}
	else if (generatorName_=="madgraphMLM") {
	  pdfvar = pdfName_;
	}
	
	if( (generatorName_=="madgraphMLM") && (pdfidx==263000) && (pdfvar != "NNPDF30_lo_as_0130.LHgrid") )
	  throw cms::Exception("WrongPDFname")
	    << "Wrong pdf name provided. ID=" << pdfidx << " NAME=" << pdfvar   ;
	
	if( (generatorName_=="madgraphMLM") && (pdfidx != 263000) && (pdfvar == "NNPDF30_lo_as_0130.LHgrid") )
	  throw cms::Exception("WrongPDFname")
	    << "Wrong pdf name provided. ID=" << pdfidx << " NAME=" << pdfvar   ;
	//std::cout << "generatorName_=" << generatorName_ << " pdfvar=" << pdfvar << std::endl;
	
	BOOST_FOREACH( boost::property_tree::ptree::value_type const& v, pt.get_child("") ) {
	  //std::cout << "v.first=" << v.first  << std::endl;
	  
	  if (v.first == "weightgroup"){
	    boost::property_tree::ptree subtree = (boost::property_tree::ptree) v.second ;
	    
	    boost::optional<std::string> weightgroupname1 = v.second.get_optional<std::string>("<xmlattr>.name");
	    boost::optional<std::string> weightgroupname2 = v.second.get_optional<std::string>("<xmlattr>.type");
	    std::cout << "weightgroupname1=" << weightgroupname1 << " weightgroupname2=" << weightgroupname2 << std::endl;
	    if ( (weightgroupname1 && weightgroupname1.get() == pdfvar)  || (weightgroupname2 && weightgroupname2.get() == pdfvar)) {
	      BOOST_FOREACH(boost::property_tree::ptree::value_type &vs,subtree) {
	        // std::cout << "vs.first=" << vs.first << " vs.second="  << vs.second << std::endl;
		if (vs.first == "weight") {
		  //std::cout << vs.first <<  "   " << vs.second.get<std::string>("<xmlattr>.id")  << "  " << vs.second.data()<< std::endl;
		  std::string strwid  = vs.second.get<std::string>("<xmlattr>.id");
		  std::string strw    = vs.second.data();
		  int id = stoi(strwid);
		  std::vector<std::string> strs;
		  if (generatorName_=="madgraphMLM") boost::split(strs, strw, boost::is_any_of(" "));
		  if (generatorName_=="powheg") boost::split(strs, strw, boost::is_any_of("="));
		  int pdf_wt_index  = 999;
		  if (generatorName_=="powheg") {
		    pdf_wt_index = stoi(strs.back());
		  }
		  else if (generatorName_=="madgraphMLM") {
		    pdf_wt_index = pdfidx+ (stoi(strs.back())) +1 ;
		  }
		  // std::cout << "id=" << id  << "  pdf_wt_index = " << pdf_wt_index << std::endl;
		  if ( (pdf_wt_index >= stoi(pdfid_1) ) && (pdf_wt_index <= stoi(pdfid_2)) ){
		    pdf_indices->push_back( id );
		  }
		  
		  std::cout << "alphas_id_1.empty() = " << alphas_id_1.empty() << "  alphas_id_2.empty() = " << alphas_id_2.empty() << std::endl;
		  if ( !alphas_id_1.empty() && !alphas_id_2.empty() ) {
		    if (pdf_wt_index == stoi(alphas_id_1) || pdf_wt_index == stoi(alphas_id_2)){
		      std::cout << "push back " << id << " into alpha_indices " << std::endl;
		      alpha_indices.push_back( id );
		    }
		  }
		}
	      }
	    }
	  }
	}
      }
      else {
	if (debugLevel>3)   std::cout << "PDF weights not saved in CMSSW. Do post-facto reweighting" << std::endl;
      }
    }
  }
}

// ------------ method called for each event  ------------
void MiniAODAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  using namespace std;
  using namespace reco;
  using namespace pat;

  h1_EventCount->Fill(1);

  //---Clear---//
  Run=0;
  Event=0;
  LumiSc=0;
  EleIDPassed->clear();
  MuonIDPassed->clear();

  //------//
  Run   = iEvent.id().run();
  Event = iEvent.id().event();
  LumiSc = iEvent.luminosityBlock(); // iEvent.id().lumi();

  //  std::cout << "\n --EVENT-- " << Event  <<   "  lumisc=" << LumiSc << "  run=" << Run << std::endl;

  edm::Handle<LHEEventProduct> EvtHandle ;
  if  ( !(RunOnData) ) {
    iEvent.getByToken( LHEEventToken_ , EvtHandle ) ;
    // std::cout << "EvtHandle.isValid() = " << EvtHandle.isValid() << std::endl;
    //Gen-HT//
    if (EvtHandle.isValid() ) {
      //
      //      std::cout << "event handle valid" << std::endl;
      if (  (sourceFileString.find("WJetsToLNu") != std::string::npos) ) {
	lhef::HEPEUP lheParticleInfo = EvtHandle->hepeup();
        // get the five vector
        // (Px, Py, Pz, E and M in GeV)
	std::vector<lhef::HEPEUP::FiveVector> allParticles = lheParticleInfo.PUP;
	std::vector<int> statusCodes = lheParticleInfo.ISTUP;
	
	double ht = 0;
	for (unsigned int i = 0; i < statusCodes.size(); i++) {
	  if (statusCodes[i] == 1) {
	    if (abs(lheParticleInfo.IDUP[i]) < 11 || abs(lheParticleInfo.IDUP[i]) > 16 || abs(lheParticleInfo.IDUP[i]) > 22) {
	      //   std::cout << "particle ID=" << lheParticleInfo.IDUP[i] << " status=" << statusCodes[i] << std::endl;
	      ht += sqrt(pow(allParticles[i][0], 2) + pow(allParticles[i][1], 2));
	    }
	  }
	}
	genHT=ht;
	//std::cout << "genHT=" << genHT << std::endl;
      } // if (  (sourceFileString.find("WJetsToLNu") != std::string::npos) ) ends
      //
      //
      if (  (sourceFileString.find("TT_") != std::string::npos) ) {
	lhef::HEPEUP lheParticleInfo = EvtHandle->hepeup();
        // get the five vector
        // (Px, Py, Pz, E and M in GeV)
	std::vector<lhef::HEPEUP::FiveVector> allParticles = lheParticleInfo.PUP; // PUP = (Px, Py, Pz, E and M in GeV) for the particle
	std::vector<int> statusCodes = lheParticleInfo.ISTUP; // ISTUP = The status codes for the particle entries in this event. 
	//double MTT = 0;
	lhef::HEPEUP::FiveVector top;
	lhef::HEPEUP::FiveVector antitop;
	top[0]=top[1]=top[2]=top[3]=top[4]=0.0;
	antitop[0]=antitop[1]=antitop[2]=antitop[3]=antitop[4]=0.0;
	int ntop=0;
	int nantitop=0;
	//	std::cout << "statusCodes.size() = " << statusCodes.size()  << std::endl;
	for (unsigned int i = 0; i < statusCodes.size(); i++) {
	  //std::cout << "particle " << i <<  "statusCode=" << statusCodes[i] << ", pdgid=" << lheParticleInfo.IDUP[i] << std::endl;
	  if (statusCodes[i] == 2) {
	    if (lheParticleInfo.IDUP[i] == 6) { // IDUP = The PDG id's for the particle entries in this event.
	      //  std::cout << "ID=" << lheParticleInfo.IDUP[i] << " status=" << statusCodes[i] << std::endl;
	      top=allParticles[i];
	      ntop++;
	    }
	    if (lheParticleInfo.IDUP[i] == -6) { 
	      //   std::cout << "ID=" << lheParticleInfo.IDUP[i] << " status=" << statusCodes[i] << std::endl;
	      antitop=allParticles[i];
	      nantitop++;
	    }
	  }
	}
	//	std::cout << "ntop, nantitop = " << ntop << ", " << nantitop << std::endl; 
	if ( (ntop==1) && (nantitop==1) ) {
	  genMTT = sqrt( (top[4]*top[4])+(antitop[4]*antitop[4])+(2*top[3]*antitop[3])-2*( (top[0]*antitop[0]) + (top[1]*antitop[1]) + (top[2]*antitop[2]) ) );
	  /*
	  genMTT2 = (antitop[3]+top[3])*(antitop[3]+top[3])
	    - (antitop[0]+top[0])*(antitop[0]+top[0])
	    - (antitop[1]+top[1])*(antitop[1]+top[1])
	    - (antitop[2]+top[2])*(antitop[2]+top[2]); */
	}
	//	std::cout << "MTT=" << genMTT  << std::endl;
      }
    } // if (EvtHandle.isValid() ) ends
  }
  
  //-- kfactor --//
  wmass_stored=0;

  ///----///
  int nGenTau=0;
  int nGenTauJet=0;
  TLorentzVector tauGen_p4[10];
  const Candidate * MyTau;
  // const Candidate * MyTauSel=0;
  double TauPt_Gen=0;

  k_fak_stored=1;
  if (!RunOnData) {
    //--GenParticles--//
    iEvent.getByToken(prunedGenToken_,pruned);
    //    Handle<edm::View<pat::PackedGenParticle> > packed;
    //    iEvent.getByToken(packedGenToken_,packed);
    //   if (sourceFileString.find("WJetsToLNu_Tune") != std::string::npos) 
    //std::cout << " Wmass=" << getWmass(pruned) << std::endl;
    if ( Overlap(pruned, genHT, genMTT) ) {
      //HT=" << genHT << std::endl ;
      //   if (sourceFileString.find("WToTau") != std::string::npos) std::cout << "Reject! sample = " << sourceFileString << " Wmass=" << getWmass(pruned) << std::endl ;
      //   if (sourceFileString.find("TT_") != std::string::npos) std::cout << "Reject! sample = " << sourceFileString << " MTT" << genMTT << std::endl ;
 
      return;
    }

    h1_genMTT->Fill(genMTT);
    ///-- W k-factor --///
    k_fak_stored=applyWKfactor(1,pruned);
    if (k_fak_stored != 100) {
      k_fak = k_fak_stored;
      k_fak_up = k_fak_stored + (k_fak_stored*(5.0/100.0));
      k_fak_down = k_fak_stored - (k_fak_stored*(5.0/100.0));
      if (k_fak_down<0) k_fak_down=k_fak ;
      //  std::cout << "k-fact / up / down : " << k_fak_stored << " / " << k_fak_stored_up << " / " << k_fak_stored_down << std::endl;
    }

    for(size_t i=0; i<pruned->size();i++){
      if(   (abs((*pruned)[i].pdgId())==15) && ( ((*pruned)[i].status()==2) )) {
	bool leptonDecayMode=false;
	MyTau = &(*pruned)[i];
	//	std::cout << "Tau found. nTauDaughter = "  <<  MyTau->numberOfDaughters() << std::endl;
	int ndaughter=MyTau->numberOfDaughters();
	for (int n=0; n<ndaughter; n++) {
	  const Candidate *MyTauDaughter=MyTau->daughter(n);
	  // std::cout << "Tau Daughter pdgID = " << MyTauDaughter->pdgId() << " and status = " << MyTauDaughter->status()  << std::endl;
	  if (  (fabs(MyTauDaughter->pdgId())==11) ||  (fabs(MyTauDaughter->pdgId())==13) ) {
	    leptonDecayMode=true ; 
	    //std::cout << "Lepton decaymode detected" << std::endl;
	  }
	  /*
	  if (  fabs(MyTauDaughter->pdgId())==24 ) {
	    std::cout << "Tau decayed to W " ;
	    std::cout << " nWDaughter = "  <<  MyTauDaughter->numberOfDaughters() << std::endl;
	    int nWdaughter = MyTauDaughter->numberOfDaughters() ; //<< std::endl;
	    for (int n2=0; n2<nWdaughter; n2++) {
	      const Candidate *MyWDaughter=MyTauDaughter->daughter(n2);
	      std::cout << "W daughter PDG ID = " << MyWDaughter->pdgId() << " and status = " << MyWDaughter->status() << std::endl;
	    }
	  }
	  */
	}
	if ( (MyTau->pt()>80.0)  &&  (fabs(MyTau->eta())<2.1)  )  {
	  // MyTauSel=MyTau;
	  TauPt_Gen=MyTau->pt();
	  //// h1_TauPt_Gen->Fill(TauPt_Gen);
	  tauGen_p4[nGenTau].SetPxPyPzE(MyTau->px(),MyTau->py(),MyTau->pz(),MyTau->energy());
	  nGenTau++;
	  if (leptonDecayMode==false) nGenTauJet++;
	}
      }
    }
    //if(   (abs((*pruned)[i].pdgId())==16) && ( ((*pruned)[i].status()==2)) && (sel_tau==true)) {
    //MyNu= &(*pruned)[i]; //used for calculating the W mass
    //MyNu->px(); // to get rid of not use warning
    //}
  }
  //  std::cout << "nGenTau= " << nGenTau << "  nGenTauJet=" << nGenTauJet << std::endl;
  h1_nGenTau->Fill(nGenTau);


  ///--PDF weight--///
  //  edm::Handle<LHEEventProduct> EvtHandle ;
  if  ( !(RunOnData) ) {
    //std::cout << "EvtHandle.isValid() = " << EvtHandle.isValid() << " This is needed for pdf and alpha_s weights" << std::endl;
    // iEvent.getByToken( LHEEventToken_ , EvtHandle ) ;
    if  ( (EvtHandle.isValid()) ) {
      if ( doPDFuncertainty) {
	inpdfweights->clear();
	alpha_s_container->clear();
	for (unsigned int i=0; i<EvtHandle->weights().size(); i++) {
	  int id_i = stoi( EvtHandle->weights()[i].id );
	  for( unsigned int j = 0; j<pdf_indices->size(); j++ ) {
	    int id_j = pdf_indices->at(j);
	    if( id_i == id_j ){
	      float pdf_weight = (EvtHandle->weights()[i].wgt)/(EvtHandle->originalXWGTUP());
	      std::cout << "pdf_weight=" << pdf_weight  << std::endl;
	      inpdfweights->push_back( pdf_weight );
	    }
	  }
	  std::cout << "alpha_indices.empty() = " << alpha_indices.empty() << std::endl;
	  if ( !alpha_indices.empty() ) {
	    std::cout << "alpha_indices is not empty " << std::endl;
	    for( unsigned int k = 0; k < alpha_indices.size(); k++ ){
	      int id_k = alpha_indices[k];
	      if(id_i == id_k ){
		float alpha = EvtHandle->weights()[i].wgt;
		std::cout << "alpha_s wt = " << alpha << std::endl;
		alpha_s_container->push_back(alpha);
	      }
	    }
	  }
	}
      }
    }   /// Event Handle valid ?
  }
  
  
  //-- probValue --//
  //-- https://github.com/cms-sw/cmssw/blob/CMSSW_8_1_X/SimGeneral/MixingModule/python/mix_2016_25ns_SpringMC_PUScenarioV1_PoissonOOTPU_cfi.py --//
  //----------//
  //--PileUp--//
  //----------//
  int MC_TrueNumInteractions=-1;
  double Lumi_Wt=1;
  double Lumi_Wt_UP=1;
  double Lumi_Wt_DOWN=1;

  if (!RunOnData) {
    Handle<std::vector< PileupSummaryInfo > >  PupInfo;
    iEvent.getByToken(puCollection_, PupInfo);
    std::vector<PileupSummaryInfo>::const_iterator PVI;
    int Tnpv = -1;
    for(PVI = PupInfo->begin(); PVI != PupInfo->end(); ++PVI) {
      int BX = PVI->getBunchCrossing();
      // std::cout << " --PVI-- " << " BX=" << BX << std::endl;
      if(BX == 0) {
    Tnpv = PVI->getTrueNumInteractions();
    // std::cout << "Tnpv=" << Tnpv << std::endl;
    continue;
      }
    }
    MC_TrueNumInteractions = Tnpv;

    Lumi_Wt = LumiWeights_.weight( MC_TrueNumInteractions );
    Lumi_Wt_UP = LumiWeights_UP_.weight( MC_TrueNumInteractions );
    Lumi_Wt_DOWN = LumiWeights_DOWN_.weight( MC_TrueNumInteractions );
  }
  //  std::cout << "RunOnData=" << RunOnData << " MC_TrueNumInteractions=" << MC_TrueNumInteractions << " Lumi_Wt=" << Lumi_Wt << std::endl;

  ///-- MC event weight --///
  double mc_event_weight=-20;
  if (!RunOnData) {
    Handle<GenEventInfoProduct> genEvtInfo;
    iEvent.getByToken(genEventInfoProductTagToken_, genEvtInfo );
    mc_event_weight = genEvtInfo->weight();
  }

  if (RunOnData) {
    mc_event_weight=1.0;
  }
  //  std::cout << "RunOnData=" << RunOnData << " mc_event_weight=" << mc_event_weight << std::endl;


   //---Trigger---//
   edm::Handle<edm::TriggerResults> triggerBits;
   edm::Handle<pat::TriggerObjectStandAloneCollection> triggerObjects;
   edm::Handle<pat::PackedTriggerPrescales> triggerPrescales;

   iEvent.getByToken(triggerBits_, triggerBits);
   iEvent.getByToken(triggerObjects_, triggerObjects);
   iEvent.getByToken(triggerPrescales_, triggerPrescales);

   const edm::TriggerNames &names = iEvent.triggerNames(*triggerBits);
   bool passTauTrig=0;
   passMuonTrig=0;

   //   std::cout << "=== TRIGGER PATHS === " << std::endl;
   for (unsigned int i = 0, n = triggerBits->size(); i < n; ++i) {
     
     //         if (triggerBits->accept(i)==true ) { 
     //  std::cout << "Trigger " << names.triggerName(i) <<
     //	 ", prescale " << triggerPrescales->getPrescaleForIndex(i) <<
     // ": " << (triggerBits->accept(i) ? "PASS" : "fail (or not run)")
     //		 << std::endl;
     // }
     if ( (names.triggerName(i)).find("HLT_LooseIsoPFTau50_Trk30_eta2p1_MET90") != std::string::npos ) {
       //  std::cout << names.triggerName(i) << " "  << (triggerBits->accept(i) ? "PASS" : "fail (or not run)") << std::endl;
       passTauTrig=triggerBits->accept(i) ;
       // std::cout << "passTauTrig=" << passTauTrig << std::endl;
     }
     if ( 
	 //(names.triggerName(i)).find("HLT_IsoMu22_eta2p1") != std::string::npos or
	 //(names.triggerName(i)).find("HLT_IsoTkMu22_eta2p1") != std::string::npos or
	 //(names.triggerName(i)).find("HLT_IsoMu22") != std::string::npos or
	 //(names.triggerName(i)).find("HLT_IsoTkMu22") != std::string::npos or
	 //  (names.triggerName(i)).find("HLT_IsoMu24_v") != std::string::npos or
         // (names.triggerName(i)).find("HLT_IsoTkMu24_v") != std::string::npos or 
	 //  (names.triggerName(i)).find("HLT_IsoMu24_eta2p1_v") != std::string::npos or
	 //  (names.triggerName(i)).find("HLT_IsoTkMu24_eta2p1_v") != std::string::npos or
	 // (names.triggerName(i)).find("HLT_IsoMu27_v") != std::string::npos or
	 //  (names.triggerName(i)).find("HLT_IsoTkMu27_v") != std::string::npos or
          //(names.triggerName(i)).find("HLT_Mu45_eta2p1") != std::string::npos or
	 // (names.triggerName(i)).find("HLT_Mu50_v") != std::string::npos or
         // (names.triggerName(i)).find("HLT_TkMu50_v") != std::string::npos
	 (names.triggerName(i)).find("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v") != std::string::npos or
	 (names.triggerName(i)).find("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v") != std::string::npos
	  ) {
       passMuonTrig=triggerBits->accept(i) ;
       if (passMuonTrig==true) break;
     }
     //// Muon Trigger for Tau-trigger efficiency measurements ////
     if ( ((names.triggerName(i)).find("HLT_Mu30_TkMu11_v") != std::string::npos) or
          ((names.triggerName(i)).find("HLT_Mu40_TkMu11_v") != std::string::npos) or
          ((names.triggerName(i)).find("HLT_Mu45_eta2p1_v") != std::string::npos) or
	  ((names.triggerName(i)).find("HLT_Mu50_v") != std::string::npos) or
          ((names.triggerName(i)).find("HLT_IsoMu22_eta2p1_v") != std::string::npos) or
	  ((names.triggerName(i)).find("HLT_IsoMu22_v") != std::string::npos) or
	  ((names.triggerName(i)).find("HLT_IsoMu24_eta2p1_v") != std::string::npos) or
	  ((names.triggerName(i)).find("HLT_IsoMu24_v") != std::string::npos)   
     ) {
       //       std::cout << "Checking Muon trigger " << names.triggerName(i) << " Status " << triggerBits->accept(i)  <<  std::endl;
       passMuonTrig_ForEff=triggerBits->accept(i) ;
       if (passMuonTrig_ForEff==true) break;
     }
     if ( (names.triggerName(i)).find("HLT_Ele25_eta2p1_WPTight_Gsf") != std::string::npos or
          (names.triggerName(i)).find("HLT_Ele25_WPTight_Gsf") != std::string::npos or
          (names.triggerName(i)).find("HLT_Ele27_WPTight_Gsf") != std::string::npos or
	  (names.triggerName(i)).find("HLT_Ele27_eta2p1_WPTight_Gsf") != std::string::npos or
          (names.triggerName(i)).find("HLT_Ele30_eta2p1_WPTight_Gsf") != std::string::npos or
	  (names.triggerName(i)).find("HLT_Ele30_WPTight_Gsf") != std::string::npos or
          (names.triggerName(i)).find("HLT_Ele32_eta2p1_WPTight_Gsf") != std::string::npos or
	  (names.triggerName(i)).find("HLT_Ele32_WPTight_Gsf") != std::string::npos or
          (names.triggerName(i)).find("HLT_Ele105_CaloIdVT_GsfTrkIdT") != std::string::npos or
	  (names.triggerName(i)).find("HLT_Ele115_CaloIdVT_GsfTrkIdT") != std::string::npos or
          (names.triggerName(i)).find("HLT_Ele145_CaloIdVT_GsfTrkIdT") != std::string::npos or
          (names.triggerName(i)).find("HLT_Ele200_CaloIdVT_GsfTrkIdT") != std::string::npos
     ) {
       passEleTrig=triggerBits->accept(i) ;
       if (passEleTrig==true) break;
     }
   }

  if (!RunOnData) {
    trig_SF = 0.9;
    trig_SF_syst_up = 0.99;
    trig_SF_syst_down = 0.81;
   }

  //   if (!RunOnData) passTauTrig=1;
  //std::cout << "RunOnData=" << RunOnData <<  " ## passTauTrig=" << passTauTrig << std::endl;

   //---Trigger MET---//
   edm::Handle<edm::TriggerResults> triggerBits_MET;
   iEvent.getByToken(triggerBits_MET_, triggerBits_MET);
   const edm::TriggerNames &names_MET = iEvent.triggerNames(*triggerBits_MET);

   bool passAllMETFilters=0;
   bool passHBHENoiseFilter=0;
   bool passHBHENoiseIsoFilter=0;
   bool passEcalDeadCellTriggerPrimitiveFilter=0;
   bool passgoodVertices=0;
   bool passeeBadScFilter=0;
   bool passglobalTightHalo2016Filter=0;
   bool NobadMuons=0;
 
   //   std::cout << " === TRIGGER PATHS (MET) === " << std::endl;
   for (unsigned int i = 0, n = triggerBits_MET->size(); i < n; ++i) {
     // std::cout << "Trigger " << names_MET.triggerName(i) <<
     // ": " << (triggerBits_MET->accept(i) ? "PASS" : "fail (or not run)")
     //	       << std::endl;
     if ( (names_MET.triggerName(i)).find("Flag_HBHENoiseFilter") != std::string::npos ) {
       passHBHENoiseFilter=triggerBits_MET->accept(i) ;
     }
     if ( (names_MET.triggerName(i)).find("Flag_HBHENoiseIsoFilter") != std::string::npos ) {
       passHBHENoiseIsoFilter=triggerBits_MET->accept(i) ;
     }
     if ( (names_MET.triggerName(i)).find("Flag_EcalDeadCellTriggerPrimitiveFilter") != std::string::npos ) {
       passEcalDeadCellTriggerPrimitiveFilter=triggerBits_MET->accept(i) ;
     }
     if ( (names_MET.triggerName(i)).find("Flag_goodVertices") != std::string::npos ) {
       passgoodVertices=triggerBits_MET->accept(i) ;
     }
     if ( (names_MET.triggerName(i)).find("Flag_eeBadScFilter") != std::string::npos ) {
       passeeBadScFilter=triggerBits_MET->accept(i) ;
     }
     if ( (names_MET.triggerName(i)).find("Flag_globalTightHalo2016Filter") != std::string::npos ) {
       passglobalTightHalo2016Filter=triggerBits_MET->accept(i) ;
     }
     ////
     if (RunOnData) {
       if ( (names_MET.triggerName(i)).find("Flag_noBadMuons") != std::string::npos ) {
	 NobadMuons=triggerBits_MET->accept(i) ;
	 // std::cout << "noBadMuons = " << NobadMuons << std::endl;
       }
     } else if (!RunOnData) {
       NobadMuons=1;
     }
     ////
   }

   if (!RunOnData) passeeBadScFilter=1;

   //---MET FILTERS THAT ARE UNAVAILABLE IN MINIAOD AS FLAG---//
   edm::Handle<bool> ifilterbadChCand;
   iEvent.getByToken(BadChCandFilterToken_, ifilterbadChCand);
   bool  filterbadChCandidate = *ifilterbadChCand;
   //   if (filterbadChCandidate<1)   std::cout << "filterbadChCandidate=" << filterbadChCandidate << std::endl;

   edm::Handle<bool> ifilterbadPFMuon;
   iEvent.getByToken(BadPFMuonFilterToken_, ifilterbadPFMuon);
   bool filterbadPFMuon = *ifilterbadPFMuon;
   //  if (filterbadPFMuon<1)   std::cout << "filterbadPFMuon=" << filterbadPFMuon << std::endl;

   //Pass All MET Filters? //
   passAllMETFilters =  passHBHENoiseFilter * passHBHENoiseIsoFilter * passEcalDeadCellTriggerPrimitiveFilter * passgoodVertices * passeeBadScFilter * passglobalTightHalo2016Filter * filterbadChCandidate * filterbadPFMuon * NobadMuons ;
   //   passAllMETFilters=1.0;
   //  std::cout << "passAllMETFilters=" << passAllMETFilters << std::endl;

   ///-- VERTEX --///
   edm::Handle<reco::VertexCollection> vertices;
   iEvent.getByToken(vtxToken_, vertices);
   if (vertices->empty()) return; // skip the event if no PV found
   // std::cout << "Number of vertices " << vertices->size() << std::endl;
   //const reco::Vertex &PV = vertices->front();
   reco::VertexCollection vtxs = *vertices;

   int nvtx=0;
   int recoVtx = vertices->size();

   if( vertices.isValid() ){
     for( reco::VertexCollection::const_iterator vtx = vtxs.begin(); vtx!=vtxs.end(); ++vtx ) {
       bool isGood = ( (vtx->ndof() >= 4.0) &&
               (abs(vtx->z()) <= 24.0) &&
               (abs(vtx->position().Rho()) <= 2.0)
               );
       //       std::cout << "vtx->ndof()=" << vtx->ndof() << " ## vtx->z()=" << vtx->z() << " ## vtx->position().Rho()=" << vtx->position().Rho() << std::endl;
       //       std::cout << "Vertex is good? " << isGood << std::endl;
       if( !isGood ) continue;
       nvtx++;
     }
   }
   //   std::cout << "Number of good vertices " << nvtx << std::endl;


   TLorentzVector LooseMu(0,0,0,0);
   int nTightMu=0;
   iEvent.getByToken(muonToken_, muons);
   for (const pat::Muon &mu : *muons) {
     //https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideMuonIdRun2#Tight_Muon
     //     if ( (mu.pt()>20.0) &&  (abs(mu.eta())<2.4) && (mu.isLooseMuon(PV)) &&  ((mu.isolationR03().sumPt/mu.pt())<0.10) ) {
     if ( (mu.pt()>20.0) &&  (abs(mu.eta())<2.4) && (mu.isLooseMuon()) &&  ((mu.isolationR03().sumPt/mu.pt())<0.10) ) {
       LooseMu.SetPxPyPzE(mu.px(),mu.py(),mu.pz(),mu.energy());
         nTightMu++;
         MuonIDPassed->push_back(1);}
     else {MuonIDPassed->push_back(0);}


     //std::cout << "mu.pt()=" << mu.pt() << " abs(mu.eta())=" << abs(mu.eta()) << " mu.isolationR03().sumPt/mu.pt()=" << mu.isolationR03().sumPt/mu.pt() << std::endl;
     //       printf("muon with pt %4.1f, dz(PV) %+5.3f, POG loose id %d, tight id %d\n",
     //     mu.pt(), mu.muonBestTrack()->dz(PV.position()), mu.isLooseMuon(), mu.isTightMuon(PV));
   }
   //std::cout << "nTightMu=" << nTightMu << std::endl;

   float mass=0.0;
   float pTz=0.0;
   float PX_z=0.0;
   float PY_z=0.0;
   //float PZ_z=0.0;
   float E_z=0.0;
   // float Phi_z=0;
   // Control region for fake tau background validation //
   //   int nTightMu=0;
   //edm::Handle<pat::MuonCollection> muons;  ///in header now
   //   edm::Handle<std::vector<pat::Muon>>  muonHandle;

   //iEvent.getByToken(muonToken_, muons);
   for (std::vector<pat::Muon>::const_iterator iMu = muons->begin(); iMu != muons->end(); ++iMu) {
     for (std::vector<pat::Muon>::const_iterator jMu = iMu+1; jMu != muons->end(); ++jMu) {
        if (iMu->pt()>20.0 && jMu->pt()>20.0 ) {
          if ( fabs(iMu->eta())<2.4 && fabs(jMu->eta())<2.4 ) {
            if ( (iMu->charge())*(jMu->charge())<0 ) {
              if ( (iMu->isLooseMuon()) && (jMu->isLooseMuon()) ) { 
                 if ( ((iMu->isolationR03().sumPt/iMu->pt())<0.10) && ((jMu->isolationR03().sumPt/jMu->pt())<0.10) ) { 
                       TLorentzVector mu1, mu2 ;
		  mu1.SetPxPyPzE(iMu->px(), iMu->py(), iMu->pz(), iMu->energy());
		  mu2.SetPxPyPzE(jMu->px(), jMu->py(), jMu->pz(), jMu->energy());
		  TLorentzVector diMuon=mu1+mu2;
		  if (diMuon.M()>mass)   {
		    mass=diMuon.M();
                     pTz=diMuon.Pt();   
                     PX_z=diMuon.Px();
		     PY_z=diMuon.Py();
	             //PZ_z=diMuon.Pz();
		     E_z=diMuon.E();	
		     //Phi_z=diMuon.Phi();
                  }	
                }
              }
           }
         }
       }
      // std::cout << "pt1,2 " << iMu->pt() << " , " << jMu->pt() << std::endl;
     }
   }

   std::vector<TLorentzVector> v_muon;
   for (std::vector<pat::Muon>::const_iterator iiMu = muons->begin(); iiMu != muons->end(); ++iiMu) {
     if (iiMu->pt()>20.0 ) {
       if ( fabs(iiMu->eta())<2.4 ) {
	 if ( (iiMu->isLooseMuon()) ) { 
	   if ( ((iiMu->isolationR03().sumPt/iiMu->pt())<0.10) ) { 
	     TLorentzVector mui1;
	     mui1.SetPxPyPzE(iiMu->px(), iiMu->py(), iiMu->pz(), iiMu->energy());
	     v_muon.push_back(mui1);
	   }	
	 }
       }
     }
   }
   


//std::cout << "dimuon mass = " << mass << " dimuon pt=" << pTz << std::endl; 
//std::cout << "dimuon px=" << PX_z << " py=" << PY_z << " pz=" << PZ_z << " E=" << E_z << std::endl; 
//   std::cout << "dimuon phi = " << Phi_z << std::endl;

   //
   /*
   int tau_OneProng[10] = {0};
   int tau_TwoProng[10] = {0};
   int tau_ThreeProng[10] = {0};
   int tau_Rare[10] = {0};
   int tau_Null[10] = {0};
   */
   int tau_DM[10]={0};
   //   int tau_DM_CR[10]={0};
   int nGoodTau=0;
   int nGoodTauCR=0;
   int nGoodTau_Old_Medium=0;
   int nGoodTau_Old_VLoose=0;
   int nGoodTau_noEleRej=0;
   int nGoodTau_noMuRej=0;
   int nGoodTau_Old_Loose=0;
   int nGoodTau_Old_VTight=0;
   int nGoodTau_Old_Tight=0;
   int nGoodTau_New_VLoose=0;
   int nGoodTau_New_Medium=0;
   int nGoodTau_New_Loose=0;
   int nGoodTau_New_VTight=0;
   int nGoodTau_New_Tight=0;
   int nGoodTau_decay= 0;
   int nGoodTau_acc= 0;
   int nGoodTau_decay_iso=0;
   int nGoodTau_decay_iso_muon=0;
   /////  int nGoodTau_decay_iso_muon_ele=0;
   int nGoodTauTrig=0;

   double tau_pt[10]={0};
   double tau_phi[10]={0};
   double tau_pt_CR[10]={0};

   double tau_px_CR[10]={0};
   double tau_py_CR[10]={0};
   double tau_pz_CR[10]={0};
   double tau_E_CR[10]={0};

   double tau_phi_CR[10]={0};
   double tau_pt_Old_Medium[10]={0};
   double tau_phi_Old_Medium[10]={0};
   double tau_pt_Old_VLoose[10]={0};
   double tau_pt_noEleRej[10]={0};
   double tau_pt_noMuRej[10]={0};
   double tau_phi_Old_VLoose[10]={0};
   double tau_phi_noEleRej[10]={0};
   double tau_phi_noMuRej[10]={0};
   double tau_pt_Old_Loose[10]={0};
   double tau_phi_Old_Loose[10]={0};
   double tau_pt_Old_VTight[10]={0};
   double tau_phi_Old_VTight[10]={0};
   double tau_pt_Old_Tight[10]={0};
   double tau_phi_Old_Tight[10]={0};
   double tau_pt_New_VLoose[10]={0};
   double tau_phi_New_VLoose[10]={0};
   double tau_pt_New_Loose[10]={0};
   double tau_phi_New_Loose[10]={0};
   double tau_pt_New_Tight[10]={0};
   double tau_phi_New_Tight[10]={0};
   double tau_pt_New_VTight[10]={0};
   double tau_phi_New_VTight[10]={0};
   double tau_pt_New_Medium[10]={0};
   double tau_phi_New_Medium[10]={0};

   double tau_eta[10]={0};
   double tau_eta_CR[10]={0};

   double tau_pt_trig[10]={0};
   double tau_phi_trig[10]={0};
   double tau_eta_trig[10]={0};

   int tau_charge[10]={0};
   double tau_dxy[10]={0};
   int tau_nTrack[10]={0};
   double tau_chIso[10]={0};
   double tau_neuIso[10]={0};
   double tau_energy[10]={0}; 

   int tau_charge_CR[10]={0};
   double tau_dxy_CR[10]={0};
   int tau_nTrack_CR[10]={0};
   double tau_chIso_CR[10]={0};
   double tau_neuIso_CR[10]={0};

   int nGoodTau_ScaleUp=0;
   double tau_pt_ScaleUp[10]={0};
   double tau_phi_ScaleUp[10]={0};
   int nGoodTau_ScaleDown=0;
   double tau_pt_ScaleDown[10]={0};
   double tau_phi_ScaleDown[10]={0};
   
   double tauScaleShiftUp=1;
   double tauScaleShiftDown=1;
   if (!RunOnData) {
     tauScaleShiftUp=1.03;
     tauScaleShiftDown=0.97;
   }
   TLorentzVector tau_NoESCorr(0,0,0,0);
   TLorentzVector tau_NoShift(0,0,0,0);

   TLorentzVector tau_NoShift_Old_Medium(0,0,0,0);
   TLorentzVector tau_NoShift_Old_VLoose(0,0,0,0);
   TLorentzVector tau_NoShift_noEleRej(0,0,0,0);
  TLorentzVector tau_NoShift_noMuRej(0,0,0,0);
   TLorentzVector tau_NoShift_Old_Loose(0,0,0,0);
   TLorentzVector tau_NoShift_Old_VTight(0,0,0,0);
   TLorentzVector tau_NoShift_Old_Tight(0,0,0,0);
  
   TLorentzVector tau_NoShift_New_VLoose(0,0,0,0);
   TLorentzVector tau_NoShift_New_Loose(0,0,0,0);
   TLorentzVector tau_NoShift_New_VTight(0,0,0,0);
   TLorentzVector tau_NoShift_New_Tight(0,0,0,0);
   TLorentzVector tau_NoShift_New_Medium(0,0,0,0);
  
   TLorentzVector tau_NoShift_acc(0,0,0,0);
   TLorentzVector tau_NoShift_decay(0,0,0,0);
   TLorentzVector tau_NoShift_decay_iso(0,0,0,0);
   TLorentzVector tau_NoShift_decay_iso_muon(0,0,0,0);
   ////   TLorentzVector tau_NoShift_decay_iso_muon_ele(0,0,0,0);
   TLorentzVector tau_ScaleUp(0,0,0,0);
   TLorentzVector tau_ScaleDown(0,0,0,0);
   // TLorentzVector tau_nonIso(0,0,0,0);

   iEvent.getByToken(tauToken_, taus);
   //  std::cout << "Tau size from main loop " << taus->size() << std::endl;
   for (const pat::Tau &tau : *taus) {
     // std::cout << "TAKE THIS TAU" << std::endl;
     tauDM=tau.decayMode();
     /*
       kNull = -1
       kOneProng0PiZero = 0 
       kOneProng1PiZero = 1 
       kOneProng2PiZero = 2 
       kOneProng3PiZero = 3
       kOneProngNPiZero = 4
       kTwoProng0PiZero = 5
       kTwoProng1PiZero = 6
       kTwoProng2PiZero = 7
       kTwoProng3PiZero = 8
       kTwoProngNPiZero = 9
       kThreeProng0PiZero = 10
       kThreeProng1PiZero = 11
       kThreeProng2PiZero = 12
       kThreeProng3PiZero = 13
       kThreeProngNPiZero = 14
       kRareDecayMode = 15
     */
     // One prong = 0 , One prong pi0 = 1,2,3,4, Three prong = 10,11,12,13,14
      tau_NoESCorr.SetPxPyPzE(tau.px(),tau.py(),tau.pz(),tau.energy());
      // tau_NoESCorr.SetPxPyPzE(tau.px(),tau.py(),tau.pz(),tau.energy());
     //float TES = 1.0 ;
     //if (!RunOnData) {
     // if (tauDM==0) {
     //	 TES = 0.982 ;
     // } else if ((tauDM>0) && (tauDM<5)) {
     //	 TES = 1.01;
     //} else if ((tauDM>9) && (tauDM<15)) {
     //	 TES = 0.996;
     //}
     // }
     //     std::cout << "tauDM=" << tauDM << " TES=" << TES << std::endl;
      tau_NoShift_acc=tau_NoESCorr ;//*TES;
     //     tau_NoShift_acc.SetPxPyPzE(tau.px(),tau.py(),tau.pz(),tau.energy());
     if (PassTauAcceptance(tau_NoShift_acc)==true) {
       nGoodTau_acc++;
     }
     // 
     //
     if ( (PassTauID_Old_VLoose(tau)==true) ) {
       tau_NoShift_Old_VLoose=tau_NoESCorr;//*TES;       
       if (PassTauAcceptance(tau_NoShift_Old_VLoose)==true) {
	 tau_pt_Old_VLoose[nGoodTau_Old_VLoose]=tau_NoShift_Old_VLoose.Pt();
	 tau_phi_Old_VLoose[nGoodTau_Old_VLoose]=tau_NoShift_Old_VLoose.Phi();
	 nGoodTau_Old_VLoose++;
       }
     }

     if ( (PassTauID_Old_Medium(tau)==true) ) {
       tau_NoShift_Old_Medium=tau_NoESCorr;//*TES;       
       if (PassTauAcceptance(tau_NoShift_Old_Medium)==true) {
	 tau_pt_Old_Medium[nGoodTau_Old_Medium]=tau_NoShift_Old_Medium.Pt();
	 tau_phi_Old_Medium[nGoodTau_Old_Medium]=tau_NoShift_Old_Medium.Phi();
	 nGoodTau_Old_Medium++;
       }
     }
     //
     //
     if ( (PassTauID_Old_Loose(tau)==true) ) {
       tau_NoShift_Old_Loose=tau_NoESCorr;//*TES;       
       if (PassTauAcceptance(tau_NoShift_Old_Loose)==true) {
	 tau_pt_Old_Loose[nGoodTau_Old_Loose]=tau_NoShift_Old_Loose.Pt();
	 tau_phi_Old_Loose[nGoodTau_Old_Loose]=tau_NoShift_Old_Loose.Phi();
	 nGoodTau_Old_Loose++;
       }
     }
     //
     //
     if ( (PassTauID_Old_Tight(tau)==true) ) {
       tau_NoShift_Old_Tight=tau_NoESCorr;//*TES;       
       if (PassTauAcceptance(tau_NoShift_Old_Tight)==true) {
	 tau_pt_Old_Tight[nGoodTau_Old_Tight]=tau_NoShift_Old_Tight.Pt();
	 tau_phi_Old_Tight[nGoodTau_Old_Tight]=tau_NoShift_Old_Tight.Phi();
	 nGoodTau_Old_Tight++;
       }
     }
     //
     //
     if ( (PassTauID_Old_VTight(tau)==true) ) {
       tau_NoShift_Old_VTight=tau_NoESCorr;//*TES;       
       if (PassTauAcceptance(tau_NoShift_Old_VTight)==true) {
	 tau_pt_Old_VTight[nGoodTau_Old_VTight]=tau_NoShift_Old_VTight.Pt();
	 tau_phi_Old_VTight[nGoodTau_Old_VTight]=tau_NoShift_Old_VTight.Phi();
	 nGoodTau_Old_VTight++;
       }
     }
     //
     //
     if ( (PassTauID_New_VTight(tau)==true) ) {
       tau_NoShift_New_VTight=tau_NoESCorr;//*TES;       
       if (PassTauAcceptance(tau_NoShift_New_VTight)==true) {
	 tau_pt_New_VTight[nGoodTau_New_VTight]=tau_NoShift_New_VTight.Pt();
	 tau_phi_New_VTight[nGoodTau_New_VTight]=tau_NoShift_New_VTight.Phi();
	 nGoodTau_New_VTight++;
       }
     }
     //
     //
     if ( (PassTauID_New_Tight(tau)==true) ) {
       tau_NoShift_New_Tight=tau_NoESCorr;//*TES;       
       if (PassTauAcceptance(tau_NoShift_New_Tight)==true) {
	 tau_pt_New_Tight[nGoodTau_New_Tight]=tau_NoShift_New_Tight.Pt();
	 tau_phi_New_Tight[nGoodTau_New_Tight]=tau_NoShift_New_Tight.Phi();
	 nGoodTau_New_Tight++;
       }
     }
     //
     //
     if ( (PassTauID_New_Medium(tau)==true) ) {
       tau_NoShift_New_Medium=tau_NoESCorr;//*TES;       
       if (PassTauAcceptance(tau_NoShift_New_Medium)==true) {
	 tau_pt_New_Medium[nGoodTau_New_Medium]=tau_NoShift_New_Medium.Pt();
	 tau_phi_New_Medium[nGoodTau_New_Medium]=tau_NoShift_New_Medium.Phi();
	 nGoodTau_New_Medium++;
       }
     }
     //
     //
     if ( (PassTauID_New_Loose(tau)==true) ) {
       tau_NoShift_New_Loose=tau_NoESCorr;//*TES;       
       if (PassTauAcceptance(tau_NoShift_New_Loose)==true) {
	 tau_pt_New_Loose[nGoodTau_New_Loose]=tau_NoShift_New_Loose.Pt();
	 tau_phi_New_Loose[nGoodTau_New_Loose]=tau_NoShift_New_Loose.Phi();
	 nGoodTau_New_Loose++;
       }
     }
     //
     //
     if ( (PassTauID_New_VLoose(tau)==true) ) {
       tau_NoShift_New_VLoose=tau_NoESCorr;//*TES;       
       if (PassTauAcceptance(tau_NoShift_New_VLoose)==true) {
	 tau_pt_New_VLoose[nGoodTau_New_VLoose]=tau_NoShift_New_VLoose.Pt();
	 tau_phi_New_VLoose[nGoodTau_New_VLoose]=tau_NoShift_New_VLoose.Phi();
	 nGoodTau_New_VLoose++;
       }
     }
   
     if ( (PassTauID_noEleRej(tau) )==true)  {
       tau_NoShift_noEleRej=tau_NoESCorr;//*TES;       
       if (PassTauAcceptance(tau_NoShift_noEleRej)==true) {
	 tau_pt_noEleRej[nGoodTau_noEleRej]=tau_NoShift_noEleRej.Pt();
	 tau_phi_noEleRej[nGoodTau_noEleRej]=tau_NoShift_noEleRej.Phi();
	 nGoodTau_noEleRej++;
       }
     }

     if ( (PassTauID_noMuRej(tau) )==true)  {
       tau_NoShift_noMuRej=tau_NoESCorr;//*TES;       
       if (PassTauAcceptance(tau_NoShift_noMuRej)==true) {
	 tau_pt_noMuRej[nGoodTau_noMuRej]=tau_NoShift_noMuRej.Pt();
	 tau_phi_noMuRej[nGoodTau_noMuRej]=tau_NoShift_noMuRej.Phi();
	 nGoodTau_noMuRej++;
       }
     }
     //
     if ( (PassTauID(tau)==true) ) {
       // std::cout << "This Tau passed ID " << std::endl;
       //define Tau energy scale
       //       float TES = 0.982 ;
       tau_NoShift=tau_NoESCorr;//*TES;       
       tau_ScaleUp=tau_NoShift*tauScaleShiftUp;
       tau_ScaleDown=tau_NoShift*tauScaleShiftDown;
       //std::cout << "\nID passed tau. Pt = " << tau_NoShift.Pt() << std::endl;             
       if (PassTauAcceptance(tau_NoShift)==true) {
	 // std::cout << "accepted as analysis tau" << std::endl;
	 //std::cout << "\nTau selected" << std::endl;
	 tau_pt[nGoodTau]=tau_NoShift.Pt();
	 tau_phi[nGoodTau]=tau_NoShift.Phi();
	 tau_eta[nGoodTau]=tau_NoShift.Eta();
	 tau_charge[nGoodTau]=tau.charge();
	 tau_dxy[nGoodTau]=fabs(tau.dxy());
	 tau_nTrack[nGoodTau]=tau.signalCands().size();
	 tau_chIso[nGoodTau]=tau.tauID("chargedIsoPtSum");
	 tau_neuIso[nGoodTau]=tau.tauID("neutralIsoPtSum");
	 tau_DM[nGoodTau] = tauDM;
	 tau_energy[nGoodTau] = tau_NoShift.Energy();
	 nGoodTau++;
       }

       if (PassTauAcceptanceForCR(tau_NoShift)==true) {
	 //std::cout << "accepted as CR tau" << std::endl;
	 //std::cout << "\nTau selected" << std::endl;
	 tau_pt_CR[nGoodTauCR]=tau_NoShift.Pt();

	 tau_px_CR[nGoodTauCR]=tau_NoShift.Px();
	 tau_py_CR[nGoodTauCR]=tau_NoShift.Py();
	 tau_pz_CR[nGoodTauCR]=tau_NoShift.Pz();
	 tau_E_CR[nGoodTauCR]=tau_NoShift.Energy();

	 tau_phi_CR[nGoodTauCR]=tau_NoShift.Phi();
	 tau_eta_CR[nGoodTauCR]=tau_NoShift.Eta();
	 tau_charge_CR[nGoodTauCR]=tau.charge();
	 tau_dxy_CR[nGoodTauCR]=fabs(tau.dxy());
	 tau_nTrack_CR[nGoodTauCR]=tau.signalCands().size();
	 tau_chIso_CR[nGoodTauCR]=tau.tauID("chargedIsoPtSum");
	 tau_neuIso_CR[nGoodTauCR]=tau.tauID("neutralIsoPtSum");
	 //tau_DM_CR[nGoodTau] = tauDM;
	 nGoodTauCR++;
       }

       //-Syst Up-//
       if (PassTauAcceptance(tau_ScaleUp)==true) {
	 // std::cout << "Tau selected" << std::endl;
	 tau_pt_ScaleUp[nGoodTau_ScaleUp]=tau_ScaleUp.Pt();
	 tau_phi_ScaleUp[nGoodTau_ScaleUp]=tau_ScaleUp.Phi();
	 nGoodTau_ScaleUp++;
       }
       
       //-Syst Down-//
       if (PassTauAcceptance(tau_ScaleDown)==true) {
	 // std::cout << "Tau selected" << std::endl;
	 tau_pt_ScaleDown[nGoodTau_ScaleDown]=tau_ScaleDown.Pt();
	 tau_phi_ScaleDown[nGoodTau_ScaleDown]=tau_ScaleDown.Phi();
	 nGoodTau_ScaleDown++;
       }

       //-For Trigger Efficiency-//
       if (PassTauAcceptanceForTrigEff(tau_NoShift)==true) {
	 tau_pt_trig[nGoodTauTrig]=tau_NoShift.Pt();
	 tau_eta_trig[nGoodTauTrig]=tau_NoShift.Eta();
	 tau_phi_trig[nGoodTauTrig]=tau_NoShift.Phi();
	 nGoodTauTrig++;
       }
     }


     if ( (PassTauID_decay(tau)==true) ) {
       tau_NoShift_decay.SetPxPyPzE(tau.px(),tau.py(),tau.pz(),tau.energy());
       if (PassTauAcceptance(tau_NoShift_decay)==true) {
	 nGoodTau_decay++;
       }
     }
     //
     if ( (PassTauID_decay_iso(tau)==true) ) {
       tau_NoShift_decay_iso.SetPxPyPzE(tau.px(),tau.py(),tau.pz(),tau.energy());
       if (PassTauAcceptance(tau_NoShift_decay_iso)==true) {
	 nGoodTau_decay_iso++;
       }
     }
     //
     if ( (PassTauID_decay_iso_muon(tau)==true) ) {
       tau_NoShift_decay_iso_muon.SetPxPyPzE(tau.px(),tau.py(),tau.pz(),tau.energy());
       if (PassTauAcceptance(tau_NoShift_decay_iso_muon)==true) {
	 nGoodTau_decay_iso_muon++;
       }
     }
     //

   }

   //   std::cout << "nGoodTau=" << nGoodTau << std::endl;
   // In each event, the tau-ID scale factor is obtained using the first good tau //
   // This should be fine because in the end we select events with one good tau //
   if (!RunOnData) {
     tauID_SF = GetTauIDScaleFactor(tau_pt[0], "nominal");

     tauID_SF_flat_syst_up = GetTauIDScaleFactorUncert(tauID_SF, tau_pt[0], "flat_up", "def", "def");
     tauID_SF_flat_syst_down = GetTauIDScaleFactorUncert(tauID_SF, tau_pt[0], "flat_down", "def", "def");

     tauID_SF_ptDep_syst_up = GetTauIDScaleFactorUncert(tauID_SF, tau_pt[0], "ptDep_up", "up5", "def");
     tauID_SF_ptDep_syst_down = GetTauIDScaleFactorUncert(tauID_SF, tau_pt[0], "ptDep_down", "def", "down35");

     tauID_SF_ptDep_syst_up50 = GetTauIDScaleFactorUncert(tauID_SF, tau_pt[0], "ptDep_up", "up50", "def");
     tauID_SF_ptDep_syst_down50 = GetTauIDScaleFactorUncert(tauID_SF, tau_pt[0], "ptDep_down", "def", "down50");
     tauID_SF_ptDep_syst_up5 = GetTauIDScaleFactorUncert(tauID_SF, tau_pt[0], "ptDep_up", "up5", "def");
     //
     tauID_SF_other = GetTauIDScaleFactor(tau_pt[0], "nominal_ptDep");
     tauID_SF_flat_syst_up_other = GetTauIDScaleFactorUncert(tauID_SF_other, tau_pt[0], "flat_up", "def", "def");
     tauID_SF_flat_syst_down_other = GetTauIDScaleFactorUncert(tauID_SF_other, tau_pt[0], "flat_down", "def", "def");
     tauID_SF_ptDep_syst_up50_other = GetTauIDScaleFactorUncert(tauID_SF_other, tau_pt[0], "ptDep_up", "up50", "def");
     tauID_SF_ptDep_syst_down50_other = GetTauIDScaleFactorUncert(tauID_SF_other, tau_pt[0], "ptDep_down", "def", "down50");


     //     std::cout << "pt=" << tau_pt[0] << " tauID_SF=" << tauID_SF << " tauID_SF_flat_syst_up=" << tauID_SF_flat_syst_up << " tauID_SF_flat_syst_down=" << tauID_SF_flat_syst_down << 
     // " tauID_SF_ptDep_syst_up=" << tauID_SF_ptDep_syst_up << " tauID_SF_ptDep_syst_down=" << tauID_SF_ptDep_syst_down << std::endl;

     //     std::cout <<  "* tauID_SF_ptDep_syst_up50=" << tauID_SF_ptDep_syst_up50 << " tauID_SF_ptDep_syst_down50=" << tauID_SF_ptDep_syst_down50 
     //	       << " tauID_SF_ptDep_syst_up5=" << tauID_SF_ptDep_syst_up5  << std::endl;

     // std::cout << " tauID_SF_other=" << tauID_SF_other << " tauID_SF_flat_syst_up_other=" << tauID_SF_flat_syst_up_other << " tauID_SF_flat_syst_down_other=" 
     //	       << tauID_SF_flat_syst_down_other <<  " tauID_SF_ptDep_syst_up50_other=" << tauID_SF_ptDep_syst_up50_other 
     //	       << " tauID_SF_ptDep_syst_down50_other=" << tauID_SF_ptDep_syst_down50_other << std::endl;

   }
   //    std::cout <<"tauPt=" << tau_pt[0] <<  " tauID_SF | tauID_SF_syst_up | tauID_SF_syst_down " <<  tauID_SF << " | " << tauID_SF_syst_up << " | " <<  tauID_SF_syst_down << std::endl ;
   
    if (!RunOnData) {

      if ( fabs(tau_eta[0]) < 1.460) {
	tauELE_SF =1.32;
	tauELE_SF_syst_up =  1.32+0.03;
	tauELE_SF_syst_down = 1.32-0.03;
      }
      
      else if ( fabs(tau_eta[0]) >= 1.460) {
	tauELE_SF = 1.38;
	tauELE_SF_syst_up = 1.38+0.04;
	tauELE_SF_syst_down = 1.38-0.04;
      }
    }




   edm::Handle<edm::ValueMap<bool> > ele_id_decisions;
   iEvent.getByToken(eleIdMapToken_ ,ele_id_decisions);

   TLorentzVector LooseEle(0,0,0,0);
   int nLooseEle=0;
   int nEleIDFail=0;
   float IDfail_ele_pT[10]={0};
   float IDfail_ele_eta[10]={0};

   iEvent.getByToken(electronToken_, electrons);
   // for (const pat::Electron &el : *electrons) {
   //   std::cout << "\n***********electrons->size() = " << electrons->size() << std::endl;
   for (size_t i = 0; i < electrons->size(); ++i){
     const auto el = electrons->ptrAt(i);
     if (el->pt() < 5) continue;
     //https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedElectronIdentificationRun2
     //if ( (el.pt()>20) &&  ( abs(el.eta())<2.5 )  && (el.electronID("cutBasedElectronID-Spring15-25ns-V1-standalone-loose")>6) ){
     bool isPassEleId  = (*ele_id_decisions)[el];
     //    std::cout << "pT=" << el->pt() << " eta=" << abs(el->eta()) << " idpass=" << isPassEleId << std::endl;
     if ( (el->pt()>20) &&  ( abs(el->eta())<2.5 )  && !(isPassEleId) ){
       IDfail_ele_pT[nEleIDFail]=el->pt();
       IDfail_ele_eta[nEleIDFail]=el->eta();
       nEleIDFail++;
      }

     //
      if ( (el->pt()>20) &&  ( abs(el->eta())<2.5 )  && isPassEleId ){
       //     if ( (el.pt()>20) &&  ( abs(el.eta())<2.5 )  && (el.electronID("egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-loose")>6) ){
      LooseEle.SetPxPyPzE(el->px(),el->py(),el->pz(),el->energy());
       nLooseEle++;
       EleIDPassed->push_back(1);}
     else {EleIDPassed->push_back(0);}
   }
   //  std::cout << "nLooseEle=" << nLooseEle << std::endl;
   if ( (nEleIDFail>0) && (nLooseEle==0) ) {
     h1_EventCount2->Fill(1);
   }
   

    ////// MET //////
   edm::Handle<pat::METCollection> mets;
   if (!RunOnData) { 
     iEvent.getByToken(metToken_, mets);
   } else if (RunOnData) {
     iEvent.getByToken(metToken_reminiaod_, mets);
   }

   const pat::MET &met = mets->front();
   double met_val=met.pt();
   double met_phi=met.phi();
   float met_px=met.px();
   float met_py=met.py();
   float met_E=met.energy();

   //control region 
   float recoil=0;
   float recoil_phi=0;
   //std::cout <<  "MET " << met.pt() << " GeV.  px=" << met.px() << " py=" << met.py() << " pz=" << met.pz() << " energy=" << met.energy() << std::endl;
   // std::cout << "met_phi=" << met_phi << std::endl;
   if (mass>0) {
     TVector3 v1;  
     v1.SetXYZ(PX_z+met_px,PY_z+met_py,E_z+met_E);	  
     recoil=v1.Perp();
     recoil_phi=v1.Phi();
     //     std::cout << "recoil px2+py2 " <<   sqrt((PX_z+met_px)*(PX_z+met_px) + (PY_z+met_py)*(PY_z+met_py)) << std::endl;
     //  std::cout << "recoil vector magnitude = " << v1.Mag() << std::endl;
     // std::cout << "recoil vector perp = " << v1.Perp() << std::endl;
     //std::cout << "met+Zpt scalar sum = " << met_val+pTz << std::endl;
     
   }
   //   std::cout << "recoil phi = " << recoil_phi << std::endl;

   //-------------------------//
   //**--MET UNCERTAINTIES--**//
   //-------------------------//
   //--JetEnergy Up--//
   double met_val_JetEnUp=met.shiftedPt(pat::MET::JetEnUp);
   double met_phi_JetEnUp=met.shiftedPhi(pat::MET::JetEnUp);
   //--JetEnergy Down--//
   double met_val_JetEnDown=met.shiftedPt(pat::MET::JetEnDown);
   double met_phi_JetEnDown=met.shiftedPhi(pat::MET::JetEnDown);
   //--JetRes Up--//
   double met_val_JetResUp=met.shiftedPt(pat::MET::JetResUp);
   double met_phi_JetResUp=met.shiftedPhi(pat::MET::JetResUp);
   //--JetRes Down--//
   double met_val_JetResDown=met.shiftedPt(pat::MET::JetResDown);
   double met_phi_JetResDown=met.shiftedPhi(pat::MET::JetResDown);
   //--MuonEnergy Up--//
   double met_val_MuonEnUp=met.shiftedPt(pat::MET::MuonEnUp);
   double met_phi_MuonEnUp=met.shiftedPhi(pat::MET::MuonEnUp);
   //--MuonEnergy Down--//
   double met_val_MuonEnDown=met.shiftedPt(pat::MET::MuonEnDown);
   double met_phi_MuonEnDown=met.shiftedPhi(pat::MET::MuonEnDown);
   //--ElectronEnergy Up--//
   double met_val_ElectronEnUp=met.shiftedPt(pat::MET::ElectronEnUp);
   double met_phi_ElectronEnUp=met.shiftedPhi(pat::MET::ElectronEnUp);
   //--ElectronEnergy Down--//
   double met_val_ElectronEnDown=met.shiftedPt(pat::MET::ElectronEnDown);
   double met_phi_ElectronEnDown=met.shiftedPhi(pat::MET::ElectronEnDown);
   //--TauEnergy Up--//
   double met_val_TauEnUp=met.shiftedPt(pat::MET::TauEnUp);
   double met_phi_TauEnUp=met.shiftedPhi(pat::MET::TauEnUp);
   //--TauEnergy Down--//
   double met_val_TauEnDown=met.shiftedPt(pat::MET::TauEnDown);
   double met_phi_TauEnDown=met.shiftedPhi(pat::MET::TauEnDown);
   //--PhotonEnergy Up--//
   double met_val_PhotonEnUp=met.shiftedPt(pat::MET::PhotonEnUp);
   double met_phi_PhotonEnUp=met.shiftedPhi(pat::MET::PhotonEnUp);
   //--PhotonEnergy Down--//
   double met_val_PhotonEnDown=met.shiftedPt(pat::MET::PhotonEnDown);
   double met_phi_PhotonEnDown=met.shiftedPhi(pat::MET::PhotonEnDown);
   //--UnclusteredEnergy Up--//
   double met_val_UnclusteredEnUp=met.shiftedPt(pat::MET::UnclusteredEnUp);
   double met_phi_UnclusteredEnUp=met.shiftedPhi(pat::MET::UnclusteredEnUp);
   //--UnclusteredEnergy Down--//
   double met_val_UnclusteredEnDown=met.shiftedPt(pat::MET::UnclusteredEnDown);
   double met_phi_UnclusteredEnDown=met.shiftedPhi(pat::MET::UnclusteredEnDown);


   //----------------//
   //--Final Weight--//
   //----------------//
   if (!RunOnData) {
     final_weight               = Lumi_Wt * mc_event_weight * k_fak * tauID_SF * trig_SF * tauELE_SF;
     final_weight_tauID_SF_other = Lumi_Wt * mc_event_weight * k_fak * tauID_SF_other * trig_SF * tauELE_SF;
     final_wt_NOPU = mc_event_weight * k_fak * tauID_SF * trig_SF * tauELE_SF;

     // syst //
     final_weight_PUweight_UP   = Lumi_Wt_UP * mc_event_weight * k_fak * tauID_SF * trig_SF * tauELE_SF;
     final_weight_PUweight_DOWN = Lumi_Wt_DOWN * mc_event_weight * k_fak * tauID_SF * trig_SF * tauELE_SF;
     final_weight_kfact_UP      = Lumi_Wt * mc_event_weight * k_fak_up * tauID_SF * trig_SF * tauELE_SF;
     final_weight_kfact_DOWN    = Lumi_Wt * mc_event_weight * k_fak_down * tauID_SF *trig_SF * tauELE_SF;
   
     final_weight_tauIDSF_flat_UP    = Lumi_Wt * mc_event_weight * k_fak * tauID_SF_flat_syst_up  *trig_SF * tauELE_SF;
     final_weight_tauIDSF_flat_DOWN  = Lumi_Wt * mc_event_weight * k_fak * tauID_SF_flat_syst_down *trig_SF * tauELE_SF;
     final_weight_tauIDSF_ptDep_UP    = Lumi_Wt * mc_event_weight * k_fak * tauID_SF_ptDep_syst_up  *trig_SF * tauELE_SF;
     final_weight_tauIDSF_ptDep_DOWN  = Lumi_Wt * mc_event_weight * k_fak * tauID_SF_ptDep_syst_down *trig_SF * tauELE_SF;
     final_weight_tauIDSF_ptDep_UP5    = Lumi_Wt * mc_event_weight * k_fak * tauID_SF_ptDep_syst_up5  *trig_SF * tauELE_SF;
     final_weight_tauIDSF_ptDep_UP50    = Lumi_Wt * mc_event_weight * k_fak * tauID_SF_ptDep_syst_up50  *trig_SF * tauELE_SF;
     final_weight_tauIDSF_ptDep_DOWN50  = Lumi_Wt * mc_event_weight * k_fak * tauID_SF_ptDep_syst_down50 *trig_SF * tauELE_SF;

     final_weight_tauIDSF_flat_UP_other    = Lumi_Wt * mc_event_weight * k_fak * tauID_SF_flat_syst_up_other  *trig_SF * tauELE_SF;
     final_weight_tauIDSF_flat_DOWN_other  = Lumi_Wt * mc_event_weight * k_fak * tauID_SF_flat_syst_down_other *trig_SF * tauELE_SF;
     final_weight_tauIDSF_ptDep_UP50_other    = Lumi_Wt * mc_event_weight * k_fak * tauID_SF_ptDep_syst_up50_other  *trig_SF * tauELE_SF;
     final_weight_tauIDSF_ptDep_DOWN50_other  = Lumi_Wt * mc_event_weight * k_fak * tauID_SF_ptDep_syst_down50_other *trig_SF * tauELE_SF;


     final_weight_tauELESF_UP    = Lumi_Wt * mc_event_weight * k_fak * tauELE_SF_syst_up * tauID_SF *trig_SF;
     final_weight_tauELESF_DOWN  = Lumi_Wt * mc_event_weight * k_fak * tauELE_SF_syst_down * tauID_SF *trig_SF;

     final_weight_trigSF_UP    = Lumi_Wt * mc_event_weight * k_fak * tauID_SF *trig_SF_syst_up * tauELE_SF;
     final_weight_trigSF_DOWN  = Lumi_Wt * mc_event_weight * k_fak * tauID_SF *trig_SF_syst_down * tauELE_SF;
   }
   else {
     final_weight               = 1.0;
     final_weight_tauID_SF_other               = 1.0;
     final_wt_NOPU =1.0;
     final_weight_PUweight_UP   = 1.0;
     final_weight_PUweight_DOWN = 1.0;
     final_weight_kfact_UP      = 1.0;
     final_weight_kfact_DOWN    = 1.0;

     final_weight_tauIDSF_flat_UP    = 1.0;
     final_weight_tauIDSF_flat_DOWN  = 1.0;
     final_weight_tauIDSF_flat_UP_other    = 1.0;
     final_weight_tauIDSF_flat_DOWN_other  = 1.0;

     final_weight_tauIDSF_ptDep_UP    = 1.0;
     final_weight_tauIDSF_ptDep_DOWN  = 1.0;

     final_weight_tauIDSF_ptDep_UP5    = 1.0;

     final_weight_tauIDSF_ptDep_UP50    = 1.0;
     final_weight_tauIDSF_ptDep_DOWN50  = 1.0;

     final_weight_tauIDSF_ptDep_UP50_other    = 1.0;
     final_weight_tauIDSF_ptDep_DOWN50_other  = 1.0;

     final_weight_tauELESF_UP    = 1.0;
     final_weight_tauELESF_DOWN  = 1.0;
     final_weight_trigSF_UP = 1.0;
     final_weight_trigSF_DOWN = 1.0;
   }
   /*   
   std::cout << "Data ? " << RunOnData <<
     " LumiWt=" << Lumi_Wt <<
     " LumiWtUP=" << Lumi_Wt_UP <<
     " LumiWtDOWN=" << Lumi_Wt_DOWN <<
     " mc_evt_wt=" << mc_event_weight <<
     " kfak=" << k_fak <<
     " kfak_up=" << k_fak_up <<
     " kfak_down=" << k_fak_down <<
     " tauID_SF=" << tauID_SF <<
   //  " tauID_SF_up=" << tauID_SF_syst_up <<
   //  " tauID_SF_down=" << tauID_SF_syst_down <<
     " tauELE_SF_up=" << tauELE_SF_syst_up <<
     " tauELE_SF_down=" << tauELE_SF_syst_down << std::endl;
   */

   if ((nGenTauJet==1))  h1_TauPt_Gen_den_reco->Fill(TauPt_Gen);
   if ((nGenTauJet==1) && (nGoodTau_acc>0))  h1_TauPt_Gen_num_reco->Fill(TauPt_Gen);
   //
   if ((nGenTauJet==1) && (nGoodTau_acc>0) )  h1_TauPt_Gen_den->Fill(TauPt_Gen);
   if ((nGenTauJet==1) && (nGoodTau>0))       h1_TauPt_Gen_num_tot->Fill(TauPt_Gen);
   if ((nGenTauJet==1) && (nGoodTau_decay>0)) h1_TauPt_Gen_num_decay->Fill(TauPt_Gen);
   //
   if ((nGenTauJet==1) && (nGoodTau_decay>0)) h1_TauPt_Gen_den_decay->Fill(TauPt_Gen);
   if ((nGenTauJet==1) && (nGoodTau_decay>0) && (nGoodTau_decay_iso>0)) h1_TauPt_Gen_num_decay_iso->Fill(TauPt_Gen);
   //
   if ((nGenTauJet==1) && (nGoodTau_decay_iso>0)) h1_TauPt_Gen_den_decay_iso->Fill(TauPt_Gen);
   if ((nGenTauJet==1) && (nGoodTau_decay_iso>0) && (nGoodTau_decay_iso_muon>0)) h1_TauPt_Gen_num_decay_iso_muon->Fill(TauPt_Gen);
   //
   if ((nGenTauJet==1) && (nGoodTau_decay_iso_muon>0)) h1_TauPt_Gen_den_decay_iso_muon->Fill(TauPt_Gen);
   if ((nGenTauJet==1) && (nGoodTau_decay_iso_muon>0) && (nGoodTau>0)) h1_TauPt_Gen_num_decay_iso_muon_ele->Fill(TauPt_Gen);
   //
   h1_nGoodTau_Reco->Fill(nGoodTau,final_weight);
   //   if (nGenTau==1) FindTauIDEfficiency(iEvent,tauGen_p4[0]);
   
   if (!RunOnData) {
     //     std::cout << "passMuonTrig_ForEff=" << passMuonTrig_ForEff << " nGoodTauTrig=" << nGoodTauTrig 
     //	       << " tauPt1=" << tau_pt_trig[0] << " tauEta1=" << tau_eta_trig[0] 
     //        << " MET=" << met_val << std::endl;
     FindTauTrigEfficiency(passMuonTrig_ForEff, passTauTrig, nGoodTauTrig, met_val, tau_pt_trig[0], tau_eta_trig[0]);

     float dphi_tau_met_trig = deltaPhi(tau_phi_trig[0],met_phi);
     float MT_trig = sqrt(2*tau_pt_trig[0]*met_val*(1- cos(dphi_tau_met_trig)));

     FindTauTrigEfficiency_Signal(passTauTrig, nGoodTauTrig, met_val, tau_pt_trig[0], tau_eta_trig[0], MT_trig);
   }  

////
////// Define the control region ///////
////
//   std::cout << "===CONTROL REGION===  " << std::endl;
//   std::cout << "pass trig and MET filters?" << std::endl;
   if (passMuonTrig && passAllMETFilters ) {
     if ( (nvtx>0) ) {
       if (nGoodTauCR==1) {
	 TLorentzVector my_control_tau;
	 my_control_tau.SetPxPyPzE(tau_px_CR[0],tau_py_CR[0],tau_pz_CR[0],tau_E_CR[0]);
	 float DR_tau_nearestMu = DRTauMu(my_control_tau, v_muon); 

	 h2_taueta_dRtaumu->Fill(tau_eta_CR[0],DR_tau_nearestMu,final_weight);
	 hprof_taueta_dRtaumu->Fill(tau_eta_CR[0],DR_tau_nearestMu,final_weight);

	 if (tau_pt_CR[0]<80) {
	   h2_taueta_dRtaumu_pT_less_80->Fill(tau_eta_CR[0],DR_tau_nearestMu,final_weight);
	   hprof_taueta_dRtaumu_pT_less_80->Fill(tau_eta_CR[0],DR_tau_nearestMu,final_weight);
	 }

	 if (  (tau_pt_CR[0]>80) && (tau_pt_CR[0]<140) ) {
	   h2_taueta_dRtaumu_pT_80_140->Fill(tau_eta_CR[0],DR_tau_nearestMu,final_weight);
	   hprof_taueta_dRtaumu_pT_80_140->Fill(tau_eta_CR[0],DR_tau_nearestMu,final_weight);
	 }

	 if (tau_pt_CR[0]>140) {
	   h2_taueta_dRtaumu_pT_above_140->Fill(tau_eta_CR[0],DR_tau_nearestMu,final_weight);
	   hprof_taueta_dRtaumu_pT_above_140->Fill(tau_eta_CR[0],DR_tau_nearestMu,final_weight);
	 }

	 float dphi_tau_met_CR = deltaPhi(tau_phi_CR[0],recoil_phi);
	 float MT_CR = sqrt(2*tau_pt_CR[0]*recoil*(1- cos(dphi_tau_met_CR)));
	 float tauptovermet=tau_pt_CR[0]/recoil ;	     
	 //  std::cout << "DR_tau_nearestMu=" << DR_tau_nearestMu << std::endl;
	 if (DR_tau_nearestMu>0.1) {
	   if (mass>81.0 && mass<101.0) {
	     h1_recoil_CR->Fill(recoil,final_weight);
	     h1_tauPt_CR->Fill(tau_pt_CR[0],final_weight);
	     h1_tauPhi_CR->Fill(tau_phi_CR[0],final_weight);
	     h1_tauEta_CR->Fill(tau_eta_CR[0],final_weight);
	     h1_dimuonMass_CR->Fill(mass,final_weight);
	     h1_dimuonPt_CR->Fill(pTz,final_weight);
	     h1_original_MET_CR->Fill(met_val,final_weight);
	     //
	     float dphi_tau_met_CR = deltaPhi(tau_phi_CR[0],recoil_phi);
	     float MT_CR = sqrt(2*tau_pt_CR[0]*recoil*(1- cos(dphi_tau_met_CR)));
	     h1_MT_CR->Fill(MT_CR,final_weight);
	     float tauptovermet=tau_pt_CR[0]/recoil ;
	     h1_tauPtOverMet_CR->Fill(tauptovermet,final_weight);
	     h1_dPhiTauMet_CR->Fill(dphi_tau_met_CR,final_weight) ;
	     //
	     h1_TauCharge_CR->Fill(tau_charge_CR[0],final_weight);
	     h1_TauDxy_CR->Fill(tau_dxy_CR[0],final_weight);
	     h1_TauChIso_CR->Fill(tau_chIso_CR[0],final_weight);
	     h1_TauNeuIso_CR->Fill(tau_neuIso_CR[0],final_weight);
	     h1_TauNTrack_CR->Fill(tau_nTrack_CR[0],final_weight);
	     //
	     // CR2 = CR + dphi and pT/MET cut
	     //
	     if (  (fabs(dphi_tau_met_CR)>2.4)  &&  (tauptovermet>0.7 && tauptovermet<1.3)  ) {
	     h1_recoil_CR2->Fill(recoil,final_weight);
	     h1_tauPt_CR2->Fill(tau_pt_CR[0],final_weight);
	     h1_tauPhi_CR2->Fill(tau_phi_CR[0],final_weight);
	     h1_tauEta_CR2->Fill(tau_eta_CR[0],final_weight);
	     h1_dimuonMass_CR2->Fill(mass,final_weight);
	     h1_dimuonPt_CR2->Fill(pTz,final_weight);
	     h1_original_MET_CR2->Fill(met_val,final_weight);
	     h1_MT_CR2->Fill(MT_CR,final_weight);
	     h1_tauPtOverMet_CR2->Fill(tauptovermet,final_weight);
	     h1_dPhiTauMet_CR2->Fill(dphi_tau_met_CR,final_weight) ;
	     h1_TauCharge_CR2->Fill(tau_charge_CR[0],final_weight);
	     h1_TauDxy_CR2->Fill(tau_dxy_CR[0],final_weight);
	     h1_TauChIso_CR2->Fill(tau_chIso_CR[0],final_weight);
	     h1_TauNeuIso_CR2->Fill(tau_neuIso_CR[0],final_weight);
	     h1_TauNTrack_CR2->Fill(tau_nTrack_CR[0],final_weight);

	     }
	   }
	   ///CR3 : CR2 but dimuon_mass changed to m>10 GeV
	   if (mass>10) {
	     if (  (fabs(dphi_tau_met_CR)>2.4)  &&  (tauptovermet>0.7 && tauptovermet<1.3)  ) {
	     h1_recoil_CR3->Fill(recoil,final_weight);
	     h1_tauPt_CR3->Fill(tau_pt_CR[0],final_weight);
	     h1_tauPhi_CR3->Fill(tau_phi_CR[0],final_weight);
	     h1_tauEta_CR3->Fill(tau_eta_CR[0],final_weight);
	     h1_dimuonMass_CR3->Fill(mass,final_weight);
	     h1_dimuonPt_CR3->Fill(pTz,final_weight);
	     h1_original_MET_CR3->Fill(met_val,final_weight);
	     h1_MT_CR3->Fill(MT_CR,final_weight);
	     h1_tauPtOverMet_CR3->Fill(tauptovermet,final_weight);
	     h1_dPhiTauMet_CR3->Fill(dphi_tau_met_CR,final_weight) ;
	     h1_TauCharge_CR3->Fill(tau_charge_CR[0],final_weight);
	     h1_TauDxy_CR3->Fill(tau_dxy_CR[0],final_weight);
	     h1_TauChIso_CR3->Fill(tau_chIso_CR[0],final_weight);
	     h1_TauNeuIso_CR3->Fill(tau_neuIso_CR[0],final_weight);
	     h1_TauNTrack_CR3->Fill(tau_nTrack_CR[0],final_weight);

	     }
	   }

	   ///CR5 : CR but dimuon_mass changed to m>10 GeV
	   if (mass>10) {
	     h1_recoil_CR5->Fill(recoil,final_weight);
	     h1_tauPt_CR5->Fill(tau_pt_CR[0],final_weight);
	     h1_tauPhi_CR5->Fill(tau_phi_CR[0],final_weight);
	     h1_tauEta_CR5->Fill(tau_eta_CR[0],final_weight);
	     h1_dimuonMass_CR5->Fill(mass,final_weight);
	     h1_dimuonPt_CR5->Fill(pTz,final_weight);
	     h1_original_MET_CR5->Fill(met_val,final_weight);
	     h1_MT_CR5->Fill(MT_CR,final_weight);
	     h1_tauPtOverMet_CR5->Fill(tauptovermet,final_weight);
	     h1_dPhiTauMet_CR5->Fill(dphi_tau_met_CR,final_weight) ;
	     h1_TauCharge_CR5->Fill(tau_charge_CR[0],final_weight);
	     h1_TauDxy_CR5->Fill(tau_dxy_CR[0],final_weight);
	     h1_TauChIso_CR5->Fill(tau_chIso_CR[0],final_weight);
	     h1_TauNeuIso_CR5->Fill(tau_neuIso_CR[0],final_weight);
	     h1_TauNTrack_CR5->Fill(tau_nTrack_CR[0],final_weight);

	   }
	 

	   ///CR4 : CR2 but dimuon_mass changed to 10<m<81 and m>101 GeV
	   if ( (mass>10 && mass<81) || (mass>101) ) {
	     if (  (fabs(dphi_tau_met_CR)>2.4)  &&  (tauptovermet>0.7 && tauptovermet<1.3)  ) {
	     h1_recoil_CR4->Fill(recoil,final_weight);
	     h1_tauPt_CR4->Fill(tau_pt_CR[0],final_weight);
	     h1_tauPhi_CR4->Fill(tau_phi_CR[0],final_weight);
	     h1_tauEta_CR4->Fill(tau_eta_CR[0],final_weight);
	     h1_dimuonMass_CR4->Fill(mass,final_weight);
	     h1_dimuonPt_CR4->Fill(pTz,final_weight);
	     h1_original_MET_CR4->Fill(met_val,final_weight);
	     h1_MT_CR4->Fill(MT_CR,final_weight);
	     h1_tauPtOverMet_CR4->Fill(tauptovermet,final_weight);
	     h1_dPhiTauMet_CR4->Fill(dphi_tau_met_CR,final_weight) ;
	     h1_TauCharge_CR4->Fill(tau_charge_CR[0],final_weight);
	     h1_TauDxy_CR4->Fill(tau_dxy_CR[0],final_weight);
	     h1_TauChIso_CR4->Fill(tau_chIso_CR[0],final_weight);
	     h1_TauNeuIso_CR4->Fill(tau_neuIso_CR[0],final_weight);
	     h1_TauNTrack_CR4->Fill(tau_nTrack_CR[0],final_weight);

	     }
	   }


	   ///CR6 : CR but dimuon_mass changed to 10<m<81 and m>101 GeV
	   if ( (mass>10 && mass<81) || (mass>101) ) {
	     h1_recoil_CR6->Fill(recoil,final_weight);
	     h1_tauPt_CR6->Fill(tau_pt_CR[0],final_weight);
	     h1_tauPhi_CR6->Fill(tau_phi_CR[0],final_weight);
	     h1_tauEta_CR6->Fill(tau_eta_CR[0],final_weight);
	     h1_dimuonMass_CR6->Fill(mass,final_weight);
	     h1_dimuonPt_CR6->Fill(pTz,final_weight);
	     h1_original_MET_CR6->Fill(met_val,final_weight);
	     h1_MT_CR6->Fill(MT_CR,final_weight);
	     h1_tauPtOverMet_CR6->Fill(tauptovermet,final_weight);
	     h1_dPhiTauMet_CR6->Fill(dphi_tau_met_CR,final_weight) ;
	     h1_TauCharge_CR6->Fill(tau_charge_CR[0],final_weight);
	     h1_TauDxy_CR6->Fill(tau_dxy_CR[0],final_weight);
	     h1_TauChIso_CR6->Fill(tau_chIso_CR[0],final_weight);
	     h1_TauNeuIso_CR6->Fill(tau_neuIso_CR[0],final_weight);
	     h1_TauNTrack_CR6->Fill(tau_nTrack_CR[0],final_weight);

	     }
	   



	 }
       }
     }
   }



   //---------------//
   //---Selection---// VETO ele, mu ONLY if that is not matched with with
   //---------------//
   ///// name it SR2
   // std::cout << "Proceed to selection cuts " << std::endl;
 
   ///
   //// tau ID (full) + veto additional ele and mu
   ///
   int nLooseEle_additional = nLooseEle;
   int nLooseMu_additional = nTightMu;
   if ( (nGoodTau==1) && (nLooseEle==1) ) {      
     float dr_tau_ele=tau_NoShift.DeltaR(LooseEle);
     // std::cout << "dr_tau_ele = " << dr_tau_ele << std::endl;
     h1_dr_tau_ele->Fill(dr_tau_ele);
     if (dr_tau_ele<0.5) {
       nLooseEle_additional=0;
     }
   }
   if ( (nGoodTau==1) && (nTightMu==1) ) {      
     float dr_tau_mu=tau_NoShift.DeltaR(LooseMu);
     // std::cout << "dr_tau_mu = " << dr_tau_mu << std::endl;
     h1_dr_tau_mu->Fill(dr_tau_mu);
     if (dr_tau_mu<0.5) {
       nLooseMu_additional=0;
     }
   }
   if (passTauTrig && passAllMETFilters ) {
     if ( (nvtx>0) && (nLooseMu_additional==0) ) { 
       if (nLooseEle_additional==0)  {
	 if ( (PassFinalCuts(nGoodTau, met_val,met_phi,tau_pt[0],tau_phi[0]) == true) ) {
	   double MT_SR2=  sqrt(2*tau_pt[0]*met_val*(1- cos(dphi_tau_met)));
	   h1_MT_SR2->Fill(MT_SR2,final_weight);
	 }
       }
     }
   }

   ///
   //// tau ID (no EleRej) + veto additional ele and mu
   ///
   int nLooseEle_additional_noEleRej = nLooseEle;
   int nLooseMu_additional_noEleRej = nTightMu;
   if ( (nGoodTau_noEleRej==1) && (nLooseEle==1) ) {      
     float dr_tau_ele_noEleRej=tau_NoShift_noEleRej.DeltaR(LooseEle);
      if (dr_tau_ele_noEleRej<0.5) {
       nLooseEle_additional_noEleRej=0;
     }
   }
   if ( (nGoodTau_noEleRej==1) && (nTightMu==1) ) {      
     float dr_tau_mu_noEleRej=tau_NoShift_noEleRej.DeltaR(LooseMu);
      if (dr_tau_mu_noEleRej<0.5) {
       nLooseMu_additional_noEleRej=0;
     }
   }
   if (passTauTrig && passAllMETFilters ) {
     if ( (nvtx>0) && (nLooseMu_additional_noEleRej==0) ) { 
       if (nLooseEle_additional_noEleRej==0)  {
	 if ( (PassFinalCuts(nGoodTau_noEleRej, met_val,met_phi,tau_pt_noEleRej[0],tau_phi_noEleRej[0]) == true) ) {
	   double MT_SR2_noEleRej=  sqrt(2*tau_pt_noEleRej[0]*met_val*(1- cos(dphi_tau_met)));
	   h1_MT_SR2_noEleRej->Fill(MT_SR2_noEleRej,final_weight);
	 }
       }
     }
   }


   ///
   //// tau ID (no MuRej) + veto additional ele and mu
   ///
   int nLooseEle_additional_noMuRej = nLooseEle;
   int nLooseMu_additional_noMuRej = nTightMu;
   if ( (nGoodTau_noMuRej==1) && (nLooseEle==1) ) {      
     float dr_tau_ele_noMuRej=tau_NoShift_noMuRej.DeltaR(LooseEle);
      if (dr_tau_ele_noMuRej<0.5) {
       nLooseEle_additional_noMuRej=0;
     }
   }
   if ( (nGoodTau_noMuRej==1) && (nTightMu==1) ) {      
     float dr_tau_mu_noMuRej=tau_NoShift_noMuRej.DeltaR(LooseMu);
      if (dr_tau_mu_noMuRej<0.5) {
       nLooseMu_additional_noMuRej=0;
     }
   }
   if (passTauTrig && passAllMETFilters ) {
     if ( (nvtx>0) && (nLooseMu_additional_noMuRej==0) ) { 
       if (nLooseEle_additional_noMuRej==0)  {
	 if ( (PassFinalCuts(nGoodTau_noMuRej, met_val,met_phi,tau_pt_noMuRej[0],tau_phi_noMuRej[0]) == true) ) {
	   double MT_SR2_noMuRej=  sqrt(2*tau_pt_noMuRej[0]*met_val*(1- cos(dphi_tau_met)));
	   h1_MT_SR2_noMuRej->Fill(MT_SR2_noMuRej,final_weight);
	 }
       }
     }
   }



   //---------------//
   //---Selection---//
   //---------------//
   // std::cout << "Proceed to selection cuts " << std::endl;
   if (passTauTrig && passAllMETFilters ) {
     if ( (nvtx>0) && (nLooseMu_additional==0) && (nLooseEle_additional==0) ) {
       //std::cout << "pt=" << tau_pt[0] << " met=" << met_val  <<   " pToverEtMiss_S2=" << pToverEtMiss_S2 << std::endl;

       //
       //** Stage 3 = Passed all other cuts, but pToverEtMiss cut not applied **//
       //
       if ( (PassFinalCuts_Except_pToverEtMiss(nGoodTau, met_val,met_phi,tau_pt[0],tau_phi[0]) == true) ) {
	 h1_pToverEtMiss_Stage3->Fill(pToverEtMiss,final_weight);
       }

       //
       //** Stage 2 = Passed all other cuts, but dphi_tau_met cut not applied **//
       //
       if ( (PassFinalCuts_Except_dphiTauMET(nGoodTau, met_val,met_phi,tau_pt[0],tau_phi[0]) == true) ) {
	 h1_dphiTauMET_Stage2->Fill(dphi_tau_met,final_weight);
       }

       //
       //** Stage1 = final stage (all cuts applied) **//
       //

       // Old_Medium //
       if ( (PassFinalCuts(nGoodTau_Old_Medium, met_val,met_phi,tau_pt_Old_Medium[0],tau_phi_Old_Medium[0]) == true) ) {
	 double MT_Old_Medium=  sqrt(2*tau_pt_Old_Medium[0]*met_val*(1- cos(dphi_tau_met)));
	 h1_MT_Old_Medium_Stage1->Fill(MT_Old_Medium,final_weight);
       }
       //

       // Old_VLoose //
       if ( (PassFinalCuts(nGoodTau_Old_VLoose, met_val,met_phi,tau_pt_Old_VLoose[0],tau_phi_Old_VLoose[0]) == true) ) {
	 double MT_Old_VLoose=  sqrt(2*tau_pt_Old_VLoose[0]*met_val*(1- cos(dphi_tau_met)));
	 h1_MT_Old_VLoose_Stage1->Fill(MT_Old_VLoose,final_weight);
       }
       //

       // noEleRej //
       if ( (PassFinalCuts(nGoodTau_noEleRej, met_val,met_phi,tau_pt_noEleRej[0],tau_phi_noEleRej[0]) == true) ) {
	 double MT_noEleRej=  sqrt(2*tau_pt_noEleRej[0]*met_val*(1- cos(dphi_tau_met)));
	 h1_MT_noEleRej_Stage1->Fill(MT_noEleRej,final_weight);
       }
       //

       // noMuRej //
       if ( (PassFinalCuts(nGoodTau_noMuRej, met_val,met_phi,tau_pt_noMuRej[0],tau_phi_noMuRej[0]) == true) ) {
	 double MT_noMuRej=  sqrt(2*tau_pt_noMuRej[0]*met_val*(1- cos(dphi_tau_met)));
	 h1_MT_noMuRej_Stage1->Fill(MT_noMuRej,final_weight);
       }
       //

       // Old_VTight //
       if ( (PassFinalCuts(nGoodTau_Old_VTight, met_val,met_phi,tau_pt_Old_VTight[0],tau_phi_Old_VTight[0]) == true) ) {
	 double MT_Old_VTight=  sqrt(2*tau_pt_Old_VTight[0]*met_val*(1- cos(dphi_tau_met)));
	 h1_MT_Old_VTight_Stage1->Fill(MT_Old_VTight,final_weight);
       }
       //

       // Old_Loose //
       if ( (PassFinalCuts(nGoodTau_Old_Loose, met_val,met_phi,tau_pt_Old_Loose[0],tau_phi_Old_Loose[0]) == true) ) {
	 double MT_Old_Loose=  sqrt(2*tau_pt_Old_Loose[0]*met_val*(1- cos(dphi_tau_met)));
	 h1_MT_Old_Loose_Stage1->Fill(MT_Old_Loose,final_weight);
       }
       //

       // Old_Tight //
       if ( (PassFinalCuts(nGoodTau_Old_Tight, met_val,met_phi,tau_pt_Old_Tight[0],tau_phi_Old_Tight[0]) == true) ) {
	 double MT_Old_Tight=  sqrt(2*tau_pt_Old_Tight[0]*met_val*(1- cos(dphi_tau_met)));
	 h1_MT_Old_Tight_Stage1->Fill(MT_Old_Tight,final_weight);
       }
       //

       // New_VLoose //
       if ( (PassFinalCuts(nGoodTau_New_VLoose, met_val,met_phi,tau_pt_New_VLoose[0],tau_phi_New_VLoose[0]) == true) ) {
	 double MT_New_VLoose=  sqrt(2*tau_pt_New_VLoose[0]*met_val*(1- cos(dphi_tau_met)));
	 h1_MT_New_VLoose_Stage1->Fill(MT_New_VLoose,final_weight);
       }
       //

       // New_VTight //
       if ( (PassFinalCuts(nGoodTau_New_VTight, met_val,met_phi,tau_pt_New_VTight[0],tau_phi_New_VTight[0]) == true) ) {
	 double MT_New_VTight=  sqrt(2*tau_pt_New_VTight[0]*met_val*(1- cos(dphi_tau_met)));
	 h1_MT_New_VTight_Stage1->Fill(MT_New_VTight,final_weight);
       }
       //

       // New_Loose //
       if ( (PassFinalCuts(nGoodTau_New_Loose, met_val,met_phi,tau_pt_New_Loose[0],tau_phi_New_Loose[0]) == true) ) {
	 double MT_New_Loose=  sqrt(2*tau_pt_New_Loose[0]*met_val*(1- cos(dphi_tau_met)));
	 h1_MT_New_Loose_Stage1->Fill(MT_New_Loose,final_weight);
       }
       //

       // New_Tight //
       if ( (PassFinalCuts(nGoodTau_New_Tight, met_val,met_phi,tau_pt_New_Tight[0],tau_phi_New_Tight[0]) == true) ) {
	 double MT_New_Tight=  sqrt(2*tau_pt_New_Tight[0]*met_val*(1- cos(dphi_tau_met)));
	 h1_MT_New_Tight_Stage1->Fill(MT_New_Tight,final_weight);
       }
       //

       // New_Medium //
       if ( (PassFinalCuts(nGoodTau_New_Medium, met_val,met_phi,tau_pt_New_Medium[0],tau_phi_New_Medium[0]) == true) ) {
	 double MT_New_Medium=  sqrt(2*tau_pt_New_Medium[0]*met_val*(1- cos(dphi_tau_met)));
	 h1_MT_New_Medium_Stage1->Fill(MT_New_Medium,final_weight);
       }
       //

       //
       //** Stage 4 = Passed all other cuts, but MET cut not applied **//
       //
       if ( (PassFinalCuts_noMET(nGoodTau, met_val,met_phi,tau_pt[0],tau_phi[0]) == true) ) {
	 h1_MET_Stage4->Fill(met_val,final_weight);
       }

       if ( (PassFinalCuts(nGoodTau, met_val,met_phi,tau_pt[0],tau_phi[0]) == true) ) {
	 h1_recoVtx_NoPUWt->Fill(recoVtx,final_wt_NOPU);
	 h1_recoVtx_WithPUWt->Fill(recoVtx,final_weight);
	 h1_TauPt_Stage1->Fill(tau_pt[0],final_weight);
	 h1_TauCharge_Stage1->Fill(tau_charge[0],final_weight);
	 h1_TauDxy_Stage1->Fill(tau_dxy[0],final_weight);
	 h1_TauChIso_Stage1->Fill(tau_chIso[0],final_weight);
	 h1_TauNeuIso_Stage1->Fill(tau_neuIso[0],final_weight);
	 h1_TauNTrack_Stage1->Fill(tau_nTrack[0],final_weight);
	 h1_TauEta_Stage1->Fill(tau_eta[0],final_weight);
	 h1_TauPhi_Stage1->Fill(tau_phi[0],final_weight);
	 h1_dPhi_tau_MET_Stage1->Fill(dphi_tau_met,final_weight);
	 h1_pToverEtMiss_Stage1->Fill(pToverEtMiss,final_weight);
   
	 h1_EventCount_Stage1->Fill(1);
	 if (nEleIDFail>0)  {
	   h1_EventCount_Stage1_needEleSF->Fill(1);
	   h2_IDfailEle_eta_pt_Stage1->Fill(IDfail_ele_eta[0],IDfail_ele_pT[0]);
	   h1_IDfailEle_eta_Stage1->Fill(IDfail_ele_eta[0]);
	   h1_IDfailEle_pt_Stage1->Fill(IDfail_ele_pT[0]);
	   //      	   std::cout << "id fail .. eta, pt = " << IDfail_ele_eta[0] << " " << IDfail_ele_pT[0] << std::endl;
	 }

	 //std::cout << "*Standard* dphi_tau_met=" << dphi_tau_met << std::endl;
	 double MT=  sqrt(2*tau_pt[0]*met_val*(1- cos(dphi_tau_met)));
	 //std::cout << "MT = " << MT << std::endl;
	 if ( RunOnData && (MT>100.0) ) { 
	   myfile << "#####\n\n " << sourceFileString << "  Event " << Event << "  Run " << Run <<  "  LumiSc=" << LumiSc << "\n" 
		  << "  MT=" << MT << "  MET=" << met_val << "  MET_phi=" << met_phi 
		  << "  TauPt=" << tau_pt[0] << "  TauPhi=" << tau_phi[0] << "  TauEta=" << tau_eta[0]
		  << "  dPhi_tau_MET=" << dphi_tau_met << "  energy=" << tau_energy[0] << "  decayMode=" << tau_DM[0]  
		  << "\n\n" ; 
	 }
	 //	 std::cout << "Final weight = " << final_weight << " MT = " << MT << std::endl;
	 h1_MT_Stage1->Fill(MT,final_weight);

	 h1_MT_Stage1_tauID_SF_other->Fill(MT,final_weight_tauID_SF_other);

	 h1_MET_Stage1->Fill(met_val,final_weight);
	 h1_MET_phi_Stage1->Fill(met_phi,final_weight);

	 if (tau_DM[0]==-1) {
	 h1_MT_Stage1_NullDM->Fill(MT,final_weight);
	 h1_MET_Stage1_NullDM->Fill(met_val,final_weight);
	 h1_TauPt_Stage1_NullDM->Fill(tau_pt[0],final_weight);
	 h1_TauEta_Stage1_NullDM->Fill(tau_eta[0],final_weight);
	 h1_TauPhi_Stage1_NullDM->Fill(tau_phi[0],final_weight);
	 }

	 if (tau_DM[0]>=0  &&  tau_DM[0]<=4) {
	 h1_MT_Stage1_OneProngDM->Fill(MT,final_weight);
	 h1_MET_Stage1_OneProngDM->Fill(met_val,final_weight);
	 h1_TauPt_Stage1_OneProngDM->Fill(tau_pt[0],final_weight);
	 h1_TauEta_Stage1_OneProngDM->Fill(tau_eta[0],final_weight);
	 h1_TauPhi_Stage1_OneProngDM->Fill(tau_phi[0],final_weight);
	 }

	 if (tau_DM[0]>=5  &&  tau_DM[0]<=9) {
	 h1_MT_Stage1_TwoProngDM->Fill(MT,final_weight);
	 h1_MET_Stage1_TwoProngDM->Fill(met_val,final_weight);
	 h1_TauPt_Stage1_TwoProngDM->Fill(tau_pt[0],final_weight);
	 h1_TauEta_Stage1_TwoProngDM->Fill(tau_eta[0],final_weight);
	 h1_TauPhi_Stage1_TwoProngDM->Fill(tau_phi[0],final_weight);
	 }

	 if (tau_DM[0]>=10  &&  tau_DM[0]<=14) {
	 h1_MT_Stage1_ThreeProngDM->Fill(MT,final_weight);
	 h1_MET_Stage1_ThreeProngDM->Fill(met_val,final_weight);
	 h1_TauPt_Stage1_ThreeProngDM->Fill(tau_pt[0],final_weight);
	 h1_TauEta_Stage1_ThreeProngDM->Fill(tau_eta[0],final_weight);
	 h1_TauPhi_Stage1_ThreeProngDM->Fill(tau_phi[0],final_weight);
	 }

	 if (tau_DM[0]==15) {
	 h1_MT_Stage1_RareDM->Fill(MT,final_weight);
	 h1_MET_Stage1_RareDM->Fill(met_val,final_weight);
	 h1_TauPt_Stage1_RareDM->Fill(tau_pt[0],final_weight);
	 h1_TauEta_Stage1_RareDM->Fill(tau_eta[0],final_weight);
	 h1_TauPhi_Stage1_RareDM->Fill(tau_phi[0],final_weight);
	 }

	 //
	 //	 h1_TauID_decayModeFinding->Fill(tau.tauID("decayModeFinding"),final_weight);
	 //	 h1_TauID_byMediumIsolationMVArun2v1DBoldDMwLT->Fill(tau.tauID("byMediumIsolationMVArun2v1DBoldDMwLT"),final_weight);
	 //	 h1_TauID_againstElectronLooseMVA6->Fill(tau.tauID("againstElectronLooseMVA6"),final_weight);
	 //	 h1_TauID_againstMuonLoose3->Fill(tau.tauID("againstMuonLoose3"),final_weight);
	 //
	 //--PU Systematics--//
	 if (!RunOnData) {
	   h1_MT_Stage1_pileupUncertUp->Fill(MT,final_weight_PUweight_UP);
	   h1_MT_Stage1_pileupUncertDown->Fill(MT,final_weight_PUweight_DOWN);
	   if (nGoodTau==1) setShiftedTree(tau_NoShift, met, final_weight_PUweight_UP,   "pileupUncertUp");
	   if (nGoodTau==1) setShiftedTree(tau_NoShift, met, final_weight_PUweight_DOWN, "pileupUncertDown");
	 }
	 
	 //--k-fact Systematics--//
	 if (!RunOnData) {
	   if (k_fak_stored != 100) {
	     h1_MT_Stage1_kFactorUp->Fill(MT,final_weight_kfact_UP);
	     h1_MT_Stage1_kFactorDown->Fill(MT,final_weight_kfact_DOWN);
	     if (nGoodTau==1) setShiftedTree(tau_NoShift, met, final_weight_kfact_UP,   "kFactorUp");
	     if (nGoodTau==1) setShiftedTree(tau_NoShift, met, final_weight_kfact_DOWN, "kFactorDown");
	   }
	 }
	 
	 //---Tau ID SF Systematics---//
	 if (!RunOnData) {
	   h1_MT_Stage1_TauIDSF_flat_Up->Fill(MT,final_weight_tauIDSF_flat_UP);
	   h1_MT_Stage1_TauIDSF_flat_Down->Fill(MT,final_weight_tauIDSF_flat_DOWN);

	   h1_MT_Stage1_TauIDSF_flat_Up_other->Fill(MT,final_weight_tauIDSF_flat_UP_other);
	   h1_MT_Stage1_TauIDSF_flat_Down_other->Fill(MT,final_weight_tauIDSF_flat_DOWN_other);

	   h1_MT_Stage1_TauIDSF_ptDep_Up->Fill(MT,final_weight_tauIDSF_ptDep_UP);
	   h1_MT_Stage1_TauIDSF_ptDep_Down->Fill(MT,final_weight_tauIDSF_ptDep_DOWN);

	   h1_MT_Stage1_TauIDSF_ptDep_Up5->Fill(MT,final_weight_tauIDSF_ptDep_UP5);

	   h1_MT_Stage1_TauIDSF_ptDep_Up50->Fill(MT,final_weight_tauIDSF_ptDep_UP50);
	   h1_MT_Stage1_TauIDSF_ptDep_Down50->Fill(MT,final_weight_tauIDSF_ptDep_DOWN50);

	   h1_MT_Stage1_TauIDSF_ptDep_Up50_other->Fill(MT,final_weight_tauIDSF_ptDep_UP50_other);
	   h1_MT_Stage1_TauIDSF_ptDep_Down50_other->Fill(MT,final_weight_tauIDSF_ptDep_DOWN50_other);

	   if (nGoodTau==1) setShiftedTree(tau_NoShift, met, final_weight_tauIDSF_flat_UP,   "TauIDSF_flat_Up");
	   if (nGoodTau==1) setShiftedTree(tau_NoShift, met, final_weight_tauIDSF_flat_DOWN, "TauIDSF_flat_Down");
	   if (nGoodTau==1) setShiftedTree(tau_NoShift, met, final_weight_tauIDSF_ptDep_UP,   "TauIDSF_ptDep_Up");
	   if (nGoodTau==1) setShiftedTree(tau_NoShift, met, final_weight_tauIDSF_ptDep_DOWN, "TauIDSF_ptDep_Down");
	 }

	 if (!RunOnData) {
	   h1_MT_Stage1_TauELESFUp->Fill(MT,final_weight_tauELESF_UP);
	   h1_MT_Stage1_TauELESFDown->Fill(MT,final_weight_tauELESF_DOWN);
	   if (nGoodTau==1) setShiftedTree(tau_NoShift, met, final_weight_tauELESF_UP,   "TauELESFUp");
	   if (nGoodTau==1) setShiftedTree(tau_NoShift, met, final_weight_tauELESF_DOWN, "TauELESFDown");
	 }

	 //final_weight_trigSF_UP
	 if (!RunOnData) {
	   //	   std::cout << "final_weight_trigSF_UP " << final_weight_trigSF_UP << " final_weight_trigSF_DOWN " << final_weight_trigSF_DOWN << std::endl;
	   h1_MT_Stage1_trigSFUp->Fill(MT,final_weight_trigSF_UP);
	   h1_MT_Stage1_trigSFDown->Fill(MT,final_weight_trigSF_DOWN);
	   if (nGoodTau==1) setShiftedTree(tau_NoShift, met, final_weight_trigSF_UP,   "trigSFUp");
	   if (nGoodTau==1) setShiftedTree(tau_NoShift, met, final_weight_trigSF_DOWN, "trigSFDown");
	 }
	 
	 if (!RunOnData) {
	   if (EvtHandle.isValid() ) {
	     if ( doPDFuncertainty) {
	       //--PDF Systematics--//
	       std::cout << "Evt selected. Size of inpdfweights = " << inpdfweights->size() << std::endl;
	       int imem=0;
	       for (std::vector<double>::iterator it = inpdfweights->begin() ; it != inpdfweights->end(); ++it) {
		 //std::cout << "weight = " << *it << std::endl;
		 double final_wt_with_pdf = (*it)*final_weight;
		 h1_MT_Stage1_pdfWt[imem]->Fill(MT,final_wt_with_pdf);
		 //      std::cout << "final_wt_with_pdf " << final_wt_with_pdf << std::endl ;
		 imem++;
	       }
	       
	       
	       //       std::cout << "Evt selected. Size of alpha_s_container = " << alpha_s_container->size() << std::endl;
	       // std::cout << "alpha_s_container->at(0) = " << alpha_s_container->at(0) << std::endl;
	       // std::cout << "alpha_s_container->at(1) = " << alpha_s_container->at(1) << std::endl;
	       
	       if ( alpha_s_container->size() != 0) {
		 if  ( alpha_s_container->at(0) <= alpha_s_container->at(1) ) { 
		   alpha_s_wt_down = alpha_s_container->at(0) ;
		   alpha_s_wt_up   = alpha_s_container->at(1);
		 }
		 
		 if  ( alpha_s_container->at(0) > alpha_s_container->at(1) ) { 
		   alpha_s_wt_up = alpha_s_container->at(0) ;
		   alpha_s_wt_down   = alpha_s_container->at(1);
		 }
	       }
	     }

	     final_weight_alpha_UP   = Lumi_Wt * mc_event_weight * k_fak * tauID_SF * alpha_s_wt_up * tauELE_SF ;
	     final_weight_alpha_DOWN = Lumi_Wt * mc_event_weight * k_fak * tauID_SF * alpha_s_wt_down * tauELE_SF ;
	     //	     std::cout << "final_weight_alpha_UP = " << final_weight_alpha_UP << " final_weight_alpha_DOWN=" << final_weight_alpha_DOWN << std::endl;
	     
	     h1_MT_Stage1_alphaUp->Fill(MT,final_weight_alpha_UP);
	     h1_MT_Stage1_alphaDown->Fill(MT,final_weight_alpha_DOWN);

	   }
	 }
       }
       
       //--Systematics--//
       if (!RunOnData) {
	 if ( (PassFinalCuts(nGoodTau, met_val_JetEnUp,met_phi_JetEnUp,tau_pt[0],tau_phi[0] ) == true) ) {
	   //std::cout << "*metUncert_JetEnUp* dphi_tau_met=" << dphi_tau_met << std::endl;
	   double MT_metUncert_JetEnUp = sqrt(2*tau_pt[0]*met_val_JetEnUp*(1- cos(dphi_tau_met)));  // always use the same dphi? or shifted dphi?
	   h1_MT_Stage1_metUncert_JetEnUp->Fill(MT_metUncert_JetEnUp,final_weight);
	   if (nGoodTau==1) setShiftedTree(tau_NoShift, met, final_weight, pat::MET::METUncertainty::JetEnUp, "JetEnUp");
	 }
	 ///
	 if ( (PassFinalCuts(nGoodTau, met_val_JetEnDown,met_phi_JetEnDown,tau_pt[0],tau_phi[0]) == true) ) {
	   //std::cout << "*metUncert_JetEnDown* dphi_tau_met=" << dphi_tau_met << std::endl;
	   double MT_metUncert_JetEnDown = sqrt(2*tau_pt[0]*met_val_JetEnDown*(1- cos(dphi_tau_met)));
	   h1_MT_Stage1_metUncert_JetEnDown->Fill(MT_metUncert_JetEnDown,final_weight);
	   if (nGoodTau==1) setShiftedTree(tau_NoShift, met, final_weight, pat::MET::METUncertainty::JetEnDown, "JetEnDown");
	 }
	 ///
	 if ( (PassFinalCuts(nGoodTau, met_val_JetResUp,met_phi_JetResUp,tau_pt[0],tau_phi[0]) == true) ) {
	   //std::cout << "*metUncert_JetResUp* dphi_tau_met=" << dphi_tau_met << std::endl;
	   double MT_metUncert_JetResUp = sqrt(2*tau_pt[0]*met_val_JetResUp*(1- cos(dphi_tau_met)));
	   h1_MT_Stage1_metUncert_JetResUp->Fill(MT_metUncert_JetResUp,final_weight);
	   if (nGoodTau==1) setShiftedTree(tau_NoShift, met, final_weight, pat::MET::METUncertainty::JetResUp, "JetResUp");
	 }
	 ///
	 if ( (PassFinalCuts(nGoodTau, met_val_JetResDown,met_phi_JetResDown,tau_pt[0],tau_phi[0]) == true) ) {
	   // std::cout << "*metUncert_JetResDown* dphi_tau_met=" << dphi_tau_met << std::endl;
	   double MT_metUncert_JetResDown = sqrt(2*tau_pt[0]*met_val_JetResDown*(1- cos(dphi_tau_met)));
	   h1_MT_Stage1_metUncert_JetResDown->Fill(MT_metUncert_JetResDown,final_weight);
	   if (nGoodTau==1) setShiftedTree(tau_NoShift, met, final_weight, pat::MET::METUncertainty::JetResDown, "JetResDown");
	 }
	 ///
	 if ( (PassFinalCuts(nGoodTau, met_val_MuonEnUp,met_phi_MuonEnUp,tau_pt[0],tau_phi[0]) == true) ) {
	   //std::cout << "*metUncert_MuonEnUp* dphi_tau_met=" << dphi_tau_met << std::endl;
	   double MT_metUncert_MuonEnUp = sqrt(2*tau_pt[0]*met_val_MuonEnUp*(1- cos(dphi_tau_met)));
	   h1_MT_Stage1_metUncert_MuonEnUp->Fill(MT_metUncert_MuonEnUp,final_weight);
	   if (nGoodTau==1) setShiftedTree(tau_NoShift, met, final_weight, pat::MET::METUncertainty::MuonEnUp, "MuonEnUp");
	 }
	 ///
	 if ( (PassFinalCuts(nGoodTau, met_val_MuonEnDown,met_phi_MuonEnDown,tau_pt[0],tau_phi[0]) == true) ) {
	   // std::cout << "*metUncert_MuonEnDown* dphi_tau_met=" << dphi_tau_met << std::endl;
	   double MT_metUncert_MuonEnDown = sqrt(2*tau_pt[0]*met_val_MuonEnDown*(1- cos(dphi_tau_met)));
	   h1_MT_Stage1_metUncert_MuonEnDown->Fill(MT_metUncert_MuonEnDown,final_weight);
	   if (nGoodTau==1) setShiftedTree(tau_NoShift, met, final_weight, pat::MET::METUncertainty::MuonEnDown, "MuonEnDown");
	 }
	 ///
	 if ( (PassFinalCuts(nGoodTau, met_val_ElectronEnUp,met_phi_ElectronEnUp,tau_pt[0],tau_phi[0]) == true) ) {
	   //std::cout << "*metUncert_ElectronEnUp* dphi_tau_met=" << dphi_tau_met << std::endl;
	   double MT_metUncert_ElectronEnUp = sqrt(2*tau_pt[0]*met_val_ElectronEnUp*(1- cos(dphi_tau_met)));
	   h1_MT_Stage1_metUncert_ElectronEnUp->Fill(MT_metUncert_ElectronEnUp,final_weight);
	   if (nGoodTau==1) setShiftedTree(tau_NoShift, met, final_weight, pat::MET::METUncertainty::ElectronEnUp, "ElectronEnUp");
	 }
	 ///
	 if ( (PassFinalCuts(nGoodTau, met_val_ElectronEnDown,met_phi_ElectronEnDown,tau_pt[0],tau_phi[0]) == true) ) {
	   //std::cout << "*metUncert_ElectronEnDown* dphi_tau_met=" << dphi_tau_met << std::endl;
	   double MT_metUncert_ElectronEnDown = sqrt(2*tau_pt[0]*met_val_ElectronEnDown*(1- cos(dphi_tau_met)));
	   h1_MT_Stage1_metUncert_ElectronEnDown->Fill(MT_metUncert_ElectronEnDown,final_weight);
	   if (nGoodTau==1) setShiftedTree(tau_NoShift, met, final_weight, pat::MET::METUncertainty::ElectronEnDown, "ElectronEnDown");
	 }
	 ///
	 if ( (PassFinalCuts(nGoodTau, met_val_TauEnUp,met_phi_TauEnUp,tau_pt[0],tau_phi[0]) == true) ) {
	   //std::cout << "*metUncert_TauEnUp* dphi_tau_met=" << dphi_tau_met << std::endl;
	   double MT_metUncert_TauEnUp = sqrt(2*tau_pt[0]*met_val_TauEnUp*(1- cos(dphi_tau_met)));
	   h1_MT_Stage1_metUncert_TauEnUp->Fill(MT_metUncert_TauEnUp,final_weight);
	   if (nGoodTau==1) setShiftedTree(tau_NoShift, met, final_weight, pat::MET::METUncertainty::TauEnUp, "TauEnUp");
	 }
	 ///
	 if ( (PassFinalCuts(nGoodTau, met_val_TauEnDown,met_phi_TauEnDown,tau_pt[0],tau_phi[0]) == true) ) {
	   //std::cout << "*metUncert_TauEnDown* dphi_tau_met=" << dphi_tau_met << std::endl;
	   double MT_metUncert_TauEnDown = sqrt(2*tau_pt[0]*met_val_TauEnDown*(1- cos(dphi_tau_met)));
	   h1_MT_Stage1_metUncert_TauEnDown->Fill(MT_metUncert_TauEnDown,final_weight);
	   if (nGoodTau==1) setShiftedTree(tau_NoShift, met, final_weight, pat::MET::METUncertainty::TauEnDown, "TauEnDown");
	 }
	 ///
	 if ( (PassFinalCuts(nGoodTau, met_val_PhotonEnUp,met_phi_PhotonEnUp,tau_pt[0],tau_phi[0]) == true) ) {
	   //std::cout << "*metUncert_PhotonEnUp* dphi_tau_met=" << dphi_tau_met << std::endl;
	   double MT_metUncert_PhotonEnUp = sqrt(2*tau_pt[0]*met_val_PhotonEnUp*(1- cos(dphi_tau_met)));
	   h1_MT_Stage1_metUncert_PhotonEnUp->Fill(MT_metUncert_PhotonEnUp,final_weight);
	   if (nGoodTau==1) setShiftedTree(tau_NoShift, met, final_weight, pat::MET::METUncertainty::PhotonEnUp, "PhotonEnUp");
	 }
	 ///
	 if ( (PassFinalCuts(nGoodTau, met_val_PhotonEnDown,met_phi_PhotonEnDown,tau_pt[0],tau_phi[0]) == true) ) {
	   //std::cout << "*metUncert_PhotonEnDown* dphi_tau_met=" << dphi_tau_met << std::endl;
	   double MT_metUncert_PhotonEnDown = sqrt(2*tau_pt[0]*met_val_PhotonEnDown*(1- cos(dphi_tau_met)));
	   h1_MT_Stage1_metUncert_PhotonEnDown->Fill(MT_metUncert_PhotonEnDown,final_weight);
	   if (nGoodTau==1) setShiftedTree(tau_NoShift, met, final_weight, pat::MET::METUncertainty::PhotonEnDown, "PhotonEnDown");
	 }
	 ///
	 if ( (PassFinalCuts(nGoodTau, met_val_UnclusteredEnUp,met_phi_UnclusteredEnUp,tau_pt[0],tau_phi[0]) == true) ) {
	   //std::cout << "*metUncert_UnclusteredEnUp* dphi_tau_met=" << dphi_tau_met << std::endl;
	   double MT_metUncert_UnclusteredEnUp = sqrt(2*tau_pt[0]*met_val_UnclusteredEnUp*(1- cos(dphi_tau_met)));
	   h1_MT_Stage1_metUncert_UnclusteredEnUp->Fill(MT_metUncert_UnclusteredEnUp,final_weight);
	   if (nGoodTau==1) setShiftedTree(tau_NoShift, met, final_weight, pat::MET::METUncertainty::UnclusteredEnUp, "UnclusteredEnUp");
	 }
	 ///
	 if ( (PassFinalCuts(nGoodTau, met_val_UnclusteredEnDown,met_phi_UnclusteredEnDown,tau_pt[0],tau_phi[0]) == true) ) {
	   //std::cout << "*metUncert_UnclusteredEnDown* dphi_tau_met=" << dphi_tau_met << std::endl;
	   double MT_metUncert_UnclusteredEnDown = sqrt(2*tau_pt[0]*met_val_UnclusteredEnDown*(1- cos(dphi_tau_met)));
	   h1_MT_Stage1_metUncert_UnclusteredEnDown->Fill(MT_metUncert_UnclusteredEnDown,final_weight);
	   if (nGoodTau==1) setShiftedTree(tau_NoShift, met, final_weight, pat::MET::METUncertainty::UnclusteredEnDown, "UnclusteredEnDown");
	 }
	 ///--Tau Scale--///
	 if ( (PassFinalCuts(nGoodTau_ScaleUp, met_val, met_phi, tau_pt_ScaleUp[0], tau_phi_ScaleUp[0]) == true) ) {
	   //std::cout << "*Tau Scale Up* dphi_tau_met=" << dphi_tau_met << std::endl;
	   double MT_TauScaleUp = sqrt(2*tau_pt_ScaleUp[0]*met_val*(1- cos(dphi_tau_met)));
	   h1_MT_Stage1_TauScaleUp->Fill(MT_TauScaleUp,final_weight);
	   if (nGoodTau_ScaleUp==1) setShiftedTree(tau_ScaleUp, met, final_weight, "TauScaleUp");
	 }
	 ///
	 if ( (PassFinalCuts(nGoodTau_ScaleDown, met_val, met_phi, tau_pt_ScaleDown[0], tau_phi_ScaleDown[0]) == true) ) {
	   //std::cout << "*Tau Scale Down* dphi_tau_met=" << dphi_tau_met << std::endl;
	   double MT_TauScaleDown = sqrt(2*tau_pt_ScaleDown[0]*met_val*(1- cos(dphi_tau_met)));
	   h1_MT_Stage1_TauScaleDown->Fill(MT_TauScaleDown,final_weight);
	   if (nGoodTau_ScaleDown==1) setShiftedTree(tau_ScaleDown, met, final_weight, "TauScaleDown");
	 }
       }
     }
   }

   /// N-1 Cut-flow ///

   if (passTauTrig && (nGoodTau>0) && (met_val>0) && (nvtx>0) ) {
     double dphi_all =  deltaPhi(tau_phi[0],met_phi);
     double MT_all = sqrt(2*tau_pt[0]*met_val*(1- cos(dphi_all)));
     h1_MT_passTauTrig->Fill(MT_all,final_weight); // 1
     if (passAllMETFilters) {
       h1_MT_passAllMETFilters->Fill(MT_all,final_weight); // 2
       if ( (nLooseMu_additional==0) && (nLooseEle_additional==0) ) {
	 h1_MT_leptonVeto->Fill(MT_all,final_weight); // 3                                                                                                                      
	 if (nGoodTau==1) {
	   h1_MT_nGoodTau->Fill(MT_all,final_weight); // 4
	   if (met_val>200) {
	     h1_MT_met_val->Fill(MT_all,final_weight); // 5
	     double pTbyET = tau_pt[0]/met_val ;
	     if ( (pTbyET>0.7) && (pTbyET<1.3) ) {
	       h1_MT_pTbyET->Fill(MT_all,final_weight); // 6
	       if (fabs(dphi_all)>2.4) {
		 h1_MT_dphi->Fill(MT_all,final_weight); // 7
	       }
	     }
	   }
	 } 
       }
     }
   }
   bool doQCDAna=true;
   qcd_lepton_ele->clear();
   qcd_weight_ele->clear();
   qcd_lepton_mu->clear();
   qcd_weight_mu->clear();
   qcd_lepton_tau->clear();
   qcd_weight_tau->clear();
   //if (not RunOnData)
   tauGenMatchMap.clear();
   tauGenMatchMapAllFlav.clear();
   //std::cout << "passEleTrig , passMuonTrig " << passEleTrig << " , " << passMuonTrig << std::endl; 
    
   if (passEleTrig || passMuonTrig) {
     QCDAnalyseTau(met,final_weight,pruned);
     //std::cout << "Call QCDAnalyseTau" << std::endl;
 
   }
   //if(tau_NoShift.Pt()>80 && calcMT(tau_NoShift,met)>50){
   iEvent.getByToken(jetToken_, jets);

   //Fill_QCD_Tree(true,tau_NoShift,met,final_weight);

   if (passTauTrig) {
       if (doQCDAna) {
	 //	  std::cout << "Call QCDAnalyse " << std::endl;
	 QCDAnalyse(met);
       }
       //       std::cout << "nGoodTau=" << nGoodTau << std::endl;
       // std::cout << "Call Fill_QCD_Tree for isolated tau" << std::endl;
       if (nGoodTau==1) Fill_QCD_Tree(true,tau_NoShift,15,jets,met,final_weight,0, tau_DM[0], tau_nTrack[0], tau_chIso[0], tau_neuIso[0]);

       //       std::cout << "qcd_lepton_tau->size() = " << qcd_lepton_tau->size() << std::endl;
       
       for(unsigned int ifake=0; ifake<qcd_lepton_tau->size(); ifake++){
	 pat::Tau qcd_lepton=(*qcd_lepton_tau)[ifake];
	 if(qcd_lepton.pt()>50 && calcMT(qcd_lepton,met)>50){
	   //  std::cout << "qcd_tau pt=" << qcd_lepton.pt() << " MT=" << calcMT(qcd_lepton,met) << " Call Fill_QCD_Tree for non_isolated tau" << std::endl;
           Fill_QCD_Tree(false,tau_NoShift,15,jets,met,final_weight,ifake,tauDM,tau_nTrack[0], tau_chIso[0], tau_neuIso[0]);
	 }
       }
       Fill_Tree(tau_NoShift,met,final_weight,"");
       if (useReweighting) helper->Tree_Filler("reweighttree");
       //}
     }

#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif

#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
}

float MiniAODAnalyzer::DRTauMu(TLorentzVector Tau, std::vector<TLorentzVector> MuonVect) {
  float dR12=999.0;
  int size=MuonVect.size();
  for (int n=0; n<size; n++) {
    float mydR12=Tau.DeltaR(MuonVect.at(n));
    if (mydR12<dR12) dR12=mydR12;
  }
  return dR12;
}

double MiniAODAnalyzer::GetTauIDScaleFactor(double tau_pt, std::string mode) {
  double tauSF=1.0;
  if (mode=="nominal") tauSF=0.95 ;
  if (mode=="nominal_ptDep") {
    tauSF=0.95 - (0.5*(tau_pt/1000.0)) ;
    if (tauSF<0.0) tauSF=0.0;
  }
  //
  return tauSF;
}


double MiniAODAnalyzer::GetTauIDScaleFactorUncert(double tauSF, double tau_pt, std::string mode, std::string up, std::string down) {
  double tauSF1=tauSF;
  // if (mode=="nominal") tauSF=0.95 ;
  // if (mode=="nominal_ptDep") tauSF=0.95 - (0.5*(tau_pt/1000.0)) ;
  //
  double flat_uncert = (tauSF * (5.0/100.0) );
  double ptDep_uncert_up    = 0.0;
  double ptDep_uncert_down  = 0.0;

  if (up=="up5") {
    ptDep_uncert_up   = (5.0/100.0)*(tau_pt/1000.0);
  }
  if (up=="up20") {
    ptDep_uncert_up   = (20.0/100.0)*(tau_pt/1000.0);
  }
  if (up=="up50") {
    ptDep_uncert_up   = (50.0/100.0)*(tau_pt/1000.0);
  }
  if (down=="down20") {
    ptDep_uncert_down = (20.0/100.0)*(tau_pt/1000.0);
  }
  if (down=="down50") {
    ptDep_uncert_down = (50.0/100.0)*(tau_pt/1000.0);
  }
  if (down=="down35") {
    ptDep_uncert_down = (35.0/100.0)*(tau_pt/1000.0);
  }
  //
  if (mode=="flat_up") tauSF1=(tauSF+flat_uncert);
  if (mode=="flat_down") tauSF1=(tauSF-flat_uncert);
  //
  if (mode=="ptDep_up") tauSF1=(tauSF+ptDep_uncert_up);
  if (mode=="ptDep_down") {
    tauSF1=(tauSF-ptDep_uncert_down);
    if (tauSF1<0.0) tauSF1=0.0;

  }
  //  std::cout << "SF=" << tauSF << " flat_uncert=" << flat_uncert << " ptDep_uncert=" << ptDep_uncert << std::endl;

  return tauSF1;
}

bool MiniAODAnalyzer::PassFinalCuts(int nGoodTau_, double met_val_,double met_phi_,double tau_pt_,double tau_phi_) {
  bool passed=false;
  if (nGoodTau_==1) {
    if ( met_val_>200 && tau_pt_>80.0) {
      dphi_tau_met = deltaPhi(tau_phi_,met_phi_);
      pToverEtMiss=tau_pt_/met_val_ ;
      if (pToverEtMiss>0.7 && pToverEtMiss<1.3) {
    // std::cout << "pToverEtMiss=" << pToverEtMiss << std::endl;
    if (fabs(dphi_tau_met)>2.4) {
      // std::cout << "dphi_tau_met=" << dphi_tau_met << std::endl;
      passed=true;
    }
      }
    }
  }
  return passed;
}


bool MiniAODAnalyzer::PassFinalCuts_noMET(int nGoodTau_, double met_val_,double met_phi_,double tau_pt_,double tau_phi_) {
  bool passed=false;
  if (nGoodTau_==1) {
    if ( met_val_>10 && tau_pt_>80.0) {
      dphi_tau_met = deltaPhi(tau_phi_,met_phi_);
      pToverEtMiss=tau_pt_/met_val_ ;
      if (pToverEtMiss>0.7 && pToverEtMiss<1.3) {
    // std::cout << "pToverEtMiss=" << pToverEtMiss << std::endl;
    if (fabs(dphi_tau_met)>2.4) {
      // std::cout << "dphi_tau_met=" << dphi_tau_met << std::endl;
      passed=true;
    }
      }
    }
  }
  return passed;
}


bool MiniAODAnalyzer::PassFinalCuts_Except_dphiTauMET(int nGoodTau_, double met_val_,double met_phi_,double tau_pt_,double tau_phi_) {
  bool passed=false;
  if (nGoodTau_==1) {
    if ( met_val_>200  && tau_pt_>80.0) {
      dphi_tau_met = deltaPhi(tau_phi_,met_phi_);
      pToverEtMiss=tau_pt_/met_val_ ;
      if (pToverEtMiss>0.7 && pToverEtMiss<1.3) {
	passed=true;
      }
    }
  }
  return passed;
}

bool MiniAODAnalyzer::PassFinalCuts_Except_pToverEtMiss(int nGoodTau_, double met_val_,double met_phi_,double tau_pt_,double tau_phi_) {
  bool passed=false;
  if (nGoodTau_==1) {
    if ( met_val_>200  && tau_pt_>80.0) {
      dphi_tau_met = deltaPhi(tau_phi_,met_phi_);
      pToverEtMiss=tau_pt_/met_val_ ;
      if (fabs(dphi_tau_met)>2.4) {
	passed=true;
      }
    }
  }
  return passed;
}

bool MiniAODAnalyzer::PassFinalCuts(TLorentzVector part1, const pat::MET part2, pat::MET::METUncertainty metUncert) {
    ///--- please make sure you asked explicitly for 1 good tau!! ---///
    return PassFinalCuts(1,part2.shiftedPt(metUncert),part2.shiftedPhi(metUncert),part1.Pt(),part1.Phi());
}

bool MiniAODAnalyzer::PassFinalCuts(TLorentzVector part1, const pat::MET part2) {
    ///--- please make sure you asked explicitly for 1 good tau!! ---///
    return PassFinalCuts(1,part2.pt(),part2.phi(),part1.Pt(),part1.Phi());
}

bool MiniAODAnalyzer::PassFinalCuts(int nGoodTau_,TLorentzVector part1, const pat::MET part2) {
    return PassFinalCuts(nGoodTau_,part2.pt(),part2.phi(),part1.Pt(),part1.Phi());
}

bool MiniAODAnalyzer::FindTauTrigEfficiency(bool passRefTrigger, bool passAnaTrigger, int nTau, double met, double tauPt, double tauEta) {
  // Denominator //
  if (passRefTrigger && (nTau>0) && (met>90) ) h1_TauPt_TrigEff_Deno->Fill(tauPt,final_weight);
  if (passRefTrigger && (nTau>0) && (met>90) ) h1_TauEta_TrigEff_Deno->Fill(tauEta,final_weight);
  if (passRefTrigger && (nTau>0) && (met>90) ) h1_met_TrigEff_Deno->Fill(met,final_weight);

  // Numerator //
  if (passRefTrigger && (nTau>0) && (met>90) && passAnaTrigger ) h1_TauPt_TrigEff_Num->Fill(tauPt,final_weight); 
  if (passRefTrigger && (nTau>0) && (met>90) && passAnaTrigger ) h1_TauEta_TrigEff_Num->Fill(tauEta,final_weight); 
  if (passRefTrigger && (nTau>0) && (met>90) && passAnaTrigger ) h1_met_TrigEff_Num->Fill(met,final_weight); 

  return true;
}


bool MiniAODAnalyzer::FindTauTrigEfficiency_Signal(bool passAnaTrigger, int nTau, double met, double tauPt, double tauEta, double MT) {
  // Denominator //
  //std::cout << "taupt=" << tauPt << " MET=" << met << " passTrig=" << passAnaTrigger << std::endl;
  if ((nTau>0) && (met>90) ) {
    // std::cout << "Enter Deno" << std::endl;
    h1_TauPt_TrigEff_Sig_Deno->Fill(tauPt,final_weight);
    h1_TauEta_TrigEff_Sig_Deno->Fill(tauEta,final_weight);
    h1_met_TrigEff_Sig_Deno->Fill(met,final_weight);
    h1_MT_TrigEff_Sig_Deno->Fill(MT,final_weight);
  }
  // Numerator //
  if ((nTau>0) && (met>90) && passAnaTrigger ) {
    h1_TauPt_TrigEff_Sig_Num->Fill(tauPt,final_weight); 
    h1_TauEta_TrigEff_Sig_Num->Fill(tauEta,final_weight); 
    h1_met_TrigEff_Sig_Num->Fill(met,final_weight); 
    h1_MT_TrigEff_Sig_Num->Fill(MT,final_weight); 
  }
  return true;
}

/*
bool MiniAODAnalyzer::FindTauIDEfficiency(const edm::Event& iEvent, TLorentzVector gen_p4) {
  edm::Handle<pat::TauCollection> taus;
  iEvent.getByToken(tauToken_, taus);
  TLorentzVector tauReco_p4;
  TLorentzVector tauGoodReco_p4;

  for (const pat::Tau &tau : *taus) {
    if ( (tau.pt()>20) && (fabs(tau.eta())<2.3) ) {
      tauReco_p4.SetPxPyPzE(tau.px(),tau.py(),tau.pz(),tau.energy());
      double deltaR_tau_gen_reco = gen_p4.DeltaR(tauReco_p4);
      if (deltaR_tau_gen_reco<0.4) {
    h1_TauPt_reco->Fill(tau.pt(),final_weight);
    h1_TauEta_reco->Fill(tau.eta(),final_weight);
    if (PassTauID(tau)==true) {
      h1_TauPt_goodreco->Fill(tau.pt(),final_weight);
      h1_TauEta_goodreco->Fill(tau.eta(),final_weight);
    }
      }
    }
  }
  return true;
}
*/

//

bool MiniAODAnalyzer::PassTauID(const pat::Tau &tau)
{
  bool passTauID_=true;
  //----Tau ID----//
  if ( tau.tauID("decayModeFinding") < 0.5 ) passTauID_=false;
  if ( tau.tauID("byVLooseIsolationMVArun2v1DBoldDMwLT") < 0.5 ) passTauID_=false;
  if ( tau.tauID("againstElectronLooseMVA6") < 0.5 ) passTauID_=false;
  if ( tau.tauID("againstMuonLoose3") < 0.5 ) passTauID_=false;
  return passTauID_;
 //  return  PassTauID_Old_VLoose(tau);
}

bool MiniAODAnalyzer::PassTauID_Old_Medium(const pat::Tau &tau) /// medium means medium electron rejection
{
  bool passTauID_=true;
  //----Tau ID----//
  if ( tau.tauID("decayModeFinding") < 0.5 ) passTauID_=false;
  if ( tau.tauID("byVLooseIsolationMVArun2v1DBoldDMwLT") < 0.5 ) passTauID_=false;
  //  if ( tau.tauID("byMediumIsolationMVArun2v1DBoldDMwLT") < 0.5 ) passTauID_=false;
  //  if ( tau.tauID("againstElectronLooseMVA6") < 0.5 ) passTauID_=false;
  if ( tau.tauID("againstElectronMediumMVA6") < 0.5 ) passTauID_=false;
  if ( tau.tauID("againstMuonLoose3") < 0.5 ) passTauID_=false;
  return passTauID_;
}

bool MiniAODAnalyzer::PassTauID_New_Medium(const pat::Tau &tau)
{
  bool passTauID_=true;
  //----Tau ID----//
  if ( tau.tauID("decayModeFindingNewDMs") < 0.5 ) passTauID_=false;
  if ( tau.tauID("byMediumIsolationMVArun2v1DBnewDMwLT") < 0.5 ) passTauID_=false;
  if ( tau.tauID("againstElectronLooseMVA6") < 0.5 ) passTauID_=false;
  if ( tau.tauID("againstMuonLoose3") < 0.5 ) passTauID_=false;
  return passTauID_;
}
//
//
bool MiniAODAnalyzer::PassTauID_Old_VLoose(const pat::Tau &tau)
{
  bool passTauID_=true;
  //----Tau ID----//
  if ( tau.tauID("decayModeFinding") < 0.5 ) passTauID_=false;
  if ( tau.tauID("byVLooseIsolationMVArun2v1DBoldDMwLT") < 0.5 ) passTauID_=false;
  if ( tau.tauID("againstElectronVLooseMVA6") < 0.5 ) passTauID_=false;
  if ( tau.tauID("againstMuonLoose3") < 0.5 ) passTauID_=false;
  return passTauID_;
}

bool MiniAODAnalyzer::PassTauID_noEleRej(const pat::Tau &tau)
{
  bool passTauID_=true;
  //----Tau ID----//
  if ( tau.tauID("decayModeFinding") < 0.5 ) passTauID_=false;
  if ( tau.tauID("byVLooseIsolationMVArun2v1DBoldDMwLT") < 0.5 ) passTauID_=false;
  //  if ( tau.tauID("againstElectronLooseMVA6") < 0.5 ) passTauID_=false;
  if ( tau.tauID("againstMuonLoose3") < 0.5 ) passTauID_=false;
  return passTauID_;
}

bool MiniAODAnalyzer::PassTauID_noMuRej(const pat::Tau &tau)
{
  bool passTauID_=true;
  //----Tau ID----//
  if ( tau.tauID("decayModeFinding") < 0.5 ) passTauID_=false;
  if ( tau.tauID("byVLooseIsolationMVArun2v1DBoldDMwLT") < 0.5 ) passTauID_=false;
  if ( tau.tauID("againstElectronLooseMVA6") < 0.5 ) passTauID_=false;
  //  if ( tau.tauID("againstMuonLoose3") < 0.5 ) passTauID_=false;
  return passTauID_;
}

bool MiniAODAnalyzer::PassTauID_New_VLoose(const pat::Tau &tau)
{
  bool passTauID_=true;
  //----Tau ID----//
  if ( tau.tauID("decayModeFindingNewDMs") < 0.5 ) passTauID_=false;
  if ( tau.tauID("byVLooseIsolationMVArun2v1DBnewDMwLT") < 0.5 ) passTauID_=false;
  if ( tau.tauID("againstElectronLooseMVA6") < 0.5 ) passTauID_=false;
  if ( tau.tauID("againstMuonLoose3") < 0.5 ) passTauID_=false;
  return passTauID_;
}
//
//
bool MiniAODAnalyzer::PassTauID_Old_Loose(const pat::Tau &tau)
{
  bool passTauID_=true;
  //----Tau ID----//
  if ( tau.tauID("decayModeFinding") < 0.5 ) passTauID_=false;
  if ( tau.tauID("byVLooseIsolationMVArun2v1DBoldDMwLT") < 0.5 ) passTauID_=false;
  if ( tau.tauID("againstElectronLooseMVA6") < 0.5 ) passTauID_=false;
  if ( tau.tauID("againstMuonLoose3") < 0.5 ) passTauID_=false;
  return passTauID_;
}

bool MiniAODAnalyzer::PassTauID_New_Loose(const pat::Tau &tau)
{
  bool passTauID_=true;
  //----Tau ID----//
  if ( tau.tauID("decayModeFindingNewDMs") < 0.5 ) passTauID_=false;
  if ( tau.tauID("byLooseIsolationMVArun2v1DBnewDMwLT") < 0.5 ) passTauID_=false;
  if ( tau.tauID("againstElectronLooseMVA6") < 0.5 ) passTauID_=false;
  if ( tau.tauID("againstMuonLoose3") < 0.5 ) passTauID_=false;
  return passTauID_;
}

//
//
bool MiniAODAnalyzer::PassTauID_Old_Tight(const pat::Tau &tau)
{
  bool passTauID_=true;
  //----Tau ID----//
  if ( tau.tauID("decayModeFinding") < 0.5 ) passTauID_=false;
  if ( tau.tauID("byVLooseIsolationMVArun2v1DBoldDMwLT") < 0.5 ) passTauID_=false;
  if ( tau.tauID("againstElectronTightMVA6") < 0.5 ) passTauID_=false;
  if ( tau.tauID("againstMuonLoose3") < 0.5 ) passTauID_=false;
  return passTauID_;
}

bool MiniAODAnalyzer::PassTauID_New_Tight(const pat::Tau &tau)
{
  bool passTauID_=true;
  //----Tau ID----//
  if ( tau.tauID("decayModeFindingNewDMs") < 0.5 ) passTauID_=false;
  if ( tau.tauID("byTightIsolationMVArun2v1DBnewDMwLT") < 0.5 ) passTauID_=false;
  if ( tau.tauID("againstElectronLooseMVA6") < 0.5 ) passTauID_=false;
  if ( tau.tauID("againstMuonLoose3") < 0.5 ) passTauID_=false;
  return passTauID_;
}
////
//
bool MiniAODAnalyzer::PassTauID_Old_VTight(const pat::Tau &tau)
{
  bool passTauID_=true;
  //----Tau ID----//
  if ( tau.tauID("decayModeFinding") < 0.5 ) passTauID_=false;
  if ( tau.tauID("byVLooseIsolationMVArun2v1DBoldDMwLT") < 0.5 ) passTauID_=false;
  if ( tau.tauID("againstElectronVTightMVA6") < 0.5 ) passTauID_=false;
  if ( tau.tauID("againstMuonLoose3") < 0.5 ) passTauID_=false;
  return passTauID_;
}

bool MiniAODAnalyzer::PassTauID_New_VTight(const pat::Tau &tau)
{
  bool passTauID_=true;
  //----Tau ID----//
  if ( tau.tauID("decayModeFindingNewDMs") < 0.5 ) passTauID_=false;
  if ( tau.tauID("byVTightIsolationMVArun2v1DBnewDMwLT") < 0.5 ) passTauID_=false;
  if ( tau.tauID("againstElectronLooseMVA6") < 0.5 ) passTauID_=false;
  if ( tau.tauID("againstMuonLoose3") < 0.5 ) passTauID_=false;
  return passTauID_;
}


bool MiniAODAnalyzer::PassTauID_decay(const pat::Tau &tau)
{

  bool passTauID1_=true;

  //----Tau ID----//
  if ( tau.tauID("decayModeFinding") < 0.5 ) passTauID1_=false;
  // std::cout << "pass? " << passTauID1_ << std::endl;
  return passTauID1_;
}


bool MiniAODAnalyzer::PassTauID_decay_iso(const pat::Tau &tau)
{

  bool passTauID_=true;

  //----Tau ID----//
  if ( tau.tauID("decayModeFinding") < 0.5 ) passTauID_=false;
  if ( tau.tauID("byMediumIsolationMVArun2v1DBoldDMwLT") < 0.5 ) passTauID_=false;
  return passTauID_;
}

bool MiniAODAnalyzer::PassTauID_decay_iso_muon(const pat::Tau &tau)
{

  bool passTauID_=true;

  //----Tau ID----//
  if ( tau.tauID("decayModeFinding") < 0.5 ) passTauID_=false;
  if ( tau.tauID("byMediumIsolationMVArun2v1DBoldDMwLT") < 0.5 ) passTauID_=false;
  if ( tau.tauID("againstMuonLoose3") < 0.5 ) passTauID_=false;

   return passTauID_;
}


bool MiniAODAnalyzer::PassTauID_NonIsolated(const pat::Tau &tau)
{
  bool passTauID_NonIso_=true;

  //----Tau ID----//
  if ( tau.tauID("decayModeFinding") < 0.5 ) passTauID_NonIso_=false;
  if ( tau.tauID("byMediumIsolationMVArun2v1DBoldDMwLT") > 0.5 ) passTauID_NonIso_=false;
  if ( tau.tauID("againstElectronLooseMVA6") < 0.5 ) passTauID_NonIso_=false;
  if ( tau.tauID("againstMuonLoose3") < 0.5 ) passTauID_NonIso_=false;

   return passTauID_NonIso_;
}
bool MiniAODAnalyzer::PassTauID_NonIsolated(const pat::Tau &tau, std::string idcheck){
    ///needs to be implemented

    //if(m_tauIso[idcheck]){
        //return tau->getUserRecord(Form("IsoFailedFull_%s",idcheck.c_str()));
    //}
    //if(tau->hasUserRecord(Form("FullIDpassed_%s",idcheck.c_str()))){
        //if(tau->getUserRecord(Form("FullIDpassed_%s",idcheck.c_str())).toBool()){
            //return false;
        //}
    //}
    bool passTauID_NonIso_=true;

    //----Tau ID----//
    if ( tau.tauID("decayModeFinding") < 0.5 ) passTauID_NonIso_=false;
    if ( tau.tauID(idcheck) > 0.5 ) passTauID_NonIso_=false;
    if ( tau.tauID("againstElectronLooseMVA6") < 0.5 ) passTauID_NonIso_=false;
    if ( tau.tauID("againstMuonLoose3") < 0.5 ) passTauID_NonIso_=false;

    return passTauID_NonIso_;
}

bool MiniAODAnalyzer::PassTauAcceptance(TLorentzVector tau) // For analysis //
{
  bool passTauAcc_=true;
  //  std::cout << "Inside PassTauAcceptance -> TAU pt=" << tau.Pt() << " energy=" << tau.Energy() << std::endl;
  //----pT----//
  if ( tau.Pt() < 50 ) passTauAcc_=false;

  //----Eta----//
  if ( fabs(tau.PseudoRapidity()) > 2.1 ) passTauAcc_=false;

  return passTauAcc_;
}

bool MiniAODAnalyzer::PassTauAcceptanceForCR(TLorentzVector tau) // For analysis - Control Region //
{
  bool passTauAcc_=true;
  //  std::cout << "Inside PassTauAcceptance -> TAU pt=" << tau.Pt() << " energy=" << tau.Energy() << std::endl;
  //----pT----//
  if ( tau.Pt() < 20 ) passTauAcc_=false;

  //----Eta----//
  if ( fabs(tau.PseudoRapidity()) > 2.1 ) passTauAcc_=false;

  return passTauAcc_;
}

bool MiniAODAnalyzer::PassTauAcceptanceForTrigEff(TLorentzVector tau) // For Trigger Efficiency //
{
  bool passTauAcc1_=true;
  //  std::cout << "Inside PassTauAcceptance -> TAU pt=" << tau.Pt() << " energy=" << tau.Energy() << std::endl;
  //----pT----//
  if ( tau.Pt() < 50 ) passTauAcc1_=false;

  //----Eta----//
  if ( fabs(tau.PseudoRapidity()) > 2.1 ) passTauAcc1_=false;

  return passTauAcc1_;
}

// ------------ method called once each job just before starting event loop  ------------
void
MiniAODAnalyzer::beginJob()
{
  if (RunOnData) myfile.open ("SelectedDataEvents.txt");
  Create_Trees();
  helper->CreateHistoUnchangedName("h_counters", 10, 0, 11, "N_{events}");
}

// ------------ method called once each job just after ending the event loop  ------------
void
MiniAODAnalyzer::endJob()
{
  
  if (RunOnData) myfile.close();

  if ( doPDFuncertainty) {
    for (int nb=0; nb<nbinMT; nb++) {
      double array[100] = {0.} ;
      double temp=0.0;
      double temp2=9.0e+100;
      for (int nh=0; nh<100; nh++) {
	double binCon = h1_MT_Stage1_pdfWt[nh]->GetBinContent(nb);
	array[nh]=binCon;
      }
      
      for(int nh2=0;nh2<100;nh2++) {
	if (array[nh2]>temp)  temp=array[nh2];
	if (array[nh2]<temp2) temp2=array[nh2];
      }
      
      //      double nominal=h1_MT_Stage1->GetBinContent(nb);
      //      std::cout << "Nominal= "<< nominal <<  " up= " << temp << " down=" << temp2 << std::endl;
      h1_MT_Stage1_pdfUncertUp->SetBinContent(nb,temp);
      h1_MT_Stage1_pdfUncertDown->SetBinContent(nb,temp2);
      
    }
  }

  TFileDirectory CutFlowDir = fs->mkdir("CutFlowDir");
  TH1F *cutflow = CutFlowDir.make<TH1F>("cutflow", "cutflow", 11, 0, 10);
  cutflow->SetBinContent(1, (h1_MT_passTauTrig->Integral()) );
  cutflow->SetBinContent(2, (h1_MT_passAllMETFilters->Integral()) );
  cutflow->SetBinContent(3, (h1_MT_leptonVeto->Integral()) );
  cutflow->SetBinContent(4, (h1_MT_nGoodTau->Integral()) );
  cutflow->SetBinContent(5, (h1_MT_met_val->Integral()) );
  cutflow->SetBinContent(6, (h1_MT_pTbyET->Integral()) );
  cutflow->SetBinContent(7, (h1_MT_dphi->Integral()) );

  cutflow->Write();


}



// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
MiniAODAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

/*
 *   Intoduce a map of trees, instead of a tree
 *   Has the advantage of not initialising the tree 3 times,
 *   modeled after the exisiting TAPAS code in HistClass.hh and WPrime's specialAna
*/
    //create a function that creates a Tree on a map (argument 1) with a given name (argument 2)

void MiniAODAnalyzer::Create_Trees(){
  //these are the variables used in the tree of the old analysis
  if (doTrees) {
    //Kinematics
    mLeptonTree["mt"]=0;
    mLeptonTree["delta_phi"]=0;
    mLeptonTree["pt"]=0;
    mLeptonTree["met"]=0;
    mLeptonTree["lepton_phi"]=0;
    mLeptonTree["lepton_eta"]=0;
    mLeptonTree["met_phi"]=0;
    mLeptonTree["jet1_et"]=0;
    mLeptonTree["jet1_phi"]=0;
    mLeptonTree["jet1_eta"]=0;
    mLeptonTree["bjet1"]=0;
    mLeptonTree["highEtEleTrig"]=0;
    mLeptonTree["kfak"]=0;
    
    //PDF
    mLeptonTree["id1"]=999;
    mLeptonTree["id2"]=999;
    mLeptonTree["x1"]=999;
    mLeptonTree["x2"]=999;
    mLeptonTree["qscale"]=-1;
    
    //general
    mLeptonTree["ThisWeight"]=0;
    mLeptonTree["lepton_type"]=0;
  }

  if (doTrees)  helper->Tree_Creater( &mLeptonTree, "slimtree");

  if(useReweighting==true){
    mReweightTree["gen_mt_JetEnUp"]=-1;
    mReweightTree["gen_mt_JetEnDown"]=-1;
    mReweightTree["gen_mt_JetResUp"]=-1;
    mReweightTree["gen_mt_JetResDown"]=-1;
    mReweightTree["gen_mt_MuonEnUp"]=-1;
    mReweightTree["gen_mt_MuonEnDown"]=-1;
    mReweightTree["gen_mt_ElectronEnUp"]=-1;
    mReweightTree["gen_mt_ElectronEnDown"]=-1;
    mReweightTree["gen_mt_TauEnUp"]=-1;
    mReweightTree["gen_mt_TauEnDown"]=-1;
    mReweightTree["gen_mt_PhotonEnUp"]=-1;
    mReweightTree["gen_mt_PhotonEnDown"]=-1;
    mReweightTree["gen_mt_UnclusteredEnUp"]=-1;
    mReweightTree["gen_mt_UnclusteredEnDown"]=-1;
    mReweightTree["gen_mt_pileupUncertUp"]=-1;
    mReweightTree["gen_mt_pileupUncertDown"]=-1;
    mReweightTree["gen_mt_kFactorUp"]=-1;
    mReweightTree["gen_mt_kFactorDown"]=-1;
    mReweightTree["gen_mt_TauScaleUp"]=-1;
    mReweightTree["gen_mt_TauScaleDown"]=-1;

    mReweightTree["gen_mt_TauIDSF_flat_Up"]=-1;
    mReweightTree["gen_mt_TauIDSF_flat_Down"]=-1;
    mReweightTree["gen_mt_TauIDSF_ptDep_Up"]=-1;
    mReweightTree["gen_mt_TauIDSF_ptDep_Down"]=-1;
    //
    mReweightTree["mt_JetEnUp"]=-1;
    mReweightTree["mt_JetEnDown"]=-1;
    mReweightTree["mt_JetResUp"]=-1;
    mReweightTree["mt_JetResDown"]=-1;
    mReweightTree["mt_MuonEnUp"]=-1;
    mReweightTree["mt_MuonEnDown"]=-1;
    mReweightTree["mt_ElectronEnUp"]=-1;
    mReweightTree["mt_ElectronEnDown"]=-1;
    mReweightTree["mt_TauEnUp"]=-1;
    mReweightTree["mt_TauEnDown"]=-1;
    mReweightTree["mt_PhotonEnUp"]=-1;
    mReweightTree["mt_PhotonEnDown"]=-1;
    mReweightTree["mt_UnclusteredEnUp"]=-1;
    mReweightTree["mt_UnclusteredEnDown"]=-1;
    mReweightTree["mt_pileupUncertUp"]=-1;
    mReweightTree["mt_pileupUncertDown"]=-1;
    mReweightTree["mt_kFactorUp"]=-1;
    mReweightTree["mt_kFactorDown"]=-1;
    mReweightTree["mt_TauScaleUp"]=-1;
    mReweightTree["mt_TauScaleDown"]=-1;

    mReweightTree["mt_TauIDSF_flat_Up"]=-1;
    mReweightTree["mt_TauIDSF_flat_Down"]=-1;
    mReweightTree["mt_TauIDSF_ptDep_Up"]=-1;
    mReweightTree["mt_TauIDSF_ptDep_Down"]=-1;
    //
    mReweightTree["delta_phi_JetEnUp"]=-1;
    mReweightTree["delta_phi_JetEnDown"]=-1;
    mReweightTree["delta_phi_JetResUp"]=-1;
    mReweightTree["delta_phi_JetResDown"]=-1;
    mReweightTree["delta_phi_MuonEnUp"]=-1;
    mReweightTree["delta_phi_MuonEnDown"]=-1;
    mReweightTree["delta_phi_ElectronEnUp"]=-1;
    mReweightTree["delta_phi_ElectronEnDown"]=-1;
    mReweightTree["delta_phi_TauEnUp"]=-1;
    mReweightTree["delta_phi_TauEnDown"]=-1;
    mReweightTree["delta_phi_PhotonEnUp"]=-1;
    mReweightTree["delta_phi_PhotonEnDown"]=-1;
    mReweightTree["delta_phi_UnclusteredEnUp"]=-1;
    mReweightTree["delta_phi_UnclusteredEnDown"]=-1;
    mReweightTree["delta_phi_pileupUncertUp"]=-1;
    mReweightTree["delta_phi_pileupUncertDown"]=-1;
    mReweightTree["delta_phi_kFactorUp"]=-1;
    mReweightTree["delta_phi_kFactorDown"]=-1;
    mReweightTree["delta_phi_TauScaleUp"]=-1;
    mReweightTree["delta_phi_TauScaleDown"]=-1;

    mReweightTree["delta_phi_TauIDSF_flat_Up"]=-1;
    mReweightTree["delta_phi_TauIDSF_flat_Down"]=-1;
    mReweightTree["delta_phi_TauIDSF_ptDep_Up"]=-1;
    mReweightTree["delta_phi_TauIDSF_ptDep_Down"]=-1;
    //
    mReweightTree["ThisWeight_JetEnUp"]=-1;
    mReweightTree["ThisWeight_JetEnDown"]=-1;
    mReweightTree["ThisWeight_JetResUp"]=-1;
    mReweightTree["ThisWeight_JetResDown"]=-1;
    mReweightTree["ThisWeight_MuonEnUp"]=-1;
    mReweightTree["ThisWeight_MuonEnDown"]=-1;
    mReweightTree["ThisWeight_ElectronEnUp"]=-1;
    mReweightTree["ThisWeight_ElectronEnDown"]=-1;
    mReweightTree["ThisWeight_TauEnUp"]=-1;
    mReweightTree["ThisWeight_TauEnDown"]=-1;
    mReweightTree["ThisWeight_PhotonEnUp"]=-1;
    mReweightTree["ThisWeight_PhotonEnDown"]=-1;
    mReweightTree["ThisWeight_UnclusteredEnUp"]=-1;
    mReweightTree["ThisWeight_UnclusteredEnDown"]=-1;
    mReweightTree["ThisWeight_pileupUncertUp"]=-1;
    mReweightTree["ThisWeight_pileupUncertDown"]=-1;
    mReweightTree["ThisWeight_kFactorUp"]=-1;
    mReweightTree["ThisWeight_kFactorDown"]=-1;
    mReweightTree["ThisWeight_TauScaleUp"]=-1;
    mReweightTree["ThisWeight_TauScaleDown"]=-1;

    mReweightTree["ThisWeight_TauIDSF_flat_Up"]=-1;
    mReweightTree["ThisWeight_TauIDSF_flat_Down"]=-1;
    mReweightTree["ThisWeight_TauIDSF_ptDep_Up"]=-1;
    mReweightTree["ThisWeight_TauIDSF_ptDep_Down"]=-1;
    //
    mReweightTree["met_JetEnUp"]=-1;
    mReweightTree["met_JetEnDown"]=-1;
    mReweightTree["met_JetResUp"]=-1;
    mReweightTree["met_JetResDown"]=-1;
    mReweightTree["met_MuonEnUp"]=-1;
    mReweightTree["met_MuonEnDown"]=-1;
    mReweightTree["met_ElectronEnUp"]=-1;
    mReweightTree["met_ElectronEnDown"]=-1;
    mReweightTree["met_TauEnUp"]=-1;
    mReweightTree["met_TauEnDown"]=-1;
    mReweightTree["met_PhotonEnUp"]=-1;
    mReweightTree["met_PhotonEnDown"]=-1;
    mReweightTree["met_UnclusteredEnUp"]=-1;
    mReweightTree["met_UnclusteredEnDown"]=-1;
    mReweightTree["met_pileupUncertUp"]=-1;
    mReweightTree["met_pileupUncertDown"]=-1;
    mReweightTree["met_kFactorUp"]=-1;
    mReweightTree["met_kFactorDown"]=-1;
    mReweightTree["met_TauScaleUp"]=-1;
    mReweightTree["met_TauScaleDown"]=-1;

    mReweightTree["met_TauIDSF_flat_Up"]=-1;
    mReweightTree["met_TauIDSF_flat_Down"]=-1;
    mReweightTree["met_TauIDSF_ptDep_Up"]=-1;
    mReweightTree["met_TauIDSF_ptDep_Down"]=-1;
    //    mReweightTree["mt_"+mSystName[std::to_string(i)]]=-1;
    //    mReweightTree["delta_phi_"+mSystName[std::to_string(i)]]=-1;
    //    mReweightTree["ThisWeight_"+mSystName[std::to_string(i)]]=-1;
    //    mReweightTree["met_"+mSystName[std::to_string(i)]]=-1;
  }

  if (useReweighting)  helper->Tree_Creater( &mReweightTree, "reweighttree");

  if (doTrees) {
    mQCDTree["lepton_n"]=0;
    mQCDTree["pt"]=0;
    mQCDTree["phi"]=0;
    mQCDTree["mt"]=0;
    mQCDTree["ntrack"]=0;
    mQCDTree["ChargedIso"]=0;
    mQCDTree["NeutralIso"]=0;
    mQCDTree["eta"]=0;
    mQCDTree["met"]=0;
    mQCDTree["met_phi"]=0;
    mQCDTree["jetHT"]=0;
    mQCDTree["iso"]=0;
    mQCDTree["nvert"]=0;
    mQCDTree["mtpos"]=0;
    mQCDTree["mtneg"]=0;
    mQCDTree["delta_phi"]=0;
    mQCDTree["ThisWeight"]=0;
    mQCDTree["QCDWeight"]=0;
    mQCDTree["lepton_type"]=0;
    mQCDTree["decay_mode"]=0;
    mQCDTree["metTriggerd"]=0;
    
    for(unsigned int ilep=1;ilep<3; ilep++){
      mQCDTree[ Form("mt_%d", ilep)   ]=0;
      mQCDTree[ Form("pt_%d", ilep)]=0;
      mQCDTree[ Form("phi_%d", ilep)]=0;
      mQCDTree[ Form("eta_%d", ilep)]=0;
      mQCDTree[ Form("delta_phi_%d", ilep)]=0;
      mQCDTree[ Form("decay_mode_%d", ilep)]=0;
      mQCDTree[ Form("ntrack_%d", ilep)]=0;
      mQCDTree[ Form("ChargedIso_%d", ilep)]=0;
      mQCDTree[ Form("NeutralIso_%d", ilep)]=0;
    
      //if(m_do_complicated_tau_stuff){
      //for(auto idisc : d_mydisc){
      //mQCDTree[Form("%s_%d", idisc.c_str() ,ilep ) ]=0;
      //}
      //}
    }
  }
  /*        if(m_do_complicated_tau_stuff){
	    for(auto idisc : d_mydisc){
	    mQCDTree[idisc]=0;
	    }
	    }
  */
  if (doTrees)  helper->Tree_Creater( &mQCDTree, "qcdtree");
  
  if (doTrees) {
    mFakeTree["mt"]=0;
    mFakeTree["delta_phi"]=0;
    mFakeTree["pt"]=0;
    mFakeTree["met"]=0;
    mFakeTree["phi"]=0;
    mFakeTree["eta"]=0;
    mFakeTree["met_phi"]=0;
    
    mFakeTree["decay_mode"]=0;
    mFakeTree["iso"]=0;
    mFakeTree["tau_n"]=0;
    mFakeTree["truth_match"]=0;
    
    mFakeTree["lepton_type"]=0;
    mFakeTree["lepton_mt"]=0;
    mFakeTree["lepton_delta_phi"]=0;
    mFakeTree["lepton_pt"]=0;
    mFakeTree["lepton_eta"]=0;
    mFakeTree["lepton_phi"]=0;
    mFakeTree["lepton_trigger"]=0;
    
    
    //general
    mFakeTree["ThisWeight"]=0;
  }
  
  if (doTrees)  helper->Tree_Creater( &mFakeTree, "fakeTree");
  

  //mFakeVectorTree["test"]=0;
  //std::vector<float> vec(20);
    ///needs to be fixed
/*
  std::vector<std::string> str= {"FakeCandPt",
  //std::vector<char *> str= {"FakeCandPt",
  "FakeCandPhi","FakeCandEta","FakeCandMET",
  "FakeCandMETPhi","FakeCandDeltaPhi","FakeCandIso",
  "FakeCandNoIso","FakeCandVetoNum"};
  std::vector<std::vector<float>> vecOfVec= {FakeCandPt,
  FakeCandPhi,FakeCandEta,FakeCandMET,
  FakeCandMETPhi,FakeCandDeltaPhi,FakeCandIso,
  FakeCandNoIso,FakeCandVetoNum};*/

  //for (auto arg:str) mFakeVectorTree[arg]=vec;
  //mFakeVectorTree["FakeCandPt"]=vec;
  //mFakeVectorTree["FakeCandPhi"]=0;
  //mFakeVectorTree["FakeCandEta"]=0;
  //mFakeVectorTree["FakeCandMET"]=0;
  //mFakeVectorTree["FakeCandMETPhi"]=0;
  //mFakeVectorTree["FakeCandDeltaPhi"]=0;
  //mFakeVectorTree["FakeCandIso"]=0;
  //mFakeVectorTree["FakeCandNoIso"]=0;
  //mFakeVectorTree["FakeCandVetoNum"]=0;
  if (doTrees)  helper->Tree_Creater( &mFakeVectorTree, "fakeVectorTree");
    ///needs to be fixed
  //for (size_t i=0;i<str.size();i++) helper->SetFakeBranches("fakeVectorTree",str[i].c_str(),&(vecOfVec[i]));
  //for (size_t i=0;i<str.size();i++) helper->SetFakeBranches("fakeVectorTree","FakeCandPt",&vecOfVec[i]);
  //for (size_t i=0;i<str.size();i++) helper->SetFakeBranches("fakeVectorTree","FakeCandPt",&FakeCandPt);

  if (doTrees) {
    helper->SetFakeBranches("fakeVectorTree","FakeCandPt",FakeCandPt);
    helper->SetFakeBranches("fakeVectorTree","FakeCandPhi",FakeCandPhi);
    helper->SetFakeBranches("fakeVectorTree","FakeCandEta",FakeCandEta);
    helper->SetFakeBranches("fakeVectorTree","FakeCandDeltaPhi",FakeCandDeltaPhi);
    helper->SetFakeBranches("fakeVectorTree","FakeCandMET",FakeCandMET);
    helper->SetFakeBranches("fakeVectorTree","FakeCandMETPhi",FakeCandMETPhi);
    helper->SetFakeBranches("fakeVectorTree","FakeCandIso",FakeCandIso);
    helper->SetFakeBranches("fakeVectorTree","FakeCandNoIso",FakeCandNoIso);
    helper->SetFakeBranches("fakeVectorTree","FakeCandVetoNum",FakeCandVetoNum);
    helper->SetFakeBranches("fakeVectorTree","FakeCandDecayMode",FakeCandDecayMode);
    helper->SetFakeBranches("fakeVectorTree","FakeCandLeptonType",FakeCandLeptonType);
    helper->SetFakeBranches("fakeVectorTree","FakeCandLeptonMt",FakeCandLeptonMt);
    helper->SetFakeBranches("fakeVectorTree","FakeCandLeptonDeltaPhi",FakeCandLeptonDeltaPhi);
    helper->SetFakeBranches("fakeVectorTree","FakeCandLeptonPt",FakeCandLeptonPt);
    helper->SetFakeBranches("fakeVectorTree","FakeCandLeptonEta",FakeCandLeptonEta);
    helper->SetFakeBranches("fakeVectorTree","FakeCandLeptonPhi",FakeCandLeptonPhi);
    helper->SetFakeBranches("fakeVectorTree","FakeCandLeptonTrigger",FakeCandLeptonTrigger);
  }
  //helper->SetFakeBranches("fakeVectorTree","FakeCandLorentz",&FakeCandLorentz);
}


void MiniAODAnalyzer::setShiftedTree(TLorentzVector sel_lepton, const pat::MET sel_met, double weight, pat::MET::METUncertainty metUncert, std::string name){

    if(useReweighting==true){
      //       for (int i=1;i<=14;i++){
      //            if ( (PassFinalCuts(sel_lepton,sel_met,mSyst[std::to_string(i)] ) == true) ) {
      /// currently the tau is not given, maybe loop over all taus and find the match?
      /*TLorentzVector tempP4 (0,0,0,0);
    reco::GenParticle* genMatch=GetTruthMatch("Tau",tau);
    tempP4.SetPxPyPzE(genMatch->px(),genMatch->py(),genMatch->pz(),genMatch->energy());
    mReweightTree["gen_mt_"+mSystName[std::to_string(i)]]=calcMT(tempP4,sel_met,mSyst[std::to_string(i)]);//gen, has to be fixed*/
      //     std::cout << "gen_mt_"+name <<  " " << calcMT(sel_lepton,sel_met,metUncert) << " " << sel_met.shiftedPt(metUncert)  << std::endl;
      mReweightTree["gen_mt_"+name]=calcMT(sel_lepton,sel_met,metUncert);//gen, has to be fixed
      mReweightTree["mt_"+name]=calcMT(sel_lepton,sel_met,metUncert);
      mReweightTree["met_"+name]=sel_met.shiftedPt(metUncert);
      mReweightTree["delta_phi_"+name]=deltaPhi(sel_lepton.Phi(),sel_met.shiftedPhi(metUncert));
      mReweightTree["ThisWeight_"+name]=weight;
      //  mReweightTree["gen_mt_"+mSystName[std::to_string(i)]]=calcMT(sel_lepton,sel_met,mSyst[std::to_string(i)]);//gen, has to be fixed
      //  mReweightTree["mt_"+ ]=calcMT(sel_lepton,sel_met,mSyst[std::to_string(i)]);
      //  mReweightTree["met_"+mSystName[std::to_string(i)]]=sel_met.shiftedPhi(metUncert);
      //  mReweightTree["delta_phi_"+mSystName[std::to_string(i)]]=deltaPhi(sel_lepton.Phi(),sel_met.shiftedPhi(metUncert));
      //  mReweightTree["ThisWeight_"+mSystName[std::to_string(i)]]=weight;
      //    }
      // }
    }
}
void MiniAODAnalyzer::setShiftedTree(TLorentzVector sel_lepton, const pat::MET sel_met, double weight, std::string name){
  if(useReweighting==true){
    mReweightTree["gen_mt_"+name]=calcMT(sel_lepton,sel_met);
    mReweightTree["mt_"+name]=calcMT(sel_lepton,sel_met);
    mReweightTree["met_"+name]=sel_met.phi();
    mReweightTree["delta_phi_"+name]=deltaPhi(sel_lepton.Phi(),sel_met.phi());
    mReweightTree["ThisWeight_"+name]=weight;
  }
}

void MiniAODAnalyzer::Fill_Tree(TLorentzVector sel_lepton, const pat::MET sel_met, double weight, std::string systematic){

    mLeptonTree["bjet1"]=0;
    mLeptonTree["mt"]=calcMT(sel_lepton,sel_met);
    mLeptonTree["delta_phi"]=deltaPhi(sel_lepton.Phi(),sel_met.phi());
    mLeptonTree["pt"]=sel_lepton.Pt();
    mLeptonTree["met"]=sel_met.pt();
    mLeptonTree["lepton_phi"]=sel_lepton.Phi();
    mLeptonTree["lepton_eta"]=sel_lepton.Eta();
    mLeptonTree["met_phi"]=sel_met.phi();
    mLeptonTree["kfak"]=k_fak_stored;
    //good for crosschecks
    /*
    if(JetList->size()>0){
        pxl::Particle* jet = (pxl::Particle*) JetList->at(0);
        mLeptonTree["jet1_et"]=jet->getPt();
        mLeptonTree["jet1_phi"]=jet->getPhi();
        mLeptonTree["jet1_eta"]=jet->getEta();
        if(jet->getUserRecord("pfCombinedInclusiveSecondaryVertexV2BJetTags").toDouble()>0.814 && mLeptonTree["bjet1"]==0)
            mLeptonTree["bjet1"]=jet->getPt();
    }else{
        mLeptonTree["jet1_et"]=-1;
        mLeptonTree["jet1_phi"]=99;
        mLeptonTree["jet1_eta"]=99;
    }*/
    //PDF
    //needed for reweighting
    if( not RunOnData ){
        mLeptonTree["id1"]=0;
        mLeptonTree["id2"]=0;
        mLeptonTree["x1"]=0;
        mLeptonTree["x2"]=0;
        mLeptonTree["qscale"]=0;
        //mLeptonTree["id1"]=m_GenEvtView->getUserRecord("f1");
        //mLeptonTree["id2"]=m_GenEvtView->getUserRecord("f2");
        //mLeptonTree["x1"]=m_GenEvtView->getUserRecord("x1");
        //mLeptonTree["x2"]=m_GenEvtView->getUserRecord("x2");
        //mLeptonTree["qscale"]=m_GenEvtView->getUserRecord("Q");
    }
    //general*/
    mLeptonTree["ThisWeight"]=weight;

    //needed when using different leptons
    /*//mLeptonTree["lepton_type"]=sel_lepton->getUserRecord("id");
    mLeptonTree["lepton_type"]=sel_lepton->getPdgNumber();
    if(abs(sel_lepton->getPdgNumber())==11){
        mLeptonTree["highEtEleTrig"]=highEtTriggStored;
    }else{
        mLeptonTree["highEtEleTrig"]=0;
    }*/
    if (doTrees) helper->Tree_Filler("slimtree");
}

//void MiniAODAnalyzer::Fill_QCD_Tree(bool iso,TLorentzVector sel_lepton, const pat::MET sel_met, double weight){
void MiniAODAnalyzer::Fill_QCD_Tree(bool iso,TLorentzVector sel_lepton, int lepton_n, edm::Handle<std::vector<pat::Jet>> JetList, const pat::MET sel_met, double weight,int ifake,int tauDM, int nTRK, double chIso, double neuIso){
  //  std::cout << "Inside Fill_QCD_Tree" << std::endl;
  float jetHT=0.0;
  for( auto myJet : *JetList){
    if ( myJet.pt()>30 ) {
      jetHT = jetHT+myJet.pt();
    }
  }

  //  if (jetHT>0)  std::cout << "jetHT = " << jetHT << std::endl;

    if(iso){
      //      std::cout << "Isolated" << std::endl;
        mQCDTree["lepton_n"]=lepton_n;
        mQCDTree["iso"]=iso;
        mQCDTree["ThisWeight"]=weight;
        mQCDTree["QCDWeight"]=1.;
	mQCDTree["ntrack"]=nTRK;
	mQCDTree["ChargedIso"]=chIso;
	mQCDTree["NeutralIso"]=neuIso;
        mQCDTree["met"]=sel_met.pt();
        mQCDTree["met_phi"]=sel_met.phi();
        mQCDTree["jetHT"]=jetHT;
        mQCDTree["decay_mode"]=tauDM;
      	//std::cout << "tau pt = " << sel_lepton.Pt() << std::endl;
        mQCDTree["pt"]=sel_lepton.Pt();
        mQCDTree["phi"]=sel_lepton.Phi();
        mQCDTree["mt"]=calcMT(sel_lepton,sel_met);
        mQCDTree["eta"]=sel_lepton.Eta();
        mQCDTree["delta_phi"]=deltaPhi(sel_lepton.Phi(),sel_met.phi());
        //mQCDTree["lepton_type"]=sel_lepton->getPdgNumber();
        mQCDTree["lepton_type"]=lepton_n;
        mQCDTree["minDphi"]=minDphiMET(*JetList,sel_met);

        //if(abs(sel_lepton->getPdgNumber())==15){
        //if(lepton_n==15){
            //mQCDTree["metTriggerd"]=sel_lepton->getUserRecord("metTriggered").toBool();
        //}
        //else{
	mQCDTree["metTriggerd"]=0;
        //}
        //mQCDTree["decay_mode"]=sel_lepton.decayMode();
        //if(abs(sel_lepton->getPdgNumber())==15 && m_do_complicated_tau_stuff){
            //for(auto idisc : d_mydisc){
                //mQCDTree[idisc]=sel_lepton->getUserRecord(idisc).toDouble();
            //}
        //}
        for(unsigned int ilep=1;ilep<3; ilep++){
            mQCDTree[ Form("mt_%d", ilep)   ]=0;
            mQCDTree[ Form("pt_%d", ilep)]=0;
            mQCDTree[ Form("eta_%d", ilep)]=0;
            mQCDTree[ Form("delta_phi_%d", ilep)]=0;
            mQCDTree[ Form("decay_mode_%d", ilep)]=0;
            //if(m_do_complicated_tau_stuff){
                //for(auto idisc : d_mydisc){
                    //mQCDTree[Form("%s_%d", idisc.c_str() ,ilep ) ]=0;
                //}
            //}
        }
	if (doTrees)  helper->Tree_Filler("qcdtree");
    }else{
      //      std::cout << "non-isolated" << std::endl;
        //if(!std::isfinite(qcd_weight[qcd_lepton->getPdgNumber()][ifake])){
            //cout<<qcd_weight[qcd_lepton->getPdgNumber()][ifake]<<" "<<ifake<<endl;
        //}
        //fill for every qcd lepton up to 2 additonal leptons
      // tau_nTrack[nGoodTau]=tau.signalCands().size();
      // tau_chIso[nGoodTau]=tau.tauID("chargedIsoPtSum");
      // tau_neuIso[nGoodTau]=tau.tauID("neutralIsoPtSum");

        mQCDTree["lepton_n"]=lepton_n;
        mQCDTree["iso"]=iso;
        mQCDTree["ThisWeight"]=weight;
        mQCDTree["met"]=sel_met.pt();
        mQCDTree["met_phi"]=sel_met.phi();
	mQCDTree["jetHT"]=jetHT;
        mQCDTree["minDphi"]=minDphiMET(*JetList,sel_met);
        pat::Tau qcd_lepton=(*qcd_lepton_tau)[0];
        mQCDTree["QCDWeight"]=(*qcd_weight_tau)[ifake];
        mQCDTree["mt"]=calcMT(qcd_lepton,sel_met);
        mQCDTree["pt"]=qcd_lepton.pt();
	mQCDTree["ntrack"]=qcd_lepton.signalCands().size();
	mQCDTree["ChargedIso"]=qcd_lepton.tauID("chargedIsoPtSum");
	mQCDTree["NeutralIso"]=qcd_lepton.tauID("neutralIsoPtSum");
	mQCDTree["phi"]=qcd_lepton.phi();
        mQCDTree["eta"]=qcd_lepton.eta();
        mQCDTree["delta_phi"]=deltaPhi(qcd_lepton.phi(),sel_met.phi());
        //mQCDTree["lepton_type"]=qcd_lepton->getPdgNumber();
        mQCDTree["lepton_type"]=lepton_n;
        mQCDTree["metTriggerd"]=0;
        //if(abs(qcd_lepton->getPdgNumber())==15){
        //if(abs(lepton_n==15){
            //if(qcd_lepton->hasUserRecord("metTriggered")){
                //mQCDTree["metTriggerd"]=qcd_lepton->getUserRecord("metTriggered").toBool();
            //}else{
                //mQCDTree["metTriggerd"]=1.;
            //}
        //}
        mQCDTree["decay_mode"]=qcd_lepton.decayMode();
        //if(abs(qcd_lepton->getPdgNumber())==15 && m_do_complicated_tau_stuff){
            //for(auto idisc : d_mydisc){
                //mQCDTree[idisc]=qcd_lepton->getUserRecord(idisc).toDouble();
            //}
        //}
        for(unsigned int i_qcd=1;i_qcd<3; i_qcd++){
            mQCDTree[ Form("mt_%d", i_qcd)   ]=0;
            mQCDTree[ Form("pt_%d", i_qcd)]=0;
            mQCDTree[ Form("eta_%d", i_qcd)]=0;
            mQCDTree[ Form("delta_phi_%d", i_qcd)]=0;
            mQCDTree[Form("decay_mode_%d", i_qcd)]=0;
            //if(m_do_complicated_tau_stuff){
                //for(auto idisc : d_mydisc){
                    //mQCDTree[Form("%s_%d", idisc.c_str(),i_qcd )]=0;
                //}
            //}
        }
        int i_qcd=1;
        for(auto i_qcd_lepton: *qcd_lepton_tau){
            //if(i_qcd_lepton== qcd_lepton){
                //continue;
            //}
            if(i_qcd> 2){
                break;
            }
            mQCDTree[ Form("mt_%d", i_qcd)   ]=calcMT(i_qcd_lepton,sel_met);
            mQCDTree[ Form("pt_%d", i_qcd)]=i_qcd_lepton.pt();
            mQCDTree[ Form("phi_%d", i_qcd)]=i_qcd_lepton.phi();
            mQCDTree[ Form("eta_%d", i_qcd)]=i_qcd_lepton.eta();
            mQCDTree[ Form("delta_phi_%d", i_qcd)]=deltaPhi(i_qcd_lepton.phi(),sel_met.phi());
            mQCDTree[Form("decay_mode_%d", i_qcd)]=i_qcd_lepton.decayMode();
	    mQCDTree[Form("ntrack_%d", i_qcd)]=i_qcd_lepton.signalCands().size();
	    mQCDTree[Form("ChargedIso_%d",i_qcd)]=i_qcd_lepton.tauID("chargedIsoPtSum");
	    mQCDTree[Form("NeutralIso_%d",i_qcd)]=i_qcd_lepton.tauID("neutralIsoPtSum");
	    //mQCDTree[Form("ntrack"]=qcd_lepton.signalCands().size();
            //~if(abs(i_qcd_lepton->getPdgNumber())==15 && m_do_complicated_tau_stuff){
                //~for(auto idisc : d_mydisc){
                    //~mQCDTree[Form("%s_%d", idisc.c_str(), i_qcd) ]=i_qcd_lepton->getUserRecord(idisc).toDouble();
                //~}
            //~}
            i_qcd++;
            //std::cout << "in" << std::endl;
        }
	if (doTrees) helper->Tree_Filler("qcdtree");
    }
}


double MiniAODAnalyzer::calcMT(TLorentzVector part1, TLorentzVector part2) {
    double mm = 2 * part1.Pt() * part2.Pt() * ( 1. - cos(deltaPhi(part1.Phi(),part2.Phi()) ));
    return sqrt(mm);
}
double MiniAODAnalyzer::calcMT(TLorentzVector part1, const pat::MET part2) {
    TLorentzVector tempPart2P4 (0,0,0,0);
    tempPart2P4.SetPxPyPzE(part2.px(),part2.py(),part2.pz(),part2.energy());
    return calcMT(part1,tempPart2P4);
}

double MiniAODAnalyzer::calcMT(TLorentzVector part1, const pat::MET part2, pat::MET::METUncertainty metUncert) {
    double mm = 2 * part1.Pt() * part2.shiftedPt(metUncert) * ( 1. - cos(deltaPhi(part1.Phi(),part2.shiftedPhi(metUncert)) ));
    return sqrt(mm);
}
double MiniAODAnalyzer::calcMT(const pat::Electron part1, const pat::MET part2) {

    TLorentzVector tempPart1P4 (0,0,0,0);
    tempPart1P4.SetPxPyPzE(part1.px(),part1.py(),part1.pz(),part1.energy());
    TLorentzVector tempPart2P4 (0,0,0,0);
    tempPart2P4.SetPxPyPzE(part2.px(),part2.py(),part2.pz(),part2.energy());
    return calcMT(tempPart1P4,tempPart2P4);
}
double MiniAODAnalyzer::calcMT(const pat::Muon part1, const pat::MET part2) {

    TLorentzVector tempPart1P4 (0,0,0,0);
    tempPart1P4.SetPxPyPzE(part1.px(),part1.py(),part1.pz(),part1.energy());
    TLorentzVector tempPart2P4 (0,0,0,0);
    tempPart2P4.SetPxPyPzE(part2.px(),part2.py(),part2.pz(),part2.energy());
    return calcMT(tempPart1P4,tempPart2P4);
}
double MiniAODAnalyzer::calcMT(const pat::Tau part1, const pat::MET part2) {

    TLorentzVector tempPart1P4 (0,0,0,0);
    tempPart1P4.SetPxPyPzE(part1.px(),part1.py(),part1.pz(),part1.energy());
    TLorentzVector tempPart2P4 (0,0,0,0);
    tempPart2P4.SetPxPyPzE(part2.px(),part2.py(),part2.pz(),part2.energy());
    return calcMT(tempPart1P4,tempPart2P4);
}

double MiniAODAnalyzer::DeltaR(auto part,auto tau){

    TLorentzVector tempPartP4 (0,0,0,0);
    tempPartP4.SetPxPyPzE(part.px(),part.py(),part.pz(),part.energy());
    TLorentzVector tempTauP4 (0,0,0,0);
    tempTauP4.SetPxPyPzE(tau.px(),tau.py(),tau.pz(),tau.energy());
    return tempTauP4.DeltaR(tempPartP4);
}
double MiniAODAnalyzer::DeltaR(pat::Muon part,pat::Tau tau){

    TLorentzVector tempPartP4 (0,0,0,0);
    tempPartP4.SetPxPyPzE(part.px(),part.py(),part.pz(),part.energy());
    TLorentzVector tempTauP4 (0,0,0,0);
    tempTauP4.SetPxPyPzE(tau.px(),tau.py(),tau.pz(),tau.energy());
    return tempTauP4.DeltaR(tempPartP4);
}
double MiniAODAnalyzer::DeltaR(pat::Electron part,pat::Tau tau){

    TLorentzVector tempPartP4 (0,0,0,0);
    tempPartP4.SetPxPyPzE(part.px(),part.py(),part.pz(),part.energy());
    TLorentzVector tempTauP4 (0,0,0,0);
    tempTauP4.SetPxPyPzE(tau.px(),tau.py(),tau.pz(),tau.energy());
    return tempTauP4.DeltaR(tempPartP4);
}
double MiniAODAnalyzer::DeltaPhi(pat::Electron part1, pat::MET part2){
    return deltaPhi(part1.phi(),part2.phi());
}
double MiniAODAnalyzer::DeltaPhi(pat::Muon part1, pat::MET part2){
    return deltaPhi(part1.phi(),part2.phi());
}
double MiniAODAnalyzer::DeltaPhi(pat::Tau part1, pat::MET part2){
    return deltaPhi(part1.phi(),part2.phi());
}
double MiniAODAnalyzer::minDphiMET(std::vector<pat::Jet> JetList, const pat::MET sel_met){
    //if(m_minDPhi!=0){
        //return m_minDPhi;
    //}
    double minDphi=TMath::Pi();
    for( auto particle : JetList){
        if(particle.pt()>50 and deltaPhi(particle.phi(),sel_met.phi())<minDphi){
            minDphi=deltaPhi(particle.phi(),sel_met.phi());
        }
    }
    //m_minDPhi=minDphi;
    return minDphi;
    //return 1;
}
/*
 * get gen variables from LHE header of miniAOD
 * gen mass of  w, save in variable,
 * so you can go as often through the event as you like,
 * without losing time
 * ht: gen level
 * ht: reco level
 *
 * */


double MiniAODAnalyzer::applyWKfactor(int mode, edm::Handle<edm::View<reco::GenParticle>> genPart){

    WtoInt=sourceFileString.find("WTo");
    WJetsInt=sourceFileString.find("WJets");
    //std::cout << std::endl<< sourceFileString << " "<< WtoInt
    //<< " " << WJetsInt    <<  " " << std::string::npos << std::endl;
    //std::cout << sourceFileString.find("Wto") << std::endl;
    bool applyKfactor=false;
    if (WtoInt!=std::string::npos or WJetsInt!=std::string::npos){
        applyKfactor=true;
    }
    //std::cout << applyKfactor << std::endl;

    ///--- mode 1 (multiplicative) is legacy and should not be used ---///
    if( not (mode==1 || mode==0) ){
        throw std::runtime_error("specialAna.cc: The k-faktor must be additive (mode=0) or multiplicative (mode=1) yours is "+std::to_string(mode));
    }

    if (applyKfactor==true){
        double mass=getWmass(genPart);
        int leptonID=getWdecay(genPart);
        double k_faktor=1.;
        if(mass>0){
            if(leptonID==11){
                k_faktor=m_kfactorHist_ele[mode]->GetBinContent(m_kfactorHist_ele[mode]->FindBin(mass));
            }else if(leptonID==13){
                k_faktor=m_kfactorHist_muo[mode]->GetBinContent(m_kfactorHist_muo[mode]->FindBin(mass));
            }else if(leptonID==15){
                k_faktor=m_kfactorHist_tau[mode]->GetBinContent(m_kfactorHist_tau[mode]->FindBin(mass));
            }
        }
        if (k_faktor<0){
            k_faktor=1.;
        }
        //weight*=k_faktor;
        return k_faktor;
    }
    else{
        return 100;
    }
}


int MiniAODAnalyzer::getWdecay(edm::Handle<edm::View<reco::GenParticle>> genPart){
    int temp_id=0;
    for(size_t i=0; i<genPart->size();i++){
        if (abs((*genPart)[i].pdgId())==15 or abs((*genPart)[i].pdgId())==13 or abs((*genPart)[i].pdgId())==11){
            temp_id=abs((*genPart)[i].pdgId());
            break;
        }
    }
    return temp_id;
}

bool MiniAODAnalyzer::Overlap(edm::Handle<edm::View<reco::GenParticle>> p1, double HT, double MTT) {
  if ( (sourceFileString.find("WJetsToLNu_Tune") != std::string::npos) && ( (getWmass(p1)>100) || (HT>100.0) ) ) return true;
  if ( (sourceFileString.find("WJetsToLNu_HT") != std::string::npos) && (getWmass(p1)>100) && (HT<100.0)  ) return true;
  if ( (sourceFileString.find("WToTauNu_M-100_") != std::string::npos) && ( (getWmass(p1)<100) || (getWmass(p1)>200)) ) return true;
  if ( (sourceFileString.find("WToTauNu_M-200_") != std::string::npos) && ( (getWmass(p1)<200) || (getWmass(p1)>500)) ) return true;
  if ( (sourceFileString.find("WToTauNu_M-500_") != std::string::npos) && ( (getWmass(p1)<500) || (getWmass(p1)>1000)) ) return true;
  if ( (sourceFileString.find("WToTauNu_M-1000_") != std::string::npos) && ( (getWmass(p1)<1000) || (getWmass(p1)>2000)) ) return true;
  if ( (sourceFileString.find("WToTauNu_M-2000_") != std::string::npos) && ( (getWmass(p1)<2000) || (getWmass(p1)>3000)) ) return true;
  if ( (sourceFileString.find("WToTauNu_M-3000_") != std::string::npos) && ( (getWmass(p1)<3000) || (getWmass(p1)>4000)) ) return true;
  if ( (sourceFileString.find("WToTauNu_M-4000_") != std::string::npos) && ( (getWmass(p1)<4000) || (getWmass(p1)>5000)) ) return true;
  if ( (sourceFileString.find("WToTauNu_M-5000_") != std::string::npos) && ( (getWmass(p1)<5000) || (getWmass(p1)>6000)) ) return true;
  if ( (sourceFileString.find("WToTauNu_M-6000_") != std::string::npos) && ( (getWmass(p1)<6000)) ) return true;
  //  if ( (sourceFileString.find("TT_Tune") != std::string::npos) && (genMTT>700) ) return true;
  // if ( (sourceFileString.find("TT_Mtt-700to1000_Tune") != std::string::npos) && ( (genMTT<700) || (genMTT>1000) ) ) return true;
  // if ( (sourceFileString.find("TT_Mtt-1000toInf_Tune") != std::string::npos) && (genMTT<1000) ) return true;
  
  return false;
}

double MiniAODAnalyzer::getWmass(edm::Handle<edm::View<reco::GenParticle>> genPart){
    //std::cout << "wmass_stored is " << wmass_stored << std::endl;
    if(wmass_stored!=0){
        return wmass_stored;
    }
    double temp_W_mass=0;
    bool First=false;
    bool second=false;
    int temp_i=0;

    for(size_t i=0; i<genPart->size();i++){
        if(abs((*genPart)[i].pdgId())==24 && First==false){
            //std::cout << "ID is " << (*genPart)[i].pdgId() << std::endl;
            //std::cout << "W mass from W is " << (*genPart)[i].mass() << std::endl;
            First=true;
            temp_W_mass=(*genPart)[i].mass();
            break;
        }
        if (second==false){
            if (abs((*genPart)[i].pdgId())==15 or abs((*genPart)[i].pdgId())==13 or abs((*genPart)[i].pdgId())==11){
                temp_i=i;
                second=true;
                break;
            }
        }
    }
    if (second==true){
        for(size_t i=0; i<genPart->size();i++){
            int sign=(*genPart)[temp_i].pdgId()/abs((*genPart)[temp_i].pdgId());
            if ( (*genPart)[i].pdgId()==(-1*((*genPart)[temp_i].pdgId()+sign))){
                temp_W_mass=((*genPart)[i].p4()+(*genPart)[temp_i].p4()).mass();
                break;
            }
        }
    }
    wmass_stored=temp_W_mass;
    return wmass_stored;
}

void MiniAODAnalyzer::SetUpDisc(){
    ///tau discriminators
    d_mydisc->push_back("byLooseCombinedIsolationDeltaBetaCorr3Hits");
    d_mydisc->push_back("byLooseCombinedIsolationDeltaBetaCorr3HitsdR03");
    d_mydisc->push_back("byLooseIsolationMVA3newDMwLT");
    d_mydisc->push_back("byLooseIsolationMVArun2v1DBnewDMwLT");
    d_mydisc->push_back("byLooseIsolationMVArun2v1PWnewDMwLT");
    d_mydisc->push_back("byLoosePileupWeightedIsolation3Hits");
    d_mydisc->push_back("byMediumCombinedIsolationDeltaBetaCorr3Hits");
    d_mydisc->push_back("byMediumCombinedIsolationDeltaBetaCorr3HitsdR03");
    d_mydisc->push_back("byMediumIsolationMVA3newDMwLT");
    d_mydisc->push_back("byMediumIsolationMVArun2v1DBoldDMwLT");
    d_mydisc->push_back("byMediumIsolationMVArun2v1PWnewDMwLT");
    d_mydisc->push_back("byMediumPileupWeightedIsolation3Hits");
    d_mydisc->push_back("byTightCombinedIsolationDeltaBetaCorr3Hits");
    d_mydisc->push_back("byTightCombinedIsolationDeltaBetaCorr3HitsdR03");
    d_mydisc->push_back("byTightIsolationMVA3newDMwLT");
    d_mydisc->push_back("byTightIsolationMVArun2v1DBnewDMwLT");
    d_mydisc->push_back("byTightIsolationMVArun2v1PWnewDMwLT");
    d_mydisc->push_back("byTightPileupWeightedIsolation3Hits");
    d_mydisc->push_back("byVLooseIsolationMVA3newDMwLT");
    d_mydisc->push_back("byVLooseIsolationMVArun2v1DBnewDMwLT");
    d_mydisc->push_back("byVLooseIsolationMVArun2v1PWnewDMwLT");
    d_mydisc->push_back("byVTightIsolationMVA3newDMwLT");
    d_mydisc->push_back("byVTightIsolationMVArun2v1DBnewDMwLT");
    d_mydisc->push_back("byVTightIsolationMVArun2v1PWnewDMwLT");
    d_mydisc->push_back("byVVTightIsolationMVA3newDMwLT");
    d_mydisc->push_back("byVVTightIsolationMVArun2v1DBnewDMwLT");
    d_mydisc->push_back("byVVTightIsolationMVArun2v1PWnewDMwLT");

    ///electron and muon discriminators
    d_mydisc_emu->push_back("againstElectronLooseMVA5");
    d_mydisc_emu->push_back("againstElectronLooseMVA6");
    d_mydisc_emu->push_back("againstElectronMediumMVA5");
    d_mydisc_emu->push_back("againstElectronMediumMVA6");
    d_mydisc_emu->push_back("againstElectronTightMVA5");
    d_mydisc_emu->push_back("againstElectronTightMVA6");
    d_mydisc_emu->push_back("againstElectronVLooseMVA5");
    d_mydisc_emu->push_back("againstElectronVLooseMVA6");
    d_mydisc_emu->push_back("againstElectronVTightMVA5");
    d_mydisc_emu->push_back("againstElectronVTightMVA6");
    d_mydisc_emu->push_back("againstMuonLoose3");
    d_mydisc_emu->push_back("againstMuonTight3");

    ///muon discriminators
    d_mydisc_mu->push_back("isPFMuon");
    d_mydisc_mu->push_back("isGlobalMuon");
    d_mydisc_mu->push_back("isTrackerMuon");
    d_mydisc_mu->push_back("isStandAloneMuon");
    d_mydisc_mu->push_back("isLooseMuon");
    d_mydisc_mu->push_back("isHighPtMuon");
}

void MiniAODAnalyzer::QCDAnalyse(const pat::MET sel_met) {
    //inverted isolation
        //inverted isolation

    double m_leptonVetoPt=20;
    double vetoConeSize=0.3;
    int numVetoEle=vetoNumberEle(m_leptonVetoPt, vetoConeSize);
    int numVetoMuo=vetoNumberMuon(m_leptonVetoPt, vetoConeSize);
    //int numVetoTau=vetoNumberTau(m_leptonVetoPt);


    if( numVetoEle==0 && taus->size()>=1 && numVetoMuo==0 ){
        int passedID=0;
        std::map<std::string,int> passedID_disc;
        for(std::vector<std::string>::iterator disc_it = d_mydisc->begin(); disc_it != d_mydisc->end(); disc_it++){
            passedID_disc[(*disc_it)]=0;
        }
        for( pat::Tau it: *taus ) {

            if(m_do_complicated_tau_stuff){
                for(std::vector<std::string>::iterator disc_it = d_mydisc->begin(); disc_it != d_mydisc->end(); disc_it++){
                    if(PassTauID_NonIsolated(it,*disc_it)){
                        passedID_disc[(*disc_it)]++;
                        //(*it)->setPdgNumber(15);
                        qcd_lepton_tau->push_back(it);
                    }
                }
            }else{
                if( PassTauID_NonIsolated(it) ){
                    passedID++;
                    //(*it)->setPdgNumber(15);
                    qcd_lepton_tau->push_back(it);
                }
            }
        }
        //std::cout << passedID << "tau" << std::endl;
    }

    if(qcd_lepton_tau->size()>0){
        unsigned int counter = 0;
        for(pat::Tau i_qcd_lepton: *qcd_lepton_tau){
            //check_single_tau_kinematics(i_qcd_lepton,sel_met);
            //if(!i_qcd_lepton->getUserRecord("passedBasicKin").toBool()){
            if (i_qcd_lepton.pt()<50){
                qcd_weight_tau->push_back(0.);
            }else{
                //double content= hist_qcd_weight_tau->GetBinContent(hist_qcd_weight_tau->FindBin(i_qcd_lepton.pt(),i_qcd_lepton.eta()));
                //qcd_weight_tau->push_back(max(0., content ));
                qcd_weight_tau->push_back(std::max(0., hist_qcd_weight_tau->GetBinContent(hist_qcd_weight_tau->FindBin(i_qcd_lepton.pt(),i_qcd_lepton.eta()))));
                //~qcd_weight[qcd_id].push_back(max(0.,qcd_weight_tau_pt->GetBinContent(qcd_weight_tau_pt->FindBin(i_qcd_lepton->getPt()  ))));
            }
            //double hist_content;
            for(unsigned int i=0;  i<qcd_lepton_tau->size();i++){
                //if (qcd_lepton_tau[i]==i_qcd_lepton){
                if (i==counter){
                    continue;
                }
                //~qcd_weight[qcd_id].back()*=(1-max(0.,qcd_weight_tau_pt->GetBinContent(qcd_weight_tau_pt->FindBin(qcd_leptons[qcd_id][i]->getPt(),i_qcd_lepton->getUserRecord("decayMode")))));
                //hist_content=hist_qcd_weight_tau->GetBinContent(hist_qcd_weight_tau->FindBin(i_qcd_lepton.pt(),i_qcd_lepton.eta()  ));
                //qcd_weight_tau->push_back()*=(1-max(0.,hist_content ));
                qcd_weight_tau->back()*=(1-std::max(0.,hist_qcd_weight_tau->GetBinContent(hist_qcd_weight_tau->FindBin(i_qcd_lepton.pt(),i_qcd_lepton.eta()  ))));
            }
            counter++;
            //cout<<qcd_weight[qcd_id].back()<<endl;
        }

    }

}



bool MiniAODAnalyzer::check_single_tau_kinematics(pat::Tau lepton, const pat::MET sel_met){
    ///turns out not to be needed atm
    bool passed=false;
    bool passedPtMet=false;
    bool passedDeltaPhi=false;

    double m_met_cut=200;
    double m_delta_phi_cut=2.4;
    double m_pt_met_min_cut=0.7;
    double m_pt_met_max_cut=1.3;
    //if(sel_met && lepton && lepton.pt()>m_pt_min_cut){
    //if(sel_met && lepton && sel_met.pt()>m_met_cut){
    if(sel_met.pt()>m_met_cut){

        if(lepton.pt()/sel_met.pt()>m_pt_met_min_cut && lepton.pt()/sel_met.pt()<m_pt_met_max_cut){
            passedPtMet=true;
        }
        if(deltaPhi(lepton.phi(),sel_met.phi())>m_delta_phi_cut){
            passedDeltaPhi=true;
        }
        if (passedDeltaPhi && passedPtMet){
            passed=true;
        }
    }
    return passed;
}

int MiniAODAnalyzer::vetoNumberEle(double ptTreshold,double vetoConeSize){
    //make veto numbers
    //we don't need std::vectors, do we?
    int numVeto=0;
    int posi=0;
    for( auto part: *electrons) {
      if( part.pt()>ptTreshold && (EleIDPassed->at(posi)==1) ){
	numVeto++;
      }
      else if(part.pt()<ptTreshold) {
        ///    Lists are Pt sorted
	break;
      }
      posi++;
    }
    return numVeto;
}

int MiniAODAnalyzer::vetoNumberMuon(double ptTreshold,double vetoConeSize){
    //make veto numbers
    //we don't need std::vectors, do we?
    int numVeto=0;
    int posi=0;
    for( auto part: *muons) {
      if( part.pt()>ptTreshold && (MuonIDPassed->at(posi)==1) ){
	numVeto++;
      }
      else if(part.pt()<ptTreshold) {
        ///    Lists are Pt sorted
	break;
      }
      posi++;
    }
    return numVeto;
    
}


int MiniAODAnalyzer::vetoNumberTau(double ptTreshold){
    //make veto numbers
    //we don't need std::vectors, do we?
    int numVeto=0;
    bool passedID=false;
    for (auto tau: *taus){
        passedID=false;
        passedID=PassTauID(tau);
        if( tau.pt()>ptTreshold && passedID ){
            numVeto++;
        }else if(tau.pt()<ptTreshold) {
            ///Lists are Pt sorted
            break;
        }
    }
    return numVeto;
}

/*
backup
void MiniAODAnalyzer::QCDAnalyse() {
    //inverted isolation
        //inverted isolation

    int qcd_id=0;


    double m_leptonVetoPt=20;
    double vetoConeSize=0.3;
    int numVetoEle=vetoNumberEle(m_leptonVetoPt, vetoConeSize);
    int numVetoMuo=vetoNumberMuon(m_leptonVetoPt, vetoConeSize);
    int numVetoTau=vetoNumberTau(m_leptonVetoPt);


    if( numVetoEle==0 && taus->size()>=1 && numVetoMuo==0 ){
        int passedID=0;
        std::map<std::string,int> passedID_disc;
        for(std::vector<std::string>::iterator disc_it = d_mydisc->begin(); disc_it != d_mydisc->end(); disc_it++){
            passedID_disc[(*disc_it)]=0;
        }
        for( pat::Tau it: *taus ) {

            if(m_do_complicated_tau_stuff){
                for(std::vector<std::string>::iterator disc_it = d_mydisc->begin(); disc_it != d_mydisc->end(); disc_it++){
                    if(PassTauID_NonIsolated(it,*disc_it)){
                        passedID_disc[(*disc_it)]++;
                        //(*it)->setPdgNumber(15);
                        qcd_lepton_tau->push_back(it);
                    }
                }
            }else{
                if( PassTauID_NonIsolated(it) ){
                    passedID++;
                    //(*it)->setPdgNumber(15);
                    qcd_lepton_tau->push_back(it);
                }
            }
        }
        std::cout << passedID << "tau" << std::endl;
    }

    ///may be needed for electron channel
    if( electrons->size()>=1 && numVetoTau==0 && numVetoMuo==0  ){
        int passedID=0;
        for( pat::Electron it: *electrons ) {

            //if( (it).electronID("loosIDnoISO")){
            std::cout << (it).electronID("loosIDnoISO") << std::endl;
                if ( ((it).electronID("loosIDnoISO") or (it).electronID("loosIDandISO") )){
                    passedID++;
                    //(*it)->setPdgNumber(11);
                    qcd_lepton_ele->push_back(it);
                }
            //}
        }
        std::cout<<passedID<<"ele" << std::endl;

    }

    ///not needed for now ///
    if( numVetoEle==0 && numVetoTau==0 && muons->size()>=1 && m_doMuo){
        int passedID=0;
        for( std::vector< pxl::Particle* >::iterator it = MuonList->begin(); it != MuonList->end(); ++it ) {
            if( (*it)->hasUserRecord("ISOfailed")){
                if (  (*it)->getUserRecord("ISOfailed").toBool() ){
                    passedID++;
                    //(*it)->setPdgNumber(13);
                    qcd_leptons[13].push_back(*it);
                }
            }
            //we can not do the fr if we tag the muons!!
            //else if (MuonList->size()==1){
                //passedID++;
                //tmpMuo=( pxl::Particle* ) MuonList->at(0);
                //break;
            //}
        }
    }


    if(qcd_lepton_tau->size()>0){
        //m_pt_min_cut=m_pt_min_cut_tau;
        //m_delta_phi_cut=m_delta_phi_cut_tau;
        //m_pt_met_min_cut=m_pt_met_min_cut_tau;
        //m_pt_met_max_cut=m_pt_met_max_cut_tau;


        ///met cut?
        //m_pt_min_cut=m_pt_min_cut_tau;
        m_delta_phi_cut=2.4;
        m_pt_met_min_cut=0.7;
        m_pt_met_max_cut=1.3;

        m_pt_met_min_cut_funk_root=m_pt_met_min_cut_funk_root_tau;
        m_pt_met_max_cut_funk_root=m_pt_met_max_cut_funk_root_tau;
        m_delta_phi_cut_funk_root=m_delta_phi_cut_funk_root_tau;
        qcd_id=15;
        for(pat::Tau i_qcd_lepton: qcd_lepton_tau){
            check_single_lepton_kinematics(i_qcd_lepton,qcd_id);
            if(!i_qcd_lepton->getUserRecord("passedBasicKin").toBool()){
                qcd_weight[qcd_id].push_back(0.);
            }else{
                qcd_weight[qcd_id].push_back(max(0.,qcd_weight_tau_pt->GetBinContent(qcd_weight_tau_pt->FindBin(i_qcd_lepton->getPt(),i_qcd_lepton->getEta()  ))));
                //qcd_weight[qcd_id].push_back(max(0.,qcd_weight_tau_pt->GetBinContent(qcd_weight_tau_pt->FindBin(i_qcd_lepton->getPt()  ))));
            }
            for(unsigned int i=0;  i<qcd_leptons[15].size();i++){
                if (qcd_leptons[qcd_id][i]==i_qcd_lepton){
                    continue;
                }
                //qcd_weight[qcd_id].back()*=(1-max(0.,qcd_weight_tau_pt->GetBinContent(qcd_weight_tau_pt->FindBin(qcd_leptons[qcd_id][i]->getPt(),i_qcd_lepton->getUserRecord("decayMode")))));
                qcd_weight[qcd_id].back()*=(1-max(0.,qcd_weight_tau_pt->GetBinContent(qcd_weight_tau_pt->FindBin(i_qcd_lepton->getPt(),i_qcd_lepton->getEta()  ))));
            }
            //cout<<qcd_weight[qcd_id].back()<<endl;
        }

    }

    if(qcd_leptons[11].size()>0){
        m_pt_min_cut=m_pt_min_cut_ele;
        m_delta_phi_cut=m_delta_phi_cut_ele;
        m_pt_met_min_cut=m_pt_met_min_cut_ele;
        m_pt_met_max_cut=m_pt_met_max_cut_ele;

        m_pt_met_min_cut_funk_root=m_pt_met_min_cut_funk_root_ele;
        m_pt_met_max_cut_funk_root=m_pt_met_max_cut_funk_root_ele;
        m_delta_phi_cut_funk_root=m_delta_phi_cut_funk_root_ele;
        qcd_id=11;
        for(pxl::Particle* i_qcd_lepton: qcd_leptons[qcd_id]){
            check_single_lepton_kinematics(i_qcd_lepton,qcd_id);
            qcd_weight[qcd_id].push_back(max(0.,qcd_weight_ele_pt->GetBinContent(qcd_weight_ele_pt->FindBin(i_qcd_lepton->getPt()))));
            for(unsigned int i=0;  i<qcd_leptons[qcd_id].size();i++){
                if (qcd_leptons[qcd_id][i]==i_qcd_lepton){
                    continue;
                }
                qcd_weight[qcd_id].back()*=(1-max(0.,qcd_weight_ele_pt->GetBinContent(qcd_weight_ele_pt->FindBin(qcd_leptons[qcd_id][i]->getPt()))));
            }
        }
    }

    if(qcd_leptons[13].size()>0){
        m_pt_min_cut=m_pt_min_cut_muo;
        m_delta_phi_cut=m_delta_phi_cut_muo;
        m_pt_met_min_cut=m_pt_met_min_cut_muo;
        m_pt_met_max_cut=m_pt_met_max_cut_muo;

        m_pt_met_min_cut_funk_root=m_pt_met_min_cut_funk_root_muo;
        m_pt_met_max_cut_funk_root=m_pt_met_max_cut_funk_root_muo;
        m_delta_phi_cut_funk_root=m_delta_phi_cut_funk_root_muo;
        qcd_id=13;
        for(pxl::Particle* i_qcd_lepton: qcd_leptons[qcd_id]){
            check_single_lepton_kinematics(i_qcd_lepton,qcd_id);
            qcd_weight[qcd_id].push_back(max(0.,qcd_weight_muo_pt->GetBinContent(qcd_weight_muo_pt->FindBin(i_qcd_lepton->getPt()))));
            for(unsigned int i=0;  i<qcd_leptons[qcd_id].size();i++){
                if (qcd_leptons[qcd_id][i]==i_qcd_lepton){
                    continue;
                }
                qcd_weight[qcd_id].back()*=(1-max(0.,qcd_weight_muo_pt->GetBinContent(qcd_weight_muo_pt->FindBin(qcd_leptons[qcd_id][i]->getPt()))));
            }
        }
    }



}
*/
void MiniAODAnalyzer::QCDAnalyseTau( const pat::MET sel_met,double weight,edm::Handle<edm::View<reco::GenParticle>> genPart) {
//void MiniAODAnalyzer::QCDAnalyseTau( const pat::MET sel_met) {

    double ptTauTreshold=80;
    double m_leptonVetoPt=20;
    double vetoConeSize=0.3;
    //int numVetoMuo=vetoNumberMuon(MuonList, m_leptonVetoPt,TauList,vetoConeSize);
    int numVetoMuo=vetoNumberMuon(m_leptonVetoPt,vetoConeSize);
    int numVetoTau=vetoNumberTau(ptTauTreshold);
    int numVetoEle=vetoNumberEle(m_leptonVetoPt,vetoConeSize);
    // std::cout << "In QCDAnalyseTau" << std::endl;
    //  std::cout << "nEle " << numVetoEle << "  nMu " << numVetoMuo << " taus size "<<taus->size()<< std::endl;
    if( ( (numVetoEle==1) xor (numVetoMuo==1 )) && taus->size()>=1){
      //std::cout << "tau size > 0 and  Ele Muo cut fulfilled " << std::endl;
      pat::Electron eleCandi;
      pat::Muon muoCandi;
      int i=0;
      bool eleBool=false;
      for( auto part: *electrons) {
	if( part.pt()>m_leptonVetoPt && EleIDPassed->at(i)==1 ){
	  eleCandi=(part);
	  eleBool=true;				
	  // std::cout << "eleBool=" << eleBool << std::endl;
	  break;
	}
	i++;
      }
      //        if (eleBool) std::cout << "electron passed " << std::endl;
      i=0;
      bool muonBool=false;
      //for( auto part: MuonList) {
      for( auto part: *muons) {
	if( part.pt()>m_leptonVetoPt && MuonIDPassed->at(i)==1 ){
	  muoCandi=(part);
	  muonBool=true;
	  // std::cout << "muonBool=" << muonBool << std::endl;
	  break;
	}
	i++;
      }
      if(eleBool==false and muonBool==false){
	//std::cout << "Event do not contain ele/muo" << std::endl; 
	return;
      }
      /// make it shorter
      FakeCandPt->clear();
      FakeCandPhi->clear();
      FakeCandEta->clear();
      FakeCandMET->clear();
      FakeCandMETPhi->clear();
      FakeCandDeltaPhi->clear();
      FakeCandIso->clear();
      FakeCandNoIso->clear();
        FakeCandVetoNum->clear();
        FakeCandLorentz->clear();
        FakeCandDecayMode->clear();
        FakeCandLeptonType->clear();
        FakeCandLeptonMt->clear();
        FakeCandLeptonDeltaPhi->clear();
        FakeCandLeptonPt->clear();
        FakeCandLeptonEta->clear();
        FakeCandLeptonPhi->clear();
        FakeCandLeptonTrigger->clear();
        for( auto tau: *taus ) {
	  //  std::cout << "\n Inside Tau Loop. Take this tau" << std::endl; 
            if(muonBool && DeltaR(muoCandi,tau)<vetoConeSize){
	      //    std::cout << "Muon too close to tau" << std::endl;
                continue;
            }
            if(eleBool &&  DeltaR(eleCandi,tau)<vetoConeSize){
	      // std::cout << "Ele too close to tau" << std::endl;
                continue;
            }

            if( not PassTauID_NonIsolated(tau)  and not PassTauID(tau)){
	      // std::cout << "tau do not pass ID (iso and noniso) " << std::endl; 
                continue;
            }
            if(!m_do_complicated_tau_stuff){
                //cout<<"ele   "<<eleTrig<<"   "<<eleCandi<<endl;
                //cout<<"muon   "<<muoTrig<<"   "<<muoCandi<<endl;
                //cout<<"------------------------- "<<endl;

	      //	      std::cout << "not complicated tau loop " << std::endl;
	      mFakeTree["mt"]=calcMT(tau,sel_met);
	      FakeCandMt->push_back(calcMT(tau,sel_met));
	      mFakeTree["delta_phi"]=DeltaPhi(tau,sel_met);
	      FakeCandDeltaPhi->push_back(DeltaPhi(tau,sel_met));
	      mFakeTree["pt"]=tau.pt();
	      FakeCandPt->push_back(tau.pt());
	      mFakeTree["met"]=sel_met.pt();
	      FakeCandMET->push_back(sel_met.pt());
	      mFakeTree["phi"]=tau.phi();
	      FakeCandPhi->push_back(tau.phi());
	      mFakeTree["eta"]=tau.eta();
	      FakeCandEta->push_back(tau.eta());
	      mFakeTree["met_phi"]=sel_met.phi();
	      FakeCandMETPhi->push_back(sel_met.phi());
	      mFakeTree["iso"]=PassTauID(tau);
	      FakeCandIso->push_back(PassTauID(tau));
	      mFakeTree["noiso"]=PassTauID_NonIsolated(tau);
	      FakeCandNoIso->push_back(PassTauID_NonIsolated(tau));
	      mFakeTree["tau_n"]=numVetoTau;
	      FakeCandVetoNum->push_back(numVetoTau);
	      mFakeTree["decay_mode"]=tau.decayMode();
	      FakeCandDecayMode->push_back(tau.decayMode());
	      
	      
	      
	      /*                mFakeTree["truth_match"]=(not runOnData and Get_Truth_match("Tau",*it)) ;
				if((*it)->hasUserRecord("decayMode")){
				mFakeTree["decay_mode"]=(*it)->getUserRecord("decayMode").toDouble();
				}else{
				mFakeTree["decay_mode"]=-1;
				}
	      */
	      if(muonBool){
		mFakeTree["lepton_type"]=13;
		FakeCandLeptonType->push_back(13);
		mFakeTree["lepton_mt"]=calcMT(muoCandi,sel_met);
		FakeCandLeptonMt->push_back(calcMT(muoCandi,sel_met));
		mFakeTree["lepton_delta_phi"]=DeltaPhi(muoCandi,sel_met);
		FakeCandLeptonDeltaPhi->push_back(DeltaPhi(muoCandi,sel_met));
		mFakeTree["lepton_pt"]=muoCandi.pt();
		FakeCandLeptonPt->push_back(muoCandi.pt());
		mFakeTree["lepton_eta"]=muoCandi.eta();
		FakeCandLeptonEta->push_back(muoCandi.eta());
		mFakeTree["lepton_phi"]=muoCandi.phi();
		FakeCandLeptonPhi->push_back(muoCandi.phi());
		mFakeTree["lepton_trigger"]=passMuonTrig;
		FakeCandLeptonTrigger->push_back(passMuonTrig);
	      }
	      if (eleBool){
		mFakeTree["lepton_type"]=11;
		FakeCandLeptonType->push_back(11);
		mFakeTree["lepton_mt"]=calcMT(eleCandi,sel_met);
		FakeCandLeptonMt->push_back(calcMT(eleCandi,sel_met));
		mFakeTree["lepton_delta_phi"]=DeltaPhi(eleCandi,sel_met);
		FakeCandLeptonDeltaPhi->push_back(DeltaPhi(eleCandi,sel_met));
		mFakeTree["lepton_pt"]=eleCandi.pt();
		FakeCandLeptonPt->push_back(eleCandi.pt());
		mFakeTree["lepton_eta"]=eleCandi.eta();
		FakeCandLeptonEta->push_back(eleCandi.eta());
		mFakeTree["lepton_phi"]=eleCandi.phi();
		FakeCandLeptonPhi->push_back(eleCandi.phi());
		mFakeTree["lepton_trigger"]=passEleTrig;
		FakeCandLeptonTrigger->push_back(passEleTrig);
	      }
	      
	      //general
	      mFakeTree["ThisWeight"]=weight;
	      
	      
	      if (doTrees) helper->Tree_Filler("fakeTree");
	      if(PassTauID_NonIsolated(tau)){
		//std::cout << "*****" << std::endl;
		//std::cout << "Non-isolated tau, fill tau_fake_pt 0" << std::endl;
		if (doFakeHist) helper->Fill(0,"Tau_fake_pt",tau.pt(),weight);
		if(muonBool) {
		  // std::cout << "muonBool True, fill tau_fake_pt 1" << std::endl;
		  if (doFakeHist) helper->Fill(1,"Tau_fake_pt",muoCandi.pt(),weight);
		}
		if(eleBool) {
		  //  std::cout << "eleBool True, fill tau_fake_pt 2" << std::endl;
		  if (doFakeHist) helper->Fill(2,"Tau_fake_pt",eleCandi.pt(),weight);
		}
		//	std::cout << "Non-isolated tau, fill Tau_fake 2D hists" << std::endl;
		if (doFakeHist)  helper->Fill( "Tau_fake_pt_eta",tau.pt(),tau.eta(),weight );
		if (doFakeHist)  helper->Fill( "Tau_fake_pt_met",tau.pt(),sel_met.pt(),weight );
		if (doFakeHist)  helper->Fill( "Tau_fake_pt_decay",tau.pt(),tau.decayMode(),weight );
		
		if(not RunOnData and (GetTruthMatch("Tau",tau)!=0)){
		  //std::cout << "filling true histograms" << std::endl;
		  if(muonBool) {
		    //  std::cout << "Truth-matched" << std::endl;
		    if (doFakeHist) helper->Fill(1,"Tau_fake_pt_true",muoCandi.pt(),weight);
		  }

		  if (doFakeHist) helper->Fill(3,"Tau_fake_pt_true",tau.pt(),weight);

		  if(eleBool) {
		    if (doFakeHist)  helper->Fill(2,"Tau_fake_pt_true",eleCandi.pt(),weight);
		  }
		  if (doFakeHist)  helper->Fill(4,"Tau_fake_pt_true",tau.pt(),weight);

		  if (doFakeHist)  helper->Fill(0,"Tau_fake_pt_true",tau.pt(),weight);

		  if (doFakeHist)  helper->Fill( "Tau_fake_pt_eta_true",tau.pt(),tau.eta(),weight );
		  if (doFakeHist)  helper->Fill( "Tau_fake_pt_met_true",tau.pt(),sel_met.pt(),weight );
		  if(tau.decayMode()){
		    if (doFakeHist)  helper->Fill( "Tau_fake_pt_decay_true",tau.pt(),tau.decayMode(),weight );
		  }
		}else if(not RunOnData  and (GetTruthMatch("Tau",tau)==0)){
		  reco::GenParticle* genMatch= GetTruthMatchAllFlavor(tau);
		  if(genMatch){
		    //lightquarks
		    if(abs(genMatch->pdgId())<5){
		      if (doFakeHist)  helper->Fill(0,"Tau_fake_pt_lightquark",tau.pt(),weight);
		      //HistClass::Fill( "Tau_fake_pt_eta_lightquark",tau.pt(),tau.eta(),weight );
		      //HistClass::Fill( "Tau_fake_pt_met_lightquark",tau.pt(),sel_met.pt(),weight );
		      //b quarks
		    }else if(abs(genMatch->pdgId())==5){
		      if (doFakeHist)   helper->Fill(0,"Tau_fake_pt_bquark",tau.pt(),weight);
		      //HistClass::Fill( "Tau_fake_pt_eta_bquark",tau.pt(),tau.eta(),weight );
		      //HistClass::Fill( "Tau_fake_pt_met_bquark",tau.pt(),sel_met.pt(),weight );
		    }else if(abs(genMatch->pdgId())==9 or abs(genMatch->pdgId())==21){
		      if (doFakeHist)   helper->Fill(0,"Tau_fake_pt_gluon",tau.pt(),weight);
		      //HistClass::Fill( "Tau_fake_pt_eta_gluon",tau.pt(),tau.eta(),weight );
		      //HistClass::Fill( "Tau_fake_pt_met_gluon",tau.pt(),sel_met.pt(),weight );
		    }else if(abs(genMatch->pdgId())==11 or abs(genMatch->pdgId())==13){
		      if (doFakeHist)   helper->Fill(0,"Tau_fake_pt_lightlepton",tau.pt(),weight);
		      //HistClass::Fill( "Tau_fake_pt_eta_gluon",tau.pt(),tau.eta(),weight );
		      //HistClass::Fill( "Tau_fake_pt_met_gluon",tau.pt(),sel_met.pt(),weight );
		    }else{
		      if (doFakeHist)   helper->Fill(0,"Tau_fake_pt_unmatched",tau.pt(),weight);
		      //std::cout<<"unassociated match: "<<genMatch->pdgId()<<std::endl;
		    }
		  }
		}
	      }else if(PassTauID(tau)){
		//std::cout << "*****" << std::endl;
		//std::cout << "passTauID, fill Tau_nofake_pt" << std::endl;
		if (doFakeHist)   helper->Fill(0,"Tau_nofake_pt",tau.pt(),weight);
		if(muonBool) {
		  if (doFakeHist)  helper->Fill(1,"Tau_nofake_pt",muoCandi.pt(),weight);
		}
		if (doFakeHist)  helper->Fill(3,"Tau_nofake_pt",tau.pt(),weight);
		if(eleBool) {
		    if (doFakeHist)  helper->Fill(2,"Tau_nofake_pt",eleCandi.pt(),weight);
		  }
		  if (doFakeHist)  helper->Fill(4,"Tau_nofake_pt",tau.pt(),weight);
		  if (doFakeHist)  helper->Fill( "Tau_nofake_pt_eta",tau.pt(),tau.eta(), weight );
		  if (doFakeHist)  helper->Fill( "Tau_nofake_pt_met",tau.pt(),sel_met.pt(),weight );
		  if(tau.decayMode()){
		    if (doFakeHist) helper->Fill( "Tau_nofake_pt_decay",tau.pt(),tau.decayMode(), weight );
		  }
		  if(not RunOnData and GetTruthMatch("Tau",tau)!=0 ){
                    //if(not RunOnData  ){
		    if (doFakeHist)  helper->Fill(0,"Tau_nofake_pt_true",tau.pt(),weight);
		    if (doFakeHist)  helper->Fill( "Tau_nofake_pt_eta_true",tau.pt(),tau.eta(), weight );
		    if (doFakeHist)  helper->Fill( "Tau_nofake_pt_met_true",tau.pt(),sel_met.pt(),weight );
		    if(tau.decayMode()){
		      if (doFakeHist) helper->Fill( "Tau_nofake_pt_decay_true",tau.pt(),tau.decayMode(), weight );
		    }
		  }else if(not RunOnData and GetTruthMatch("Tau",tau)==0){
		    reco::GenParticle* genMatch= GetTruthMatchAllFlavor(tau);
		    if(genMatch){
		      //lightquarks
		      if(abs(genMatch->pdgId())<5){
			if (doFakeHist)  helper->Fill(0,"Tau_nofake_pt_lightquark",tau.pt(),weight);
			//helper->Fill( "Tau_nofake_pt_eta_lightquark",tau.pt(),tau.eta(),weight );
			//helper->Fill( "Tau_nofake_pt_met_lightquark",tau.pt(),sel_met.pt(),weight );
			//b quarks
		      }else if(abs(genMatch->pdgId())==5){
			if (doFakeHist)  helper->Fill(0,"Tau_nofake_pt_bquark",tau.pt(),weight);
			//helper->Fill( "Tau_nofake_pt_eta_bquark",tau.pt(),tau.eta(),weight );
			//helper->Fill( "Tau_nofake_pt_met_bquark",tau.pt(),sel_met.pt(),weight );
		      }else if(abs(genMatch->pdgId())==9 or abs(genMatch->pdgId())==21){
			if (doFakeHist)  helper->Fill(0,"Tau_nofake_pt_gluon",tau.pt(),weight);
			//helper->Fill( "Tau_nofake_pt_eta_gluon",tau.pt(),tau.eta(),weight );
			//helper->Fill( "Tau_nofake_pt_met_gluon",tau.pt(),sel_met.pt(),weight );
		      }else if(abs(genMatch->pdgId())==11 or abs(genMatch->pdgId())==13){
			if (doFakeHist)  helper->Fill(0,"Tau_nofake_pt_lightlepton",tau.pt(),weight);
			//helper->Fill( "Tau_nofake_pt_eta_gluon",tau.pt(),tau.eta(),weight );
			//helper->Fill( "Tau_nofake_pt_met_gluon",tau.pt(),sel_met.pt(),weight );
		      }else{
			if (doFakeHist)  helper->Fill(0,"Tau_nofake_pt_unmatched",tau.pt(),weight);
			//std::cout<<"unassociated match: "<<genMatch->pdgId()<<std::endl;
		      }
		    }
		  }
                }
            }/*else{//do complicated tau stuff
	       for(std::vector<std::string>::iterator disc_it = d_mydisc.begin(); disc_it != d_mydisc.end(); disc_it++){
	       //if(PassTauID_NonIsolated(tau,*disc_it)){
	       if(PassTauID_NonIsolated(tau)){
	       helper->Fill(0,Form("Tau_fake_pt_%s",(*disc_it).c_str()),tau.pt(),weight);
	       helper->Fill( Form("Tau_fake_pt_eta_%s",(*disc_it).c_str()),tau.pt(),tau.eta(),weight );
                        helper->Fill( Form("Tau_fake_pt_met_%s",(*disc_it).c_str()),tau.pt(),sel_met.pt(),weight );
                        if(tau.decayMode())
                            helper->Fill( Form("Tau_fake_pt_decay_%s",(*disc_it).c_str()),tau.pt(),tau.decayMode().toDouble(),weight );
                        if(not RunOnData and GetTruthMatch("Tau",tau) ){
                            helper->Fill(0,Form("Tau_fake_pt_%s_true",(*disc_it).c_str()),tau.pt(),weight);
                            helper->Fill( Form("Tau_fake_pt_eta_%s_true",(*disc_it).c_str()),tau.pt(),tau.eta(),weight );
                            helper->Fill( Form("Tau_fake_pt_met_%s_true",(*disc_it).c_str()),tau.pt(),sel_met.pt(),weight );
                            if(tau.decayMode())
                                helper->Fill( Form("Tau_fake_pt_decay_%s_true",(*disc_it).c_str()),tau.pt(),tau.decayMode().toDouble(),weight );
                        }
                    }
                    //else if(PassTauID(tau,*disc_it)){
                    else if(PassTauID(tau)){
                        helper->Fill(0,Form("Tau_nofake_pt_%s",(*disc_it).c_str()),tau.pt(),weight);
                        helper->Fill( Form("Tau_nofake_pt_eta_%s",(*disc_it).c_str()),tau.pt(),tau.eta(), weight );
                        helper->Fill( Form("Tau_fake_pt_met_%s",(*disc_it).c_str()),tau.pt(),sel_met.pt(),weight );
                        if(tau-decayMode())
                            helper->Fill( Form("Tau_nofake_pt_decay_%s",(*disc_it).c_str()),tau.pt(),tau.decayMode(), weight );
                        if(not RunOnData and Get_Truth_match("Tau",tau) ){
                            helper->Fill(0,Form("Tau_nofake_pt_%s_true",(*disc_it).c_str()),tau.pt(),weight);
                            helper->Fill( Form("Tau_nofake_pt_eta_%s_true",(*disc_it).c_str()),tau.pt(),tau.eta(), weight );
                            helper->Fill( Form("Tau_fake_pt_met_%s_true",(*disc_it).c_str()),tau.pt(),sel_met.pt(),weight );
                            if(tau.decayMode())
                                helper->Fill( Form("Tau_nofake_pt_decay_%s_true",(*disc_it).c_str()),tau.pt(),tau.decayMode(), weight );

                        }
                    }
                }
            }*/
        }

        //std::cout << FakeCandDeltaPhi.size() << " " << FakeCandPt.size() << std::endl;
        /*
        mFakeVectorTree["FakeCandPt"]=FakeCandPt;
        mFakeVectorTree["FakeCandPhi"]=FakeCandPhi;
        mFakeVectorTree["FakeCandEta"]=FakeCandEta;
        mFakeVectorTree["FakeCandMET"]=FakeCandMET;
        mFakeVectorTree["FakeCandMETPhi"]=FakeCandMETPhi;
        mFakeVectorTree["FakeCandDeltaPhi"]=FakeCandDeltaPhi;
        mFakeVectorTree["FakeCandIso"]=FakeCandIso;
        mFakeVectorTree["FakeCandNoIso"]=FakeCandNoIso;
        mFakeVectorTree["FakeCandVetoNum"]=FakeCandVetoNum;*/

        //std::cout << FakeCandEta[-1] << std::endl;
        //helper->Tree_Filler("fakeVectorTree");
    }
}

//reco::GenParticle MiniAODAnalyzer::GetTruthMatch(std::string name, auto lepton) {
reco::GenParticle* MiniAODAnalyzer::GetTruthMatch(std::string name, auto lepton) {
  // std::cout << "Inside GetTruthMatch" << std::endl;
    if (tauGenMatchMap.end()  !=tauGenMatchMap.find(&lepton)){
        return tauGenMatchMap[&lepton];
    }

    int part_temp_id = 0;
    if (name == "Tau") {
        part_temp_id = 15;
    } else if (name ==  "Muon") {
        part_temp_id = 13;
    } else if (name == "Ele") {
        part_temp_id = 11;
    }
    double temp_delta_r = 0.8;
    reco::GenParticle* gen_match=0;

    for (auto part_i: *pruned){
      //      if (TMath::Abs(part_i.pdgId()) == 15) std::cout << "Tau " << part_i.pdgId() << std::endl;
        if (part_temp_id != (TMath::Abs(part_i.pdgId())) ) continue;
        double test_delta_r = DeltaR(lepton,part_i);
	//	std::cout << "delta r " << test_delta_r << std::endl;
        if (test_delta_r < temp_delta_r) {
            temp_delta_r = test_delta_r;
            gen_match = (&part_i);
        }
    }
    /*
    if (gen_match){
        std::cout << "found something "
                <<"\n delta r" << temp_delta_r
                 <<  " gen_match " << gen_match
                 << " pt of gen_match " << gen_match->pt()
                 << " pt of lepton " << lepton.pt()
                 << " eta of gen_match " << gen_match->eta()
                 << " eta of lepton " << lepton.eta()
        << std::endl;
	}*/
    // else {
        //std::cout << "found nothing "
                  //<<  "\n delta r " << temp_delta_r
                 //<< " pt of lepton " << lepton.pt()
                 //<< " eta of lepton " << lepton.eta()
        //<< std::endl;
    // }

    tauGenMatchMap[&lepton]=gen_match;
    // std::cout << "returning " << tauGenMatchMap[&lepton] << std::endl;
    return tauGenMatchMap[&lepton];
}
reco::GenParticle* MiniAODAnalyzer::GetTruthMatchAllFlavor(auto lepton) {
    if (tauGenMatchMapAllFlav.end()  !=tauGenMatchMapAllFlav.find(&lepton)){
        return tauGenMatchMapAllFlav[&lepton];
    }

    double temp_delta_r = 0.5;
    reco::GenParticle* gen_match=0;
    for (auto part_i: *pruned){
        if ( abs(part_i.pdgId())<22 && abs(part_i.pdgId())!=12 && abs(part_i.pdgId())!=14 && abs(part_i.pdgId())!=16 && fabs(lepton.pt()-part_i.pt())/part_i.pt()<0.5 ) {
            double test_delta_r = DeltaR(lepton,part_i);
            if (test_delta_r < temp_delta_r ) {
                temp_delta_r = test_delta_r;
                gen_match = (&part_i);
            }
        }
    }

    tauGenMatchMapAllFlav[&lepton]=gen_match;
    return tauGenMatchMapAllFlav[&lepton];
}

/*
pxl::Particle* specialAna::Get_Truth_match_all_flavor(pxl::Particle* lepton) {
    if (gen_match_map_all_flav.end()  !=gen_match_map_all_flav.find(lepton)){
        return gen_match_map_all_flav[lepton];
    }
    double temp_delta_r = 0.5;
    pxl::Particle* gen_match = 0;
    for (std::vector< pxl::Particle* >::const_iterator part_it = S3ListGen->begin(); part_it != S3ListGen->end(); ++part_it) {
        pxl::Particle *part_i = *part_it;
        double test_delta_r = lepton->getVector().deltaR(part_i->getVector());
        if (test_delta_r < temp_delta_r && fabs(lepton->getPt()-part_i->getPt())/part_i->getPt()<0.5 && abs(part_i->getPdgNumber())<23 && abs(part_i->getPdgNumber())!=12 && abs(part_i->getPdgNumber())!=14 && abs(part_i->getPdgNumber())!=16 && abs(part_i->getPdgNumber())!=22 ) {
            temp_delta_r = test_delta_r;
            gen_match = part_i;
        }
    }
    gen_match_map_all_flav[lepton]=gen_match;
    return gen_match;
}*/


//define this as a plug-in
DEFINE_FWK_MODULE(MiniAODAnalyzer);
