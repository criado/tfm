#ifndef PRISMATOID
#define PRISMATOID     // Decaf ninja style. Bit ninjustu but with class.
                       // My humble interpretation of what SimplicialComplex.hpp
#include <map>         // should be.
#include <vector>      //
#include <algorithm>   // Author: Francisco "Paco" Criado
#include <iostream>    //
#include <fstream>     // I think my coding style is evolving into something
#include <iomanip>     // more readable after this.
#include <queue>       // Brief reminder: A simplex has dim vertices.
#include <ctime>
#include <chrono>
#include <random>
#include <cstdlib>
#include <set>
#include <string>
#include <bitset>
#include <cassert>
#include <cmath>
#include <cstring>

#define N 14
#define LAYER2 ((1<<N)-1)
#define LAYER1 (((1<<N)-1)<<N)

//PLAN_A is minimizing the facets
//PLAN_B is minimizing the geometric mean of card(ustar(v))
//       (Actually it just uses the product)
//PLAN_C is minimizing the ustar of the smallest vertex
//PLAN_D minimizes the number of faces
//PLAN_E is generalized mean (k=3)
//PLAN_Z should be starting with crosspolytope

//#define DEBUG
#define PLAN_E

using namespace std;

typedef unsigned int mask;
typedef pair<int, long long unsigned> il;
typedef vector<int> vi;
typedef default_random_engine rng;

class flip { public:               // f: face to remove
  mask f,l,v;                      // l: maximal face to add
};                                 // v: apex of the cone (frontier flip)
class prismatoid { public:
  //////////////////////////////////////////////////////////////////////////////
  // Public stuff (that should be private)
  //////////////////////////////////////////////////////////////////////////////

  mask base1, base2;               // The actual vertices in each base.
  int dim;                         // A facet has dim vertices
  int numFacets;                   // Number of facets.
  bool changeBases=false;          // Can we add/remove vertices?

  map<mask,mask> SC;               // Face and ustar of face
  map<mask, il> dists;             // Pair <distance, width> of each facet
  il distBase2;                    // (distance,width) for base2
  set<mask> adyBase2;              // The set of the ridges adyacent to base2

  map<mask,int> options;           // Set of ustars of ridges. That's it.
                                   // Frontier flips have dim vertices.
                                   // The int represents reference counting.

  //////////////////////////////////////////////////////////////////////////////
  // Public methods
  //////////////////////////////////////////////////////////////////////////////

  // S1: Constructors and IO
  prismatoid(int _dim);            // Crosspolytope
  prismatoid(istream& input, rng& gen);// Reads prismatoid from file
  void write(ostream& output);     // Writes prismatoid to file

  // S2: Flippin' magic
  flip execFlip(rng& gen);         // These two update everything.
  void execFlip(flip fl);          // The first chooses flip at random.

  // S3: Costs and graph stuff
  double cost();                   // Cost of this prismatoid (various options).
  bool feasible();                 // Do we want this prismatoid?
  pair<vi,vi> statsForSantos();    // f-vector and layers

  // S4: Dont panic
  bool everythingIsOK();           // Is everything OK?

  private:
  //////////////////////////////////////////////////////////////////////////////
  // Private stuff
  //////////////////////////////////////////////////////////////////////////////

  void cascadeFacets();            // Completes the construction from the facets

  void initOptions();              // Inits the options list
  void initGraph();                // Inits graph and dists
  void updateDists(queue<mask>& q);// Updates dists by cascading

  flip findFlip(rng& gen);         // Finds a flip or crashes tryin'
  bool checkFlip(mask u, flip& fl);// Checks the validity of u as option,
                                   //  returns the flip fl (by reference)
};

////////////////////////////////////////////////////////////////////////////////
// S0: Ancient bit-jutsu techniques
////////////////////////////////////////////////////////////////////////////////
inline uint countBits(mask i) {
  i= i-((i>>1)&0x55555555); i= (i&0x33333333)+((i>>2)&0x33333333);
  return (((i+(i>>4))&0x0F0F0F0F)*0x01010101) >> 24;
}

// for (mask x=firstElement(f); x!=0; x=nextElement(f,x))
inline mask firstElement(mask f) {return f&-f;}
inline void nextElement(mask f, mask& x) {x= ((x|~f)+x)&f;}

// mask x=0; do{ stuff } while(x=nextSubset(f,x), x!=0)
inline void  nextSubset(mask f, mask& x) {x= ((x|~f)+1)&f;}

inline void printMask(mask f) {
  for(int i=0; i<2*N; i++) if(((1<<i)&f)!=0) cout<<i<<" "; cout<<endl;
}
inline mask readMask() {
  string str; getline(cin, str); mask res=0;
  for(int i=0; i<str.size(); i++) res=2*res+((str[i]=='1')?1:0);
  return res;
}

inline bool in(mask a, mask b) {return !(a&(~b));}

#endif
