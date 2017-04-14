from ROOT import TFile, TTree, TH1F
from itertools import izip


# ______________________________________________________________________________
# Configurables

filename = "L1Ntuple_1.root"

tfile = TFile.Open(filename)
tree_event = tfile.Get("l1EventTree/L1EventTree")
tree_data = tfile.Get("l1UpgradeTree/L1UpgradeTree")
tree_emul = tfile.Get("l1UpgradeEmuTree/L1UpgradeTree")
tree_data_tfmuon = tfile.Get("l1UpgradeTfMuonTree/L1UpgradeTfMuonTree")
tree_emul_tfmuon = tfile.Get("l1UpgradeTfMuonEmuTree/L1UpgradeTfMuonTree")
assert(tree_event)
assert(tree_data)
assert(tree_emul)
assert(tree_data_tfmuon)
assert(tree_emul_tfmuon)


# ______________________________________________________________________________
# Histograms

h_mode = TH1F("mode", "; diff in mode", 33, -16.5, 16.5)

# ______________________________________________________________________________
# Functions

to_list = lambda x : [xx for xx in x]

class MyMuon:
  def __init__(self, bx, qual, mtf, pt, eta, phi):
    self.bx = bx
    self.qual = qual
    self.mtf = mtf
    self.pt = pt
    self.eta = eta
    self.phi = phi

  def __repr__(self):
    return "({0} {1} {2} {3} {4} {5})".format(self.bx, self.qual, self.mtf, self.pt, self.eta, self.phi)

def make_muons(evt):
  muons = []
  for (bx, qual, mtf, pt, eta, phi) in izip(evt.muonBx, evt.muonQual, evt.muonTfMuonIdx, evt.muonIEt, evt.muonIEta, evt.muonIPhi):
    m = MyMuon(bx, qual, mtf, pt, eta, phi)
    muons.append(m)
  return muons

def filter_muons(muons):
  #f = lambda m : (-1 <= m.bx <= +1) and (m.qual != 0) and ((0 <= m.mtf <= 17) or (90 <= m.mtf <= 107))
  f = lambda m : True
  return [m for m in muons if f(m)]


# ______________________________________________________________________________
# Event loop

for (ievt, (evt0, evt1, evt2, evt3, evt4)) in enumerate(izip(tree_event, tree_data, tree_emul, tree_data_tfmuon, tree_emul_tfmuon)):
  run_number, event_number, lumi_number = evt0.Event.run, evt0.Event.event, evt0.Event.lumi

  muons1 = make_muons(evt1)
  muons2 = make_muons(evt2)

  muons1_f = filter_muons(muons1)
  muons2_f = filter_muons(muons2)


  print ievt, run_number, event_number, lumi_number, len(muons1_f), len(muons2_f)
  print "..", muons1_f
  print "..", muons2_f


  ## Debug
  #for (bx, qual, mtf, pt, eta, phi) in izip(evt1.muonBx, evt1.muonQual, evt1.muonTfMuonIdx, evt1.muonEt, evt1.muonEta, evt1.muonPhi):
  #  print "....", "evt1", bx, qual, mtf, pt, eta, phi

  ## Debug
  #for (bx, qual, mtf, pt, eta, phi) in izip(evt2.muonBx, evt2.muonQual, evt2.muonTfMuonIdx, evt2.muonEt, evt2.muonEta, evt2.muonPhi):
  #  print "....", "evt2", bx, qual, mtf, pt, eta, phi

  ## Debug
  #for (bx, qual, mtf, pt, eta, phi) in izip(evt3.L1UpgradeEmtfMuon.tfMuonBx, evt3.L1UpgradeEmtfMuon.tfMuonHwQual, evt3.L1UpgradeEmtfMuon.tfMuonTrackFinderType, evt3.L1UpgradeEmtfMuon.tfMuonHwPt, evt3.L1UpgradeEmtfMuon.tfMuonHwEta, evt3.L1UpgradeEmtfMuon.tfMuonHwPhi):
  #  print "....", "evt3", bx, qual, mtf, pt, eta, phi

  # Debug
  for (bx, qual, mtf, pt, eta, phi) in izip(evt4.L1UpgradeEmtfMuon.tfMuonBx, evt4.L1UpgradeEmtfMuon.tfMuonHwQual, evt4.L1UpgradeEmtfMuon.tfMuonTrackFinderType, evt4.L1UpgradeEmtfMuon.tfMuonHwPt, evt4.L1UpgradeEmtfMuon.tfMuonHwEta, evt4.L1UpgradeEmtfMuon.tfMuonHwPhi):
    print "....", "evt4", bx, qual, mtf, pt, eta, phi

