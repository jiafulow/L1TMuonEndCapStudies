#ifndef L1TMuonEndCapStudies_EmuAccuracy_Helper_hh
#define L1TMuonEndCapStudies_EmuAccuracy_Helper_hh

// Reference
//   https://github.com/abrinke1/EMTFAnalyzer/blob/master/AWBTools/macros/Helper.py

float calcPhiGlobDeg(int phi_loc_int, int sector) {
    float phi_loc_deg = ((float) phi_loc_int / 60.0) - 22;
    float phi_glob_deg = phi_loc_deg + 60 * (sector - 1) + 15;
    if (phi_glob_deg <   0)  phi_glob_deg = phi_glob_deg + 360;
    if (phi_glob_deg > 360)  phi_glob_deg = phi_glob_deg - 360;
    return phi_glob_deg;
}

template<class T1, class T2>
bool tracksMatch(const T1& trk1, const T2& trk2) {
    float phi_loc_deg_corr = (trk1.Phi_loc_int() / (0.625 * 60)) * 0.979755;
    int phi_GMT = std::floor(phi_loc_deg_corr - 35);

    if (trk1.Phi_GMT() != phi_GMT) {
        std::cout << "trk1 phi_GMT (calc) = " << trk1.Phi_GMT() << " (" << phi_GMT << ") from phi_loc " << trk1.Phi_loc_int() << std::endl;
    }

    if (trk1.Phi_loc_int() == trk2.Phi_loc_int() && trk1.Sector() == trk2.Sector() && trk1.Phi_GMT() != trk2.Phi_GMT()) {
        std::cout << "trk1 phi_loc (GMT) = " << trk1.Phi_loc_int() << " (" << trk1.Phi_GMT() << "), trk2 phi_loc (GMT) = " << trk2.Phi_loc_int() << " (" << trk2.Phi_GMT() << ")" << std::endl;
    }

    // Exact match requirement
    //if (trk1.Eta_GMT() == trk2.Eta_GMT() && trk1.Phi_loc_int() == trk2.Phi_loc_int() && trk1.BX() == trk2.BX() && trk1.Mode() == trk2.Mode())
    //    return true;
    //else
    //    return false;

    // Non-exact match requirement
    if (std::abs(trk1.Eta_GMT() - trk2.Eta_GMT()) < 6 )
        if (std::abs(trk1.Phi_GMT() - trk2.Phi_GMT()) < 6 )
            return true;
        //else if (trk1.Sector() == trk2.Sector() + 1 and std::abs(trk1.Phi_GMT() + 95 - trk2.Phi_GMT()) < 8)
        //    return true;
        //else if (trk1.Sector() == 1 and trk2.Sector() == 6 and std::abs(trk1.Phi_GMT() + 95 - trk2.Phi_GMT()) < 8)
        //    return true;
        else
            return false;
    else
        return false;
}

template<class T>
void printEvent(const T& eid) {
    std::cout << "*********** Run " << eid.run() << ", Lumi " << eid.luminosityBlock() << ", Event " << eid.event() << " ***********" << std::endl;
}

template<class T>
void printEMTFHit(const T& hit) {
    std::cout
        << "BX = "            << hit.BX()
        << ", endcap = "       << hit.Endcap()
        << ", station = "      << hit.Station()
        << ", sector = "       << hit.Sector()
        << ", subsector = "    << hit.Subsector()
        << ", ring = "         << hit.Ring()
        << ", CSC ID = "       << hit.CSC_ID()
        << ", chamber = "      << hit.Chamber()
        << ", strip = "        << hit.Strip()
        << ", wire = "         << hit.Wire()
        << ", neighbor = "     << hit.Neighbor()
        << ", valid = "        << hit.Valid()
        << std::endl;
}

template<class T>
void printEMTFHitExtra(const T& hit) {
    printEMTFHit(hit);
    std::cout
        << "phi_loc_int = "   << hit.Phi_loc_int()
        << ", theta_int = "    << hit.Theta_int()
        //<< ", phi_glob_deg = " << hit.Phi_glob_deg()
        << ", phi_glob_deg = " << calcPhiGlobDeg(hit.Phi_loc_int(), hit.Sector())
        << ", eta = "          << hit.Eta()
        << std::endl;
}

void printSimulatorHitHeader() {
    std::cout << "EMULATOR HITS FOR SIMULATOR: bx, endcap, sector, subsector, station, valid, quality, CLCT pattern, wiregroup, cscid, bend, halfstrip" << std::endl;
}

template<class T>
void printSimulatorHit(const T& hit) {
    // From Matt:
    //if(C3.Id() > 9)
    //    std::cout<<BX-3<<" "<<endcap<<" "<<sector<<" "<<sub<<" "<<station<<" 1 "<<quality<<" "<<pattern<<" "<<wire<<" "<<C3.Id() - 9<<" 0 "<<strip<<"\n";
    //else
    //    std::cout<<BX-3<<" "<<endcap<<" "<<sector<<" "<<sub<<" "<<station<<" 1 "<<quality<<" "<<pattern<<" "<<wire<<" "<<C3.Id()<<" 0 "<<strip<<"\n";

    int bx      = hit.BX() + 6 - 3;
    int endcap  = (hit.Endcap() == 1) ? 1 : 2;
    int sector  = hit.Sector();
    int sub     = (hit.Subsector() < 0) ? 0 : hit.Subsector();
    int station = hit.Station();
    int valid   = hit.Valid();
    int quality = hit.Quality();
    int pattern = hit.Pattern();
    int wire    = hit.Wire();
    //int cscid   = (hit.Ring() == 4) ? hit.CSC_ID() + 9 : hit.CSC_ID();
    int cscid   = hit.CSC_ID();
    int bend    = (hit.Bend() < 0) ? 0 : hit.Bend();
    int strip   = hit.Strip();

    std::cout << bx << " " << endcap << " " << sector << " " << sub << " " << station << " " << valid << " " << quality << " " << pattern << " " << wire << " " << cscid << " " << bend << " " << strip << std::endl;
    //std::cout << bx << ", " << endcap << ", " << sector << ", " << sub << ", " << station << ", " << valid << ", " << quality << ", " << pattern << ", " << wire << ", " << cscid << ", " << bend << ", " << strip << std::endl;
}

template<class T>
void printEMTFTrack(const T& trk) {
    std::cout
        << "BX = "            << trk.BX()
        << ", sector = "       << trk.Sector()
        << ", mode = "         << trk.Mode()
        << ", quality = "      << trk.Quality()
        << ", phi_loc_int = "  << trk.Phi_loc_int()
        << ", phi_GMT = "      << trk.Phi_GMT()
        << ", eta_GMT = "      << trk.Eta_GMT()
        << ", pT_GMT = "       << trk.Pt_GMT()
        << ", phi_glob_deg = " << trk.Phi_glob_deg()
        << ", eta = "          << trk.Eta()
        << ", pT = "           << trk.Pt()
        << ", has some (all) neighbor hits = " << trk.Has_neighbor() << " (" << trk.All_neighbor() << ")"
        << std::endl;
}

template<class T>
void printPtLUT(const T& trk) {
    if (trk.DPhi_12() != -999)  std::cout << "dPhi_12 = " << trk.DPhi_12() << ", ";
    if (trk.DPhi_13() != -999)  std::cout << "dPhi_13 = " << trk.DPhi_13() << ", ";
    if (trk.DPhi_14() != -999)  std::cout << "dPhi_14 = " << trk.DPhi_14() << ", ";
    if (trk.DPhi_23() != -999)  std::cout << "dPhi_23 = " << trk.DPhi_23() << ", ";
    if (trk.DPhi_24() != -999)  std::cout << "dPhi_24 = " << trk.DPhi_24() << ", ";
    if (trk.DPhi_34() != -999)  std::cout << "dPhi_34 = " << trk.DPhi_34() << ", ";
    if (trk.DTheta_12() != -999)  std::cout << "dTheta_12 = " << trk.DTheta_12() << ", ";
    if (trk.DTheta_13() != -999)  std::cout << "dTheta_13 = " << trk.DTheta_13() << ", ";
    if (trk.DTheta_14() != -999)  std::cout << "dTheta_14 = " << trk.DTheta_14() << ", ";
    if (trk.DTheta_23() != -999)  std::cout << "dTheta_23 = " << trk.DTheta_23() << ", ";
    if (trk.DTheta_24() != -999)  std::cout << "dTheta_24 = " << trk.DTheta_24() << ", ";
    if (trk.DTheta_34() != -999)  std::cout << "dTheta_34 = " << trk.DTheta_34() << ", ";
    if (trk.CLCT_1() != -999)  std::cout << "clct_1 = " << trk.CLCT_1() << ", ";
    if (trk.CLCT_2() != -999)  std::cout << "clct_2 = " << trk.CLCT_2() << ", ";
    if (trk.CLCT_3() != -999)  std::cout << "clct_3 = " << trk.CLCT_3() << ", ";
    if (trk.CLCT_4() != -999)  std::cout << "clct_4 = " << trk.CLCT_4() << ", ";
    if (trk.FR_1() != -999)  std::cout << "fr_1 = " << trk.FR_1() << ", ";
    if (trk.FR_2() != -999)  std::cout << "fr_2 = " << trk.FR_2() << ", ";
    if (trk.FR_3() != -999)  std::cout << "fr_3 = " << trk.FR_3() << ", ";
    if (trk.FR_4() != -999)  std::cout << "fr_4 = " << trk.FR_4() << ", ";
    std::cout << "address = " << trk.Pt_LUT_addr() << std::endl;
}

#endif
