#ifndef GENERICHEALTHBEHAVIOR_H
#define GENERICHEALTHBEHAVIOR_H

#include <model/behaviors/health.h>

/**
 * @brief The GenericHealthBehavior class sets the basic actions to change an objects health and death.
 */
class GenericHealthBehavior : public Health {
public:
    /**
     * @brief GenericHealthBehavior default constructor
     * @param owner the owner of the behavior.
     */
    GenericHealthBehavior(QPointer<GameObject> owner)
        : Health(owner) {};
    GenericHealthBehavior() = delete;
    /**
     * @brief getHealthChanged changes the health of an object by a specified amount.
     * This function will change the health up to the MAX_HEALTH and MIN_HEALTH.
     * If the amount to change makes the health lower than MIN_HEALTH, it will
     * cause the object to die. If it makes it bigger than MAX_HEALTH, it will
     * cause the object to heal to the max and returns the difference between
     * current health (before healing) and MAX_HEALTH.
     * @param amount the amount to change, can be negative.
     * @return the health changed.
     */
    int getHealthChanged(int amount) override;
    /**
     * @brief die kills an object, deletes them.
     * Since the health = 0  is set in the getHealthChanged function.
     */
    void die() override;
};

#endif // GENERICHEALTHBEHAVIOR_H
