#include "../include/map.hpp"

using namespace std;

/* aux func */
Block getBlock(string id) {
  for (auto it = blockMap.begin(); it != blockMap.end(); ++it) {
    if (it->second.getId() == id) {
      return it->second;
    }
  }
  return blockMap["AIR"];
}

/* constructor */
Map::Map(string path)
    : m_workingAreaCoord(0, 0), m_workingAreaWidth(0), m_workingAreaHeight(0) {
  m_save = true;
  if (!initLegthFromCSV(path) || !loadFromCSV(path)) {
    cerr << "Map : cant open the map " << path << endl;
    Createmap cm(MAP_WIDTH);
    cm.generate();
    cm.saveinfile(path);
  }
}

Map::Map(int height, int width) {
  m_save = false;
  for (int y = 0; y < height; y++) {
    m_map.push_back(std::vector<Tile>());
    for (int x = 0; x < width; x++) {
      m_map[y].push_back(Tile(getBlock("AIR"), x, y));
    }
  }
}
/* destructor */
void Map::clear() {
  for (int y = 0; y < m_height; y++) {
    m_map[y].clear();
  }
  m_map.clear();
}

/* init function */

bool Map::initLegthFromCSV(string pathFile) {
  int width = 0;
  m_width = 0;
  m_height = 0;
  ifstream fichier(pathFile);
  if (fichier) {
    string ligne;
    while (getline(fichier, ligne)) {
      stringstream ss(ligne);
      string c;
      while (getline(ss, c, ';')) {
        width++;
      }
      if (width > m_width) {
        m_width = width;
      }
      width = 0;
      m_height++;
    }
    return true;
  } else {
    return false;
  }
}

bool Map::loadFromCSV(string pathFile) {
  ifstream fichier(pathFile);
  if (fichier) {
    string ligne;
    int y = 0;
    while (getline(fichier, ligne)) {
      stringstream ss(ligne);
      string c;
      int x = 0;
      m_map.push_back(std::vector<Tile>());
      while (getline(ss, c, ';')) {
        m_map[y].push_back(chooseTile(c, x, y));
        x++;
      }
      if (x < m_width) {
        for (int i = x; i < m_width; i++) {
          m_map[y].push_back(chooseTile("0", i, y));
        }
      }
      y++;
    }
    return true;
  } else {
    return false;
  }
}

/* getters */

Tile Map::chooseTile(string c, int x, int y) { return Tile(getBlock(c), x, y); }

Tile *Map::find_tile(int world_x, int world_y) {
  for (int y = m_workingAreaCoord.getY(); y < m_workingAreaHeight; y++) {
    for (int x = m_workingAreaCoord.getX(); x < m_workingAreaWidth; x++) {
      if (world_x >= m_map[y][x].getX() &&
          world_x <= m_map[y][x].getX() + TILE_SIZE &&
          world_y >= m_map[y][x].getY() &&
          world_y <= m_map[y][x].getY() + TILE_SIZE) {
        return &m_map[y][x];
      }
    }
  }
  return nullptr;
}

Clock Map::getBreakingClock(int mouseX, int mouseY) {
  Tile *target = find_tile(mouseX, mouseY);
  if (!target) {
    cerr << "getBreakingClock : target is nullptr" << endl;
    return Clock();
  }
  return target->getBreakingClock();
}

bool Map::isBreaking(int mouseX, int mouseY) {
  Tile *target = find_tile(mouseX, mouseY);
  if (target) {
    return target->isBreaking();
  } else {
    // cerr << "isBreaking : target is nullptr" << endl;
    return false;
  }
}

/* setters */
void Map::setIsBreaking(bool isBreaking, int mouseX, int mouseY) {
  Tile *target = find_tile(mouseX, mouseY);
  if (target) {
    target->setBreaking(isBreaking);
  } else {
    cerr << "setIsBreaking : target is nullptr" << endl;
  }
}

void Map::resetBreakingClock(int mouseX, int mouseY) {
  Tile *target = find_tile(mouseX, mouseY);
  if (target) {
    target->resetBreakingClock();
  } else {
    cerr << "resetBreakingClock : target is nullptr" << endl;
  }
}

/* other */

void Map::save(string path) {
  if (m_save) {
    ofstream fichier(path);
    if (fichier) {
      for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
          fichier << m_map[y][x].getBlock()->getId();
          fichier << ";";
        }
        fichier << endl;
      }
    }
  }
}

void Map::collide(Entity *entity, int camX, int camY) {
  for (int y = m_workingAreaCoord.getY(); y < m_workingAreaHeight; y++) {
    for (int x = m_workingAreaCoord.getX(); x < m_workingAreaWidth; x++) {
      if (m_map[y][x].getBlock()->isSolid()) {
        m_map[y][x].collide(entity);
      }
    }
  }
}

void Map::collide(Entity *entity) {
  if (entity->getX() < 0)
    entity->setCollision("left", true);
  if (entity->getX() + entity->getWidth() > get_width() * TILE_SIZE)
    entity->setCollision("right", true);
  if (entity->getY() < 0)
    entity->setCollision("up", true);
  if (entity->getY() + entity->getHeight() > get_height() * TILE_SIZE)
    entity->setCollision("down", true);
}

void Map::add_tile(Block block, int mouseX, int mouseY) {
  Tile *target = find_tile(mouseX, mouseY);
  if (target) {
    target->setBlock(block);
  } else {
    cerr << "add_tile : target is nullptr" << endl;
  }
}

void Map::supr_tile(int mouseX, int mouseY) {
  Tile *target = find_tile(mouseX, mouseY);
  if (target) {
    target->setBlock(blockMap["AIR"]);
  } else {
    cerr << "supr_tile: target is nullptr" << endl;
  }
}

void Map::update(int camX, int camY) {
  int newCamX = (camX - CAM_WIDTH) / TILE_SIZE;
  int newCamY = (camY - CAM_HEIGHT) / TILE_SIZE;
  int newCamWidth = (camX + CAM_WIDTH) / TILE_SIZE;
  int newCamHeight = (camY + CAM_HEIGHT) / TILE_SIZE;
  if (newCamX < 0)
    newCamX = 0;
  if (newCamY < 0)
    newCamY = 0;
  if (newCamWidth >= get_width())
    newCamWidth = get_width();
  if (newCamHeight >= get_height())
    newCamHeight = get_height();

  m_workingAreaCoord.setCoord(newCamX, newCamY);
  m_workingAreaWidth = newCamWidth;
  m_workingAreaHeight = newCamHeight;
}

string Map::toString() {
  string res;
  for (int y = 0; y < m_map.size(); y++) {
    for (int x = 0; x < m_map[y].size(); x++) {
      res += m_map[y][x].toString();
    }
  }
  return res;
}

bool Map::checkMonsterCollisionAt(int x, int y) {
  // Convertir les coordonnées du monde en coordonnées de la carte
  int mapX = x / TILE_SIZE;
  int mapY = y / TILE_SIZE;

  // Vérifier si les coordonnées de la carte pointent vers un bloc solide
  if (mapX >= 0 && mapX < m_width && mapY >= 0 && mapY < m_height) {
    return m_map[mapY][mapX].getBlock()->isSolid();
  }

  // Si les coordonnées sont en dehors des limites de la carte, considérer comme
  // une collision
  return true;
}
