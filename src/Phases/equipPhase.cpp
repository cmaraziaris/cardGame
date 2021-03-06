/* equipPhase.cpp */
#include <iostream>
#include <string>

#include "basicHeader.hpp"

using std::cout;
using std::endl;

namespace // namespace_start
{

void upgradeGreenCard (PlayerPtr ,      GreenCardPtr );
bool hasEnoughMoney   (PlayerPtr ,      GreenCardPtr );
bool hasEnoughHonor   (PersonalityPtr , GreenCardPtr );
bool hasntReachedLimit(PersonalityPtr , GreenCardPtr );

/* ========================================================================= */
/* Player attempts to purchase an upgrade for a greencard. */
void upgradeGreenCard(PlayerPtr player, GreenCardPtr card)
{
  if (player->makePurchase(card->getEffectCost()) == true)  // Successful 
  {                                                         // Purchase
    card->upgrade();
    printF ("The selected GreenCard has been upgraded!\nNew stats:" , 1 , GRN , BOLD);
    card->print();
    printF ("Current amount of money: " , 0 , YEL , BOLD);
    cout << player->getCurrMoney() << endl;
  }
  else {
    printF ("Not enough money to make the purchase." , 1 , GRN , BOLD);
    printF ("Current money: " , 0 , YEL , BOLD);
    cout << player->getCurrMoney();
    printF ("\nEffect cost: " , 0 ,GRN , BOLD); 
    cout << card->getEffectCost()  << endl;
  }
}
/* ========================================================================= */
/* Check whether a player has enough money to cover a greencard's cost */
bool hasEnoughMoney(PlayerPtr player, GreenCardPtr card)
{ 
  uint16_t currMoney = player->getCurrMoney();
  if (currMoney >= card->getCost()) 
    return true;

  return false;
}
/* ========================================================================= */
/* Check whether a personality has enough honor 
 * to cover a greencard's minimum honor required for attachment
 */
bool hasEnoughHonor(PersonalityPtr person, GreenCardPtr card)
{ 
  if (person->getHonor() >= card->getMinHonor())
    return true;

  return false;
}
/* ========================================================================= */
/* Check whether a personality hasn't reached the maximum number of a 
 * specific kind of attachments (i.e. an item type) already attached to them
 */
bool hasntReachedLimit(PersonalityPtr person, GreenCardPtr card)
{
  uint16_t maxCardPerPers = card->getMaxPerPersonality();

  if (card->getGreenCardType() == GreenCardType::FOLLOWER)
  {
    FollowerPtr curr = std::dynamic_pointer_cast<Follower> (card);

    FollowerListPtr followers = person->getFollowers();

    for (auto i : *followers) /* Find all attachments of the same type */
    {
      if (i->getFollowerType() == curr->getFollowerType())
        --maxCardPerPers;

      if (maxCardPerPers == 0) 
        return false;          /* We've reached the limit */
    }
  }
  else // cardType == ITEM
  {
    ItemPtr curr = std::dynamic_pointer_cast<Item> (card);

    ItemListPtr items = person->getItems();

    for (auto i : *items)
    {
      if (i->getItemType() == curr->getItemType())
        --maxCardPerPers;
      
      if (maxCardPerPers == 0) 
          return false;
    }
  }

  return true;
}


}; // namespace_end


/* ========================================================================= */

void Game::equipmentPhase (PlayerPtr player)
{
  printF ("Equipment Phase Started !" , 1 , GRN , FILL);
  printF ("Press ENTER to continue . . ." , 1);
  std::cin.clear();
  std::cin.sync();
  std::cin.get();

  printF ("Printing " , 0 , MAG , BOLD); 
  cout << player->getUserName(); 
  printF ("'s Army!" , 1 , MAG , BOLD);
  player->printArmy();
  
  printF ("Printing " , 0 , MAG , BOLD); 
  cout << player->getUserName(); 
  printF ("'s Hand!" , 1 , MAG , BOLD);
  player->printHand();

  printF ("Type 'Y' (YES) or '<any other key>' (NO) after each card's \
appearance if you want to enhance the personality's attributes!" , 1 , MAG , BOLD);

  for (auto pers : *(player->getArmy()))
  {                        /* Choose a personality from the army to equip */
    pers->print();
    cout << player->getUserName();
    printF (" , do you want to equip this Personality?" , 1 , GRN , BOLD);
    printF ("> Your answer : " , 0 , MAG , BOLD);
    std::string answer;
    std::getline(std::cin, answer);
    cout << endl;

    if ((answer != "Y")&&(answer != "y")) continue;

    printF ("Printing Cards that are available for purchase in Hand :" , 1 , MAG , BOLD);
    
    for (auto it = player->getHand()->begin() ; it != player->getHand()->end() ; )
    {                      /* Choose a greencard from the hand to equip */
      /* Check if a purchase can be made considering specific limitations */
      GreenCardPtr & card  = *it;
      if ( hasEnoughMoney(player, card) 
        && hasEnoughHonor(pers, card)
        && hasntReachedLimit(pers, card))
      {
        card->print();
        cout << player->getUserName();
        printF ("'s Current balance: " , 0 ,YEL , BOLD);
        cout << player->getCurrMoney() << endl;

        cout << endl;
        printF ("Proceed to purchase?", 1 , YEL , BOLD);
        printF ("> Your answer : " , 0 , MAG , BOLD);
        std::getline(std::cin , answer);
        cout << endl;

        if ((answer != "Y")&&(answer != "y")) { it++; continue; }

        player->makePurchase(card->getCost());  /* Make the purchase */

        card->attachToPersonality(pers);
        
        printF ("Purchase Completed ! " , 1 , YEL , BOLD);
    
        if (player->getCurrMoney() < card->getEffectCost()) { it++; continue; }
        
        printF ("Current balance: " , 0 , YEL , BOLD);
        cout << player->getCurrMoney() << endl;
        printF ("Do you also want to upgrade this card ?" , 1 , GRN , BOLD);
        printF ("> Your answer : " , 0 , MAG , BOLD);
        std::getline(std::cin , answer);
        cout << endl;

        if ((answer == "Y")||(answer == "y"))          /* Attempt to upgrade the greencard */
          upgradeGreenCard(player, card);

        cout << player->getUserName();
        printF ("'s Remaining money: " , 0 , YEL , BOLD);
        cout << player->getCurrMoney() << endl;

        /* Remove Card from player's hand */
        it = player->getHand()->erase(it);
        continue;
      }
      it++;
    }
  }
  printF ("Equipment Phase Ended !" , 1 , GRN , FILL);
}
/* ========================================================================= */