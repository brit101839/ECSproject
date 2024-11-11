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

class Component;
class Entity;

using ComponentID = std::size_t;

inline ComponentID getComponentTypeID() 
{
	static ComponentID lastID = 0;
	return lastID++;
}

template <typename T> 
inline ComponentID getComponentTypeID() noexcept
{
	static ComponentID typeID = getComponentTypeID();
	return typeID;
}

constexpr std::size_t maxComponents = 32;

using ComponentBitSet = std::bitset<maxComponents>;
using ComponentArray = std::array<Component*, maxComponents>;

class Component
{
public:
	Entity* entity;

	virtual void init() {};
	virtual void update(GLFWwindow* window) {};
	virtual void draw() {};

	virtual ~Component() {};
};

class Entity
{
private:

	bool _active = true;
	std::vector<std::unique_ptr<Component>> _components;

	ComponentArray _componentArray;
	ComponentBitSet _componentBitSet;

public:

	void update(GLFWwindow* window)
	{
		for (auto& c : _components) c->update(window);
	}
	void draw() 
	{
		for (auto& c : _components) c->draw();
	}

	bool isActive() const { return _active; }
	void destroy() { _active = false; }

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

public:

	void update(GLFWwindow* window)
	{
		for (auto& e : entities) e->update(window);
	}

	void draw()
	{
		for (auto& e : entities) e->draw();
	}

	void refresh()
	{
		entities.erase(std::remove_if(std::begin(entities), std::end(entities),
			[](const std::unique_ptr<Entity>& mEntity)
			{
				return !mEntity->isActive();
			}),
			std::end(entities));
	}

	Entity& addEntity()
	{
		Entity* e = new Entity();
		std::unique_ptr<Entity> uPtr{ e };
		entities.emplace_back(std::move(uPtr));
		return *e;
	}
}; 

#endif // !__ECS__