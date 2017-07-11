#pragma once

#include <cstddef>
#include "ISystem.hpp"

namespace kengine
{
    using IgnoreComponents = std::nullptr_t;
    using AllComponents = void;

    template<typename CRTP, typename RegisteredComponent = IgnoreComponents, typename ...DataPackets>
    class System : public ISystem, public putils::Module<CRTP, DataPackets...>
    {
    protected:
        using super = System<CRTP, RegisteredComponent, DataPackets...>;

        void execute() override {}

        void registerGameObject(GameObject &go) override
        {
            const auto it = std::find(_gameObjects.begin(), _gameObjects.end(), &go);
            if (it == _gameObjects.end())
                _gameObjects.push_back(&go);
        }

        void removeGameObject(GameObject &go) override
        {
            const auto it = std::find(_gameObjects.begin(), _gameObjects.end(), &go);
            if (it != _gameObjects.end())
                _gameObjects.erase(it);
        }

        bool hasGameObject(GameObject &go) const noexcept
        { return std::find(_gameObjects.begin(), _gameObjects.end(), &go) != _gameObjects.end(); }

    protected:
        using ISystem::ISystem;
        std::vector<GameObject*> &getGameObjects() { return _gameObjects; }
        const std::vector<GameObject*> &getGameObjects() const { return _gameObjects; }

    public:
        pmeta::type_index getCompType() const noexcept final
        {
            static_assert(std::is_base_of<System, CRTP>::value, "System's first template parameter should be inheriting class");
            return pmeta::type<RegisteredComponent>::index;
        }

        pmeta::type_index getType() const noexcept final
        {
            return pmeta::type<CRTP>::index;
        }

    private:
        std::vector<GameObject*> _gameObjects;
    };
}