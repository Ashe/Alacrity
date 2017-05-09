#include "TileInvisibleWall.h"

bool TileInvisibleWall::bump() {
	setHidden(false);
	return Tile::bump();
}
