; ModuleID = 'vector.c'
source_filename = "vector.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

%struct.vector = type { i32, ptr }

@minArgC = dso_local local_unnamed_addr constant i32 3, align 4
@.str = private unnamed_addr constant [8 x i8] c"MULT %d\00", align 1
@str = private unnamed_addr constant [22 x i8] c"error in input stream\00", align 1

; Function Attrs: nofree nounwind uwtable
define dso_local void @readVector(ptr nocapture noundef %0, i32 noundef %1, ptr nocapture noundef readonly %2) local_unnamed_addr #0 {
  store i32 %1, ptr %0, align 8, !tbaa !3
  %4 = sext i32 %1 to i64
  %5 = shl nsw i64 %4, 2
  %6 = tail call noalias ptr @malloc(i64 noundef %5) #9
  %7 = getelementptr inbounds %struct.vector, ptr %0, i64 0, i32 1
  store ptr %6, ptr %7, align 8, !tbaa !9
  %8 = icmp sgt i32 %1, 0
  br i1 %8, label %9, label %21

9:                                                ; preds = %3
  %10 = zext nneg i32 %1 to i64
  br label %11

11:                                               ; preds = %9, %11
  %12 = phi i64 [ 0, %9 ], [ %19, %11 ]
  %13 = getelementptr inbounds ptr, ptr %2, i64 %12
  %14 = load ptr, ptr %13, align 8, !tbaa !10
  %15 = tail call i64 @strtol(ptr nocapture noundef nonnull %14, ptr noundef null, i32 noundef 10) #10
  %16 = trunc i64 %15 to i32
  %17 = load ptr, ptr %7, align 8, !tbaa !9
  %18 = getelementptr inbounds i32, ptr %17, i64 %12
  store i32 %16, ptr %18, align 4, !tbaa !11
  %19 = add nuw nsw i64 %12, 1
  %20 = icmp eq i64 %19, %10
  br i1 %20, label %21, label %11, !llvm.loop !12

21:                                               ; preds = %11, %3
  ret void
}

; Function Attrs: mustprogress nofree nounwind willreturn allockind("alloc,uninitialized") allocsize(0) memory(inaccessiblemem: readwrite)
declare dso_local noalias noundef ptr @malloc(i64 noundef) local_unnamed_addr #1

; Function Attrs: nofree norecurse nosync nounwind memory(argmem: read) uwtable
define dso_local i32 @mult(i32 %0, ptr nocapture readonly %1, i32 %2, ptr nocapture readonly %3) local_unnamed_addr #2 {
  %5 = icmp sgt i32 %0, 0
  br i1 %5, label %6, label %47

6:                                                ; preds = %4
  %7 = zext nneg i32 %0 to i64
  %8 = icmp ult i32 %0, 8
  br i1 %8, label %33, label %9

9:                                                ; preds = %6
  %10 = and i64 %7, 2147483640
  br label %11

11:                                               ; preds = %11, %9
  %12 = phi i64 [ 0, %9 ], [ %27, %11 ]
  %13 = phi <4 x i32> [ zeroinitializer, %9 ], [ %25, %11 ]
  %14 = phi <4 x i32> [ zeroinitializer, %9 ], [ %26, %11 ]
  %15 = getelementptr inbounds i32, ptr %1, i64 %12
  %16 = getelementptr inbounds i32, ptr %15, i64 4
  %17 = load <4 x i32>, ptr %15, align 4, !tbaa !11
  %18 = load <4 x i32>, ptr %16, align 4, !tbaa !11
  %19 = getelementptr inbounds i32, ptr %3, i64 %12
  %20 = getelementptr inbounds i32, ptr %19, i64 4
  %21 = load <4 x i32>, ptr %19, align 4, !tbaa !11
  %22 = load <4 x i32>, ptr %20, align 4, !tbaa !11
  %23 = mul nsw <4 x i32> %21, %17
  %24 = mul nsw <4 x i32> %22, %18
  %25 = add <4 x i32> %23, %13
  %26 = add <4 x i32> %24, %14
  %27 = add nuw i64 %12, 8
  %28 = icmp eq i64 %27, %10
  br i1 %28, label %29, label %11, !llvm.loop !14

29:                                               ; preds = %11
  %30 = add <4 x i32> %26, %25
  %31 = tail call i32 @llvm.vector.reduce.add.v4i32(<4 x i32> %30)
  %32 = icmp eq i64 %10, %7
  br i1 %32, label %47, label %33

33:                                               ; preds = %6, %29
  %34 = phi i64 [ 0, %6 ], [ %10, %29 ]
  %35 = phi i32 [ 0, %6 ], [ %31, %29 ]
  br label %36

36:                                               ; preds = %33, %36
  %37 = phi i64 [ %45, %36 ], [ %34, %33 ]
  %38 = phi i32 [ %44, %36 ], [ %35, %33 ]
  %39 = getelementptr inbounds i32, ptr %1, i64 %37
  %40 = load i32, ptr %39, align 4, !tbaa !11
  %41 = getelementptr inbounds i32, ptr %3, i64 %37
  %42 = load i32, ptr %41, align 4, !tbaa !11
  %43 = mul nsw i32 %42, %40
  %44 = add nsw i32 %43, %38
  %45 = add nuw nsw i64 %37, 1
  %46 = icmp eq i64 %45, %7
  br i1 %46, label %47, label %36, !llvm.loop !17

47:                                               ; preds = %36, %29, %4
  %48 = phi i32 [ 0, %4 ], [ %31, %29 ], [ %44, %36 ]
  ret i32 %48
}

; Function Attrs: nounwind uwtable
define dso_local noundef i32 @main(i32 noundef %0, ptr nocapture noundef readonly %1) local_unnamed_addr #3 {
  %3 = getelementptr inbounds ptr, ptr %1, i64 2
  %4 = load ptr, ptr %3, align 8, !tbaa !10
  %5 = tail call i64 @strtol(ptr nocapture noundef nonnull %4, ptr noundef null, i32 noundef 10) #10
  %6 = trunc i64 %5 to i32
  %7 = tail call i32 @llvm.smax.i32(i32 %6, i32 0)
  %8 = getelementptr inbounds ptr, ptr %1, i64 3
  %9 = zext nneg i32 %7 to i64
  %10 = shl nuw nsw i64 %9, 2
  %11 = tail call noalias ptr @malloc(i64 noundef %10) #9
  %12 = icmp slt i32 %6, 1
  br i1 %12, label %40, label %13

13:                                               ; preds = %2, %13
  %14 = phi i64 [ %20, %13 ], [ 0, %2 ]
  %15 = getelementptr inbounds ptr, ptr %8, i64 %14
  %16 = load ptr, ptr %15, align 8, !tbaa !10
  %17 = tail call i64 @strtol(ptr nocapture noundef nonnull %16, ptr noundef null, i32 noundef 10) #10
  %18 = trunc i64 %17 to i32
  %19 = getelementptr inbounds i32, ptr %11, i64 %14
  store i32 %18, ptr %19, align 4, !tbaa !11
  %20 = add nuw nsw i64 %14, 1
  %21 = icmp eq i64 %20, %9
  br i1 %21, label %22, label %13, !llvm.loop !12

22:                                               ; preds = %13
  %23 = getelementptr ptr, ptr %1, i64 %9
  %24 = getelementptr ptr, ptr %23, i64 3
  %25 = tail call noalias ptr @malloc(i64 noundef %10) #9
  br label %26

26:                                               ; preds = %22, %26
  %27 = phi i64 [ %33, %26 ], [ 0, %22 ]
  %28 = getelementptr inbounds ptr, ptr %24, i64 %27
  %29 = load ptr, ptr %28, align 8, !tbaa !10
  %30 = tail call i64 @strtol(ptr nocapture noundef nonnull %29, ptr noundef null, i32 noundef 10) #10
  %31 = trunc i64 %30 to i32
  %32 = getelementptr inbounds i32, ptr %25, i64 %27
  store i32 %31, ptr %32, align 4, !tbaa !11
  %33 = add nuw nsw i64 %27, 1
  %34 = icmp eq i64 %33, %9
  br i1 %34, label %35, label %26, !llvm.loop !12

35:                                               ; preds = %26
  %36 = getelementptr inbounds ptr, ptr %1, i64 1
  %37 = load ptr, ptr %36, align 8, !tbaa !10
  %38 = load i8, ptr %37, align 1, !tbaa !18
  %39 = icmp eq i8 %38, 109
  br i1 %39, label %45, label %89

40:                                               ; preds = %2
  %41 = getelementptr inbounds ptr, ptr %1, i64 1
  %42 = load ptr, ptr %41, align 8, !tbaa !10
  %43 = load i8, ptr %42, align 1, !tbaa !18
  %44 = icmp eq i8 %43, 109
  br i1 %44, label %86, label %89

45:                                               ; preds = %35
  br i1 %12, label %86, label %46

46:                                               ; preds = %45
  %47 = icmp slt i32 %6, 8
  br i1 %47, label %72, label %48

48:                                               ; preds = %46
  %49 = and i64 %9, 2147483640
  br label %50

50:                                               ; preds = %50, %48
  %51 = phi i64 [ 0, %48 ], [ %66, %50 ]
  %52 = phi <4 x i32> [ zeroinitializer, %48 ], [ %64, %50 ]
  %53 = phi <4 x i32> [ zeroinitializer, %48 ], [ %65, %50 ]
  %54 = getelementptr inbounds i32, ptr %11, i64 %51
  %55 = getelementptr inbounds i32, ptr %54, i64 4
  %56 = load <4 x i32>, ptr %54, align 4, !tbaa !11
  %57 = load <4 x i32>, ptr %55, align 4, !tbaa !11
  %58 = getelementptr inbounds i32, ptr %25, i64 %51
  %59 = getelementptr inbounds i32, ptr %58, i64 4
  %60 = load <4 x i32>, ptr %58, align 4, !tbaa !11
  %61 = load <4 x i32>, ptr %59, align 4, !tbaa !11
  %62 = mul nsw <4 x i32> %60, %56
  %63 = mul nsw <4 x i32> %61, %57
  %64 = add <4 x i32> %62, %52
  %65 = add <4 x i32> %63, %53
  %66 = add nuw i64 %51, 8
  %67 = icmp eq i64 %66, %49
  br i1 %67, label %68, label %50, !llvm.loop !19

68:                                               ; preds = %50
  %69 = add <4 x i32> %65, %64
  %70 = tail call i32 @llvm.vector.reduce.add.v4i32(<4 x i32> %69)
  %71 = icmp eq i64 %49, %9
  br i1 %71, label %86, label %72

72:                                               ; preds = %46, %68
  %73 = phi i64 [ 0, %46 ], [ %49, %68 ]
  %74 = phi i32 [ 0, %46 ], [ %70, %68 ]
  br label %75

75:                                               ; preds = %72, %75
  %76 = phi i64 [ %84, %75 ], [ %73, %72 ]
  %77 = phi i32 [ %83, %75 ], [ %74, %72 ]
  %78 = getelementptr inbounds i32, ptr %11, i64 %76
  %79 = load i32, ptr %78, align 4, !tbaa !11
  %80 = getelementptr inbounds i32, ptr %25, i64 %76
  %81 = load i32, ptr %80, align 4, !tbaa !11
  %82 = mul nsw i32 %81, %79
  %83 = add nsw i32 %82, %77
  %84 = add nuw nsw i64 %76, 1
  %85 = icmp eq i64 %84, %9
  br i1 %85, label %86, label %75, !llvm.loop !20

86:                                               ; preds = %75, %68, %40, %45
  %87 = phi i32 [ 0, %45 ], [ 0, %40 ], [ %70, %68 ], [ %83, %75 ]
  %88 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str, i32 noundef %87)
  ret i32 0

89:                                               ; preds = %40, %35
  %90 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str)
  tail call void @exit(i32 noundef 1) #11
  unreachable
}

; Function Attrs: nofree nounwind
declare dso_local noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #4

; Function Attrs: noreturn nounwind
declare dso_local void @exit(i32 noundef) local_unnamed_addr #5

; Function Attrs: mustprogress nofree nounwind willreturn
declare dso_local i64 @strtol(ptr noundef readonly, ptr nocapture noundef, i32 noundef) local_unnamed_addr #6

; Function Attrs: nofree nounwind
declare noundef i32 @puts(ptr nocapture noundef readonly) local_unnamed_addr #7

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @llvm.smax.i32(i32, i32) #8

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @llvm.vector.reduce.add.v4i32(<4 x i32>) #8

attributes #0 = { nofree nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { mustprogress nofree nounwind willreturn allockind("alloc,uninitialized") allocsize(0) memory(inaccessiblemem: readwrite) "alloc-family"="malloc" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { nofree norecurse nosync nounwind memory(argmem: read) uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { nofree nounwind "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #5 = { noreturn nounwind "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #6 = { mustprogress nofree nounwind willreturn "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #7 = { nofree nounwind }
attributes #8 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
attributes #9 = { nounwind allocsize(0) }
attributes #10 = { nounwind }
attributes #11 = { noreturn nounwind }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"uwtable", i32 2}
!2 = !{!"clang version 18.1.6 (Fedora 18.1.6-3.fc40)"}
!3 = !{!4, !5, i64 0}
!4 = !{!"", !5, i64 0, !8, i64 8}
!5 = !{!"int", !6, i64 0}
!6 = !{!"omnipotent char", !7, i64 0}
!7 = !{!"Simple C/C++ TBAA"}
!8 = !{!"any pointer", !6, i64 0}
!9 = !{!4, !8, i64 8}
!10 = !{!8, !8, i64 0}
!11 = !{!5, !5, i64 0}
!12 = distinct !{!12, !13}
!13 = !{!"llvm.loop.mustprogress"}
!14 = distinct !{!14, !13, !15, !16}
!15 = !{!"llvm.loop.isvectorized", i32 1}
!16 = !{!"llvm.loop.unroll.runtime.disable"}
!17 = distinct !{!17, !13, !16, !15}
!18 = !{!6, !6, i64 0}
!19 = distinct !{!19, !13, !15, !16}
!20 = distinct !{!20, !13, !16, !15}
