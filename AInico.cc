#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME AInico

typedef vector<vector<bool>> Taulerbools;
//typedef pair<pair<Pos,Dir>,int> element_cua;

pair<int,Dir> min_dist;
Taulerbools tauler (25,vector<bool>(25,false));
set<Pos> proximes_pos;
map<int,int> workers_deixan; //id rondas;

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
   /**BFS for workers and its aux func
   * Searches for the nearset boost position to the position pos_ant
   * 
   * 
   * QUEEN SEARCH BELOW
   */
  bool next2queen(Pos p){
    if(pos_ok(p+Up) and cell(p+Up).id != -1 and ant(cell(p+Up).id).type == Queen) return true;
    else if(pos_ok(p+Right) and cell(p+Right).id != -1 and ant(cell(p+Right).id).type == Queen) return true;
    else if(pos_ok(p+Down) and cell(p+Down).id != -1 and ant(cell(p+Down).id).type == Queen) return true;
    else if(pos_ok(p+Left) and cell(p+Left).id != -1 and ant(cell(p+Left).id).type == Queen) return true;
    return false;
  }

  bool trobat_r(Pos p){
    
    if(pos_ok(p + Up) and cell(p+Up).id != -1 and ant( cell(p + Up).id ).type == Queen and ant( cell(p + Up).id ).player == me() ){
      return true;
    }
    else if(pos_ok(p + Right) and cell(p+Right).id != -1 and ant(cell(p + Right).id).type == Queen and ant( cell(p + Up).id ).player == me() ){
       return true;
    }
    else if(pos_ok(p + Down) and cell(p+Down).id != -1 and  ant(cell(p + Down).id).type == Queen and ant( cell(p + Up).id ).player == me()) return true;
    else if(pos_ok(p + Left) and cell(p+Left).id != -1 and  ant(cell(p + Left).id).type == Queen and ant( cell(p + Up).id ).player == me()) return true;
    return false;
  }

  bool valid_r(Cell c, Pos p) {
    if(c.type == Soil and c.id == -1 and tauler[p.i][p.j] == false) return true;
    return false;
  }

  Dir BFS_Reina(Pos p, bool& found) {
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

  bool trobat_w(Cell c, Pos p){
    if(c.bonus != None and not next2queen(p)) return true;
    return false;
  }

  bool valid_w(Cell c, Pos p) {
    if(c.type != Water and c.id == -1 and tauler[p.i][p.j] == false) return true;
    return false;
  }

  Dir BFS_Boost(Pos p, bool& found) {
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
    if(c.id != -1 and ant(c.id).type == Worker) return true;
    return false;
  }

  bool valid_s(Cell c, Pos p) {
    if(c.type != Water and tauler[p.i][p.j] == false and c.id == -1) return true;
    if(c.id != -1 and ant(c.id).player != me() and tauler[p.i][p.j] == false) return true; //Si no es water si no esta visitado y si en el caso de que haya una hormiga no sea mia
    return false;
  }

  Dir BFS_Enemies(Pos p, bool& found) {
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
   *  Las intentan dejar su boost al lado de la reina pero puede pasar que ya haya un boost, las haria moverse random si les pasa esto o que??
   *  La queen no hace lay aunque tenga reservas
   *  Mirar que hacen las workers si no hay boosts en el mapa, no queremos que una se quede parada al lado de la reina y esta no pueda coger el boost que le acaban de dejar
   * 
   */

  virtual void play () {
      // Round 0 move right
    if (round() == 0) {
      return;
    }
     
    else{
      proximes_pos.clear();
      //NO NOMES SI ID == -1 ja que ens interesa que mati la reina si cal a formigues enemigues
      if(round()%queen_period() == 0){
        vector<int> reina = queens(me());
        Pos pos_queen = ant(reina[0]).pos;
        if(tengo_reservas(Worker,reina[0])) lay(reina[0],Dir(random(0, 3)),Worker); //Habra que crear random tambien soldiers

        
         //menjar tot menys altres reines PRIORIZAMOS COMER HORMIGAS QUE BOOSTS
        if( cell(pos_queen + Up).id != -1 and ant(cell(pos_queen + Up).id).player != me() and ant(cell(pos_queen + Up).id).type != Queen){
            proximes_pos.insert(pos_queen + Up);
            move(reina[0],Up);     
        } 
        else if( cell(pos_queen + Right).id != -1 and ant(cell(pos_queen + Right).id).player != me() and ant(cell(pos_queen + Right).id).type != Queen){
            proximes_pos.insert(pos_queen + Right);
            move(reina[0],Right);     
        }
        else if( cell(pos_queen + Down).id != -1 and ant(cell(pos_queen + Down).id).player != me() and ant(cell(pos_queen + Down).id).type != Queen){
            proximes_pos.insert(pos_queen + Down);
            move(reina[0],Down);     
        }
        else if( cell(pos_queen + Left).id != -1 and ant(cell(pos_queen + Left).id).player != me() and ant(cell(pos_queen + Left).id).type != Queen){
            proximes_pos.insert(pos_queen + Left);
            move(reina[0],Left);     
        }

        /*BOOSTS Y PREVENCION DE SI HAY BOOST PERO TAMBIEN UNA HORMIGA ENEMIGA ME LA COMO*/
        else if( cell(pos_queen + Up).bonus != None){
          //if(cell(pos_queen + Up).id != -1 and ant(cell(pos_queen + Up).id).player != me())
          if(not (cell(pos_queen + Up).id != -1 and ant(cell(pos_queen + Up).id).player == me())){
            proximes_pos.insert(pos_queen + Up);
            move(reina[0],Up);
          }  
        } 
        else if( cell(pos_queen + Right).bonus != None){
          if(not (cell(pos_queen + Right).id != -1 and ant(cell(pos_queen + Right).id).player == me())){
            proximes_pos.insert(pos_queen + Right);
            move(reina[0],Right);
          }
        } 
        else if( cell(pos_queen + Down).bonus != None){
          if(not (cell(pos_queen + Down).id != -1 and ant(cell(pos_queen + Down).id).player == me())){
            proximes_pos.insert(pos_queen + Down);
            move(reina[0],Down);
          }
        } 
        else if( cell(pos_queen + Left).bonus != None){
          if(not (cell(pos_queen + Left).id != -1 and ant(cell(pos_queen + Left).id).player == me())){
            proximes_pos.insert(pos_queen + Left);
            move(reina[0],Left);
          }
        }

       

        //MIRAR RESERVAS I ON PUC FER LAY 

      }
      
      //QUEEN NO HA DE MOURES SI LAS WORKERS ENCARA NO SHAN POGUT MOURE I NO PODEN ANARSE A DONARLI EL BOOST A LALTRE QUEEN
      vector<int> vec_workers = workers(me());
      int sizew = vec_workers.size();

      for(int i = 0; i < sizew; ++i){  
        
        tauler = Taulerbools (25,vector<bool>(25,false));
        Ant formiga_w = ant(vec_workers[i]);
       
        if(cell(formiga_w.pos).bonus != None and not next2queen(formiga_w.pos)) take(vec_workers[i]);
        
        bool found = false;
        Dir d;
        
        if(next2queen(formiga_w.pos) and formiga_w.bonus != None){
          pair<int,int> aux;
          aux.first = formiga_w.id;
          aux.second = 2;
          
          leave(formiga_w.id); 
        } 

        if(formiga_w.bonus != None) d = BFS_Reina(formiga_w.pos,found);
        else d = BFS_Boost(formiga_w.pos,found);

        Pos p = pos_mod(formiga_w.pos,d);
        set<Pos>::const_iterator it;
        it = proximes_pos.find(p);
        if(it == proximes_pos.end()) proximes_pos.insert(p);
        if(found and it == proximes_pos.end()){
          move(vec_workers[i],d);
          found = false;
        }
        
      }
      
      vector<int>vec_soldiers = soldiers(me());
      int sizes = vec_soldiers.size();

      for(int j = 0; j < sizes; ++j){
        cerr << "ENTRA EN EL BFS ENEMIES" << endl;
        tauler = Taulerbools (25,vector<bool>(25,false));
        Ant formiga_w = ant(vec_soldiers[j]);
        bool found = false;

        Dir d = BFS_Enemies(formiga_w.pos,found);
        cerr << formiga_w.id << " " << d << endl;
        Pos p = pos_mod(formiga_w.pos,d);
        set<Pos>::const_iterator it;
        it = proximes_pos.find(p);
        if(it == proximes_pos.end()) proximes_pos.insert(p);
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
