# g4stuff
Geant4 stuff.  Learn about Geant4, as well as a couple of other things.  

This scope is optimal for my goals at work:
  * Geant4 (examples, mostly)
  * multi-threading (for Geant, and in general)
  * git
  * also check Geant's method of choosing interaction channel
  * other things? (MPI?)

---
## Geant4
Do some Geant examples (`INSTALL_PREFIX/share/Geant4-10.2.1/examples`).

Start with (at least elements of) these things in `examples/`:
  * `basic/B2/`
  * `basic/B4/`
  * `extended/runAndEvent/RE05/` (and `RE06/`)
  * `extended/parameterisations/Par01/`
  * `extended/eventgenerator/particleGun/`
  * `advanced/air_shower/`
  * `advanced/gammaray_telescope`
  * `advanced/lAr_calorimeter`


---
## git
Get **very** familiar with git.  


---
## Multi-Threading
Play with stuff for for general multithreading, Geant4 MT, and art-HPC.  

#### General Multithreading
See `test1` through `test4` (simple tests using `std::thread`).  

#### Geant4 Multithreading
See parallel-processing examples in Geant's examples directory: `Geant4-10.2.1/examples/extended/parallel/` (especially `ThreadsafeScorers/`).  

#### art-HPC multithreading
See `test2.cc` (by Jim Kowalkowski) for event writing.


---
## Geant Cascade termination

Do an actual study of cascade termination in Geant to see which situations might be affected by their method of choosing between interaction channels.

Geant compares interaction lengths in the given material/volume and always chooses the channel with the shortest interaction length.  This, as opposed to some way which selects interaction channels stochastically according to the real branching ratios.  This is generally not a problem because changes in dominant interaction channel usually occur quickly with energy (i.e. Channel A >> Channel B, then there are only a couple of interactions where Channel A ~= Channel B, then Channel A << Channel B).  

**So problems could occur in cases where the interactions lengths are comparable for many interactions.**  This may happen in cascades, in which the handoff from one interaction channel to another is one of the central physical mechanisms which control cascade development.  (Especially in multi-component cascades, which can have an entire ecosystem of 'channel handoffs' that may occur throughout the life of the cascade.)  

**Possible effects:**
  * Geant may produce cascades which do not exhibit enough stochastic variation in development.  
  * Distributions of cascade maxima might be too narrow.  
  * Very early transitions from elastic to inelastic scattering could be suppressed?  
  * What about very long cascades which are outliers on the tail?  

**TODO:** Look for interactions which might exhibit this problem, and then test this with actual simulations.

Possible physical scenarios:
  * pion cascades (neutral+charged): energy handoff from pi0 to pi+-, then termination with p+- decay 
  * electron cascades? (termination of bremsstrahlung and pair production)
  * any cascade initiated by a muon? (e.g. does this matter for `g-2`?)
    * in metal
    * in gas
    * in an electromagnetic field

