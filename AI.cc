#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Nico

// DISCLAIMER: The following Demo player is *not* meant to do anything
// sensible. It is provided just to illustrate how to use the API.
// Please use AINull.cc as a template for your player.

struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }


  /**
   * Types and attributes for your player can be defined here.
   */

  const vector<NutrientType> nutrients = { Carbohydrate, Protein, Lipid };


  // Default direction to be used during all match.
  Dir default_dir;
  

  // Returns true if winning.
  bool winning() {
    for (int pl = 0; pl < num_players(); ++pl)
      if (pl != me() and score(me()) <= score(pl))
        return false;
    return true;
  }
  /**
   * Searches for the nearset boost position to the position pos_ant
   */
  Pos BFS_Boost(Pos pos_ant){

  }

  /**
   * Searches for the nearset enemies  position to the position pos_ant
   */
  Pos BFS_Enemies(Pos pos_ant){

  }
  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {

    // Round 0 ants wait for boosts to spawn
    if (round() == 0) {
      return;
    }
     //Si round es < 8 es imposible que ens trobem amb alguna formiga, prioritzem els boosts per les workers
    vector<int> vec_workers = workers(me());
    int sizew = vec_workers.size();
    for(int i = 0; i < sizew; ++i){  
      Ant formiga_w = ant(vec_workers[i]);
      BFS_boost(formiga_w.pos);
      move(formiga_w.id,/*direccio*/);
    }

    vector<int>vec_soldiers = soldiers(me());
    int sizes = vec_soldiers.size();
    for(int j = 0; j < sizes; ++j){
      Ant formiga_s = ant(vec_soldiers[j]);
      BFS_Enemies(formiga_s.pos);
      move(formiga_s.id,/*direccio*/);
    }

    
    

    // Write debugging messages, e.g., info related to queen of player 1 (if it exists).
    vector<int> queen1_ids = queens(1);
    if (not queen1_ids.empty()) {
      Ant queen1 = ant(queen1_ids[0]);
      cerr << "Data of queen of player 1"    << endl;
      cerr << "Type     : " << queen1.type   << endl; // This should print 'Queen'!
      cerr << "Id       : " << queen1.id     << endl;
      cerr << "Player   : " << queen1.player << endl; // This should print '1'!
      cerr << "Position : " << queen1.pos    << endl;
      cerr << "Bonus    : " << queen1.bonus  << endl; // This should print 'None'!
      for (NutrientType n : nutrients)
        cerr << "Reserve of " << n << ": " << queen1.reserve[n] << endl;
    }

    // Write debugging messages, e.g., the matrix of id's.
    for (int i = 0; i < board_rows(); ++i) {
      for (int j = 0; j < board_cols(); ++j) {
        cerr << cell(i, j).id <<  "\t";
      }
      cerr << endl;
    }

 
};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);