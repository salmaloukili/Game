#ifndef GENERICPOISONABLEBEHAVIOR_H
#define GENERICPOISONABLEBEHAVIOR_H

#include <model/behaviors/poison.h>

/**
 * @brief The GenericPoisonableBehavior class handles actions from objects that can be harmed by poison.
 * All objects that have a poison behavior can get poisoned, but not all of them get harmed by poison.
 * Probably a bad class name but do not want to change. HurtByPoisonBehavior?
 */
class GenericPoisonableBehavior : public QObject, public Poison {
    Q_OBJECT
public:
    /**
     * @brief GenericPoisonableBehavior default constructor.
     * This behavior connects the tick signal of its owner to its poisonEffect slot.
     * @param owner the owner of the behavior.
     */
    GenericPoisonableBehavior(QPointer<GameObject> owner)
        : Poison(owner) {
        // Slightly inefficient to connect and check always if there is poison, but since it is
        // only used on the protagonist it is fine for now. The getPoisoned function can be
        // overwritten in this behavior to only connect the slot when it gets poisoned.
        connect(owner, &GameObject::tick, this, &GenericPoisonableBehavior::poisonEffect);
    };
    GenericPoisonableBehavior() = delete;

public slots:
    /**
     * @brief poisonEffect slot handles the poison effect on the object every tick.
     * It only affects objects that have both the health behavior and poison levels > 0.
     */
    void poisonEffect();
};

#endif // GENERICPOISONABLEBEHAVIOR_H
