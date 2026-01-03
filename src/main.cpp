#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

enum Color { White, Blue, Black, Red, Green, ColorLess };

enum LandType { Land, BasicLand };

class Pair {
public:
    int first;
    Color second;

    Pair(int f, Color s) : first(std::max(0, f)), second(s) {}

    void display() {
        std::cout << "Pair: " << first << ", Color: " << second << std::endl;
    }
};

class Price {
public:
    std::vector<Pair> pairs;

    void addPair(int num, Color c) {
        if (pairs.size() < 6) {
            pairs.push_back(Pair(num, c));
        } else {
            std::cout << "Cannot add more than 6 pairs." << std::endl;
        }
    }

    void display() {
        std::cout << "Price: ";
        for (size_t i = 0; i < pairs.size(); ++i) {
            pairs[i].display();
        }
        std::cout << std::endl;
    }
};

class Card {
public:
    std::string name;
    std::string instructions;
    Color color;

    Card(std::string n, std::string instr, Color c = ColorLess) : name(n), instructions(instr), color(c) {}

    virtual void display() = 0; // Pure virtual function
};

class Player {
public:
    std::string name;
    int life;
    std::vector<Card*> hand;

    Player(std::string n) : name(n), life(20) {}

    void addToHand(Card* c) {
        hand.push_back(c);
    }

    void removeFromHand(Card* c) {
        auto it = std::find(hand.begin(), hand.end(), c);
        if (it != hand.end()) {
            hand.erase(it);
        }
    }

    void display() {
        std::cout << "Player: " << name << ", Life: " << life << ", Hand size: " << hand.size() << std::endl;
        for (size_t i = 0; i < hand.size(); ++i) {
            std::cout << "Hand card " << (i + 1) << ":" << std::endl;
            hand[i]->display();
        }
    }
};

class Game {
public:
    std::vector<Card*> deck;
    std::map<std::string, int> allowedCopies;

    void setAllowedCopies(std::string name, int max) {
        allowedCopies[name] = max;
    }

    void addCard(Card* c) {
        if (deck.size() >= 60) {
            std::cout << "Deck is full (60 cards)." << std::endl;
            return;
        }
        if (allowedCopies.find(c->name) == allowedCopies.end()) {
            std::cout << "Card " << c->name << " not allowed in this format." << std::endl;
            return;
        }
        int count = 0;
        for (auto card : deck) {
            if (card->name == c->name) count++;
        }
        if (count >= allowedCopies[c->name]) {
            std::cout << "Too many copies of " << c->name << " (max " << allowedCopies[c->name] << ")." << std::endl;
            return;
        }
        deck.push_back(c);
    }

    void displayDeck() {
        std::cout << "Deck size: " << deck.size() << std::endl;
        for (size_t i = 0; i < deck.size(); ++i) {
            std::cout << "Deck card " << (i + 1) << ":" << std::endl;
            deck[i]->display();
        }
    }
};

class SpellCard : public Card {
public:
    Price price;
    std::string type;
    std::vector<std::string> subtypes;
    int power;
    int toughness;

    SpellCard(std::string n, Price p, std::string instr, std::string t = "", std::vector<std::string> sts = {}, int pow = -1, int tough = -1, Color c = ColorLess)
        : Card(n, instr, c), price(p), type(t), subtypes(sts), power(pow), toughness(tough) {}

    void addSubtype(std::string s) {
        subtypes.push_back(s);
    }

    void removeSubtype(std::string s) {
        auto it = std::find(subtypes.begin(), subtypes.end(), s);
        if (it != subtypes.end()) {
            subtypes.erase(it);
        }
    }

    bool isInstant() {
        return type == "Instant";
    }

    void display() override {
        std::cout << "SpellCard: " << name << std::endl;
        price.display();
        std::cout << "Instructions: " << instructions << std::endl;
        std::cout << "Color: " << color << std::endl;
        if (!type.empty()) {
            std::cout << "Type: " << type;
            if (!subtypes.empty()) {
                std::cout << " - ";
                for (size_t i = 0; i < subtypes.size(); ++i) {
                    std::cout << subtypes[i];
                    if (i < subtypes.size() - 1) std::cout << " ";
                }
            }
            std::cout << std::endl;
        }
        if (power != -1) {
            std::cout << "Power: " << power << ", Toughness: " << toughness << std::endl;
        }
    }
};

class LandCard : public Card {
public:
    LandType landType;

    LandCard(std::string n, std::string instr, LandType lt, Color c = ColorLess) : Card(n, instr, c), landType(lt) {}

    void display() override {
        std::cout << "LandCard: " << name << " (" << (landType == BasicLand ? "Basic Land" : "Land") << ")" << std::endl;
        std::cout << "Instructions: " << instructions << std::endl;
        std::cout << "Color: " << color << std::endl;
    }
};



int main() {
    std::cout << "Hello, C++!" << std::endl;

    Player p("Alice");
    p.display();

    Pair p1(1, White);
    p1.display();

    Pair p2(2, Black);
    p2.display();

    Pair p3(3, Red);
    p3.display();

    Price price;
    price.addPair(1, White);
    price.addPair(2, Blue);
    price.addPair(1, Black);
    price.display();

    SpellCard* spell = new SpellCard("Lightning Bolt", price, "Deal 3 damage to any target.", "Instant", {}, -1, -1, Red);
    spell->display();
    std::cout << "Is instant: " << (spell->isInstant() ? "Yes" : "No") << std::endl;
    p.addToHand(spell);

    LandCard* land = new LandCard("Forest", "Tap: Add G.", BasicLand, Green);
    land->display();
    p.addToHand(land);

    SpellCard* creature = new SpellCard("Grizzly Bears", price, "No special abilities.", "Creature", {"Bear"}, 2, 2, Green);
    creature->display();
    std::cout << "Is instant: " << (creature->isInstant() ? "Yes" : "No") << std::endl;
    p.addToHand(creature);

    p.display();

    Game game;
    game.setAllowedCopies("Lightning Bolt", 4);
    game.setAllowedCopies("Forest", 10);
    game.setAllowedCopies("Grizzly Bears", 4);
    game.addCard(spell);
    game.addCard(land);
    game.addCard(creature);
    game.displayDeck();

    creature->addSubtype("Spirit");
    creature->display();

    creature->removeSubtype("Bear");
    creature->display();

    return 0;
}