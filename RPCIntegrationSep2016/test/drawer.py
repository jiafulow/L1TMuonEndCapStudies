#!/usr/bin/env python

from ROOT import TFile, TH1F, TH2F, TProfile, TGraphAsymmErrors, TEfficiency, TColor, TLine, gROOT, gStyle, gPad


# ______________________________________________________________________________
# Useful functions

palette = map(lambda x: TColor.GetColor(x), ("#333333", "#377eb8", "#e41a1c", "#984ea3", "#ff7f00", "#4daf4a"))


# ______________________________________________________________________________
# Main function
def main(histos, options):

  if options.verbose:
    print "Found %i histograms:" % len(histos)
    for k in sorted(histos.keys()):
      print "  %s" % k

  # Efficiency vs pT
  special_hname = "eff_vs_pt_mode%i_eta%i"
  for j in xrange(4):
    frame, leg = None, None
    for i in xrange(4):
      hname = special_hname % (i, j)
      h = histos[hname]

      if not frame:
        frame = h.GetCopyTotalHisto().Clone(hname+"_frame"); frame.Reset()
        frame.SetMinimum(0); frame.SetMaximum(1.2)
        frame.SetStats(0); frame.Draw()
        xmin, xmax = frame.GetXaxis().GetXmin(), frame.GetXaxis().GetXmax()
        tline = TLine(); tline.SetLineColor(1)
        tline.DrawLine(xmin, 1.0, xmax, 1.0)

      gr = h.CreateGraph()
      gr.SetMarkerColor(palette[i]); gr.SetLineColor(palette[i])
      gr.SetMarkerStyle(20); gr.SetMarkerSize(0.8)
      gr.Draw("p")

    imgname = special_hname % (99, j)
    gPad.SetLogx(True)
    gPad.Print("%s.png" % (options.outdir+imgname))
    gPad.SetLogx(False)

  # Efficiency vs eta
  special_hname = "eff_vs_eta_mode%i_l1pt%i"
  for j in xrange(4):
    frame, leg = None, None
    for i in xrange(4):
      hname = special_hname % (i, j)
      h = histos[hname]

      if not frame:
        frame = h.GetCopyTotalHisto().Clone(hname+"_frame"); frame.Reset()
        frame.SetMinimum(0); frame.SetMaximum(1.2)
        frame.SetStats(0); frame.Draw()
        xmin, xmax = frame.GetXaxis().GetXmin(), frame.GetXaxis().GetXmax()
        tline = TLine(); tline.SetLineColor(1)
        tline.DrawLine(xmin, 1.0, xmax, 1.0)

      gr = h.CreateGraph()
      gr.SetMarkerColor(palette[i]); gr.SetLineColor(palette[i])
      gr.SetMarkerStyle(20); gr.SetMarkerSize(0.8)
      gr.Draw("p")

    imgname = special_hname % (99, j)
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
  if not os.path.isfile(options.infile):
    raise Exception("File does not exist: %s" % options.infile)
  options.tfile = TFile.Open(options.infile)

  # Setup basic drawer
  gROOT.LoadMacro("tdrstyle.C")
  gROOT.ProcessLine("setTDRStyle();")
  gROOT.ProcessLine("gErrorIgnoreLevel = kWarning;")
  gROOT.SetBatch(True)

  histos = {}
  for k in options.tfile.GetListOfKeys():
    h = k.ReadObj()
    if h.ClassName() in ["TH1F", "TH2F", "TProfile", "TEfficiency"]:
      hname = h.GetName()
      histos[hname] = h

  # Call the main function
  main(histos, options)
