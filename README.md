# BlackJack - console game

Game of Black Jack is probably the only game in which player stands a chance of beating the casino.
Program might be useful to practice some tactics and tricks.
Game allows for play on multiple slots and decks to help to strategize.

## Rules

Full rules and tips: https://www.cs.mcgill.ca/~rwest/wikispeedia/wpcd/wp/b/Blackjack.htm

TLDR
Cards 2 through 10 are worth their face value, face cards (jack, queen, king) are also worth 10. An ace's value is 11 or 1.
Whoever gets total sum of thier cards closest to 21 (player or dealer) - wins.
Exceeding 21 will loose hand on the spot (hand will be thrown out and bet will be lost).

After comitting a bet, players get dealt 2 face-up cards and play only against dealer with 1 card up, 1 card face-down.
Player has a choice of:
- hit me - get another card, get offered choice again
- pass - move to another player
if you have only 2 starting cards:
- double down - double the bet, get just one card
- split - if both cards are of the same rank, split them, get only 1 card on each, commit extra bet
- insurance - only if delaer has an Ace you can commit 1/2 of bet to defend against Black Jack, if dealer has a blackjack player will get paid 200% of insuance but loose the bet so it will be net 0, if not but dealer still wins, insurance is lost

Once all players finished with decisions, delaer uncovers 2nd card and draws to 17.
Each player compares his hand with dealer's.
Win is paid 100% of a bet
Black Jack (A + card valued 10) is paid 150% of bet.
Tie - no gain no loss.
