#include <iostream>
#include <cmath>
#include <string>

#include "AvalancheMicroscopic.hh"
#include "FundamentalConstants.hh"
#include "GarfieldConstants.hh"
#include "Random.hh"
#include "MediumMagboltz.hh"

namespace Garfield {

AvalancheMicroscopic::AvalancheMicroscopic() :
  sensor(0), 
  nPhotons(0), nElectrons(0), nHoles(0), nIons(0), 
  nElectronEndpoints(0), nHoleEndpoints(0),
  usePlotting(false), viewer(0),
  plotExcitations(true), plotIonisations(true), plotAttachments(true),   histElectronEnergy(0), histHoleEnergy(0),
  hasElectronEnergyHistogram(false), hasHoleEnergyHistogram(false),
  histDistance(0), hasDistanceHistogram(false), distanceOption('r'),
  nDistanceHistogramTypes(0),
  histSecondary(0), hasSecondaryHistogram(false),
  useSignal(false), useInducedCharge(false),
  useDriftLines(false), usePhotons(false), 
  useBandStructureDefault(true),
  useNullCollisionSteps(false), useBfield(false),
  rb11(1.), rb12(0.), rb13(0.), rb21(0.), rb22(1.), rb23(0.),
  rb31(0.), rb32(0.), rb33(1.), rx22(1.), rx23(0.), rx32(0.), rx33(1.),
  deltaCut(0.), gammaCut(0.),
  sizeCut(-1), nCollSkip(1),
  hasTimeWindow(false), tMin(0.), tMax(0.),
  hasUserHandleStep(false), 
  hasUserHandleAttachment(false),
  hasUserHandleInelastic(false),
  hasUserHandleIonisation(false),
  userHandleStep(0), 
  userHandleAttachment(0), 
  userHandleInelastic(0),
  userHandleIonisation(0),
  debug(false) {
  
  className = "AvalancheMicroscopic";
  distanceHistogramType.clear();
  
  stack.reserve(1000);
  endpointsElectrons.reserve(1000);
  endpointsHoles.reserve(1000);
  photons.reserve(100);
  stack.clear();
  endpointsElectrons.clear();
  endpointsHoles.clear();
  photons.clear();

  distanceHistogramType.clear();
  
}

void 
AvalancheMicroscopic::SetSensor(Sensor* s) {

  if (!s) {
    std::cerr << className << "::SetSensor:\n";
    std::cerr << "    Sensor pointer is a null pointer.\n";
    return;
  }
  sensor = s;

}

void
AvalancheMicroscopic::EnablePlotting(ViewDrift* view) {

  if (!view) {
    std::cerr << className << "::EnablePlotting:\n";
    std::cerr << "    Viewer pointer is a null pointer.\n";
    return;
  }
  
  viewer = view;
  usePlotting = true;
  if (!useDriftLines) {
    std::cout << className << "::EnablePlotting:\n";
    std::cout << "    Enabling storage of drift line.\n";
    EnableDriftLines();
  }

}

void
AvalancheMicroscopic::DisablePlotting() {
 
  viewer = 0;
  usePlotting = false;

}

void 
AvalancheMicroscopic::EnableElectronEnergyHistogramming(TH1* histo) {

  if (!histo) {
    std::cerr << className << "::EnableElectronEnergyHistogramming:\n"; 
    std::cerr << "    Histogram pointer is a null pointer.\n";
    return;
  }
  
  histElectronEnergy = histo;
  hasElectronEnergyHistogram = true;
  
}

void 
AvalancheMicroscopic::DisableElectronEnergyHistogramming() {

  hasElectronEnergyHistogram = false;
  
}

void 
AvalancheMicroscopic::EnableHoleEnergyHistogramming(TH1* histo) {

  if (!histo) {
    std::cerr << className << "::EnableHoleEnergyHistogramming:\n"; 
    std::cerr << "    Histogram pointer is a null pointer.\n";
    return;
  }
  
  histHoleEnergy = histo;
  hasHoleEnergyHistogram = true;
  
}

void 
AvalancheMicroscopic::DisableHoleEnergyHistogramming() {

  hasHoleEnergyHistogram = false;
  
}

void 
AvalancheMicroscopic::SetDistanceHistogram(TH1* histo, const char opt) {

  if (!histo) {
    std::cerr << className << "::SetDistanceHistogram:\n"; 
    std::cerr << "    Histogram pointer is a null pointer.\n";
    return;
  }
  
  histDistance = histo;
  hasDistanceHistogram = true;

  if (opt == 'x' || opt == 'y' || opt == 'z' || opt == 'r') {
    distanceOption = opt;
  } else {
    std::cerr << className << "::SetDistanceHistogram:";
    std::cerr << "    Unknown option " << opt << ".\n";
    std::cerr << "    Valid options are x, y, z, r.\n";
    std::cerr << "    Using default value (r).\n";
    distanceOption = 'r';
  }

  if (nDistanceHistogramTypes <= 0) {
    std::cout << className << "::SetDistanceHistogram:\n";
    std::cout << "    Don't forget to call EnableDistanceHistogramming.\n";
  }
  
}

void
AvalancheMicroscopic::EnableDistanceHistogramming(const int type) {

  // Check if this type of collision is already registered 
  // for histogramming.
  if (nDistanceHistogramTypes > 0) {
    for (int i = nDistanceHistogramTypes; i--;) {
      if (distanceHistogramType[i] == type) {
        std::cout << className << "::EnableDistanceHistogramming:\n";
        std::cout << "    Collision type " << type 
                  << " is already histogrammed.\n";
        return;
      }
    }
  }

  distanceHistogramType.push_back(type);
  ++nDistanceHistogramTypes;
  std::cout << className << "::EnableDistanceHistogramming:\n";
  std::cout << "    Histogramming of collision type " 
            << type << " enabled.\n";
  if (!hasDistanceHistogram) {
    std::cout << "    Don't forget to set the histogram.\n";
  }

}

void
AvalancheMicroscopic::DisableDistanceHistogramming(const int type) {

  if (nDistanceHistogramTypes <= 0) {
    std::cerr << className << "::DisableDistanceHistogramming:\n";
    std::cerr << "    Collision type " << type 
              << " is not histogrammed.\n";
    return;
  } 

  for (int i = nDistanceHistogramTypes; i--;) {
    if (distanceHistogramType[i] == type) {
      distanceHistogramType.erase(distanceHistogramType.begin() + i);
      --nDistanceHistogramTypes;
      std::cout << "    Histogramming of collision type "
                << type << " disabled.\n";
      return;
    }
  }
  
  std::cerr << className << "::DisableDistanceHistogramming:\n"; 
  std::cerr << "    Collision type " << type 
            << " is not histogrammed.\n";

}

void 
AvalancheMicroscopic::DisableDistanceHistogramming() {

  hasDistanceHistogram = false;
  nDistanceHistogramTypes = 0;
  distanceHistogramType.clear();
  
}

void 
AvalancheMicroscopic::EnableSecondaryEnergyHistogramming(TH1* histo) {

  if (!histo) {
    std::cerr << className << "::EnableSecondaryEnergyHistogramming:\n"; 
    std::cerr << "    Histogram pointer is a null pointer.\n";
    return;
  }
  
  histSecondary = histo;
  hasSecondaryHistogram = true;
  
}

void 
AvalancheMicroscopic::DisableSecondaryEnergyHistogramming() {

  hasSecondaryHistogram = false;
  
}

void 
AvalancheMicroscopic::SetCollisionSteps(const int n) {

  if (n <= 0) {
    std::cerr << className << "::SetCollisionSteps:\n";
    std::cerr << "    Number of collisions to be skipped set to" 
              << " default value (100).\n";
    nCollSkip = 100;
    return;
  }
  
  nCollSkip = n;
  
}

void
AvalancheMicroscopic::SetTimeWindow(const double t0, const double t1) {

  if (fabs(t1 - t0) < Small) {
    std::cerr << className << "::SetTimeWindow:\n";
    std::cerr << "    Time interval must be greater than zero.\n";
    return;
  }

  tMin = std::min(t0, t1);
  tMax = std::max(t0, t1);
  hasTimeWindow = true;

}

void
AvalancheMicroscopic::UnsetTimeWindow() {

  hasTimeWindow = false;

}

void 
AvalancheMicroscopic::GetElectronEndpoint(const int i, 
  double& x0, double& y0, double& z0, double& t0, double& e0,
  double& x1, double& y1, double& z1, double& t1, double& e1,
  int& status) const {
  
  if (i < 0 || i >= nElectronEndpoints) {
    std::cerr << className << "::GetElectronEndpoint:\n";
    std::cerr << "    Endpoint index " << i << " out of range.\n";
    x0 = y0 = z0 = t0 = e0 = 0.;
    x1 = y1 = z1 = t1 = e1 = 0.;
    status = 0;
    return;
  }

  x0 = endpointsElectrons[i].x0; 
  y0 = endpointsElectrons[i].y0; 
  z0 = endpointsElectrons[i].z0;
  t0 = endpointsElectrons[i].t0; 
  e0 = endpointsElectrons[i].e0;
  x1 = endpointsElectrons[i].x;  
  y1 = endpointsElectrons[i].y;  
  z1 = endpointsElectrons[i].z;
  t1 = endpointsElectrons[i].t;  
  e1 = endpointsElectrons[i].energy;  
  status = endpointsElectrons[i].status; 

}

// megan: added versions with id, potential0, potential
void
AvalancheMicroscopic::GetElectronEndpoint(const int i,
  double& x0, double& y0, double& z0, double& t0, double& e0,
  double& x1, double& y1, double& z1, double& t1, double& e1,
  int& status, int& id) const {
  
  if (i < 0 || i >= nElectronEndpoints) {
    std::cerr << className << "::GetElectronEndpoint:\n";
    std::cerr << "    Endpoint index " << i << " out of range.\n";
    x0 = y0 = z0 = t0 = e0 = 0.;
    x1 = y1 = z1 = t1 = e1 = 0.;
    status = 0; id = 0;
    return;
  }
  
  x0 = endpointsElectrons[i].x0;
  y0 = endpointsElectrons[i].y0;
  z0 = endpointsElectrons[i].z0;
  t0 = endpointsElectrons[i].t0;
  e0 = endpointsElectrons[i].e0;
  x1 = endpointsElectrons[i].x;
  y1 = endpointsElectrons[i].y;
  z1 = endpointsElectrons[i].z;
  t1 = endpointsElectrons[i].t;
  e1 = endpointsElectrons[i].energy;
  status = endpointsElectrons[i].status;
  id = endpointsElectrons[i].id;
  
}

void
AvalancheMicroscopic::GetElectronEndpoint(const int i,
  double& x0, double& y0, double& z0, double& t0, double& e0,
  double& x1, double& y1, double& z1, double& t1, double& e1,
  int& status, int& id, double& potential0, double& potential1) const {

  if (i < 0 || i >= nElectronEndpoints) {
    std::cerr << className << "::GetElectronEndpoint:\n";
    std::cerr << "    Endpoint index " << i << " out of range.\n";
    x0 = y0 = z0 = t0 = e0 = 0.;
    x1 = y1 = z1 = t1 = e1 = 0.;
    status = 0; id = 0;
    return;
  }

  x0 = endpointsElectrons[i].x0;
  y0 = endpointsElectrons[i].y0;
  z0 = endpointsElectrons[i].z0;
  t0 = endpointsElectrons[i].t0;
  e0 = endpointsElectrons[i].e0;
  x1 = endpointsElectrons[i].x;
  y1 = endpointsElectrons[i].y;
  z1 = endpointsElectrons[i].z;
  t1 = endpointsElectrons[i].t;
  e1 = endpointsElectrons[i].energy;
  potential0 = endpointsElectrons[i].potential0;
  potential1 = endpointsElectrons[i].potential;
  status = endpointsElectrons[i].status;
  id = endpointsElectrons[i].id;

}

void
AvalancheMicroscopic::GetElectronEndpoint(const int i,
  double& x0, double& y0, double& z0, double& t0, double& e0,
  double& x1, double& y1, double& z1, double& t1, double& e1,
  int& status, int& id, double& potential0, double& potential1, double& xi, double& yi, double& zi, double& mdi0, double& mdi, double& mdimax) const {
  
  if (i < 0 || i >= nElectronEndpoints) {
    std::cerr << className << "::GetElectronEndpoint:\n";
    std::cerr << "    Endpoint index " << i << " out of range.\n";
    x0 = y0 = z0 = t0 = e0 = 0.;
    x1 = y1 = z1 = t1 = e1 = 0.;
    status = 0; id = 0;
    return;
  }
    
  x0 = endpointsElectrons[i].x0;
  y0 = endpointsElectrons[i].y0;
  z0 = endpointsElectrons[i].z0;
  t0 = endpointsElectrons[i].t0;
  e0 = endpointsElectrons[i].e0;
  x1 = endpointsElectrons[i].x;
  y1 = endpointsElectrons[i].y;
  z1 = endpointsElectrons[i].z;
  t1 = endpointsElectrons[i].t;
  e1 = endpointsElectrons[i].energy;
  potential0 = endpointsElectrons[i].potential0;
  potential1 = endpointsElectrons[i].potential;
  status = endpointsElectrons[i].status;
  id = endpointsElectrons[i].id;
  xi = endpointsElectrons[i].xi;
  yi = endpointsElectrons[i].yi;
  zi = endpointsElectrons[i].zi;
  mdi0 = endpointsElectrons[i].mdi0;
  mdi = endpointsElectrons[i].mdi;  
  mdimax = endpointsElectrons[i].mdimax;  
}


void 
AvalancheMicroscopic::GetElectronEndpoint(const int i, 
  double& x0, double& y0, double& z0, double& t0, double& e0,
  double& x1, double& y1, double& z1, double& t1, double& e1,
  double& dx1, double& dy1, double& dz1,
  int& status) const {
  
  if (i < 0 || i >= nElectronEndpoints) {
    std::cerr << className << "::GetElectronEndpoint:\n";
    std::cerr << "    Endpoint index " << i << " out of range.\n";
    x0 = y0 = z0 = t0 = e0 = 0.;
    x1 = y1 = z1 = t1 = e1 = 0.;
    dx1 = dy1 = dz1 = 0.;
    status = 0;
    return;
  }

  x0 = endpointsElectrons[i].x0; 
  y0 = endpointsElectrons[i].y0; 
  z0 = endpointsElectrons[i].z0;
  t0 = endpointsElectrons[i].t0; 
  e0 = endpointsElectrons[i].e0;
  x1 = endpointsElectrons[i].x;  
  y1 = endpointsElectrons[i].y;  
  z1 = endpointsElectrons[i].z;
  t1 = endpointsElectrons[i].t;  
  e1 = endpointsElectrons[i].energy; 
  dx1 = endpointsElectrons[i].kx;
  dy1 = endpointsElectrons[i].ky;
  dz1 = endpointsElectrons[i].kz; 
  status = endpointsElectrons[i].status; 

}

void 
AvalancheMicroscopic::GetHoleEndpoint(const int i, 
  double& x0, double& y0, double& z0, double& t0, double& e0,
  double& x1, double& y1, double& z1, double& t1, double& e1,
  int& status) const {
  
  if (i < 0 || i >= nHoleEndpoints) {
    std::cerr << className << "::GetHoleEndpoint:\n";
    std::cerr << "    Endpoint index " << i << " out of range.\n";
    x0 = y0 = z0 = t0 = e0 = 0.;
    x1 = y1 = z1 = t1 = e1 = 0.;
    status = 0;
    return;
  }

  x0 = endpointsHoles[i].x0; 
  y0 = endpointsHoles[i].y0; 
  z0 = endpointsHoles[i].z0;
  t0 = endpointsHoles[i].t0; 
  e0 = endpointsHoles[i].e0;
  x1 = endpointsHoles[i].x;  
  y1 = endpointsHoles[i].y;  
  z1 = endpointsHoles[i].z;
  t1 = endpointsHoles[i].t;  
  e1 = endpointsHoles[i].energy;  
  status = endpointsHoles[i].status; 

}

int 
AvalancheMicroscopic::GetNumberOfElectronDriftLinePoints(const int i) const {

  if (i < 0 || i >= nElectronEndpoints) {
    std::cerr << className << "::GetNumberOfElectronDriftLinePoints:\n"; 
    std::cerr << "    Endpoint index (" << i << ") out of range.\n";
    return 0;
  }
  
  if (!useDriftLines) return 2;

  return endpointsElectrons[i].driftLine.size() + 2;

}

int 
AvalancheMicroscopic::GetNumberOfHoleDriftLinePoints(const int i) const {

  if (i < 0 || i >= nHoleEndpoints) {
    std::cerr << className << "::GetNumberOfHoleDriftLinePoints:\n"; 
    std::cerr << "    Endpoint index (" << i << ") out of range.\n";
    return 0;
  }
  
  if (!useDriftLines) return 2;

  return endpointsHoles[i].driftLine.size() + 2;

}

void 
AvalancheMicroscopic::GetElectronDriftLinePoint(
  double& x, double& y, double& z, double &t,
  const int ip, const int iel) const {
  
  if (iel < 0 || iel >= nElectronEndpoints) {
    std::cerr << className << "::GetElectronDriftLinePoint:\n";
    std::cerr << "    Endpoint index (" << iel << ") out of range.\n";
    return;
  }

  if (ip <= 0) {
    x = endpointsElectrons[iel].x0; 
    y = endpointsElectrons[iel].y0; 
    z = endpointsElectrons[iel].z0;
    t = endpointsElectrons[iel].t0;
    return;
  }

  const int np = endpointsElectrons[iel].driftLine.size();
  if (ip > np) {
    x = endpointsElectrons[iel].x; 
    y = endpointsElectrons[iel].y; 
    z = endpointsElectrons[iel].z;
    t = endpointsElectrons[iel].t;
    return;
  }

  x = endpointsElectrons[iel].driftLine[ip - 1].x;
  y = endpointsElectrons[iel].driftLine[ip - 1].y;
  z = endpointsElectrons[iel].driftLine[ip - 1].z;
  t = endpointsElectrons[iel].driftLine[ip - 1].t;

}

void 
AvalancheMicroscopic::GetHoleDriftLinePoint(
  double& x, double& y, double& z, double &t,
  const int ip, const int ih) const {
  
  if (ih < 0 || ih >= nHoleEndpoints) {
    std::cerr << className << "::GetHoleDriftLinePoint:\n";
    std::cerr << "    Endpoint index (" << ih << ") out of range.\n";
    return;
  }

  if (ip <= 0) {
    x = endpointsHoles[ih].x0; 
    y = endpointsHoles[ih].y0; 
    z = endpointsHoles[ih].z0;
    t = endpointsHoles[ih].t0;
    return;
  }

  const int np = endpointsHoles[ih].driftLine.size();
  if (ip > np) {
    x = endpointsHoles[ih].x; 
    y = endpointsHoles[ih].y; 
    z = endpointsHoles[ih].z;
    t = endpointsHoles[ih].t;
    return;
  }

  x = endpointsHoles[ih].driftLine[ip - 1].x;
  y = endpointsHoles[ih].driftLine[ip - 1].y;
  z = endpointsHoles[ih].driftLine[ip - 1].z;
  t = endpointsHoles[ih].driftLine[ip - 1].t;

}

void 
AvalancheMicroscopic::GetPhoton(const int i, double& e,
  double& x0, double& y0, double& z0, double& t0,
  double& x1, double& y1, double& z1, double& t1,
  int& status) const {
 
  if (i < 0 || i >= nPhotons) {
    std::cerr << className << "::GetPhoton:\n";
    std::cerr << "    Photon " << i << " does not exist.\n";
    return;
  }

  x0 = photons[i].x0; x1 = photons[i].x1;
  y0 = photons[i].y0; y1 = photons[i].y1;
  z0 = photons[i].z0; z1 = photons[i].z1;
  t0 = photons[i].t0; t1 = photons[i].t1;
  status = photons[i].status;
  e = photons[i].energy;


}

void
AvalancheMicroscopic::SetUserHandleStep(
    void (*f)(double x, double y, double z, double t,
              double e, double dx, double dy, double dz,
              bool hole)) {

  if (!f) {
    std::cerr << className << "::SetUserHandleStep:\n";
    std::cerr << "    Function pointer is a null pointer.\n";
    return;
  }
  userHandleStep = f;
  hasUserHandleStep = true;

}

void
AvalancheMicroscopic::UnsetUserHandleStep() {

  userHandleStep = 0;
  hasUserHandleStep = false;

}

void 
AvalancheMicroscopic::SetUserHandleAttachment(
    void (*f)(double x, double y, double z, double t, 
              int type, int level, Medium* m)) {
         
  userHandleAttachment = f;
  hasUserHandleAttachment = true;
  
}

void 
AvalancheMicroscopic::UnsetUserHandleAttachment() {
  
  userHandleAttachment = 0;
  hasUserHandleAttachment = false;
  
}

void 
AvalancheMicroscopic::SetUserHandleInelastic(
    void (*f)(double x, double y, double z, double t, 
              int type, int level, Medium* m)) {
         
  userHandleInelastic = f;
  hasUserHandleInelastic = true;
  
}

void 
AvalancheMicroscopic::UnsetUserHandleInelastic() {
  
  userHandleInelastic = 0;
  hasUserHandleInelastic = false;
  
}

void 
AvalancheMicroscopic::SetUserHandleIonisation(
    void (*f)(double x, double y, double z, double t, 
              int type, int level, Medium* m)) {
         
  userHandleIonisation = f;
  hasUserHandleIonisation = true;
  
}

void 
AvalancheMicroscopic::UnsetUserHandleIonisation() {
  
  userHandleIonisation = 0;
  hasUserHandleIonisation = false;
  
}

bool 
AvalancheMicroscopic::DriftElectron(
    const double x0, const double y0, const double z0, const double t0, 
    const double e0, const double dx0, const double dy0, const double dz0) {

  // Clear the list of electrons and photons.
  endpointsElectrons.clear(); 
  endpointsHoles.clear();
  photons.clear();

  // Reset the particle counters.
  nPhotons = nElectrons = nHoles = nIons = 0; 
  nElectronEndpoints = nHoleEndpoints = 0;

  return TransportElectron(x0, y0, z0, t0, e0, dx0, dy0, dz0, false, false);

}

bool 
AvalancheMicroscopic::AvalancheElectron(
    const double x0, const double y0, const double z0, const double t0, 
    const double e0, const double dx0, const double dy0, const double dz0) {

  // Clear the list of electrons, holes and photons.
  endpointsElectrons.clear();
  endpointsHoles.clear();
  photons.clear();

  // Reset the particle counters.
  nPhotons = nElectrons = nHoles = nIons = 0; 
  nElectronEndpoints = nHoleEndpoints = 0;

  return TransportElectron(x0, y0, z0, t0, e0, dx0, dy0, dz0, true, false);

}

  // Azriel new function to inititalize a cloud of electrons/ions/excitations for transport
bool 
AvalancheMicroscopic::AvalancheCloud(
    const int nIonization, const double x0[], const double y0[], const double z0[], const double t0[], 
    const double e0[], const double dx0[], const double dy0[], const double dz0[], bool deBroglieRecomb, const double movieframetime[], const int numberofmovieframes) {

  // Clear the list of electrons, holes and photons.
  endpointsElectrons.clear();
  endpointsHoles.clear();
  photons.clear();

  // Reset the particle counters.
  nPhotons = nElectrons = nHoles = nIons = 0; 
  nElectronEndpoints = nHoleEndpoints = 0;

  return TransportCloud(nIonization, x0, y0, z0, t0, e0, dx0, dy0, dz0, true, false, deBroglieRecomb, movieframetime, numberofmovieframes);

}

bool 
AvalancheMicroscopic::TransportElectron(
    const double x0, const double y0, const double z0, const double t0, 
    const double e0, const double dx0, const double dy0, const double dz0,
    const bool aval, bool hole) {
  
  // Make sure that the sensor is defined.
  if (!sensor) {
    std::cerr << className << "::TransportElectron:\n";
    std::cerr << "    Sensor is not defined.\n";
    return false;
  }

  // Make sure that the starting point is inside a medium.
  Medium* medium = 0;
  if (!sensor->GetMedium(x0, y0, z0, medium)) {
    std::cerr << className << "::TransportElectron:\n";
    std::cerr << "    No medium at initial position.\n";
    return false;
  }
  if (medium == 0) {
    std::cerr << className << "::TransportElectron:\n";
    std::cerr << "    No medium at initial position.\n";
    return false;
  }

  // Make sure that the medium is "driftable" and microscopic.
  if (!medium->IsDriftable() || !medium->IsMicroscopic()) {
    std::cerr << className << "::TransportElectron:\n";
    std::cerr << "    Medium at initial position does not provide " 
              << " microscopic tracking data.\n";
    return false;
  }

  // If the medium is a semiconductor, use "band structure" stepping.
  bool useBandStructure = useBandStructureDefault;
  if (medium->IsSemiconductor() && useBandStructureDefault) {
    useBandStructure = true;
  } else {
    useBandStructure = false;
  }
  if (debug) {
    std::cout << className << "::TransportElectron:\n";
    std::cout << "    Starting to drift in medium " 
              << medium->GetName() << ".\n";
  }
  
  // Get the id number of the drift medium.
  int id = medium->GetId();    
  
  // Numerical prefactors in equation of motion
  const double c1 = SpeedOfLight * sqrt(2. / ElectronMass);
  const double c2 = c1 * c1 / 4.;

  // Temporary stack of photons produced in the de-excitation cascade.
  std::vector<double> stackPhotonsTime;   stackPhotonsTime.clear();
  std::vector<double> stackPhotonsEnergy; stackPhotonsEnergy.clear();
    
  // Electric and magnetic field
  double ex = 0., ey = 0., ez = 0., emag = 0.;
  double bx = 0., by = 0., bz = 0., bmag = 0.;
  int status = 0;
  // Cyclotron frequency
  double cwt = 1., swt = 0.;
  double wb = 0.;
  // Flag indicating if magnetic field is usable
  bool bOk = true;

  // Current position, direction, velocity and energy
  double x = x0, y = y0, z = z0, t = t0;
  double kx = dx0, ky = dy0, kz = dz0;
  double vx = dx0, vy = dy0, vz = dz0;
  double energy = e0;
  // Index of the conduction band (irrelevant for gases)
  int band = -1;
  
  // Timestep
  double dt = 0.;
  // Direction, velocity and energy after a step
  double newKx = 0., newKy = 0., newKz = 0.;
  double newVx = 0., newVy = 0., newVz = 0.;
  double newEnergy = 0.;
  // Collision type (elastic, ionisation, attachment, inelastic, ...)
  int cstype;
  // Cross-section term
  int level;

  // Number of secondaries
  int nion = 0, ndxc = 0; 
  
  // Random number
  double r;
  // Numerical factors
  double a1 = 0., a2 = 0., a3 = 0., a4 = 0.;

  // Clear the stack.
  stack.clear();     
  // Add the initial electron to the stack.
  electron newElectron;
  newElectron.status = 0;
  if (hole) {
    newElectron.hole = true;
  } else {
    newElectron.hole = false;
  }
  newElectron.x0 = x0;  newElectron.x = x0;
  newElectron.y0 = y0;  newElectron.y = y0;
  newElectron.z0 = z0;  newElectron.z = z0;
  newElectron.t0 = t0;  newElectron.t  = t0;
  newElectron.kx = dx0; 
  newElectron.ky = dy0; 
  newElectron.kz = dz0;
  newElectron.e0 = std::max(e0, Small); 
  newElectron.energy = newElectron.e0;
  newElectron.band = band;
  // Previous coordinates for distance histogramming.
  newElectron.xLast = x0; 
  newElectron.yLast = y0; 
  newElectron.zLast = z0;
  newElectron.driftLine.clear();
  stack.push_back(newElectron);
  if (hole) {
    ++nHoles;
  } else {
    ++nElectrons;
  }

  if (useBandStructure) {
    // With band structure, (kx, ky, kz) represents the momentum.
    // No normalization in this case.
    medium->GetElectronMomentum(std::max(e0, Small), kx, ky, kz, band);
    stack[0].kx = kx;
    stack[0].ky = ky;
    stack[0].kz = kz;
    stack[0].band = band;
  } else {
    stack[0].band = 0;
    band = 0;
    // Check the given initial direction.
    const double k = sqrt(dx0 * dx0 + dy0 * dy0 + dz0 * dz0);
    if (fabs(k) < Small) {
      // Direction has zero norm, draw a random direction.
      const double phi = TwoPi * RndmUniform();
      const double ctheta = 2 * RndmUniform() - 1.;
      const double stheta = sqrt(1. - ctheta * ctheta);
      stack[0].kx = cos(phi) * stheta;
      stack[0].ky = sin(phi) * stheta;
      stack[0].kz = ctheta;
    } else {
      // Normalise the direction to 1.
      stack[0].kx /= k; 
      stack[0].ky /= k; 
      stack[0].kz /= k;
    }
  }

  // Get the null-collision rate.
  double fLim = medium->GetElectronNullCollisionRate(band);
  if (fLim <= 0.) {
    std::cerr << className << "::TransportElectron:\n";
    std::cerr << "    Got null-collision rate <= 0.\n";
    return false;
  }

  // Status flag
  bool ok = true;
  while (1) {
    // If the list of electrons/holes is exhausted, we're done.
    const int nSize = stack.size();
    if (nSize <= 0) break;
    // Loop over all electrons/holes in the avalanche.
    for (int iE = nSize; iE--;) {
      // Get an electron/hole from the stack.
      x = stack[iE].x; 
      y = stack[iE].y; 
      z = stack[iE].z;
      t = stack[iE].t; 
      energy = stack[iE].energy; 
      band = stack[iE].band; 
      kx = stack[iE].kx; 
      ky = stack[iE].ky; 
      kz = stack[iE].kz;
      hole = stack[iE].hole;

      ok = true;
      
      // Count number of collisions between updates.
      int nCollTemp = 0;

      // Get the local electric field and medium.
      sensor->ElectricField(x, y, z, ex, ey, ez, medium, status);
      // Sign change for electrons.
      if (!hole) {
        ex = -ex; ey = -ey; ez = -ez;
      }
      
      if (debug) {
        std::cout << className << "::TransportElectron:\n";
        if (hole) {
          std::cout << "    Drifting hole " << iE+1 << ".\n";
        } else {
          std::cout << "    Drifting electron " << iE+1 << ".\n";
        }
        std::cout << "    Field [V/cm] at (" 
                  << x  << ", " << y  << ", " << z  << "): "
                  << ex << ", " << ey << ", " << ez << "\n";
        std::cout << "    Status: " << status << "\n";
        std::cout << "    Medium: " << medium->GetName() << "\n";
      }

      if (status != 0) {
        // Electron/hole is not inside a drift medium.
        stack[iE].x = x; 
        stack[iE].y = y; 
        stack[iE].z = z;
        stack[iE].t = t; 
        stack[iE].energy = energy; 
        stack[iE].band = band;
        stack[iE].kx = kx; 
        stack[iE].ky = ky; 
        stack[iE].kz = kz;
        stack[iE].status = StatusLeftDriftMedium;
        if (hole) {
          endpointsHoles.push_back(stack[iE]);
        } else {
          endpointsElectrons.push_back(stack[iE]);
        }
        stack.erase(stack.begin() + iE);
        if (debug) {
          std::cout << className << "::TransportElectron:\n";
          if (hole) {
            std::cout << "    Hole left the drift medium.\n";  
          } else {
            std::cout << "    Electron left the drift medium.\n";
          }
          std::cout << "    At " << x << ", " << y << "," << z << "\n";
        }
        continue;
      }

      // If switched on, get the local magnetic field.
      if (useBfield) {
        sensor->MagneticField(x, y, z, bx, by, bz, status);
        if (hole) {
          bx *=  Tesla2Internal; 
          by *=  Tesla2Internal;
          bz *=  Tesla2Internal;
        } else {
          bx *= -Tesla2Internal; 
          by *= -Tesla2Internal; 
          bz *= -Tesla2Internal;
        }
        // Make sure that neither E nor B are zero.
        bmag = sqrt(bx * bx + by * by + bz * bz);
        emag = sqrt(ex * ex + ey * ey + ez * ez);
        if (bmag > Small && emag > Small) bOk = true;
        else bOk = false;
      }
     
      // Trace the electron/hole. 
      while (1) {

        bool isNullCollision = false;

        // Make sure the electron energy exceeds the transport cut.
        if (energy < deltaCut) {
          stack[iE].x = x; 
          stack[iE].y = y; 
          stack[iE].z = z;
          stack[iE].t = t; 
          stack[iE].energy = energy; 
          stack[iE].band = band;
          stack[iE].kx = kx; 
          stack[iE].ky = ky; 
          stack[iE].kz = kz;
          stack[iE].status = StatusBelowTransportCut;
          if (hole) {
            endpointsHoles.push_back(stack[iE]);
          } else {
            endpointsElectrons.push_back(stack[iE]);
          }
          stack.erase(stack.begin() + iE);
          if (debug) {
            std::cout << className << "::TransportElectron:\n";
            std::cout << "    Kinetic energy (" << energy << ")"
                      << " below transport cut.\n";
          }
          ok = false;
          break;
        }
        
        // Fill the energy distribution histogram.
        if (hole && hasHoleEnergyHistogram) {
          histHoleEnergy->Fill(energy);
        } else if (!hole && hasElectronEnergyHistogram) {
          histElectronEnergy->Fill(energy);
        }

        // Check if the electrons is within the specified time window.
        if (hasTimeWindow && (t < tMin || t > tMax)) {
          stack[iE].x = x; 
          stack[iE].y = y; 
          stack[iE].z = z;
          stack[iE].t = t; 
          stack[iE].energy = energy; 
          stack[iE].band = band;
          stack[iE].kx = kx; 
          stack[iE].ky = ky; 
          stack[iE].kz = kz;
          stack[iE].status = StatusOutsideTimeWindow;
          if (hole) {
            endpointsHoles.push_back(stack[iE]);
          } else {
            endpointsElectrons.push_back(stack[iE]);
          }
          stack.erase(stack.begin() + iE);
          if (debug) {
            std::cout << className << "::TransportElectron:\n";
            if (hole) {
              std::cout << "    Hole left the time window.\n";  
            } else {
              std::cout << "    Electron left the time window.\n";
            }
            std::cout << "    Time: " << t << "\n";
          }
          ok = false;
          break;
        }
       
        if (medium->GetId() != id) {
          // Medium has changed.
          if (!medium->IsMicroscopic()) {
            // Electron/hole has left the microscopic drift medium.
            stack[iE].x = x; 
            stack[iE].y = y; 
            stack[iE].z = z;
            stack[iE].t = t; 
            stack[iE].energy = energy;
            stack[iE].band = band;
            stack[iE].kx = kx; 
            stack[iE].ky = ky; 
            stack[iE].kz = kz;
            stack[iE].status = StatusLeftDriftMedium;
            if (hole) {
              endpointsHoles.push_back(stack[iE]);
            } else {
              endpointsElectrons.push_back(stack[iE]);
            }
            stack.erase(stack.begin() + iE);
            ok = false;
            if (debug) {
              std::cout << className << "::TransportElectron:\n";
              std::cout << "    Medium at " << x << ", " << y << ", " << z 
                        << " does not have microscopic data.\n";
            }
            break;
          }
          id = medium->GetId();
          if (medium->IsSemiconductor() && useBandStructureDefault) {
            useBandStructure = true;
          } else {
            useBandStructure = false;
          }
          // Update the null-collision rate.
          fLim = medium->GetElectronNullCollisionRate(band);
          if (fLim <= 0.) {
            std::cerr << className << "::TransportElectron:\n"; 
            std::cerr << "    Got null-collision rate <= 0.\n";
            return false;
          }          
        }

        if (useBfield && bOk) {
          // Calculate the cyclotron frequency.
          wb = OmegaCyclotronOverB * bmag;
          // Rotate the direction vector into the local coordinate system.
          ComputeRotationMatrix(bx, by, bz, bmag, ex, ey, ez);
          RotateGlobal2Local(kx, ky, kz);
          // Calculate the electric field in the rotated system.
          RotateGlobal2Local(ex, ey, ez);
          // Calculate the velocity vector in the local frame.
          const double v = c1 * sqrt(energy);
          vx = v * kx; vy = v * ky; vz = v * kz;
          a1 = vx * ex;
          a2 = c2 * ex * ex;
          a3 = ez / bmag - vy;
          a4 = (ez / wb); 
        } else if (useBandStructure) {
          energy = medium->GetElectronEnergy(kx, ky, kz, vx, vy, vz, band);
        } else {
          // No band structure, no magnetic field.
          // Calculate the velocity vector.
          const double v = c1 * sqrt(energy);
          vx = v * kx; vy = v * ky; vz = v * kz;
          
          a1 = vx * ex + vy * ey + vz * ez;
          a2 = c2 * (ex * ex + ey * ey + ez * ez);
        }

        if (hasUserHandleStep) {
          userHandleStep(x, y, z, t, energy, kx, ky, kz, hole);
        }
  
        // Determine the timestep.
        dt = 0.;
        while (1) {
          // Sample the flight time.
          r = RndmUniformPos();
          dt += - log(r) / fLim;
          // Calculate the energy after the proposed step.
          if (useBfield && bOk) {
            cwt = cos(wb * dt); swt = sin(wb * dt);
            newEnergy = std::max(energy + (a1 + a2 * dt) * dt + 
                                 a4 * (a3 * (1. - cwt) + vz * swt), 
                                 Small);
          } else if (useBandStructure) {
            newEnergy = std::max(medium->GetElectronEnergy(
                                              kx + ex * dt * SpeedOfLight,
                                              ky + ey * dt * SpeedOfLight,
                                              kz + ez * dt * SpeedOfLight, 
                                              newVx, newVy, newVz, band), 
                                 Small);
          } else {
            newEnergy = std::max(energy + (a1 + a2 * dt) * dt, Small);
          }
          // Get the real collision rate at the updated energy.
          double fReal = medium->GetElectronCollisionRate(newEnergy, band);
          if (fReal <= 0.) {
            std::cerr << className << "::TransportElectron:\n";
            std::cerr << "    Got collision rate <= 0.\n";
            std::cerr << "    At " << newEnergy << " eV (band " << band << ").\n";
            return false;
          }
          if (fReal > fLim) {
            // Real collision rate is higher than null-collision rate.
            dt += log(r) / fLim;
            // Increase the null collision rate and try again.
            std::cerr << className << "::TransportElectron:\n";
            std::cerr << "    Increasing null-collision rate by 5%.\n"; 
            if (useBandStructure) std::cerr << "    Band " << band << "\n";
            fLim *= 1.05;
            continue;
          }

          // Check for real or null collision.
          if (RndmUniform() <= fReal / fLim) break;
          if (useNullCollisionSteps) {
            isNullCollision = true;
            break;
          }
        }
        if (!ok) break;

        // Increase the collision counter.
        ++nCollTemp;

        // Update the directions (at instant before collision)
        // and calculate the proposed new position.
        if (useBfield && bOk) {
          // Calculate the new velocity.
          newVx = vx + 2. * c2 * ex * dt;
          newVy = vz * swt - a3 * cwt + ez / bmag;
          newVz = vz * cwt + a3 * swt;
          // Normalise and rotate back to the lab frame.
          const double v = sqrt(newVx * newVx + newVy * newVy + 
                                newVz * newVz);
          newKx = newVx / v; newKy = newVy / v; newKz = newVz / v; 
          RotateLocal2Global(newKx, newKy, newKz);
          // Calculate the step in coordinate space.
          vx += c2 * ex * dt;
          ky = (vz * (1. - cwt) - a3 * swt) / (wb * dt) + ez / bmag;
          kz = (vz * swt + a3 * (1. - cwt)) / (wb * dt); 
          vy = ky; vz = kz;
          // Rotate back to the lab frame.
          RotateLocal2Global(vx, vy, vz);
        } else if (useBandStructure) {
          // Update the wave-vector.
          newKx = kx + ex * dt * SpeedOfLight;
          newKy = ky + ey * dt * SpeedOfLight;
          newKz = kz + ez * dt * SpeedOfLight;
          // Average velocity over the step.
          vx = 0.5 * (vx + newVx);
          vy = 0.5 * (vy + newVy);
          vz = 0.5 * (vz + newVz);
        } else {
          // Update the direction.
          a1 = sqrt(energy / newEnergy);
          a2 = 0.5 * c1 * dt / sqrt(newEnergy);
          newKx = kx * a1 + ex * a2; 
          newKy = ky * a1 + ey * a2; 
          newKz = kz * a1 + ez * a2;

          // Calculate the step in coordinate space.
          a1 = c1 * sqrt(energy);
          a2 = dt * c2; 
          vx = kx * a1 + ex * a2;
          vy = ky * a1 + ey * a2;
          vz = kz * a1 + ez * a2;
        }
 
        // Get the electric field and medium at the proposed new position.
        sensor->ElectricField(x + vx * dt, y + vy * dt, z + vz * dt, 
                              ex, ey, ez, medium, status);
        if (!hole) {
          ex = -ex; ey = -ey; ez = -ez;
        }
        
        // Check if the electron is still inside a drift medium.
        if (status != 0) {
          // Try to terminate the drift line close to the boundary
          // by means of iterative bisection.
          stack[iE].x = x; 
          stack[iE].y = y; 
          stack[iE].z = z;
          stack[iE].t = t; 
          stack[iE].energy = energy;
          double dx = vx * dt, dy = vy * dt, dz = vz * dt;
          double d = sqrt(dx * dx + dy * dy + dz * dz);
          if (d > 0) {
            dx /= d; dy /= d; dz /= d;
          }
          // Mid-point
          double xM = x, yM = y, zM = z;
          while (d > BoundaryDistance) {
            d *= 0.5;
            dt *= 0.5;
            xM = x + d * dx; yM = y + d * dy; zM = z + d * dz; 
            // Check if the mid-point is inside the drift medium.
            sensor->ElectricField(xM, yM, zM, ex, ey, ez, medium, status);
            if (status == 0) {
              x = xM; y = yM; z = zM; t += dt;
            } 
          }
          // Place the endpoint OUTSIDE the drift medium
          x += d * dx; y += d * dy; z += d * dz; 
          if (useSignal) {
            if (hole) {
              sensor->AddSignal(+1, stack[iE].t, 
                                t - stack[iE].t, 
                                0.5 * (x + stack[iE].x), 
                                0.5 * (y + stack[iE].y),
                                0.5 * (z + stack[iE].z), 
                                vx, vy, vz);
            } else {
              sensor->AddSignal(-1, stack[iE].t,
                                t - stack[iE].t,
                                0.5 * (x + stack[iE].x),
                                0.5 * (y + stack[iE].y),
                                0.5 * (z + stack[iE].z),
                                vx, vy, vz);
            }
          }
          stack[iE].x = x; 
          stack[iE].y = y; 
          stack[iE].z = z;
          stack[iE].t = t;
          stack[iE].kx = newKx; 
          stack[iE].ky = newKy; 
          stack[iE].kz = newKz;
          stack[iE].status = StatusLeftDriftMedium;
          if (hole) {
            endpointsHoles.push_back(stack[iE]);
          } else {
            endpointsElectrons.push_back(stack[iE]);
          }
          stack.erase(stack.begin() + iE);
          ok = false;
          if (debug) {
            std::cout << className << "::TransportElectron:\n";
            if (hole) {
              std::cout << "    Hole left the drift medium.\n";
            } else {
              std::cout << "    Electron left the drift medium.\n";
            }
            std::cout << "    At " << x << ", " << y << "," << z << "\n";
          }
          break;
        }

        // Check if the new position is inside the user area.
        if (!sensor->IsInArea(x + vx * dt, y + vy * dt, z + vz * dt)) {
          // Try to terminate the drift line close to the boundary
          // by means of iterative bisection.
          stack[iE].x = x; 
          stack[iE].y = y; 
          stack[iE].z = z;
          stack[iE].t = t; 
          stack[iE].energy = energy;
          double dx = vx * dt, dy = vy * dt, dz = vz * dt;
          double d = sqrt(dx * dx + dy * dy + dz * dz);
          if (d > 0) {
            dx /= d; dy /= d; dz /= d;
          }
          // Mid-point
          double xM = x, yM = y, zM = z;
          while (d > BoundaryDistance) {
            d *= 0.5;
            dt *= 0.5;
            xM = x + d * dx; yM = y + d * dy; zM = z + d * dz; 
            // Check if the mid-point is inside the drift area.
            if (sensor->IsInArea(xM, yM, zM)) {
              x = xM; y = yM; z = zM; t += dt;
            }
          }
          // Place the endpoint OUTSIDE the drift area.
          x += d * dx; y += d * dy; z += d * dz;

          // If switched on, calculate the induced signal over this step.
          if (useSignal) {
            if (hole) {
              sensor->AddSignal(+1, stack[iE].t,
                                t - stack[iE].t,
                                0.5 * (x + stack[iE].x),
                                0.5 * (y + stack[iE].y),
                                0.5 * (z + stack[iE].z),
                                vx, vy, vz);
            } else {
              sensor->AddSignal(-1, stack[iE].t, 
                                t - stack[iE].t, 
                                0.5 * (x + stack[iE].x), 
                                0.5 * (y + stack[iE].y),
                                0.5 * (z + stack[iE].z), 
                                vx, vy, vz);
            }
          }
          stack[iE].x = x; 
          stack[iE].y = y; 
          stack[iE].z = z;
          stack[iE].t = t;
          stack[iE].kx = newKx; 
          stack[iE].ky = newKy; 
          stack[iE].kz = newKz;
          stack[iE].status = StatusLeftDriftArea;
          if (hole) {
            endpointsHoles.push_back(stack[iE]);
          } else {
            endpointsElectrons.push_back(stack[iE]);
          }
          stack.erase(stack.begin() + iE);
          ok = false;
          if (debug) {
            std::cout << className << "::TransportElectron:\n";
            if (hole) {
              std::cout << "    Hole left the drift area.\n";
            } else {
              std::cout << "    Electron left the drift area.\n";
            }
            std::cout << "    At " << x << ", " << y << ", " << z << "\n";
          }
          break;
        }

        // Check if the electron/hole has crossed a wire.
        double xCross = x, yCross = y, zCross = z;
        if (sensor->IsWireCrossed(x, y, z, 
                                  x + vx * dt, y + vy * dt, z + vz * dt,
                                  xCross, yCross, zCross)) {
          // If switched on, calculated the induced signal over this step.
          if (useSignal) {
            dt = sqrt(pow(xCross - x, 2) + 
                      pow(yCross - y, 2) + 
                      pow(zCross - z, 2)) / 
                 sqrt(vx * vx + vy * vy + vz * vz); 
            if (hole) {
              sensor->AddSignal(+1, t, dt, 0.5 * (x + xCross),
                                           0.5 * (y + yCross),
                                           0.5 * (z + zCross), vx, vy, vz);
            } else {
              sensor->AddSignal(-1, t, dt, 0.5 * (x + xCross),
                                           0.5 * (y + yCross),
                                           0.5 * (z + zCross), vx, vy, vz);
            }
          }
          stack[iE].x = xCross; 
          stack[iE].y = yCross; 
          stack[iE].z = zCross;
          stack[iE].t = t + dt;
          stack[iE].kx = newKx; 
          stack[iE].ky = newKy; 
          stack[iE].kz = newKz;
          stack[iE].status = StatusLeftDriftMedium;
          if (hole) {
            endpointsHoles.push_back(stack[iE]);
          } else {
            endpointsElectrons.push_back(stack[iE]);
          }
          stack.erase(stack.begin() + iE);
          ok = false;
          if (debug) {
            std::cout << className << "::TransportElectron:\n";
            std::cout << "    Electron/hole hit a wire.\n";
            std::cout << "    At " << x << ", " << y << "," << z << "\n";
          }
          break;
        }
        
        // If switched on, calculate the induced signal.
        if (useSignal) {
          if (hole) {
            sensor->AddSignal(+1, t, dt, x + 0.5 * vx * dt,
                                         y + 0.5 * vy * dt,
                                         z + 0.5 * vz * dt, vx, vy, vz);
          } else {
            sensor->AddSignal(-1, t, dt, x + 0.5 * vx * dt, 
                                         y + 0.5 * vy * dt,
                                         z + 0.5 * vy * dt, vx, vy, vz);
          }
        }

        // Update the coordinates.
        x += vx * dt; y += vy * dt; z += vz * dt; t += dt;

        // If switched on, get the magnetic field at the new location.
        if (useBfield) {
          sensor->MagneticField(x, y, z, bx, by, bz, status);
          if (hole) {
            bx *=  Tesla2Internal;
            by *=  Tesla2Internal;
            bz *=  Tesla2Internal;
          } else {
            bx *= -Tesla2Internal; 
            by *= -Tesla2Internal; 
            bz *= -Tesla2Internal;
          }
          // Make sure that neither E nor B are zero.
          bmag = sqrt(bx * bx + by * by + bz * bz);
          emag = sqrt(ex * ex + ey * ey + ez * ez);
          if (bmag > Small && emag > Small) bOk = true;
          else bOk = false;
        }

        if (isNullCollision) {
          energy = newEnergy;
          kx = newKx; ky = newKy; kz = newKz;
          continue;
        }
        
        // Get the collision type and parameters.
        medium->GetElectronCollision(newEnergy, cstype, level, 
                                     energy, newKx, newKy, newKz, 
                                     nion, ndxc, band);

        // If activated, histogram the distance with respect to the
        // last collision.
        if (hasDistanceHistogram && histDistance != 0 &&
            nDistanceHistogramTypes > 0) {
          for (int iType = nDistanceHistogramTypes; iType--;) {
            if (distanceHistogramType[iType] != cstype) continue;
            if (debug) {
              std::cout << className << "::TransportElectron:\n";
              std::cout << "    Collision type: " << cstype << "\n";
              std::cout << "    Fill distance histogram.\n";
              getchar();
            }
            switch (distanceOption) {
              case 'x':
                histDistance->Fill(stack[iE].xLast - x);
                break;
              case 'y':
                histDistance->Fill(stack[iE].yLast - y);
                break;
              case 'z':
                histDistance->Fill(stack[iE].zLast - z);
                break;
              case 'r':
                const double r2 = pow(stack[iE].xLast - x, 2) + 
                                  pow(stack[iE].yLast - y, 2) + 
                                  pow(stack[iE].zLast - z, 2);
                histDistance->Fill(sqrt(r2));
                break;
            }
            stack[iE].xLast = x; 
            stack[iE].yLast = y; 
            stack[iE].zLast = z;
            break;  
          }
        }

        switch (cstype) {
          // Elastic collision
          case ElectronCollisionTypeElastic:
            break;
          // Ionising collision
          case ElectronCollisionTypeIonisation:
            if (usePlotting && plotIonisations) {
              viewer->AddIonisationMarker(x, y, z);
            }
            if (hasUserHandleIonisation) {
              userHandleIonisation(x, y, z, t, cstype, level, medium);
            }
            for (int j = nion; j--;) {
              int itype;
              double esec;
              medium->GetIonisationProduct(j, itype, esec);
              if (itype == IonProdTypeElectron) {
                esec = std::max(esec, Small);
                if (hasSecondaryHistogram) histSecondary->Fill(esec);
                // Add the secondary electron to the stack.
                newElectron = stack[iE];
                newElectron.hole = false;
                newElectron.x0 = x; newElectron.x = x;
                newElectron.y0 = y; newElectron.y = y;
                newElectron.z0 = z; newElectron.z = z;
                newElectron.t0 = t; newElectron.t = t;
                newElectron.energy = esec;
                newElectron.e0 = newElectron.energy;
                if (useBandStructure) {
                  newElectron.band = -1;
                  medium->GetElectronMomentum(esec, 
                                              newElectron.kx, 
                                              newElectron.ky,
                                              newElectron.kz, 
                                              newElectron.band);
                } else {
                  // Randomise the secondary electron direction.
                  const double phi = TwoPi * RndmUniform();
                  const double ctheta = 2 * RndmUniform() - 1.;
                  const double stheta = sqrt(1. - ctheta * ctheta);
                  newElectron.kx = cos(phi) * stheta;
                  newElectron.ky = sin(phi) * stheta;
                  newElectron.kz = ctheta;
                }
                newElectron.status = 0;
                newElectron.driftLine.clear();
                if (aval && (sizeCut <= 0 || (int)stack.size() < sizeCut)) {
                  stack.push_back(newElectron);
                }
                // Increment the electron counter.
                ++nElectrons;
              } else if (itype == IonProdTypeHole) {
                esec = std::max(esec, Small);
                // Add the secondary hole to the stack.
                newElectron = stack[iE];
                newElectron.hole = true;
                newElectron.x0 = x; newElectron.x = x;
                newElectron.y0 = y; newElectron.y = y;
                newElectron.z0 = z; newElectron.z = z;
                newElectron.t0 = t; newElectron.t = t;
                newElectron.energy = esec;
                newElectron.e0 = newElectron.energy;
                if (useBandStructure) {
                  newElectron.band = -1;
                  medium->GetElectronMomentum(esec, 
                                              newElectron.kx, 
                                              newElectron.ky,
                                              newElectron.kz, 
                                              newElectron.band);
                } else {
                  // Randomise the secondary hole direction.
                  const double phi = TwoPi * RndmUniform();
                  const double ctheta = 2 * RndmUniform() - 1.;
                  const double stheta = sqrt(1. - ctheta * ctheta);
                  newElectron.kx = cos(phi) * stheta;
                  newElectron.ky = sin(phi) * stheta;
                  newElectron.kz = ctheta;
                }
                newElectron.status = 0;
                newElectron.driftLine.clear();
                if (aval && (sizeCut <= 0 || (int)stack.size() < sizeCut)) {
                  stack.push_back(newElectron);
                }
                // Increment the hole counter.
                ++nHoles;
              } else if (itype == IonProdTypeIon) {
                ++nIons;
              }
            }
            if (debug) {
              std::cout << className << "::TransportElectron:\n";
              std::cout << "    Ionisation.\n";
              std::cout << "    At " << x << "," << y << "," << z << "\n"; 
            }
            break;
          // Attachment
          case ElectronCollisionTypeAttachment:
            if (usePlotting && plotAttachments) {
              viewer->AddAttachmentMarker(x, y, z);
            }
            if (hasUserHandleAttachment) {
              userHandleAttachment(x, y, z, t, cstype, level, medium);
            }
            stack[iE].x = x; 
            stack[iE].y = y; 
            stack[iE].z = z;
            stack[iE].t = t; 
            stack[iE].energy = energy;
            stack[iE].status = StatusAttached;
            if (hole) {
              endpointsHoles.push_back(stack[iE]);
              --nHoles;
            } else {
              endpointsElectrons.push_back(stack[iE]);
              --nElectrons;
            }
            stack.erase(stack.begin() + iE);
            ok = false;
            break;
          // Inelastic collision
          case ElectronCollisionTypeInelastic:
            if (hasUserHandleInelastic) {
              userHandleInelastic(x, y, z, t, cstype, level, medium);
            }
            break;
          // Excitation
          case ElectronCollisionTypeExcitation:
            if (usePlotting && plotExcitations) {
              viewer->AddExcitationMarker(x, y, z);
            }
            if (hasUserHandleInelastic) {
              userHandleInelastic(x, y, z, t, cstype, level, medium);
            }
            if (ndxc > 0) {
              // Get the electrons and photons produced in the 
              // deexcitation cascade.
              double tDxc = 0., sDxc = 0., eDxc = 0.;
              int typeDxc = 0;
              stackPhotonsTime.clear(); stackPhotonsEnergy.clear();
              for (int j = ndxc; j--;) {
                if (!medium->GetDeexcitationProduct(j, tDxc, sDxc,
                                                    typeDxc, eDxc)) {
                  std::cerr << className << "::TransportElectron:\n";
                  std::cerr << "    Cannot retrieve deexcitation product "
                            << j << "/" << ndxc << ".\n";
                  break;
                }
                
                if (typeDxc == DxcProdTypeElectron) {
                  if (!aval || (sizeCut > 0 && (int)stack.size() >= sizeCut)) continue;
                  // Penning ionisation
                  newElectron = stack[iE];
                  double xDxc = x, yDxc = y, zDxc = z;
                  if (sDxc > Small) {
                    // Randomise the point of creation
                    double phiDxc = TwoPi * RndmUniform();
                    double cthetaDxc = 1. - 2 * RndmUniform();
                    double sthetaDxc = sqrt(1. - cthetaDxc * cthetaDxc);
                    xDxc += sDxc * cos(phiDxc) * sthetaDxc;
                    yDxc += sDxc * sin(phiDxc) * sthetaDxc;
                    zDxc += sDxc * cthetaDxc;
                  }
                  // Get the electric field and medium at this location.
                  Medium* dxcMedium = 0;
                  double fx = 0., fy = 0., fz = 0.;
                  sensor->ElectricField(xDxc, yDxc, zDxc, 
                                        fx, fy, fz, dxcMedium, status);
                  // Check if this location is inside a drift medium.
                  if (status != 0) continue;
                  // Check if this location is inside the drift area.
                  if (!sensor->IsInArea(xDxc, yDxc, zDxc)) continue;
                  // Make sure we haven't jumped across a wire.
                  double xCross, yCross, zCross;
                  if (sensor->IsWireCrossed(x, y, z, xDxc, yDxc, zDxc, 
                                            xCross, yCross, zCross)) {
                    continue;
                  } 
                  newElectron.x0 = xDxc; newElectron.x = xDxc;
                  newElectron.y0 = yDxc; newElectron.y = yDxc;
                  newElectron.z0 = zDxc; newElectron.z = zDxc;
                  newElectron.t0 = t + tDxc; newElectron.t = t + tDxc;
                  newElectron.energy = std::max(eDxc, Small);
                  newElectron.e0 = newElectron.energy;
                  // Randomise the initial direction.
                  const double phi = TwoPi * RndmUniform();
                  const double ctheta = 2 * RndmUniform() - 1.;
                  const double stheta = sqrt(1. - ctheta * ctheta);
                  newElectron.kx = cos(phi) * stheta;
                  newElectron.ky = sin(phi) * stheta;
                  newElectron.kz = ctheta;
                  newElectron.status = 0;
                  newElectron.driftLine.clear();
                  // Add the electron to the list.
                  stack.push_back(newElectron);
                  // Increment the electron and ion counters.
                  ++nElectrons; ++nIons;
                } else if (typeDxc == DxcProdTypePhoton && usePhotons && 
                           eDxc > gammaCut) {
                  // Radiative de-excitation
                  stackPhotonsTime.push_back(t + tDxc);
                  stackPhotonsEnergy.push_back(eDxc);
                }
              }
                
              // Transport the photons (if any)
              const int nSizePhotons = stackPhotonsTime.size();
              for (int j = nSizePhotons; j--;) {
                if (aval) {
                  TransportPhoton(x, y, z, 
                                  stackPhotonsTime[j], 
                                  stackPhotonsEnergy[j]);
                }
              }
            }
            break;
          // Super-elastic collision
          case ElectronCollisionTypeSuperelastic:
            break;
          // Acoustic phonon scattering (intravalley)
          case ElectronCollisionTypeAcousticPhonon:
            break;
          // Optical phonon scattering (intravalley)
          case ElectronCollisionTypeOpticalPhonon:
            break;
          // Intervalley scattering (phonon assisted)
          case ElectronCollisionTypeIntervalleyG:
          case ElectronCollisionTypeIntervalleyF:
          case ElectronCollisionTypeInterbandXL:
          case ElectronCollisionTypeInterbandXG:
          case ElectronCollisionTypeInterbandLG:
            break;
          // Coulomb scattering
          case ElectronCollisionTypeImpurity:
            break;
          default:
            std::cerr << className << "::TransportElectron:\n"; 
            std::cerr << "    Unknown collision type.\n";
            ok = false;
            break;
        }

        // Continue with the next electron/hole?
        if (!ok || 
            nCollTemp > nCollSkip || 
            cstype == ElectronCollisionTypeIonisation || 
            (plotExcitations && cstype == ElectronCollisionTypeExcitation) ||
            (plotAttachments && cstype == ElectronCollisionTypeAttachment)) {
          break;
        }
        kx = newKx; ky = newKy; kz = newKz;

      }
      
      if (!ok) continue;
      
      if (!useBandStructure) {
        // Normalise the direction vector.
        const double k = sqrt(kx * kx + ky * ky + kz * kz);
        kx /= k; ky /= k; kz /= k;
      }
      // Update the stack.
      stack[iE].energy = energy; 
      stack[iE].t = t;
      stack[iE].x = x; 
      stack[iE].y = y; 
      stack[iE].z = z;
      stack[iE].kx = kx; 
      stack[iE].ky = ky; 
      stack[iE].kz = kz;
      // Add a new point to the drift line (if enabled).
      if (useDriftLines) {
        point newPoint;
        newPoint.x = x; newPoint.y = y; newPoint.z = z; newPoint.t = t;
        stack[iE].driftLine.push_back(newPoint);
      }
    }
  }
  nElectronEndpoints = endpointsElectrons.size();
  nHoleEndpoints     = endpointsHoles.size();

  // Calculate the induced charge.
  if (useInducedCharge) {
    for (int i = nElectronEndpoints; i--;) {
      sensor->AddInducedCharge(-1, 
                               endpointsElectrons[i].x0, 
                               endpointsElectrons[i].y0, 
                               endpointsElectrons[i].z0,
                               endpointsElectrons[i].x,  
                               endpointsElectrons[i].y,  
                               endpointsElectrons[i].z);
    }
    for (int i = nHoleEndpoints; i--;) {
      sensor->AddInducedCharge(+1,
                               endpointsHoles[i].x0,
                               endpointsHoles[i].y0,
                               endpointsHoles[i].z0,
                               endpointsHoles[i].x,
                               endpointsHoles[i].y,
                               endpointsHoles[i].z);
    }
  }

  // Plot the drift paths and photon tracks.
  if (usePlotting) {
    // Electrons
    for (int i = nElectronEndpoints; i--;) {
      const int np = GetNumberOfElectronDriftLinePoints(i);
      int jL;
      if (np <= 0) continue;
      viewer->NewElectronDriftLine(np, jL, 
                                   endpointsElectrons[i].x0, endpointsElectrons[i].y0, 
                                   endpointsElectrons[i].z0);
      for (int jP = np; jP--;) {
        GetElectronDriftLinePoint(x, y, z, t, jP, i);
        viewer->SetDriftLinePoint(jL, jP, x, y, z);
      }
    }
    // Holes
    for (int i = nHoleEndpoints; i--;) {
      const int np = GetNumberOfHoleDriftLinePoints(i);
      int jL;
      if (np <= 0) continue;
      viewer->NewHoleDriftLine(np, jL, 
                               endpointsHoles[i].x0, endpointsHoles[i].y0, 
                               endpointsHoles[i].z0);
      for (int jP = np; jP--;) {
        GetHoleDriftLinePoint(x, y, z, t, jP, i);
        viewer->SetDriftLinePoint(jL, jP, x, y, z);
      }
    }
    // Photons
    for (int i = nPhotons; i--;) {
      viewer->NewPhotonTrack(photons[i].x0, photons[i].y0, photons[i].z0,
                             photons[i].x1, photons[i].y1, photons[i].z1);
    } 
  }
  return true;
    
}

  // Azriel new function (based on TransportElectron) to transport the cloud of electrons ions and excitations
bool AvalancheMicroscopic::TransportCloud(
    const int nIonization, const double x0[], const double y0[], const double z0[], const double t0[], const double e0[], const double dx0[], const double dy0[], const double dz0[],
    const bool aval, bool hole, bool deBroglieRecomb, const double movieframetime[], const int numberofmovieframes) {

//  std::cout << "deBroglieRecomb: " << deBroglieRecomb << std::endl;
 
  // megan: introduce variable to keep track of total number of electrons (including ones that already recombined or attached)
  int nIonizationTotal = nIonization;

  // enable use of null collision steps
  EnableNullCollisionSteps();
  std::cout << std::endl << "Null Collision Steps: " ;
  if (useNullCollisionSteps) std::cout << "on" << std::endl;
  else std::cout << "off" << std::endl;

  // Make sure that the sensor is defined.
  if (!sensor) {
    std::cerr << className << "::TransportCloud:\n";
    std::cerr << "    Sensor is not defined.\n";
    return false;
  }
  // Numerical prefactors in equation of motion
  const double c1 = SpeedOfLight * sqrt(2. / ElectronMass);
  const double c2 = c1 * c1 / 4.;
  
  // Temporary stack of photons produced in the de-excitation cascade.
  std::vector<double> stackPhotonsTime;   stackPhotonsTime.clear();
  std::vector<double> stackPhotonsEnergy; stackPhotonsEnergy.clear();
  
  // Electric and magnetic field
  double ex = 0., ey = 0., ez = 0., emag = 0.;
  double bx = 0., by = 0., bz = 0., bmag = 0.;
  int status = 0;
  // Cyclotron frequency
  double cwt = 1., swt = 0.;
  double wb = 0.;
  // Flag indicating if magnetic field is usable
  bool bOk = true;
  
  // Index of the conduction band (irrelevant for gases)
  int band = -1;
  
  // Timestep
  double dt = 0.;
  // Direction, velocity and energy after a step
  double newKx = 0., newKy = 0., newKz = 0.;
  double newVx = 0., newVy = 0., newVz = 0.;
  double newEnergy = 0.;
  // Collision type (elastic, ionisation, attachment, inelastic, ...)
  int cstype;
  // Cross-section term
  int level;
  
  // Number of secondaries
  int nion = 0, ndxc = 0; 
  
  // Random number
  double r;
  // Numerical factors
  double a1 = 0., a2 = 0., a3 = 0., a4 = 0.;
  
  // Clear the stack.
  stack.clear();     
 
  // Make sure that the starting point of each ionization electron is inside a medium.
  Medium* medium;
  double OnsagerRadius = 0.0;
  double DielectricConst = 1.0;
  int id = 0;
  bool useBandStructure =  false;
  double fLim = 0.;
  double finer_tracking_factor = 10.;
  std::cout << "finer_tracking_factor = " << finer_tracking_factor << std::endl << std::endl;
  double vx, vy, vz;
  double kx, ky, kz;
  electron newElectron;
  bool first_electron = true;

  // megan: factor to multiply OnsagerRadius that determines distance at which electrons are released
  double onsagerFactor = 0.1;
  std::cout << "Released " << onsagerFactor << " times the Onsager radius away from the ion." << std::endl;

  long int counter_steps = 0;

  // enters each electron-ion pair in cloud into stack and computes initial direction
  // uses stack.push_back to enter at the end
  for (int ionization = 0; ionization < nIonization; ionization++) {
    medium = 0;
    bool medium_ok = true;

    if (!sensor->GetMedium(x0[ionization], y0[ionization], z0[ionization], medium)) {
      std::cerr << className << "::TransportCloud:\n";
      std::cerr << "    No medium at initial position of ioniztion: " << ionization << "\n";
      medium_ok = false;
    }
    if (medium == 0) {
      std::cerr << className << "::TransportCloud:\n";
      std::cerr << "    No medium at initial positions of ionization: " << ionization << "\n";
      medium_ok = false;
    }
    
    // Make sure that the medium is "driftable" and microscopic.
    if (!medium->IsDriftable() || !medium->IsMicroscopic()) {
      std::cerr << className << "::TransportCloud:\n";
      std::cerr << "    Medium at initial position of ionization: " << ionization << " does not provide" 
		<< " microscopic tracking data.\n";
      medium_ok = false;
    }
    
    // If the medium is a semiconductor, use "band structure" stepping.
    useBandStructure = useBandStructureDefault;
    if (medium->IsSemiconductor() && useBandStructureDefault) {
      useBandStructure = true;
    } else {
      useBandStructure = false;
    }

    if (debug) {
      std::cout << className << "::TransportCloud:\n";
      std::cout << "    Starting to drift ionization " << ionization << " in medium " 
		<< medium->GetName() << ".\n";
    }
    
    // Get the id number of the drift medium.
    id = medium->GetId();    

    double thedx0, thedy0, thedz0;
    thedx0 = dx0[ionization];
    thedy0 = dy0[ionization]; 
    thedz0 = dz0[ionization];
  
    if (medium_ok) {

      // if no initial direction given to electron choose one at random
      const double k = sqrt(thedx0 * thedx0 + thedy0 * thedy0 + thedz0 * thedz0);
      if (fabs(k) < Small) {
	// Direction has zero norm, draw a random direction.
	const double phi = TwoPi * RndmUniform();
	const double ctheta = 2 * RndmUniform() - 1.;
	const double stheta = sqrt(1. - ctheta * ctheta);
	thedx0 = cos(phi) * stheta;
	thedy0 = sin(phi) * stheta;
	thedz0 = ctheta;
      }

      // Current position, direction, velocity and energy
      kx = thedx0, ky = thedy0, kz = thedz0;
      vx = thedx0, vy = thedy0, vz = thedz0;

      
      OnsagerRadius = 0.;
      DielectricConst = 0.;
      if ( medium->GetClassName() == "MediumMagboltz" ) {
	OnsagerRadius = ((MediumMagboltz*)medium)->GetOnsagerRadius();
	DielectricConst =  medium->GetDielectricConstant();
      }
      if (nIonization == 0) std::cout << "The Onsager radius is " << OnsagerRadius << " cm \n";
      
      // Add the electron to the stack.
 
      newElectron.status = 0;
      if (hole) {
	newElectron.hole = true;
      } else {
	newElectron.hole = false;
      }
      // x0,y0,z0 positions represent immobile parent ion position
      // place the corresponding electron at onsagerFactor times the Onsager radius away from the ion in the 
      // initial direction

      newElectron.x0 = x0[ionization];  newElectron.x = x0[ionization]+thedx0*OnsagerRadius*onsagerFactor;
      newElectron.y0 = y0[ionization];  newElectron.y = y0[ionization]+thedy0*OnsagerRadius*onsagerFactor;
      newElectron.z0 = z0[ionization];  newElectron.z = z0[ionization]+thedz0*OnsagerRadius*onsagerFactor;
      newElectron.t0 = t0[ionization];  newElectron.t  = t0[ionization];
      
      // megan: store distance to nearest ion - at first save this as OnsagerRadius*onsagerFactor
      newElectron.mdi = OnsagerRadius*onsagerFactor;

      // Azriel also store position of original parent ion
      newElectron.xi = x0[ionization];
      newElectron.yi = y0[ionization];
      newElectron.zi = z0[ionization];

      newElectron.kx = thedx0; 
      newElectron.ky = thedy0; 
      newElectron.kz = thedz0;
      newElectron.e0 = std::max(e0[ionization], Small);
      // initial kinetic energy of the electron should be corrected by the potential energy from 
      // parent ion and other ions and electrons in the cloud (see below)
      newElectron.energy = newElectron.e0;
      newElectron.band = band;
      // Previous coordinates for distance histogramming.
      newElectron.xLast = newElectron.x; 
      newElectron.yLast = newElectron.y; 
      newElectron.zLast = newElectron.z;
      newElectron.driftLine.clear();
      // megan: add electron id to keep track of them
      newElectron.id = ionization+1;
      stack.push_back(newElectron);

//      megan: to verify that onsager radius and potential are correctly incorporated
//      std::cout << "\n" << "just after added (xi,yi,zi,e0,x,y,z,dist,energy) " 
//      << stack[ionization].xi << " "  << stack[ionization].yi << " " << stack[ionization].zi << " " 
//      << stack[ionization].e0 << " \n"
//      << stack[ionization].x << " " << stack[ionization].y << " " << stack[ionization].z << " "
//      << sqrt((stack[ionization].x-stack[ionization].xi)*(stack[ionization].x-stack[ionization].xi)+(stack[ionization].y-stack[ionization].yi)*(stack[ionization].y-stack[ionization].yi)
//      + (stack[ionization].z-stack[ionization].zi)*(stack[ionization].z-stack[ionization].zi))
//      << " " << stack[ionization].energy << "\n\n";
     
      if (hole) {
	++nHoles;
      } else {
	++nElectrons;
      }

      if (useBandStructure) {
	// With band structure, (kx, ky, kz) represents the momentum.
	// No normalization in this case.
	medium->GetElectronMomentum(std::max(e0[ionization], Small), kx, ky, kz, band);
	stack[0].kx = kx;
	stack[0].ky = ky;
	stack[0].kz = kz;
	stack[0].band = band;
      } else {
	stack[0].band = 0;
	band = 0;
	// Check the given initial direction.
	const double k = sqrt(dx0[ionization] * dx0[ionization] + dy0[ionization] * dy0[ionization] +
			      dz0[ionization] * dz0[ionization]);
	if (fabs(k) < Small) {
	  // Direction has zero norm, draw a random direction.
	  const double phi = TwoPi * RndmUniform();
	  const double ctheta = 2 * RndmUniform() - 1.;
	  const double stheta = sqrt(1. - ctheta * ctheta);
	  stack[0].kx = cos(phi) * stheta;
	  stack[0].ky = sin(phi) * stheta;
	  stack[0].kz = ctheta;
	} else {
	  // Normalise the direction to 1.
	  stack[0].kx /= k; 
	  stack[0].ky /= k; 
	  stack[0].kz /= k;
	}
      }


      // Get the null-collision rate.
      if (first_electron) {
	first_electron = false;
	fLim = medium->GetElectronNullCollisionRate(band)*finer_tracking_factor;
        //std::cout << "fLim = " << fLim << std::endl;
	if (fLim <= 0.) {
	  std::cerr << className << "::Cloud:\n";
	  std::cerr << "    Got null-collision rate <= 0.\n";
	  return false;
	}
      }
    }
  }
    
  // Here correct the kinetic energy of the initial electrons to account for the initial potential 
  // at the creation location from the other ions and electrons
  const int n1Size = stack.size();
  double potential = 0.;
  double rdist = 0.;
  double x, y, z, t, energy;
  double x2, y2, z2;
  double x3, y3, z3;
  double xion, yion, zion;
  double minDistIon;
  if (n1Size > 0) { 
    // Loop over all electrons/holes in the avalanche.
    for (int iE = n1Size; iE--;) {
       // Get an electron/hole from the stack.
      potential = 0.;
      x = stack[iE].x; 
      y = stack[iE].y; 
      z = stack[iE].z;
      minDistIon = 1e99;
      for (int iE2 = n1Size; iE2--;) {

	x2 = stack[iE2].x; 
	y2 = stack[iE2].y; 
	z2 = stack[iE2].z;

	xion = stack[iE2].xi; 
	yion = stack[iE2].yi; 
	zion = stack[iE2].zi;
	
        // do calculations for an electron (no sign change in potential needed)
	// distance to ion
	rdist = sqrt((xion-x)*(xion-x)+(yion-y)*(yion-y)+(zion-z)*(zion-z));
	potential -= ElementaryCharge/(4*Pi*DielectricConst*rdist);

        // store shortest distance to ion for recombination
        if (rdist < minDistIon){
          minDistIon = rdist;
        }

	// distance to electron
	rdist = sqrt((x2-x)*(x2-x)+(y2-y)*(y2-y)+(z2-z)*(z2-z));  
	if (iE != iE2) {
	  // potential from electrons excluding self
	  potential += ElementaryCharge/(4*Pi*DielectricConst*rdist);
	}	
      }

      // save the initial closest ion - not necessarily parent ion (was first calculated as such in newElectron)
      stack[iE].mdi0 = minDistIon;
      stack[iE].mdi = minDistIon;
      stack[iE].mdimax = minDistIon;

      if (stack[iE].e0 - potential < 0) stack[iE].energy = 0.0254;
      else stack[iE].energy = stack[iE].e0 - potential;
      stack[iE].potential0 = potential;
      stack[iE].potential = potential;

      std::cout << "Ek0 (ID,e0,potential,Ek) " << stack[iE].id << " " << stack[iE].e0 << " " << potential << " " << stack[iE].energy << "\n";

//      to verify that onsager radius is and potential are correctly incorporated
//      std::cout << "\n" << "after compute potential (xi,yi,zi,e0,x,y,z,dist,kinetic energy) "
//      << stack[iE].xi << " "  << stack[iE].yi << " " << stack[iE].zi << " "
//      << stack[iE].e0 << " \n"
//      << stack[iE].x << " " << stack[iE].y << " " << stack[iE].z << " "
//      << sqrt((stack[iE].x-stack[iE].xi)*(stack[iE].x-stack[iE].xi)+(stack[iE].y-stack[iE].yi)*(stack[iE].y-stack[iE].yi)
//      + (stack[iE].z-stack[iE].zi)*(stack[iE].z-stack[iE].zi))
//      << " " << stack[iE].energy << "\n\n";

    }
  }

  // Status flag
  double dex,dey,dez;
  double rdist_dx, rdist_dy, rdist_dz;
  double potential_dx, potential_dy, potential_dz;
  double cloud_ex, cloud_ey, cloud_ez;
  double dre;
//  double minDistIon;
  int minDistIonIndex;
  // choose a scale to compute dv/dx dy dz as a fraction of Onsager radius
  // alternative would be to use the null step size (effect?)
  // was previously:
  // dre = OnsagerRadius / 10.0; 
  // megan: dre is no longer dependent on Onsager radius: dre = 1A=1e-8cm
  dre = 1e-8;
  bool ok = true;

// turns true when first particle hits tMax
  // megan: make variables needed for movie
  int framenumber=0;
  int tMaxprint=0;
  // moved to example.C to make the number of frames variable if desired
//  int numberofmovieframes = 100;
//  double movieframetime[numberofmovieframes-1];
//  for (int k = 0; k < numberofmovieframes; k++) {movieframetime[k]=k*(tMax-tMin)/numberofmovieframes;}
// was going to try to use this if printing out each electron as each hit a specific time
/*  int timematrix[numelectrons][numberofmovieframes-1];
  for (int k = 0; k < numelectrons; k++) {
    for (int j = 0; j < numberofmovieframes-1; j++) {
      timematrix[k][j] = 0;
    }
  }
*/
  
  while (1) {
    // If the list of electrons/holes is exhausted, we're done.
    const int nSize = stack.size();
    if (nSize <= 0) break;

    // Loop over all electrons/holes in the avalanche.
    for (int iE = nSize; iE--;) {
      // Get an electron/hole from the stack.
      x = stack[iE].x; 
      y = stack[iE].y; 
      z = stack[iE].z;
      t = stack[iE].t; 
      energy = stack[iE].energy; 
      band = stack[iE].band; 
      kx = stack[iE].kx; 
      ky = stack[iE].ky; 
      kz = stack[iE].kz;
      hole = stack[iE].hole;

      //      std::cout << "Electron from stack (xi,yi,zi,x,y,z,t) " << stack[iE].xi << " "  << stack[iE].yi << " " << stack[iE].zi << " " << x << " " << y << " " << z << " " << t << "\n";
      minDistIon = 1e99;
      minDistIonIndex = -1;
      
      ok = true;
      
      // Count number of collisions between updates.
      int nCollTemp = 0;

      // Get the local electric field and medium.
      sensor->ElectricField(x, y, z, ex, ey, ez, medium, status);


      // Azriel Here add the electric field from the ions and other electrons
      potential = 0.;
      potential_dx = 0.;
      potential_dy = 0.;
      potential_dz = 0.;

      // variable to save the time of the newest and oldest other electron
      // toldest = the e- with the smallest elapsed time, hasn't been updated as recently as others
      // tnewest = the e- with the largest elasped time, has advanced the most compared to the others
      double toldest = 1e99;
      double tnewest = -1e99;
      int toldestindex = -1;
//      int tnewestindex = -1;

      const int n2Size = stack.size();

      // if there is only one electron this invalidates the requirement that it needs to catch up to the oldest
      if (n2Size == 1) {
	toldest = -1e99;
        toldestindex = iE;
	tnewest = 1e99;
      }

      // loops over current electrons to find toldest and tyoungest and calculates potential before
      // tracking this electron begins
      // megan: split into 2 for loops with the same conditions so that can find toldest and toldestindex
      // first and skip calculations if we don't actually want to track this current electron (iE)
      // since we just declared n2Size = stack.size(), this will always be the correct stack size
      for (int iE2 = n2Size; iE2--;) {

	// find the oldest electron
//	if ((stack[iE2].t < toldest) && (iE != iE2)) toldest = stack[iE2].t;
        // megan: remove condition that iE != iE2 since want to continue to trace this e- if it's the oldest
        if (stack[iE2].t < toldest) {
          toldest = stack[iE2].t;
          toldestindex = iE2;
        }

	// find the newest electron
	if ((stack[iE2].t > tnewest) && (iE != iE2)) tnewest = stack[iE2].t;

      }

      if (iE != toldestindex && n2Size>0) continue;

      // since we just declared n2Size = stack.size(), this will always be the correct stack size      
      for (int iE2 = n2Size; iE2--;) {
       
	x2 = stack[iE2].x; 
	y2 = stack[iE2].y; 
	z2 = stack[iE2].z;

	xion = stack[iE2].xi; 
	yion = stack[iE2].yi; 
	zion = stack[iE2].zi;
	
	// distance to ions (calculations assume we are computing the force on a positive charge)
	// field is then reversed if indeed we are tracking an electron (most common case)
	rdist = sqrt((xion-x)*(xion-x)+(yion-y)*(yion-y)+(zion-z)*(zion-z));
	// std::cout << "Location 1 distance to ion: "<< rdist << "\n";
	potential += ElementaryCharge/(4*Pi*DielectricConst*rdist);
	rdist_dx = sqrt((xion-(x+dre))*(xion-(x+dre))+(yion-y)*(yion-y)+(zion-z)*(zion-z));
	potential_dx += ElementaryCharge/(4*Pi*DielectricConst*rdist_dx);
	rdist_dy = sqrt((xion-x)*(xion-x)+(yion-(y+dre))*(yion-(y+dre))+(zion-z)*(zion-z));
	potential_dy += ElementaryCharge/(4*Pi*DielectricConst*rdist_dy);
	rdist_dz = sqrt((xion-x)*(xion-x)+(yion-y)*(yion-y)+(zion-(z+dre))*(zion-(z+dre)));
	potential_dz += ElementaryCharge/(4*Pi*DielectricConst*rdist_dz);

        // store shortest distance to ion for recombination
        if (rdist < minDistIon){
          minDistIon = rdist;
          minDistIonIndex = iE2;
        }

	// distance to electron
	rdist = sqrt((x2-x)*(x2-x)+(y2-y)*(y2-y)+(z2-z)*(z2-z));  
	rdist_dx = sqrt((x2-(x+dre))*(x2-(x+dre))+(y2-y)*(y2-y)+(z2-z)*(z2-z));
	rdist_dy = sqrt((x2-x)*(x2-x)+(y2-(y+dre))*(y2-(y+dre))+(z2-z)*(z2-z));
	rdist_dz = sqrt((x2-x)*(x2-x)+(y2-y)*(y2-y)+(z2-(z+dre))*(z2-(z+dre)));

	if (iE != iE2) {
	  // potential from electrons excluding self
	  potential -= ElementaryCharge/(4*Pi*DielectricConst*rdist);
	  potential_dx -= ElementaryCharge/(4*Pi*DielectricConst*rdist_dx);
	  potential_dy -= ElementaryCharge/(4*Pi*DielectricConst*rdist_dy);
	  potential_dz -= ElementaryCharge/(4*Pi*DielectricConst*rdist_dz);
 //std::cout << "top potential for last electron " << stack[iE].id << ", iE " << iE << " of stack size " << stack.size() << " from electron " << stack[iE2].id << ", iE2 " << iE2
 //<< " : "  << ElementaryCharge/(4*Pi*DielectricConst*rdist) << std::endl;
	}	
      }

      if (minDistIon > stack[iE].mdimax) stack[iE].mdimax = minDistIon;

      // add external field and ions/electrons field
      // std::cout << "Potential " << potential << " Potential dx " << potential_dx << "\n";
      cloud_ex = -(potential_dx-potential)/dre;
      cloud_ey = -(potential_dy-potential)/dre;
      cloud_ez = -(potential_dz-potential)/dre;

      // Sign change for electrons.
      if (!hole) {
        ex = -ex; ey = -ey; ez = -ez;
        potential = -potential;
        potential_dx = -potential_dx; potential_dy = -potential_dy; potential_dz = -potential_dz; 
        cloud_ex = -cloud_ex; cloud_ey = -cloud_ey; cloud_ez = -cloud_ez;
      }

      if (fabs(potential) > 8 || fabs(potential_dx) > 8 || fabs(potential_dy) > 8 || fabs(potential_dz) > 8) {
        std::cout << "High potential of " << potential << std::endl;
        std::cout << "    p_dx " << potential_dx << " p_dy " << potential_dy << " p_dz " << potential_dz << std::endl;
        std::cout << "    (x,y-.2,z,x0,y0-.2,z0,e0) " << x << " " << y-.2 << " " << z << " " << stack[iE].x0 << " " << stack[iE].y0-.2 << " " << stack[iE].z0 << " " << stack[iE].e0 << std::endl;
        std::cout << "    (id,t,energy,newEnergy,e_cloud,counter_steps,minDistIon) "  << stack[iE].id << " " << t << " " << energy << " " << newEnergy << " "
        << sqrt(cloud_ex*cloud_ex + cloud_ey*cloud_ey + cloud_ez*cloud_ez) << " " << counter_steps << " " << minDistIon << std::endl << std::endl;
      }

      // Azriel here need to add protections if field too high (near charges)

      // if (potential > 1.0) { std::cerr << "V: " << potential << " " << x << " " << y << " " << z << "\n";}

      // if the electric field between x,y,z and x+dre,y+dre,z+dre is greater than 4.445e8 V/cm,
      // equal to the electric field at the radius from a +1 ion where the electron has a potential of -8eV,
      // use only electric field due to parallel plates
      if (sqrt(cloud_ex*cloud_ex + cloud_ey*cloud_ey + cloud_ez*cloud_ez) < 4.445e8) {
      
        ex += cloud_ex;
        ey += cloud_ey;
        ez += cloud_ez;
      } else {
        std::cout << "High cloud electric field of " << sqrt(cloud_ex*cloud_ex + cloud_ey*cloud_ey + cloud_ez*cloud_ez) 
        << " eV calculated, using only electric field due to electrodes" << std::endl;
        std::cout << "    potential " << potential << " p_dx " << potential_dx << " p_dy " << potential_dy << " p_dz " << potential_dz << std::endl;
        std::cout << "    (x,y,z,x0,y0,z0,e0) " << x << " " << y << " " << z << " " << stack[iE].x0 << " " << stack[iE].y0 << " " << stack[iE].z0 << " " << stack[iE].e0 << std::endl;
        std::cout << "    (id,t,energy,newEnergy,counter_steps,minDistIon) "  << stack[iE].id << " " << t << " " << energy << " " << newEnergy << " " << counter_steps << " " << minDistIon 
        << std::endl << std::endl;
      }

      if (debug) {
        std::cout << className << "::TransportCloud:\n";
        if (hole) {
          std::cout << "    Drifting hole " << iE+1 << ".\n";
        } else {
          std::cout << "    Drifting electron " << iE+1 << ".\n";
        }
        std::cout << "    Field [V/cm] at (" 
                  << x  << ", " << y  << ", " << z  << "): "
                  << ex << ", " << ey << ", " << ez << "\n";
        std::cout << "    Status: " << status << "\n";
        std::cout << "    Medium: " << medium->GetName() << "\n";
      }

      if (status != 0) {
        // Electron/hole is not inside a drift medium.
        stack[iE].x = x; 
        stack[iE].y = y; 
        stack[iE].z = z;
        stack[iE].t = t; 
        stack[iE].energy = energy; 
        stack[iE].potential = potential;
        stack[iE].band = band;
        stack[iE].kx = kx; 
        stack[iE].ky = ky; 
        stack[iE].kz = kz;
        stack[iE].mdi = minDistIon;
        stack[iE].status = StatusLeftDriftMedium;
        if (hole) {
          endpointsHoles.push_back(stack[iE]);
        } else {
          endpointsElectrons.push_back(stack[iE]);
        }
        stack.erase(stack.begin() + iE);
        if (debug) {
          std::cout << className << "::TransportCloud:\n";
          if (hole) {
            std::cout << "    Hole left the drift medium.\n";  
          } else {
            std::cout << "    Electron left the drift medium.\n";
          }
          std::cout << "    At " << x << ", " << y << "," << z << "\n";
        }
        continue;
      }

      // If switched on, get the local magnetic field.
      if (useBfield) {
        sensor->MagneticField(x, y, z, bx, by, bz, status);
        if (hole) {
          bx *=  Tesla2Internal; 
          by *=  Tesla2Internal;
          bz *=  Tesla2Internal;
        } else {
          bx *= -Tesla2Internal; 
          by *= -Tesla2Internal; 
          bz *= -Tesla2Internal;
        }
        // Make sure that neither E nor B are zero.
        bmag = sqrt(bx * bx + by * by + bz * bz);
        emag = sqrt(ex * ex + ey * ey + ez * ez);
        if (bmag > Small && emag > Small) bOk = true;
        else bOk = false;

        //        std::cout << "Debug : Applying magnetic field: " << bmag << " " << emag << " " << bOk << std::endl;
      }

      // Trace the electron/hole. 
      while (1) {

      // megan: print the location of each electron for n frames between tMin and tMax for movie
      // currently prints status for all electrons when one has reached the next time milestone in movieframetime[]
      //if () framenumber++;
//old      if (t>=movieframetime[framenumber]&&(t<movieframetime[framenumber+1]||framenumber+1==numberofmovieframes)&&framenumber<numberofmovieframes) {
        if (numberofmovieframes!=0&&t>=movieframetime[framenumber]&&framenumber<=numberofmovieframes) {
          framenumber++;
          int numelectrons = stack.size();
          
          std::cout << "movie frame " << framenumber-1 << " at time " << movieframetime[framenumber-1] << " ns for electron " << stack[iE].id 
          << " (x,y-.2,z,t,kE,potential,kx,ky,kz,minDistIon):" << std::endl 
          << x << " " << y-.2 << " " << z << " " << t << " " << energy << " " << potential << " " << kx << " " << ky << " " << kz << " " << minDistIon << std::endl;
          std::cout << "  began at (x0,y0-.2,z0,t0,e0,potential0,mdi0) " << stack[iE].x0 << " " << stack[iE].y0-.2 << " " << stack[iE].z0 << " " << stack[iE].t0 
          << " " << stack[iE].e0 << " " << stack[iE].potential0 << " " << stack[iE].mdi0 << std::endl;
          std::cout << "  paired ion at (xion,yion-.2,zion) " << stack[iE].xi << " " << stack[iE].yi-.2 << " " << stack[iE].zi 
          << " minDistIonMax= " << stack[iE].mdimax << std::endl << std::endl;

          for (int i = 0; i < numelectrons; i++) {
            if (i != iE) {
              std::cout << "movie frame " << framenumber-1 << " at time " << movieframetime[framenumber-1] << " ns for electron " << stack[i].id 
              << " (x,y-.2,z,t,kE,potential,kx,ky,kz,minDistIon):" << std::endl 
              << stack[i].x << " " << stack[i].y-.2 << " " << stack[i].z << " " << stack[i].t << " " << stack[i].energy << " " << stack[i].potential << " " 
              << stack[i].kx << " " << stack[i].ky << " " << stack[i].kz << " " << stack[i].mdi << std::endl;
              std::cout << "  began at (x0,y0-.2,z0,t0,e0,potential0,mdi0) " << stack[i].x0 << " " << stack[i].y0-.2 << " " << stack[i].z0 << " " << stack[i].t0 
              << " " << stack[i].e0 << " " << stack[i].potential0 << " " << stack[i].mdi0 << std::endl;
              std::cout << "  paired ion at (xion,yion-.2,zion) " << stack[i].xi << " " << stack[i].yi-.2 << " " << stack[i].zi 
              << " minDistIonMax= " << stack[i].mdimax << std::endl << std::endl;
            } 
          } 
        }
/*       
        if (t>=tMax&&tMaxprint==0) {
          tMaxprint++;
          int numelectrons = stack.size();
          std::cout << "final simultaneous in-simulation printout at max time " << tMax << " ns for electron " << stack[iE].id 
          << " (x,y-.2,z,t,kE,potential,kx,ky,kz,minDistIon):" << std::endl
          << x << " " << y-.2 << " " << z << " " << t << " " << energy << " " << potential << " " << kx << " " << ky << " " << kz << " " << minDistIon << std::endl;
          std::cout << "  began at (x0,y0-.2,z0,t0,e0,potential0,mdi0) " << stack[iE].x0 << " " << stack[iE].y0-.2 << " " << stack[iE].z0 << " " << stack[iE].t0
          << " " << stack[iE].e0 << " " << stack[iE].potential0 << " " << stack[iE].mdi0 << std::endl;
          std::cout << "  paired ion at (xion,yion-.2,zion) " << stack[iE].xi << " " << stack[iE].yi-.2 << " " << stack[iE].zi 
          << " minDistIonMax= " << stack[iE].mdimax << std::endl << std::endl;

          for (int i = 0; i < numelectrons; i++) {
            if (i != iE) {
              std::cout << "final simultaneous in-simulation printout at max time " << tMax << " ns for electron " << stack[i].id 
              << " (x,y-.2,z,t,kE,potential,kx,ky,kz,minDistIon):" << std::endl
              << stack[i].x << " " << stack[i].y-.2 << " " << stack[i].z << " " << stack[i].t << " " << stack[i].energy << " " << stack[i].potential << " "
              << stack[i].kx << " " << stack[i].ky << " " << stack[i].kz << " " << stack[i].mdi << std::endl;
              std::cout << "  began at (x0,y0-.2,z0,t0,e0,potential0,mdi0) " << stack[i].x0 << " " << stack[i].y0-.2 << " " << stack[i].z0 << " " << stack[i].t0
              << " " << stack[i].e0 << " " << stack[i].potential0 << " " << stack[i].mdi0 << std::endl;
              std::cout << "  paired ion at (xion,yion-.2,zion) " << stack[i].xi << " " << stack[i].yi-.2 << " " << stack[i].zi 
              << " minDistIonMax= " << stack[i].mdimax << std::endl << std::endl;            
            }
          }
        }

        if (t>=tMax) {
          // print each electron as it gets to tMax - so they're not collected from the same instant in the simulation
          std::cout << "final in-simulation printout at max time " << tMax << " ns for electron " << stack[iE].id << " (x,y-.2,z,t,kE,potential,kx,ky,kz,minDistIon):"
          << std::endl << x << " " << y-.2 << " " << z << " " << t << " " << energy << " " << potential << " " << kx << " " << ky << " " << kz << " " << minDistIon << std::endl;
          std::cout << "  began at (x0,y0-.2,z0,t0,e0,potential0,mdi0) " << stack[iE].x0 << " " << stack[iE].y0-.2 << " " << stack[iE].z0 << " " << stack[iE].t0
          << " " << stack[iE].e0 << " " << stack[iE].potential0 << " " << stack[iE].mdi0 << std::endl;
          std::cout << "  paired ion at (xion,yion-.2,zion) " << stack[iE].xi << " " << stack[iE].yi-.2 << " " << stack[iE].zi 
          << " minDistIonMax= " << stack[iE].mdimax << std::endl << std::endl;
        }
*/

/*
// print collision counters and counter_steps to try to see why they're sometimes 0 at the end
      int pnElastic;
      int pnIonising;
      int pnAttachment;
      int pnInelastic;
      int pnExcitation;
      int pnSuperelastic;
      medium -> GetNumberOfElectronCollisions(pnElastic,pnIonising,pnAttachment,pnInelastic,pnExcitation,pnSuperelastic);
std::cout << "current collisions. elastic: " << pnElastic << " ionizations: " << pnInelastic << " attachments: " << pnAttachment << " inelastic: " << pnInelastic
<< " excitation: " << pnExcitation << " superelastic: " << pnSuperelastic << " total: " << pnElastic+pnIonising+pnAttachment+pnInelastic+pnExcitation+pnSuperelastic
<< " counter_steps: " << counter_steps << std::endl;      
*/

// print position, time, and energy at each step
//        std::cout << "Electron " << iE+1 << " is at (x,y-.2,z,t,e,potential): " << stack[iE].x << " " << stack[iE].y-.2 << " " << stack[iE].z << " " 
//        << stack[iE].t << " with E(eV)= " << stack[iE].energy << " " << potential << std::endl;

        bool isNullCollision = false;

        // Make sure the electron energy exceeds the transport cut.
        if (energy < deltaCut) {
          stack[iE].x = x; 
          stack[iE].y = y; 
          stack[iE].z = z;
          stack[iE].t = t; 
          stack[iE].energy = energy; 
          stack[iE].potential = potential;
          stack[iE].band = band;
          stack[iE].kx = kx; 
          stack[iE].ky = ky; 
          stack[iE].kz = kz;
          stack[iE].mdi = minDistIon;
          stack[iE].status = StatusBelowTransportCut;
          if (hole) {
            endpointsHoles.push_back(stack[iE]);
          } else {
            endpointsElectrons.push_back(stack[iE]);
          }
          stack.erase(stack.begin() + iE);
          if (debug) {
            std::cout << className << "::TransportCloud:\n";
            std::cout << "    Kinetic energy (" << energy << ")"
                      << " below transport cut.\n";
          }
          ok = false;
          break;
        }
        
        // Fill the energy distribution histogram.
        if (hole && hasHoleEnergyHistogram) {
          histHoleEnergy->Fill(energy);
        } else if (!hole && hasElectronEnergyHistogram) {
          histElectronEnergy->Fill(energy);
        }
// std::cout << "testing??" << std::endl;
//        std::cout << "Electron " << iE+1 << " is at (x,y,z,t,e,potential): " << x << " " << y << " " << z << " " << t << " with E(eV)= " << newEnergy << " " << potential << std::endl;

// this line is the one that matches up with beginning and end for non-recomb cases and that was used in 10.10.13 simulations
//        std::cout << "Electron " << iE+1 << " is at (x,y,z,t,e,potential): " << x << " " << y << " " << z << " " << t << " with E(eV)= " << energy << " " << potential << std::endl;


        // Check if the electrons is within the specified time window.
        if (hasTimeWindow && (t < tMin || t > tMax)) {
          stack[iE].x = x; 
          stack[iE].y = y; 
          stack[iE].z = z;
          stack[iE].t = t; 
          stack[iE].energy = energy; 
          stack[iE].potential = potential;
          stack[iE].band = band;
          stack[iE].kx = kx; 
          stack[iE].ky = ky; 
          stack[iE].kz = kz;
          stack[iE].mdi = minDistIon;
          stack[iE].status = StatusOutsideTimeWindow;
          if (hole) {
            endpointsHoles.push_back(stack[iE]);
          } else {
            endpointsElectrons.push_back(stack[iE]);
          }
//int testvar = stack.begin();
//std::cout << "on time out check stack.begin(): " << testvar << std::endl;
          stack.erase(stack.begin() + iE);
          if (debug) {
            std::cout << className << "::TransportCloud:\n";
            if (hole) {
              std::cout << "    Hole left the time window.\n";  
            } else {
              std::cout << "    Electron left the time window.\n";
            }
            std::cout << "    Time: " << t << "\n";
          }
          ok = false;
          break;
        }
       
        if (medium->GetId() != id) {
          // Medium has changed.
          if (!medium->IsMicroscopic()) {
            // Electron/hole has left the microscopic drift medium.
            stack[iE].x = x; 
            stack[iE].y = y; 
            stack[iE].z = z;
            stack[iE].t = t; 
            stack[iE].energy = energy;
            stack[iE].potential = potential;
            stack[iE].band = band;
            stack[iE].kx = kx; 
            stack[iE].ky = ky; 
            stack[iE].kz = kz;
            stack[iE].mdi = minDistIon;
            stack[iE].status = StatusLeftDriftMedium;
            if (hole) {
              endpointsHoles.push_back(stack[iE]);
            } else {
              endpointsElectrons.push_back(stack[iE]);
            }
            stack.erase(stack.begin() + iE);
            ok = false;
            if (debug) {
              std::cout << className << "::TransportCloud:\n";
              std::cout << "    Medium at " << x << ", " << y << ", " << z 
                        << " does not have microscopic data.\n";
            }
            break;
          }
          id = medium->GetId();
          if (medium->IsSemiconductor() && useBandStructureDefault) {
            useBandStructure = true;
          } else {
            useBandStructure = false;
          }
          // Update the null-collision rate.
          fLim = medium->GetElectronNullCollisionRate(band)*finer_tracking_factor;
          if (fLim <= 0.) {
            std::cerr << className << "::TransportCloud:\n"; 
            std::cerr << "    Got null-collision rate <= 0.\n";
            return false;
          }          
        }

        if (useBfield && bOk) {
          // Calculate the cyclotron frequency.
          wb = OmegaCyclotronOverB * bmag;
          // Rotate the direction vector into the local coordinate system.
          ComputeRotationMatrix(bx, by, bz, bmag, ex, ey, ez);
          RotateGlobal2Local(kx, ky, kz);
          // Calculate the electric field in the rotated system.
          RotateGlobal2Local(ex, ey, ez);
          // Calculate the velocity vector in the local frame.
          const double v = c1 * sqrt(energy);
          vx = v * kx; vy = v * ky; vz = v * kz;
          a1 = vx * ex;
          a2 = c2 * ex * ex;
          a3 = ez / bmag - vy;
          a4 = (ez / wb); 
        } else if (useBandStructure) {
          energy = medium->GetElectronEnergy(kx, ky, kz, vx, vy, vz, band);
        } else {
          // No band structure, no magnetic field.
          // Calculate the velocity vector.
          const double v = c1 * sqrt(energy);
          vx = v * kx; vy = v * ky; vz = v * kz;
          
          a1 = vx * ex + vy * ey + vz * ez;
          a2 = c2 * (ex * ex + ey * ey + ez * ez);
        }

        if (hasUserHandleStep) {
          userHandleStep(x, y, z, t, energy, kx, ky, kz, hole);
        }
  
        // Determine the timestep.
        dt = 0.;
        while (1) {
          // Sample the flight time.
          r = RndmUniformPos();
	  // Azriel increase the null collision rate by finer_tracking_factor to make sure field transport and 
	  // recombination condition are accurately simulated 
	  // may be more natural to make this rate as a function of the onsager radius 
//std::cout << "null coll stepwise (x,y-.2,z,t,energy,electron,nCollTemp,counter_steps) " << x << " " << y-.2 << " " << z << " " << t << " " << energy << " " ;
//std::cout << stack[iE].id << " " << nCollTemp << " " << counter_steps << std::endl;
          dt += - log(r) / fLim ;
          // Calculate the energy after the proposed step.
          if (useBfield && bOk) {
            cwt = cos(wb * dt); swt = sin(wb * dt);
            newEnergy = std::max(energy + (a1 + a2 * dt) * dt + 
                                 a4 * (a3 * (1. - cwt) + vz * swt), 
                                 Small);
          } else if (useBandStructure) {
            newEnergy = std::max(medium->GetElectronEnergy(
                                              kx + ex * dt * SpeedOfLight,
                                              ky + ey * dt * SpeedOfLight,
                                              kz + ez * dt * SpeedOfLight, 
                                              newVx, newVy, newVz, band), 
                                 Small);
          } else {
            newEnergy = std::max(energy + (a1 + a2 * dt) * dt, Small);
          }
          // Get the real collision rate at the updated energy.
          double fReal = medium->GetElectronCollisionRate(newEnergy, band);
          if (fReal <= 0.) {
            std::cerr << className << "::TransportCloud:\n";
            std::cerr << "    Got collision rate <= 0.\n";
            std::cerr << "    At " << newEnergy << " eV (band " << band << ").\n";
            return false;
          }
          if (fReal > fLim) {
            // Real collision rate is higher than null-collision rate.
            dt += log(r) / fLim;
            // Increase the null collision rate and try again.
            std::cerr << className << "::TransportCloud:\n";
            std::cerr << "    Increasing null-collision rate by 5%.\n"; 
            if (useBandStructure) std::cerr << "    Band " << band << "\n";
            fLim *= 1.05;
            continue;
          }

//std::cout << " fLim= " << fLim << " fReal= " << fReal << " newEnergy= " << newEnergy << " null= " << isNullCollision
//<< " dt= " << dt << " x= " << x << "\n";

          // Check for real or null collision.
          if (RndmUniform() <= fReal / fLim) {
//std::cout << "fReal= " << fReal << " newEnergy= " << newEnergy << " null= R "
//<< " dt= " << dt << " x= " << x << " y= " << y << " z= " << z;
            break;
          }
          if (useNullCollisionSteps) {
            isNullCollision = true;

//std::cout << "fReal= " << fReal << " newEnergy= " << newEnergy << " null= " << isNullCollision
//<< " dt= " << dt << " x= " << x << " y= " << y << " z= " << z;
            break;
          } else {
//std::cout << "fReal= " << fReal << " newEnergy= " << newEnergy << " null= " << isNullCollision
//<< " dt= " << dt << " x= " << x << " y= " << y << " z= " << z << " vx= " << vx << " dx= " << 0 << "\n";
          }
        }
        if (!ok) break;

        // Increase the collision counter.
        ++nCollTemp;

//if (!isNullCollision) {
//std::cout << "real coll stepwise (x,y-.2,z,t,energy,potential,electron,nCollTemp) " << x << " " << y-.2 << " " << z << " " ;
//old std::cout << sqrt((stack[iE].x-x)*(stack[iE].x-x)+(stack[iE].y-y)*(stack[iE].y-y)+(stack[iE].z-z)*(stack[iE].z-z)) << " " << t << " " << t-stack[iE].t << " " << newEnergy << " " ;
//std::cout << t << " " << newEnergy << " " ;
//std::cout << potential << " " << stack[iE].id << " " << nCollTemp << std::endl;
//}
        // Update the directions (at instant before collision)
        // and calculate the proposed new position.
        if (useBfield && bOk) {
          // Calculate the new velocity.
          newVx = vx + 2. * c2 * ex * dt;
          newVy = vz * swt - a3 * cwt + ez / bmag;
          newVz = vz * cwt + a3 * swt;
          // Normalise and rotate back to the lab frame.
          const double v = sqrt(newVx * newVx + newVy * newVy + 
                                newVz * newVz);
          newKx = newVx / v; newKy = newVy / v; newKz = newVz / v; 
          RotateLocal2Global(newKx, newKy, newKz);
          // Calculate the step in coordinate space.
          vx += c2 * ex * dt;
          ky = (vz * (1. - cwt) - a3 * swt) / (wb * dt) + ez / bmag;
          kz = (vz * swt + a3 * (1. - cwt)) / (wb * dt); 
          vy = ky; vz = kz;
          // Rotate back to the lab frame.
          RotateLocal2Global(vx, vy, vz);

          //          std::cout << "Yasu: " << vx << " " << vy << " " << vz << std::endl;
        } else if (useBandStructure) {
          // Update the wave-vector.
          newKx = kx + ex * dt * SpeedOfLight;
          newKy = ky + ey * dt * SpeedOfLight;
          newKz = kz + ez * dt * SpeedOfLight;
          // Average velocity over the step.
          vx = 0.5 * (vx + newVx);
          vy = 0.5 * (vy + newVy);
          vz = 0.5 * (vz + newVz);
        } else {
          // Update the direction.
          a1 = sqrt(energy / newEnergy);
          a2 = 0.5 * c1 * dt / sqrt(newEnergy);
          newKx = kx * a1 + ex * a2; 
          newKy = ky * a1 + ey * a2; 
          newKz = kz * a1 + ez * a2;

          // Calculate the step in coordinate space.
          a1 = c1 * sqrt(energy);
          a2 = dt * c2; 
          vx = kx * a1 + ex * a2;
          vy = ky * a1 + ey * a2;
          vz = kz * a1 + ez * a2;
        }
 
        // Get the electric field and medium at the proposed new position:
	x3 = x + vx * dt;
	y3 = y + vy * dt;
	z3 = z + vz * dt;
//std::cout << "a1 " << a1 << " a2 " << a2 << " c1 " << c1 << " c2 " << c2 << " ex " << ex << " ey " << ey << " ez " << ex << " kx " << kx << " ky " << ky << " kz " 
//<< kz << std::endl; 
//std::cout << "energy (beg)= " << energy <<  " newEnergy (end)= " << newEnergy << " null= " << isNullCollision << " dt= " << dt << " vx= " << vx << " dx= " << x3-x 
//<< " v= " << sqrt(vx*vx + vy*vy + vz*vz) << " vcalc= " << sqrt(2*energy*1.60217657e-19/9.10938215e-31)*1e-7 << " dist= " << sqrt((x-x3)*(x-x3)+(y-y3)*(y-y3)+(z-z3)*(z-z3)) 
//<< " t= " << t << " x= " << x << "\n";

        sensor->ElectricField(x3, y3, z3, 
                              ex, ey, ez, medium, status);

	// add here the field from elelctrons and ions in the event
	potential = 0.;
	potential_dx = 0.;
	potential_dy = 0.;
	potential_dz = 0.;
	
	minDistIon = 1e99;
	minDistIonIndex = -1;

	// consider all the ions/electrons that still exist
        // megan: used to be iE2 = n1Size -  wrong varible (it is from the beginning of the simulation when the initial kinetic energy is corrected,
        // always equal to size of original cloud). using stack.size() always gets the correct size after recombinations and electrons that have timed out.
	for (int iE2 = stack.size(); iE2--;) {

	  x2 = stack[iE2].x; 
	  y2 = stack[iE2].y; 
	  z2 = stack[iE2].z;

	  xion = stack[iE2].xi; 
	  yion = stack[iE2].yi; 
	  zion = stack[iE2].zi;
	  
          // distance to ions (calculations assume we are computing the force on a positive charge)
          // field is then reversed if indeed we are tracking an electron (most common case)
	  rdist = sqrt((xion-x3)*(xion-x3)+(yion-y3)*(yion-y3)+(zion-z3)*(zion-z3));
	  // std::cout << "Location 1 distance to ion: " << rdist << "\n" ;
	  potential += ElementaryCharge/(4*Pi*DielectricConst*rdist);
	  rdist_dx = sqrt((xion-(x3+dre))*(xion-(x3+dre))+(yion-y3)*(yion-y3)+(zion-z3)*(zion-z3));
	  potential_dx += ElementaryCharge/(4*Pi*DielectricConst*rdist_dx);
	  rdist_dy = sqrt((xion-x3)*(xion-x3)+(yion-(y3+dre))*(yion-(y3+dre))+(zion-z3)*(zion-z3));
	  potential_dy += ElementaryCharge/(4*Pi*DielectricConst*rdist_dy);
	  rdist_dz = sqrt((xion-x3)*(xion-x3)+(yion-y3)*(yion-y3)+(zion-(z3+dre))*(zion-(z3+dre)));
	  potential_dz += ElementaryCharge/(4*Pi*DielectricConst*rdist_dz);

	  // store shortest distance to ion for recombination
	  if (rdist < minDistIon){
	    minDistIon = rdist;
	    minDistIonIndex = iE2;
	  }
	  // distance to electrons
	  rdist = sqrt((x2-x3)*(x2-x3)+(y2-y3)*(y2-y3)+(z2-z3)*(z2-z3));  
	  rdist_dx = sqrt((x2-(x3+dre))*(x2-(x3+dre))+(y2-y3)*(y2-y3)+(z2-z3)*(z2-z3));
	  rdist_dy = sqrt((x2-x3)*(x2-x3)+(y2-(y3+dre))*(y2-(y3+dre))+(z2-z3)*(z2-z3));
	  rdist_dz = sqrt((x2-x3)*(x2-x3)+(y2-y3)*(y2-y3)+(z2-(z3+dre))*(z2-(z3+dre)));
	  
	  if (iE != iE2) {
	    // potential from electrons excluding self
	    potential -= ElementaryCharge/(4*Pi*DielectricConst*rdist);
	    potential_dx -= ElementaryCharge/(4*Pi*DielectricConst*rdist_dx);
	    potential_dy -= ElementaryCharge/(4*Pi*DielectricConst*rdist_dy);
	    potential_dz -= ElementaryCharge/(4*Pi*DielectricConst*rdist_dz);
// std::cout << "mid-trace potential for last electron " << stack[iE].id << ", iE " << iE << " of stack size " << stack.size() << " from electron " << stack[iE2].id << ", iE2 " << iE2
// << " : "  << ElementaryCharge/(4*Pi*DielectricConst*rdist) << std::endl;
	  }	
	}

	// if (potential > 4.0) { std::cerr << "V: " << potential << " " << x << " " << y << " " << z << "\n";}

        // add external field and ions/electrons field
        cloud_ex = -(potential_dx-potential)/dre;
        cloud_ey = -(potential_dy-potential)/dre;
        cloud_ez = -(potential_dz-potential)/dre;

        // Sign change for electrons.
        if (!hole) {
          ex = -ex; ey = -ey; ez = -ez;
          potential = -potential;
          potential_dx = -potential_dx; potential_dy = -potential_dy; potential_dz = -potential_dz;
          cloud_ex = -cloud_ex; cloud_ey = -cloud_ey; cloud_ez = -cloud_ez;
        }

        if (fabs(potential) > 8 || fabs(potential_dx) > 8 || fabs(potential_dy) > 8 || fabs(potential_dz) > 8) {
          std::cout << "High potential of " << potential << std::endl;
          std::cout << "    p_dx " << potential_dx << " p_dy " << potential_dy << " p_dz " << potential_dz << std::endl;
          std::cout << "    (x,y-.2,z,x0,y0-.2,z0,e0) " << x << " " << y-.2 << " " << z << " " << stack[iE].x0 << " " << stack[iE].y0-.2 << " " << stack[iE].z0 << " " << stack[iE].e0 << std::endl;
          std::cout << "    (id,t,energy,newEnergy,e_cloud,counter_steps,minDistIon) " << stack[iE].id << " " << t << " " << energy << " " << newEnergy << " "
          << sqrt(cloud_ex*cloud_ex + cloud_ey*cloud_ey + cloud_ez*cloud_ez) << " " << counter_steps << " " << minDistIon << std::endl << std::endl;
        }

        // if the electric field between x,y,z and x+dre,y+dre,z+dre is greater than 4.445e8 V/cm,
        // equal to the electric field at the radius from a +1 ion where the electron has a potential of -8eV,
        // use only electric field due to parallel plates
        if (sqrt(cloud_ex*cloud_ex + cloud_ey*cloud_ey + cloud_ez*cloud_ez) < 4.445e8) {          
          ex += cloud_ex;
          ey += cloud_ey;
          ez += cloud_ez;
        } else {
          std::cout << "High cloud electric field of " << sqrt(cloud_ex*cloud_ex + cloud_ey*cloud_ey + cloud_ez*cloud_ez)
          << " eV calculated, using only electric field due to electrodes" << std::endl;
          std::cout << "    potential " << potential << " p_dx " << potential_dx << " p_dy " << potential_dy << " p_dz " << potential_dz << std::endl;
          std::cout << "    (x,y-.2,z,x0,y0-.2,z0,e0) " << x << " " << y-.2 << " " << z << " " << stack[iE].x0 << " " << stack[iE].y0-.2 << " " << stack[iE].z0 << " " << stack[iE].e0 << std::endl;
          std::cout << "    (id,t,energy,newEnergy,counter_steps,minDistIon) " << stack[iE].id << " " << t << " " << energy << " " << newEnergy << " " << counter_steps << " " << minDistIon 
          << std::endl << std::endl;
        }
      
	//std::cout << "Cloud field " << cloud_ex << " " << cloud_ey << " " << cloud_ez << "\n"; 
	// Azriel here need to add protections if field too high (near charges)
	
	counter_steps++;
	//if (counter_steps % 1000000 ==0) {
	//  std::cout << "step counter at " << counter_steps << " " 
        //   << x << " " << y << " " << z << " " << t << " " 
        //   << newEnergy << "\n";
	//}
	// std::cerr << "V: " << potential << " V+y " << potential_dy << " cloud ey (on hole) " << cloud_ey << "\n"; 
	// std::cerr << "External field:  " << ey << "\n"; 

        // Check if the electron is still inside a drift medium.
        if (status != 0) {
          // Try to terminate the drift line close to the boundary
          // by means of iterative bisection.
          stack[iE].x = x; 
          stack[iE].y = y; 
          stack[iE].z = z;
          stack[iE].t = t; 
          stack[iE].energy = energy;
          stack[iE].potential = potential;
          stack[iE].mdi = minDistIon;
          double dx = vx * dt, dy = vy * dt, dz = vz * dt;
          double d = sqrt(dx * dx + dy * dy + dz * dz);
          if (d > 0) {
            dx /= d; dy /= d; dz /= d;
          }
          // Mid-point
          double xM = x, yM = y, zM = z;
          while (d > BoundaryDistance) {
            d *= 0.5;
            dt *= 0.5;
            xM = x + d * dx; yM = y + d * dy; zM = z + d * dz; 
            // Check if the mid-point is inside the drift medium.
            sensor->ElectricField(xM, yM, zM, ex, ey, ez, medium, status);
            if (status == 0) {
              x = xM; y = yM; z = zM; t += dt;
            } 
          }
          // Place the endpoint OUTSIDE the drift medium
          x += d * dx; y += d * dy; z += d * dz; 
          if (useSignal) {
            if (hole) {
              sensor->AddSignal(+1, stack[iE].t, 
                                t - stack[iE].t, 
                                0.5 * (x + stack[iE].x), 
                                0.5 * (y + stack[iE].y),
                                0.5 * (z + stack[iE].z), 
                                vx, vy, vz);
            } else {
              sensor->AddSignal(-1, stack[iE].t,
                                t - stack[iE].t,
                                0.5 * (x + stack[iE].x),
                                0.5 * (y + stack[iE].y),
                                0.5 * (z + stack[iE].z),
                                vx, vy, vz);
            }
          }
          stack[iE].x = x; 
          stack[iE].y = y; 
          stack[iE].z = z;
          stack[iE].t = t;
          stack[iE].kx = newKx; 
          stack[iE].ky = newKy; 
          stack[iE].kz = newKz;
          stack[iE].status = StatusLeftDriftMedium;
          if (hole) {
            endpointsHoles.push_back(stack[iE]);
          } else {
            endpointsElectrons.push_back(stack[iE]);
          }
          stack.erase(stack.begin() + iE);
          ok = false;
          if (debug) {
            std::cout << className << "::TransportCloud:\n";
            if (hole) {
              std::cout << "    Hole left the drift medium.\n";
            } else {
              std::cout << "    Electron left the drift medium.\n";
            }
            std::cout << "    At " << x << ", " << y << "," << z << "\n";
          }
          break;
        }

        // Check if the new position is inside the user area.
        if (!sensor->IsInArea(x + vx * dt, y + vy * dt, z + vz * dt)) {
          // Try to terminate the drift line close to the boundary
          // by means of iterative bisection.
          stack[iE].x = x; 
          stack[iE].y = y; 
          stack[iE].z = z;
          stack[iE].t = t; 
          stack[iE].mdi = minDistIon;
          stack[iE].energy = energy;
          stack[iE].potential = potential;
          double dx = vx * dt, dy = vy * dt, dz = vz * dt;
          double d = sqrt(dx * dx + dy * dy + dz * dz);
          if (d > 0) {
            dx /= d; dy /= d; dz /= d;
          }
          // Mid-point
          double xM = x, yM = y, zM = z;
          while (d > BoundaryDistance) {
            d *= 0.5;
            dt *= 0.5;
            xM = x + d * dx; yM = y + d * dy; zM = z + d * dz; 
            // Check if the mid-point is inside the drift area.
            if (sensor->IsInArea(xM, yM, zM)) {
              x = xM; y = yM; z = zM; t += dt;
            }
          }
          // Place the endpoint OUTSIDE the drift area.
          x += d * dx; y += d * dy; z += d * dz;

          // If switched on, calculate the induced signal over this step.
          if (useSignal) {
            if (hole) {
              sensor->AddSignal(+1, stack[iE].t,
                                t - stack[iE].t,
                                0.5 * (x + stack[iE].x),
                                0.5 * (y + stack[iE].y),
                                0.5 * (z + stack[iE].z),
                                vx, vy, vz);
            } else {
              sensor->AddSignal(-1, stack[iE].t, 
                                t - stack[iE].t, 
                                0.5 * (x + stack[iE].x), 
                                0.5 * (y + stack[iE].y),
                                0.5 * (z + stack[iE].z), 
                                vx, vy, vz);
            }
          }
          stack[iE].x = x; 
          stack[iE].y = y; 
          stack[iE].z = z;
          stack[iE].t = t;
          stack[iE].kx = newKx; 
          stack[iE].ky = newKy; 
          stack[iE].kz = newKz;
          stack[iE].status = StatusLeftDriftArea;
          if (hole) {
            endpointsHoles.push_back(stack[iE]);
          } else {
            endpointsElectrons.push_back(stack[iE]);
          }
          stack.erase(stack.begin() + iE);
          ok = false;
          if (debug) {
            std::cout << className << "::TransportCloud:\n";
            if (hole) {
              std::cout << "    Hole left the drift area.\n";
            } else {
              std::cout << "    Electron left the drift area.\n";
            }
            std::cout << "    At " << x << ", " << y << ", " << z << "\n";
          }
          break;
        }

        // Check if the electron/hole has crossed a wire.
        double xCross = x, yCross = y, zCross = z;
        if (sensor->IsWireCrossed(x, y, z, 
                                  x + vx * dt, y + vy * dt, z + vz * dt,
                                  xCross, yCross, zCross)) {
          // If switched on, calculated the induced signal over this step.
          if (useSignal) {
            dt = sqrt(pow(xCross - x, 2) + 
                      pow(yCross - y, 2) + 
                      pow(zCross - z, 2)) / 
                 sqrt(vx * vx + vy * vy + vz * vz); 
            if (hole) {
              sensor->AddSignal(+1, t, dt, 0.5 * (x + xCross),
                                           0.5 * (y + yCross),
                                           0.5 * (z + zCross), vx, vy, vz);
            } else {
              sensor->AddSignal(-1, t, dt, 0.5 * (x + xCross),
                                           0.5 * (y + yCross),
                                           0.5 * (z + zCross), vx, vy, vz);
            }
          }
          stack[iE].x = xCross; 
          stack[iE].y = yCross; 
          stack[iE].z = zCross;
          stack[iE].t = t + dt;
          stack[iE].potential = potential;
          stack[iE].kx = newKx; 
          stack[iE].ky = newKy; 
          stack[iE].kz = newKz;
          stack[iE].mdi = minDistIon;
          stack[iE].status = StatusLeftDriftMedium;
          if (hole) {
            endpointsHoles.push_back(stack[iE]);
          } else {
            endpointsElectrons.push_back(stack[iE]);
          }
          stack.erase(stack.begin() + iE);
          ok = false;
          if (debug) {
            std::cout << className << "::TransportCloud:\n";
            std::cout << "    Electron/hole hit a wire.\n";
            std::cout << "    At " << x << ", " << y << "," << z << "\n";
          }
          break;
        }
        
        // If switched on, calculate the induced signal.
        if (useSignal) {
          if (hole) {
            sensor->AddSignal(+1, t, dt, x + 0.5 * vx * dt,
                                         y + 0.5 * vy * dt,
                                         z + 0.5 * vz * dt, vx, vy, vz);
          } else {
            sensor->AddSignal(-1, t, dt, x + 0.5 * vx * dt, 
                                         y + 0.5 * vy * dt,
                                         z + 0.5 * vy * dt, vx, vy, vz);
          }
        }

        // Update the coordinates.
        x += vx * dt; y += vy * dt; z += vz * dt; t += dt;

	// Azriel show the step (null or not)
//	// if (iE == 80) {
//	  std::cout << " e- " << iE+1 << " at " << x << " " << y << " " << z << " " << t << " with E(eV)= " << newEnergy << " " << potential << " null= " << isNullCollision
//          << " nCollTemp " << nCollTemp << " counter_steps " << counter_steps << " dist " << sqrt(vx*dt*vx*dt + vy*dt*vy*dt + vz*dt*vz*dt) << " dt " << dt 
//          << " fLim " << fLim << "\n";    
	// }

        // If switched on, get the magnetic field at the new location.
        if (useBfield) {
          sensor->MagneticField(x, y, z, bx, by, bz, status);
          if (hole) {
            bx *=  Tesla2Internal;
            by *=  Tesla2Internal;
            bz *=  Tesla2Internal;
          } else {
            bx *= -Tesla2Internal; 
            by *= -Tesla2Internal; 
            bz *= -Tesla2Internal;
          }
          // Make sure that neither E nor B are zero.
          bmag = sqrt(bx * bx + by * by + bz * bz);
          emag = sqrt(ex * ex + ey * ey + ez * ez);
          if (bmag > Small && emag > Small) bOk = true;
          else bOk = false;
        }

        // Azriel check around here if total energy is negative..... IN PROGRESS
	// total energy includes only the kinetic energy and the potential 
	// from electrons and ions (but excludes the external electric field, correct??)
	// and electron within Onsager radius of some ion
	if (newEnergy > 8.0) {
	  std::cout << "High kinetic energy of: " << newEnergy << "\n";
	  newEnergy = 7.0;
	  std::cout << "Force to remain below ionization threshold: " << newEnergy << "\n";
          std::cout << "    potential " << potential << " p_dx " << potential_dx << " p_dy " << potential_dy << " p_dz " << potential_dz << std::endl;
          std::cout << "    (x,y-.2,z,x0,y0-.2,z0,e0) " << x << " " << y-.2 << " " << z << " " << stack[iE].x0 << " " << stack[iE].y0-.2 << " " << stack[iE].z0 << " " << stack[iE].e0 << std::endl;
          std::cout << "    (id,t,energy,newEnergy,e_cloud,counter_steps,minDistIon) " << stack[iE].id << " " <<  t << " " << energy << " " << newEnergy << " " 
          << sqrt(cloud_ex*cloud_ex + cloud_ey*cloud_ey + cloud_ez*cloud_ez) << " " << counter_steps << " " << minDistIon << std::endl << std::endl;
	}
/*
        // megan: just print info when energy is high to see how it increases to above 8
        if (newEnergy > 7.0) {
          std::cout << "(test) High kinetic energy of: " << newEnergy << "\n";
          std::cout << "    potential " << potential << " p_dx " << potential_dx << " p_dy " << potential_dy << " p_dz " << potential_dz << std::endl;
          std::cout << "    (x,y-.2,z,x0,y0-.2,z0,e0) " << x << " " << y-.2 << " " << z << " " << stack[iE].x0 << " " << stack[iE].y0-.2 << " " << stack[iE].z0 << " " << stack[iE].e0 << std::endl;
          std::cout << "    (id,t,energy,newEnergy,e_cloud,counter_steps,minDistIon) " << stack[iE].id << " " <<  t << " " << energy << " " << newEnergy << " "
          << sqrt(cloud_ex*cloud_ex + cloud_ey*cloud_ey + cloud_ez*cloud_ez) << " " << counter_steps << " " << minDistIon << std::endl << std::endl;
        }
*/

        // print position, time, and energy at each step -- where to put this??
//        std::cout << "Electron " << iE+1 << " is at (x,y,z,t,e,potential): " << x << " " << y << " " << z << " " << t << " with E(eV)= " << newEnergy << " " << potential << std::endl; 
//        std::cout << "Electron " << iE+1 << " is at (x,y,z,t,e,potential): " << x << " " << y << " " << z << " " << t << " with E(eV)= " << energy << " " << potential << std::endl;

        // use the electron's deBroglie wavelength as the radius for the recombination condition instead of the
        // Onsager radius if deBroglieRecomb (passed from command line arg in example.C as 0 or 1) is true. True by default.
        double recombRadius;
        double deBroglieWavelength = 1.226426e-7/sqrt(newEnergy);
        if (deBroglieRecomb && deBroglieWavelength < OnsagerRadius) recombRadius = deBroglieWavelength;
        else recombRadius = OnsagerRadius;
//      std::cout << "recombRadius= " << recombRadius << std::endl;

	if ( (newEnergy + potential < 0.) && (minDistIon < recombRadius) ) {
	  // recombine

          // first update the stack so that the end position and time matches the one printed below recomb info
          // here changed energy to newEnergy, since this is what's used for the recombination condition and x,y,z already updated
          stack[iE].energy = newEnergy;
          stack[iE].potential = potential;
          stack[iE].t = t;
          stack[iE].x = x;
          stack[iE].y = y;
          stack[iE].z = z;
          stack[iE].kx = kx;
          stack[iE].ky = ky;
          stack[iE].kz = kz;
          stack[iE].mdi = minDistIon;

	  std::cout << "Electron " << stack[iE].id << " will recombine, Ek= " << newEnergy << " and potential= "<< potential << " at R= " << minDistIon << "\n";
	  std::cout << "Time to recombine = " << t <<  "\n";
	  std::cout << "Origin x,y,z,e0,potential0 " << stack[iE].x0 << " " <<  stack[iE].y0 << " " <<  stack[iE].z0 << " " << stack[iE].e0 << " " << stack[iE].potential0 << "\n";
	  std::cout << "Final x,y,z " << x << " " <<  y << " " <<  z << "\n";
          std::cout << "Ion x,y,z " << stack[minDistIonIndex].xi << " " <<  stack[minDistIonIndex].yi << " " <<  stack[minDistIonIndex].zi 
          << " , minDistIonMax= " << stack[iE].mdimax << "\n\n";
	  // for the absorbing ion, make that stack element disappear storing the parent ion position
	  // of the electron in the x,y,z position of it
	  // and move the orphaned electron to the still active ion storing the location of it parent 
	  // ion as well
	  double xtemp, ytemp, ztemp;
	  int miE2 =  minDistIonIndex;

	  // swap ion positions between electrons
	  xtemp = stack[iE].xi; ytemp = stack[iE].yi; ztemp = stack[iE].zi;
	  stack[iE].xi = stack[miE2].xi;  stack[iE].yi = stack[miE2].yi; stack[iE].zi = stack[miE2].zi;
	  stack[miE2].xi = xtemp; stack[miE2].yi = ytemp; stack[miE2].zi = ztemp;
	  
	  // remove electron/ion iE from the stack 
          stack[iE].status = StatusRecombined;

	  // in this block can store info on recombined electrons before deletion
	  // for instance how long or distance before recombination
          if (hole) {
            endpointsHoles.push_back(stack[iE]);
          } else {
            endpointsElectrons.push_back(stack[iE]);
          }
//std::cout << "on recomb check stack.begin(): " << stack.begin() << std::endl;
          stack.erase(stack.begin() + iE);
          ok = false;
          if (debug) {
            std::cout << className << "::TransportCloud:\n";
            std::cout << "    Electron/hole pair recombined.\n";
            std::cout << "    At " << x << ", " << y << "," << z << "\n";
          }
	  // this breaks out of the tracing the individual iE electron and re-enters loop with correct number of electrons in the stack
          break;
	  
	}
// std::cout << "testing #5" << std::endl;	
//        std::cout << "Electron " << iE+1 << " is at (x,y,z,t,e,potential): " << x << " " << y << " " << z << " " << t << " with E(eV)= " << newEnergy << " " << potential << std::endl;
//        std::cout << "Electron " << iE+1 << " is at (x,y,z,t,e,potential): " << x << " " << y << " " << z << " " << t << " with E(eV)= " << energy << " " << potential << std::endl;

        if (isNullCollision) {
          energy = newEnergy;
          kx = newKx; ky = newKy; kz = newKz;
/* is updating the stack with info from a null collision a good thing to do?
          // megan: without this the electron being tracked only switches after a real collision.
          // now if there have only been null collisions, update the stack and go to next electron
          if (nCollTemp >= nCollSkip) {
            // Update the stack and switch to next electron
            stack[iE].energy = newEnergy;
            stack[iE].potential = potential;
            stack[iE].t = t;
            stack[iE].x = x;
            stack[iE].y = y;
            stack[iE].z = z;
            stack[iE].kx = kx;
            stack[iE].ky = ky;
            stack[iE].kz = kz;            
            stack[iE].mdi = minDistIon;
            // continue goes back to 2nd while loop, break to the 1st while/for loop
            break;
          }
*/
          continue;
        }

// check to see what's happening in the elastic collisions
// currently seems like finer_tracking_factor = 100 means that e-s don't get scattered backwards ever
// fixed - directions weren't updated after elastic collision where nCollTemp > nCollSkip, changed position of that line
//std::cout << std::endl << "energy before " << newEnergy << " kx0 " << newKx << " ky0 " << newKy << " kz0 " << newKz << std::endl;

        // Get the collision type and parameters.
        medium->GetElectronCollision(newEnergy, cstype, level, 
                                     energy, newKx, newKy, newKz, 
                                     nion, ndxc, band);

//std::cout << "energy after " << energy << " kxf " << newKx << " kyf " << newKy << " kzf " << newKz << std::endl;  

        // If activated, histogram the distance with respect to the
        // last collision.
        if (hasDistanceHistogram && histDistance != 0 &&
            nDistanceHistogramTypes > 0) {
          for (int iType = nDistanceHistogramTypes; iType--;) {
            if (distanceHistogramType[iType] != cstype) continue;
            if (debug) {
              std::cout << className << "::TransportCloud:\n";
              std::cout << "    Collision type: " << cstype << "\n";
              std::cout << "    Fill distance histogram.\n";
              getchar();
            }
            switch (distanceOption) {
              case 'x':
                histDistance->Fill(stack[iE].xLast - x);
                break;
              case 'y':
                histDistance->Fill(stack[iE].yLast - y);
                break;
              case 'z':
                histDistance->Fill(stack[iE].zLast - z);
                break;
              case 'r':
                const double r2 = pow(stack[iE].xLast - x, 2) + 
                                  pow(stack[iE].yLast - y, 2) + 
                                  pow(stack[iE].zLast - z, 2);
                histDistance->Fill(sqrt(r2));
                break;
            }
            stack[iE].xLast = x; 
            stack[iE].yLast = y; 
            stack[iE].zLast = z;
            break;  
          }
        }

        switch (cstype) {
          // Elastic collision
          case ElectronCollisionTypeElastic:
            break;
          // Ionising collision
          case ElectronCollisionTypeIonisation:
            if (usePlotting && plotIonisations) {
              viewer->AddIonisationMarker(x, y, z);
            }
            if (hasUserHandleIonisation) {
              userHandleIonisation(x, y, z, t, cstype, level, medium);
            }
            for (int j = nion; j--;) {
              int itype;
              double esec;
              medium->GetIonisationProduct(j, itype, esec);
              if (itype == IonProdTypeElectron) {
                esec = std::max(esec, Small);
                if (hasSecondaryHistogram) histSecondary->Fill(esec);
                // Add the secondary electron to the stack.
                newElectron = stack[iE];
                // megan: what should initial direction be? currently random - see below
                newElectron.hole = false;
                newElectron.x0 = x; newElectron.x = x;
                newElectron.y0 = y; newElectron.y = y;
                newElectron.z0 = z; newElectron.z = z; 
                newElectron.t0 = t; newElectron.t = t;
                // megan: should energy by esec-potential?
                // since new ion will also be in the stack ...
                newElectron.energy = esec;
                newElectron.e0 = newElectron.energy;
                // megan: addition to keep new e-s at a unique id
                nIonizationTotal++;
                newElectron.id = nIonizationTotal;
                newElectron.potential0 = potential;
                newElectron.potential = potential;
                newElectron.mdi = onsagerFactor*OnsagerRadius;
                // megan: make parent ion .5 xenon radius away from the location of the ionization
                newElectron.xi = x;
                newElectron.yi = y;
                newElectron.zi = z;
                if (useBandStructure) {
                  newElectron.band = -1;
                  medium->GetElectronMomentum(esec, 
                                              newElectron.kx, 
                                              newElectron.ky,
                                              newElectron.kz, 
                                              newElectron.band);
                } else {
                  // Randomise the secondary electron direction.
                  // megan: and place it onsagerFactor*OnsagerRadius away from ion at x,y,z
                  // but ion should be 1 atomic radius away from ionizing electron?
                  // van der waals radius for xenon = 219pm = 2.19e-10cm
                  const double phi = TwoPi * RndmUniform();
                  const double ctheta = 2 * RndmUniform() - 1.;
                  const double stheta = sqrt(1. - ctheta * ctheta);
                  newElectron.kx = cos(phi) * stheta;
                  newElectron.ky = sin(phi) * stheta;
                  newElectron.kz = ctheta;
                  newElectron.x = x + newElectron.kx*OnsagerRadius*onsagerFactor;
                  newElectron.y = y + newElectron.ky*OnsagerRadius*onsagerFactor;
                  newElectron.z = z + newElectron.kz*OnsagerRadius*onsagerFactor;
                }

                std::cout << "Electron-ion pair produced via ionisation by electron " << stack[iE].id << " of energy " << energy << " and potential " << potential << "\n at (x,y,z,t): "
                << x << " " << y << " " << z << " " << t << "\n New electron details are (x,y,z,t,energy,id): " << x << " " << y << " " << z << " " << t << " " 
                << esec << " " << nIonizationTotal << std::endl;

                newElectron.status = 0;
                newElectron.driftLine.clear();
                if (aval && (sizeCut <= 0 || (int)stack.size() < sizeCut)) {
                  stack.push_back(newElectron);
                }
                // Increment the electron counter.
                ++nElectrons;
              } else if (itype == IonProdTypeHole) {
                esec = std::max(esec, Small);
                // Add the secondary hole to the stack.
                newElectron = stack[iE];
                newElectron.hole = true;
                newElectron.x0 = x; newElectron.x = x;
                newElectron.y0 = y; newElectron.y = y;
                newElectron.z0 = z; newElectron.z = z;
                newElectron.t0 = t; newElectron.t = t;
                newElectron.energy = esec;
                newElectron.e0 = newElectron.energy;
                if (useBandStructure) {
                  newElectron.band = -1;
                  medium->GetElectronMomentum(esec, 
                                              newElectron.kx, 
                                              newElectron.ky,
                                              newElectron.kz, 
                                              newElectron.band);
                } else {
                  // Randomise the secondary hole direction.
                  const double phi = TwoPi * RndmUniform();
                  const double ctheta = 2 * RndmUniform() - 1.;
                  const double stheta = sqrt(1. - ctheta * ctheta);
                  newElectron.kx = cos(phi) * stheta;
                  newElectron.ky = sin(phi) * stheta;
                  newElectron.kz = ctheta;
                }
                newElectron.status = 0;
                newElectron.driftLine.clear();
                if (aval && (sizeCut <= 0 || (int)stack.size() < sizeCut)) {
                  stack.push_back(newElectron);
                }
                // Increment the hole counter.
                ++nHoles;
              } else if (itype == IonProdTypeIon) {
                ++nIons;
              }
            }
            if (debug) {
              std::cout << className << "::TransportCloud:\n";
              std::cout << "    Ionisation.\n";
              std::cout << "    At " << x << "," << y << "," << z << "\n"; 
            }
            break;
          // Attachment
          case ElectronCollisionTypeAttachment:
            if (usePlotting && plotAttachments) {
              viewer->AddAttachmentMarker(x, y, z);
            }
            if (hasUserHandleAttachment) {
              userHandleAttachment(x, y, z, t, cstype, level, medium);
            }
            stack[iE].x = x; 
            stack[iE].y = y; 
            stack[iE].z = z;
            stack[iE].t = t; 
            stack[iE].energy = energy;
            stack[iE].potential = potential;
            stack[iE].mdi = minDistIon;
            stack[iE].status = StatusAttached;
            if (hole) {
              endpointsHoles.push_back(stack[iE]);
              --nHoles;
            } else {
              endpointsElectrons.push_back(stack[iE]);
              --nElectrons;
            }
            std::cout << "Electron " << stack[iE].id << " of " << nIonizationTotal << " has attached at (x,y,z,t,e,potential,status):\n" 
            << x << " " << y << " " << z << " " << t << " with E(eV)= " << energy << " " << potential << " status = " << stack[iE].status << std::endl;
            stack.erase(stack.begin() + iE);
            ok = false;
            break;
          // Inelastic collision
          case ElectronCollisionTypeInelastic:
            if (hasUserHandleInelastic) {
              userHandleInelastic(x, y, z, t, cstype, level, medium);
            }
            break;
          // Excitation
          case ElectronCollisionTypeExcitation:
            if (usePlotting && plotExcitations) {
              viewer->AddExcitationMarker(x, y, z);
            }
            if (hasUserHandleInelastic) {
              userHandleInelastic(x, y, z, t, cstype, level, medium);
            }
            if (ndxc > 0) {
              // Get the electrons and photons produced in the 
              // deexcitation cascade.
              double tDxc = 0., sDxc = 0., eDxc = 0.;
              int typeDxc = 0;
              stackPhotonsTime.clear(); stackPhotonsEnergy.clear();
              for (int j = ndxc; j--;) {
                if (!medium->GetDeexcitationProduct(j, tDxc, sDxc,
                                                    typeDxc, eDxc)) {
                  std::cerr << className << "::TransportCloud:\n";
                  std::cerr << "    Cannot retrieve deexcitation product "
                            << j << "/" << ndxc << ".\n";
                  break;
                }
                
                if (typeDxc == DxcProdTypeElectron) {
                  if (!aval || (sizeCut > 0 && (int)stack.size() >= sizeCut)) continue;
                  // Penning ionisation
                  newElectron = stack[iE];
                  double xDxc = x, yDxc = y, zDxc = z;
                  if (sDxc > Small) {
                    // Randomise the point of creation
                    double phiDxc = TwoPi * RndmUniform();
                    double cthetaDxc = 1. - 2 * RndmUniform();
                    double sthetaDxc = sqrt(1. - cthetaDxc * cthetaDxc);
                    xDxc += sDxc * cos(phiDxc) * sthetaDxc;
                    yDxc += sDxc * sin(phiDxc) * sthetaDxc;
                    zDxc += sDxc * cthetaDxc;
                  }
                  // Get the electric field and medium at this location.
                  Medium* dxcMedium = 0;
                  double fx = 0., fy = 0., fz = 0.;
                  sensor->ElectricField(xDxc, yDxc, zDxc, 
                                        fx, fy, fz, dxcMedium, status);
                  // Check if this location is inside a drift medium.
                  if (status != 0) continue;
                  // Check if this location is inside the drift area.
                  if (!sensor->IsInArea(xDxc, yDxc, zDxc)) continue;
                  // Make sure we haven't jumped across a wire.
                  double xCross, yCross, zCross;
                  if (sensor->IsWireCrossed(x, y, z, xDxc, yDxc, zDxc, 
                                            xCross, yCross, zCross)) {
                    continue;
                  } 
                  newElectron.x0 = xDxc; newElectron.x = xDxc;
                  newElectron.y0 = yDxc; newElectron.y = yDxc;
                  newElectron.z0 = zDxc; newElectron.z = zDxc;
                  newElectron.t0 = t + tDxc; newElectron.t = t + tDxc;
                  newElectron.energy = std::max(eDxc, Small);
                  newElectron.e0 = newElectron.energy;
                  // Randomise the initial direction.
                  const double phi = TwoPi * RndmUniform();
                  const double ctheta = 2 * RndmUniform() - 1.;
                  const double stheta = sqrt(1. - ctheta * ctheta);
                  newElectron.kx = cos(phi) * stheta;
                  newElectron.ky = sin(phi) * stheta;
                  newElectron.kz = ctheta;
                  newElectron.status = 0;
                  newElectron.driftLine.clear();
                  // Add the electron to the list.
                  stack.push_back(newElectron);
                  // Increment the electron and ion counters.
                  ++nElectrons; ++nIons;
                } else if (typeDxc == DxcProdTypePhoton && usePhotons && 
                           eDxc > gammaCut) {
                  // Radiative de-excitation
                  stackPhotonsTime.push_back(t + tDxc);
                  stackPhotonsEnergy.push_back(eDxc);
                }
              }
                
              // Transport the photons (if any)
              const int nSizePhotons = stackPhotonsTime.size();
              for (int j = nSizePhotons; j--;) {
                if (aval) {
                  TransportPhoton(x, y, z, 
                                  stackPhotonsTime[j], 
                                  stackPhotonsEnergy[j]);
                }
              }
            }
            break;
          // Super-elastic collision
          case ElectronCollisionTypeSuperelastic:
            break;
          // Acoustic phonon scattering (intravalley)
          case ElectronCollisionTypeAcousticPhonon:
            break;
          // Optical phonon scattering (intravalley)
          case ElectronCollisionTypeOpticalPhonon:
            break;
          // Intervalley scattering (phonon assisted)
          case ElectronCollisionTypeIntervalleyG:
          case ElectronCollisionTypeIntervalleyF:
          case ElectronCollisionTypeInterbandXL:
          case ElectronCollisionTypeInterbandXG:
          case ElectronCollisionTypeInterbandLG:
            break;
          // Coulomb scattering
          case ElectronCollisionTypeImpurity:
            break;
          default:
            std::cerr << className << "::TransportCloud:\n"; 
            std::cerr << "    Unknown collision type.\n";
            ok = false;
            break;
        }

        // Continue with the next electron/hole?
	// change condition to make sure it keeps tracking until it catches up in time with the oldest electron
	// IN PROGRESS for the condition wrt the newest electron  (how?)
 
//std::cout << "nCollTemp " << nCollTemp << " nCollSkip " << nCollSkip << " cstype " << cstype << " t " << t << " toldest " << toldest << std::endl;


        // megan edit:  moved this line from below the next if statement to here so that direction gets
        // updated after a real collision where nCollTemp>nCollSkip. now the check still only happens after
        // real collisions, but this is the easiest place to make sure that all of the variables that
        // get written to the stack are self-consistent and from the same moment in the electron's path.
        // later check that between real collisions isn't too long to wait for many electronss in a cloud.

        kx = newKx; ky = newKy; kz = newKz;

        if (!ok || 
// old            ( (nCollTemp > nCollSkip) && (t > toldest) )||
            // megan: changed t > toldest to t >= toldest ... does this change anything?
            // also changed to nCollTemp >= nCollSkip instead of >
            ( (nCollTemp >= nCollSkip) && (t > toldest) )|| 
            cstype == ElectronCollisionTypeIonisation || 
            (plotExcitations && cstype == ElectronCollisionTypeExcitation) ||
            (plotAttachments && cstype == ElectronCollisionTypeAttachment)) {
          // megan: should this be continue instead of break? if !ok, maybe need to break instead of continue
          // if change to continue, need to update the stack here
//std::cout << "nCollTemp >= nCollSkip: " << nCollTemp << " > " << nCollSkip << " electron id " << stack[iE].id << std::endl;
          break;
        }	
      }
      
      if (!ok) continue;
// std::cout << "testing #6" << std::endl;      
      if (!useBandStructure) {
        // Normalise the direction vector.
        const double k = sqrt(kx * kx + ky * ky + kz * kz);
        kx /= k; ky /= k; kz /= k;
      }
      // Update the stack.
      // should be energy, not newEnergy, since GetElectronCollision(...) passes energy by reference
      stack[iE].energy = energy; 
      stack[iE].potential = potential;
      stack[iE].t = t;
      stack[iE].x = x; 
      stack[iE].y = y; 
      stack[iE].z = z;
      stack[iE].kx = kx; 
      stack[iE].ky = ky; 
      stack[iE].kz = kz;
      stack[iE].mdi = minDistIon;

// print position, time, and energy at each step - ?? this line didn't show up in simulations from 10.10.13
//        std::cout << "3Electron is at (x,y,z,t,e,potential): " << stack[iE].x << " " << stack[iE].y << " " << stack[iE].z << " " << stack[iE].t << " with E(eV)= " << stack[iE].energy << " " 
//        << potential << std::endl;

      // Add a new point to the drift line (if enabled).
      if (useDriftLines) {
        point newPoint;
        newPoint.x = x; newPoint.y = y; newPoint.z = z; newPoint.t = t;
        stack[iE].driftLine.push_back(newPoint);
      }
    }
  }
  nElectronEndpoints = endpointsElectrons.size();
  nHoleEndpoints     = endpointsHoles.size();

  // Calculate the induced charge.
  if (useInducedCharge) {
    for (int i = nElectronEndpoints; i--;) {
      sensor->AddInducedCharge(-1, 
                               endpointsElectrons[i].x0, 
                               endpointsElectrons[i].y0, 
                               endpointsElectrons[i].z0,
                               endpointsElectrons[i].x,  
                               endpointsElectrons[i].y,  
                               endpointsElectrons[i].z);
    }
    for (int i = nHoleEndpoints; i--;) {
      sensor->AddInducedCharge(+1,
                               endpointsHoles[i].x0,
                               endpointsHoles[i].y0,
                               endpointsHoles[i].z0,
                               endpointsHoles[i].x,
                               endpointsHoles[i].y,
                               endpointsHoles[i].z);
    }
  }

  // Plot the drift paths and photon tracks.
  if (usePlotting) {
    // Electrons
    for (int i = nElectronEndpoints; i--;) {
      const int np = GetNumberOfElectronDriftLinePoints(i);
      int jL;
      if (np <= 0) continue;
      viewer->NewElectronDriftLine(np, jL, 
                                   endpointsElectrons[i].x0, endpointsElectrons[i].y0, 
                                   endpointsElectrons[i].z0);
      for (int jP = np; jP--;) {
        GetElectronDriftLinePoint(x, y, z, t, jP, i);
        viewer->SetDriftLinePoint(jL, jP, x, y, z);
      }
    }
    // Holes
    for (int i = nHoleEndpoints; i--;) {
      const int np = GetNumberOfHoleDriftLinePoints(i);
      int jL;
      if (np <= 0) continue;
      viewer->NewHoleDriftLine(np, jL, 
                               endpointsHoles[i].x0, endpointsHoles[i].y0, 
                               endpointsHoles[i].z0);
      for (int jP = np; jP--;) {
        GetHoleDriftLinePoint(x, y, z, t, jP, i);
        viewer->SetDriftLinePoint(jL, jP, x, y, z);
      }
    }
    // Photons
    for (int i = nPhotons; i--;) {
      viewer->NewPhotonTrack(photons[i].x0, photons[i].y0, photons[i].z0,
                             photons[i].x1, photons[i].y1, photons[i].z1);
    } 
  }
  return true;
    
}

void
AvalancheMicroscopic::TransportPhoton(const double x0, const double y0, 
                                      const double z0, 
                                      const double t0, const double e0) {

  // Make sure that the sensor is defined.
  if (sensor == 0) {
    std::cerr << className << "::TransportPhoton:\n";
    std::cerr << "    Sensor is not defined.\n";
    return;
  }

  // Make sure that the starting point is inside a medium.
  Medium* medium;
  if (!sensor->GetMedium(x0, y0, z0, medium)) {
    std::cerr << className << "::TransportPhoton:\n";
    std::cerr << "    No medium at initial position.\n";
    return;
  }
  
  // Make sure that the medium is "driftable" and microscopic.
  if (!medium->IsDriftable() || !medium->IsMicroscopic()) {
    std::cerr << className << "::TransportPhoton:\n";
    std::cerr << "    Medium at initial position does not provide " 
              << " microscopic tracking data.\n";
    return;
  }
  
  if (debug) {
    std::cout << className << "::TransportPhoton:\n";
    std::cout << "    Starting photon transport in medium " 
              << medium->GetName() << ".\n";
  }
  
  // Get the id number of the drift medium.
  int id = medium->GetId();

  // Position 
  double x = x0, y = y0, z = z0;
  double t = t0;
  // Initial direction (randomised)
  double ctheta = 2 * RndmUniform() - 1.;
  double stheta = sqrt(1. - ctheta * ctheta);
  double phi = TwoPi * RndmUniform();
  double dx = cos(phi) * stheta;
  double dy = sin(phi) * stheta;
  double dz = ctheta;
  // Energy 
  double e = e0;
  // Photon collision rate
  double f = 0.;
  // Timestep
  double dt = 0.;

  int type, level;
  double e1;
  int nsec = 0;
  double esec = 0.;

  f = medium->GetPhotonCollisionRate(e);
  if (f <= 0.) return;

  dt = - log(RndmUniformPos()) / f;
  t += dt;
  dt *= SpeedOfLight;
  x += dt * dx; y += dt * dy; z += dt * dz;

  // Check if the photon is still inside a medium.
  if (!sensor->GetMedium(x, y, z, medium) || medium->GetId() != id) {
    // Try to terminate the photon track close to the boundary
    // by means of iterative bisection.
    dx *= dt; dy *= dt; dz *= dt;
    x -= dx; y -= dy; z -= dz;
    double delta = sqrt(dx * dx + dy * dy + dz * dz);
    if (delta > 0) {
      dx /= delta; dy /= delta; dz /= delta;
    }
    // Mid-point
    double xM = x, yM = y, zM = z;
    while (delta > BoundaryDistance) {
      delta *= 0.5;
      dt *= 0.5;
      xM = x + delta * dx; yM = y + delta * dy; zM = z + delta * dz; 
      // Check if the mid-point is inside the drift medium.
      if (sensor->GetMedium(xM, yM, zM, medium) && medium->GetId() == id) {
        x = xM; y = yM; z = zM; t += dt;
      }
    }
    photon newPhoton;
    newPhoton.x0 = x0; newPhoton.y0 = y0; newPhoton.z0 = z0;
    newPhoton.x1 = x;  newPhoton.y1 = y;  newPhoton.z1 = z;
    newPhoton.energy = e0;
    newPhoton.status = StatusLeftDriftMedium;
    photons.push_back(newPhoton);
    ++nPhotons;
    return;
  }

  if (!medium->GetPhotonCollision(e, type, level, e1, ctheta, 
                                  nsec, esec)) return;
 
  if (type == PhotonCollisionTypeIonisation) {
    // Randomise secondary electron direction.
    phi = TwoPi * RndmUniform();
    ctheta = 2 * RndmUniform() - 1.;
    stheta = sqrt(1. - ctheta * ctheta);
    // Add the secondary electron to the stack.
    electron newElectron;
    newElectron.hole = false;
    newElectron.x0 = x; newElectron.x = x;
    newElectron.y0 = y; newElectron.y = y;
    newElectron.z0 = z; newElectron.z = z;
    newElectron.t0 = t; newElectron.t = t; 
    newElectron.energy = std::max(esec, Small);
    newElectron.e0 = newElectron.energy;
    newElectron.kx = cos(phi) * stheta;
    newElectron.ky = sin(phi) * stheta;
    newElectron.kz = ctheta;
    newElectron.status = 0;
    newElectron.driftLine.clear();
    if (sizeCut <= 0 || (int)stack.size() < sizeCut) stack.push_back(newElectron);
    // Increment the electron and ion counters.        
    ++nElectrons; ++nIons;
  } else if (type == PhotonCollisionTypeExcitation) {
    double tDxc = 0.;
    double sDxc = 0.;
    int typeDxc = 0;
    std::vector<double> stackPhotonsTime;   stackPhotonsTime.clear();
    std::vector<double> stackPhotonsEnergy; stackPhotonsEnergy.clear();
    for (int j = nsec; j--;) {
      if (!medium->GetDeexcitationProduct(j, tDxc, sDxc,
                                          typeDxc, esec)) continue;
      if (typeDxc == DxcProdTypeElectron) {
        // Ionisation
        phi = TwoPi * RndmUniform();
        ctheta = 2 * RndmUniform() - 1.;
        stheta = sqrt(1. - ctheta * ctheta);
        // Add the electron to the stack.
        electron newElectron;
        newElectron.hole = false;
        newElectron.x0 = x; newElectron.x = x;
        newElectron.y0 = y; newElectron.y = y;
        newElectron.z0 = z; newElectron.z = z;
        newElectron.t0 = t + tDxc; newElectron.t = t + tDxc; 
        newElectron.energy = std::max(esec, Small);
        newElectron.e0 = newElectron.energy;
        newElectron.kx = cos(phi) * stheta;
        newElectron.ky = sin(phi) * stheta;
        newElectron.kz = ctheta;
        newElectron.status = 0;
        newElectron.driftLine.clear();
        stack.push_back(newElectron);
        // Increment the electron and ion counters.        
        ++nElectrons; ++nIons;
      } else if (typeDxc == DxcProdTypePhoton && 
                 usePhotons && esec > gammaCut) {
        // Radiative de-excitation
        stackPhotonsTime.push_back(t + tDxc);
        stackPhotonsEnergy.push_back(esec);
      }
    }
    // Transport the photons (if any).
    const int nSizePhotons = stackPhotonsTime.size();
    for (int k = nSizePhotons; k--;) {
      TransportPhoton(x, y, z, stackPhotonsTime[k], stackPhotonsEnergy[k]);
    }
  }

  photon newPhoton;
  newPhoton.x0 = x0; newPhoton.y0 = y0; newPhoton.z0 = z0;
  newPhoton.x1 = x;  newPhoton.y1 = y;  newPhoton.z1 = z;
  newPhoton.energy = e0;
  newPhoton.status = -2;
  photons.push_back(newPhoton);
  ++nPhotons;

}

void 
AvalancheMicroscopic::ComputeRotationMatrix(
    const double bx, const double by, const double bz, const double bmag, 
    const double ex, const double ey, const double ez) {

  // Adopting the Magboltz convention, the stepping is performed
  // in a coordinate system with the B field along the x axis
  // and the electric field at an angle btheta in the x-z plane.

  // Calculate the first rotation matrix (to align B with x axis).
  const double bt = by * by + bz * bz;
  if (bt < Small) {
    // B field is already along axis.
    rb11 = rb22 = rb33 = 1.;
    rb12 = rb13 = rb21 = rb23 = rb31 = rb32 = 0.;
  } else {
    rb11 = bx / bmag;
    rb12 = by / bmag; rb21 = -rb12;
    rb13 = bz / bmag; rb31 = -rb13;
    rb22 = (rb11 * by * by + bz * bz) / bt;
    rb33 = (rb11 * bz * bz + by * by) / bt;
    rb23 = rb32 = (rb11 - 1.) * by * bz / bt;
  }
  // Calculate the second rotation matrix (rotation around x axis).
  const double fy = rb21 * ex + rb22 * ey + rb23 * ez;
  const double fz = rb31 * ex + rb32 * ey + rb33 * ez;
  const double ft = sqrt(fy * fy + fz * fz);
  if (ft < Small) {
    // E and B field are parallel.
    rx22 = rx33 = 1.;
    rx23 = rx32 = 0.;
  } else {
    rx22 = rx33 = fz / ft;
    rx23 = - fy / ft; rx32 = -rx23;
  }

}

void
AvalancheMicroscopic::RotateGlobal2Local(double& dx, double& dy, double& dz) {

  const double dx1 = rb11 * dx + rb12 * dy + rb13 * dz;
  const double dy1 = rb21 * dx + rb22 * dy + rb23 * dz;
  const double dz1 = rb31 * dx + rb32 * dy + rb33 * dz;
  
  dx = dx1;
  dy = rx22 * dy1 + rx23 * dz1;
  dz = rx32 * dy1 + rx33 * dz1;

}

void
AvalancheMicroscopic::RotateLocal2Global(double& dx, double& dy, double& dz) {

  const double dx1 = dx;
  const double dy1 = rx22 * dy + rx32 * dz;
  const double dz1 = rx23 * dy + rx33 * dz;
  
  dx = rb11 * dx1 + rb21 * dy1 + rb31 * dz1;
  dy = rb12 * dx1 + rb22 * dy1 + rb32 * dz1;
  dz = rb13 * dx1 + rb23 * dy1 + rb33 * dz1;

}

}
