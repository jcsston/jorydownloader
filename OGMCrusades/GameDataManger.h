#ifndef _GAME_DATA_MANGER_H_
#define _GAME_DATA_MANGER_H_

#include <SDL/SDL.h>
#include <string>
#include <map>

class GameDataManger
{
public:
  GameDataManger();
  ~GameDataManger();

  static Uint8 *GetData(const std::string &filename);
  static long GetDataSize(const std::string &filename);
  static SDL_RWops *GetDataRW(const std::string &filename);

protected:
  class DataEntry {
  public:
    DataEntry() {
      rw = NULL;
      buffer = NULL;
      size = 0;
    };
    // This class requires manual cleanup

    SDL_RWops *rw;
    Uint8 *buffer;
    long size;
  };

  DataEntry &_LookupData(const std::string &filename);
  
  std::string m_BasePath;
  std::map<std::string, DataEntry> m_Cache;
};

#endif // _GAME_DATA_MANGER_H_
