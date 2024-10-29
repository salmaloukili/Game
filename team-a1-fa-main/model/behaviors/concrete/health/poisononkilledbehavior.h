#ifndef POISONONKILLEDBEHAVIOR_H
#define POISONONKILLEDBEHAVIOR_H

#include "generichealthbehavior.h"

/**
 * @brief The PoisonOnKilledBehavior class triggers the poisoning of neighbors when object dies.
 * In the requirements, the poison enemies poison until they run out of poison. IMO this looks a
 * bit cooler since it makes the enemies more annoying and unpredictable.
 */
class PoisonOnKilledBehavior : public QObject, public GenericHealthBehavior {
    Q_OBJECT
public:
    /**
     * @brief PoisonOnKilledBehavior Default constructor
     * @param owner poisoning
     */
    PoisonOnKilledBehavior(QPointer<GameObject> owner)
        : GenericHealthBehavior(owner) {};
    PoisonOnKilledBehavior() = delete;

    /**
     * @brief die overrides die from the Health interface to start the death process.
     */
    void die() override;

private:
    // Variables to handle current poisoning state.
    unsigned int m_nextPoison = 0, m_poisonTimes = 0, m_tickCount = 0, m_count;

public slots:
    /**
     * @brief spreadPoison main slot to handle the spreading of the poison.
     * It will spread poison to a random amount of neigbors + offset after a random
     * amount of ticks.
     */
    void spreadPoison();
};

#endif // POISONONKILLEDBEHAVIOR_H
