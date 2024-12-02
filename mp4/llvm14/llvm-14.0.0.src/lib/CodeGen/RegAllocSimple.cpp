//===----------------------------------------------------------------------===//
//
/// A register allocator simplified from RegAllocFast.cpp
//
//===----------------------------------------------------------------------===//

#include <map>
#include <set>

#include "llvm/ADT/Statistic.h"
#include "llvm/CodeGen/LiveIntervals.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/RegAllocRegistry.h"
#include "llvm/CodeGen/RegisterClassInfo.h"
#include "llvm/CodeGen/SlotIndexes.h"
#include "llvm/CodeGen/TargetInstrInfo.h"
#include "llvm/InitializePasses.h"
#include "llvm/Pass.h"

using namespace llvm;

#define DEBUG_TYPE "regalloc"
#define endl "\n"
#define DBGS(s) dbgs() << s << "@" << __LINE__ << ":\t\t"

STATISTIC(NumStores, "Number of stores added");
STATISTIC(NumLoads, "Number of loads added");

namespace {
/// This is class where you will implement your register allocator in
class RegAllocSimple : public MachineFunctionPass {
   public:
    static char ID;
    RegAllocSimple() : MachineFunctionPass(ID) {}

   private:
    /// Some information that might be useful for register allocation
    /// They are initialized in runOnMachineFunction
    MachineFrameInfo *MFI;
    MachineRegisterInfo *MRI;
    const TargetRegisterInfo *TRI;
    const TargetInstrInfo *TII;
    RegisterClassInfo RegClassInfo;

    // - maintain information about live registers
    using RegSet = DenseSet<uint>;
    DenseMap<Register, MCPhysReg> LiveVirtRegs;
    DenseMap<Register, int> SpillMap;
    DenseSet<MCRegister> UsedInInstr;
    DenseSet<MCRegister> LivePhysRegs;
    DenseSet<Register> DirtyVirtReg;
    DenseSet<MCRegister> ExistingPhysRegs;  // - For Prioritize
    DenseSet<MachineOperand *> VirtRegAcrossFunction;

   public:
    StringRef
    getPassName() const override { return "Simple Register Allocator"; }

    void getAnalysisUsage(AnalysisUsage &AU) const override {
        AU.setPreservesCFG();
        // At -O1/-O2, llc fails to schedule some required passes if this pass
        // does not preserve these anlyses; these are preserved by recomputing
        // them at the end of runOnFunction(), you can safely ignore these
        AU.addRequired<LiveIntervals>();
        AU.addPreserved<LiveIntervals>();
        AU.addRequired<SlotIndexes>();
        AU.addPreserved<SlotIndexes>();
        MachineFunctionPass::getAnalysisUsage(AU);
    }

    /// Ask the Machine IR verifier to check some simple properties
    /// Enabled with the -verify-machineinstrs flag in llc
    MachineFunctionProperties getRequiredProperties() const override {
        return MachineFunctionProperties().set(
            MachineFunctionProperties::Property::NoPHIs);
    }

    MachineFunctionProperties getSetProperties() const override {
        return MachineFunctionProperties().set(
            MachineFunctionProperties::Property::NoVRegs);
    }

    MachineFunctionProperties getClearedProperties() const override {
        return MachineFunctionProperties().set(
            MachineFunctionProperties::Property::IsSSA);
    }

   private:
    /// Allocate physical register for virtual register operand
    void invalidateAndSpill(MachineOperand &MO, detail::DenseMapPair<llvm::Register, llvm::MCPhysReg> &kv, bool isKill = false) {
        auto qwq = allocateStackSlot(kv.first);
        TII->storeRegToStackSlot(*MO.getParent()->getParent(), MO.getParent(), kv.second, isKill, qwq, MRI->getRegClass(kv.first), TRI);
        NumStores++;
        LiveVirtRegs.erase(kv.first);
    }
    bool overlapIn(const DenseSet<MCRegister> &S, const MCPhysReg &R) {
        for (MCRegUnitIterator Units(R, TRI); Units.isValid(); ++Units)
            if (S.contains(*Units))
                return true;
        return false;
    }
    bool overlapUnit(const DenseSet<MCRegister> &S, const MCPhysReg &R) {
        DenseSet<MCRegister> tmp;
        for (auto w : S)
            insertRegUnit(tmp, w);
        return overlapIn(tmp, R);
    }
    bool overlapUnit(const MCPhysReg &R1, const MCPhysReg &R2) {
        DenseSet<MCRegister> tmp;
        insertRegUnit(tmp, R1);
        return overlapIn(tmp, R2);
    }
    void insertRegUnit(DenseSet<MCRegister> &S, const MCPhysReg &PhysReg) {
        for (MCRegUnitIterator Units(PhysReg, TRI); Units.isValid(); ++Units)
            S.insert(*Units);
    }
    MCRegister getUnusedPhysReg(MachineOperand &MO, const llvm::TargetRegisterClass *RC) {
        // - Find unused physical register P
        auto IsExistingPhysReg = [&](MCPhysReg R) {
            return overlapUnit(ExistingPhysRegs, R);
        };
        auto IsLivePhysRegs = [&](MCPhysReg R) {
            return overlapUnit(LivePhysRegs, R);
        };
        auto IsUsedInInstr = [&](MCPhysReg R) {
            return overlapUnit(UsedInInstr, R);
        };
        auto IsAllocByLiveVirtReg = [&](MCPhysReg R) {
            for (auto kv : LiveVirtRegs)
                if (overlapUnit(R, kv.second)) return true;
            return false;
        };
        DBGS("REGFIND") << "Try1" << endl;
        for (auto R : RegClassInfo.getOrder(RC)) {
            if (!IsAllocByLiveVirtReg(R) && !IsExistingPhysReg(R)) {
                // DBGS("REGFIND") << "Looking@" << printReg(R) << endl;
                if (VirtRegAcrossFunction.contains(&MO)) {
                    // - Try allocate callee-saved reg if across calls
                    if (TRI->isCalleeSavedPhysReg(R, *MO.getParent()->getParent()->getParent()))
                        return R;
                } else {
                    // - Try allocate caller-saved reg if not across calls
                    if (!TRI->isCalleeSavedPhysReg(R, *MO.getParent()->getParent()->getParent()))
                        return R;
                }
            }
        }

        DBGS("REGFIND") << "Try2" << endl;
        for (auto R : RegClassInfo.getOrder(RC)) {
            //- Try allocate nonexisting physical reg
            if (!IsAllocByLiveVirtReg(R) && !IsExistingPhysReg(R))
                return R;
        }
        DBGS("REGFIND") << "Try3" << endl;
        for (auto R : RegClassInfo.getOrder(RC)) {
            //- Try allocate non-living physical reg
            if (!IsAllocByLiveVirtReg(R) && !IsLivePhysRegs(R))
                return R;
        }
        // - If not found;
        // -    Find P that currently use, spill; not in UsedInInstr
        DBGS("REGFIND") << "Try4" << endl;
        for (auto kv : LiveVirtRegs) {
            if (!IsUsedInInstr(kv.second) && !IsLivePhysRegs(kv.second)) {
                // - insert Store
                // assert(SpillMap.count(kv.first));
                invalidateAndSpill(MO, kv, MO.isKill());
                break;
            }
        }
        for (auto R : RegClassInfo.getOrder(RC)) {
            //- Try allocate non-living physical reg
            if (!IsAllocByLiveVirtReg(R) && !IsLivePhysRegs(R))
                return R;
        }
        assert(false);
    }
    int allocateStackSlot(Register R) {
        if (SpillMap.count(R))
            return SpillMap[R];
        return SpillMap[R] = MFI->CreateSpillStackObject(
                   TRI->getSpillSize(*MRI->getRegClass(R)),
                   TRI->getSpillAlign(*MRI->getRegClass(R)));
    }

    void allocateOperand(MachineOperand &MO, Register VirtReg) {
        MCRegister P;
        if (LiveVirtRegs.count(VirtReg)) {
            P = LiveVirtRegs[VirtReg];
        } else {
            DBGS("RegClass") << printRegClassOrBank(VirtReg, *MRI, TRI) << endl;
            auto RC = MRI->getRegClass(VirtReg);
            P = getUnusedPhysReg(MO, RC);

            if (MO.isUse()) {
                // - A virtual register not in LiveVirt must be Spilled.
                int Slot = allocateStackSlot(VirtReg);
                // DBGS("Reg Load") << MO;
                TII->loadRegFromStackSlot(*MO.getParent()->getParent(), MO.getParent(), P, Slot, RC, TRI);
                NumLoads++;
            }
            DBGS("allocateOperand") << "Assign " << printReg(VirtReg) << " to " << printReg(P) << endl;
            LiveVirtRegs[VirtReg] = P;
        }
        UsedInInstr.insert(P);
        if (MO.isDef()) {
            DirtyVirtReg.insert(VirtReg);
        }

        if (MO.isDead() || MO.isKill()) {
            DBGS("Erase") << printReg(VirtReg) << endl;
            LiveVirtRegs.erase(VirtReg);
        }

        // - replace use of MO

        if (MO.getSubReg()) {
            MO.setReg(TRI->getSubReg(P, MO.getSubReg()));
            MO.setSubReg(0);
        } else
            MO.setReg(P);

        DBGS("SubRegInfo") << MO.getSubReg() << endl;
    }

    void invalidateUnpreservedRegs(MachineOperand &MO) {
        for (auto kv : LiveVirtRegs) {
            if (MO.clobbersPhysReg(kv.second)) {
                // - Function call modifies this register
                // - so spill virtual reg
                // if (!MO.isKill() && !MO.isDead())
                invalidateAndSpill(MO, kv);
                DBGS("Call Invalidate") << "reg of " << printReg(kv.first) << endl;
            }
        }
    }
    void handlePhysicalOperand(MachineOperand &MO, Register PhysReg) {
        if (MO.isDef()) {
            for (auto kv : LiveVirtRegs) {
                if (overlapUnit(PhysReg, kv.second)) {
                    // - This register is defined by MO
                    // - so spill virtual reg
                    if (!MO.isKill() && !MO.isDead())
                        invalidateAndSpill(MO, kv);
                    DBGS("Phys Invalidate") << "reg of " << printReg(kv.first) << endl;
                }
            }
            LivePhysRegs.insert(PhysReg);
        }
        if (MO.isDead() || MO.isKill()) {
            LivePhysRegs.erase(PhysReg);
        }
    }

    void allocateInstruction(MachineInstr &MI) {
        DBGS("In") << MI;
        UsedInInstr.clear();
        for (auto &MO : MI.operands()) {
            if (MO.isReg()) {
                auto R = MO.getReg();
                if (R.isVirtual())
                    allocateOperand(MO, R);
                if (R.isPhysical())
                    handlePhysicalOperand(MO, R);
            }
            if (MO.isRegMask()) {
                DBGS("REGMASK") << MO << endl;
                invalidateUnpreservedRegs(MO);
            }
        }
        DBGS("Out") << MI << endl;
    }

    void preprocessBasicBlock(MachineBasicBlock &MBB) {
        LiveVirtRegs.clear();
        LivePhysRegs.clear();
        ExistingPhysRegs.clear();
        DirtyVirtReg.clear();
        for (auto args : MBB.liveins()) {
            LivePhysRegs.insert(args.PhysReg);
        }
        for (MachineInstr &I : MBB) {
            // - Probe Physical register uses
            for (auto &MO : I.operands()) {
                if (MO.isReg() && MO.getReg().isPhysical()) {
                    ExistingPhysRegs.insert(MO.getReg());
                }
            }
        }
        VirtRegAcrossFunction.clear();
        RegSet AfterFunction, BeforeFunction;

        for (MachineInstr &I : reverse(MBB)) {
            for (auto &MO : reverse(I.operands())) {
                if (MO.isReg() && MO.getReg().isVirtual()) {
                    auto rv = MO.getReg().virtRegIndex();
                    if (!BeforeFunction.contains(rv))
                        AfterFunction.insert(rv);
                    else {
                        DBGS("VirtRegAcrossFunction") << rv << endl;
                        VirtRegAcrossFunction.insert(&MO);
                    }
                }
                if (MO.isRegMask()) {
                    set_union(BeforeFunction, AfterFunction);
                    AfterFunction.clear();
                }
            }
        }
    }
    void allocateBasicBlock(MachineBasicBlock &MBB) {
        preprocessBasicBlock(MBB);
        for (MachineInstr &I : MBB) {
            allocateInstruction(I);
        }
        // - Spill all regs in LVR[]
        for (auto kv : LiveVirtRegs) {
            if (DirtyVirtReg.contains(kv.first)) {
                DBGS("DirtySpill")
                    // << *MBB.getFirstTerminator();
                    // dbgs()
                    << printReg(kv.first) << ' ' << printReg(kv.second) << endl;
                auto qwq = allocateStackSlot(kv.first);
                TII->storeRegToStackSlot(MBB, MBB.getFirstTerminator(), kv.second, true, qwq, MRI->getRegClass(kv.first), TRI);
                NumStores++;
            } else {
                DBGS("NonDirtyNoSpill") << printReg(kv.first) << ' ' << printReg(kv.second) << endl;
            }
        }
    }

    static bool set_union(RegSet &A, const RegSet &B) {
        bool changed = 0;
        for (auto q : B) {
            if (!A.contains(q)) {
                A.insert(q);
                changed = 1;
            }
        }
        return changed;
    }

    void setKillFlags(MachineFunction &MF) {
        DenseMap<MachineBasicBlock *, RegSet> ReachableVReg;
        for (auto &MBB : MF) {
            for (auto &MI : reverse(MBB)) {
                for (auto &MO : reverse(MI.operands())) {
                    if (MO.isReg() && MO.getReg().isVirtual()) {
                        if (MO.isUse())
                            ReachableVReg[&MBB].insert(MO.getReg().virtRegIndex());
                        else
                            ReachableVReg[&MBB].erase(MO.getReg().virtRegIndex());
                    }
                }
            }
        }
        for (bool changed = 1; changed;) {
            changed = 0;
            for (auto &MBB : MF) {
                auto &RS = ReachableVReg[&MBB];
                for (auto SBB : MBB.successors()) {
                    changed |= set_union(RS, ReachableVReg[SBB]);
                }
            }
        }
        for (auto &MBB : MF) {
            RegSet RS;
            for (auto SBB : MBB.successors()) {
                set_union(RS, ReachableVReg[SBB]);
            }
            for (auto &MI : reverse(MBB)) {
                for (auto &MO : reverse(MI.operands())) {
                    if (MO.isReg() && MO.getReg().isVirtual()) {
                        auto Vi = MO.getReg().virtRegIndex();
                        if (MO.isUse()) {
                            if (!RS.contains(Vi)) {
                                DBGS("Set Kill") << printReg(MO.getReg()) << " in " << MI;
                                RS.insert(Vi);
                                MO.setIsKill();
                            }
                        } else {
                            DBGS("Set UnKill") << printReg(MO.getReg()) << " in " << MI;
                            RS.erase(Vi);
                        }
                    }
                }
            }
        }
    }

    bool runOnMachineFunction(MachineFunction &MF) override {
        dbgs() << "simple regalloc running on: " << MF.getName() << "\n";

        // Get some useful information about the target
        MRI = &MF.getRegInfo();
        const TargetSubtargetInfo &STI = MF.getSubtarget();
        TRI = STI.getRegisterInfo();
        TII = STI.getInstrInfo();
        MFI = &MF.getFrameInfo();
        MRI->freezeReservedRegs(MF);
        RegClassInfo.runOnMachineFunction(MF);

        // x Subregisters? didn't check overlaps
        // x Simple function calls
        // x Physical regs
        // x no dirty no spill
        // x no spill at return
        // x don't spill after killed
        // x use callee saved register for vreg across function

        setKillFlags(MF);

        SpillMap.clear();
        // Allocate each basic block locally
        for (MachineBasicBlock &MBB : MF) {
            allocateBasicBlock(MBB);
        }

        MRI->clearVirtRegs();

        // Recompute the analyses that we marked as preserved above, you can
        // safely ignore this code
        SlotIndexes &SI = getAnalysis<SlotIndexes>();
        SI.releaseMemory();
        SI.runOnMachineFunction(MF);

        LiveIntervals &LI = getAnalysis<LiveIntervals>();
        LI.releaseMemory();
        LI.runOnMachineFunction(MF);

        return true;
    }
};
}  // namespace

/// Create the initializer and register the pass
char RegAllocSimple::ID = 0;
FunctionPass *llvm::createSimpleRegisterAllocator() { return new RegAllocSimple(); }
INITIALIZE_PASS(RegAllocSimple, "regallocsimple", "Simple Register Allocator", false, false)
static RegisterRegAlloc simpleRegAlloc("simple", "simple register allocator", createSimpleRegisterAllocator);
#undef endl
#undef DBGS