#include "GameDataManger.h"

static GameDataManger g_Manager;

GameDataManger::GameDataManger()
{

}

GameDataManger::~GameDataManger()
{

}

Uint8 *GameDataManger::GetData(const std::string &filename)
{
  return g_Manager._LookupData(filename).buffer;
}

long GameDataManger::GetDataSize(const std::string &filename)
{
  return g_Manager._LookupData(filename).size;
}

SDL_RWops *GameDataManger::GetDataRW(const std::string &filename)
{
  return g_Manager._LookupData(filename).rw;
}

GameDataManger::DataEntry &GameDataManger::_LookupData(const std::string &filename)
{
  DataEntry &entry = m_Cache[filename];
  int ret = -1;

  if (entry.rw != NULL) {
    // Good we have this in the cache
    return entry;
  }

  // We first try to get this file from the data/ sub folder if it exists
  FILE *pFile = NULL;
  std::string dataFilename; 
  dataFilename = "data\\";
  dataFilename += filename;
  pFile = fopen(dataFilename.c_str(), "rb");
  if (pFile == NULL) {
    // Try *nix path types
    dataFilename = "data/";
    dataFilename += filename;
    pFile = fopen(dataFilename.c_str(), "rb");
  }
  if (pFile != NULL) {
    fseek(pFile, 0, SEEK_END);
    entry.size = ftell(pFile);
    entry.buffer = new Uint8[entry.size];
    fseek(pFile, 0, SEEK_SET);
    fread((void *)entry.buffer, entry.size, 1, pFile);
    fclose(pFile);
    entry.rw = SDL_RWFromConstMem(entry.buffer, entry.size);
    return entry;
  }

  return entry;
}
