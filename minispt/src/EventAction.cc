/// \file EventAction.cc
/// \brief Implementation of the EventAction class

#include "EventAction.hh"
#include "RunData.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4UnitsTable.hh"

#include "Randomize.hh"
#include <iomanip>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction()
        : G4UserEventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::PrintEventStatistics(
      ) const
{
  // print event statistics
  /*G4cout
    << "   Absorber: total energy: "
    << std::setw(7) << G4BestUnit(absoEdep, "Energy")
    << "       total track length: "
    << std::setw(7) << G4BestUnit(absoTrackLength, "Length")
    << G4endl
    << "        Gap: total energy: "
    << std::setw(7) << G4BestUnit(gapEdep, "Energy")
    << "       total track length: "
    << std::setw(7) << G4BestUnit(gapTrackLength, "Length")
    << G4endl;//*/
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* /*event*/)
{
  RunData *runData = static_cast<RunData*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());
  runData->Reset();

  //G4String fileName = "CrashRndmData/G4Worker0_currentEvent.rndm";
  //CLHEP::HepRandom::getTheEngine()->restoreStatus(fileName.c_str());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event*)
{
  RunData *runData = static_cast<RunData*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());
  runData->FillPerEvent();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
