; ModuleID = 'vector.c'
source_filename = "vector.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

%struct.vector = type { i32, ptr }

@minArgC = dso_local constant i32 3, align 4
@.str = private unnamed_addr constant [8 x i8] c"MULT %d\00", align 1
@.str.1 = private unnamed_addr constant [23 x i8] c"error in input stream\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @readVector(ptr noundef %0, i32 noundef %1, ptr noundef %2) #0 {
  %4 = alloca ptr, align 8
  %5 = alloca i32, align 4
  %6 = alloca ptr, align 8
  %7 = alloca i32, align 4
  store ptr %0, ptr %4, align 8
  store i32 %1, ptr %5, align 4
  store ptr %2, ptr %6, align 8
  %8 = load i32, ptr %5, align 4
  %9 = load ptr, ptr %4, align 8
  %10 = getelementptr inbounds %struct.vector, ptr %9, i32 0, i32 0
  store i32 %8, ptr %10, align 8
  %11 = load i32, ptr %5, align 4
  %12 = sext i32 %11 to i64
  %13 = mul i64 %12, 4
  %14 = call noalias ptr @malloc(i64 noundef %13) #5
  %15 = load ptr, ptr %4, align 8
  %16 = getelementptr inbounds %struct.vector, ptr %15, i32 0, i32 1
  store ptr %14, ptr %16, align 8
  store i32 0, ptr %7, align 4
  br label %17

17:                                               ; preds = %34, %3
  %18 = load i32, ptr %7, align 4
  %19 = load i32, ptr %5, align 4
  %20 = icmp slt i32 %18, %19
  br i1 %20, label %21, label %37

21:                                               ; preds = %17
  %22 = load ptr, ptr %6, align 8
  %23 = load i32, ptr %7, align 4
  %24 = sext i32 %23 to i64
  %25 = getelementptr inbounds ptr, ptr %22, i64 %24
  %26 = load ptr, ptr %25, align 8
  %27 = call i32 @atoi(ptr noundef %26) #6
  %28 = load ptr, ptr %4, align 8
  %29 = getelementptr inbounds %struct.vector, ptr %28, i32 0, i32 1
  %30 = load ptr, ptr %29, align 8
  %31 = load i32, ptr %7, align 4
  %32 = sext i32 %31 to i64
  %33 = getelementptr inbounds i32, ptr %30, i64 %32
  store i32 %27, ptr %33, align 4
  br label %34

34:                                               ; preds = %21
  %35 = load i32, ptr %7, align 4
  %36 = add nsw i32 %35, 1
  store i32 %36, ptr %7, align 4
  br label %17, !llvm.loop !4

37:                                               ; preds = %17
  ret void
}

; Function Attrs: nounwind allocsize(0)
declare dso_local noalias ptr @malloc(i64 noundef) #1

; Function Attrs: nounwind willreturn memory(read)
declare dso_local i32 @atoi(ptr noundef) #2

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @mult(i32 %0, ptr %1, i32 %2, ptr %3) #0 {
  %5 = alloca %struct.vector, align 8
  %6 = alloca %struct.vector, align 8
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = getelementptr inbounds { i32, ptr }, ptr %5, i32 0, i32 0
  store i32 %0, ptr %9, align 8
  %10 = getelementptr inbounds { i32, ptr }, ptr %5, i32 0, i32 1
  store ptr %1, ptr %10, align 8
  %11 = getelementptr inbounds { i32, ptr }, ptr %6, i32 0, i32 0
  store i32 %2, ptr %11, align 8
  %12 = getelementptr inbounds { i32, ptr }, ptr %6, i32 0, i32 1
  store ptr %3, ptr %12, align 8
  store i32 0, ptr %8, align 4
  store i32 0, ptr %7, align 4
  br label %13

13:                                               ; preds = %34, %4
  %14 = load i32, ptr %7, align 4
  %15 = getelementptr inbounds %struct.vector, ptr %5, i32 0, i32 0
  %16 = load i32, ptr %15, align 8
  %17 = icmp slt i32 %14, %16
  br i1 %17, label %18, label %37

18:                                               ; preds = %13
  %19 = load i32, ptr %8, align 4
  %20 = getelementptr inbounds %struct.vector, ptr %5, i32 0, i32 1
  %21 = load ptr, ptr %20, align 8
  %22 = load i32, ptr %7, align 4
  %23 = sext i32 %22 to i64
  %24 = getelementptr inbounds i32, ptr %21, i64 %23
  %25 = load i32, ptr %24, align 4
  %26 = getelementptr inbounds %struct.vector, ptr %6, i32 0, i32 1
  %27 = load ptr, ptr %26, align 8
  %28 = load i32, ptr %7, align 4
  %29 = sext i32 %28 to i64
  %30 = getelementptr inbounds i32, ptr %27, i64 %29
  %31 = load i32, ptr %30, align 4
  %32 = mul nsw i32 %25, %31
  %33 = add nsw i32 %19, %32
  store i32 %33, ptr %8, align 4
  br label %34

34:                                               ; preds = %18
  %35 = load i32, ptr %7, align 4
  %36 = add nsw i32 %35, 1
  store i32 %36, ptr %7, align 4
  br label %13, !llvm.loop !6

37:                                               ; preds = %13
  %38 = load i32, ptr %8, align 4
  ret i32 %38
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main(i32 noundef %0, ptr noundef %1) #0 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca ptr, align 8
  %6 = alloca %struct.vector, align 8
  %7 = alloca %struct.vector, align 8
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  store i32 0, ptr %3, align 4
  store i32 %0, ptr %4, align 4
  store ptr %1, ptr %5, align 8
  %10 = load ptr, ptr %5, align 8
  %11 = getelementptr inbounds ptr, ptr %10, i64 2
  %12 = load ptr, ptr %11, align 8
  %13 = call i32 @atoi(ptr noundef %12) #6
  %14 = icmp sge i32 %13, 0
  br i1 %14, label %15, label %20

15:                                               ; preds = %2
  %16 = load ptr, ptr %5, align 8
  %17 = getelementptr inbounds ptr, ptr %16, i64 2
  %18 = load ptr, ptr %17, align 8
  %19 = call i32 @atoi(ptr noundef %18) #6
  store i32 %19, ptr %8, align 4
  br label %21

20:                                               ; preds = %2
  store i32 0, ptr %8, align 4
  br label %21

21:                                               ; preds = %20, %15
  %22 = load i32, ptr %8, align 4
  %23 = load ptr, ptr %5, align 8
  %24 = getelementptr inbounds ptr, ptr %23, i64 3
  call void @readVector(ptr noundef %6, i32 noundef %22, ptr noundef %24)
  %25 = load i32, ptr %8, align 4
  %26 = load ptr, ptr %5, align 8
  %27 = load i32, ptr %8, align 4
  %28 = add nsw i32 3, %27
  %29 = sext i32 %28 to i64
  %30 = getelementptr inbounds ptr, ptr %26, i64 %29
  call void @readVector(ptr noundef %7, i32 noundef %25, ptr noundef %30)
  %31 = load ptr, ptr %5, align 8
  %32 = getelementptr inbounds ptr, ptr %31, i64 1
  %33 = load ptr, ptr %32, align 8
  %34 = load i8, ptr %33, align 1
  %35 = sext i8 %34 to i32
  switch i32 %35, label %48 [
    i32 109, label %36
  ]

36:                                               ; preds = %21
  %37 = getelementptr inbounds { i32, ptr }, ptr %6, i32 0, i32 0
  %38 = load i32, ptr %37, align 8
  %39 = getelementptr inbounds { i32, ptr }, ptr %6, i32 0, i32 1
  %40 = load ptr, ptr %39, align 8
  %41 = getelementptr inbounds { i32, ptr }, ptr %7, i32 0, i32 0
  %42 = load i32, ptr %41, align 8
  %43 = getelementptr inbounds { i32, ptr }, ptr %7, i32 0, i32 1
  %44 = load ptr, ptr %43, align 8
  %45 = call i32 @mult(i32 %38, ptr %40, i32 %42, ptr %44)
  store i32 %45, ptr %9, align 4
  %46 = load i32, ptr %9, align 4
  %47 = call i32 (ptr, ...) @printf(ptr noundef @.str, i32 noundef %46)
  br label %50

48:                                               ; preds = %21
  %49 = call i32 (ptr, ...) @printf(ptr noundef @.str.1)
  call void @exit(i32 noundef 1) #7
  unreachable

50:                                               ; preds = %36
  ret i32 0
}

declare dso_local i32 @printf(ptr noundef, ...) #3

; Function Attrs: noreturn nounwind
declare dso_local void @exit(i32 noundef) #4

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nounwind allocsize(0) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { nounwind willreturn memory(read) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { noreturn nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #5 = { nounwind allocsize(0) }
attributes #6 = { nounwind willreturn memory(read) }
attributes #7 = { noreturn nounwind }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"uwtable", i32 2}
!2 = !{i32 7, !"frame-pointer", i32 2}
!3 = !{!"clang version 18.1.6 (Fedora 18.1.6-3.fc40)"}
!4 = distinct !{!4, !5}
!5 = !{!"llvm.loop.mustprogress"}
!6 = distinct !{!6, !5}
