#include "TileInverseInvisibleWall.h"

bool TileInverseInvisibleWall::bump() {
	setHidden(true);
	return true;
}