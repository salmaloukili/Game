#ifndef GENERICHEALINGBEHAVIOR_H
#define GENERICHEALINGBEHAVIOR_H

#include <model/behaviors/health.h>
#include <model/behaviors/movement.h>

/**
 * @brief The GenericHealingBehavior class handles behavior of objects that heal others.
 */
class GenericHealingBehavior : public Health {
public:
    /**
     * @brief GenericHealingBehavior default constructor for GHB
     * @param owner the owner of the behavior.
     */
    GenericHealingBehavior(QPointer<GameObject> owner)
        : Health(owner) {};
    GenericHealingBehavior() = delete;

public:
    /**
     * @brief heal heals target by an amount and cures it of poison.
     * This will heal the target using as much health as the owner has.
     * If the owner has less health than the target can be healed, it is
     * destroyed after healing.
     * @param target target to heal.
     * @return amount healed.
     */
    int heal(const QPointer<GameObject> &target) override;
};

#endif // GENERICHEALINGBEHAVIOR_H
