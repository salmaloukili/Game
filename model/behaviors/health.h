#ifndef HEALTH_H
#define HEALTH_H

#include "behavior.h"

#include "model/gameobject.h"
#include <QSharedPointer>
/**
 * @brief The Health class is an abstract class that handles health behaviors.
 * This class is abstract to avoid repeating the same things for all the behaviors that
 * extend it. Most objects that have health do not have all the actions. For
 * example, enemies do not heal but they need to get their health decreased through attacks.
 */
class Health : public Behavior {
public:
    /**
     * @brief Health default constructor.
     * @param owner the owner of the behavior.
     */
    Health(QPointer<GameObject> owner)
        : Behavior(owner) {};
    Health() = delete;

    // Abstract class
    virtual ~Health() = 0;
    Health &operator=(const Health &) {
        return *this;
    };

    // Health settings
    static const struct SETTINGS {
        // Max health possible for an object
        static constexpr int MAX_HEALTH = 100;
        // Min health possible for an object
        static constexpr int MIN_HEALTH = 0;
        // Health of a health pack
        static constexpr int HEALTH_PACK_AMOUNT = 36;
    } Settings;

    /**
     * @brief heal heals an object.
     * It is important to differentiate a simple health change and healing.
     * Healing can involve more than simply changing health. For example,
     * it can remove poison on the target, it can decrease the health of the owner, etc.
     * @param target object to heal.
     * @return amount healed.
     */
    virtual int heal(const QPointer<GameObject> &target) {
        return 0;
    };
    /**
     * @brief getHealthChange change the health of the owner.
     * @param amount amount to change the health, can be negative.
     * @return amount of health changed.
     */
    virtual int getHealthChanged(int amount) {
        return 0;
    };

    /**
     * @brief die handles the death of the owner
     */
    virtual void die() {
        return;
    };
};

#endif // HEALTH_H
