//---------------------------------
// Livemore Loops -- SLC (uTC)
// M.A.Hicks, CSA Group, UvA

// Implementation based of various reference implementations
// including the original FORTRAN but mostly from Roy Longbottom, 1996.

//---------------------------------
// Livermore Header File
// This file contains the various data structures used throughout
// the execution of the Livermore kernels.
//---------------------------------

// Loop number specifications:
// INNER - Specifies the number of times that the loop
// should perform the calculation being measured
// [this is a specified aspect of the loops]
// NB -- SEE LARGEST DEFINITION BELOW
static unsigned long inner[25] = 
  {0, // (padding)
   1000, //1
   500,
   1000,
   1000, //4
   1000,
   100,
   1000,
   1000,   //8
   1000,
   1000,
   1000,
   1000, //12
   1000,
   200,
   10,
   10,   //16
   10,
   10,
   10,
   10,   //20
   10,
   10,
   10,
   10,   //24
};  
// define the largest inner loop size for the arrays below
m4_define([[LARGEST]], [[8000]])

//**IMPORTANT**
//the block size of the outer create is set to the value below:
//N.B. Default is 0
m4_define([[SHARED_BLOCK]],[[2]]) //used by loops 3,5,6,11
static unsigned long blocksize[25] = 
{ 999, // (padding)
  0, //1
  0,
  0,
  0, //4
  0,
  0,
  0,
  0, //8
  0,
  0,
  0,
  0, //12
  0,
  0,
  0,
  0, //16
  0,
  0,
  0,
  0, //20
  0,
  0,
  0,
  0, //24
};


typedef double (*array2d)[LARGEST]; // typedef array types so they fit as type spec in the SL macros
typedef double (*array3d)[LARGEST][LARGEST];

// the following is used to hold
// some of the isolated scalars:
static double Xtra[40];
