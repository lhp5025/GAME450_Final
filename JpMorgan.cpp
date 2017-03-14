#include <iostream>
#include "Mage.cpp"

using namespace std;
// Luke Poniatowski (lhp5025) - Prog. As. 3, 
// JP Morgan, Q Learnign Bot
int main() {
	int DUEL_COUNT;		// Nubmer of duels
	cin >> DUEL_COUNT;

	
	Mage Me = Mage();
	
	for (int iPrime = 0; iPrime < DUEL_COUNT; iPrime++){
		cerr << "Version 2.4" << endl;

		int END_SIGNAL = 0;

		while (END_SIGNAL == 0){
			// Mage AI, do action
			Me.Act();
			cin >> END_SIGNAL;
		}
		Me.resetMageStats();
	}

	return 0;
}