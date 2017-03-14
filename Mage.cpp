#include <iostream>
#include <algorithm>
#include <array>
#include <vector>
#include <random>
#include <time.h> 

using namespace std;
// Luke Poniatowski (lhp5025) - Prog. As. 3, 
class Mage {
	
	
private: // Private Members
	struct State {
		// Q values of state actions
		double actions[8];
		//

		// Features
		int op_globe;
		int op_stone;
		int op_streng;

		int op_aciton;

		int my_globe;
		int my_stone;
		int my_streng;

		int my_hp_higher;



		State(int _op_globe, int _op_stone, int _op_streng, int _my_globe, int _my_stone, int _my_streng, int _my_hp_higher, int _op_aciton) {
			for (int ip = 0; ip < 8; ip++) {
				actions[ip] = 0.0;
			}
			actions[6] = actions[7] = 10000;
			op_globe = _op_globe;
			op_stone = _op_stone;
			op_streng = _op_streng;
			my_globe = _my_globe;
			my_stone = _my_stone;
			my_streng = _my_streng;
			my_hp_higher = _my_hp_higher;
			op_aciton = _op_aciton;
		}

		bool isEqual(State s1) {
			if (s1.op_globe == op_globe &&
				s1.op_stone == op_stone &&
				s1.op_streng == op_streng &&
				s1.my_globe == my_globe &&
				s1.my_stone == my_stone &&
				s1.my_streng == my_streng &&
				s1.my_hp_higher == my_hp_higher &&
				s1.op_aciton == op_aciton
				) {
				return true;
			}
			else {
				return false;
			}
		}
	};

	//
	int OP_action = 1;
	int Dmg_dealt = 0;
	int Dmg_taken = 0;
	int Health_below_50 = 0;
	int Invulnerability = 0;
	int Stoneskin = 0;
	int Strength_active = 0;
	int Action = 1;

	//
	vector<State> State_list;
	vector<double*> Action_history = vector<double*>();

	//
	int my_golbe_act = 0;
	int my_stonskin_act = 0;
	int my_stren_act = 0;
	//
	int my_globe_counter = 0;
	int my_stonsking_counter = 0;
	int my_strength_counter = 0;
	int health_potion_count = 1;
	int strength_potion_cout = 1;
	//
	double learnig_rate = .1;
	double leach_back = .05;
	double q_total = 0;
	//
	int my_hp = 100;
	int op_hp = 100;
	int my_hp_higher = 0;
	int epsilon = 25;

public:	// Public Methods
	Mage() {
		// Fill state list with all posiable feature states
		State_list = vector<State>();
		for (int a = 0; a < 2; a++) {
			for (int b = 0; b < 2; b++) {
				for (int c = 0; c < 2; c++) {
					for (int a1 = 0; a1 < 2; a1++) {
						for (int b1 = 0; b1 < 2; b1++) {
							for (int c1 = 0; c1 < 2; c1++) {
								for (int d1 = 0; d1 < 2; d1++) {
									for (int e1 = 1; e1 < 9; e1++) {
										State_list.push_back(State(a, b, c, a1, b1, c1, d1, e1));
									}
								}
							}
						}
					}
				}
			}
		}

	}

	// Reset for new match
	void resetMageStats() {
		health_potion_count = 1;
		strength_potion_cout = 1;
		//
		my_golbe_act = 0;
		my_stonskin_act = 0;
		my_stren_act = 0; 
		//
		my_globe_counter = 0;
		my_stonsking_counter = 0;
		my_strength_counter = 0;
		//
		Dmg_dealt = 0;
		Dmg_taken = 0;
		//
		Health_below_50 = 0;
		Invulnerability = 0;
		Stoneskin = 0;
		Strength_active = 0;
		//
		my_hp = 100;
		op_hp = 100;
		my_hp_higher = 0;
	}

	// Perform Action
	void Act() {

		// Decide what action to use
		Action = decide_action();
		cout << Action << endl;
		
		// Determine if I use a aura
		if (Action == 5) {
			my_globe_counter = 5;
			my_stonsking_counter = 0;
			my_strength_counter = 0;
		}
		else if (Action == 6) {
			my_stonsking_counter = 5;
			my_globe_counter = 0;
			my_strength_counter = 0;
		}
		else if (Action == 8) {
			my_strength_counter = 5;
			my_stonsking_counter = 0;
			my_globe_counter = 0;
		}

		// If my globe is active
		if (my_globe_counter > 0) {
			my_golbe_act = 1;
		}
		else {
			my_golbe_act = 0;
		}

		// If my stonesking is active
		if (my_stonsking_counter > 0) {
			my_stonskin_act = 1;
		}
		else {
			my_stonskin_act = 0;
		}

		// If strength potion is active
		if (my_strength_counter > 0) {
			my_stren_act = 1;
		}
		else {
			my_stren_act = 0;
		}


		// INPUT
		cin >> OP_action;
		cin >> Dmg_dealt;
		cin >> Dmg_taken;
		cin >> Health_below_50;
		cin >> Invulnerability;
		cin >> Stoneskin;
		cin >> Strength_active;

		// Adjust health values
		my_hp -= Dmg_taken;
		op_hp -= Dmg_dealt;

		// If my hp is higher
		if (my_hp > op_hp) {
			my_hp_higher = 1;
			epsilon++; // Decrease % random chance
		}
		else {
			my_hp_higher = 0;
			if (epsilon > 5)
				epsilon--; // Increase % random chance
		}

		// Find current state, adjust q value
		State compare_state = State(Invulnerability, Stoneskin, Strength_active, my_golbe_act, my_stonskin_act, my_stren_act, my_hp_higher, OP_action);
		int counter = 0;
		int found = 0;
		for each (State s in State_list) {
			if (compare_state.isEqual(s)) {
				found = counter;
				//break;
			}
			counter++;
		}
		int Current_Qval = Q_value();
		q_total += Current_Qval;


		// Set Q val for current state
		State_list[found].actions[Action - 1] += learnig_rate * Current_Qval;
		cerr << State_list[found].actions[Action - 1] << endl;

		// Leechback to prev decision
		for (int age = 1; age < 3 && age < Action_history.size(); age++) {
			*Action_history[Action_history.size() - age] += Current_Qval * leach_back / age;
		}

		// Keep a log of states and actions
		Action_history.push_back(&State_list[found].actions[Action -1]);

		//
		my_globe_counter--;
		my_stonsking_counter--;
		my_strength_counter--;

	}

private: // Private Methods
	/* Actions
	1. Magic Missile - dealing 10 points of magic damage to the opponent.
	2. Fireball - dealing 15 points of magic damage to the opponent, but will randomly miss the opponent 33% of the time, causing no damage.
	3. Attack with a dagger - dealing 8 points of physical damage.
	4. Attack with a crossbow - dealing 10 points of physical damage, but may give the opponent an attack of opportunity 33% of the time, and lose 5 points of health yourself.
	5. Globe of Invulnerability - starting from the round it is cast, for 5 rounds, fully negates all magic damage taken on oneself from magic missile or fireball.
	6. Stoneskin - starting from the round it is cast, for 5 rounds, reduces all physical damage taken (from dagger/crossbow) on oneself by half.
	7. Drink a healing potion - instantly gets back 30 points of health (on average, could be as high as 40 or as low as 20). You have only one healing potion per duel. Use it wisely.
	8. Drink a strength potion - starting from the round after it is used, for 5 rounds, all physical attacks deal double damage. You have only one strength potion per duel. Use it wisely.
	*/
	int decide_action() {
		srand((unsigned int)time(NULL));
		// % chance to perform a random action this turn
		int gamma = rand() % epsilon + 1;
		int TO_RETURN = 2;

		if (gamma != 1) {
			State compare_state = State(Invulnerability, Stoneskin, Strength_active, my_golbe_act, my_stonskin_act, my_stren_act, my_hp_higher, OP_action);

			//
			int counter = 0;
			for each (State s in State_list) {
				if (compare_state.isEqual(s)) {
					// Find max q value
					double max_q_val = State_list[counter].actions[0];
					int max_action = 1;
					for (int index = 0; index < 8; index++) {
						if (State_list[counter].actions[index] > max_q_val) {

							if (index > 5) {
								if (health_potion_count > 0 && index == 6) {
									health_potion_count--;
									max_q_val = State_list[counter].actions[index];
									max_action = index + 1;
								}
								else if (strength_potion_cout > 0 && index == 7) {
									strength_potion_cout--;
									max_q_val = State_list[counter].actions[index];
									max_action = index + 1;
								}
							}
							else {
								max_q_val = State_list[counter].actions[index];
								max_action = index + 1;
							}

						}

					}

					return  max_action;
				}
				counter++;
			}


		}
		else {
			TO_RETURN = rand() % 6 + 1;
		}

		return TO_RETURN;
	}

	double gravity = -1; //	Gravity value to ground the q values
	double Q_value() {
		return (Dmg_dealt - Dmg_taken) + gravity + future_val(Action);

	}

	// Determines possiable future value
	double future_val(int act) {
		double TO_RETURN = 0;

		if (act == 5 && (my_globe_counter > 0 && my_globe_counter < 5)) {
			TO_RETURN--;
		}
		if (act == 6 && (my_stonsking_counter > 0 && my_stonsking_counter < 5)) {
			TO_RETURN--;
		}
		if ((act == 3 || act == 4) && my_stren_act == 1)
			TO_RETURN += 2;

		if ((act == 3 || act == 4) && Stoneskin == 1)
			TO_RETURN--;

		if ((act == 1 || act == 2) && Invulnerability == 1)
			TO_RETURN -= 2;

		return TO_RETURN;
	}


};
