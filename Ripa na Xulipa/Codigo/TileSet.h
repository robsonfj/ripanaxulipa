

#ifndef __IDJ__TileSet__
#define __IDJ__TileSet__

#include <iostream>

using std::string;

class TileSet{
	
protected:
	int tileWidth;
	int tileHeight;
	
public:
	virtual ~TileSet(){};
	virtual void Open (string file) = 0;
	virtual void Render (unsigned int index, float x, float y) = 0;
	int GetTileWidth (){return tileWidth;};
	int GetTileHeight(){return tileHeight;};

};

#endif /* defined(__IDJ__TileSet__) */
