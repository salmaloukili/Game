#ifndef POISON_H
#define POISON_H

#include "behavior.h"
#include "model/gameobject.h"

#define GEOM_SUM(a, b) (a * 8 * (1 - b * b) / (1 - b))

/**
 * @brief The Poison class is an abstract class that handles poison actions.
 */
class Poison : public Behavior {
public:
    /**
     * @brief Poison Default constructor
     * @param owner of the behavior
     */
    Poison(QPointer<GameObject> owner)
        : Behavior(owner) {};
    // Abstract
    Poison() = delete;
    virtual ~Poison() = 0;
    Poison &operator=(const Poison &) {
        return *this;
    };

    /**
     * @brief Settings for Poison.
     */
    const static struct SETTINGS {
        ///@{
        /**
         * @brief Max/Min allowed amount of poison for a Game Object to get during gameplay.
         */
        constexpr static int MAX_POISON = 100;
        constexpr static int MIN_POISON = 0;
        ///}@
        /// Min that Game Object will poison per tick/action.
        constexpr static int MIN_POISON_PER_ACTION = 2;
        /// How much a Game Object will poison per tick/action.
        constexpr static int MAX_POISON_PER_ACTION = 10;
        ///@{
        /**
         * @brief Max/Min times a PEnemy will spread its poison.
         */
        constexpr static int POISON_SPREAD_TIMES_MIN = 5;
        constexpr static int POISON_SPREAD_TIMES_MAX = 7;
        ///@}
        /// The Min ticks between poison spread
        constexpr static int POISON_SPREAD_MIN_TICKS = 5;
        /// The Max ticks between poison spread
        constexpr static int POISON_SPREAD_MAX_TICKS = 10;
        /// How much damage poison does per tick.
        constexpr static int DAMAGE_PER_TICK = 1;
        /**
         * @brief PENEMY_POISON_LEVEL The Poison of a PEnemy.
         * Due to the geometric formula, ensures the PEnemy has enough poison to get all the neighbors.
         */
        constexpr static int PENEMY_POISON_LEVEL = GEOM_SUM(MAX_POISON_PER_ACTION, POISON_SPREAD_TIMES_MAX);
    } Settings;

    /**
     * @brief poison poison a target.
     * @param target target to be poisoned.
     * @return poison amount.
     */
    virtual int poison(const QPointer<GameObject> &target) {
        return 0;
    };
    /**
     * @brief getPoisoned get poisoned a ceirtain amount.
     * @param level amount to be poisoned.
     * @return poison gained by object.
     * Poison gained can be lower than level since there is a MAX_POISON level.
     */
    virtual int getPoisoned(int level);
};

#endif // POISON_H
