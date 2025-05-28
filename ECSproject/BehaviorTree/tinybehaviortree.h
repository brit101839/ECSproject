#ifndef TINYBEHAVIORTREE_H
#define TINYBEHAVIORTREE_H

#include <memory>
#include <vector>
#include <tuple>
#include <string>
#include <iostream>

namespace TBT{

enum class BehaviorResult {
	IDLE = 0,
	RUNNING,
	SUCCESS,
	FAILURE
};

class BehaviorNodeBase{
public:

protected:
	BehaviorNodeBase* parent = nullptr;
	static void setParent(BehaviorNodeBase* child, BehaviorNodeBase* parent){child->parent = parent;}
};

template<typename... Payload>
class BehaviorNode : public BehaviorNodeBase {
public:
	BehaviorNode(){}
	BehaviorNode(const BehaviorNode&) = delete;
	virtual BehaviorResult tick(Payload...) = 0;
};

template<typename... Payload>
class SequenceNode : public BehaviorNode<Payload...> {
public:
	SequenceNode() : currentIndex(0){}
	SequenceNode(const SequenceNode&) = delete;
	void addChild(std::unique_ptr<BehaviorNode<Payload...>>&& node) {
		BehaviorNodeBase::setParent(&*node, this);
		children.push_back(std::move(node));
	}
	BehaviorResult tick(Payload... payload) override {
		// for (auto it = children.begin(); it != children.end(); ++it) {
		for (size_t i = currentIndex; i < children.size(); ++i) {
			auto& node = children[i];
			// auto& node = *it;
			auto result = node->tick(payload...);
			if (result == BehaviorResult::FAILURE) {
				currentIndex = 0;  // Reset the index on failure
				return BehaviorResult::FAILURE;
			}
			if (result == BehaviorResult::RUNNING) {
				currentIndex = i;  // Remember the current index if RUNNING
				return BehaviorResult::RUNNING;
			}
		}
		currentIndex = 0;  // Reset the index on failure
		return BehaviorResult::SUCCESS;
	}
protected:
	std::vector<std::unique_ptr<BehaviorNode<Payload...>>> children;
	size_t currentIndex = 0; // Track the index of the currently running child node
};

template<typename... Payload>
class FallbackNode : public BehaviorNode<Payload...> {
public:
	FallbackNode() : currentIndex(0){}
	FallbackNode(const FallbackNode&) = delete;
	void addChild(std::unique_ptr<BehaviorNode<Payload...>>&& node) {
		BehaviorNodeBase::setParent(&*node, this);
		children.push_back(std::move(node));
	}
	BehaviorResult tick(Payload... payload) override {
		for (size_t i = currentIndex; i < children.size(); ++i) {
		// for (auto it = children.begin(); it != children.end(); ++it) {
			auto& node = children[i];
			// auto& node = *it;
			auto result = node->tick(payload...);
			if (result == BehaviorResult::SUCCESS) {
				currentIndex = 0;
				return BehaviorResult::SUCCESS;
			}
			if (result == BehaviorResult::RUNNING) {
				currentIndex = i;
				return BehaviorResult::RUNNING;
			}
				
		}
		currentIndex = 0;  // Reset the index on failure
		return BehaviorResult::FAILURE;
	}
protected:
	std::vector<std::unique_ptr<BehaviorNode<Payload...>>> children;
	size_t currentIndex = 0; // Track the index of the currently running child node
};


template<typename... Payload>
class PeelNodeBase : public BehaviorNode<Payload...> {
public:
	using TuplePayload = std::tuple<Payload...>;
	BehaviorResult tick(Payload... payload) override {
		return tickTuple(TuplePayload(payload...));
	}

	virtual BehaviorResult tickTuple(const TuplePayload&) = 0;
};

template<typename... Args>
class InvertNode : public BehaviorNode<Args...> {
public:
	void setChild(std::unique_ptr<BehaviorNode<Args...>>&& node) {
		_child = std::move(node);
	}

	BehaviorResult tick(Args... args) override {
		if (!_child) return BehaviorResult::FAILURE;

		BehaviorResult result = _child->tick(args...);
		switch (result) {
		case BehaviorResult::SUCCESS: return BehaviorResult::FAILURE;
		case BehaviorResult::FAILURE: return BehaviorResult::SUCCESS;
		case BehaviorResult::RUNNING: return BehaviorResult::RUNNING;
		}
		return BehaviorResult::FAILURE; // fallback
	}

private:
	std::unique_ptr<BehaviorNode<Args...>> _child;
};

template<typename... Args>
class LoggingDecorator : public BehaviorNode<Args...> {
private:
	std::unique_ptr<BehaviorNode<Args...>> _child;
	std::string _name;

public:
	LoggingDecorator(std::string name, std::unique_ptr<BehaviorNode<Args...>> child)
		: _name(std::move(name)), _child(std::move(child)) {
	}

	void setChild(std::unique_ptr<BehaviorNode<Args...>> child) {
		_child = std::move(child);
	}

	BehaviorResult tick(Args&... args) override {
		BehaviorResult result = _child->tick(args...);
		std::cout << "[Log] Node '" << _name << "' returned " << behaviorResultToString(result) << std::endl;
		return result;
	}

	static std::string behaviorResultToString(BehaviorResult result) {
		switch (result) {
		case BehaviorResult::SUCCESS: return "SUCCESS";
		case BehaviorResult::FAILURE: return "FAILURE";
		case BehaviorResult::RUNNING: return "RUNNING";
		default: return "UNKNOWN";
		}
	}
};

// Template metaprogramming helpers to unpack tuple into argument list
template<int ...> struct Seq_ {};
template<int N, int ...S> struct Gens_ : Gens_<N - 1, N - 1, S...> { };
template<int ...S> struct Gens_<0, S...>{ typedef Seq_<S...> type; };


#define TBT_ARGS(...) __VA_ARGS__


#define TBT_PEEL_NODE_MACRO(name, base, sub, func) \
namespace TBT{ \
template<typename SuperNode, typename... SubPayload> \
class name ## PeelNodeMacro : public SuperNode { \
public: \
	using SubNode = BehaviorNode<sub>; \
	using TuplePayload = typename SuperNode::TuplePayload; \
	name ## PeelNodeMacro(){} \
	name ## PeelNodeMacro(const name ## PeelNodeMacro&) = delete; \
	void setChild(std::unique_ptr<SubNode>&& node) { \
		SuperNode::setParent(&*node, this); \
		child = std::move(node); \
	} \
\
	std::tuple<sub> peel(const TuplePayload& payload) { \
		return std::tuple<SubPayload...> func; \
	} \
\
	template<int ...S>\
	BehaviorResult callFunc(Seq_<S...>, std::tuple<sub>& params) \
	{ \
		return child->tick(std::get<S>(params) ...); \
	} \
\
	BehaviorResult tickTuple(const TuplePayload& payload) override { \
		auto peeled = peel(payload); \
		return callFunc(typename Gens_<sizeof...(SubPayload)>::type(), peeled); \
	} \
protected: \
	std::unique_ptr<SubNode> child; \
};\
} /* namespace TBT */\
using name = name ## PeelNodeMacro<PeelNodeBase<base>, sub>;

template<typename... Payload>
class BehaviorTree {
public:
	BehaviorTree(){}
	void setRoot(std::unique_ptr<BehaviorNode<Payload...>>&& node) {
		rootNode = std::move(node);
	}

	void tickRoot(Payload... payload) {
		if(rootNode){
			rootNode->tick(payload...);
		}
	}
protected:
	std::unique_ptr<BehaviorNode<Payload...>> rootNode;
};

}

#endif // TINYBEHAVIORTREE_H
