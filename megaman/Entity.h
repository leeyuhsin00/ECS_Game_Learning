#pragma once

#include "Components.h"
#include <tuple>
#include <string>
#include <optional>
#include <type_traits>

typedef std::tuple<
    std::optional<CTransform>,
    std::optional<CLifespan>,
    std::optional<CInput>,
    std::optional<CBoundingBox>,
    std::optional<CAnimation>,
    std::optional<CGravity>,
    std::optional<CState>,
    std::optional<CJumpTimer>,
    std::optional<CFriction>
> ComponentTuple;

/*  Entity class to contain the different components: optional because not every class has every component
    member variables: m_active - displayed if active, vice versa; m_tag - type of entity ("Player", "Enemy",
    "Tile") 
    private constructor + friend EntityManager so only entity manager can create entities
    member functions: destroy() - set m_active to false;
*/

class Entity
{
private:
    friend class EntityManager;

    ComponentTuple        m_components;
    bool                  m_active        {true};
    size_t                m_id            {0};
    std::string           m_tag           {"default"};

    Entity(const size_t& id, const std::string& tag);

public:
    bool isActive() const;
    const std::string&    tag()           const;
    const size_t          id()            const;
    void                  destroy();

    template <typename T>
    bool hasComponent() const
    {
        if constexpr (std::is_same_v<T, CTransform>) return std::get<std::optional<CTransform>>(m_components).has_value();
        else if constexpr (std::is_same_v<T, CLifespan>) return std::get<std::optional<CLifespan>>(m_components).has_value();
        else if constexpr (std::is_same_v<T, CInput>) return std::get<std::optional<CInput>>(m_components).has_value();
        else if constexpr (std::is_same_v<T, CBoundingBox>) return std::get<std::optional<CBoundingBox>>(m_components).has_value();
        else if constexpr (std::is_same_v<T, CAnimation>) return std::get<std::optional<CAnimation>>(m_components).has_value();
        else if constexpr (std::is_same_v<T, CGravity>) return std::get<std::optional<CGravity>>(m_components).has_value();
        else if constexpr (std::is_same_v<T, CState>) return std::get<std::optional<CState>>(m_components).has_value();
        else if constexpr (std::is_same_v<T, CJumpTimer>) return std::get<std::optional<CJumpTimer>>(m_components).has_value();
        else if constexpr (std::is_same_v<T, CFriction>) return std::get<std::optional<CFriction>>(m_components).has_value();
        else static_assert(false, "Component type not in ComponentTuple");
    }

    template <typename T, typename... TArgs>
    T& addComponent(TArgs&&... mArgs)
    {
        if constexpr (std::is_same_v<T, CTransform>) return std::get<std::optional<CTransform>>(m_components).emplace(std::forward<TArgs>(mArgs)...);
        else if constexpr (std::is_same_v<T, CLifespan>) return std::get<std::optional<CLifespan>>(m_components).emplace(std::forward<TArgs>(mArgs)...);
        else if constexpr (std::is_same_v<T, CInput>) return std::get<std::optional<CInput>>(m_components).emplace(std::forward<TArgs>(mArgs)...);
        else if constexpr (std::is_same_v<T, CBoundingBox>) return std::get<std::optional<CBoundingBox>>(m_components).emplace(std::forward<TArgs>(mArgs)...);
        else if constexpr (std::is_same_v<T, CAnimation>) return std::get<std::optional<CAnimation>>(m_components).emplace(std::forward<TArgs>(mArgs)...);
        else if constexpr (std::is_same_v<T, CGravity>) return std::get<std::optional<CGravity>>(m_components).emplace(std::forward<TArgs>(mArgs)...);
        else if constexpr (std::is_same_v<T, CState>) return std::get<std::optional<CState>>(m_components).emplace(std::forward<TArgs>(mArgs)...);
        else if constexpr (std::is_same_v<T, CJumpTimer>) return std::get<std::optional<CJumpTimer>>(m_components).emplace(std::forward<TArgs>(mArgs)...);
        else if constexpr (std::is_same_v<T, CFriction>) return std::get<std::optional<CFriction>>(m_components).emplace(std::forward<TArgs>(mArgs)...);
        else static_assert(false, "Component type not in ComponentTuple");
    }

    template <typename T>
    T& getComponent()
    {
        if constexpr (std::is_same_v<T, CTransform>) return std::get<std::optional<CTransform>>(m_components).value();
        else if constexpr (std::is_same_v<T, CLifespan>) return std::get<std::optional<CLifespan>>(m_components).value();
        else if constexpr (std::is_same_v<T, CInput>) return std::get<std::optional<CInput>>(m_components).value();
        else if constexpr (std::is_same_v<T, CBoundingBox>) return std::get<std::optional<CBoundingBox>>(m_components).value();
        else if constexpr (std::is_same_v<T, CAnimation>) return std::get<std::optional<CAnimation>>(m_components).value();
        else if constexpr (std::is_same_v<T, CGravity>) return std::get<std::optional<CGravity>>(m_components).value();
        else if constexpr (std::is_same_v<T, CState>) return std::get<std::optional<CState>>(m_components).value();
        else if constexpr (std::is_same_v<T, CJumpTimer>) return std::get<std::optional<CJumpTimer>>(m_components).value();
        else if constexpr (std::is_same_v<T, CFriction>) return std::get<std::optional<CFriction>>(m_components).value();
        else static_assert(false, "Component type not in ComponentTuple");
    }

    template <typename T>
    const T& getComponent() const
    {
        if constexpr (std::is_same_v<T, CTransform>) return std::get<std::optional<CTransform>>(m_components).value();
        else if constexpr (std::is_same_v<T, CLifespan>) return std::get<std::optional<CLifespan>>(m_components).value();
        else if constexpr (std::is_same_v<T, CInput>) return std::get<std::optional<CInput>>(m_components).value();
        else if constexpr (std::is_same_v<T, CBoundingBox>) return std::get<std::optional<CBoundingBox>>(m_components).value();
        else if constexpr (std::is_same_v<T, CAnimation>) return std::get<std::optional<CAnimation>>(m_components).value();
        else if constexpr (std::is_same_v<T, CGravity>) return std::get<std::optional<CGravity>>(m_components).value();
        else if constexpr (std::is_same_v<T, CState>) return std::get<std::optional<CState>>(m_components).value();
        else if constexpr (std::is_same_v<T, CJumpTimer>) return std::get<std::optional<CJumpTimer>>(m_components).value();
        else if constexpr (std::is_same_v<T, CFriction>) return std::get<std::optional<CFriction>>(m_components).value();
        else static_assert(false, "Component type not in ComponentTuple");
    }

    template <typename T>
    void removeComponent()
    {
        if constexpr (std::is_same_v<T, CTransform>) std::get<std::optional<CTransform>>(m_components).reset();
        else if constexpr (std::is_same_v<T, CLifespan>) std::get<std::optional<CLifespan>>(m_components).reset();
        else if constexpr (std::is_same_v<T, CInput>) std::get<std::optional<CInput>>(m_components).reset();
        else if constexpr (std::is_same_v<T, CBoundingBox>) std::get<std::optional<CBoundingBox>>(m_components).reset();
        else if constexpr (std::is_same_v<T, CAnimation>) std::get<std::optional<CAnimation>>(m_components).reset();
        else if constexpr (std::is_same_v<T, CGravity>) std::get<std::optional<CGravity>>(m_components).reset();
        else if constexpr (std::is_same_v<T, CState>) std::get<std::optional<CState>>(m_components).reset();
        else if constexpr (std::is_same_v<T, CJumpTimer>) std::get<std::optional<CJumpTimer>>(m_components).reset();
        else if constexpr (std::is_same_v<T, CFriction>) std::get<std::optional<CFriction>>(m_components).reset();
        else static_assert(false, "Component type not in ComponentTuple");
    }
};
