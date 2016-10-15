#ifndef Helper_hh
#define Helper_hh

struct TracksMatch {
  template<class T1, class T2>
  bool operator()(const T1& trk1, const T2& trk2) const {
    bool m = (
        std::make_tuple(
          trk1.BX(),
          trk1.Endcap(),
          trk1.Sector(),
          trk1.Pt_LUT_addr(),
          //trk1.Pt_GMT(),
          trk1.Mode(),
          trk1.Eta_GMT(),
          trk1.Phi_GMT(),
          trk1.Charge_GMT(),
          trk1.Quality()
        ) == std::make_tuple(
          trk2.BX(),
          trk2.Endcap(),
          trk2.Sector(),
          trk2.Pt_LUT_addr(),
          //trk2.Pt_GMT(),
          trk2.Mode(),
          trk2.Eta_GMT(),
          trk2.Phi_GMT(),
          trk2.Charge_GMT(),
          trk2.Quality()
        )
    );
    return m;
  }
};

struct TrackPrint {
  template<class T>
  void operator()(const T& trk) const {
    std::cout
        << "bx: "           << trk.BX()
        << " endcap: "      << trk.Endcap()
        << " sector: "      << trk.Sector()
        << " ptlut_addr: "  << trk.Pt_LUT_addr()
        << " gmt_pt: "      << trk.Pt_GMT()
        << " mode: "        << trk.Mode()
        << " eta: "         << trk.Eta_GMT()
        << " phi: "         << trk.Phi_GMT()
        << " charge: "      << trk.Charge_GMT()
        << " quality: "     << trk.Quality()
        << std::endl;
    return;
  }
};

struct HitPrint {
  template<class T>
  void operator()(const T& h) const {

#ifdef SEP2016_VERSION
    int bx      = h.bx + 3;
    int sector  = h.pc_sector;
    int station = (h.pc_station == 0 && h.subsector == 1) ? 1 : h.pc_station;
    int chamber = h.pc_chamber + 1;
    int strip   = (h.station == 1 && h.ring == 4) ? h.strip + 128 : h.strip;  // ME1/1a
    std::cout << bx << " " << h.endcap << " " << sector << " " << h.subsector << " " << station << " " << h.valid << " " << h.quality << " " << h.pattern << " " << h.wire << " " << chamber << " " << h.bend << " " << strip << std::endl;
#endif

    return;
  }
};

#endif
