; ModuleID = 'ternary.c'
source_filename = "ternary.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none) uwtable
define dso_local i32 @test(i32 noundef %0, i32 noundef %1, i32 noundef %2, i32 noundef %3) local_unnamed_addr #0 {
  %5 = icmp eq i32 %0, 0
  %6 = mul nsw i32 %1, 5
  %7 = add nsw i32 %1, 1
  %8 = select i1 %5, i32 %7, i32 %6
  %9 = icmp eq i32 %2, 0
  %10 = mul nsw i32 %3, 3
  %11 = add nsw i32 %3, -1
  %12 = select i1 %9, i32 %11, i32 %10
  %13 = add nsw i32 %12, %8
  ret i32 %13
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"uwtable", i32 2}
!2 = !{!"clang version 18.1.6 (Fedora 18.1.6-3.fc40)"}
