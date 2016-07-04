/*

based on B2PrimaryGeneratorAction (Geant 4.10.2 examples)

Primary generator action class.  Defines a single partible which hits
the tracker 'perpendicular to the input face'.  

*/

#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h

#include "Geant4/G4VUserPrimaryGeneratorAction.hh"
#include "globals.h"

class G4ParticleGun;    //why the forward declarations?
class G4Event;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
	public:
		PrimaryGeneratorAction();
		virtual ~PrimaryGeneratorAction();
	
		virtual void GeneratePrimaries(G4Event *);
	
		G4ParticleGun * GetParticleGun() { return fParticleGun; }
	
		void SetRandomFlag(G4bool);
	
	private:
		G4ParticleGun * fParticleGun;
	
};

#endif
