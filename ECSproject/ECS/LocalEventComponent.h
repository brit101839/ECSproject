#pragma once

#include "ECS.h"
#include "../EventSystem.h"

class LocalEventComponent :public Component {
private:
	EventSystem _eventManager;

public:
	LocalEventComponent() = default;

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
};