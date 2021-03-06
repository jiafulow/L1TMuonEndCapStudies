{

float table_common_dphi[9][4][12];  // [ipair][ifr][ieta]
for (int i=0; i<9; ++i)
  for (int j=0; j<4; ++j)
    for (int k=0; k<12; ++k)
      table_common_dphi[i][j][k] = 1.0;

float table_common_dphi_2[9+16][4][12];  // [ipair][ifr][ieta]
for (int i=0; i<(9+16); ++i)
  for (int j=0; j<4; ++j)
    for (int k=0; k<12; ++k)
      table_common_dphi_2[i][j][k] = 1.0;

float table_mean_uncorr[9+16][4][8];  // [ipair][ifr][ipt]
float table_mean_corr[9+16][4][8];
float table_sigma_uncorr[9+16][4][8];
float table_sigma_corr[9+16][4][8];
for (int i=0; i<(9+16); ++i)
  for (int j=0; j<4; ++j)
    for (int k=0; k<8; ++k) {
      table_mean_uncorr[i][j][k] = 0.0;
      table_mean_corr[i][j][k] = 0.0;
      table_sigma_uncorr[i][j][k] = 0.0;
      table_sigma_corr[i][j][k] = 0.0;
    }


TString hname;
TString prefix;
TH1* h1;
TH2* h2;
TProfile* pf;
TH1* num, * denom;
TH2* sitrep;
TF1* f1;
TGraphAsymmErrors* gr;
TGraphAsymmErrors* gra;
TGraphAsymmErrors* grb;
double ymin, ymax;

//TFile *_file0 = TFile::Open("histos.root");
//TFile *_file0 = TFile::Open("histos_add.root");
//TFile *_file0 = TFile::Open("histos_add_20170123.root");
//TFile *_file0 = TFile::Open("histos_add_20170201.root");
//TFile *_file0 = TFile::Open("histos_add_20170203.root");
//TFile *_file0 = TFile::Open("histos_add_20170206.root");
//TFile *_file0 = TFile::Open("histos_add_20170210.root");
TFile *_file0 = TFile::Open("histos_add_20170211.root");
TString outdir = "figures_deflection/";

gStyle->SetPaintTextFormat(".3f");
gStyle->SetPalette(kBird);

int palette[5];
palette[0] = TColor::GetColor("#00A1E1");
palette[1] = TColor::GetColor("#E0006B");
palette[2] = TColor::GetColor("#F78700");
palette[3] = TColor::GetColor("#0E9F00");
palette[4] = TColor::GetColor("#A800E0");

int palette2[2];
palette2[0] = TColor::GetColor("#808080");
palette2[1] = TColor::GetColor("#A800E0");

int palette8[8];
palette8[0] = TColor::GetColor("#4DEEFF");  // lighten 30%
palette8[1] = TColor::GetColor("#FF4DB8");  // lighten 30%
palette8[2] = TColor::GetColor("#FFD44D");  // lighten 30%
palette8[3] = TColor::GetColor("#5BEC4D");  // lighten 30%
palette8[4] = TColor::GetColor("#005595");  // darken 30%
palette8[5] = TColor::GetColor("#94001F");  // darken 30%
palette8[6] = TColor::GetColor("#AB3B00");  // darken 30%
palette8[7] = TColor::GetColor("#005300");  // darken 30%

TLine * tline = new TLine();
tline->SetLineColor(kGray+3);

TLatex * tlatex = new TLatex();
tlatex->SetNDC();
tlatex->SetTextFont(42);
tlatex->SetTextSize(0.035);

TLegend* tlegend = new TLegend(0.06,0.68,0.52,0.94);
tlegend->SetFillStyle(0);
tlegend->SetLineColor(0);
tlegend->SetShadowColor(0);
tlegend->SetBorderSize(0);

TLegend* tlegend2 = new TLegend(0.46,0.68,0.92,0.94);
tlegend2->SetFillStyle(0);
tlegend2->SetLineColor(0);
tlegend2->SetShadowColor(0);
tlegend2->SetBorderSize(0);

static const int print_pt[8] = {2, 3, 5, 10, 20, 50, 100, 200};
//static const char* const print_angle[9] = {"#Delta#phi(1/1,2)", "#Delta#phi(1/1,3)", "#Delta#phi(1/1,4)", "#Delta#phi(1/2,2)", "#Delta#phi(1/2,3)", "#Delta#phi(1/2,4)", "#Delta#phi(2,3)", "#Delta#phi(2,4)", "#Delta#phi(3,4)"};
static const char* const print_angle[9+16] = {"#Delta#phi(1/1,2)", "#Delta#phi(1/1,3)", "#Delta#phi(1/1,4)", "#Delta#phi(1/2,2)", "#Delta#phi(1/2,3)", "#Delta#phi(1/2,4)", "#Delta#phi(2,3)", "#Delta#phi(2,4)", "#Delta#phi(3,4)",
    "#Delta#phi(RE1/2,ME2)", "#Delta#phi(RE1/2,ME3)", "#Delta#phi(RE1/2,ME4)", "null", "#Delta#phi(ME1/1,RE2)", "#Delta#phi(ME1/2,RE2)", "#Delta#phi(RE2,ME3)", "#Delta#phi(RE2,ME4)", "#Delta#phi(ME1/1,RE3)", "#Delta#phi(ME1/2,RE3)", "#Delta#phi(ME2,RE3)", "#Delta#phi(RE3,ME4)", "#Delta#phi(ME1/1,RE4)", "#Delta#phi(ME1/2,RE4)", "#Delta#phi(ME2,RE4)", "#Delta#phi(ME3,RE4)"};
static const char* const print_fr[5] = {"R/R", "R/F", "F/R", "F/F", "all"};
static const char* const print_corr[2] = {"uncorr", "corr"};


// _____________________________________________________________________________
// Deflection angles vs eta
if (0) {
  for (int ipair=0; ipair<9; ++ipair) {
    for (int ipt=0; ipt<8; ++ipt) {
      hname = Form("deflection_stp%i_frp4_pt%i", ipair, ipt);
      h2 = (TH2*) _file0->Get(hname);
      h2 = (TH2*) h2->Clone(hname + "_tmp1");
      if (ipair == 0 || ipair == 1 || ipair == 2) {
        // pass
      } else if (ipair == 3 || ipair == 4 || ipair == 5) {
        h2->RebinY(2);
      } else if (ipair == 6 || ipair == 7 || ipair == 8) {
        h2->RebinY(4);
      }
      h2->SetStats(0);
      h2->Draw("colz2");

      gPad->Print(outdir + hname + ".png");
    }

    for (int ieta=0; ieta<12; ++ieta) {
      for (int ipt=0; ipt<8-3; ++ipt) {
        hname = Form("deflection_stp%i_frp4_pt%i", ipair, ipt);
        h2 = (TH2*) _file0->Get(hname);
        h2 = (TH2*) h2->Clone(hname + "_tmp2");
        if (ipair == 0 || ipair == 1 || ipair == 2) {
          // pass
        } else if (ipair == 3 || ipair == 4 || ipair == 5) {
          h2->RebinY(2);
        } else if (ipair == 6 || ipair == 7 || ipair == 8) {
          h2->RebinY(4);
        }
        h2->RebinX(4);
        h1 = h2->ProjectionY(hname + "_tmp2_py", ieta+1, ieta+1, "");
        h1->SetMarkerStyle(20); h1->SetMarkerSize(0.1);
        h1->SetMarkerColor(palette8[ipt]); h1->SetLineColor(palette8[ipt]);

        if (ipt == 0) {
          h1->SetMinimum(0.5); h1->SetMaximum(5e5);
          TString xtitle = h1->GetXaxis()->GetTitle(); xtitle = xtitle(0,27); xtitle = xtitle + Form("{%.1f#leq|#eta|<%.1f}",1.2+0.1*ieta, 1.2+0.1*(ieta+1));
          h1->GetXaxis()->SetTitle(xtitle);
          h1->SetStats(0); h1->Draw("e");
          h1->GetXaxis()->SetRangeUser(-600,600);
          tlegend->Clear(); tlegend2->Clear();
        } else {
          h1->Draw("e same");
        }

        // Fit
        int status = h1->Fit("gaus", "Q0", "", h1->GetMean()-3.0*h1->GetRMS(), h1->GetMean()+3.0*h1->GetRMS());
        if (h1->Integral() > 100. && status == 0) {
          f1 = h1->GetFunction("gaus");
          f1->SetLineWidth(2); f1->SetLineColor(palette8[ipt]);
          f1->Draw("same");
          tlegend->AddEntry(h1, Form("#color[%i]{#mu(p_{T} = %i GeV) = %.1f}", f1->GetLineColor(), print_pt[ipt], f1->GetParameter(1)), "");
          tlegend2->AddEntry(h1, Form("#color[%i]{#sigma(p_{T} = %i GeV) = %.1f}", f1->GetLineColor(), print_pt[ipt], f1->GetParameter(2)), "");
        } else {
          f1 = new TF1();
          f1->SetLineWidth(2); f1->SetLineColor(palette8[ipt]);
          tlegend->AddEntry(h1, Form("#color[%i]{#mu(p_{T} = %i GeV) = NaN}", f1->GetLineColor(), print_pt[ipt]), "");
          tlegend2->AddEntry(h1, Form("#color[%i]{#sigma(p_{T} = %i GeV) = NaN}", f1->GetLineColor(), print_pt[ipt]), "");
        }
      }

      tlegend->Draw(); tlegend2->Draw();
      gPad->SetLogy(true);

      hname = Form("deflection_stp%i_frp4_pt%i_eta%i", ipair, 99, ieta);
      gPad->Print(outdir + hname + ".png");
      gPad->SetLogy(false);
    }
  }
}


// _____________________________________________________________________________
// Fraction of F/F, F/R, R/F, R/R [2]
if (0) {
  TH1* fraction_histos[5];

  for (int ifr=0; ifr<5; ++ifr) {
    hname = Form("fraction_frp%i", ifr);
    h1 = new TH1F(hname, "; ", 9+16, 0, 9+16);
    fraction_histos[ifr] = h1;
  }

  for (int ipair=0; ipair<(9+16); ++ipair) {
    for (int ifr=0; ifr<5; ++ifr) {
      for (int ieta=0; ieta<12; ++ieta) {
        hname = Form("deflection_stp%i_frp%i_eta%i", ipair, ifr, ieta);
        h2 = (TH2*) _file0->Get(hname);
        h1 = fraction_histos[ifr];
        h1->SetBinContent(ipair+1, h1->GetBinContent(ipair+1) + h2->GetEntries());

        // Debug
        std::cout << "DEBUG: " << ipair << " " << ifr << " " << ieta << " " << h2->GetEntries() << " " << h1->GetBinContent(ipair+1) << std::endl;
      }
    }
  }

  // To stack the fraction histos
  for (int ifr=0; ifr<4; ++ifr) {
    for (int jfr=ifr+1; jfr<4; ++jfr) {
      fraction_histos[ifr]->Add(fraction_histos[jfr]);
    }
    fraction_histos[ifr]->Divide(fraction_histos[4]);
  }

  for (int ifr=0; ifr<5; ++ifr) {
    h1 = fraction_histos[ifr];
    h1->SetLineColor(palette[ifr]);
    h1->SetFillColor(palette[ifr]);
    for (int ipair=0; ipair<(9+16); ++ipair) {
      h1->GetXaxis()->SetBinLabel(ipair+1, print_angle[ipair]);
    }
    h1->GetXaxis()->LabelsOption("u");
    if (ifr == 0) {
      h1->SetMinimum(0); h1->SetMaximum(1.3);
      h1->SetStats(0); h1->Draw("hist");
    } else if (ifr < 4) {
      h1->Draw("same hist");
    } else {
      // pass
    }
  }

  {
    int ifr=0;
    h1 = fraction_histos[ifr];

    h1->GetXaxis()->SetRangeUser(0, 9);
    gPad->RedrawAxis();
    gPad->Print(outdir + "fraction_frp_1.png");
    h1->GetXaxis()->SetRangeUser(9, 9+16);
    h1->GetXaxis()->SetLabelSize(0.035);  // default: 0.05
    gPad->RedrawAxis();
    gPad->Print(outdir + "fraction_frp_2.png");
  }

  {
    int ifr=4;
    h1 = fraction_histos[ifr];
    h1->SetStats(0); h1->Draw("hist");

    h1->GetXaxis()->SetRangeUser(0, 9);
    gPad->RedrawAxis();
    gPad->Print(outdir + "total_frp_1.png");
    h1->GetXaxis()->SetRangeUser(9, 9+16);
    h1->GetXaxis()->SetLabelSize(0.035);  // default: 0.05
    gPad->RedrawAxis();
    gPad->Print(outdir + "total_frp_2.png");
  }
}


// _____________________________________________________________________________
// Deflection angles vs pT [2]
if (0) {
  for (int ipair=0; ipair<(9+16); ++ipair) {
    for (int ifr=0; ifr<4; ++ifr) {
      for (int ieta=0; ieta<12; ++ieta) {
        hname = Form("deflection_stp%i_frp%i_eta%i", ipair, ifr, ieta);
        h2 = (TH2*) _file0->Get(hname);
        h2->GetYaxis()->SetTitleOffset(1.4);
        h2->SetMarkerStyle(6); h2->SetMarkerColor(kGray+3);
        h2->SetStats(0);
        h2->Draw();

        pf = h2->ProfileX("_pfx", 1, -1, "s");
        pf->SetMarkerStyle(1); pf->SetMarkerColor(palette[ifr]);
        pf->SetLineWidth(2); pf->SetLineColor(palette[ifr]);
        pf->Draw("same");

        gPad->Print(outdir + hname + ".png");
      }
    }
  }
}

// _____________________________________________________________________________
// Ratios of deflection angles vs pT [2]
if (0) {
  for (int ipair=0; ipair<(9+16); ++ipair) {
    for (int ifr=0; ifr<4; ++ifr) {
      for (int ieta=0; ieta<12; ++ieta) {
        if (ipair == 9+3)  continue;  // null

        ymin = -4.; ymax = 8.;
        int denom_ipair = -1, denom_ifr = -1;
        int denom_csc_ipair = -1, denom_csc_ifr = -1;

        if (ipair < 9) {
          if (ipair == 4 || ipair == 5 || ipair == 6 || ipair == 7) {
            denom_ipair = ipair; denom_ifr = 2;  // F/R
          } else {
            denom_ipair = ipair; denom_ifr = 3;  // F/F
          }
          denom_csc_ipair = denom_ipair; denom_csc_ifr = denom_ifr;

        } else if (ipair < (9+16)) {
          if (ipair == 9+0 || ipair == 9+3 || ipair == 9+4 || ipair == 9+5 || ipair == 9+8 || ipair == 9+9 || ipair == 9+10 || ipair == 9+12 || ipair == 9+13 || ipair == 9+14) {
            denom_ipair = ipair; denom_ifr = 3;  // F/F
          } else if (ipair == 9+1 || ipair == 9+2 || ipair == 9+6 || ipair == 9+7 || ipair == 9+11 || ipair == 9+15) {
            denom_ipair = ipair; denom_ifr = 2;  // F/R
          }

          if (ipair == 9+0) {
            denom_csc_ipair = 3; denom_csc_ifr = 3;  // (1/2,2) F/F
          } else if (ipair == 9+1) {
            denom_csc_ipair = 4; denom_csc_ifr = 2;  // (1/2,3) F/R
          } else if (ipair == 9+2) {
            denom_csc_ipair = 5; denom_csc_ifr = 2;  // (1/2,4) F/R
          } else if (ipair == 9+3) {
            // pass
          } else if (ipair == 9+4) {
            denom_csc_ipair = 0; denom_csc_ifr = 3;  // (1/1,2) F/F
          } else if (ipair == 9+5) {
            denom_csc_ipair = 3; denom_csc_ifr = 3;  // (1/2,2) F/F
          } else if (ipair == 9+6) {
            denom_csc_ipair = 6; denom_csc_ifr = 2;  // (2,3) F/R
          } else if (ipair == 9+7) {
            denom_csc_ipair = 7; denom_csc_ifr = 2;  // (2,4) F/R
          } else if (ipair == 9+8) {
            denom_csc_ipair = 1; denom_csc_ifr = 3;  // (1/1,3) F/F
          } else if (ipair == 9+9) {
            denom_csc_ipair = 4; denom_csc_ifr = 2;  // (1/2,3) F/R
          } else if (ipair == 9+10) {
            denom_csc_ipair = 6; denom_csc_ifr = 2;  // (2,3) F/R
          } else if (ipair == 9+11) {
            denom_csc_ipair = 8; denom_csc_ifr = 3;  // (3,4) F/F
          } else if (ipair == 9+12) {
            denom_csc_ipair = 2; denom_csc_ifr = 3;  // (1/1,4) F/F
          } else if (ipair == 9+13) {
            denom_csc_ipair = 5; denom_csc_ifr = 2;  // (1/2,4) F/R
          } else if (ipair == 9+14) {
            denom_csc_ipair = 7; denom_csc_ifr = 2;  // (2,4) F/R
          } else if (ipair == 9+15) {
            denom_csc_ipair = 8; denom_csc_ifr = 3;  // (3,4) F/F
          }
        }

        //bool good_stats = false;
        bool good_stats = true;
        //if (ipair == 0 || ipair == 1 || ipair == 2) {
        //  // pass
        //} else if (ipair < 9) {
        //  if (denom_ifr == 2 && (ifr == 0 || ifr == 3))
        //    good_stats = false;
        //  else if (denom_ifr == 3 && (ifr == 1 || ifr == 2))
        //    good_stats = false;
        //} else if (ipair < (9+16)) {
        //  if (denom_ifr == 2 && (ifr == 0 || ifr == 3))
        //    good_stats = false;
        //  else if (denom_ifr == 3 && (ifr == 1 || ifr == 2))
        //    good_stats = false;
        //}

        hname = Form("deflection_stp%i_frp%i_eta%i", denom_csc_ipair, denom_csc_ifr, ieta);
        h2 = (TH2*) _file0->Get(hname);
        h2 = (TH2*) h2->Clone(hname + "_tmp3");
        if (1) {  // do not distinguish F/F from F/R, R/R from R/F
          if (!(ipair == 0 || ipair == 1 || ipair == 2)) {
            if ((denom_csc_ifr%2) == 0) {
              h2->Add((TH2*) _file0->Get(hname.ReplaceAll(Form("_frp%i", denom_csc_ifr), Form("_frp%i", denom_csc_ifr+1))));
            } else {
              h2->Add((TH2*) _file0->Get(hname.ReplaceAll(Form("_frp%i", denom_csc_ifr), Form("_frp%i", denom_csc_ifr-1))));
            }
          }
        }
        if (1) {  // modify range
          for (int binx=0; binx < h2->GetNbinsX()+2; ++binx) {
            if (h2->GetXaxis()->GetBinLowEdge(binx) < 0.01 || h2->GetXaxis()->GetBinLowEdge(binx) >= 0.25) {
              for (int biny=0; biny < h2->GetNbinsY()+2; ++biny) {
                 h2->SetBinContent(binx, biny, 0);
              }
            }
            if (((ipair == 6 || ipair == 7 || ipair == 8) || (ipair == 9+6 || ipair == 9+7 || ipair == 9+10 || ipair == 9+11 || ipair == 9+14 || ipair == 9+15)) && ((ieta == 0 && h2->GetXaxis()->GetBinLowEdge(binx) >= 0.15) || (ieta == 1 && h2->GetXaxis()->GetBinLowEdge(binx) >= 0.18))) {  // these angles at low eta and low pT are problematic
              for (int biny=0; biny < h2->GetNbinsY()+2; ++biny) {
                h2->SetBinContent(binx, biny, 0);
              }
            }
          }
          for (int binx=0; binx+1 < h2->GetNbinsX()+2; ++binx) {
            for (int biny=0; biny < h2->GetNbinsY()+2; ++biny) {
              h2->SetBinContent(binx, biny, h2->GetBinContent(binx+1, biny));
            }
          }
        }

        if (ipair == 0 || ipair == 1 || ipair == 2) {
          if (good_stats)  h2->RebinX(2);  else  h2->RebinX(4);
        } else if (ipair == 3 || ipair == 4 || ipair == 5) {
          if (good_stats)  h2->RebinX(4);  else  h2->RebinX(6);
        } else if (ipair == 6 || ipair == 7 || ipair == 8) {
          if (good_stats)  h2->RebinX(6);  else  h2->RebinX(8);
        } else if (ipair == 9+0 || ipair == 9+1 || ipair == 9+2 || ipair == 9+3) {
          if (good_stats)  h2->RebinX(4);  else  h2->RebinX(6);
        } else if (ipair == 9+4 || ipair == 9+5 || ipair == 9+8 || ipair == 9+9 || ipair == 9+12 || ipair == 9+13) {
          if (good_stats)  h2->RebinX(4);  else  h2->RebinX(6);
        } else if (ipair == 9+6 || ipair == 9+7 || ipair == 9+10 || ipair == 9+11 || ipair == 9+14 || ipair == 9+15) {
          if (good_stats)  h2->RebinX(6);  else  h2->RebinX(8);
        }
        pf = h2->ProfileX(hname + "_tmp3_pfx", 1, -1, "s");
        //pf->SetMarkerStyle(1); pf->SetMarkerColor(palette[ifr]);
        //pf->SetLineWidth(2); pf->SetLineColor(palette[ifr]);
        //pf->Draw("same");
        denom = pf->ProjectionX(hname + "_tmp3_px", "e");  // denom 1D histo

        hname = Form("deflection_stp%i_frp%i_eta%i", ipair, ifr, ieta);
        h2 = (TH2*) _file0->Get(hname);
        h2 = (TH2*) h2->Clone(hname + "_tmp4");
        if (1) {  // do not distinguish F/F from F/R, R/R from R/F
          if (!(ipair == 0 || ipair == 1 || ipair == 2)) {
            if ((ifr%2) == 0) {
              h2->Add((TH2*) _file0->Get(hname.ReplaceAll(Form("_frp%i", ifr), Form("_frp%i", ifr+1))));
            } else {
              h2->Add((TH2*) _file0->Get(hname.ReplaceAll(Form("_frp%i", ifr), Form("_frp%i", ifr-1))));
            }
          }
        }
        if (ipair == 9+4) {  // stp13 hack
          hname = Form("deflection_stp%i_frp%i_eta%i", ipair, ifr, ieta);
          if (ieta == 3) {
            h2->Add((TH2*) _file0->Get(hname.ReplaceAll(Form("_eta%i", ieta), Form("_eta%i", ieta+1))));
            if (1) {  // do not distinguish F/F from F/R, R/R from R/F
              if ((ifr%2) == 0) {
                h2->Add((TH2*) _file0->Get(hname.ReplaceAll(Form("_frp%i", ifr), Form("_frp%i", ifr+1))));
              } else {
                h2->Add((TH2*) _file0->Get(hname.ReplaceAll(Form("_frp%i", ifr), Form("_frp%i", ifr-1))));
              }
            }
          } else if (ieta == 4) {
            h2->Add((TH2*) _file0->Get(hname.ReplaceAll(Form("_eta%i", ieta), Form("_eta%i", ieta-1))));
            if (1) {  // do not distinguish F/F from F/R, R/R from R/F
              if ((ifr%2) == 0) {
                h2->Add((TH2*) _file0->Get(hname.ReplaceAll(Form("_frp%i", ifr), Form("_frp%i", ifr+1))));
              } else {
                h2->Add((TH2*) _file0->Get(hname.ReplaceAll(Form("_frp%i", ifr), Form("_frp%i", ifr-1))));
              }
            }
          }
        }
        if (1) {  // modify range
          for (int binx=0; binx < h2->GetNbinsX()+2; ++binx) {
            if (h2->GetXaxis()->GetBinLowEdge(binx) < 0.01 || h2->GetXaxis()->GetBinLowEdge(binx) >= 0.25) {
              for (int biny=0; biny < h2->GetNbinsY()+2; ++biny) {
                 h2->SetBinContent(binx, biny, 0);
              }
            }
            if (((ipair == 6 || ipair == 7 || ipair == 8) || (ipair == 9+6 || ipair == 9+7 || ipair == 9+10 || ipair == 9+11 || ipair == 9+14 || ipair == 9+15)) && ((ieta == 0 && h2->GetXaxis()->GetBinLowEdge(binx) >= 0.15) || (ieta == 1 && h2->GetXaxis()->GetBinLowEdge(binx) >= 0.18))) {  // these angles at low eta and low pT are problematic
              for (int biny=0; biny < h2->GetNbinsY()+2; ++biny) {
                h2->SetBinContent(binx, biny, 0);
              }
            }
          }
          for (int binx=0; binx+1 < h2->GetNbinsX()+2; ++binx) {
            for (int biny=0; biny < h2->GetNbinsY()+2; ++biny) {
              h2->SetBinContent(binx, biny, h2->GetBinContent(binx+1, biny));
            }
          }
        }

        if (ipair == 0 || ipair == 1 || ipair == 2) {
          if (good_stats)  h2->RebinX(2);  else  h2->RebinX(4);
        } else if (ipair == 3 || ipair == 4 || ipair == 5) {
          if (good_stats)  h2->RebinX(4);  else  h2->RebinX(6);
        } else if (ipair == 6 || ipair == 7 || ipair == 8) {
          if (good_stats)  h2->RebinX(6);  else  h2->RebinX(8);
        } else if (ipair == 9+0 || ipair == 9+1 || ipair == 9+2 || ipair == 9+3) {
          if (good_stats)  h2->RebinX(4);  else  h2->RebinX(6);
        } else if (ipair == 9+4 || ipair == 9+5 || ipair == 9+8 || ipair == 9+9 || ipair == 9+12 || ipair == 9+13) {
          if (good_stats)  h2->RebinX(4);  else  h2->RebinX(6);
        } else if (ipair == 9+6 || ipair == 9+7 || ipair == 9+10 || ipair == 9+11 || ipair == 9+14 || ipair == 9+15) {
          if (good_stats)  h2->RebinX(6);  else  h2->RebinX(8);
        }
        pf = h2->ProfileX(hname + "_tmp4_pfx", 1, -1, "s");
        //pf->SetMarkerStyle(1); pf->SetMarkerColor(palette[ifr]);
        //pf->SetLineWidth(2); pf->SetLineColor(palette[ifr]);
        //pf->Draw("same");
        num = pf->ProjectionX(hname + "_tmp4_px", "e");  // num 1D histo
        num->Divide(denom);

        if (1) {  // increase errors at small pT (high 1/pT)
          for (int b=1; b<num->GetNbinsX()+1; ++b) {
            if (num->GetBinContent(b) > 1e-6) {
              //num->SetBinError(b, num->GetBinError(b) * (num->GetBinCenter(b) / num->GetBinCenter(1)));
              num->SetBinError(b, num->GetBinError(b) * (1.0 + (1.0 * float(b-1) / num->GetNbinsX())));
            }
          }
        }
        if (1) {  // avoid discontinuity of dphi ~0
          for (int b=1; b<num->GetNbinsX()+1; ++b) {
            double near_zero = 10.;
            double denom_binc = denom->GetBinContent(b);
            double num_binc = num->GetBinContent(b) * denom_binc;  // recover numerator bin content before division
            if (fabs(num_binc) > 1e-9) {
              if (fabs(num_binc) < near_zero || fabs(denom_binc) < near_zero) {  // almost zero but not identically zero
                if ((num_binc < 0. && denom_binc >= 0.) || (num_binc >= 0. && denom_binc < 0.)) {
                  num->SetBinContent(b, 0.);
                  num->SetBinError(b, 999999.);
                }
              }
            }
          }
        }
        if (1) {  // limit the range
          for (int b=0; b<num->GetNbinsX()+2; ++b) {
            if (num->GetBinContent(b) < ymin) {
              num->SetBinContent(b, ymin);
              num->SetBinError(b, 999999.);
            } else if (num->GetBinContent(b) > ymax) {
              num->SetBinContent(b, ymax);
              num->SetBinError(b, 999999.);
            }
          }
        }

        TString ytitle = h2->GetYaxis()->GetTitle();
        ytitle = "Ratio; num = " + ytitle;
        num->GetYaxis()->SetTitle(ytitle);
        num->GetYaxis()->SetTitleFont(h2->GetYaxis()->GetTitleFont());
        num->GetYaxis()->SetTitleSize(h2->GetYaxis()->GetTitleSize());
        num->GetYaxis()->SetLabelFont(h2->GetYaxis()->GetLabelFont());
        num->GetYaxis()->SetLabelSize(h2->GetYaxis()->GetLabelSize());
        num->SetMarkerStyle(1); num->SetMarkerColor(palette[ifr]);
        num->SetLineWidth(2); num->SetLineColor(palette[ifr]);
        //num->SetMinimum(ymin); num->SetMaximum(ymax);
        num->SetStats(0);
        num->Draw();

        // Debug
        //if (hname == "deflection_stp9_frp0_eta3") {
        //  for (int b=0; b<num->GetNbinsX()+2; ++b) {
        //    std::cout << "DEBUG: " << b << " " << pf->GetBinContent(b) << " " << pf->GetBinError(b) << " " << pf->GetBinEntries(b) << " " << num->GetBinContent(b) << " " << num->GetBinError(b) << std::endl;
        //  }
        //}

        // Debug
        //if (1) {
        //  int discontinuous = -1;
        //  for (int b=1; b<num->GetNbinsX()+1; ++b) {
        //    if (num->GetBinCenter(b) > 0.4+0.04)
        //      continue;
        //    if (pf->GetBinEntries(b) < 12) {
        //      discontinuous = b;
        //      break;
        //    }
        //  }
        //  if (discontinuous != -1) {
        //    int b = discontinuous;
        //    std::cout << "DEBUG: NOT CONTINUOUS " << hname << " bin " << b << " " << num->GetBinCenter(b) << " " << pf->GetBinEntries(b) << std::endl;
        //  }
        //}

        // Debug
        if (1) {
          for (int b=1; b<num->GetNbinsX()+1; ++b) {
            if (num->GetBinContent(b) < 0.) {
              std::cout << "DEBUG: NEGATIVE RATIO " << hname << " bin " << b << " " << num->GetBinContent(b) << " " << denom->GetBinContent(b) << std::endl;
            }
          }
        }

        TF1* f1 = new TF1("f1", "pol0");
        //TF1* f1 = new TF1("f1", "pol2");
        int status = num->Fit(f1, "Q0");
        //int status = num->Fit(f1, "Q0", "", 0.04, 0.4+0.04);
        if (status == 0) {
          f1->SetLineWidth(2); f1->SetLineColor(palette[ifr]);
          f1->Draw("same");
          //tlatex->DrawLatex(0.2, 0.88, Form("Fit: %.3f + %.3f x + %.3f x^{2}", f1->GetParameter(0), f1->GetParameter(1), f1->GetParameter(2)));
          tlatex->DrawLatex(0.2, 0.88, Form("Fit: %.3f + nan x + nan x^{2}", f1->GetParameter(0)));

          // Set corrections
          bool good = true;
          if (ipair == 0 || ipair == 1 || ipair == 2) {  // has ME1/1
            if (ieta < 3)  good = false;
          } else if (ipair == 3 || ipair == 4 || ipair == 5) {  // has ME1/2
            if (ieta >= 5)  good = false;
          } else if (ipair == 9+4 || ipair == 9+8 || ipair == 9+12) {  // has ME1/1
            if (ieta < 3)  good = false;
          } else if (ipair == 9+5 || ipair == 9+9 || ipair == 9+13) {  // has ME1/2
            if (ieta >= 5)  good = false;
          }
          if (ipair == 9+0 || ipair == 9+1 || ipair == 9+2 || ipair == 9+3) {  // has RE1/2
            if (ieta >= 5)  good = false;
          } else if (ipair == 9+4 || ipair == 9+5 || ipair == 9+6 || ipair == 9+7) {  // has RE2
            //if ((ipair == 9+4 || ipair == 9+5) && ieta >= 5)  good = false;
            //if ((ipair == 9+6 || ipair == 9+7) && ieta >= 4)  good = false;
            if (ieta >= 5)  good = false;
          } else if (ipair == 9+8 || ipair == 9+9 || ipair == 9+10 || ipair == 9+11) {  // has RE3
            if (ieta >= 6)  good = false;
          } else if (ipair == 9+12 || ipair == 9+13 || ipair == 9+14 || ipair == 9+15) {  // has RE4
            if (ieta >= 6)  good = false;
          }
          if (good) {
            table_common_dphi_2[ipair][ifr][ieta] = f1->GetParameter(0);
          }
        }

        num->SetMinimum(ymin); num->SetMaximum(ymax);
        tline->DrawLine(0, 1.0, 0.5, 1.0);

        hname = Form("deflection_ratio_stp%i_frp%i_eta%i", ipair, ifr, ieta);
        gPad->Print(outdir + hname + ".png");
      }
    }
  }

  if (1) {
    for (int ipair=0; ipair<(9+16); ++ipair) {
      hname = Form("sitrep_stp%i", ipair);
      sitrep = new TH2F(hname, "; #eta bin; FR bin", 12, 0, 12, 4, 0, 4);

      for (int ifr=0; ifr<4; ++ifr) {
        for (int ieta=0; ieta<12; ++ieta) {
          sitrep->SetBinContent(ieta+1, ifr+1, table_common_dphi_2[ipair][ifr][ieta]);
        }
        sitrep->GetYaxis()->SetBinLabel(ifr+1, print_fr[ifr]);
      }

      ymin = 0.4; ymax = 1.8;
      sitrep->SetMinimum(ymin); sitrep->SetMaximum(ymax);
      sitrep->SetNdivisions(512, "X");
      sitrep->SetNdivisions(504, "Y");
      sitrep->SetStats(0);
      sitrep->Draw("COLZ TEXT");
      hname = Form("sitrep_stp%i", ipair);
      gPad->Print(outdir + hname + ".png");
    }

    std::cout << "Dumping the table ..." << std::endl;
    std::cout << "\nstatic const float table_common_dphi_2[9+16][4][12] = {\n  ";
    for (int ipair=0; ipair<(9+16); ++ipair) {
      std::cout << "{\n    ";
      for (int ifr=0; ifr<4; ++ifr) {
        std::cout << "{";
        for (int ieta=0; ieta<12; ++ieta) {
          std::cout << table_common_dphi_2[ipair][ifr][ieta] << (ieta+1<12 ? "," : "");
        }
        std::cout << (ifr+1<4 ? "},\n    " : "}\n  ");
      }
      std::cout << (ipair+1<(9+16) ? "}, " : "}\n");
    }
    std::cout << "};";
    std::cout << std::endl;
  }

}

// _____________________________________________________________________________
// Apply [2]
if (1) {
  int bgcolor1 = TColor::GetColor("#FFFFFF");
  int bgcolor2 = TColor::GetColor("#FFF0F0");

  for (int j=0; j<2; ++j) {
    if      (j == 0) prefix = "";         // uncorr
    else if (j == 1) prefix = "common_";  // corr

    for (int ipair=0; ipair<(9+16); ++ipair) {
      //IETA//for (int ieta=8; ieta<9; ++ieta) {
        for (int ipt=0; ipt<8; ++ipt) {
          for (int ifr=0; ifr<5; ++ifr) {
            hname = Form("deflection_stp%i_frp%i_pt%i", ipair, ifr, ipt); hname = prefix + hname;
            h2 = (TH2*) _file0->Get(hname);
            h2 = (TH2*) h2->Clone(hname + "_tmp5");
            if (ipair == 0 || ipair == 1 || ipair == 2) {
              // pass
            } else if (ipair == 3 || ipair == 4 || ipair == 5) {
              h2->RebinY(2);
            } else if (ipair == 6 || ipair == 7 || ipair == 8) {
              h2->RebinY(4);
            } else if (ipair == 9+0 || ipair == 9+1 || ipair == 9+2 || ipair == 9+3) {
              h2->RebinY(2);
            } else if (ipair  == 9+4 || ipair == 9+5 || ipair == 9+8 || ipair == 9+9 || ipair == 9+12 || ipair == 9+13) {
              h2->RebinY(2);
            } else if (ipair < (9+16)) {
              h2->RebinY(4);
            }
            if (ipt < 4) {
              h2->RebinY(2);
            }
            h2->RebinX(4);
            //IETA//h1 = h2->ProjectionY(hname + "_tmp5_py", ieta+1, ieta+1, "");
            h1 = h2->ProjectionY(hname + "_tmp5_py", 0+1, 5+1, "");
            //h1 = h2->ProjectionY(hname + "_tmp5_py");
            h1->SetMarkerStyle(20); h1->SetMarkerSize(0.1);
            h1->SetMarkerColor(palette[ifr]); h1->SetLineColor(palette[ifr]);

            if (ifr == 0) {
              h1->SetMinimum(0.5); h1->SetMaximum(5e5);
              //IETA//TString xtitle = h1->GetXaxis()->GetTitle(); xtitle = xtitle(0,27); xtitle = xtitle + Form("{%.1f#leq|#eta|<%.1f}",1.2+0.1*ieta, 1.2+0.1*(ieta+1));
              TString xtitle = h1->GetXaxis()->GetTitle();
              //xtitle = xtitle(0,27);
              h1->GetXaxis()->SetTitle(xtitle);
              h1->SetStats(0); h1->Draw("e");
              if (ipt < 4)
                h1->GetXaxis()->SetRangeUser(-600,600);
              else
                h1->GetXaxis()->SetRangeUser(-60,60);
              tlegend->Clear(); tlegend2->Clear();
            } else {
              h1->Draw("e same");
            }

            // Fit
            int status = h1->Fit("gaus", "Q0", "", h1->GetMean()-3.0*h1->GetRMS(), h1->GetMean()+3.0*h1->GetRMS());
            if (h1->Integral() > 100. && status == 0) {
              f1 = h1->GetFunction("gaus");
              f1->SetLineWidth(2); f1->SetLineColor(palette[ifr]);
              f1->Draw("same");
              tlegend->AddEntry(h1, Form("#color[%i]{#mu(p_{T} = %i GeV, %s) = %.1f}", f1->GetLineColor(), print_pt[ipt], print_fr[ifr], f1->GetParameter(1)), "");
              tlegend2->AddEntry(h1, Form("#color[%i]{#sigma(p_{T} = %i GeV, %s) = %.1f}", f1->GetLineColor(), print_pt[ipt], print_fr[ifr], f1->GetParameter(2)), "");
            } else {
              f1 = new TF1();
              f1->SetLineWidth(2); f1->SetLineColor(palette[ifr]);
              tlegend->AddEntry(h1, Form("#color[%i]{#mu(p_{T} = %i GeV, %s) = NaN}", f1->GetLineColor(), print_pt[ipt], print_fr[ifr]), "");
              tlegend2->AddEntry(h1, Form("#color[%i]{#sigma(p_{T} = %i GeV, %s) = NaN}", f1->GetLineColor(), print_pt[ipt], print_fr[ifr]), "");
            }
          }

          tlegend->Draw(); tlegend2->Draw();
          gPad->SetLogy(true);
          //if (ipair < 9)  gPad->SetFillColor(bgcolor1);  else  gPad->SetFillColor(bgcolor2);

          //IETA//hname = Form("deflection_stp%i_frp%i_pt%i_eta%i", ipair, 99, ipt, ieta); hname = prefix + hname;
          hname = Form("deflection_stp%i_frp%i_pt%i", ipair, 99, ipt); hname = prefix + hname;
          gPad->Print(outdir + hname + ".png");
          gPad->SetLogy(false);
          //gPad->SetFillColor(0);
        }
      //IETA//}
    }
  }

  // ___________________________________________________________________________
  // Convoluted
  for (int ipair=0; ipair<(9+16); ++ipair) {
    //IETA//for (int ieta=8; ieta<9; ++ieta) {
      for (int ipt=0; ipt<8; ++ipt) {
        for (int j=0; j<2; ++j) {
          if      (j == 0) prefix = "";         // uncorr
          else if (j == 1) prefix = "common_";  // corr

          hname = Form("deflection_stp%i_frp%i_pt%i", ipair, 4, ipt); hname = prefix + hname;
          h2 = (TH2*) _file0->Get(hname);
          h2 = (TH2*) h2->Clone(hname + "_tmp6");
          if (ipair == 0 || ipair == 1 || ipair == 2) {
            // pass
          } else if (ipair == 3 || ipair == 4 || ipair == 5) {
            h2->RebinY(2);
          } else if (ipair == 6 || ipair == 7 || ipair == 8) {
            h2->RebinY(4);
          } else if (ipair == 9+0 || ipair == 9+1 || ipair == 9+2 || ipair == 9+3) {
            h2->RebinY(2);
          } else if (ipair  == 9+4 || ipair == 9+5 || ipair == 9+8 || ipair == 9+9 || ipair == 9+12 || ipair == 9+13) {
            h2->RebinY(2);
          } else if (ipair < (9+16)) {
            h2->RebinY(4);
          }
          if (ipt < 4) {
            h2->RebinY(2);
          }
          h2->RebinX(4);
          //IETA//h1 = h2->ProjectionY(hname + "_tmp6_py", ieta+1, ieta+1, "");
          h1 = h2->ProjectionY(hname + "_tmp6_py", 0+1, 5+1, "");
          //h1 = h2->ProjectionY(hname + "_tmp6_py");
          h1->SetMarkerStyle(20); h1->SetMarkerSize(0.1);
          h1->SetMarkerColor(palette2[j]); h1->SetLineColor(palette2[j]);

          if (j == 0) {
            h1->SetMinimum(0.5); h1->SetMaximum(5e5);
            //IETA//TString xtitle = h1->GetXaxis()->GetTitle(); xtitle = xtitle(0,27); xtitle = xtitle + Form("{%.1f#leq|#eta|<%.1f}",1.2+0.1*ieta, 1.2+0.1*(ieta+1));
            TString xtitle = h1->GetXaxis()->GetTitle(); xtitle = xtitle(0,27);
            h1->GetXaxis()->SetTitle(xtitle);
            h1->SetStats(0); h1->Draw("e");
            if (ipt < 4)
              h1->GetXaxis()->SetRangeUser(-600,600);
            else
              h1->GetXaxis()->SetRangeUser(-60,60);
            tlegend->Clear(); tlegend2->Clear();
          } else {
            h1->Draw("e same");
          }

          // Fit
          int status = h1->Fit("gaus", "Q0", "", h1->GetMean()-3.0*h1->GetRMS(), h1->GetMean()+3.0*h1->GetRMS());
          if (h1->Integral() > 100. && status == 0) {
            f1 = h1->GetFunction("gaus");
            f1->SetLineWidth(2); f1->SetLineColor(palette2[j]);
            f1->Draw("same");
            tlegend->AddEntry(h1, Form("#color[%i]{#mu(p_{T} = %i GeV, %s) = %.1f}", f1->GetLineColor(), print_pt[ipt], print_corr[j], f1->GetParameter(1)), "");
            tlegend2->AddEntry(h1, Form("#color[%i]{#sigma(p_{T} = %i GeV, %s) = %.1f}", f1->GetLineColor(), print_pt[ipt], print_corr[j], f1->GetParameter(2)), "");

            if (j == 0) {
              table_mean_uncorr[ipair][0][ipt] = f1->GetParameter(1);
              table_sigma_uncorr[ipair][0][ipt] = f1->GetParameter(2);
            } else {
              table_mean_corr[ipair][0][ipt] = f1->GetParameter(1);
              table_sigma_corr[ipair][0][ipt] = f1->GetParameter(2);
            }
          } else {
            f1 = new TF1();
            f1->SetLineWidth(2); f1->SetLineColor(palette2[j]);
            tlegend->AddEntry(h1, Form("#color[%i]{#mu(p_{T} = %i GeV, %s) = NaN}", f1->GetLineColor(), print_pt[ipt], print_corr[j]), "");
            tlegend2->AddEntry(h1, Form("#color[%i]{#sigma(p_{T} = %i GeV, %s) = NaN}", f1->GetLineColor(), print_pt[ipt], print_corr[j]), "");
          }
        }

        tlegend->Draw(); tlegend2->Draw();
        gPad->SetLogy(true);
        if (ipair < 9)  gPad->SetFillColor(bgcolor1);  else  gPad->SetFillColor(bgcolor2);

        //IETA//hname = Form("deflection_stp%i_frp%i_pt%i_eta%i", ipair, 99, ipt, ieta); hname = prefix + hname;
        hname = Form("deflection_stp%i_frp%i_pt%i_convoluted", ipair, 99, ipt); hname = prefix + hname;
        gPad->Print(outdir + hname + ".png");
        gPad->SetLogy(false);
        gPad->SetFillColor(0);
      }
    //IETA//}
  }

  // Resolution
  for (int ipair=0; ipair<(9+16); ++ipair) {
    hname = Form("deflection_stp%i_frp%i_pt%i", ipair, 4, 0); hname = prefix + hname;
    h2 = (TH2*) _file0->Get(hname);
    TString xtitle = h2->GetYaxis()->GetTitle(); xtitle = xtitle(0,27);

    hname = Form("sigma_stp%i", ipair);
    h1 = new TH1F(hname, Form("; 1/p_{T} [GeV]; #sigma of [%s]", xtitle.Data()), 100, 0, 0.5);
    gra = new TGraphAsymmErrors(8-1);
    grb = new TGraphAsymmErrors(8-1);

    for (int ipt=(0+1); ipt<8; ++ipt) {
      gra->SetPoint(ipt-1, 1.0/print_pt[ipt], table_sigma_uncorr[ipair][0][ipt]);
      grb->SetPoint(ipt-1, 1.0/print_pt[ipt], table_sigma_corr[ipair][0][ipt]);
    }

    //ymax = gra->GetMaximum();
    ymax = TMath::MaxElement(gra->GetN(), gra->GetY());
    ymax = TMath::Min(ymax, 200.0);
    h1->SetMinimum(0); h1->SetMaximum(ymax * 2.0);
    h1->SetStats(0); h1->Draw();
    gra->SetLineWidth(2); gra->SetLineColor(palette2[0]);
    gra->SetMarkerStyle(20); gra->SetMarkerColor(palette2[0]);
    grb->SetLineWidth(2); grb->SetLineColor(palette2[1]);
    grb->SetMarkerStyle(20); grb->SetMarkerColor(palette2[1]);
    gra->Draw("lp");
    grb->Draw("lp");

    gPad->Print(outdir + hname + ".png");

    hname = Form("sigmaovermean_stp%i", ipair);
    h1 = new TH1F(hname, Form("; 1/p_{T} [GeV]; #sigma/|#mu| of [%s]", xtitle.Data()), 100, 0, 0.5);
    gra = new TGraphAsymmErrors(8-1);
    grb = new TGraphAsymmErrors(8-1);

    for (int ipt=(0+1); ipt<8; ++ipt) {
      gra->SetPoint(ipt-1, 1.0/print_pt[ipt], table_sigma_uncorr[ipair][0][ipt] / fabs(table_mean_uncorr[ipair][0][ipt]));
      grb->SetPoint(ipt-1, 1.0/print_pt[ipt], table_sigma_corr[ipair][0][ipt] / fabs(table_mean_corr[ipair][0][ipt]));
    }

    //ymax = gra->GetMaximum();
    ymax = TMath::MaxElement(gra->GetN(), gra->GetY());
    ymax = TMath::Min(ymax, 10.0);
    h1->SetMinimum(0); h1->SetMaximum(ymax * 2.0);
    h1->SetStats(0); h1->Draw();
    gra->SetLineWidth(2); gra->SetLineColor(palette2[0]);
    gra->SetMarkerStyle(20); gra->SetMarkerColor(palette2[0]);
    grb->SetLineWidth(2); grb->SetLineColor(palette2[1]);
    grb->SetMarkerStyle(20); grb->SetMarkerColor(palette2[1]);
    gra->Draw("lp");
    grb->Draw("lp");

    gPad->Print(outdir + hname + ".png");
  }
}


}

