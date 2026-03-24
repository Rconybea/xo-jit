Uses arena allocators for fast+efficient parsing.

Composition of nested state machines.

## SchematikaParser

Parser to convert schematika text to expressions.

### Details

Partial GCObject facet support, so a SchmeatikaParser instance can
be a gc root.

## SyntaxStateMachine

a state machine dedicated to some particular Schematika syntax.
Examples: if-expression, type declaration, function call

## DToplevelSeqSsm

top-level expression sequence
