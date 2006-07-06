#include "TileMap.h"
#include "csvparser.h"
#include <fstream>

TileMap::TileMap()
{
  m_Terrain = NULL;
  m_Objects = NULL;
}

TileMap::~TileMap()
{
  if (m_Terrain != NULL) {
    delete [] m_Terrain;
    m_Terrain = NULL;
  }
  if (m_Objects != NULL) {
    delete [] m_Objects;
    m_Objects = NULL;
  }
}

void TileMap::Load(const std::string &filename)
{
  CSVParser csv;
  std::ifstream f(filename.c_str());
  char line[256];

  memset(line, 0, sizeof(line));

  f.getline(line, 255);
  csv << line;
  int iColumn = 0;
  int iRow = 0;
  csv >> iColumn >> iRow;

  int i = 0;
  m_Terrain = new char[iColumn*iRow];
  for (int r = 0; r < iRow; r++) {
    f.getline(line, 255);
    if (((strlen(line) == 0) || (line[0] == '#')) && (!f.eof())) {
      r--;
      continue;
    }
    csv << line;
    for (int c = 0; c < iColumn; c++) {
      csv >> i;
      m_Terrain[(r*iColumn)+c] = i;
    }
  }

  m_Objects = new char[iColumn*iRow];
  for (int r = 0; r < iRow; r++) {
    f.getline(line, 255);
    if (((strlen(line) == 0) || (line[0] == '#')) && (!f.eof())) {
      r--;
      continue;
    }
    csv << line;
    for (int c = 0; c < iColumn; c++) {
      csv >> i;
      m_Objects[(r*iColumn)+c] = i;
    }
  }

}

char *TileMap::GetTerrain()
{
  return m_Terrain;
}

char *TileMap::GetObjects()
{
  return m_Objects;
}