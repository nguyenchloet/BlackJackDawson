//Blackjack
//Plays a simple version of the casino game of blackjack; for 1 - 7 players

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>

// CARD CLASS

class Card {
    public: 
        enum rank { ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING};
        enum suit { CLUBS, SPADES, HEARTS, DIAMONDS };

        //overloading << operator so can send Card object to standard output
        friend std::ostream& operator<<(std::ostream& os, const Card& aCard);
        
        Card(rank r = QUEEN, suit s = HEARTS, bool ifu = true);

        //returns the value of a card, 1 - 11
        int GetValue() const;

        //flips a card; if face up, becomes face down and vice versa
        void Flip();

    private:
        rank m_Rank;
        suit m_Suit;
        bool m_IsFaceUp;
};

// constructor outside of the class
Card::Card(rank r, suit s, bool ifu): m_Rank(r), m_Suit(s), m_IsFaceUp(ifu) {}

int Card::GetValue() const {
    //if a cards is face down, its value is 0
    int value = 0;
    if (m_IsFaceUp) {
        //value is number showing on card
        value = m_Rank;
        //value is 10 for face cards
        if (value > 10) {
            value = 10;
        }
    }
    return value;
}

// void function to toggle if m_IsFaceUp
void Card::Flip() {
    m_IsFaceUp = !(m_IsFaceUp);
}
