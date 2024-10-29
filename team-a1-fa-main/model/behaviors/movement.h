#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "behavior.h"

#include <model/gameobject.h>

#include <QSharedPointer>

/**
 * @brief The Movement class handles the actions of moving. It is an abstract class.
 * All the functions return false by default so that objects that can move cannot
 * be walked through by default, and objects that can be stepped on cannot be moved.
 * Also, some objects can be stepped on by enemies but not trigger their behaviors.
 * E.G. Doorways can be stepped on by enemies (so they return true on isSteppable and
 * getSteppedOn) but do not start a new level.
 */
class Movement : public Behavior {
public:
    /**
     * @brief Movement default constructor.
     * @param owner of the behavior.
     */
    Movement(QPointer<GameObject> owner)
        : Behavior(owner) {};
    Movement() = delete;

    // Abstract
    virtual ~Movement() = 0;
    Movement &operator=(const Movement &) {
        return *this;
    };

    /// Movement settings
    static const struct SETTINGS {
        /// Max energy for a GameObject
        static constexpr int MAX_ENERGY = 100;
    } Settings;

    /**
     * @brief stepOn handles stepping on a ceirtain target.
     * @param target to step on.
     * @return false by default.
     */
    virtual bool stepOn(const QPointer<GameObject> target) {
        return false;
    };
    /**
     * @brief stepOn overloaded function to step on a direction.
     * @param direction relative to the owner.
     * @return false by default.
     */
    virtual bool stepOn(Direction direction) {
        return false;
    };
    /**
     * @brief isStepable checks weather the owner is steppable.
     * @return false by default.
     */
    virtual bool isSteppable() {
        return false;
    };
    /**
     * @brief getSeppedOn handles the source stepping on the owner.
     * @param source object stepping on owner.
     * @return false by default.
     */
    virtual bool getSteppedOn(const QPointer<GameObject> &source) {
        return false;
    };
};

#endif // MOVEMENT_H
