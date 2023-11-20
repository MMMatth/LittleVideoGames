#include "../include/tile.hpp"

Tile::Tile(Block block, int x, int y)
    : m_coord(Coord(x * TILE_SIZE, y * TILE_SIZE)), m_block(block) {}

Tile::Tile(int x, int y)
    : m_coord(Coord(x * TILE_SIZE, y * TILE_SIZE)), m_block(Block()) {}

bool Tile::estDansCam(int posCamX, int posCamY, int largeur, int hauteur) {
  if (m_coord.getX() >= posCamX - largeur &&
      m_coord.getX() <= posCamX + largeur &&
      m_coord.getY() >= posCamY - hauteur &&
      m_coord.getY() <= posCamY + hauteur) {
    return true;
  }
  return false;
}

bool isColliding(int char_x, int char_y, int char_width, int char_height,
                 int char_speed, int tile_x, int tile_y) {
  return tile_x + TILE_SIZE > char_x && tile_x < char_x + char_width &&
         tile_y + TILE_SIZE > char_y &&
         tile_y < char_y + char_height + char_speed;
}

void Tile::collide(Character *character) {
  int char_x = character->getX() - character->getWidth() / 2;
  int char_y = character->getY();
  int char_width = character->getWidth();
  int char_height = character->getHeight();
  int char_speed = character->getSpeed();

  int tile_x = m_coord.getX();
  int tile_y = m_coord.getY();

  if (!m_block.isSolid())
    return;
  if (isColliding(char_x, char_y, char_width, char_height, char_speed, tile_x,
                  tile_y)) {
    character->setCollision("down", true);
  }
  if (isColliding(char_x + char_speed, char_y, char_width, char_height, 0,
                  tile_x, tile_y)) {
    character->setCollision("right", true);
  }
  if (isColliding(char_x - char_speed, char_y, char_width, char_height, 0,
                  tile_x, tile_y)) {
    character->setCollision("left", true);
  }
  if (isColliding(char_x, char_y - char_speed, char_width, char_height, 0,
                  tile_x, tile_y)) {
    character->setCollision("up", true);
  }
}

bool Tile::collide(Monster *monster) {
  int monster_x = monster->getX() - monster->getWidth() / 2;
  int monster_y = monster->getY();
  int monster_width = monster->getWidth();
  int monster_height = monster->getHeight();
  int monster_speed = monster->getSpeed();

  int tile_x = m_coord.getX();
  int tile_y = m_coord.getY();

  if (!m_block.isSolid())
    return false;

  if (isColliding(monster_x, monster_y, monster_width, monster_height,
                  monster_speed, tile_x, tile_y)) {
    monster->setCollision("down", true);

    return true;
  }
  if (isColliding(monster->getX() + monster->getWidth() + monster_speed,
                  monster_y, monster_width, monster_height, 0, tile_x,
                  tile_y)) {
    monster->setCollision("right", true);
    return true;
  }
  if (isColliding(monster_x - monster_speed, monster_y, monster_width,
                  monster_height, 0, tile_x, tile_y)) {
    monster->setCollision("left", true);
    return true;
  }
  if (isColliding(monster_x, monster_y - monster_speed, monster_width,
                  monster_height, 0, tile_x, tile_y)) {
    monster->setCollision("up", true);
    return true;
  }
  return false;
}
