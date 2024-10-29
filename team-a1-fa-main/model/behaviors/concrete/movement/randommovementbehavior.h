#ifndef RANDOMMOVEMENTBEHAVIOR_H
#define RANDOMMOVEMENTBEHAVIOR_H

#include "genericmovebehavior.h"
#include <QObject>

/**
 * @brief The RandomMovementBehavior class makes an object move randomly every tick.
 */
class RandomMovementBehavior : public QObject, public GenericMoveBehavior {
    Q_OBJECT
public:
    /**
     * @brief RandomMovementBehavior connects the owner's tick to the moveRandomly function
     * @param owner of the behavior
     */
    RandomMovementBehavior(QPointer<GameObject> owner)
        : GenericMoveBehavior(owner) {
        connect(owner, &GameObject::tick, this, &RandomMovementBehavior::moveRandomly);
    };
    RandomMovementBehavior() = delete;

public slots:
    /**
     * @brief moveRandomly make a random movement in a direction.
     * The directions are all multiples of 45 degrees so the owner can move to
     * all eight neighbors.
     */
    void moveRandomly();
};

#endif // RANDOMMOVEMENTBEHAVIOR_H
