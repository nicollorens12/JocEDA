#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME NicoAI

typedef vector<vector<bool>> Taulerbools;

struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }


  void visitat(Taulerbools& tauler, Pos p){

  }


  Pos BFS_Boost(Pos pos_ant){
    Taulerbools Tauler_visited(board_rows(),vector<bool>(board_cols(),false));
    queue <pair<pair<Pos,Dir>,int>> cua; //Pos Dir Dist
    int min_dist_esq, min_dist_dret, min_dist_adalt, min_dist_abaix;
    Dir d = Right;
    Pos aux_pos = pos_ant + d;
    if(pos_ok(aux_pos)){
      Cell c(aux_pos);
      if(c.bonus != None) return aux_pos;

      else if(c.type == Water) Tauler_visited[aux_pos.i][aux_pos.j] = true;
      else if(c.type == Soil){
          if(c.id == -1) cua.push(aux_pos);
          else{}
        }
    }
    aux_pos = pos_ant;
    d = Down;
    if(pos_ok(aux_pos)){
        Cell c(aux_pos);
        if(c.type == Soil and c.id == -1){
          cua.push(aux_pos);
        }
    }
    
    
    
    
  }

  /**Taulerbools Tauler_visited(board_rows(),vector<bool>(board_cols(),false));
   * Searches for the nearset enemies  position to the position pos_ant
   */
  Pos BFS_Enemies(Pos pos_ant){}
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
    }
  }

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
