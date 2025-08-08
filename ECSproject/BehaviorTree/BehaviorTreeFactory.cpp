#include "TreeBuilder.h"
#include "FightingTree.h"
#include "BehaviorTreeFactory.h"

BehaviorResult TryExitFighting::tick(AIstate& state, AIContext& context)
{
	if (context.getAttack()->attacking) {
		// std::cout << "should not exit fighting" << std::endl;
		return BehaviorResult::RUNNING;
	}
	else if (context.getSkill()->usingSkill) {
		return BehaviorResult::RUNNING;
	}
	else if (context.getDist() < 100.f) {
		return BehaviorResult::FAILURE;
	}
	// state.fighting = false;
	// std::cout << "should exit fighting" << std::endl;
	return BehaviorResult::SUCCESS;
}

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

	BehaviorTree<AIstate&, AIContext&> tree = BehaviorTree<AIstate&, AIContext&>();

	tree.setRoot(TreeBuilder::FallbackNodeList(
		TreeBuilder::SequenceNodeList(
			// try get in fighting state
			TreeBuilder::FallbackNodeList(
				// if is fighting continous, Success
				TreeBuilder::PeelState(std::make_unique<IsFighting>()),
				// if not try get in fighting state
				TreeBuilder::PeelContext(std::make_unique<TryFighting>())
			),

			// if into fighting state, execute fighting tree
			TreeBuilder::PeelState(std::make_unique<SetFighingState>()),	// set fighting state
			std::move(fightingTreeFactory.buildFightingTree()),						// run fighting tree

			// if can't fighting, the fighting flow end. Run the following behavior
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
