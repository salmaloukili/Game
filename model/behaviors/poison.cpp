#include "poison.h"
Poison::~Poison() {};

int Poison::getPoisoned(int level) {
    QVariant poisonLevel = m_owner->getData(DataRole::PoisonLevel);
    if(poisonLevel.isNull()) {
        throw("Cannot change poison level of object.");
    }

    // PEnemies could get poisoned as well, their poison level is much
    // higher than the MAX so it doesn't matter. Poison for them is "Good"
    int newPoison = poisonLevel.toInt() + level;
    if(newPoison > Settings.MAX_POISON) {
        newPoison = Settings.MAX_POISON;
    }

    m_owner->setData(DataRole::PoisonLevel, QVariant(newPoison));

    return newPoison - poisonLevel.toInt();
}
