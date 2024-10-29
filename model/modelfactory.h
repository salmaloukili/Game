

#ifndef MODELFACTORY_H
#define MODELFACTORY_H

#include "gameobjectmodel.h"
#include <node.h>

/**
 * @brief The ObjectModelFactory class is responsible for creating and populating the game world model.
 * It includes methods to generate the game world grid and pathfinding nodes.
 */
class ObjectModelFactory {
public:
    /**
     * @brief Creates a game model consisting of a grid of GameObjects and pathfinding nodes.
     * @param nrOfEnemies The number of enemies to create.
     * @param nrOfHealthpacks The number of health packs to create.
     * @param pRatio The poison ratio, affects the generation of poison tiles/enemies.
     * @param level The current game level, affects the world generation difficulty.
     * @param rows The number of rows in the game world grid.
     * @param columns The number of columns in the game world grid.
     * @return A pair consisting of a pointer to the generated GameObjectModel and a vector of Nodes for pathfinding.
     */
    static QPair<GameObjectModel *, std::vector<Node>> createModel(unsigned int nrOfEnemies, unsigned int nrOfHealthpacks,
                                                                   float pRatio, int level, int rows = 30, int columns = 40);

    /**
     * @brief Generates a world image based on Perlin noise to simulate terrain. Used in world creation.
     * @param width The width of the world (image) to generate.
     * @param height The height of the world (image) to generate.
     * @param difficulty The difficulty factor, influencing the generation of the Perlin noise terrain.
     */
    static void createWorld(int width, int height, double difficulty = 1.0);
};

#endif // MODELFACTORY_H
