#ifndef Helper_hh
#define Helper_hh

struct TracksMatch {
  template<class T1, class T2>
  bool operator()(const T1& trk1, const T2& trk2) const {
    bool m = (
        std::make_tuple(
          trk1.Endcap(),
          trk1.Sector(),
          trk1.Pt_LUT_addr(),
          trk1.Mode(),
          trk1.Eta_GMT(),
          trk1.Phi_GMT(),
          //trk1.Charge_GMT(),
          trk1.Quality()
        ) == std::make_tuple(
          trk2.Endcap(),
          trk2.Sector(),
          trk2.Pt_LUT_addr(),
          trk2.Mode(),
          trk2.Eta_GMT(),
          trk2.Phi_GMT(),
          //trk2.Charge_GMT(),
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
        << "endcap: " << trk.Endcap()
        << " sector: " << trk.Sector()
        << " ptlut_addr: " << trk.Pt_LUT_addr()
        << " mode: " << trk.Mode()
        << " eta: " << trk.Eta_GMT()
        << " phi: " << trk.Phi_GMT()
        << " charge: " << trk.Charge_GMT()
        << " quality: " << trk.Quality()
        << std::endl;
    return;
  }
};

#endif
