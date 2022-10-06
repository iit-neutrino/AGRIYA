#include <cassert>
#include <TGaxis.h>
#include <TH1.h>
#include <TColor.h>
#include <TStyle.h>
#include <TROOT.h>


/// @brief Setup the default style for all plotting
/// @param defaultStyle 
void setupStyle(TStyle* defaultStyle = nullptr);

/// We tried to be color blind friendly and used scheme suggested here: https://personal.sron.nl/~pault/
const Int_t l1 = 2001; 
const Int_t l2 = 2002; 
const Int_t l3 = 2003; 
const Int_t l4 = 2004; 
const Int_t l5 = 2005; 

const Int_t c1 = 2011;
const Int_t c2 = 2012;
const Int_t c3 = 2013;
const Int_t c4 = 2014;
const Int_t c5 = 2015;

const Int_t colors[] = {l1,l2,l3,l4,l5}; // #colors >= #levels - 1
const Int_t contColors[] = {c1,c2,c3,c4,c5}; // #colors >= #levels - 1