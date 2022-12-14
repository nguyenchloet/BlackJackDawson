//Blackjack -  Beginning CPP Through Game Programming by Michael Dawson Chapter 10 - Inheritance and Polymorphism: Blackjack
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

// GENERIC PLAYER CLASS

class GenericPlayer : public Hand {
        friend std::ostream& operator<<(std::ostream& os, const GenericPlayer& aGenericPlayer);

    public: 
        GenericPlayer(const std::string& name = " ");

        virtual ~GenericPlayer();

        //indicates whether or not generic player wants to keep hitting
        virtual bool IsHitting() const = 0;

        //returns whether generic player has busted - has a total greater than 21
        bool IsBusted() const;

        //announces that the generic player busts
        void Bust() const;

    protected:
        std::string m_Name;
};

// constructor outside of the class 
GenericPlayer::GenericPlayer(const std::string& name):
    m_Name(name)
{}

// destructor outside of class
GenericPlayer::~GenericPlayer() {}

// check if total is greater than 21
bool GenericPlayer::IsBusted() const {
    return (GetTotal() > 21);
}

// announce name of player busted
void GenericPlayer::Bust() const {
    std::cout << m_Name << " busts. " << std::endl;
}

// PLAYER CLASS

class Player : public GenericPlayer {
    public:
        Player(const std::string& name = "");
        virtual ~Player();

        //returns whether or not the player wants another hit
        virtual bool IsHitting() const;
        
        //announces that the player wins
        void Win() const;
        //announces that the player loses
        void Lose() const;
        //announces that the player pushes
        void Push() const;
};

//constructor outside the class
Player::Player(const std::string& name):
    GenericPlayer(name)
{}

//destructor outside of the class
Player::~Player() {}

// only instance of function outside of GenericPlayer and Player constructor
bool Player::IsHitting() const {
    char response;
    std::cout << m_Name << ", do you want a hit? (Y/N):";
    std::cin >> response;
    return (response == 'Y' || response == 'y');
}

void Player::Win() const {
    std::cout << m_Name << " wins.\n" << std::endl;
}

void Player::Lose() const {
    std::cout << m_Name << " loses.\n" << std::endl;
}

void Player::Push() const {
    std::cout << m_Name << " pushes.\n" << std::endl;
}

// HOUSE CLASS

class House : public GenericPlayer {
    public:
        House(const std::string& name = "House");

        virtual ~House();

        //indicates whether house is hitting - will always hit on 16 or less
        virtual bool IsHitting() const;

        // flips over first card 
        void FlipFirstCard();
};

// constructor outside the class
House::House(const std::string& name):
    GenericPlayer(name)
{}

House::~House()
{}

// if GetTotal <=, return bool True, else False
bool House::IsHitting() const {
    return (GetTotal() <= 16);
}

// House deals with flipping the first card which initially is FaceDown, as shown in BlackJack.cpp file
void House::FlipFirstCard() {
    // using .empty() function for vectors / heap
    if (!m_Cards.empty()) {
        m_Cards[0]->Flip();
    } else {
        std::cout << "No card to flip.";
    }
}

// DECK CLASS

class Deck: public Hand {
    public:

        Deck();

        virtual ~Deck();

        //create a standard deck of 52 cards
        void Populate();

        // shuffle cards
        void Shuffle();

        // deal one card to a hand
        void Deal(Hand& Hand);

        // give additional cards to a generic player
        void AdditionalCards(GenericPlayer& aGenericPlayer);
};

Deck::Deck() {
    m_Cards.reserve(52);
    Populate();
}

Deck::~Deck() {}

void Deck::Populate() {
    Clear();
    // create a standard card deck 
    for (int s=Card::ACE; s<=Card::KING; ++s){
        for (int r=Card::CLUBS; r<=Card::DIAMONDS; ++r){
            Add(new Card(static_cast<Card::rank>(r),static_cast<Card::suit>(s)));
        }
    }
}

void Deck::Shuffle() {
    std::random_shuffle(m_Cards.begin(), m_Cards.end());
}

void Deck::Deal(Hand& aHand){
    if (!m_Cards.empty()) {
        aHand.Add(m_Cards.back());
        m_Cards.pop_back();
    } else {
        std::cout << "Out of cards. Unable to deal." << std::endl;
    }
}

void Deck::AdditionalCards(GenericPlayer& aGenericPlayer){
    //continue to deal a card as long as generic player isn???t busted and wants another hit
    while (aGenericPlayer.IsHitting() && !(aGenericPlayer.IsBusted())) {
        Deal(aGenericPlayer);
        std::cout << aGenericPlayer << std::endl;
        if (aGenericPlayer.IsBusted()){
            aGenericPlayer.Bust();
        }
    }
}

// GAME CLASS

class Game {
    public:
        Game(const std::vector<std::string>& names);
        ~Game();
        // plays the game of blackjack
        void Play();
    private:
        Deck m_Deck;
        House m_House;
        std::vector<Player> m_Players;
};

Game::Game(const std::vector<std::string>& names) {
    //create a vector of players from a vector of names
    std::vector<std::string>::const_iterator pName;
    for (pName = names.begin(); pName != names.end(); ++pName) {
        m_Players.push_back(Player(*pName));
    }
    // seed random number generator
    srand(static_cast<unsigned int>(time(0)));
    m_Deck.Populate();
    m_Deck.Shuffle();
}

Game::~Game()
{}

void Game::Play() {
    // deal initial 2 cards to everyone
    std::vector<Player>::iterator pPlayer;
    for (int i = 0; i < 2; ++i) {
        for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer) {
            m_Deck.Deal(*pPlayer);
        }
        m_Deck.Deal(m_House);
    }
    // hide house's first card
    m_House.FlipFirstCard();
    
    // display everyone's hand
    for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer) {
        std::cout << *pPlayer << std::endl;
    }
    std::cout << m_House<< std::endl;
    //deal additional cards to players
    for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer) {
        m_Deck.AdditionalCards(*pPlayer);
    } 
    //reveal house???s first card 
    m_House.FlipFirstCard(); 
    std::cout << std::endl << m_House;

    //deal additional cards to house
    m_Deck.AdditionalCards(m_House);

    if (m_House.IsBusted()){
        //everyone still playing wins
        for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer) {
            if (!(pPlayer->IsBusted())) {
                pPlayer->Win();
            }
        }
    } else {
        //compare each player still playing to house
        for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer) {
            if (!(pPlayer->IsBusted())) {
                if (pPlayer->GetTotal() > m_House.GetTotal()) {
                    pPlayer->Win();
                } else if (pPlayer->GetTotal() < m_House.GetTotal()) {
                    pPlayer->Lose();
                } else {
                    pPlayer->Push();
                }
            }
        }
    }

    //remove everyone's cards
    for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer) {
       pPlayer->Clear(); 
    }
    m_House.Clear();
}

//function prototypes 
std::ostream& operator<<(std::ostream& os, const Card& aCard);
std::ostream& operator<<(std::ostream& os, const GenericPlayer& aGenericPlayer);

int main() {
    std::cout << "\t\tWelcome to Blackjack!\n\n";

    int numPlayers = 0;
    while (numPlayers < 1 || numPlayers > 7) {
        std::cout << "How many players? (1 - 7): ";
        std::cin >> numPlayers; 
    }
    std::vector<std::string> names;
    std::string name;

    for (int i = 0; i < numPlayers; ++i) {
        std::cout << "Enter player name: "; 
        std::cin >> name; 
        names.push_back(name);
    }
    std::cout << std::endl;

    //the game loop
    Game aGame(names);
    char again = ???y???;
    while (again != ???n??? && again != ???N???) {
        aGame.Play();
        std::cout << "\nDo you want to play again? (Y/N): ";
        std::cin >> again;
    }
    return 0;
}
