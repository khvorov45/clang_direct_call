//===- llvm/CodeGen/MachineRegionInfo.h -------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CODEGEN_MACHINEREGIONINFO_H
#define LLVM_CODEGEN_MACHINEREGIONINFO_H

#include "llvm_include_llvm_ADT_DepthFirstIterator.h"
#include "llvm_include_llvm_Analysis_RegionInfo.h"
#include "llvm_include_llvm_Analysis_RegionIterator.h"
#include "llvm_include_llvm_CodeGen_MachineBasicBlock.h"
#include "llvm_include_llvm_CodeGen_MachineDominanceFrontier.h"
#include "llvm_include_llvm_CodeGen_MachineDominators.h"
#include "llvm_include_llvm_CodeGen_MachineFunction.h"
#include "llvm_include_llvm_CodeGen_MachineFunctionPass.h"
#include "llvm_include_llvm_CodeGen_MachineLoopInfo.h"
#include <cassert>

namespace llvm {

class MachinePostDominatorTree;
class MachineRegion;
class MachineRegionNode;
class MachineRegionInfo;

template <> struct RegionTraits<MachineFunction> {
  using FuncT = MachineFunction;
  using BlockT = MachineBasicBlock;
  using RegionT = MachineRegion;
  using RegionNodeT = MachineRegionNode;
  using RegionInfoT = MachineRegionInfo;
  using DomTreeT = MachineDominatorTree;
  using DomTreeNodeT = MachineDomTreeNode;
  using PostDomTreeT = MachinePostDominatorTree;
  using DomFrontierT = MachineDominanceFrontier;
  using InstT = MachineInstr;
  using LoopT = MachineLoop;
  using LoopInfoT = MachineLoopInfo;

  static unsigned getNumSuccessors(MachineBasicBlock *BB) {
    return BB->succ_size();
  }
};

class MachineRegionNode : public RegionNodeBase<RegionTraits<MachineFunction>> {
public:
  inline MachineRegionNode(MachineRegion *Parent, MachineBasicBlock *Entry,
                           bool isSubRegion = false)
      : RegionNodeBase<RegionTraits<MachineFunction>>(Parent, Entry,
                                                      isSubRegion) {}

  bool operator==(const MachineRegion &RN) const {
    return this == reinterpret_cast<const MachineRegionNode *>(&RN);
  }
};

class MachineRegion : public RegionBase<RegionTraits<MachineFunction>> {
public:
  MachineRegion(MachineBasicBlock *Entry, MachineBasicBlock *Exit,
                MachineRegionInfo *RI, MachineDominatorTree *DT,
                MachineRegion *Parent = nullptr);
  ~MachineRegion();

  bool operator==(const MachineRegionNode &RN) const {
    return &RN == reinterpret_cast<const MachineRegionNode *>(this);
  }
};

class MachineRegionInfo : public RegionInfoBase<RegionTraits<MachineFunction>> {
public:
  explicit MachineRegionInfo();
  ~MachineRegionInfo() override;

  // updateStatistics - Update statistic about created regions.
  void updateStatistics(MachineRegion *R) final;

  void recalculate(MachineFunction &F, MachineDominatorTree *DT,
                   MachinePostDominatorTree *PDT, MachineDominanceFrontier *DF);
};

class MachineRegionInfoPass : public MachineFunctionPass {
  MachineRegionInfo RI;

public:
  static char ID;

  explicit MachineRegionInfoPass();
  ~MachineRegionInfoPass() override;

  MachineRegionInfo &getRegionInfo() { return RI; }

  const MachineRegionInfo &getRegionInfo() const { return RI; }

  /// @name MachineFunctionPass interface
  //@{
  bool runOnMachineFunction(MachineFunction &F) override;
  void releaseMemory() override;
  void verifyAnalysis() const override;
  void getAnalysisUsage(AnalysisUsage &AU) const override;
  void print(raw_ostream &OS, const Module *) const override;
  void dump() const;
  //@}
};

template <>
template <>
inline MachineBasicBlock *
RegionNodeBase<RegionTraits<MachineFunction>>::getNodeAs<MachineBasicBlock>()
    const {
  assert(!isSubRegion() && "This is not a MachineBasicBlock RegionNode!");
  return getEntry();
}

template <>
template <>
inline MachineRegion *
RegionNodeBase<RegionTraits<MachineFunction>>::getNodeAs<MachineRegion>()
    const {
  assert(isSubRegion() && "This is not a subregion RegionNode!");
  auto Unconst =
      const_cast<RegionNodeBase<RegionTraits<MachineFunction>> *>(this);
  return reinterpret_cast<MachineRegion *>(Unconst);
}

RegionNodeGraphTraits(MachineRegionNode, MachineBasicBlock, MachineRegion);
RegionNodeGraphTraits(const MachineRegionNode, MachineBasicBlock,
                      MachineRegion);

RegionGraphTraits(MachineRegion, MachineRegionNode);
RegionGraphTraits(const MachineRegion, const MachineRegionNode);

template <>
struct GraphTraits<MachineRegionInfo *>
    : public GraphTraits<FlatIt<MachineRegionNode *>> {
  using nodes_iterator = df_iterator<NodeRef, df_iterator_default_set<NodeRef>,
                                     false, GraphTraits<FlatIt<NodeRef>>>;

  static NodeRef getEntryNode(MachineRegionInfo *RI) {
    return GraphTraits<FlatIt<MachineRegion *>>::getEntryNode(
        RI->getTopLevelRegion());
  }

  static nodes_iterator nodes_begin(MachineRegionInfo *RI) {
    return nodes_iterator::begin(getEntryNode(RI));
  }

  static nodes_iterator nodes_end(MachineRegionInfo *RI) {
    return nodes_iterator::end(getEntryNode(RI));
  }
};

template <>
struct GraphTraits<MachineRegionInfoPass *>
    : public GraphTraits<MachineRegionInfo *> {
  using nodes_iterator = df_iterator<NodeRef, df_iterator_default_set<NodeRef>,
                                     false, GraphTraits<FlatIt<NodeRef>>>;

  static NodeRef getEntryNode(MachineRegionInfoPass *RI) {
    return GraphTraits<MachineRegionInfo *>::getEntryNode(&RI->getRegionInfo());
  }

  static nodes_iterator nodes_begin(MachineRegionInfoPass *RI) {
    return GraphTraits<MachineRegionInfo *>::nodes_begin(&RI->getRegionInfo());
  }

  static nodes_iterator nodes_end(MachineRegionInfoPass *RI) {
    return GraphTraits<MachineRegionInfo *>::nodes_end(&RI->getRegionInfo());
  }
};

extern template class RegionBase<RegionTraits<MachineFunction>>;
extern template class RegionNodeBase<RegionTraits<MachineFunction>>;
extern template class RegionInfoBase<RegionTraits<MachineFunction>>;

} // end namespace llvm

#endif // LLVM_CODEGEN_MACHINEREGIONINFO_H
