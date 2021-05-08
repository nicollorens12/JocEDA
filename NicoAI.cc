#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME NicoAI

typedef vector<vector<bool>> Taulerbools;
typedef pair<pair<Pos,Dir>,int> element_cua;
struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }

  void comprobacio_w(Taulerbools& Tauler, queue<element_cua>& cua, element_cua elem){
    Cell c = cell(elem.first.first);
    if(c.type == Water) Tauler[elem.first.first.i][elem.first.first.j] = true;
    else if(c.type == Soil and Tauler[elem.first.first.i][elem.first.first.j] == false){
        if(c.id == -1){
          cua.push(elem);
          Tauler[elem.first.first.i][elem.first.first.j] = true;
        } 
        else Tauler[elem.first.first.i][elem.first.first.j] = true;
    }
    
  }

  void comprobacio_veins_w(Taulerbools& Tauler, queue<element_cua>& cua, element_cua element_inicial, int& esq, int& abaix, int& dreta, int& adalt){
    element_cua elem;

    elem.first.first = Pos(element_inicial.first.first.i - 1, element_inicial.first.first.j);
    elem.first.second = element_inicial.first.second;
    elem.second = element_inicial.second + 1;

    if(pos_ok(elem.first.first)){ //esquerra
      Cell c = cell(elem.first.first);
      if(c.bonus != None) esq = elem.second;
      else comprobacio_w(Tauler,cua,elem);
    }

    elem.first.first = Pos(element_inicial.first.first.i, element_inicial.first.first.j - 1); //abaix
    elem.first.second = element_inicial.first.second;
    elem.second = element_inicial.second + 1;

    if(pos_ok(elem.first.first)){ //abaix
      Cell c = cell(elem.first.first);
      if(c.bonus != None) abaix = elem.second;
      else comprobacio_w(Tauler,cua,elem);
    }

    elem.first.first = Pos(element_inicial.first.first.i + 1, element_inicial.first.first.j); //dreta
    elem.first.second = element_inicial.first.second;
    elem.second = element_inicial.second + 1;

    if(pos_ok(elem.first.first)){ //dreta
      Cell c = cell(elem.first.first);
      if(c.bonus != None) dreta = elem.second;
      else comprobacio_w(Tauler,cua,elem);
    }

    elem.first.first = Pos(element_inicial.first.first.i, element_inicial.first.first.j + 1); //adalt
    elem.first.second = element_inicial.first.second;
    elem.second = element_inicial.second + 1;

    if(pos_ok(elem.first.first)){ //adalt
      Cell c = cell(elem.first.first);
      if(c.bonus != None) adalt = elem.second;
      else comprobacio_w(Tauler,cua,elem);
    }
  }
  
  Dir BFS_Boost(Pos pos_ant){
    Taulerbools Tauler_visited(board_rows(),vector<bool>(board_cols(),false));
    queue <element_cua> cua; //Pos Dir Dist
    int min_dist_esq = 25*25, min_dist_dret = 25*25, min_dist_adalt = 25*25, min_dist_abaix = 25*25;

    element_cua elem;
    elem.first.first = Pos(pos_ant.i - 1, pos_ant.j);
    elem.first.second = Left;
    elem.second = 1;

    if(pos_ok(elem.first.first)){ //esquerra
      Cell c = cell(elem.first.first);
      if(c.bonus != None) min_dist_esq = elem.second;
      else comprobacio_w(Tauler_visited,cua,elem);
    }

    elem.first.first = Pos(pos_ant.i, pos_ant.j - 1); //abaix
    elem.first.second = Down;
    elem.second = 1;

    if(pos_ok(elem.first.first)){ //abaix
      Cell c = cell(elem.first.first);
      if(c.bonus != None) min_dist_abaix = elem.second;
      else comprobacio_w(Tauler_visited,cua,elem);
    }

    elem.first.first = Pos(pos_ant.i + 1, pos_ant.j); //dreta
    elem.first.second = Right;
    elem.second = 1;

    if(pos_ok(elem.first.first)){ //dreta
      Cell c = cell(elem.first.first);
      if(c.bonus != None) min_dist_dret = elem.second;
      else comprobacio_w(Tauler_visited,cua,elem);
    }

    elem.first.first = Pos(pos_ant.i, pos_ant.j + 1); //adalt
    elem.first.second = Up;
    elem.second = 1;

    if(pos_ok(elem.first.first)){ //adalt
      Cell c = cell(elem.first.first);
      if(c.bonus != None) min_dist_adalt = elem.second;
      else comprobacio_w(Tauler_visited,cua,elem);
    }

    while(not cua.empty()){
      element_cua aux = cua.front();
      comprobacio_veins_w(Tauler_visited,cua,aux,min_dist_esq,min_dist_abaix,min_dist_dret,min_dist_adalt);
      cua.pop();
    }
    
    if (min_dist_esq <= min_dist_abaix and min_dist_esq <= min_dist_dret and min_dist_esq <= min_dist_adalt) return Left;
    else if(min_dist_dret <= min_dist_abaix and min_dist_dret <= min_dist_esq and min_dist_dret <= min_dist_adalt) return Right;
    else if(min_dist_abaix <= min_dist_dret and min_dist_abaix <= min_dist_esq and min_dist_abaix <= min_dist_adalt) return Down;
    else return Up;
    
  }
  
  Dir BFS_Boost(Pos pos_ant){
    Taulerbools Tauler_visited(board_rows(),vector<bool>(board_cols(),false));
    queue <element_cua> cua; //Pos Dir Dist
    int min_dist_esq = 25*25, min_dist_dret = 25*25, min_dist_adalt = 25*25, min_dist_abaix = 25*25;

    element_cua elem;
    elem.first.first = Pos(pos_ant.i - 1, pos_ant.j);
    elem.first.second = Left;
    elem.second = 1;

    if(pos_ok(elem.first.first)){ //esquerra
      Cell c = cell(elem.first.first);
      if(c.bonus != None) min_dist_esq = elem.second;
      else comprobacio_w(Tauler_visited,cua,elem);
    }

    elem.first.first = Pos(pos_ant.i, pos_ant.j - 1); //abaix
    elem.first.second = Down;
    elem.second = 1;

    if(pos_ok(elem.first.first)){ //abaix
      Cell c = cell(elem.first.first);
      if(c.bonus != None) min_dist_abaix = elem.second;
      else comprobacio_w(Tauler_visited,cua,elem);
    }

    elem.first.first = Pos(pos_ant.i + 1, pos_ant.j); //dreta
    elem.first.second = Right;
    elem.second = 1;

    if(pos_ok(elem.first.first)){ //dreta
      Cell c = cell(elem.first.first);
      if(c.bonus != None) min_dist_dret = elem.second;
      else comprobacio_w(Tauler_visited,cua,elem);
    }

    elem.first.first = Pos(pos_ant.i, pos_ant.j + 1); //adalt
    elem.first.second = Up;
    elem.second = 1;

    if(pos_ok(elem.first.first)){ //adalt
      Cell c = cell(elem.first.first);
      if(c.bonus != None) min_dist_adalt = elem.second;
      else comprobacio_w(Tauler_visited,cua,elem);
    }

    while(not cua.empty()){
      element_cua aux = cua.front();
      comprobacio_veins_w(Tauler_visited,cua,aux,min_dist_esq,min_dist_abaix,min_dist_dret,min_dist_adalt);
      cua.pop();
    }
    
    if (min_dist_esq <= min_dist_abaix and min_dist_esq <= min_dist_dret and min_dist_esq <= min_dist_adalt) return Left;
    else if(min_dist_dret <= min_dist_abaix and min_dist_dret <= min_dist_esq and min_dist_dret <= min_dist_adalt) return Right;
    else if(min_dist_abaix <= min_dist_dret and min_dist_abaix <= min_dist_esq and min_dist_abaix <= min_dist_adalt) return Down;
    else return Up;
  }


  /**Taulerbools Tauler_visited(board_rows(),vector<bool>(board_cols(),false));
   * Searches for the nearset enemies  position to the position pos_ant
   * 
   * 
   * ENEMY SEARCH BELOW
   */

  void comprobacio_s(Taulerbools& Tauler, queue<element_cua>& cua, element_cua elem){
    Cell c = cell(elem.first.first);
    if(c.type == Water) Tauler[elem.first.first.i][elem.first.first.j] = true;
    else if(c.type == Soil and Tauler[elem.first.first.i][elem.first.first.j] == false){
      if(c.id == -1){
        cua.push(elem);
        Tauler[elem.first.first.i][elem.first.first.j] = true;
      } 
      else Tauler[elem.first.first.i][elem.first.first.j] = true;
    }  
  }

  void comprobacio_veins_s(Taulerbools& Tauler, queue<element_cua>& cua, element_cua element_inicial, int& esq, int& abaix, int& dreta, int& adalt){
    element_cua elem;

    elem.first.first = Pos(element_inicial.first.first.i - 1, element_inicial.first.first.j);
    elem.first.second = element_inicial.first.second;
    elem.second = element_inicial.second + 1;

    if(pos_ok(elem.first.first)){ //esquerra
      Cell c = cell(elem.first.first);
      Ant aux = ant(c.id);
      if(aux.player != me() and aux.type == Worker) esq = elem.second;
      else comprobacio_s(Tauler,cua,elem);
    }

    elem.first.first = Pos(element_inicial.first.first.i, element_inicial.first.first.j - 1); //abaix
    elem.first.second = element_inicial.first.second;
    elem.second = element_inicial.second + 1;

    if(pos_ok(elem.first.first)){ //abaix
      Cell c = cell(elem.first.first);
      Ant aux = ant(c.id);
      if(aux.player != me() and aux.type == Worker) abaix = elem.second;
      else comprobacio_s(Tauler,cua,elem);
    }

    elem.first.first = Pos(element_inicial.first.first.i + 1, element_inicial.first.first.j); //dreta
    elem.first.second = element_inicial.first.second;
    elem.second = element_inicial.second + 1;

    if(pos_ok(elem.first.first)){ //dreta
      Cell c = cell(elem.first.first);
      Ant aux = ant(c.id);
      if(aux.player != me() and aux.type == Worker) dreta = elem.second;
      else comprobacio_s(Tauler,cua,elem);
    }

    elem.first.first = Pos(element_inicial.first.first.i, element_inicial.first.first.j + 1); //adalt
    elem.first.second = element_inicial.first.second;
    elem.second = element_inicial.second + 1;

    if(pos_ok(elem.first.first)){ //adalt
      Cell c = cell(elem.first.first);
      Ant aux = ant(c.id);
      if(aux.player != me() and aux.type == Worker) adalt = elem.second;
      else comprobacio_s(Tauler,cua,elem);
    }
  }


  Dir BFS_Enemies(Pos pos_ant){
    Taulerbools Tauler_visited(board_rows(),vector<bool>(board_cols(),false));
    queue <element_cua> cua; //Pos Dir Dist
    int min_dist_esq = 25*25, min_dist_dret = 25*25, min_dist_adalt = 25*25, min_dist_abaix = 25*25;

    element_cua elem;
    elem.first.first = Pos(pos_ant.i - 1, pos_ant.j);
    elem.first.second = Left;
    elem.second = 1;

    if(pos_ok(elem.first.first)){ //esquerra
      Cell c = cell(elem.first.first);
      Ant aux = ant(c.id);
      if(aux.player != me() and aux.type == Worker) min_dist_esq = elem.second;
      else comprobacio_s(Tauler_visited,cua,elem);
    }

    elem.first.first = Pos(pos_ant.i, pos_ant.j - 1); //abaix
    elem.first.second = Down;
    elem.second = 1;

    if(pos_ok(elem.first.first)){ //abaix
      Cell c = cell(elem.first.first);
      Ant aux = ant(c.id);
      if(aux.player != me() and aux.type == Worker) min_dist_abaix = elem.second;
      else comprobacio_s(Tauler_visited,cua,elem);
    }

    elem.first.first = Pos(pos_ant.i + 1, pos_ant.j); //dreta
    elem.first.second = Right;
    elem.second = 1;

    if(pos_ok(elem.first.first)){ //dreta
      Cell c = cell(elem.first.first);
      Ant aux = ant(c.id);
      if(aux.player != me() and aux.type == Worker) min_dist_dret = elem.second;
      else comprobacio_s(Tauler_visited,cua,elem);
    }

    elem.first.first = Pos(pos_ant.i, pos_ant.j + 1); //adalt
    elem.first.second = Up;
    elem.second = 1;

    if(pos_ok(elem.first.first)){ //adalt
      Cell c = cell(elem.first.first);
      Ant aux = ant(c.id);
      if(aux.player != me() and aux.type == Worker) min_dist_adalt = elem.second;
      else comprobacio_s(Tauler_visited,cua,elem);
    }

    while(not cua.empty()){
      element_cua aux = cua.front();
      comprobacio_veins_s(Tauler_visited,cua,aux,min_dist_esq,min_dist_abaix,min_dist_dret,min_dist_adalt);
      cua.pop();
    }
    
    if (min_dist_esq <= min_dist_abaix and min_dist_esq <= min_dist_dret and min_dist_esq <= min_dist_adalt) return Left;
    else if(min_dist_dret <= min_dist_abaix and min_dist_dret <= min_dist_esq and min_dist_dret <= min_dist_adalt) return Right;
    else if(min_dist_abaix <= min_dist_dret and min_dist_abaix <= min_dist_esq and min_dist_abaix <= min_dist_adalt) return Down;
    else return Up;
  }
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
        move(formiga_w.id,BFS_Boost(formiga_w.pos));
      }

      vector<int>vec_soldiers = soldiers(me());
      int sizes = vec_soldiers.size();
      for(int j = 0; j < sizes; ++j){
        Ant formiga_s = ant(vec_soldiers[j]);
        move(formiga_s.id,BFS_Enemies(formiga_s.pos));
      }
    }
  }

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
