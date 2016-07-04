
#include "PrimaryGeneratorAction.h"

#include "Geant4/G4LogicalVolumeStore.hh"
#include "Geant4/G4LogicalVolume.hh"
#include "Geant4/G4Box.hh"
#include "Geant4/G4Event.hh"
#include "Geant4/G4ParticleGun.hh"
#include "Geant4/G4ParticleTable.hh"
#include "Geant4/G4ParticleDefinition.hh"
#include "Geant4/G4SystemOfUnits.hh"
#include "Geant4/Randomize.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
	: G4VUserPrimaryGeneratorAction()