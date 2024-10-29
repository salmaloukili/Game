#ifndef GENERICATTACKBEHAVIOR_H
#define GENERICATTACKBEHAVIOR_H

#include <model/behaviors/attack.h>

/**
 * @brief The GenericAttackBehavior class handles the basic actions of a class that can attack or be attacked.
 */
class GenericAttackBehavior : public Attack {
public:
    /**
     * @brief GenericAttackBehavior constructor.
     * @param owner the owner of the behavior.
     */
    GenericAttackBehavior(QPointer<GameObject> owner)
        : Attack(owner) {};
    GenericAttackBehavior() = delete;
    /**
     * @brief attack this attacks the target with a random strength between 1 and the object strength.
     * It simply gets all behaviors from the target and calls the getattacked function.
     * If the target itself is not attackable but any of its children are, it will work.
     * @param target the target to attack, can be a tile or a character.
     * @return the health lost by target and all its children.
     */
    int attack(const QPointer<GameObject> &target) override;
    /**
     * @brief attack overloaded function, attacks on a direction relative to the owner.
     * @param direction the direction to attack.
     * @return the health lost by target.
     */
    int attack(Direction direction) override;
    /**
     * @brief attack overloaded, attacks on the current direction the owner is facing.
     * @return the health lost of target.
     */
    int attack() override;
    /**
     * @brief getAttacked handles the attack by decreasing the health of the object if it has it.
     * @param by The source of the attack.
     * @param strength The amount of health to loose.
     * @return the amount of health lost.
     */
    int getAttacked(const QPointer<GameObject> &by, int strength) override;
};

#endif // GENERICATTACKBEHAVIOR_H
