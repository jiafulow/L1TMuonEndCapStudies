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

TString hname;
TString prefix;
TH1* h1;
TH2* h2;
TProfile* pf;
TH1* num, * denom;
TH2* sitrep;
TF1* f1;
TGraphAsymmErrors* gr;

//TFile *_file0 = TFile::Open("histos.root");
//TFile *_file0 = TFile::Open("histos_add.root");
//TFile *_file0 = TFile::Open("histos_add_20170123.root");
TFile *_file0 = TFile::Open("histos_add_20170201.root");
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
        if (ipair == 9+3)  continue;

        if (ipair < 9) {
          if (ipair == 4 || ipair == 5 || ipair == 6 || ipair == 7) {
            hname = Form("deflection_stp%i_frp%i_eta%i", ipair, 2, ieta);  // F/R
          } else {
            hname = Form("deflection_stp%i_frp%i_eta%i", ipair, 3, ieta);  // F/F
          }
        } else if (ipair < (9+16)) {
          if (ipair == 9+0) {
            hname = Form("deflection_stp%i_frp%i_eta%i", 3, 3, ieta);  // (1/2,2) F/F
          } else if (ipair == 9+1) {
            hname = Form("deflection_stp%i_frp%i_eta%i", 4, 2, ieta);  // (1/2,3) F/R
          } else if (ipair == 9+2) {
            hname = Form("deflection_stp%i_frp%i_eta%i", 5, 2, ieta);  // (1/2,4) F/R
          } else if (ipair == 9+3) {
            // pass
          } else if (ipair == 9+4) {
            hname = Form("deflection_stp%i_frp%i_eta%i", 0, 3, ieta);  // (1/1,2) F/F
          } else if (ipair == 9+5) {
            hname = Form("deflection_stp%i_frp%i_eta%i", 3, 3, ieta);  // (1/2,2) F/F
          } else if (ipair == 9+6) {
            hname = Form("deflection_stp%i_frp%i_eta%i", 6, 2, ieta);  // (2,3) F/R
          } else if (ipair == 9+7) {
            hname = Form("deflection_stp%i_frp%i_eta%i", 7, 2, ieta);  // (2,4) F/R
          } else if (ipair == 9+8) {
            hname = Form("deflection_stp%i_frp%i_eta%i", 1, 3, ieta);  // (1/1,3) F/F
          } else if (ipair == 9+9) {
            hname = Form("deflection_stp%i_frp%i_eta%i", 4, 2, ieta);  // (1/2,3) F/R
          } else if (ipair == 9+10) {
            hname = Form("deflection_stp%i_frp%i_eta%i", 6, 2, ieta);  // (2,3) F/R
          } else if (ipair == 9+11) {
            hname = Form("deflection_stp%i_frp%i_eta%i", 8, 3, ieta);  // (3,4) F/F
          } else if (ipair == 9+12) {
            hname = Form("deflection_stp%i_frp%i_eta%i", 2, 3, ieta);  // (1/1,4) F/F
          } else if (ipair == 9+13) {
            hname = Form("deflection_stp%i_frp%i_eta%i", 5, 2, ieta);  // (1/2,4) F/R
          } else if (ipair == 9+14) {
            hname = Form("deflection_stp%i_frp%i_eta%i", 7, 2, ieta);  // (2,4) F/R
          } else if (ipair == 9+15) {
            hname = Form("deflection_stp%i_frp%i_eta%i", 8, 3, ieta);  // (3,4) F/F
          }
        }

        h2 = (TH2*) _file0->Get(hname);
        h2 = (TH2*) h2->Clone(hname + "_tmp3");
        if (ipair == 0 || ipair == 1 || ipair == 2) {
          // pass
        } else if (ipair == 3 || ipair == 4 || ipair == 5) {
          h2->RebinX(2);
        } else if (ipair == 6 || ipair == 7 || ipair == 8) {
          h2->RebinX(2);
        } else if (9+0 <= ipair && ipair < 9+16) {
          if (ipair == 9+4 && ieta>0) {  // stp13 hack
            h2->Add((TH2*) _file0->Get(hname.ReplaceAll(Form("_eta%i", ieta), Form("_eta%i", ieta-1)))); h2->RebinX(2);
          }
          h2->RebinX(2);
        }
        pf = h2->ProfileX(hname + "_tmp3_pfx", 1, -1, "s");
        pf->SetMarkerStyle(1); pf->SetMarkerColor(palette[ifr]);
        pf->SetLineWidth(2); pf->SetLineColor(palette[ifr]);
        //pf->Draw("same");
        denom = pf->ProjectionX(hname + "_tmp3_px", "e");  // denom 1D histo

        hname = Form("deflection_stp%i_frp%i_eta%i", ipair, ifr, ieta);
        h2 = (TH2*) _file0->Get(hname);
        h2 = (TH2*) h2->Clone(hname + "_tmp4");
        if (ipair == 0 || ipair == 1 || ipair == 2) {
          // pass
        } else if (ipair == 3 || ipair == 4 || ipair == 5) {
          h2->RebinX(2);
        } else if (ipair == 6 || ipair == 7 || ipair == 8) {
          h2->RebinX(2);
        } else if (9+0 <= ipair && ipair < 9+16) {
          if (ipair == 9+4 && ieta>0) {  // stp13 hack
            h2->Add((TH2*) _file0->Get(hname.ReplaceAll(Form("_eta%i", ieta), Form("_eta%i", ieta-1)))); h2->RebinX(2);
          }
          h2->RebinX(2);
        }
        pf = h2->ProfileX(hname + "_tmp4_pfx", 1, -1, "s");
        pf->SetMarkerStyle(1); pf->SetMarkerColor(palette[ifr]);
        pf->SetLineWidth(2); pf->SetLineColor(palette[ifr]);
        //pf->Draw("same");
        num = pf->ProjectionX(hname + "_tmp4_px", "e");  // num 1D histo

        TString ytitle = h2->GetYaxis()->GetTitle();
        ytitle = "Ratio; num = " + ytitle;
        float ymin = -4., ymax = 8.;
        num->GetYaxis()->SetTitle(ytitle);
        num->GetYaxis()->SetTitleFont(h2->GetYaxis()->GetTitleFont());
        num->GetYaxis()->SetTitleSize(h2->GetYaxis()->GetTitleSize());
        num->GetYaxis()->SetLabelFont(h2->GetYaxis()->GetLabelFont());
        num->GetYaxis()->SetLabelSize(h2->GetYaxis()->GetLabelSize());
        num->Divide(denom);
        for (int b=0; b<num->GetNbinsX()+2; ++b) {
          if (num->GetBinContent(b) < ymin) {
            num->SetBinContent(b, ymin);
            num->SetBinError(b, 999999.);
          } else if (num->GetBinContent(b) > ymax) {
            num->SetBinContent(b, ymax);
            num->SetBinError(b, 999999.);
          }
        }
        num->SetMarkerStyle(1); num->SetMarkerColor(palette[ifr]);
        num->SetLineWidth(2); num->SetLineColor(palette[ifr]);
        //num->SetMinimum(ymin); num->SetMaximum(ymax);
        num->SetStats(0);
        num->Draw();

        // Debug
        if (hname == "deflection_stp14_frp3_eta4") {
          for (int b=0; b<num->GetNbinsX()+2; ++b) {
            std::cout << "DEBUG: " << b << " " << pf->GetBinContent(b) << " " << pf->GetBinError(b) << " " << pf->GetBinEntries(b) << " " << num->GetBinContent(b) << " " << num->GetBinError(b) << std::endl;
          }
        }

        TF1* f1 = new TF1("f1", "pol0");
        //TF1* f1 = new TF1("f1", "pol2");
        int status = num->Fit(f1, "Q0", "", 0.01, 0.4+0.01);
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
            if ((ipair == 9+4 || ipair == 9+5) && ieta >= 5)  good = false;
            if ((ipair == 9+6 || ipair == 9+7) && ieta >= 4)  good = false;
          } else if (ipair == 9+8 || ipair == 9+9 || ipair == 9+10 || ipair == 9+11) {  // has RE3
            if (ieta >= 6)  good = false;
          } else if (ipair == 9+12 || ipair == 9+13 || ipair == 9+14 || ipair == 9+15) {  // has RE4
            if (ieta >= 7)  good = false;
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

    // Fix corrections
    for (int ieta=0; ieta<12; ++ieta) {
      if (ipair < 9) {
        if (ipair == 3 || ipair == 8) {
          table_common_dphi_2[ipair][2][ieta] = table_common_dphi_2[ipair][3][ieta];  // replace F/R with F/F
          table_common_dphi_2[ipair][1][ieta] = table_common_dphi_2[ipair][0][ieta];  // replace R/F with R/R
        } else if (ipair == 4 || ipair == 5 || ipair == 6 || ipair == 7) {
          table_common_dphi_2[ipair][3][ieta] = table_common_dphi_2[ipair][2][ieta];  // replace F/F with F/R
          table_common_dphi_2[ipair][0][ieta] = table_common_dphi_2[ipair][1][ieta];  // replace R/R with R/F
        }
      } else if (ipair < (9+16)) {
        if (ipair == 9+0 || ipair == 9+3 || ipair == 9+4 || ipair == 9+5 || ipair == 9+8 || ipair == 9+9 || ipair == 9+10 || ipair == 9+12 || ipair == 9+13 || ipair == 9+14) {
          table_common_dphi_2[ipair][2][ieta] = table_common_dphi_2[ipair][3][ieta];  // replace F/R with F/F
          table_common_dphi_2[ipair][1][ieta] = table_common_dphi_2[ipair][0][ieta];  // replace R/F with R/R
        } else if (ipair == 9+1 || ipair == 9+2 || ipair == 9+6 || ipair == 9+7 || ipair == 9+11 || ipair == 9+15) {
          table_common_dphi_2[ipair][3][ieta] = table_common_dphi_2[ipair][2][ieta];  // replace F/F with F/R
          table_common_dphi_2[ipair][0][ieta] = table_common_dphi_2[ipair][1][ieta];  // replace R/R with R/F
        }
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

      float ymin = 0.2, ymax = 1.4;
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
            } else if (ipair == 9+4 || ipair == 9+5 || ipair == 9+6 || ipair == 9+7 || ipair == 9+8 || ipair == 9+9 || ipair == 9+10 || ipair == 9+11 || ipair == 9+12 || ipair == 9+13 || ipair == 9+14 || ipair == 9+15) {
              h2->RebinY(4);
            }
            if (ipt < 5) {
              h2->RebinY(2);
            }
            h2->RebinX(4);
            //IETA//h1 = h2->ProjectionY(hname + "_tmp5_py", ieta+1, ieta+1, "");
            h1 = h2->ProjectionY(hname + "_tmp5_py");
            h1->SetMarkerStyle(20); h1->SetMarkerSize(0.1);
            h1->SetMarkerColor(palette[ifr]); h1->SetLineColor(palette[ifr]);

            if (ifr == 0) {
              h1->SetMinimum(0.5); h1->SetMaximum(5e5);
              //IETA//TString xtitle = h1->GetXaxis()->GetTitle(); xtitle = xtitle(0,27); xtitle = xtitle + Form("{%.1f#leq|#eta|<%.1f}",1.2+0.1*ieta, 1.2+0.1*(ieta+1));
              TString xtitle = h1->GetXaxis()->GetTitle(); xtitle = xtitle(0,27);
              h1->GetXaxis()->SetTitle(xtitle);
              h1->SetStats(0); h1->Draw("e");
              if (ipt < 5)
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

          //IETA//hname = Form("deflection_stp%i_frp%i_pt%i_eta%i", ipair, 99, ipt, ieta); hname = prefix + hname;
          hname = Form("deflection_stp%i_frp%i_pt%i", ipair, 99, ipt); hname = prefix + hname;
          gPad->Print(outdir + hname + ".png");
          gPad->SetLogy(false);
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
          } else if (ipair == 9+4 || ipair == 9+5 || ipair == 9+6 || ipair == 9+7 || ipair == 9+8 || ipair == 9+9 || ipair == 9+10 || ipair == 9+11 || ipair == 9+12 || ipair == 9+13 || ipair == 9+14 || ipair == 9+15) {
            h2->RebinY(4);
          }
          if (ipt < 5) {
            h2->RebinY(2);
          }
          h2->RebinX(4);
          //IETA//h1 = h2->ProjectionY(hname + "_tmp6_py", ieta+1, ieta+1, "");
          h1 = h2->ProjectionY(hname + "_tmp6_py");
          h1->SetMarkerStyle(20); h1->SetMarkerSize(0.1);
          h1->SetMarkerColor(palette2[j]); h1->SetLineColor(palette2[j]);

          if (j == 0) {
            h1->SetMinimum(0.5); h1->SetMaximum(5e5);
            //IETA//TString xtitle = h1->GetXaxis()->GetTitle(); xtitle = xtitle(0,27); xtitle = xtitle + Form("{%.1f#leq|#eta|<%.1f}",1.2+0.1*ieta, 1.2+0.1*(ieta+1));
            TString xtitle = h1->GetXaxis()->GetTitle(); xtitle = xtitle(0,27);
            h1->GetXaxis()->SetTitle(xtitle);
            h1->SetStats(0); h1->Draw("e");
            if (ipt < 5)
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
          } else {
            f1 = new TF1();
            f1->SetLineWidth(2); f1->SetLineColor(palette2[j]);
            tlegend->AddEntry(h1, Form("#color[%i]{#mu(p_{T} = %i GeV, %s) = NaN}", f1->GetLineColor(), print_pt[ipt], print_corr[j]), "");
            tlegend2->AddEntry(h1, Form("#color[%i]{#sigma(p_{T} = %i GeV, %s) = NaN}", f1->GetLineColor(), print_pt[ipt], print_corr[j]), "");
          }
        }

        tlegend->Draw(); tlegend2->Draw();
        gPad->SetLogy(true);

        //IETA//hname = Form("deflection_stp%i_frp%i_pt%i_eta%i", ipair, 99, ipt, ieta); hname = prefix + hname;
        hname = Form("deflection_stp%i_frp%i_pt%i_convoluted", ipair, 99, ipt); hname = prefix + hname;
        gPad->Print(outdir + hname + ".png");
        gPad->SetLogy(false);
      }
    //IETA//}
  }
}


}

