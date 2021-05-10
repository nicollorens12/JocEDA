#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME AInico

typedef vector<vector<bool>> Taulerbools;
typedef pair<pair<Pos,Dir>,int> element_cua;

pair<int,Dir> min_dist;

struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }

  


  /**Taulerbools Tauler_visited(board_rows(),vector<bool>(board_cols(),false));
   * Searches for the nearset enemies  position to the position pos_ant
   * 
   * 
   * ENEMY SEARCH BELOW
   */



  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
      // Round 0 move right
    if (round() == 0) {
      return;
    }
     //Si round es < 8 es imposible que ens trobem amb alguna formiga, prioritzem els boosts per les workers
    else{
    
      vector <pair<pair<int,Pos>,Dir>> resultats;
      vector<int> vec_workers = workers(me());
      int sizew = vec_workers.size();
      for(int i = 0; i < sizew; ++i){  
        min_dist.first = -1;
        Ant formiga_w = ant(vec_workers[i]);
        Dir d = BFS_Boost(formiga_w.pos);
        
        if(min_dist.first != -1){
          pair<pair<int,Pos>,Dir> aux;
          aux.first.first = formiga_w.id;
          if(d == Left) aux.first.second = Pos(formiga_w.pos.i - 1, formiga_w.pos.i);
          else if(d == Down) aux.first.second = Pos(formiga_w.pos.i , formiga_w.pos.i - 1);
          else if(d == Right) aux.first.second = Pos(formiga_w.pos.i + 1, formiga_w.pos.i);
          else if(d == Up) aux.first.second = Pos(formiga_w.pos.i, formiga_w.pos.i + 1);
          aux.second = d;
          resultats.push_back(aux);
          //if(formiga_w.bonus != None) //Busca a la reina
        }
      }
      int vec_size = resultats.size();
      for(int i = 0; i < vec_size; ++i){
        for(int j = i +1; j < vec_size; ++j){
          if(resultats[i].first.second.i == resultats[j].first.second.i and resultats[i].first.second.j == resultats[j].first.second.j) resultats[j].first.first = -2;
        }
      }
      for(int k = 0; k < vec_size; ++k){
        if(resultats[k].first.first > -1) move(resultats[k].first.first, resultats[k].second);
      }

      vector<int>vec_soldiers = soldiers(me());
      int sizes = vec_soldiers.size();
      for(int j = 0; j < sizes; ++j){
        min_dist.first = -1;
        Ant formiga_s = ant(vec_soldiers[j]);
        Dir d = BFS_Enemies(formiga_s.pos);
        if(min_dist.first != -1){
          move(formiga_s.id,BFS_Enemies(formiga_s.pos));
        }    
      }
    }
  }

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
