#include "generichealthbehavior.h"

int GenericHealthBehavior::getHealthChanged(int amount) {
    QVariant currentHealth = m_owner->getData(DataRole::Health);
    if(currentHealth.isNull()) {
        throw("Cannot change health of object without health");
    }
    // Do not heal if fully healed
    if(currentHealth == Settings.MAX_HEALTH && amount > 0) {
        return 0;
    }

    // Check if health change is mortal or bigger than max.
    int newHealth = currentHealth.toInt() + amount;
    if(newHealth <= Settings.MIN_HEALTH) {
        m_owner->setData(DataRole::Health, 0);
        die();
        return 0;
    } else if(newHealth > Settings.MAX_HEALTH) {
        newHealth = Settings.MAX_HEALTH;
    }

    m_owner->setData(DataRole::Health, QVariant(newHealth));
    return newHealth - currentHealth.toInt();
}

void GenericHealthBehavior::die() {
    // Not all objects disappear when they die so we cannot use the
    // Destroyed DataRole here
    delete m_owner;
}
