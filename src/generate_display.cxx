#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>

#include "TFile.h"
#include "TTree.h"

int main(int argc, char** argv) {
  if (argc != 4) {
    std::cout << "Error: incorrect number of arguments." << std::endl;
    return 1;
  }
  std::string input_file_path = argv[1];
  std::string input_file_name = argv[2];
  std::string event_number = argv[3];
  //sets photon collection to be isolated tracks instead
  bool tk_mode = true;
  //map from pdgid to particle name
  std::unordered_map<int,std::string> particle_name;
  particle_name[1] = "d-";
  particle_name[-1] = "d+";
  particle_name[2] = "u+";
  particle_name[-2] = "u-";
  particle_name[3] = "s-";
  particle_name[-3] = "s+";
  particle_name[4] = "c+";
  particle_name[-4] = "c-";
  particle_name[5] = "b-";
  particle_name[-5] = "b+";
  particle_name[6] = "t+";
  particle_name[-6] = "t-";
  particle_name[11] = "e-";
  particle_name[-11] = "e+";
  particle_name[12] = "νe";
  particle_name[-12] = "νebar";
  particle_name[13] = "μ-";
  particle_name[-13] = "μ+";
  particle_name[14] = "νμ";
  particle_name[-14] = "νμbar";
  particle_name[15] = "τ-";
  particle_name[-15] = "τ+";
  particle_name[16] = "ντ";
  particle_name[-16] = "ντbar";
  particle_name[21] = "g";
  particle_name[22] = "γ";
  particle_name[23] = "Z";
  particle_name[24] = "W+";
  particle_name[-24] = "W-";
  particle_name[25] = "H";
  //temp, manually set things
  std::vector<float> *el_pt = 0;
  std::vector<float> *el_eta = 0;
  std::vector<float> *el_phi = 0;
  std::vector<bool> *el_sig = 0;
  std::vector<float> *mu_pt = 0;
  std::vector<float> *mu_eta = 0;
  std::vector<float> *mu_phi = 0;
  std::vector<bool> *mu_sig = 0;
  std::vector<float> *photon_pt = 0;
  std::vector<float> *photon_eta = 0;
  std::vector<float> *photon_phi = 0;
  std::vector<bool> *photon_sig = 0;
  std::vector<float> *tk_pt = 0;
  std::vector<float> *tk_eta = 0;
  std::vector<float> *tk_phi = 0;
  std::vector<float> *jet_pt = 0;
  std::vector<float> *jet_eta = 0;
  std::vector<float> *jet_phi = 0;
  std::vector<float> *jet_deepcsv = 0;
  std::vector<bool> *jet_isgood = 0;
  float met = 0;
  float met_phi = 0;
  std::vector<float> *mc_pt = 0;
  std::vector<float> *mc_eta = 0;
  std::vector<float> *mc_phi = 0;
  std::vector<int> *mc_id = 0;
  std::vector<int> *mc_mom = 0;
  //open file and get event
  TFile* root_file = TFile::Open((input_file_path+"/"+input_file_name+".root").c_str());
  TTree* event_tree = static_cast<TTree*>(root_file->Get("tree"));
  if (std::stoi(event_number) >= event_tree->GetEntries() || std::stoi(event_number) < 0) {
    root_file->Close();
    std::cout << "Error: entry number out of bounds" << std::endl;
    return 1;
  }
  event_tree->SetBranchAddress("el_pt",&el_pt);
  event_tree->SetBranchAddress("el_eta",&el_eta);
  event_tree->SetBranchAddress("el_phi",&el_phi);
  event_tree->SetBranchAddress("el_sig",&el_sig);
  event_tree->SetBranchAddress("mu_pt",&mu_pt);
  event_tree->SetBranchAddress("mu_eta",&mu_eta);
  event_tree->SetBranchAddress("mu_phi",&mu_phi);
  event_tree->SetBranchAddress("mu_sig",&mu_sig);
  event_tree->SetBranchAddress("photon_pt",&photon_pt);
  event_tree->SetBranchAddress("photon_eta",&photon_eta);
  event_tree->SetBranchAddress("photon_phi",&photon_phi);
  event_tree->SetBranchAddress("photon_sig",&photon_sig);
  event_tree->SetBranchAddress("tk_pt",&tk_pt);
  event_tree->SetBranchAddress("tk_eta",&tk_eta);
  event_tree->SetBranchAddress("tk_phi",&tk_phi);
  event_tree->SetBranchAddress("jet_pt",&jet_pt);
  event_tree->SetBranchAddress("jet_eta",&jet_eta);
  event_tree->SetBranchAddress("jet_phi",&jet_phi);
  event_tree->SetBranchAddress("jet_isgood",&jet_isgood);
  event_tree->SetBranchAddress("jet_deepcsv",&jet_deepcsv);
  event_tree->SetBranchAddress("mc_pt",&mc_pt);
  event_tree->SetBranchAddress("mc_eta",&mc_eta);
  event_tree->SetBranchAddress("mc_phi",&mc_phi);
  event_tree->SetBranchAddress("mc_id",&mc_id);
  event_tree->SetBranchAddress("mc_mom",&mc_mom);
  event_tree->SetBranchAddress("met",&met);
  event_tree->SetBranchAddress("met_phi",&met_phi);
  event_tree->GetEvent(std::stoi(event_number));
  //write output
  bool first_jet = true;
  bool first_electron = true;
  bool first_muon = true;
  bool first_photon = true;
  bool first_truth_particle = true;
  std::ofstream output_file;
  output_file.open(("data/"+input_file_name+"_"+event_number+".js").c_str());
  output_file << "var jet_list = [";
  for (unsigned int jet_idx = 0; jet_idx < jet_eta->size(); jet_idx++) {
    if (jet_isgood->at(jet_idx)) {
      if (first_jet)  
        first_jet = false;
      else 
        output_file << ",";
      output_file << "{pt:" << std::to_string(jet_pt->at(jet_idx));
      output_file << ",eta:" << std::to_string(jet_eta->at(jet_idx));
      output_file << ",phi:" << std::to_string(jet_phi->at(jet_idx));
      if (jet_deepcsv->at(jet_idx) > 0.6321)
        output_file << ",is_b:true}";
      else 
        output_file << ",is_b:false}";
    }
  }
  output_file << "];\nvar electron_list=[";
  for (unsigned int el_idx = 0; el_idx < el_eta->size(); el_idx++) {
    if (el_sig->at(el_idx)) {
      if (first_electron)  
        first_electron = false;
      else 
        output_file << ",";
      output_file << "{pt:" << std::to_string(el_pt->at(el_idx));
      output_file << ",eta:" << std::to_string(el_eta->at(el_idx));
      output_file << ",phi:" << std::to_string(el_phi->at(el_idx));
      output_file << "}";
    }
  }
  output_file << "];\nvar muon_list=[";
  for (unsigned int mu_idx = 0; mu_idx < mu_eta->size(); mu_idx++) {
    if (mu_sig->at(mu_idx)) {
      if (first_muon)  
        first_muon = false;
      else 
        output_file << ",";
      output_file << "{pt:" << std::to_string(mu_pt->at(mu_idx));
      output_file << ",eta:" << std::to_string(mu_eta->at(mu_idx));
      output_file << ",phi:" << std::to_string(mu_phi->at(mu_idx));
      output_file << "}";
    }
  }
  output_file << "];\nvar photon_list=[";
  if (tk_mode) {
    for (unsigned int tk_idx = 0; tk_idx < tk_eta->size(); tk_idx++) {
      if (first_photon)  
        first_photon = false;
      else 
        output_file << ",";
      output_file << "{pt:" << std::to_string(tk_pt->at(tk_idx));
      output_file << ",eta:" << std::to_string(tk_eta->at(tk_idx));
      output_file << ",phi:" << std::to_string(tk_phi->at(tk_idx));
      output_file << "}";
    }
  } 
  else {
    for (unsigned int photon_idx = 0; photon_idx < photon_eta->size(); photon_idx++) {
      if (photon_sig->at(photon_idx)) {
        if (first_photon)  
          first_photon = false;
        else 
          output_file << ",";
        output_file << "{pt:" << std::to_string(photon_pt->at(photon_idx));
        output_file << ",eta:" << std::to_string(photon_eta->at(photon_idx));
        output_file << ",phi:" << std::to_string(photon_phi->at(photon_idx));
        output_file << "}";
      }
    }
  }
  output_file << "];\nvar met = {pt:" << met << ",phi:" << met_phi << "};\nvar truth_list = [";
  for (unsigned int mc_idx = 0; mc_idx < mc_eta->size(); mc_idx++) {
    if (first_truth_particle)  
      first_truth_particle = false;
    else 
      output_file << ",";
    output_file << "{pt:" << std::to_string(mc_pt->at(mc_idx));
    output_file << ",eta:" << std::to_string(mc_eta->at(mc_idx));
    output_file << ",phi:" << std::to_string(mc_phi->at(mc_idx));
    if (particle_name.count(mc_id->at(mc_idx))>0) {
      output_file << ",id:\"" << particle_name[mc_id->at(mc_idx)] << "\"";
    }
    else {
      output_file << ",id:\"unknown\"";
    }
    if (particle_name.count(mc_mom->at(mc_idx))>0) {
      output_file << ",mom:\"" << particle_name[mc_id->at(mc_idx)] << "\"";
    }
    else {
      output_file << ",mom:\"unknown\"";
    }
    output_file << "}";
  }
  output_file << "];";
  output_file.close();
  root_file->Close();
  return 0;
}
