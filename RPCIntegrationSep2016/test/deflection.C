{

TString hname;
TH1* h1;
TH2* h2;
TProfile* pf;
TH1* num, * denom;
TH2* sitrep;
TF1* f1;
TGraphAsymmErrors* gr;

//TFile *_file0 = TFile::Open("histos.root");
TFile *_file0 = TFile::Open("histos_add.root");
TString outdir = "figures_deflection/";

gStyle->SetPaintTextFormat(".3f");
gStyle->SetPalette(kBird);

int palette[4];
palette[0] = TColor::GetColor("#00A1E1");
palette[1] = TColor::GetColor("#E0006B");
palette[2] = TColor::GetColor("#F78700");
palette[3] = TColor::GetColor("#0E9F00");

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


// Deflection angles vs eta

if (0) {
  int print_pt[8] = {2, 3, 5, 10, 20, 50, 100, 200};

  for (int ipair=0; ipair<9; ++ipair) {
    for (int ipt=0; ipt<8; ++ipt) {
      hname = Form("deflection_stp%i_pt%i", ipair, ipt);
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
        hname = Form("deflection_stp%i_pt%i", ipair, ipt);
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
        h1 = h2->ProjectionY(hname + "_px", ieta+1, ieta+1, "");

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
        h1->SetMarkerStyle(20); h1->SetMarkerSize(0.1);
        h1->SetMarkerColor(palette8[ipt]); h1->SetLineColor(palette8[ipt]);
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

      hname = Form("deflection_stp%i_pt%i_eta%i", ipair, 99, ieta);
      gPad->Print(outdir + hname + ".png");
      gPad->SetLogy(false);
    }
  }
}


// Deflection angles vs pT
if (0) {
  for (int ipair=0; ipair<9; ++ipair) {
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

// Ratios of deflection angles vs pT
if (1) {
  for (int ipair=0; ipair<9; ++ipair) {
    hname = Form("sitrep_stp%i", ipair);
    sitrep = new TH2F(hname, "; #eta bin; FR bin", 12, 0, 12, 4, 0, 4);

    for (int ifr=0; ifr<4; ++ifr) {
      for (int ieta=0; ieta<12; ++ieta) {
        hname = Form("deflection_stp%i_frp%i_eta%i", ipair, 3, ieta);  // F+F
        h2 = (TH2*) _file0->Get(hname);
        h2->GetYaxis()->SetTitleOffset(1.4);
        h2->SetMarkerStyle(6); h2->SetMarkerColor(kGray+3);
        h2->SetStats(0);
        //h2->Draw();

        pf = h2->ProfileX("_pfx", 1, -1, "s");
        pf->SetMarkerStyle(1); pf->SetMarkerColor(palette[ifr]);
        pf->SetLineWidth(2); pf->SetLineColor(palette[ifr]);
        //pf->Draw("same");

        denom = pf->ProjectionX("_px_denom", "e");  // denom 1D histo
        if (ipair == 0 || ipair == 1 || ipair == 2) {
          // pass
        } else if (ipair == 3 || ipair == 4 || ipair == 5) {
          denom->Rebin(2);
        } else if (ipair == 6 || ipair == 7 || ipair == 8) {
          denom->Rebin(2);
        }

        hname = Form("deflection_stp%i_frp%i_eta%i", ipair, ifr, ieta);
        h2 = (TH2*) _file0->Get(hname);
        h2->GetYaxis()->SetTitleOffset(1.4);
        h2->SetMarkerStyle(6); h2->SetMarkerColor(kGray+3);
        h2->SetStats(0);
        //h2->Draw();

        pf = h2->ProfileX("_pfx", 1, -1, "s");
        pf->SetMarkerStyle(1); pf->SetMarkerColor(palette[ifr]);
        pf->SetLineWidth(2); pf->SetLineColor(palette[ifr]);
        //pf->Draw("same");

        num = pf->ProjectionX("_px", "e");  // num 1D histo
        if (ipair == 0 || ipair == 1 || ipair == 2) {
          // pass
        } else if (ipair == 3 || ipair == 4 || ipair == 5) {
          num->Rebin(2);
        } else if (ipair == 6 || ipair == 7 || ipair == 8) {
          num->Rebin(2);
        }

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

        TF1* f1 = new TF1("f1", "pol0");
        //TF1* f1 = new TF1("f1", "pol2");
        int status = num->Fit(f1, "Q0", "", 0.01, 0.45+0.01);
        if (status == 0) {
          f1->SetLineWidth(2); f1->SetLineColor(palette[ifr]);
          f1->Draw("same");
          tlatex->DrawLatex(0.2, 0.88, Form("Fit: %.3f + %.3f x + %.3f x^{2}", f1->GetParameter(0), f1->GetParameter(1), f1->GetParameter(2)));
          sitrep->SetBinContent(ieta+1, ifr+1, f1->GetParameter(0));
        }

        num->SetMinimum(ymin); num->SetMaximum(ymax);
        tline->DrawLine(0, 1.0, 0.5, 1.0);

        hname = Form("deflection_ratio_stp%i_frp%i_eta%i", ipair, ifr, ieta);
        gPad->Print(outdir + hname + ".png");
      }
    }

    if (1) {
      // Reset bins
      if (ipair == 0 || ipair == 1 || ipair == 2) {
        for (int ifr=0; ifr<4; ++ifr) {
          for (int ieta=0; ieta<4; ++ieta) {
            sitrep->SetBinContent(ieta+1, ifr+1, 0.);
          }
        }
      } else if (ipair == 3 || ipair == 4 || ipair == 5) {
        for (int ifr=0; ifr<4; ++ifr) {
          for (int ieta=5; ieta<12; ++ieta) {
            sitrep->SetBinContent(ieta+1, ifr+1, 0.);
          }
        }
      }

      //sitrep->SetMinimum(0.2); sitrep->SetMaximum(1.8);
      sitrep->SetNdivisions(512, "X");
      sitrep->SetNdivisions(504, "Y");
      sitrep->GetYaxis()->SetBinLabel(1, "R+R");
      sitrep->GetYaxis()->SetBinLabel(2, "R+F");
      sitrep->GetYaxis()->SetBinLabel(3, "F+R");
      sitrep->GetYaxis()->SetBinLabel(4, "F+F");
      sitrep->SetStats(0);
      sitrep->Draw("COLZ TEXT");
      hname = Form("sitrep_stp%i", ipair);
      gPad->Print(outdir + hname + ".png");
    }
  }
}


}

