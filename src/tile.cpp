#include "../include/tile.hpp"

Tile::Tile(Block block, int x, int y)
    : m_position(Point(x * TAILLE_CASE, y * TAILLE_CASE)), m_block(block) {}

Tile::Tile(int x, int y)
    : m_position(Point(x * TAILLE_CASE, y * TAILLE_CASE)), m_block(Block()) {}

bool Tile::estDansCam(int posCamX, int posCamY, int largeur, int hauteur) {
  if (m_position.getX() >= posCamX - largeur &&
      m_position.getX() <= posCamX + largeur &&
      m_position.getY() >= posCamY - hauteur &&
      m_position.getY() <= posCamY + hauteur) {
    return true;
  }
  return false;
}

void Tile::collide(Personnage *perso) {
  int persoX = perso->getX();
  int persoY = perso->getY();
  int persoLargeur = perso->getLargeur();
  int persoHauteur = perso->getHauteur();
  int persoVitesse = perso->getVitesse();

  int blockX = m_position.getX();
  int blockY = m_position.getY();

  if (m_block.isSolid() && blockX + TAILLE_CASE > persoX &&
      blockX < persoX + persoLargeur && blockY + TAILLE_CASE > persoY &&
      blockY < persoY + persoHauteur + persoVitesse) {
    perso->setCollision("down", true);
  }
  if (m_block.isSolid() && blockX + TAILLE_CASE > persoX && // left
      blockX < persoX + persoLargeur + persoVitesse &&
      blockY + TAILLE_CASE > persoY && blockY < persoY + persoHauteur // down
  ) {
    perso->setCollision("right", true);
  }
  if (m_block.isSolid() && blockX + TAILLE_CASE > persoX - persoVitesse &&
      blockX < persoX + persoLargeur && blockY + TAILLE_CASE > persoY &&
      blockY < persoY + persoHauteur) {
    perso->setCollision("left", true);
  }
  if (m_block.isSolid() && blockX + TAILLE_CASE > persoX - persoVitesse &&
      blockX < persoX + persoLargeur && blockY + TAILLE_CASE > persoY &&
      blockY < persoY + persoHauteur) {
    perso->setCollision("left", true);
  }
  if (m_block.isSolid() && blockX + TAILLE_CASE > persoX &&
      blockX < persoX + persoLargeur &&
      blockY + TAILLE_CASE > persoY - persoVitesse &&
      blockY < persoY + persoHauteur) {
    perso->setCollision("up", true);
  }
}