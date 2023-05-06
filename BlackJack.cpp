#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <random>
#include <chrono>
#include "windows.h"

void sep();

class Card
{
public:
	char rank = '0';
	int val = 0;

	Card(const char& c) 
	{
		rank = c;
		if (c == 'A') 
			val = 1;
		else if (c == 'T' || c == 'J' || c == 'Q' || c == 'K') 
			val = 10;
		else 
			val = int(c - '0');
	}
};

class Hand
{
public:
	std::vector<Card> cont;
	int val = 0;
	std::string rep;

	bool hasAce = 0;
	bool bj = 0;
	bool dd = 0;
	bool ins = 0;
	
	void addCard(std::vector<Card>& deck)
	{
		cont.push_back(deck[0]);
		val += deck[0].val;
		rep += deck[0].rank;

		checkHand(deck[0]);
		deck.erase(deck.begin());
	}

	void display(std::string&& who, bool&& hide = 0) const
	{
		Sleep(500);
		std::cout << who << ": [";
		if (hide == 1)
			std::cout << rep[0] << "X]";
		else {
			std::cout << rep << "] = " << val;
			if (val < 11 && hasAce == 1) std::cout << " or " << val + 10;
		}
		std::cout << std::endl;
	}

	char decision(const Hand& dealer) const
	{
		char dec;
		std::vector<std::string> poss = possibleDec(dealer);
		
		while (1) {
			try {
				std::cout << "Decision: ";
				for (auto& item : poss)
					std::cout << item << "(" << item[0] << "), ";
				std::cout << "\b\b ? ";
				std::cin >> dec;
				for (auto& item : poss) {
					if (dec == item[0])
						return dec;
				}
				std::cin.clear();
				std::cin.ignore(100, '\n');
				throw std::invalid_argument("Incorrect value");
			}
			catch (const std::invalid_argument& e) {
				std::cout << e.what() << std::endl;
			}
		}
			return dec;
	}

	void pass() 
	{
		if (hasAce && val < 11)
			val += 10;
		sep();
	}

	void doubleDown(std::vector<Card>& deck)
	{
		addCard(deck);
		dd = true;
		Sleep(500);
		std::cout << "Only 1 card coming." << std::endl;
	}

	void insure()
	{
		ins = true;
		std::cout << "Insured" << std::endl;
	}

	float winner(const Hand& dealer, const int& bet) const
	{
		float res = -1;
		if (bj == true)
			res = 1.5f;
		else {
			if (val > dealer.val)
				res = 1;
			else if (val == dealer.val) {
				if (!dealer.bj)
					res = 0;
			}
		}
		if (dd == true)
			res *= 2;
		if (ins == true) {
			if (dealer.bj == true)
				res = 0;
			else
				res -= 0.5f * res;
		}
		displayWin(res, bet);
		return res;
	}

private:
	void checkHand(const Card& card) 
	{
		if (card.rank == 'A')
			hasAce = 1;
		
		if (hasAce == 1 && val == 11) {
			val = 21;
			if (size(cont) == 2) bj = 1; 
		}
	}

	std::vector<std::string> possibleDec(const Hand& dealer) const
	{
		std::vector<std::string> poss = { "pass", "hit" };
		if (cont.size() == 2) {
			poss.push_back("double");
			if (rep[0] == rep[1])
				poss.push_back("split");
			if (dealer.rep[0] == 'A')
				poss.push_back("insure");
		}
		return poss;
	}

	void displayWin(const float& num, const int& bet) const
	{
		if (num > 0)
			std::cout << "WIN (+" << bet*num << ")\t";
		else if (num == 0)
			std::cout << "TIE \t\t";
		else if (num < 0)
			std::cout << "LOSS (" << bet * num << ")\t";
	}
};

void sep()
{
	std::cout << "------------------------------------" << std::endl;
}

int inputHandler(const char* msg, int&& low, int&& high)
{
	int user_input = 0;
	while (1) {
		try {
			printf("%s ? (%d - %d): ", msg, low, high);
			std::cin >> user_input;
			if (std::cin.fail()) {
				std::cin.clear();
				std::cin.ignore(1000, '\n');
				throw std::invalid_argument("Incorrect value");
			}
			if (user_input < low || user_input > high)
				throw std::invalid_argument("Incorrect value");
			break;

		}
		catch (const std::invalid_argument& e) {
			std::cout << e.what() << std::endl;
		}
	}
	return user_input;
}

std::vector<Card> freshDeck(const int &n)
{
	std::vector<Card> deck;
	char ranks[] = { 'A', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K' };
	for (char item : ranks) {
		for (int i = 0; i < 4 * n;i++) 
			deck.push_back(Card(item));
	}
	unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();;
	shuffle(deck.begin(), deck.end(), std::default_random_engine(seed));
	return deck;
}

void displaySlots(const std::vector<Hand>& hands, const int& stack, const int& busts) 
{
		for (int i = 0; i < hands.size() ;i++)
			std::cout << "Slot " << i + 1 << ":\t\t";
		std::cout << "Stack: " << stack;
		if(busts > 0)
			std::cout << "\tBusts: " << busts;
		std::cout << std::endl;

		for (int i = 0; i < hands.size();i++)
			std::cout << hands[i].rep << "\t\t";
		std::cout << std::endl;

		for (int i = 0; i < hands.size();i++) {
			std::cout << "= " << hands[i].val << '\t';
			if (hands[i].hasAce && hands[i].val < 11)
				std::cout << " or " << hands[i].val + 10;
			std::cout << '\t';
		}
	std::cout << std::endl;
}

void split(std::vector<Card>& deck, std::vector<Hand>& hands, const int& ind)
{
	hands[ind].cont.pop_back();
	hands[ind].val = hands[ind].val / 2;
	hands[ind].rep = hands[ind].rep[0];
	hands.insert(hands.begin() + ind + 1, hands[ind]);
	hands[ind].addCard(deck);
	hands[ind + 1].addCard(deck);
}

void bust(std::vector<Hand>& hands, int& ind, int& bal, int& bet, int& count)
{
	hands.erase(hands.begin() + ind);
	bal -= bet;
	count++;
	std::cout << "Bust" << std::endl;
	sep();
}


void game(int stack, std::vector<Card> deck) 
{	
	int slots = 0;
	int bet = 10;
	int bal = 0;
	int busts = 0;
	slots = inputHandler("How many slots", 1, min(5, stack/bet));
	//bet = inputHandler("What size of a bet per slot", 1, 10);

	std::vector<Hand> hands;
	hands.clear();
	hands.reserve(slots);
	Hand dealer;

	//--------------- Dealing hands ----------------
	Hand temp_hand;
	for (int i = 0; i < slots;i++)
		hands.emplace_back(temp_hand);

	for (int j = 0; j < 2;j++) {
		for (int i = 0; i < slots;i++) {
			hands[i].addCard(deck);
		}
		dealer.addCard(deck);
	}
	displaySlots(hands, stack, busts);

	dealer.display("Dealer", 1);
	sep();

	//------------------- Decisions -----------------
	for (int i = 0; i < slots ;i++) {
		hands[i].display("Slot " + std::to_string(i + 1 + busts));

		if (hands[i].val < 21) {
			switch (hands[i].decision(dealer)) {
			case 'p':
				hands[i].pass();
				break;
			case 'h':
				hands[i].addCard(deck);
				i--;
				break;
			case 'd':
				hands[i].doubleDown(deck);
				hands[i].display("Slot " + std::to_string(i + 1 + busts));
				sep();
				break;
			case 's':
				split(deck, hands, i);
				hands[i].display("Slot " + std::to_string(i + 1 + busts));
				Sleep(500);
				hands[i + 1].display("Slot " + std::to_string(i + 2 + busts));
				sep();
				i ++;
				slots++;
				break;
			case 'i':
				hands[i].insure();
				i--;
				break;
			}
		}
		else if (hands[i].val == 21)
			sep();
		else if (hands[i].val > 21) {
			bust(hands, i, bal, bet, busts);
			i--;
			slots--;
		}
	}

	// ------------------- Dealer -----------------
	if (!hands.empty()) {
		Sleep(1000);
		dealer.display("Dealer");
		while (dealer.val < 17) {
			dealer.addCard(deck);
			Sleep(1000);
			dealer.display("Dealer");
		}
		if (dealer.val > 21) {
			std::cout << "Dealer busted!" << std::endl;
			dealer.val = 0;
		}
		std::cout << std::endl;
		
	// ------------------- Final eval -----------------
		displaySlots(hands, stack, busts);
		for (auto& hand : hands) {
			bal += hand.winner(dealer, bet) * bet;
		}
	}
	stack += bal;
	std::cout << "Total: " << bal << std::endl;
	sep();

	if (stack < bet) {
		std::cout << "GAME OVER";
		exit(0);
	}
	if (deck.size() < 30) {
		std::cout << "End of deck" << std::endl;
		int num_decks = 0;
		num_decks = inputHandler("How many decks", 1, 6);
		deck = freshDeck(num_decks);
	}
	game(stack, deck);
}

int main() {

	int stack = 100;
	int num_decks = 0;
	num_decks = inputHandler("How many decks", 1, 6);
	std::vector<Card> deck;
	deck = freshDeck(num_decks);

	game(stack, deck);
}