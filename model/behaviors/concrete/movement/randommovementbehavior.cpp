#include "randommovementbehavior.h"

#include <QRandomGenerator>

void RandomMovementBehavior::moveRandomly() {
    bool steppable = true;
    float energy = 0;
    for(const auto &neighbor : m_owner->getAllNeighbors()) {
        if(!neighbor) {
            continue;
        }

        // AND the steppability of every neighbor, if any is not steppable result will be false.
        steppable &= neighbor->getBehavior<Movement>()->isSteppable();

        // Check if there is a protagonist in the neigbor. TODO: Put this in the game object.
        for(const auto &data : neighbor->getAllData()) {
            if(data[DataRole::Type].value<ObjectType>() == ObjectType::Protagonist) {
                return;
            }
        }
        // Get the energy of the tile around it, if it is infinite, make it very big.
        float neighborEnergy = neighbor->getData(DataRole::Energy).toFloat();
        neighborEnergy = neighborEnergy == INFINITY ? 1000000000 : neighborEnergy;

        // Save the biggest energy of all neighbors.
        energy = neighborEnergy > energy ? neighborEnergy : energy;
    }

    // If no neigbors are steppable or the energy is lower than the highest neighbor stop moving.
    // Enemies should have large energies, I don't like it that much when they stop moving.
    if(!steppable || energy > m_owner->getData(DataRole::Energy).toFloat()) {
        m_owner->setData(DataRole::Energy, 0);
        disconnect(m_owner, &GameObject::tick, this, &RandomMovementBehavior::moveRandomly);
        return;
    }

    // Declares a uniform distribution from 0 to 7. Much more natural looking than the default
    // RandomGenerator (they don't aggregate in corners).
    std::uniform_int_distribution<> dist(0, 7);
    int direction, count = 0;
    do {
        // Ensure it is uniquely seeded by using the global static method of QRandomGenerator.
        direction = dist(*QRandomGenerator::global()) * 45;

        // If it can't move it is probably stuck, exit.
        if(count > 7) {
            return;
        }
        count++;
    } while(!GenericMoveBehavior::stepOn(static_cast<Direction>(direction)));
}
