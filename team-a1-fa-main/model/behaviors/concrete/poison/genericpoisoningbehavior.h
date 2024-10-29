#ifndef GENERICPOISONINGBEHAVIOR_H
#define GENERICPOISONINGBEHAVIOR_H

#include <model/behaviors/poison.h>

/**
 * @brief The GenericPoisoningBehavior class handles the poison action from an object.
 */
class GenericPoisoningBehavior : public Poison {
public:
    /**
     * @brief GenericPoisoningBehavior default constructor
     * @param owner the owner of the behavior.
     */
    GenericPoisoningBehavior(QPointer<GameObject> owner)
        : Poison(owner) {};
    GenericPoisoningBehavior() = delete;
    /**
     * @brief poison poison a target while decreasing the poison level.
     * @param target the target to poison.
     * @return the poison level lost.
     */
    int poison(const QPointer<GameObject> &target) override;
};

#endif // GENERICPOISONINGBEHAVIOR_H
