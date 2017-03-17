#ifndef helper_hh
#define helper_hh

struct TracksMatch {
  template<class T1, class T2>
  bool operator()(const T1& trk1, const T2& trk2) const {
    bool m = (
        std::make_tuple(
          trk1.BX(),
          trk1.Endcap(),
          trk1.Sector(),
          trk1.PtLUT().address,
          //trk1.GMT_pt(),
          trk1.Mode(),
          trk1.GMT_eta(),
          trk1.GMT_phi(),
          trk1.GMT_charge(),
          trk1.GMT_quality()
        ) == std::make_tuple(
          trk2.BX(),
          trk2.Endcap(),
          trk2.Sector(),
          trk2.PtLUT().address,
          //trk2.GMT_pt(),
          trk2.Mode(),
          trk2.GMT_eta(),
          trk2.GMT_phi(),
          trk2.GMT_charge(),
          trk2.GMT_quality()
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
        << " ptlut_addr: "  << trk.PtLUT().address
        << " gmt_pt: "      << trk.GMT_pt()
        << " mode: "        << trk.Mode()
        << " eta: "         << trk.GMT_eta()
        << " phi: "         << trk.GMT_phi()
        << " charge: "      << trk.GMT_charge()
        << " quality: "     << trk.GMT_quality()
        << std::endl;
    return;
  }
};

struct HitPrint {
  template<class T>
  void operator()(const T& h) const {
    int bx      = h.BX() + 3;
    int endcap  = (h.Endcap() == 1) ? 1 : 2;
    int sector  = h.PC_sector();
    int station = (h.PC_station() == 0 && h.Subsector() == 1) ? 1 : h.PC_station();
    int chamber = h.PC_chamber() + 1;
    int strip   = (h.Station() == 1 && h.Ring() == 4) ? h.Strip() + 128 : h.Strip();  // ME1/1a
    std::cout << bx << " " << endcap << " " << sector << " " << h.Subsector() << " " << station << " " << h.Valid() << " " << h.Quality() << " " << h.Pattern() << " " << h.Wire() << " " << chamber << " " << h.Bend() << " " << strip << std::endl;
    return;
  }
};

// See http://stackoverflow.com/a/21510185
namespace details {
  template <class T> struct _reversed {
    T& t; _reversed(T& _t): t(_t) {}
    decltype(t.rbegin()) begin() { return t.rbegin(); }
    decltype(t.rend()) end() { return t.rend(); }
  };
}
template <class T> details::_reversed<T> reversed(T& t) { return details::_reversed<T>(t); }

#endif
