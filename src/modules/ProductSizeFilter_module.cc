
// ======================================================================
//
// ProductSizeFilter_plugin
//
// here is an example of the stuff we want to use as filter conditions:
//       Size in bytes   Fraction Data Product Name
//         138517605      0.608 sim::SimPhotonss_largeant__G4.
//          62785966      0.275 sim::SimChannels_largeant__G4.
//          26289610      0.115 simb::MCParticles_largeant__G4.
//
//
//  These data products cause issues on theta.
//
/*
https://dune.github.io/computing-training-202105/06-intro-art-larsoft/index.html
Note from documentation about naming:

simb::MCParticles_largeant__G4Stage1

here, simb::MCParticle is the name of the class that defines the data product.
The “s” after the data product name is added by art – you have no choice in this
even if the plural of your noun ought not to just add an “s”.
The underscore separates the data product name from the module name, “largeant”.
Another underscore separates the module name and the instance name, which in this
example is the empty string – there are two underscores together there.
The last string is the process name and usually is not needed to be specified
in data product retrieval.

SS: simb::MCParticles size is still not what we are checking against. 
Will deal with it later if needed. 
*/
// ======================================================================


#include <art/Framework/Principal/Handle.h>
#include <art/Framework/Principal/Event.h>
#include <art/Framework/Core/SharedFilter.h>
#include <art/Framework/Core/ModuleMacros.h>

#include <fhiclcpp/types/Atom.h>

// I suspect these may not be the correct paths to this headers.
// If nusidata is too difficult to find, then it can likely be removed from this

#include "lardataobj/Simulation/SimPhotons.h"
#include "lardataobj/Simulation/SimChannel.h"
#include "nusimdata/SimulationBase/MCParticle.h"

namespace art {
  class ProductSizeFilter;
}
using namespace fhicl;
using art::ProductSizeFilter;

// ======================================================================

size_t size_in_bytes(std::vector<sim::OnePhoton> const& v) {
  return v.size()*sizeof(sim::IDE) + sizeof(v);
}

size_t size_in_bytes(sim::SimPhotons const& t) {
  return sizeof(int) + size_in_bytes(static_cast<std::vector<sim::OnePhoton>const& >(t));
} 
size_t size_in_bytes(std::vector<sim::SimPhotons> const& v) {
  size_t res = 0;
  for (auto const& x : v) res += size_in_bytes(x);
  return res + sizeof(v);
}

size_t size_in_bytes(std::vector<sim::IDE> const& v) {
  return (v.size()*sizeof(sim::IDE))+ sizeof(v);
}

size_t size_in_bytes(sim::TDCIDE const& t) {
  return sizeof(t.first) + size_in_bytes(t.second);
}

size_t size_in_bytes(std::vector<sim::TDCIDE> const& v) {
  size_t res = 0;
  for (auto const& x : v) res += size_in_bytes(x);
  return res + sizeof(v);
}

size_t size_in_bytes(sim::SimChannel const& s) {
  return sizeof(unsigned int) + size_in_bytes(s.TDCIDEMap());
}

size_t size_in_bytes(std::vector<sim::SimChannel> const& v) {
  size_t res = 0;
  for (auto const& x : v) res += size_in_bytes(x);
  return res + sizeof(v);
}
/// 
//
//
class art::ProductSizeFilter : public SharedFilter {
  public:
    struct Config {
      Atom<size_t> maxSizePhotons{Name("maxSizePhotons"),500000000};
      Atom<size_t> maxSizeChannels{Name("maxSizeChannels"),500000000};
      Atom<size_t> maxSizeParticles{Name("maxSizeParticles"),500000000};
    };

    using Parameters = Table<Config>;
    explicit ProductSizeFilter(Parameters const&, ProcessingFrame const&);

  private:
    bool filter(Event&, ProcessingFrame const&) override;

    size_t max_size_Photons_;
    size_t max_size_Channels_;
    size_t max_size_Particles_;

}; // ProductSizeFilter

// ======================================================================

ProductSizeFilter::ProductSizeFilter(Parameters const& config, ProcessingFrame const&)
  : SharedFilter{config}
  , max_size_Photons_{config().maxSizePhotons()}
  , max_size_Channels_{config().maxSizeChannels()}
  , max_size_Particles_{config().maxSizeParticles()}
{
  async<InEvent>();
}

  bool
art::ProductSizeFilter::filter(art::Event& e, ProcessingFrame const& )
{

  art::Handle<std::vector<sim::SimPhotons>> h_photons;
  art::Handle<std::vector<sim::SimChannel>> h_channels;
  art::Handle<std::vector<simb::MCParticle>> h_particles;

  // no instance label
  if (e.getByLabel("largeant", "", h_photons)) {
    size_t photons_size = size_in_bytes(*h_photons);
    for (auto i=0; i< h_photons->size(); ++i) {
      photons_size+=((*h_photons)[i].size()*sizeof(sim::OnePhoton));
    }
    if (photons_size > max_size_Photons_) {
    std::cout << "Event:" << e.id() << ", Photons size: " << photons_size << std::endl;
    return false;
    }
  }
  
  if (e.getByLabel("largeant", "", h_channels)) {
    size_t channels_size = size_in_bytes(*h_channels); 
    if (channels_size > max_size_Channels_) {
    std::cout << "Event: " << e.id() << ", channels size: " << channels_size << std::endl;
    return false;
    }
  }

  if (e.getByLabel("largeant", "", h_particles) and h_particles->size() > max_size_Particles_) {
    std::cout << "Event: " << e.id() << std::endl; 
    return false;
  }

  std::cout << e.id() << ", particles size: " << h_particles->size() << std::endl;

  return true;

}

DEFINE_ART_MODULE(art::ProductSizeFilter)
