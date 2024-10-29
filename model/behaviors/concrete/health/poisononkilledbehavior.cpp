#include "poisononkilledbehavior.h"
#include "model/behaviors/attack.h"
#include "model/behaviors/poison.h"

#include <QRandomGenerator>

#include <model/behaviors/concrete/movement/genericwalkablebehavior.h>

void PoisonOnKilledBehavior::die() {
    // Make object inert
    m_owner->removeBehavior<Attack>();
    m_owner->setBehavior<Movement>(QSharedPointer<GenericWalkableBehavior>::create(m_owner));

    // Calculate the times to spread poison (the tile offset will be based on this)
    m_count = m_poisonTimes = QRandomGenerator::global()->bounded(
      Poison::SETTINGS::POISON_SPREAD_TIMES_MIN,
      Poison::SETTINGS::POISON_SPREAD_TIMES_MAX);

    // Calculate the poisoning ticks.
    m_nextPoison = QRandomGenerator::global()->bounded(
      Poison::SETTINGS::POISON_SPREAD_MIN_TICKS,
      Poison::SETTINGS::POISON_SPREAD_MAX_TICKS);

    connect(m_owner, &GameObject::tick, this, &PoisonOnKilledBehavior::spreadPoison);
}

void PoisonOnKilledBehavior::spreadPoison() {
    m_tickCount++;

    if(m_nextPoison > m_tickCount) {
        return;
    }

    // Poison all neighbors until the count is up. Then delete object.
    if(m_count) {
        m_tickCount = 0;
        m_nextPoison = QRandomGenerator::global()->bounded(
          Poison::SETTINGS::POISON_SPREAD_MIN_TICKS,
          Poison::SETTINGS::POISON_SPREAD_MAX_TICKS);

        for(const auto &n : m_owner->getAllNeighbors(m_poisonTimes - m_count)) {
            if(n) {
                m_owner->getBehavior<Poison>()->poison(n);
            }
        }

        m_count--;
    } else {
        m_owner->setData(DataRole::Destroyed, true);
        // Could also call the die function in health...
        delete m_owner;
    }
}
