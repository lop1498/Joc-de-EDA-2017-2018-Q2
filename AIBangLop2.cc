#include "Player.hh"

/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */

#define PLAYER_NAME BangLop2


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

  static constexpr int I[8] = { 1, 1, 0, -1, -1, -1,  0,  1 };
  static constexpr int J[8] = { 0, 1, 1,  1,  0, -1, -1, -1 };
  
  typedef vector<int> VE;
  typedef vector<char> Row;
  typedef vector<Row> Graph;

			void recorre_mapa(Graph& G){
			  
				for(int i=0; i<G.size(); ++i){
					for(int j=0; j<G[0].size();++j) G[i][j] = what(i,j);
				}
			}
	
		Position bfs(const Graph &G, Data sold,vector<vector<Position> >& v,bool& b){ //troba la posició del poste ocupat o lliure més proper
			
			Position aux;
			Position fi;
			bool trobat = false;
			Graph visited(G.size(),Row(G[0].size(),'F'));
			Graph enqueued(G.size(),Row(G[0].size(),'F'));
			queue<Position> Q;
			aux.i = sold.pos.i;
			aux.j = sold.pos.j;
			Q.push(aux);
					
			while(not Q.empty() and not trobat){
				Position actual = Q.front();
				Q.pop();

				int x = actual.i;
				int y = actual.j;
				
				if (visited[x][y] == 'F'){
					
					visited[x][y] = 'T';
					if (post_owner(x,y) >= -1 and post_owner(x,y) != me() and pos_ok(x,y)){
						trobat = true;
						fi.i = x;
						fi.j = y;

					}
					else if (G[x][y] != 3 and G[x][y] != 4 and G[x][y] > -1){
						
						if (y > 0 and visited[x][y-1] == 'F' and which_soldier(x,y-1) == 0 and fire_time(x,y-1) == 0 and enqueued[x][y-1] == 'F') { // esquerra
							aux.i = x;
							aux.j = y-1;
							v[x][y-1] = actual;
							Q.push(aux);
							enqueued[x][y-1] = 'T';
						}
						
						if (y > 0 and x > 0 and visited[x-1][y-1] == 'F' and which_soldier(x-1,y-1) == 0 and fire_time(x-1,y-1) == 0 and enqueued[x-1][y-1]  == 'F') { //diagonal adalt a lesquerra
							aux.i = x-1;
							aux.j = y-1;
							v[x-1][y-1] = actual;
							Q.push(aux);
							enqueued[x-1][y-1] = 'T';
						}
						
						if (y < G[0].size() - 1 && x < G.size() -1 and visited[x+1][y+1] == 'F' and which_soldier(x+1,y+1) == 0 and fire_time(x+1,y+1) == 0 and enqueued[x+1][y+1]  == 'F') { //diagonal abaix a la dreta
							aux.i = x+1;
							aux.j = y+1;
							v[x+1][y+1] = actual;
							Q.push(aux);
							enqueued[x+1][y+1] = 'T';
						}
						
						if (y > 0 && x < G.size() -1 and visited[x+1][y-1] == 'F' and which_soldier(x+1,y-1) == 0 and fire_time(x+1,y-1) == 0 and enqueued[x+1][y-1]  == 'F') { //diagonal abaix a lesquerra
							aux.i = x+1;
							aux.j = y-1;
							v[x+1][y-1] = actual;
							Q.push(aux);
							enqueued[x+1][y-1] = 'T';
						}
						if (y < G[0].size() - 1 && x > 0 and visited[x-1][y+1] == 'F' and which_soldier(x-1,y+1) == 0 and fire_time(x-1,y+1) == 0 and enqueued[x-1][y+1]  == 'F') { //diagonal adalt a la dreta
							aux.i = x-1;
							aux.j = y+1;
							v[x-1][y+1] = actual;
							Q.push(aux);
							enqueued[x-1][y+1] = 'T';
						}
						
						if (y < G[0].size() - 1 && visited[x][y+1] == 'F' and which_soldier(x,y+1) == 0 and fire_time(x,y+1) == 0 and enqueued[x][y+1] == 'F') { //dreta
							aux.i = x;
							aux.j = y+1;
							v[x][y+1] = actual;
							Q.push(aux);
							enqueued[x][y+1] = 'T';
						}
						
						if (x > 0 && visited[x-1][y] == 'F' and which_soldier(x-1,y) == 0 and fire_time(x-1,y) == 0 and enqueued[x-1][y]  == 'F') { //amunt
							aux.i = x-1;
							aux.j = y;
							v[x-1][y] = actual;
							Q.push(aux);
							enqueued[x-1][y] = 'T';
						}
						
						if (x < G.size() - 1 && visited[x+1][y] == 'F' and which_soldier(x+1,y) == 0 and fire_time(x+1,y) == 0 and enqueued[x+1][y] == 'F') { //abaix
							aux.i = x+1;
							aux.j = y;
							v[x+1][y] = actual;
							Q.push(aux);
							enqueued[x+1][y] = 'T';
						}
					}
				}
			}
			
			b = trobat;
			return fi;
		}	  
	
		Position direccio(const vector<vector<Position> >& v, Position pos, Data id){
			
			int i = pos.i;
			int j = pos.j;

			while (v[i][j].i != id.pos.i or v[i][j].j != id.pos.j){
				int david = i;
				i = v[i][j].i;
				j = v[david][j].j;
			}

			Position ret;
			ret.i = i;
			ret.j = j;
			return ret;
		}
	
	  void play_soldier(int id, Graph& mapa) {
			
			Data in = data(id);
			int i = in.pos.i;
			int j = in.pos.j;
			int player = in.player;
			bool atac = false;
			bool poste = false;
			
			if (not poste){
				//cout << "entra al for del atac" << endl;
				for (int k = 0; k < 8; ++k) { // si te un enemic al costat l'ataca
					int ii = i + I[k];
					int jj = j + J[k];
					if (ii >= 0 and ii < MAX and jj >= 0 and jj < MAX) {
						int id2 = which_soldier(ii, jj);
						if (id2 and data(id2).player != me()) {
							atac = true;
							command_soldier(id, ii, jj);
						}
					}
				}
			}
			if (not atac and not poste){ // bfs
				bool trobat = false;
				Position sold = data(id).pos;
				vector<vector<Position> > v(60,vector<Position>(60));
				Position base;
				Position moviment;
				base = bfs(mapa,data(id),v,trobat);
		
				if (trobat and pos_ok(base.i,base.j)) {
					moviment = direccio(v,base,data(id));
					command_soldier(id,moviment.i,moviment.j);
				}
			}
		}
	
		Position direccio_h(const vector<vector<Position> >& v,Data id,Position pos){
			
			int i = pos.i;
			int j = pos.j;
			
			cout << i << " " << j << endl;
			cout << id.pos.i << " " << id.pos.j << endl;
			
			while (v[i][j].i != id.pos.i or v[i][j].j != id.pos.j){
				int david = i;
				i = v[i][j].i;
				j = v[david][j].j;
				//cout << i << " 1 " << j << endl;
				//cout << v[i][j].i << " 2 " << v[i][j].j << endl;
				//cout << v[v[i][j].i][v[i][j].j].i << " 3 " << v[v[i][j].i][v[i][j].j].i << endl;

			}
			

			Position ret;
			ret.i = i;
			ret.j = j;
			return ret;
		}
		
		void comm(int id, Position dir){
			
			Data hel = data(id);
			if (dir.i > hel.pos.i) {
				if (hel.orientation == 0) command_helicopter(id,FORWARD2);
				else command_helicopter(id,CLOCKWISE);
			}
			else if (dir.i < hel.pos.i) {
				if (hel.orientation == 2) command_helicopter(id,FORWARD2);
				else command_helicopter(id,CLOCKWISE);
			}
			else if (dir.j < hel.pos.j){
				 if (hel.orientation == 3) command_helicopter(id,FORWARD2);
				 else command_helicopter(id,CLOCKWISE);
			 } 
			else if (dir.j > hel.pos.j){
				 if (hel.orientation == 1) command_helicopter(id,FORWARD2);
				 else command_helicopter(id,CLOCKWISE);
			}
			
		}
		
		bool conta_soldats(Data id){
			
			int bons = 0;
			int dolents = 0;
			
			int J1[25] = { -2,-1,0,1,2,-2,-1,0,1,2,-2,-1,0,1,2,-2,-1,0,1,2,-2,-1,0,1,2 };
			int I1[25] = { -2,-2,-2,-2,-2,-1,-1,-1,-1,-1,0,0,0,0,0,1,1,1,1,1,2,2,2,2,2};
			
			for (int k = 0; k < 25; ++k) { 
				int ii = id.pos.i + I1[k];
				int jj = id.pos.j + J1[k];
				if (ii >= 0 and ii < MAX and jj >= 0 and jj < MAX and which_soldier(ii,jj) > 0) {
					
					int p = which_soldier(ii,jj);
					if (data(p).player != id.player) ++dolents;
					else if (data(p).player == id.player) ++bons;
				}
			}
			
			if(dolents - bons >= 4) return true;
			else return false;
		}
		
		void play_helicopter(int id,Graph& G) {
			
			vector<vector<Position> > v(60,vector<Position>(60,'F'));
			bool b;
			Data hel = data(id);
			int x = hel.pos.i;
			int y = hel.pos.j;
			
			//if (data(id).napalm == 0 and conta_soldats(hel)) command_helicopter(id,NAPALM);
			
				Position pos = bfs_h(G,hel,v,b);
				
				if (b and (pos.i != hel.pos.i or pos.j != hel.pos.j)){
					Position dir = direccio_h(v,hel,pos);
					//cout << dir.i << " " << dir.j << endl;
					comm(id,dir);
				
			}
		}
		
		bool recorre(Graph& G,int x, int y, char mirar){
			
			int J1[25] = { -2,-1,0,1,2,-2,-1,0,1,2,-2,-1,0,1,2,-2,-1,0,1,2,-2,-1,0,1,2 };
			int I1[25] = { -2,-2,-2,-2,-2,-1,-1,-1,-1,-1,0,0,0,0,0,1,1,1,1,1,2,2,2,2,2};
			
			int cont = 0;
			for (int k = 0; k < 25; ++k) { 
				int ii = x + I1[k];
				int jj = y + J1[k];
				//if (mirar == 'T') cout << ii << " " << jj << " " << G[ii][jj] << endl;
				if (ii >= 0 and ii < MAX and jj >= 0 and jj < MAX) {
					if (G[ii][jj] == mirar) ++cont;	
				}
			}
			
			if (mirar == 4){
				if (cont == 0) return true;
				else return false;
			}
			if (mirar == -1 ){
				if(cont == 0) return true;
				else return false;
			}
			if (mirar == 'T'){
				//cout << "hi han unes " << cont << endl;
				if (cont < 25) return true;
				else return false;
			}
			if (mirar == 'F'){
				//cout << "hi han unes " << cont << endl;
				if (cont > 0) return true;
				else return false;
			}
			
			
		}
		
		bool recorre_w(Graph& G, int x, int y, Data id){
			
			int J1[25] = { -2,-1,0,1,2,-2,-1,0,1,2,-2,-1,0,1,2,-2,-1,0,1,2,-2,-1,0,1,2 };
			int I1[25] = { -2,-2,-2,-2,-2,-1,-1,-1,-1,-1,0,0,0,0,0,1,1,1,1,1,2,2,2,2,2};
			
			for (int k = 0; k < 25; ++k) { 
				int ii = x + I1[k];
				int jj = y + J1[k];
				if (ii >= 0 and ii < MAX and jj >= 0 and jj < MAX) {
					
					if (which_helicopter(ii,jj) > 0 and which_helicopter(ii,jj) != id.id) return false;	
				}
			}
			
			return true;
		}
		
		Position bfs_h(Graph &G, Data hel,vector<vector<Position> >& v,bool& b){ //troba la posició del poste ocupat o lliure més proper
		
			int J1[25] = { -2,-1,0,1,2,-2,-1,0,1,2,-2,-1,0,1,2,-2,-1,0,1,2,-2,-1,0,1,2 };
			int I1[25] = { -2,-2,-2,-2,-2,-1,-1,-1,-1,-1,0,0,0,0,0,1,1,1,1,1,2,2,2,2,2};
			
			//cout << "hola soc el helicopter " << hel.id << "i estic a la posicio " << hel.pos.i << " " << hel.pos.j << endl;
			
			Position aux;
			b = false;
			bool primer = true;
			Graph visited(G.size(),Row(G[0].size(),'F'));
			queue<Position> Q;
			aux.i = hel.pos.i;
			aux.j = hel.pos.j;
			Q.push(aux);
			
			while(not Q.empty() and not b){
				
				Position actual = Q.front();
				Q.pop();
				int x = actual.i;
				int y = actual.j;
				//cout << "el valor es " << recorre(visited,x,y,'F') << endl;
				
				if (recorre(visited,x,y,'F')){
					
					for (int k = 0; k < 25; ++k) { 
						int ii = x + I1[k];
						int jj = y + J1[k];
						visited[ii][jj] = 'T';
					}	
					
					//cout << "entro abans del else if" << endl;
					//cout << "la funcio recorre1 em dona valor " << recorre(G,x,y,4) << endl;
					//cout << "la funcio recorre2 em dona valor " << recorre(G,x,y,-1) << endl;
					//cout << recorre(G,x,y,4)<< endl;
					if (post_owner(x,y) != me() and post_value(x,y) > -1){
						b = true;
						Position fi;
						fi.i = x;
						fi.j = y;
						//cout << "HE TROBAT LA POSICIO I VAL " << fi.i << " " << fi.j << endl;
						return fi;

					}
					
					else if (recorre(G,x,y,4) and recorre_w(G,x,y,hel)){
						//cout << "ACTUAL VAL " << x << " " << y << endl;
						//cout << "entro en el else if" << endl;
						//cout << "el valor del recorre amb y-1 es " << recorre(G,x,y-1,'T') << endl;
						
						//cout << "el valor de recorre_w es " << recorre_w(G,x,y-1,hel) << endl;
						if (y > 3 and recorre(visited,x,y-1,'T') and recorre_w(G,x,y-1,hel) and which_helicopter(x,y-3) == 0){ // esquerra
							//cout << "entra al if i fa push" << endl;
							aux.i = x;
							aux.j = y-1;
							v[x][y-1] = actual;
							//cout << x << " if esquerra " << y-1 << endl;
							Q.push(aux);
						}
						if (y < G[0].size()-4 && recorre(visited,x,y+1,'T') and recorre_w(G,x,y+1,hel) and which_helicopter(x,y+3) == 0){ //dreta
							//cout << "entra al if2 i fa push" << endl;
							aux.i = x;
							aux.j = y+1;
							v[x][y+1] = actual;
							Q.push(aux);
							//cout << x << " if dreta " << y+1 << endl;
						}
						
						if (x > 3 && recorre(visited,x-1,y,'T') and recorre_w(G,x-1,y,hel) and which_helicopter(x-3,y) == 0) { //amunt
							//cout << "entra al if3 i fa push" << endl;
							aux.i = x-1;
							aux.j = y;
							v[x-1][y] = actual;
							Q.push(aux);
							//cout << x-1 << " if amunt " << y << endl;
						}
						
						if (x < G.size()-4 && recorre(visited,x+1,y,'T') and recorre_w(G,x+1,y,hel) and which_helicopter(x,y-3) == 0) { //abaix
							//cout << "entra al if4 i fa push" << endl;
							aux.i = x+1;
							aux.j = y;
							v[x+1][y] = actual;
							Q.push(aux);
							//cout << x+1 << " if avall " << y << endl;
						}
					}
				}
			}
		}
	
			
		  void throw_parachuter(int helicopter_id) {
			// We get the data of the helicopter...
			/*
			Data in = data(helicopter_id);
			int ii = in.pos.i;
			int jj = in.pos.j;
			  
			if (pos_ok(ii,jj) and (what(ii,jj) == 1 or what(ii,jj) == 2) and fire_time(ii,jj) == 0) command_parachuter(ii,jj);
			// We get the data of the helicopter...
			
			if (pos_ok(ii,jj) and (what(ii,jj) == 1 or what(ii,jj) == 2) and fire_time(ii,jj) == 0) command_parachuter(ii,jj);
			*/
			int J1[25] = { -2,-1,0,1,2,-2,-1,0,1,2,-2,-1,0,1,2,-2,-1,0,1,2,-2,-1,0,1,2 };
			int I1[25] = { -2,-2,-2,-2,-2,-1,-1,-1,-1,-1,0,0,0,0,0,1,1,1,1,1,2,2,2,2,2};
			Data in = data(helicopter_id);
			int x = in.pos.i;
			int y = in.pos.j;
			for (int k = 0; k < 25; ++k) { 
						int ii = x + I1[k];
						int jj = y + J1[k];
						if (pos_ok(ii,jj) and (what(ii,jj) == 1 or what(ii,jj) == 2) and fire_time(ii,jj) == 0 and which_soldier(ii,jj) == 0) command_parachuter(ii,jj);
			}	
		}
		
					
		  /**
		   * Play method, invoked once per each round.
		   */
		  virtual void play () {

			int player = me();
			VE H = helicopters(player); // helicopters of my player
			VE S = soldiers(player); //    soldiers of my player
			Row columnes(60);
			Graph mapa(60,columnes);
			recorre_mapa(mapa);
			int k = 0;
			 for (int i = 0; i < (int)H.size(); ++i){
				 if (conta_soldats(data(H[i])) and data(H[i]).napalm == 0) command_helicopter(H[i],NAPALM);
				 else{
					 play_helicopter(H[i],mapa);
					 while (data(H[i]).parachuters.size() > 0 and k<4){
						  throw_parachuter(H[i]);
						  ++k;
					}
				}
			 }
			for (int i = 0; i < (int)S.size(); ++i) play_soldier(S[i],mapa);
			
			
		  }

		};

		constexpr int PLAYER_NAME::I[8];
		constexpr int PLAYER_NAME::J[8];

		/**
		 * Do not modify the following line.
		 */
		RegisterPlayer(PLAYER_NAME);

