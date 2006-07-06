#ifndef _TILE_MAP_H_
#define _TILE_MAP_H_

#include <string>

class TileMap
{
public:
  TileMap();
  ~TileMap();

  void Load(const std::string &filename);

  char *GetTerrain();
  char *GetObjects();

protected:
  char *m_Terrain;
  char *m_Objects;
};

#endif // _TILE_MAP_H_