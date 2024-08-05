; ModuleID = 'ex_cpp.cpp'
source_filename = "ex_cpp.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.closure_type = type { ptr, ptr }

; Function Attrs: mustprogress noinline nounwind optnone
define dso_local noundef double @_Z4sqrtd(double noundef %x) #0 {
entry:
  %x.addr = alloca double, align 8
  store double %x, ptr %x.addr, align 8
  %0 = load double, ptr %x.addr, align 8
  %div = fdiv double %0, 1.000000e+02
  ret double %div
}

; Function Attrs: mustprogress noinline nounwind optnone
define dso_local noundef double @_Z9wrap_sqrtP8env_typed(ptr noundef %env, double noundef %x) #0 {
entry:
  %env.addr = alloca ptr, align 8
  %x.addr = alloca double, align 8
  store ptr %env, ptr %env.addr, align 8
  store double %x, ptr %x.addr, align 8
  %0 = load double, ptr %x.addr, align 8
  %call = call noundef double @_Z4sqrtd(double noundef %0)
  ret double %call
}

; Function Attrs: mustprogress noinline nounwind optnone
define dso_local noundef double @_Z5twiceP8env_type12closure_typed(ptr noundef %env, ptr %fnclosure.coerce0, ptr %fnclosure.coerce1, double noundef %x) #0 {
entry:
  %fnclosure = alloca %struct.closure_type, align 8
  %env.addr = alloca ptr, align 8
  %x.addr = alloca double, align 8
  %tmp1 = alloca double, align 8
  %tmp2 = alloca double, align 8
  %0 = getelementptr inbounds { ptr, ptr }, ptr %fnclosure, i32 0, i32 0
  store ptr %fnclosure.coerce0, ptr %0, align 8
  %1 = getelementptr inbounds { ptr, ptr }, ptr %fnclosure, i32 0, i32 1
  store ptr %fnclosure.coerce1, ptr %1, align 8
  store ptr %env, ptr %env.addr, align 8
  store double %x, ptr %x.addr, align 8
  %fnptr = getelementptr inbounds %struct.closure_type, ptr %fnclosure, i32 0, i32 0
  %2 = load ptr, ptr %fnptr, align 8
  %envptr = getelementptr inbounds %struct.closure_type, ptr %fnclosure, i32 0, i32 1
  %3 = load ptr, ptr %envptr, align 8
  %4 = load double, ptr %x.addr, align 8
  %call = call noundef double %2(ptr noundef %3, double noundef %4)
  store double %call, ptr %tmp1, align 8
  %fnptr1 = getelementptr inbounds %struct.closure_type, ptr %fnclosure, i32 0, i32 0
  %5 = load ptr, ptr %fnptr1, align 8
  %envptr2 = getelementptr inbounds %struct.closure_type, ptr %fnclosure, i32 0, i32 1
  %6 = load ptr, ptr %envptr2, align 8
  %7 = load double, ptr %tmp1, align 8
  %call3 = call noundef double %5(ptr noundef %6, double noundef %7)
  store double %call3, ptr %tmp2, align 8
  %8 = load double, ptr %tmp2, align 8
  ret double %8
}

; Function Attrs: mustprogress noinline nounwind optnone
define dso_local { ptr, ptr } @_Z17make_some_closurev() #0 {
entry:
  %retval = alloca %struct.closure_type, align 8
  %fnptr = getelementptr inbounds %struct.closure_type, ptr %retval, i32 0, i32 0
  store ptr @_Z9wrap_sqrtP8env_typed, ptr %fnptr, align 8
  %envptr = getelementptr inbounds %struct.closure_type, ptr %retval, i32 0, i32 1
  store ptr null, ptr %envptr, align 8
  %0 = load { ptr, ptr }, ptr %retval, align 8
  ret { ptr, ptr } %0
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone
define dso_local noundef i32 @main() #1 {
entry:
  %closure = alloca %struct.closure_type, align 8
  %y = alloca double, align 8
  %agg.tmp = alloca %struct.closure_type, align 8
  %call = call { ptr, ptr } @_Z17make_some_closurev()
  %0 = getelementptr inbounds { ptr, ptr }, ptr %closure, i32 0, i32 0
  %1 = extractvalue { ptr, ptr } %call, 0
  store ptr %1, ptr %0, align 8
  %2 = getelementptr inbounds { ptr, ptr }, ptr %closure, i32 0, i32 1
  %3 = extractvalue { ptr, ptr } %call, 1
  store ptr %3, ptr %2, align 8
  call void @llvm.memcpy.p0.p0.i64(ptr align 8 %agg.tmp, ptr align 8 %closure, i64 16, i1 false)
  %4 = getelementptr inbounds { ptr, ptr }, ptr %agg.tmp, i32 0, i32 0
  %5 = load ptr, ptr %4, align 8
  %6 = getelementptr inbounds { ptr, ptr }, ptr %agg.tmp, i32 0, i32 1
  %7 = load ptr, ptr %6, align 8
  %call1 = call noundef double @_Z5twiceP8env_type12closure_typed(ptr noundef null, ptr %5, ptr %7, double noundef 4.000000e+00)
  store double %call1, ptr %y, align 8
  ret i32 0
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #2

attributes #0 = { mustprogress noinline nounwind optnone "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-features"="+cx8,+mmx,+sse,+sse2,+x87" }
attributes #1 = { mustprogress noinline norecurse nounwind optnone "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-features"="+cx8,+mmx,+sse,+sse2,+x87" }
attributes #2 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 18.1.5"}
