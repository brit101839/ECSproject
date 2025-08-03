#include "TreeBuilder.h"
#include "FightingTree.h"
#include "BehaviorTreeFactory.h"

BehaviorTree<AIstate&, AIContext&> BehaviorTreeFactory::buildTree()
{
	/*auto wrapPeelAIstate = [](std::unique_ptr<BehaviorNode<AIstate&>>&& child) {
			auto peelAIstate = std::make_unique<PeelAIstate>();
			peelAIstate->setChild(std::move(child));
			return std::move(peelAIstate);
			};
		auto wrapPeelAIContext = [](std::unique_ptr<BehaviorNode<AIContext&>>&& child) {
			auto peelAIContext = std::make_unique<PeelAIContext>();
			peelAIContext->setChild(std::move(child));
			return std::move(peelAIContext);
			};*/

	FightingTreeFactory fightingTreeFactory;
	auto fightingTree = fightingTreeFactory.buildTree();

	auto tree = BehaviorTree<AIstate&, AIContext&>();

	tree.setRoot(TreeBuilder::FallbackNodeList(
		TreeBuilder::SequenceNodeList(
			TreeBuilder::FallbackNodeList(
				TreeBuilder::PeelState(std::make_unique<IsFighting>()),
				TreeBuilder::PeelContext(std::make_unique<TryFighting>())
			),
			TreeBuilder::PeelState(std::make_unique<SetFighingState>()),
			TreeBuilder::PeelContext(std::move(fightingTree)),
			TreeBuilder::Invert(TreeBuilder::PeelContext(std::make_unique<TryFighting>()))
		),
		TreeBuilder::SequenceNodeList(
			TreeBuilder::PeelContext(std::make_unique<TryTracking>()),
			TreeBuilder::PeelContext(std::make_unique<Tracking>())
		),
		TreeBuilder::SequenceNodeList(
			TreeBuilder::PeelContext(std::make_unique<TryBacking>()),
			TreeBuilder::PeelContext(std::make_unique<Backing>())
		),
		TreeBuilder::PeelContext(std::make_unique<Patrol>())
	));

	return tree;
}