define double @root4(ptr %.env, double %x) {
entry:
  %x1 = alloca double, align 8
  store double %x, ptr %x1, align 8
  %x2 = load double, ptr %x1, align 8
  %calltmp = call double @w.sqrt(ptr null, double %x2)
  %calltmp3 = call double @w.sqrt(ptr null, double %calltmp)
  ret double %calltmp3
}

; ----------------------------------------------------------------

define double @twice(ptr %.env, { ptr, ptr } %f, double %x) {
entry:
  %f1 = alloca { ptr, ptr }, align 8
  store { ptr, ptr } %f, ptr %f1, align 8
  %x2 = alloca double, align 8
  store double %x, ptr %x2, align 8
  %f3 = load { ptr, ptr }, ptr %f1, align 8
  %fnptr = extractvalue { ptr, ptr } %f3, 0
  %envptr = extractvalue { ptr, ptr } %f3, 1
  %f4 = load { ptr, ptr }, ptr %f1, align 8
  %fnptr5 = extractvalue { ptr, ptr } %f4, 0
  %envptr6 = extractvalue { ptr, ptr } %f4, 1
  %x7 = load double, ptr %x2, align 8
  %calltmp = call double %fnptr5(ptr %envptr6, double %x7)
  %calltmp8 = call double %fnptr(ptr %envptr, double %calltmp)
  ret double %calltmp8
}


define double @twice(ptr %.env, { ptr, ptr } %f, double %x) {
entry:
  %f1 = alloca { ptr, ptr }, align 8
  %f.elt = extractvalue { ptr, ptr } %f, 0
  store ptr %f.elt, ptr %f1, align 8
  %f1.repack9 = getelementptr inbounds { ptr, ptr }, ptr %f1, i64 0, i32 1
  %f.elt10 = extractvalue { ptr, ptr } %f, 1
  store ptr %f.elt10, ptr %f1.repack9, align 8
  %calltmp = call double %f.elt(ptr %f.elt10, double %x)
  %calltmp8 = call double %f.elt(ptr %f.elt10, double %calltmp)
  ret double %calltmp8
}


;; ----------------------------------------------------------------

define double @root_2x(ptr %.env, double %x2) {
entry:
  %x21 = alloca double, align 8
  store double %x2, ptr %x21, align 8
  %envptr = insertvalue { ptr, ptr } { ptr @twice, ptr undef }, ptr %.env, 1
  %fnptr = extractvalue { ptr, ptr } %envptr, 0
  %envptr2 = extractvalue { ptr, ptr } %envptr, 1
  %x23 = load double, ptr %x21, align 8
  %calltmp = call double %fnptr(ptr %envptr2, { ptr, ptr } { ptr @w.sqrt, ptr null }, double %x23)
  ret double %calltmp
}
