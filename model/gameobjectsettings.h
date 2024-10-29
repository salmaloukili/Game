#ifndef GAMEOBJECTSETTINGS_H
#define GAMEOBJECTSETTINGS_H

#include "model/behaviors/concrete/movement/randommovementbehavior.h"
#include <model/behaviors/concrete/poison/genericpoisonablebehavior.h>
#include <model/behaviors/concrete/poison/genericpoisoningbehavior.h>
#include <model/behaviors/concrete/movement/genericmovebehavior.h>
#include <model/behaviors/concrete/movement/healonstepbehavior.h>
#include <model/behaviors/concrete/movement/newlevelonstep.h>
#include <model/behaviors/concrete/movement/poisononstepbehavior.h>
#include <model/behaviors/concrete/health/generichealingbehavior.h>
#include <model/behaviors/concrete/health/generichealthbehavior.h>
#include <model/behaviors/concrete/health/poisononkilledbehavior.h>
#include <model/behaviors/concrete/attack/counterattackbehavior.h>
#include <model/behaviors/concrete/attack/genericattackbehavior.h>

/**
 * @brief GameObjectSettings class provides configurations for different types of GameObjects.
 * It contains nested structures for each GameObject type, defining their default data and behaviors.
 */
class GameObjectSettings {
    /**
     * @brief Settings for Protagonist GameObject.
     */
    struct ProtagonistSettings {
        /**
         * @brief Default data for Protagonist.
         */
        inline static const QList<QPair<DataRole, QVariant>> defaultData {
          {DataRole::Type, QVariant::fromValue<ObjectType>(ObjectType::Protagonist)},
          {DataRole::Health, Health::SETTINGS::MAX_HEALTH},
          {DataRole::Energy, Movement::SETTINGS::MAX_ENERGY},
          {DataRole::Strength, (float)Attack::SETTINGS::PLAYER_STRENGTH},
          {DataRole::PoisonLevel, Poison::SETTINGS::MIN_POISON},
          {DataRole::Direction, QVariant::fromValue<Direction>(Direction::Down)},

        };
        /**
         * @brief Set up a GameObject as a Protagonist with predefined settings and behaviors.
         * @param obj Pointer to the GameObject to be set up.
         */
        static void setObject(GameObject *obj) {
            obj->setData(defaultData);
            obj->setBehavior<Attack>(QSharedPointer<GenericAttackBehavior>::create(obj));
            obj->setBehavior<Movement>(QSharedPointer<GenericMoveBehavior>::create(obj));
            obj->setBehavior<Poison>(QSharedPointer<GenericPoisonableBehavior>::create(obj));
            obj->setBehavior<Health>(QSharedPointer<GenericHealthBehavior>::create(obj));
        };
    };

    /**
     * @brief Settings for Tile GameObject.
     */
    struct TileSettings {
        inline static const QList<QPair<DataRole, QVariant>> defaultData {
          {DataRole::Type, QVariant::fromValue<ObjectType>(ObjectType::Tile)},
          {DataRole::PoisonLevel, Poison::SETTINGS::MIN_POISON},
        };
        /**
         * @brief Set up a GameObject as a Tile with predefined settings and behaviors.
         * @param obj Pointer to the GameObject to be set up.
         */

        static void setObject(GameObject *obj) {
            obj->setData(defaultData);
            obj->setBehavior<Movement>(QSharedPointer<PoisonOnStepBehavior>::create(obj));
            obj->setBehavior<Poison>(QSharedPointer<GenericPoisoningBehavior>::create(obj));
        };
    };

    /**
     * @brief Settings for Doorway GameObject.
     */

    struct DoorSettings {
        inline static const QList<QPair<DataRole, QVariant>> defaultData {
          {DataRole::Type, QVariant::fromValue<ObjectType>(ObjectType::Doorway)},
        };
        /**
         * @brief Set up a GameObject as a Doorway with predefined settings and behaviors.
         * @param obj Pointer to the GameObject to be set up.
         */
        static void setObject(GameObject *obj) {
            obj->setData(defaultData);
            obj->setBehavior<Movement>(QSharedPointer<NewLevelOnStep>::create(obj));
        };
    };

    /**
     * @brief Settings for HealthPack GameObject.
     */

    struct HealthPackSettings {
        inline static const QList<QPair<DataRole, QVariant>> defaultData {
          {DataRole::Type, QVariant::fromValue<ObjectType>(ObjectType::HealthPack)},
          {DataRole::Health, Health::SETTINGS::HEALTH_PACK_AMOUNT},
        };
        /**
         * @brief Set up a GameObject as a HealthPack with predefined settings and behaviors.
         * @param obj Pointer to the GameObject to be set up.
         */
        static void setObject(GameObject *obj) {
            obj->setData(defaultData);
            obj->setBehavior<Movement>(QSharedPointer<HealOnStepBehavior>::create(obj));
            obj->setBehavior<Health>(QSharedPointer<GenericHealingBehavior>::create(obj));
        };
    };

    /**
     * @brief Settings for Enemy GameObject.
     */

    struct EnemySettings {
        inline static const QList<QPair<DataRole, QVariant>> defaultData {
          {DataRole::Type, QVariant::fromValue<ObjectType>(ObjectType::Enemy)},
          {DataRole::Health, Health::SETTINGS::MAX_HEALTH},
          {DataRole::Strength, (float)Attack::SETTINGS::ENEMY_STRENGTH},
        };
        /**
         * @brief Set up a GameObject as an Enemy with predefined settings and behaviors.
         * @param obj Pointer to the GameObject to be set up.
         */
        static void setObject(GameObject *obj) {
            obj->setData(defaultData);
            obj->setBehavior<Attack>(QSharedPointer<CounterAttackBehavior>::create(obj));
            obj->setBehavior<Health>(QSharedPointer<GenericHealthBehavior>::create(obj));
        };
    };

    /**
     * @brief Settings for PoisonEnemy GameObject.
     */

    struct PoisonEnemySettings {
        inline static const QList<QPair<DataRole, QVariant>> defaultData {
          {DataRole::Type, QVariant::fromValue<ObjectType>(ObjectType::PoisonEnemy)},
          {DataRole::Health, Health::SETTINGS::MAX_HEALTH},
          {DataRole::Strength, Attack::SETTINGS::ENEMY_STRENGTH},
          {DataRole::PoisonLevel, Poison::SETTINGS::PENEMY_POISON_LEVEL},
        };
        /**
         * @brief Set up a GameObject as a PoisonEnemy with predefined settings and behaviors.
         * @param obj Pointer to the GameObject to be set up.
         */
        static void setObject(GameObject *obj) {
            obj->setData(defaultData);
            obj->setBehavior<Attack>(QSharedPointer<CounterAttackBehavior>::create(obj));
            obj->setBehavior<Health>(QSharedPointer<PoisonOnKilledBehavior>::create(obj));
            obj->setBehavior<Poison>(QSharedPointer<GenericPoisoningBehavior>::create(obj));
        };
    };

    /**
     * @brief Settings for MovingEnemy GameObject.
     */
    struct MovingEnemySettings {
        inline static const QList<QPair<DataRole, QVariant>> defaultData {
          {DataRole::Type, QVariant::fromValue<ObjectType>(ObjectType::MovingEnemy)},
          {DataRole::Health, Health::SETTINGS::MAX_HEALTH},
          {DataRole::Strength, Attack::SETTINGS::ENEMY_STRENGTH},
          {DataRole::Energy, Movement::SETTINGS::MAX_ENERGY * 10},

        };
        /**
         * @brief Set up a GameObject as a MovingEnemy with predefined settings and behaviors.
         * @param obj Pointer to the GameObject to be set up.
         */
        static void setObject(GameObject *obj) {
            obj->setData(defaultData);
            obj->setBehavior<Attack>(QSharedPointer<CounterAttackBehavior>::create(obj));
            obj->setBehavior<Health>(QSharedPointer<GenericHealthBehavior>::create(obj));
            obj->setBehavior<Movement>(QSharedPointer<RandomMovementBehavior>::create(obj));
        };
    };

public:
    /**
     * @brief Gets the setup function for a specific type of GameObject.
     * @param type The ObjectType of the GameObject.
     * @return A function to set up the GameObject with its specific settings and behaviors.
     */
    static std::function<void(GameObject *)> getFunction(ObjectType type) {
        switch(type) {
        case ObjectType::Tile:
            return TileSettings::setObject;
        case ObjectType::Doorway:
            return DoorSettings::setObject;
        case ObjectType::HealthPack:
            return HealthPackSettings::setObject;
        case ObjectType::Protagonist:
            return ProtagonistSettings::setObject;
        case ObjectType::Enemy:
            return EnemySettings::setObject;
        case ObjectType::PoisonEnemy:
            return PoisonEnemySettings::setObject;
        case ObjectType::MovingEnemy:
            return MovingEnemySettings::setObject;
        default:
            return EnemySettings::setObject;
        }
    }
};

#endif // GAMEOBJECTSETTINGS_H
