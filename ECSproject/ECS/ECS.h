//
// ECS.h

#ifndef __ECS__
#define __ECS__

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>
#include "../vector2D/Vector2D.h"
#include "../shader/Shader.h"

class Component;
class Entity;
class Manager;

using ComponentID = std::size_t;
using Group = std::size_t;

inline ComponentID getNewComponentTypeID() 
{
	static ComponentID lastID = 0;
	return lastID++;
}

template <typename T> 
inline ComponentID getComponentTypeID() noexcept
{
	static ComponentID typeID = getNewComponentTypeID();
	return typeID;
}

constexpr std::size_t maxComponents = 32;
constexpr std::size_t maxGroup = 32;

using ComponentBitSet = std::bitset<maxComponents>;
using GroupBitSet = std::bitset<maxGroup>;

using ComponentArray = std::array<Component*, maxComponents>;

class Component
{
public:
	Entity* entity;

	virtual void init() {};
	virtual void update(GLFWwindow* window, double deltaTime) {};
	virtual void draw(Shader& shader, Vector2D cameraPos) {};

	virtual ~Component() {};
};

class Entity
{
private:

	Manager& _manager;

	bool _active = true;
	std::vector<std::unique_ptr<Component>> _components;

	ComponentArray _componentArray;
	ComponentBitSet _componentBitSet;
	GroupBitSet _groupBitSet;

public:

	Entity(Manager& mManager) : _manager(mManager) {}

	void update(GLFWwindow* window, double deltaTime)
	{
		for (auto& c : _components) c->update(window, deltaTime);
	}
	void draw(Shader& shader, Vector2D cameraPos)
	{
		for (auto& c : _components) c->draw(shader, cameraPos);
	}

	bool isActive() const { return _active; }
	void destroy() { _active = false; }

	bool hasGroup(Group mGroup) { return _groupBitSet[mGroup]; }
	void addGroup(Group mGroup);
	void delGroup(Group mGroup)
	{
		_groupBitSet[mGroup] = false;
	}

	template <typename T>
	bool hasComponent() const
	{
		return _componentBitSet[getComponentTypeID<T>()];
	}

	template <typename T, typename... TArgs>
	T& addcomponent(TArgs&&... mArgs)
	{
		T* c(new T(std::forward<TArgs>(mArgs)...));
		c->entity = this;
		std::unique_ptr<Component> uPrt{ c };
		_components.emplace_back(std::move(uPrt));

		_componentArray[getComponentTypeID<T>()] = c;
		_componentBitSet[getComponentTypeID<T>()] = true;

		c->init();
		return *c;
	}

	template<typename T> 
	T& getComponent() const
	{
		auto ptr(_componentArray[getComponentTypeID<T>()]);
		if (ptr == nullptr) {
			throw std::runtime_error("Component not found!");
		}
		return *static_cast<T*>(ptr);
	}

	
};

class Manager
{
private:

	std::vector<std::unique_ptr<Entity>> entities;
	std::array<std::vector<Entity*>, maxGroup> groupedEntities;

public:

	void update(GLFWwindow* window, double deltaTime)
	{
		for (auto& e : entities) e->update(window, deltaTime);
	}

	void draw(Shader& shader, Vector2D cameraPos)
	{
		for (auto& e : entities) e->draw(shader, cameraPos);
	}

	void refresh()
	{
		for (auto i(0u); i < maxGroup; i++) 
		{
			auto& v(groupedEntities[i]);
			v.erase(
				std::remove_if(std::begin(v), std::end(v),
					[i](Entity* mEntity)
					{
						return !mEntity->isActive() || !mEntity->hasGroup(i);
					}
				), std::end(v)
			);
		}

		entities.erase(std::remove_if(std::begin(entities), std::end(entities),
			[](const std::unique_ptr<Entity>& mEntity)
			{
				return !mEntity->isActive();
			}),
			std::end(entities));
	}

	void AddToGroup(Entity* mEntity, Group mGroup)
	{
		groupedEntities[mGroup].emplace_back(mEntity);
	}

	std::vector<Entity*>& getGroup(Group mGroup)
	{
		return groupedEntities[mGroup];
	}

	Entity& addEntity()
	{
		Entity* e = new Entity(*this);
		std::unique_ptr<Entity> uPtr{ e };
		entities.emplace_back(std::move(uPtr));
		return *e;
	}
}; 

#endif // !__ECS__