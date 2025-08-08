#include "TreeBuilder.h"
#include "FightingTree.h"

std::unique_ptr<BehaviorNode<AIstate&, AIContext&>> FightingTreeFactory::buildFightingTree(void)
{
	std::unique_ptr<FallbackNode<AIstate&, AIContext&>> rootNode = TreeBuilder::FallbackNodeList(
		TreeBuilder::SequenceNodeList(
			TreeBuilder::PeelContext(std::make_unique<CanUseSkill>()),
			TreeBuilder::PeelContext(std::make_unique<ShouldUseSkill>()),
			TreeBuilder::PeelContext(std::make_unique<UsingSkill>())
		),
		TreeBuilder::SequenceNodeList(
			TreeBuilder::PeelContext(std::make_unique<CanNormalAttack>()),
			TreeBuilder::PeelContext(std::make_unique<ShouldNormalAttack>()),
			TreeBuilder::PeelContext(std::make_unique<UseNormalAttack>())
		)
	);

	return rootNode;
}
