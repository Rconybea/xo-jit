# xo-interpreter2/CMakeLists.txt

cmake_minimum_required(VERSION 3.10)

project(xo_interpreter2 VERSION 1.0)
enable_language(CXX)

include(GNUInstallDirs)
include(cmake/xo-bootstrap-macros.cmake)

xo_cxx_toplevel_options3()

# ----------------------------------------------------------------
# c++ settings

# one-time project-specific c++ flags. usually empty
set(PROJECT_CXX_FLAGS "")
add_definitions(${PROJECT_CXX_FLAGS})

# ----------------------------------------------------------------
# output targets

add_subdirectory(src/interpreter2)
add_subdirectory(src/skrepl)
add_subdirectory(utest)

# ----------------------------------------------------------------

# only supporting forward_children(), for virtual root.
xo_add_genfacetimpl(
    TARGET xo-interpreter2-facetimpl-gcobject-virtualschematikamachine
    FACET_PKG xo_alloc2
    INPUT idl/IGCObject_DVirtualSchematikaMachine.json5
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-interpreter2-facetimpl-gcobject-vsmdefcontframe
    FACET_PKG xo_alloc2
    INPUT idl/IGCObject_DVsmDefContFrame.json5
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-interpreter2-facetimpl-printable-vsmdefcontframe
    FACET_PKG xo_printable2
    INPUT idl/IPrintable_DVsmDefContFrame.json5
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-interpreter2-facetimpl-gcobject-vsmapplyframe
    FACET_PKG xo_alloc2
    INPUT idl/IGCObject_DVsmApplyFrame.json5
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-interpreter2-facetimpl-printable-vsmapplyframe
    FACET_PKG xo_printable2
    INPUT idl/IPrintable_DVsmApplyFrame.json5
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-interpreter2-facetimpl-gcobject-vsmevalargsframe
    FACET_PKG xo_alloc2
    INPUT idl/IGCObject_DVsmEvalArgsFrame.json5
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-interpreter2-facetimpl-printable-vsmevalargsframe
    FACET_PKG xo_printable2
    INPUT idl/IPrintable_DVsmEvalArgsFrame.json5
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-interpreter2-facetimpl-gcobject-vsmapplyclosureframe
    FACET_PKG xo_alloc2
    INPUT idl/IGCObject_DVsmApplyClosureFrame.json5
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-interpreter2-facetimpl-printable-vsmapplyclosureframe
    FACET_PKG xo_printable2
    INPUT idl/IPrintable_DVsmApplyClosureFrame.json5
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-interpreter2-facetimpl-gcobject-vsmifelsecontframe
    FACET_PKG xo_alloc2
    INPUT idl/IGCObject_DVsmIfElseContFrame.json5
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-interpreter2-facetimpl-printable-vsmifelsecontframe
    FACET_PKG xo_printable2
    INPUT idl/IPrintable_DVsmIfElseContFrame.json5
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-interpreter2-facetimpl-gcobject-vsmseqcontframe
    FACET_PKG xo_alloc2
    INPUT idl/IGCObject_DVsmSeqContFrame.json5
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-interpreter2-facetimpl-printable-vsmseqcontframe
    FACET_PKG xo_printable2
    INPUT idl/IPrintable_DVsmSeqContFrame.json5
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
#
#xo_add_genfacetimpl(
#    TARGET xo-interpreter2-facetimpl-procedure-closure
#    FACET_PKG xo_procedure2
#    REPR Closure
#    INPUT idl/IProcedure_DClosure.json5
#)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-interpreter2-facetimpl-gcobject-closure
    FACET_PKG xo_alloc2
    INPUT idl/IGCObject_DClosure.json5
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-interpreter2-facetimpl-printable-closure
    FACET_PKG xo_printable2
    INPUT idl/IPrintable_DClosure.json5
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-interpreter2-facetimpl-gcobject-localenv
    FACET_PKG xo_alloc2
    INPUT idl/IGCObject_DLocalEnv.json5
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-interpreter2-facetimpl-printable-localenv
    FACET_PKG xo_printable2
    INPUT idl/IPrintable_DLocalEnv.json5
)

# ----------------------------------------------------------------

xo_add_genfacetimpl(
    TARGET xo-interpreter2-facetimpl-runtimecontext-vsmrcx
    FACET_PKG xo_procedure2
    INPUT idl/IRuntimeContext_DVsmRcx.json5
    )

# ----------------------------------------------------------------

xo_add_genfacet_all(xo-interpreter2-genfacet-all)

# ----------------------------------------------------------------
# cmake helper (for external xo-interpreter2 users)

xo_export_cmake_config(${PROJECT_NAME} ${PROJECT_VERSION} ${PROJECT_NAME}Targets)

# ----------------------------------------------------------------
# input dependencies
#
# NOTE: dependency set here must be kept consistent with
#       xo-interpreter2/cmake/xo_interpreter2Config.cmake.in

#xo_headeronly_dependency(${SELF_LIB} xo_flatstring)

# end CMakeLists.txt
