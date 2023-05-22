target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

declare dso_local i32 @printf(i8*, ...) #1
@.str.7 = private unnamed_addr constant [6 x i8] c"hello\00", align 1
define i32 @sum(i32 %v0, i32 %v2) #0 {
%v4 = alloca i32
store i32 %v0, i32* %v4
%v5 = alloca i32
store i32 %v2, i32* %v5
ret i32 0
}

define void @main() #0 {
%v7 = call i32 (i8*, ...) @printf(i8* getelementptr (i8, i8* @.str.7, i64 0, i64 0)
ret void
}

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { argmemonly nofree nounwind willreturn }
