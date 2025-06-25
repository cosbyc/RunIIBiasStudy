double computebias(TString filename, int nbins, double xmin, double xmax, TString mass) {

  //TH1F h("h", "", 200, -20.0, 20.0);
  TH1F h("h", "", 200, -5.0, 5.0);
  TF1  f("f", "gaus", xmin, xmax);
  
  TFile file(filename);
  
  if (file.IsZombie()) return 0.0;    
  
  TTree* tree = (TTree*)file.Get("tree_fit_sb");
  
  if (tree == NULL) return 0.0;
  
  TTreeReader reader(tree);
  
  TTreeReaderValue<double> r   (reader, "r"      );
  TTreeReaderValue<double> rErr(reader, "rErr"   );
  TTreeReaderValue<double> rLoErr(reader, "rLoErr"   );
  TTreeReaderValue<double> rHiErr(reader, "rHiErr"   );
  
  std::vector<Double_t>  observations;
  Double_t median = -999;
  while(reader.Next()) {
      h.Fill((*r-0.) / (0.5*(*rHiErr+*rLoErr)));
      observations.push_back((*r-0.) / (0.5*(*rHiErr+*rLoErr)));
      //h.Fill((*r-0.025) / (0.5*(*rHiErr+*rLoErr)));
      //observations.push_back((*r-0.025) / (0.5*(*rHiErr+*rLoErr)));
  }
  
  Int_t obs_size = observations.size();
  std::sort(observations.begin(), observations.end());
  if(obs_size % 2 == 0) median =  ( observations[obs_size/2. - 1] + observations[obs_size/2.] ) / 2.;
  else median = observations[obs_size/2. - 0.5];
  
  std::cout<<"observations.size = "<<obs_size<<" median = "<<median<<"\n";
  
  f.SetParameters(h.Integral()*0.1, 0.0, 1.0);
  h.Fit("f");

  ////////////////////////////////


  TCanvas *c1 = new TCanvas("c1", "c1", 600, 600);
  c1->SetTopMargin(0.07);
  c1->SetRightMargin(0.07);
  
  h.GetXaxis()->SetLabelSize(18);
  h.GetXaxis()->SetLabelFont(43);
  h.GetXaxis()->SetTitleFont(63);
  h.GetXaxis()->SetTitleSize(22);
  h.GetXaxis()->SetTitleOffset(0.9);
  
  h.GetYaxis()->SetLabelSize(20);
  h.GetYaxis()->SetLabelFont(43);
  h.GetYaxis()->SetTitleFont(63);
  h.GetYaxis()->SetTitleSize(22);
  h.GetYaxis()->SetTitleOffset(1.1);
 
  h.SetXTitle("Pull");
  h.SetYTitle("N(toys)");
  
  h.SetMarkerSize(1.5);
  h.SetMarkerColor(kBlack);
  
  h.Draw("");
  
  c1->SaveAs("PullDistribution_"+mass+".png");

  ////////////////////////////////


  //h.SaveAs("biasfit_"+filename+".root");
  
  file.Close();

  std::cout<<"filename: "<<filename<<std::endl;
  std::cout<<"gaussian mean "<<f.GetParameter(1)<<"\n";
  std::cout<<"gaussian sigma "<<f.GetParameter(2)<<"\n";

  return f.GetParameter(1);
  //return median;
}


void makeLowMassBiasPlot(){

    //const int nmasses = 14;  
    //double mass_lm[nmasses]  = {14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40};
    //TString ts_mass_lm[nmasses] = {"14", "16", "18", "20", "22", "24", "26", "28", "30", "32", "34", "36", "38", "40"};
    const int nmasses = 80;  
    double mass_lm[nmasses]  = {0.235, 0.246, 0.259, 0.272, 0.286, 0.301, 0.316, 0.332, 0.349, 0.367, 0.386, 0.405, 0.426, 0.448, 0.471, 0.495, 0.634, 0.667, 0.701, 0.899, 1.152, 1.211, 1.273, 1.338, 1.406, 1.478, 1.553, 1.632, 1.716, 1.803, 1.895, 1.992, 2.094, 2.2, 2.313, 2.43, 2.554, 4.201, 4.415, 4.641, 4.877, 5.126, 5.388, 5.663, 5.951, 6.255, 6.574, 6.909, 7.262, 7.632, 8.022, 8.431};
    TString ts_mass_lm[nmasses] = {"0.235", "0.246", "0.259", "0.272", "0.286", "0.301", "0.316", "0.332", "0.349", "0.367", "0.386", "0.405", "0.426", "0.448", "0.471", "0.495", "0.634", "0.667", "0.701", "0.899", "1.152", "1.211", "1.273", "1.338", "1.406", "1.478", "1.553", "1.632", "1.716", "1.803", "1.895", "1.992", "2.094", "2.2", "2.313", "2.43", "2.554", "4.201", "4.415", "4.641", "4.877", "5.126", "5.388", "5.663", "5.951", "6.255", "6.574", "6.909", "7.262", "7.632", "8.022", "8.431"};
    double bias_toyPdf0_fitPdf0_lm[nmasses];

    //TString ts_cat[2] = {"catB","catO"};
    //TString ts_fitPdf[2] = {"0","1"};

    //for(int i = 0; i < 2; i++){//categories
    for(int j = 0; j < nmasses; j++){//mass
    
      //if (j>0) break;

      TString f_name_lm = "lowMassTrees2017/fitDiagnostics"+ts_mass_lm[j]+".root";
      bias_toyPdf0_fitPdf0_lm[j] = 100 * computebias(f_name_lm, 40, -5, 5, ts_mass_lm[j]);
      std::cout<<"BIAS "<<bias_toyPdf0_fitPdf0_lm[j]<<"\n";
    }

  TLegend *legend_lm = new TLegend(0.55,0.6,0.85,0.9,NULL,"brNDC");
  legend_lm->SetBorderSize(0);

  //low mass
  TGraphErrors * tg_toyPdf0_fitPdf0_lm = new TGraphErrors(nmasses, mass_lm, bias_toyPdf0_fitPdf0_lm);
  tg_toyPdf0_fitPdf0_lm->SetName("tg_toyPdf0_fitPdf0_lm");
  tg_toyPdf0_fitPdf0_lm->SetLineColor(kRed);
  tg_toyPdf0_fitPdf0_lm->SetMarkerColor(kRed);
  tg_toyPdf0_fitPdf0_lm->SetMarkerStyle(20);
  tg_toyPdf0_fitPdf0_lm->SetLineStyle(2);
  legend_lm->AddEntry(tg_toyPdf0_fitPdf0_lm,"Bern 3 fit to Bern 3","lp");



  /////////////////////////////////////////////////////
  gStyle->SetOptTitle(0); 
  gStyle->SetOptStat(0);
  /////////////////////////////////////////////////////
    
  TCanvas *c1 = new TCanvas("c1", "c1", 600, 600);
  c1->SetTopMargin(0.07);
  c1->SetRightMargin(0.07);
  
  //TH2F *hframe = new TH2F("","",500, 10, 210, 500, -100, 100);
  //TH2F *hframe = new TH2F("","",500, 10, 50, 500, -20, 20);
  //New range for low mass
  TH2F *hframe = new TH2F("","",500, 1, 10, 500, -31, 31);
  hframe->GetXaxis()->SetLabelSize(18);
  hframe->GetXaxis()->SetLabelFont(43);
  hframe->GetXaxis()->SetTitleFont(63);
  hframe->GetXaxis()->SetTitleSize(22);
  hframe->GetXaxis()->SetTitleOffset(0.9);
  
  hframe->GetYaxis()->SetLabelSize(20);
  hframe->GetYaxis()->SetLabelFont(43);
  hframe->GetYaxis()->SetTitleFont(63);
  hframe->GetYaxis()->SetTitleSize(22);
  hframe->GetYaxis()->SetTitleOffset(1.1);
  
  hframe->SetXTitle("dimuon mass (GeV/c^{2})");
  hframe->SetYTitle("bias (in %)");
  
  hframe->SetMarkerSize(1.5);
  hframe->SetMarkerColor(kBlack);
  
  hframe->Draw("");
  legend_lm->Draw("same");
  //legend_hm->Draw("same");
  
  tg_toyPdf0_fitPdf0_lm->Draw("L*");
  
  c1->SaveAs("Bias_vs_dimuonmass2017.png");
}

