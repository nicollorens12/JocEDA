#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME NicoTheAntv3

typedef vector<vector<bool>> Taulerbools;
//typedef pair<pair<Pos,Dir>,int> element_cua;

pair<int,Dir> min_dist;
Taulerbools tauler (25,vector<bool>(25,false));
set<Pos> proximes_pos;
map<int,int> workers_deixan; //id rondas;
int min_queen_dist_boost;
struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }

   struct element {
    Pos pos;
    Dir dir;
    int dist;

    element(Pos p, Dir di, int ds) {
        pos = p;
        dir = di;
        dist = ds;
    }
};

  Dir opposite_dir(Dir d, Pos p,bool move){
    if(d == Up){
      if(cell(p+Down).id == -1){
        move = true;
        return Down;
      } 
      else if(cell(p+Left).id == -1){
        move = true;
        return Left;
      }
      else if(cell(p+Right).id == -1){
        move = true;
        return Right;
      } 
      return Left;
    }

    else if(d == Right){
      if(cell(p+Left).id == -1){
        move = true;
        return Left;
      } 
      else if(cell(p+Up).id == -1){
        move = true;
        return Up;
      }
      else if(cell(p+Down).id == -1){
        move = true;
        return Down;
      } 
      return Left;
    }  
    else if(d == Left){
      if(cell(p+Right).id == -1){
        move = true;
        return Right;
      } 
      else if(cell(p+Up).id == -1){
        move = true;
        return Up;
      }
      else if(cell(p+Down).id == -1){
        move = true;
        return Down;
      } 
      return Left;
    } 
    else if(d == Down){
      if(cell(p+Up).id == -1){
        move = true;
        return Up;
      } 
      else if(cell(p+Right).id == -1){
        move = true;
        return Right;
      }
      else if(cell(p+Left).id == -1){
        move = true;
        return Left;
      } 
      return Left;
    } 
    return Left;
  }

  bool around_enemy_queen(Pos p){
    if(pos_ok(p+Up) and cell(p+Up).id != -1 and ant(cell(p+Up).id).type == Queen and ant( cell(p + Up).id ).player != me() ) return true;
    else if(pos_ok(p+Right) and cell(p+Right).id != -1 and ant(cell(p+Right).id).type == Queen and ant( cell(p + Right).id ).player != me()) return true;
    else if(pos_ok(p+Down) and cell(p+Down).id != -1 and ant(cell(p+Down).id).type == Queen and ant( cell(p + Down).id ).player != me() ) return true;
    else if(pos_ok(p+Left) and cell(p+Left).id != -1 and ant(cell(p+Left).id).type == Queen and ant( cell(p + Left).id ).player != me()) return true;
    return false;
  }

  bool around_enemy_soldier(Pos p){
    if(pos_ok(p+Up) and cell(p+Up).id != -1 and ant(cell(p+Up).id).type == Soldier and ant( cell(p + Up).id ).player != me() ) return true;
    else if(pos_ok(p+Right) and cell(p+Right).id != -1 and ant(cell(p+Right).id).type == Soldier and ant( cell(p + Right).id ).player != me()) return true;
    else if(pos_ok(p+Down) and cell(p+Down).id != -1 and ant(cell(p+Down).id).type == Soldier and ant( cell(p + Down).id ).player != me() ) return true;
    else if(pos_ok(p+Left) and cell(p+Left).id != -1 and ant(cell(p+Left).id).type == Soldier and ant( cell(p + Left).id ).player != me()) return true;
    return false;
  }

  Dir free_space(Pos p, bool& found){ //Returns dir to free space
      if(cell(p+Up).id == -1){
        found = true;
        return Up;
      } 
      else if(cell(p+Right).id == -1){
        found = true;
        return Right;
      }
      else if(cell(p+Down).id == -1){
        found = true;
        return Down;
      }
      else if(cell(p+Left).id == -1){
        found = true;
        return Left;
      }
      found = false;
      return Left;   
  }

  bool tengo_reservas(AntType t,int id){
    if(t == Worker){
      if(ant(id).reserve[0] > worker_carbo() and ant(id).reserve[0] > worker_prote() and ant(id).reserve[0] > worker_lipid()) return true;
      return false;
    }
    else{
      if(ant(id).reserve[0] > soldier_carbo() and ant(id).reserve[0] > soldier_prote() and ant(id).reserve[0] > soldier_lipid()) return true;
      return false;
    }
    return false;
  }

  bool next2queen(Pos p){
    if(pos_ok(p+Up) and cell(p+Up).id != -1 and ant(cell(p+Up).id).type == Queen and ant( cell(p + Up).id ).player == me() ) return true;
    else if(pos_ok(p+Right) and cell(p+Right).id != -1 and ant(cell(p+Right).id).type == Queen and ant( cell(p + Right).id ).player == me()) return true;
    else if(pos_ok(p+Down) and cell(p+Down).id != -1 and ant(cell(p+Down).id).type == Queen and ant( cell(p + Down).id ).player == me() ) return true;
    else if(pos_ok(p+Left) and cell(p+Left).id != -1 and ant(cell(p+Left).id).type == Queen and ant( cell(p + Left).id ).player == me()) return true;
    return false;
  }

  /**BFS for soldiers and its aux func for searhing enemy worker ants
   * Searches for the nearset enemies  position to the position pos_ant
   * 
   * 
   * SOLDIER SEARCH BELOW
   */


  bool trobat_soldiers(Cell c){
    if(c.id != -1 and ant(c.id).type == Soldier and ant(c.id).player != me())   return true;
    return false;
  }

  bool valid_soldiers(Cell c, Pos p) {
    if(c.type != Water and tauler[p.i][p.j] == false and c.id == -1) return true;
    if(c.id != -1 and ant(c.id).player != me() and ant(c.id).type != Queen and tauler[p.i][p.j] == false) return true; //Si no es water si no esta visitado y si en el caso de que haya una hormiga no sea mia
    return false;
  }

  int BFS_Queen_Enemies(Pos p, Dir& d) {
    tauler = Taulerbools (25,vector<bool>(25,false));
    queue<element> cua;
    cua.push(element(Pos(p + Down) , Down   , 1));
    cua.push(element(Pos(p + Up) ,   Up     , 1));
    cua.push(element(Pos(p + Right), Right  , 1));
    cua.push(element(Pos(p + Left),  Left   , 1));

    while (not cua.empty()) {
        if (pos_ok(cua.front().pos) and valid_soldiers( cell(cua.front().pos), cua.front().pos) ) {
            if ( trobat_soldiers(cell(cua.front().pos)) ) {
                d = cua.front().dir;
                return cua.front().dist;
            }
            else {
                cua.push( element(Pos(cua.front().pos + Down),     cua.front().dir, cua.front().dist + 1));
                cua.push( element(Pos(cua.front().pos + Up),       cua.front().dir, cua.front().dist + 1));
                cua.push( element(Pos(cua.front().pos + Right),    cua.front().dir, cua.front().dist + 1));
                cua.push( element(Pos(cua.front().pos + Left),     cua.front().dir, cua.front().dist + 1));
            }
            tauler[cua.front().pos.i] [cua.front().pos.j] = true;
        }
        
        cua.pop();
    }

    return -1;
}




  /**BFS for queens boosts
   * Searches for the nearset boost position to the quuens pos returns dist
   * 
   * 
   * QUEEN SEARCH BELOW
   */
  
  bool trobat_boost(Cell c, Pos p){
    if(c.bonus != None ) return true;
    return false;
  }

  bool valid_boost(Cell c, Pos p) {
    if(c.type != Water and c.id == -1 and tauler[p.i][p.j] == false) return true;
    if(c.type != Water and c.id != -1 and ant(c.id).type != Queen and ant(c.id).player != me() and tauler[p.i][p.j] == false and not around_enemy_queen(p)) return true;
    return false;
  }

  int BFS_Queen_Boost(Pos p, Dir& d) {
    tauler = Taulerbools (25,vector<bool>(25,false));
    queue<element> cua;
    cua.push(element(Pos(p.i + 1, p.j) , Down   , 1));
    cua.push(element(Pos(p.i - 1, p.j) , Up     , 1));
    cua.push(element(Pos(p.i , p.j + 1), Right  , 1));
    cua.push(element(Pos(p.i , p.j - 1), Left   , 1));

    while (not cua.empty()) {
        if (pos_ok(cua.front().pos) and valid_boost( cell(cua.front().pos), cua.front().pos) ) {
            if ( trobat_boost(cell(cua.front().pos), cua.front().pos) ) {
                d = cua.front().dir;
                return cua.front().dist;
            }
            else {
                cua.push( element(Pos(cua.front().pos.i + 1,    cua.front().pos.j),      cua.front().dir, cua.front().dist + 1));
                cua.push( element(Pos(cua.front().pos.i - 1,    cua.front().pos.j),      cua.front().dir, cua.front().dist + 1));
                cua.push( element(Pos(cua.front().pos.i ,       cua.front().pos.j + 1),  cua.front().dir, cua.front().dist + 1));
                cua.push( element(Pos(cua.front().pos.i ,       cua.front().pos.j - 1),  cua.front().dir, cua.front().dist + 1));
            }
            tauler[cua.front().pos.i] [cua.front().pos.j] = true;
        }
        
        cua.pop();
    }

    return -1;
}


    /**BFS for workers and its aux func
   * Searches for the path to queen   
   * 
   * QUEEN SEARCH BELOW
   */

  bool trobat_r(Pos p){
    
    if(pos_ok(p + Up) and cell(p+Up).id != -1 and ant( cell(p + Up).id).type == Queen and ant( cell(p + Up).id ).player == me() and cell(p).bonus == None ) return true;
    else if(pos_ok(p + Right) and cell(p+Right).id != -1 and ant(cell(p + Right).id).type == Queen and ant( cell(p + Right).id ).player == me() and cell(p).bonus == None ) return true;
    else if(pos_ok(p + Down) and cell(p+Down).id != -1 and  ant(cell(p + Down).id).type == Queen and ant( cell(p + Down).id ).player == me() and cell(p).bonus == None ) return true;
    else if(pos_ok(p + Left) and cell(p+Left).id != -1 and  ant(cell(p + Left).id).type == Queen and ant( cell(p + Left).id ).player == me() and cell(p).bonus == None ) return true;
    return false;
  }

  bool valid_r(Cell c, Pos p) {
    if(c.type == Soil and c.id == -1 and tauler[p.i][p.j] == false and not around_enemy_soldier(p)) return true;
    return false;
  }

  Dir BFS_Reina(Pos p, bool& found) {
    tauler = Taulerbools (25,vector<bool>(25,false));
    queue<element> cua;
    cua.push(element(Pos(p.i + 1, p.j) , Down   , 1));
    cua.push(element(Pos(p.i - 1, p.j) , Up     , 1));
    cua.push(element(Pos(p.i , p.j + 1), Right  , 1));
    cua.push(element(Pos(p.i , p.j - 1), Left   , 1));

    while (not cua.empty()) {
        if (pos_ok(cua.front().pos) and valid_r( cell(cua.front().pos), cua.front().pos) ) {
    
            if ( trobat_r(cua.front().pos) ) {
                found = true;
                return cua.front().dir;
            }
            else {
                cua.push( element(Pos(cua.front().pos.i + 1,    cua.front().pos.j),      cua.front().dir, cua.front().dist + 1));
                cua.push( element(Pos(cua.front().pos.i - 1,    cua.front().pos.j),      cua.front().dir, cua.front().dist + 1));
                cua.push( element(Pos(cua.front().pos.i ,       cua.front().pos.j + 1),  cua.front().dir, cua.front().dist + 1));
                cua.push( element(Pos(cua.front().pos.i ,       cua.front().pos.j - 1),  cua.front().dir, cua.front().dist + 1));
            }
            tauler[cua.front().pos.i] [cua.front().pos.j] = true;
        }
        
        cua.pop();
    }

    return Left;
}




  /**BFS for workers and its aux func
   * Searches for the nearset boost position to the position pos_ant
   * 
   * 
   * WORKER SEARCH BELOW
   */


  Pos pos_mod(Pos p, Dir d){
    if(d == Left) return Pos(p.i,p.j-1);
    else if(d == Up) return Pos(p.i-1,p.j);
    else if(d == Right) return Pos(p.i,p.j+1);
    else return Pos(p.i+1,p.j);
  }

  bool trobat_w(Cell c, Pos p){ //not next2queen(p)
    if(c.bonus != None and not next2queen(p)) return true;
    return false;
  }

  bool valid_w(Cell c, Pos p) {
    if(c.type != Water and c.id == -1 and tauler[p.i][p.j] == false and not around_enemy_soldier(p)) return true;
    return false;
  }

  Dir BFS_Boost(Pos p, bool& found) {
    tauler = Taulerbools (25,vector<bool>(25,false));
    queue<element> cua;
    cua.push(element(Pos(p.i + 1, p.j) , Down   , 1));
    cua.push(element(Pos(p.i - 1, p.j) , Up     , 1));
    cua.push(element(Pos(p.i , p.j + 1), Right  , 1));
    cua.push(element(Pos(p.i , p.j - 1), Left   , 1));

    while (not cua.empty()) {
      //cerr << p.i << " " << p.j << endl;
        if (pos_ok(cua.front().pos) and valid_w( cell(cua.front().pos), cua.front().pos) ) {
            if ( trobat_w(cell(cua.front().pos), cua.front().pos) ) {
                found = true;
                return cua.front().dir;
            }
            else {
                cua.push( element(Pos(cua.front().pos.i + 1,    cua.front().pos.j),      cua.front().dir, cua.front().dist + 1));
                cua.push( element(Pos(cua.front().pos.i - 1,    cua.front().pos.j),      cua.front().dir, cua.front().dist + 1));
                cua.push( element(Pos(cua.front().pos.i ,       cua.front().pos.j + 1),  cua.front().dir, cua.front().dist + 1));
                cua.push( element(Pos(cua.front().pos.i ,       cua.front().pos.j - 1),  cua.front().dir, cua.front().dist + 1));
            }
            tauler[cua.front().pos.i] [cua.front().pos.j] = true;
        }
        
        cua.pop();
    }

    return Left;
}

/**BFS for soldiers and its aux func for searhing enemy worker ants
   * Searches for the nearset enemies  position to the position pos_ant
   * 
   * 
   * SOLDIER SEARCH BELOW
   */


  bool trobat_s(Cell c){
    if(c.id != -1 and ant(c.id).type == Worker and ant(c.id).player != me())   return true;
    return false;
  }

  bool valid_s(Cell c, Pos p) {
    if(c.type != Water and tauler[p.i][p.j] == false and c.id == -1) return true;
    if(c.id != -1 and ant(c.id).player != me() and ant(c.id).type == Worker and tauler[p.i][p.j] == false) return true; //Si no es water si no esta visitado y si en el caso de que haya una hormiga no sea mia
    return false;
  }

  Dir BFS_Enemies(Pos p, bool& found) {
    tauler = Taulerbools (25,vector<bool>(25,false));
    queue<element> cua;
    cua.push(element(Pos(p.i + 1, p.j) , Down   , 1));
    cua.push(element(Pos(p.i - 1, p.j) , Up     , 1));
    cua.push(element(Pos(p.i , p.j + 1), Right  , 1));
    cua.push(element(Pos(p.i , p.j - 1), Left   , 1));

    while (not cua.empty()) {
        if (pos_ok(cua.front().pos) and valid_s( cell(cua.front().pos), cua.front().pos) ) {
            if ( trobat_s(cell(cua.front().pos)) ) {
                found = true;
                return cua.front().dir;
            }
            else {
                cua.push( element(Pos(cua.front().pos.i + 1,    cua.front().pos.j),      cua.front().dir, cua.front().dist + 1));
                cua.push( element(Pos(cua.front().pos.i - 1,    cua.front().pos.j),      cua.front().dir, cua.front().dist + 1));
                cua.push( element(Pos(cua.front().pos.i ,       cua.front().pos.j + 1),  cua.front().dir, cua.front().dist + 1));
                cua.push( element(Pos(cua.front().pos.i ,       cua.front().pos.j - 1),  cua.front().dir, cua.front().dist + 1));
            }
            tauler[cua.front().pos.i] [cua.front().pos.j] = true;
        }
        
        cua.pop();
    }

    return Left;
}




  /**
   * Play method, invoked once per each round.
   */

   /** COSAS PENDIENTES DE RESOLVER
    * Una worker con boost se ha comido otra worker de mi equipo que estaba parada a su lado (Ronda 8 turn 2 de la ROUND 1)
   */

  virtual void play () {
    if (round() == 0) {
      return;
    }
     
    else{ 
      proximes_pos.clear();
      vector<int> reina = queens(me());
      Pos pos_queen = ant(reina[0]).pos; 
      tauler = Taulerbools (25,vector<bool>(25,false));

      if(round()%queen_period() == 0){
        Dir d_s;
        Dir d_b;
        int nearest_dist_s = BFS_Queen_Enemies(ant(reina[0]).pos,d_s);
        tauler = Taulerbools (25,vector<bool>(25,false));
        int nearest_dist_b = BFS_Queen_Boost(ant(reina[0]).pos,d_b);     

        if(nearest_dist_s < 4 and nearest_dist_s > -1){ //Primera prioridad matar a soldados
            proximes_pos.insert(ant(reina[0]).pos + d_s);
            move(reina[0],d_s); 
        }

        //Segunda prioridad, matar a workers
        
        else if(pos_ok(pos_queen + Up) and cell(pos_queen + Up).id != -1 and ant(cell(pos_queen + Up).id).player != me() and ant(cell(pos_queen + Up).id).type != Queen){
            proximes_pos.insert(pos_queen + Up);
            move(reina[0],Up);     
        } 
        else if(pos_ok(pos_queen + Right) and  cell(pos_queen + Right).id != -1 and ant(cell(pos_queen + Right).id).player != me() and ant(cell(pos_queen + Right).id).type != Queen){
            proximes_pos.insert(pos_queen + Right);
            move(reina[0],Right);     
        }
        else if(pos_ok(pos_queen + Down) and  cell(pos_queen + Down).id != -1 and ant(cell(pos_queen + Down).id).player != me() and ant(cell(pos_queen + Down).id).type != Queen){
            proximes_pos.insert(pos_queen + Down);
            move(reina[0],Down);     
        }
        else if(pos_ok(pos_queen + Left) and  cell(pos_queen + Left).id != -1 and ant(cell(pos_queen + Left).id).player != me() and ant(cell(pos_queen + Left).id).type != Queen){
            proximes_pos.insert(pos_queen + Left);
            move(reina[0],Left);     
        }

        //tercera prioridad, si tengo boosts a 2 de cerca ves a comerlos
        else if(nearest_dist_b < 3 and nearest_dist_b > -1 ){
            proximes_pos.insert(ant(reina[0]).pos + d_b);
            move(reina[0],d_b); 
        } 

        //cuarta prioridad, si no tengo workers ves a buscar boosts
        else if(workers(me()).size() == 0 and nearest_dist_b > -1){
            proximes_pos.insert(ant(reina[0]).pos+d_b);
            move(reina[0],d_b);     
        }

      }
      else{

        if(workers(me()).size() < 2){
         lay(reina[0],Dir(random(0,3)),Worker);
        }

        else if(soldiers(me()).size() == 0){
         lay(reina[0],Dir(random(0,3)),Soldier);
         }
        else{
         lay(reina[0],Dir(random(0,3)),Worker);
        }

      }
      
      vector<int> vec_workers = workers(me());
      int sizew = vec_workers.size();

      for(int i = 0; i < sizew; ++i){  
        
        tauler = Taulerbools (25,vector<bool>(25,false));
        Ant formiga_w = ant(vec_workers[i]);
       
        if(cell(formiga_w.pos).bonus != None and not next2queen(formiga_w.pos) and formiga_w.bonus == None) take(vec_workers[i]); //falla por esto
        
        bool found = false;
        Dir d;
        
        if(next2queen(formiga_w.pos) and formiga_w.bonus != None) leave(formiga_w.id);
    
        if(formiga_w.bonus != None) d = BFS_Reina(formiga_w.pos,found);
        else d = BFS_Boost(formiga_w.pos,found);
        
        bool move_it = false;
        if(not found){
          Dir aux;
          int dist = BFS_Queen_Enemies(formiga_w.pos,aux);
          if(dist < 2){         
            aux = opposite_dir(aux,formiga_w.pos,move_it);
            if(move_it) d = aux;
          }
        }
        
        if(found or move_it){
          Pos p;
          p = formiga_w.pos + d;
          set<Pos>::const_iterator it;
          it = proximes_pos.find(p);
          if(it == proximes_pos.end()) proximes_pos.insert(p);

          if(found and it == proximes_pos.end()){
          move(vec_workers[i],d);
          found = false;
          }
        }  
        
      }
      
      vector<int>vec_soldiers = soldiers(me());
      int sizes = vec_soldiers.size();

      for(int j = 0; j < sizes; ++j){
        tauler = Taulerbools (25,vector<bool>(25,false));
        Ant formiga_w = ant(vec_soldiers[j]);
        bool found = false;

        Dir d = BFS_Enemies(formiga_w.pos,found);
        set<Pos>::const_iterator it;
        it = proximes_pos.find(formiga_w.pos+d);
        if(it == proximes_pos.end()) proximes_pos.insert(formiga_w.pos+d);
        if(found and it == proximes_pos.end()){
          move(vec_soldiers[j],d);
          found = false;
        }
      }
      
      
    }
  }
};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
