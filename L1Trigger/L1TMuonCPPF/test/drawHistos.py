#!/usr/bin/python
import ROOT
from ROOT import gSystem, TFile, gROOT, gStyle, TGaxis
from ROOT import TCanvas, TH1

gROOT.SetBatch()
TGaxis.SetMaxDigits(3)
gStyle.SetOptStat(0)
f = TFile.Open('DQM_CPPF_EMTF.root','read')
c1 = TCanvas('c1', '',1100,1000)

h = []
h2d = []
hdir = "EMTF_CPPF_DQM"
#Declaration of the list of the different setting of the histograms

hnames = ["h1CeVsCuMatches",  "h2CeVsCuPhiCePhiCuDiff", "h2CeVsEuPhiCePhiEuDiff", "h2CeVsEePhiCePhiEeDiff", "h2CuVsEuPhiCuPhiEuDiff", "h2EeVsEuPhiEePhiEuDiff"] 
hxaxisnames = ["", "\phi_{CPPF emulated} - \phi_{CPPF unpacked}", "\phi_{CPPF emulated} - \phi_{EMTF unpacked}", "\phi_{CPPF emulated} - \phi_{EMTF emulated}", "\phi_{CPPF unpacked} - \phi_{EMTF unpacked}", "\phi_{EMTF emulated} - \phi_{EMTF unpacked}"]
hyaxisnames = ["", "", "", "", "", ""]


h2dnames = ["h2CeVsCuChamberCuZoneCu","h2CeVsCuChamberCeZoneCe","h2EeVsEuChamberEuZoneEu","h2EeVsEuChamberEeZoneEe",
            "h2CuVsEuChamberCuZoneCu","h2CuVsEuChamberEuZoneEu","h2CeVsEeChamberCeZoneCe","h2CeVsEeChamberEeZoneEe",
            "h2CuVsEuChamberCuZoneCuBox","h2CuVsEuChamberCuChamberEuBox", "h2EeVsEuPhiEePhiEu", "h2CuVsEuPhiCuPhiEu"]
h2dxaxisnames = ["Subsector(CPPF unpacked)", "Subsector(CPPF emulated)", "Subsector(EMTF unpacked)", "Subsector(EMTF emulated)", 
                 "Subsector(CPPF unpacked)", "Subsector(EMTF unpacked)", "Subsector(CPPF emulated)", "Subsector(EMTF emulated)",
                 "Subsector(CPPF unpacked)", "Subsector(EMTF unpacked)", "\phi_{EMTF emulated}", "\phi_{CPPF unpacked}"]
h2dyaxisnames = ["","","","",
                 "","","","",
                 "", "Subsector(CPPF unpacked)", "\phi_{EMTF unpacked}", "\phi_{EMTF unpacked}"]


'''

hnames = ["Matches_unpacker", "Matches_unpacker_ch", "Matches_unpacker_bx", "Matches_unpacker_int", "Matches_emtf", 
            "Matches_emtf_ch", "Matches_emtf_bx", "Matches_emtf_int"]
h2dnames = ["chamber_emu_unpacker", "phi_emu_unpacker", "theta_emu_unpacker", "bx_emu_unpacker", "phi_emu_unpacker_bx", 
            "theta_emu_unpacker_bx","bx_emu_unpacker_bx", "phi_emu_unpacker_int", 
            "theta_emu_unpacker_int", "bx_occupancy_unpacker", "bx_occupancy_emu_unpacker", 
            "occupancy_unpacker", "occupancy_emu_unpacker", "bx_sector_unpacker", 
            "bx_sector_emu_unpacker", "chamber_emu_emtf_test", "chamber_emu_emtf", "phi_emu_emtf", 
            "theta_emu_emtf", "bx_emu_emtf", "phi_emu_emtf_bx", "theta_emu_emtf_bx", "bx_emu_emtf_bx", 
            "phi_emu_emtf_int", "theta_emu_emtf_int", "occupancy_emtf", "occupancy_emu_emtf", "bx_occupancy_emtf", 
            "bx_occupancy_emu_emtf", "bx_sector_emtf", "bx_sector_emu_emtf",
            "chamber_unpacker_emtf_test", "chamber_unpacker_emtf", "phi_unpacker_emtf", 
            "theta_unpacker_emtf", "bx_unpacker_emtf", "phi_unpacker_emtf_bx", "theta_unpacker_emtf_bx", "bx_unpacker_emtf_bx", 
            "phi_unpacker_emtf_int", "theta_unpacker_emtf_int", "occupancy_unpacker_emtf_e","occupancy_unpacker_emtf_u",  
            "bx_occupancy_unpacker_emtf_e","bx_occupancy_unpacker_emtf_u",  "bx_sector_unpacker_emtf_e", "bx_sector_unpacker_emtf_u"]
h2dxaxisnames = ["Subsector(CPPF emulator)", "#phi(Emulator)", "#theta(Emulator)", "BX(Emulator)",  "#phi(Emulator)", 
             "#theta(Emulator)", "BX(Emulator)","#phi_{int}(Emulator)", 
             "#theta_{int}(Emulator)", "BX(Unpacker)", "BX(Unpacker)", 
             "EMTF sector(CPPF unpacker)", "EMTF sector(CPPF emulator)", "", 
             "", "Subsector(EMTF emulator)", "Subsector(EMTF emulator)", "", 
             "", "", "", "", "", 
             "", "", "", "", "", 
             "", "", "", 
             "", "Subsector(CPPF unpacked)", "#phi_{int}(CPPF unpacker)", 
             "#theta_{int}(CPPF unpacker)", "Time BX (CPPF unpacker)", "#phi_{int}(CPPF unpacker)", "#theta_{int}(CPPF unpacker)", "", 
             "#phi_{int}(CPPF unpacker)", "#theta_{int}(CPPF unpacker)", "EMTF sector(EMTF unpacker)", "EMTF sector(CPPF unpackeri)", 
             "Time BX (EMTF unpacker)", "Time BX (CPPF unpacker)", "", ""
             ]
h2dyaxisnames = ["Subsector(CPPF unpacked)", "#phi(Unpacker)", "#theta(Unpacker)", "BX(Unpacker)",  "#phi(Unpacker)", 
             "#theta(Unpacker)", "BX(Unpacker)","#phi_{int}(Unpacker)", 
             "#theta_{int}(Unpacker)", "BX(Emulator)", "BX(Emulator)",
             "", "", "", 
             "", "Subsector(EMTF unpacked)", "Subsector(EMTF unpacked)", "", 
             "", "", "", "", "", 
             "", "", "", "", "", 
             "", "", "", 
             "", "Subsector(EMTF unpacked)", "#phi_{int}(EMTF unpacker)", 
             "#theta_{int}(EMTF unpacker)", "Time BX (EMTF unpacker)", "#phi_{int}(EMTF unpacker)", "#theta_{int}(EMTF unpacker)", "", 
             "#phi_{int}(EMTF unpacker)", "#theta_{int}(EMTF unpacker)", "", "", 
             "", "", "", ""
             ]
'''
axislabels = ["RE-4/3", "RE-4/2", "RE-3/3", "RE-3/2", "RE-2/2", "RE-1/2", "RE+1/2", "RE+2/2", "RE+3/2", "RE+3/3", "RE+4/2", "RE+4/3"]

#Drawing 1D histograms
for i in range(0,len(hnames)):
  h.append(f.Get(hdir+"/"+hnames[i]))
  h[i].SetTitle('')
  h[i].GetXaxis().SetTitle(str(hxaxisnames[i]))
  h[i].GetYaxis().SetTitle(str(hyaxisnames[i]))
  h[i].Draw()
  c1.SaveAs(hnames[i]+".png")
  c1.SetLogy(1)
  c1.SaveAs(hnames[i]+"Log.png")
  c1.SetLogy(0)
  
#Drawing 2D histograms
c1.SetRightMargin(0.15)
for i in range(0,len(h2dnames)):
  h2d.append(f.Get(hdir+"/"+h2dnames[i]))
  h2d[i].SetTitle('')
  h2d[i].GetXaxis().SetTitle(str(h2dxaxisnames[i]))
  h2d[i].GetYaxis().SetTitle(str(h2dyaxisnames[i]))
  if i < 9 : 
    for j in range(0,12):
      h2d[i].GetYaxis().SetBinLabel(j+1,axislabels[j]) 
  h2d[i].Draw("COLZ")
  c1.SaveAs(h2dnames[i]+".png")

for i in [0,2,4,6]:
  h2d.append(f.Get(hdir+"/"+h2dnames[i]))
  h2d.append(f.Get(hdir+"/"+h2dnames[i+1]))
  h2d[i].Add(h2d[i+1],-1.000001)
  h2d[i].SetTitle('')
  h2d[i].GetXaxis().SetTitle("Subsector")
  h2d[i].GetYaxis().SetTitle(str(h2dyaxisnames[i]))
  if True: 
    for j in range(0,12):
      h2d[i].GetYaxis().SetBinLabel(j+1,axislabels[j]) 
  h2d[i].Draw("COLZ")
  c1.SaveAs(h2dnames[i]+"Diff.png")
