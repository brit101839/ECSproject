#pragma once

#include "tinybehaviortree.h"
#include "../ECS/AIContext.h"
#include <memory>
#include <utility>

using namespace TBT;

TBT_PEEL_NODE_MACRO(PeelAIstate, TBT_ARGS(AIstate&, AIContext&), AIstate&, { std::get<0>(payload) })
TBT_PEEL_NODE_MACRO(PeelAIContext, TBT_ARGS(AIstate&, AIContext&), AIContext&, { std::get<1>(payload) })

class TreeBuilder
{
public:
	template<typename T>
	static std::unique_ptr<BehaviorNode<AIContext&>> Wrap(std::unique_ptr<T>&& node) {
		return std::unique_ptr<BehaviorNode<AIContext&>>(std::move(node));
	}

	template<typename Base, typename Derived>
	static std::unique_ptr<Base> static_unique_ptr_cast(std::unique_ptr<Derived>&& d) {
		return std::unique_ptr<Base>(static_cast<Base*>(d.release()));
	}

	template<typename... Args>
	static auto FallbackNodeList(Args&&... args) {
		auto node = std::make_unique<FallbackNode<AIstate&, AIContext&>>();
		(node->addChild(std::forward<Args>(args)), ...);
		return node;
	}

	template<typename... Args>
	static auto SequenceNodeList(Args&&... args) {
		auto node = std::make_unique<SequenceNode<AIstate&, AIContext&>>();
		(node->addChild(std::forward<Args>(args)), ...);
		return node;
	}

	template<typename T>
	static auto Invert(std::unique_ptr<T>&& node) {
		auto invert = std::make_unique<InvertNode<AIstate&, AIContext&>>();
		invert->setChild(std::move(node));
		return invert;
	}

	template<typename T>
	static auto PeelState(std::unique_ptr<T>&& node) {
		auto peel = std::make_unique<PeelAIstate>();
		peel->setChild(std::move(node));
		return peel;
	}

	template<typename T>
	static auto PeelContext(std::unique_ptr<T>&& node) {
		auto peel = std::make_unique<PeelAIContext>();
		peel->setChild(std::move(node));
		return peel;
	}
};