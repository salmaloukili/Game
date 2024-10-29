#ifndef ATTACK_H
#define ATTACK_H

#include <QSharedPointer>

#include <model/gameobject.h>

/**
 * @brief The Attack interface handles all the actions related to attacking and getting attacked.
 */
class Attack : public Behavior {
public:
    /**
     * @brief Attack default constructor.
     * @param owner the object to which this behavior applies.
     */
    Attack(QPointer<GameObject> owner)
        : Behavior(owner) {};
    ///@{
    /// Interface without default constructor
    Attack() = delete;
    virtual ~Attack() = 0;
    Attack &operator=(const Attack &) { return *this; };
    ///@}
    /**
     * @brief Settings of the Attack Behavior
     **/
    static const struct SETTINGS {
        ///@{
        /**
         * @brief These decide the upper range for the attack strength randint.
         */
        static constexpr int PLAYER_STRENGTH = 30;
        static constexpr int ENEMY_STRENGTH = 5;
        ///@}
    } Settings;

    /**
     * @brief attack owner attacks the target.
     * @param target to attack.
     * @return attack damage.
     */
    virtual int attack(const QPointer<GameObject> &target) = 0;
    /**
     * @brief attack overloaded function to attack in a direction relative to owner.
     * @param target to attack.
     * @return attack damage.
     */
    virtual int attack(Direction direction) = 0;
    /**
     * @brief attack in the direction the owner is facing.
     * @return attack damage.
     */
    virtual int attack() = 0;
    /**
     * @brief getAttacked this is called by the attack function whenever owner attacks target.
     * This function can be implemented differently for things that cannot be attacked or that
     * behave differently when attacked.
     * @param strength the amount it is being attacked with.
     * @return the health loss.
     */
    virtual int getAttacked(const QPointer<GameObject> &by, int strength) = 0;
};

#endif // ATTACK_H
