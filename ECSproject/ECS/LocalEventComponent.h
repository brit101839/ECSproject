#pragma once

#include "ECS.h"
#include "../EventSystem.h"

class LocalEventComponent :public Component {
private:
	EventSystem& _globalEventsys;
	EventSystem _eventManager;

public:
	LocalEventComponent(EventSystem& g) : _globalEventsys(g) {};

	template<typename EventType>
	void subscribe(std::function<void(Event&)> callBack) {
		_eventManager.subscribe<EventType>(callBack);
	}

	template <typename EventType>
	void publish(EventType& event) {
		_eventManager.publish(event);
	}

	EventSystem& getEventSystem() {
		return _eventManager;
	}

	void destroyEntity() {
		DestroyEntityEvent event(entity);
		_globalEventsys.publish<DestroyEntityEvent>(event);
		entity->destroy(); // Destroy the entity when the explosion is complete
	}
};