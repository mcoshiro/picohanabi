#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>

int main(int argc, char** argv) {
	if (argc != 4) {
		std::cout << "Error: incorrect number of arguments." << std::endl;
		return 1;
	}
	std::string input_file_path = argv[1];
	std::string input_file_name = argv[2];
	std::string event_number = argv[3];
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
	std::vector<float> el_eta;
	std::vector<float> el_phi;
	std::vector<bool> el_sig;
	std::vector<float> mu_eta;
	std::vector<float> mu_phi;
	std::vector<bool> mu_sig;
	std::vector<float> photon_eta;
	std::vector<float> photon_phi;
	std::vector<bool> photon_sig;
	std::vector<float> jet_eta;
	std::vector<float> jet_phi;
	std::vector<float> jet_deepcsv;
	std::vector<bool> jet_isgood;
	float met_phi = 2.7;
	std::vector<float> mc_eta;
	std::vector<float> mc_phi;
	std::vector<float> mc_id;
	std::vector<float> mc_mom;
	el_eta.push_back(1.2);
	el_eta.push_back(1.3);
	el_phi.push_back(1.2);
	el_phi.push_back(1.3);
	el_sig.push_back(true);
	el_sig.push_back(false);
	mu_eta.push_back(-2.3);
	mu_eta.push_back(-2.0);
	mu_phi.push_back(3.1);
	mu_phi.push_back(2.8);
	mu_sig.push_back(true);
	mu_sig.push_back(true);
	photon_eta.push_back(0.3);
	photon_eta.push_back(1.2);
	photon_phi.push_back(-0.5);
	photon_phi.push_back(1.3);
	photon_sig.push_back(true);
	photon_sig.push_back(false);
	jet_eta.push_back(-1.0);
	jet_eta.push_back(1.0);
	jet_phi.push_back(0.23);
	jet_phi.push_back(-0.23);
	jet_isgood.push_back(true);
	jet_isgood.push_back(true);
	jet_deepcsv.push_back(0.02);
	jet_deepcsv.push_back(0.97);
	mc_eta.push_back(1.2);
	mc_phi.push_back(1.2);
	mc_id.push_back(11);
	mc_mom.push_back(-24);
	//write output
	bool first_jet = true;
	bool first_electron = true;
	bool first_muon = true;
	bool first_photon = true;
	bool first_truth_particle = true;
	std::ofstream output_file;
	output_file.open(("data/"+input_file_name+"_"+event_number+".js").c_str());
	output_file << "var jet_list = [";
	for (unsigned int jet_idx = 0; jet_idx < jet_eta.size(); jet_idx++) {
		if (jet_isgood[jet_idx]) {
			if (first_jet)	
				first_jet = false;
			else 
				output_file << ",";
			output_file << "{eta:" << std::to_string(jet_eta[jet_idx]);
			output_file << ",phi:" << std::to_string(jet_phi[jet_idx]);
			if (jet_deepcsv[jet_idx] > 0.6321)
				output_file << ",is_b:true}";
			else 
				output_file << ",is_b:false}";
		}
	}
	output_file << "];\nvar electron_list=[";
	for (unsigned int el_idx = 0; el_idx < el_eta.size(); el_idx++) {
		if (el_sig[el_idx]) {
			if (first_electron)	
				first_electron = false;
			else 
				output_file << ",";
			output_file << "{eta:" << std::to_string(el_eta[el_idx]);
			output_file << ",phi:" << std::to_string(el_phi[el_idx]);
			output_file << "}";
		}
	}
	output_file << "];\nvar muon_list=[";
	for (unsigned int mu_idx = 0; mu_idx < mu_eta.size(); mu_idx++) {
		if (mu_sig[mu_idx]) {
			if (first_muon)	
				first_muon = false;
			else 
				output_file << ",";
			output_file << "{eta:" << std::to_string(mu_eta[mu_idx]);
			output_file << ",phi:" << std::to_string(mu_phi[mu_idx]);
			output_file << "}";
		}
	}
	output_file << "];\nvar photon_list=[";
	for (unsigned int photon_idx = 0; photon_idx < photon_eta.size(); photon_idx++) {
		if (photon_sig[photon_idx]) {
			if (first_photon)	
				first_photon = false;
			else 
				output_file << ",";
			output_file << "{eta:" << std::to_string(photon_eta[photon_idx]);
			output_file << ",phi:" << std::to_string(photon_phi[photon_idx]);
			output_file << "}";
		}
	}
	output_file << "];\nvar met = {phi:" << met_phi << "};\nvar truth_list = [";
	for (unsigned int mc_idx = 0; mc_idx < mc_eta.size(); mc_idx++) {
		if (first_truth_particle)	
			first_truth_particle = false;
		else 
			output_file << ",";
		output_file << "{eta:" << std::to_string(mc_eta[mc_idx]);
		output_file << ",phi:" << std::to_string(mc_phi[mc_idx]);
		if (particle_name.count(mc_id[mc_idx])>0) {
			output_file << ",id:\"" << particle_name[mc_id[mc_idx]] << "\"";
		}
		else {
			output_file << ",id:\"unknown\"";
		}
		if (particle_name.count(mc_mom[mc_idx])>0) {
			output_file << ",mom:\"" << particle_name[mc_id[mc_idx]] << "\"";
		}
		else {
			output_file << ",mom:\"unknown\"";
		}
		output_file << "}";
	}
	output_file << "];";
	output_file.close();
	return 0;
}
