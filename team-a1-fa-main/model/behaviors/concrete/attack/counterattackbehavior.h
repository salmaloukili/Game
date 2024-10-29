#ifndef COUNTERATTACKBEHAVIOR_H
#define COUNTERATTACKBEHAVIOR_H

#include "genericattackbehavior.h"

/**
 * @brief The CounterAttackBehavior class extends the GenericAttackBehavior so that the owner attacks back after being attacked.
 */
class CounterAttackBehavior : public GenericAttackBehavior {
public:
    /**
     * @brief CounterAttackBehavior default constructor.
     * @param owner the owner of the behavior.
     */
    CounterAttackBehavior(QPointer<GameObject> owner)
        : GenericAttackBehavior(owner) {};

public:
    /**
     * @brief getAttacked overrides the default getAttack to attack back.
     * @param by source of the attack.
     * @param strength strength of the attack taken.
     * @return strength of the attack given.
     */
    int getAttacked(const QPointer<GameObject> &by, int strength) override;
};

#endif // COUNTERATTACKBEHAVIOR_H
