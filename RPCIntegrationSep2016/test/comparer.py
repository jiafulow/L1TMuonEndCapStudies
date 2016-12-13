#!/usr/bin/env python

from ROOT import TFile, TH1, TH1F, TH2F, TProfile, TGraphAsymmErrors, TEfficiency, TColor, TLine, TLegend, TLatex, gROOT, gStyle, gPad


# ______________________________________________________________________________
# Globals

palette = map(lambda x: TColor.GetColor(x), ("#333333", "#377eb8", "#e41a1c", "#984ea3", "#ff7f00", "#4daf4a"))

# Legend
tlegend = TLegend(0.70,0.14,0.96,0.30)
tlegend.SetFillStyle(0)
tlegend.SetLineColor(0)
tlegend.SetShadowColor(0)
tlegend.SetBorderSize(0)

# Text
tlatex = TLatex()
tlatex.SetNDC()
tlatex.SetTextFont(42)
tlatex.SetTextSize(0.035)

# Lines
tline = TLine()
tline.SetLineColor(1)


# ______________________________________________________________________________
# Main function
def main(histos, options):

  if options.infile == "histos.genmode0.root":
    filenames = [
      "histos.genmode0.root",
      "histos.zone.root",
    ]
  elif options.infile == "histos.genmode1.root":
    filenames = [
      "histos.genmode1.root",
      "histos.rpc.root",
    ]
  else:
    raise Exception("Fail")

  TH1.AddDirectory(False)

  # Efficiency vs pT
  special_hname = "eff_vs_pt_mode%i_eta3"
  ilabels = ["ideal", "EMTF", "", ""]
  jlabels = ["MuOpen", "DoubleMu", "SingleMu", "Mode15"]
  klabel = "gen 1.24<|#eta|<2.4, L1T p_{T}>0, %s"

  for j in xrange(4):
    frame = None
    for i, filename in enumerate(filenames):
      tfile = TFile.Open(filename)
      hname2 = (special_hname % j)
      h = tfile.Get(hname2)
      assert h != None, "Cannot get %s" % hname2
      hname = (special_hname + "_file%i") % (j, i)
      h = h.Clone(hname)
      histos[hname] = h

      if not frame:
        frame = h.GetCopyTotalHisto().Clone(hname+"_frame"); frame.Reset()
        frame.SetMinimum(0); frame.SetMaximum(1.2)
        frame.GetYaxis().SetTitle("efficiency")
        frame.SetStats(0); frame.Draw()
        xmin, xmax = frame.GetXaxis().GetXmin(), frame.GetXaxis().GetXmax()
        tline.DrawLine(xmin, 1.0, xmax, 1.0)
        tlatex.DrawLatex(0.2, 0.88, klabel % jlabels[j])
        tlegend.Clear()

      gr = h.CreateGraph()
      gr.SetMarkerColor(palette[i]); gr.SetLineColor(palette[i])
      gr.SetMarkerStyle(20); gr.SetMarkerSize(0.8)
      gr.Draw("p")
      tlegend.AddEntry(gr, "#color[%i]{%s}" % (palette[i], ilabels[i]), "")

    tlegend.Draw()
    imgname = (special_hname + "_file%i") % (j, 99)
    gPad.SetLogx(True)
    gPad.Print("%s.png" % (options.outdir+imgname))
    gPad.SetLogx(False)

  # Efficiency vs eta
  special_hname = "eff_vs_eta_mode%i_l1pt0"
  ilabels = ["ideal", "EMTF", "", ""]
  jlabels = ["MuOpen", "DoubleMu", "SingleMu", "Mode15"]
  klabel = "gen p_{T}>20, L1T p_{T}>0, %s"

  for j in xrange(4):
    frame = None
    for i, filename in enumerate(filenames):
      tfile = TFile.Open(filename)
      hname2 = (special_hname % j)
      h = tfile.Get(hname2)
      assert h != None, "Cannot get %s" % hname2
      hname = (special_hname + "_file%i") % (j, i)
      h = h.Clone(hname)
      histos[hname] = h

      if not frame:
        frame = h.GetCopyTotalHisto().Clone(hname+"_frame"); frame.Reset()
        frame.SetMinimum(0); frame.SetMaximum(1.2)
        frame.GetYaxis().SetTitle("efficiency")
        frame.SetStats(0); frame.Draw()
        xmin, xmax = frame.GetXaxis().GetXmin(), frame.GetXaxis().GetXmax()
        tline.DrawLine(xmin, 1.0, xmax, 1.0)
        tlatex.DrawLatex(0.2, 0.88, klabel % jlabels[j])
        tlegend.Clear()

      gr = h.CreateGraph()
      gr.SetMarkerColor(palette[i]); gr.SetLineColor(palette[i])
      gr.SetMarkerStyle(20); gr.SetMarkerSize(0.8)
      gr.Draw("p")
      tlegend.AddEntry(gr, "#color[%i]{%s}" % (palette[i], ilabels[i]), "")

    tlegend.Draw()
    imgname = (special_hname + "_file%i") % (j, 99)
    gPad.Print("%s.png" % (options.outdir+imgname))



# ______________________________________________________________________________
if __name__ == '__main__':
  import argparse
  import os

  # Setup argument parser
  parser = argparse.ArgumentParser()
  parser.add_argument("--infile", default="histos.root", help="input file (default: %(default)s)")
  parser.add_argument("--outdir", default="figures/", help="output directory (default: %(default)s)")
  parser.add_argument("-v", "--verbose", action="count", default=0, help="verbosity (default: %(default)s)")
  options = parser.parse_args()
  if not options.outdir.endswith("/"):
    options.outdir += "/"
  if not os.path.exists(options.outdir):
    os.makedirs(options.outdir)
  #if not os.path.isfile(options.infile):
  #  raise Exception("File does not exist: %s" % options.infile)
  #options.tfile = TFile.Open(options.infile)

  # Setup basic drawer
  gROOT.LoadMacro("tdrstyle.C")
  gROOT.ProcessLine("setTDRStyle();")
  gROOT.ProcessLine("gErrorIgnoreLevel = kWarning;")
  gROOT.SetBatch(True)
  gStyle.SetNdivisions(510, "Y")
  gStyle.SetEndErrorSize(0)
  gStyle.SetPadGridX(True)
  gStyle.SetPadGridY(True)

  histos = {}
  #for k in options.tfile.GetListOfKeys():
  #  h = k.ReadObj()
  #  if h.ClassName() in ["TH1F", "TH2F", "TProfile", "TEfficiency"]:
  #    hname = h.GetName()
  #    histos[hname] = h

  # Call the main function
  main(histos, options)
