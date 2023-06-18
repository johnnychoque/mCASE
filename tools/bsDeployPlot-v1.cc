#include "plc++demos.h"

#ifdef USE_NAMESPACE
using namespace std;
#endif

#define CIRCLE 4
#define SQUARE 6
#define TRIANGLE 7
#define GREEN 3
#define PINK 13

class bsDeployPlot {

public:
  bsDeployPlot(int, const char **);
  void plotBS(int);

private:
  // Class data
  plstream *pls;

};

bsDeployPlot::bsDeployPlot( int argc, const char ** argv )
{
  // plplot initialization
  pls = new plstream();

  // Parse and process command line arguments.
  pls->parseopts(&argc, argv, PL_PARSE_FULL);

  // Initialize PLplot.
  pls->init();
  pls->font(2);

  // Make log plots using two different styles.
  plotBS(0);

  delete pls;
}

void bsDeployPlot::plotBS( int type )
{
  int i,nPoints;
  
  PLFLT *valX = new PLFLT[50];
  PLFLT *valY = new PLFLT[50];
  PLINT *bsType = new PLINT[50];
  PLINT *opType = new PLINT[50];
  PLFLT *tmpX = new PLFLT[1];
  PLFLT *tmpY = new PLFLT[1];
  
  PLINT opColor, bsShape;
  
  FILE *pFile;
  int fpos;
  int bsID;
  float posX, posY;
  int bsTypeID, opTypeID;
  
  pls->adv(0);

  pFile = fopen("../results/001_BS.txt","r");

  nPoints=0;
  do {
	fpos = fscanf (pFile, "%d %f %f %d %d", &bsID, &posX, &posY, &bsTypeID, &opTypeID);
	valX[nPoints] = posX;
	valY[nPoints] = posY;
	bsType[nPoints] = bsTypeID;
	opType[nPoints] = opTypeID;
	nPoints++;
  } while (fpos != EOF);

  pls->vpor(0.15, 0.85, 0.1, 0.9);
  pls->wind(0.0, 1000.0, 0.0, 1000.0);

  pls->col0(1);
  pls->box("bcnst", 0.0, 0, "bcnstv", 0.0, 0);
  
  for (i=0; i<nPoints; i++) {
	tmpX[0] = valX[i];
	tmpY[0] = valY[i];
	switch (opType[i]) {
	case 0:
		opColor = GREEN;
		break;
	case 1:
		opColor = PINK;
		break;
	}
	switch (bsType[i]) {
	case 0:
		bsShape = CIRCLE;
		break;
	case 1:
		bsShape = SQUARE;
		break;
	case 2:
		bsShape = TRIANGLE;
		break;
	}
	pls->col0(opColor);
	pls->poin(1,tmpX,tmpY,bsShape);
  }

  delete[] valX;
  delete[] valY;
  delete[] bsType;
  delete[] opType;
  delete[] tmpX;
  delete[] tmpY;
  
}


int main( int argc, const char ** argv )
{
  bsDeployPlot *x = new bsDeployPlot( argc, argv );

  delete x;
}
