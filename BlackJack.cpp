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

// HAND CLASS

class Hand {
    public:
        //constructor
        Hand();
        //destructor
        virtual ~Hand();

        // adds a card to the hand
        void Add(Card* pCard);

        // clears hand of all cards
        void Clear();

        //gets hand total value, intelligently treats aces as 1 or 11
        int GetTotal() const;

    protected:
        std::vector<Card*> m_Cards;
};

Hand::Hand() {}

Hand::~Hand() {
    Clear();
}

// push hand to end of vector
void Hand::Add(Card* pCard) {
    m_Cards.push_back(pCard); 
}

// empty the entire vector
void Hand::Clear() {
    //iterate through vector, freeing all memory on the heap
    std::vector<Card*>::iterator iter = m_Cards.begin();
    for (iter = m_Cards.begin(); iter != m_Cards.end(); ++iter) {
        delete *iter;
        *iter = 0;
    }
    // clear pointers from vector
    m_Cards.clear();
}

int Hand::GetTotal() const {
    // if no cards in hand, return 0
    if (m_Cards.empty()) {
        return 0;
    }
    
    //if a first card has value of 0, then card is face down; return 0
    if (m_Cards[0]->GetValue() == 0) {
        return 0;
    }
   
    //add up card values, treat each ace as 1
    int total = 0;
    std::vector<Card*>::const_iterator iter;
    for (iter = m_Cards.begin(); iter != m_Cards.end(); ++iter ) {
        total+= (*iter)->GetValue();
    }
    
    // determine if hand contains an ace
    bool containsAce = false;
    for (iter = m_Cards.begin(); iter != m_Cards.end(); ++iter) {
        if ((*iter)->GetValue() == Card::ACE ) {
            containsAce = true;
        }
    }

    //if hand contains ace and total is low enough, treat ace as 11
    // ACE card can be either 1 or 11 in value (initial ACE value is 1, thus if used as 11, ten is added)
    if (containsAce && total < 10) {
        total+=10;
    }
    
    // else ACE will remain value 1
    return total;

}
