cmake-facet-export
==================

:date: 2026-02

Background
----------

``xo_add_genfacetimpl()`` in ``xo_cxx.cmake`` uses ``genfacet`` to regenerate facet
implementation boilerplate from ``.json5`` IDL files.  When ``FACET_PKG`` is specified
(e.g. ``FACET_PKG xo_gc``), the macro resolves the IDL directory via a cmake custom target::

    get_target_property(_facet_dir share_${GF_FACET_PKG} path)

``share_${PROJECT_NAME}`` is a custom target created by ``xo_cxx_toplevel_options2()`` for
every xo satellite, with its ``path`` property set to ``${PROJECT_SOURCE_DIR}`` (the package
source root). Subdirectory ``idl/`` within that root holds the facet IDL files.

Problem
-------

Custom cmake targets are not exportable through the standard ``install(TARGETS ... EXPORT ...)``
mechanism.  As a result, ``share_xo_gc`` and similar targets do not exist when a satellite is
consumed as an installed dependency (e.g. in a standalone nix build or any build that uses
``find_package(xo_gc)`` rather than building xo-gc in the same cmake invocation).

This caused ``nix-build -A xo.object2`` to fail at cmake configure time with::

    CMake Error: get_target_property() called with non-existent target "share_xo_gc".

xo-gc was unaffected because it uses only ``xo_add_genfacet()`` (which does not reference
``share_*`` targets), while xo-object2 uses ``xo_add_genfacetimpl()`` with ``FACET_PKG``.

Immediate fix (2026-02)
-----------------------

A guard was added to ``xo_add_genfacetimpl()`` to skip target creation gracefully when the
``share_*`` target is absent, since all callers annotate these as
``# note: manual target; generated code committed to git``::

    if(NOT TARGET share_${GF_FACET_PKG})
        message(STATUS "xo_add_genfacetimpl: share_${GF_FACET_PKG} not available; skipping")
        return()
    endif()

This unblocked the nix build while leaving a forward path to full support.

Long-term design
----------------

To make ``xo_add_genfacetimpl()`` fully functional in standalone builds:

1. **Export ``share_`` targets universally** — ``xo_export_cmake_config()`` generates and
   installs a ``${PROJECT_NAME}Share.cmake`` file for every satellite.  This file recreates
   ``share_${PROJECT_NAME}`` pointing to the installed data root::

       if(NOT TARGET share_xo_gc)
           add_custom_target(share_xo_gc)
           set_property(TARGET share_xo_gc PROPERTY path
               "${CMAKE_CURRENT_LIST_DIR}/../../../share/xo_gc")
       endif()

   Doing this universally (not only for facet-providers) keeps ``*Config.cmake.in`` templates
   consistent and provides future-proofing for other attributes that may be added to
   ``share_`` targets.

2. **Include the Share file** — every ``cmake/${pkg}Config.cmake.in`` adds::

       include("${CMAKE_CURRENT_LIST_DIR}/@PROJECT_NAME@Share.cmake")

3. **Install IDL files** — packages whose IDL is consumed by ``xo_add_genfacetimpl()``
   install their ``idl/`` tree::

       install(DIRECTORY idl/
           DESTINATION share/${PROJECT_NAME}/idl
           FILES_MATCHING PATTERN "*.json5")

   This maps to the same relative path that ``genfacet`` would find under the source root.

The skip-if-absent guard is retained as graceful degradation for older installed packages
that predate this change.
