# xo-alloc -- arena allocator and incremental garbage collector

# Rules for writing garbage-collected classes.

Topics
* allocation  - allocate Objects (inheriting xo::Object) before owned scratch space.
                 Can relax this if/when abandon the bad-for-locality use of two pointers
                 into to-space to keep track of grey objects.  Want to use stack anyway
                 so we can do depth-first search.
* destructors - can omit except for finalization
* assignment   - MUST USE Object::assign_member() to assign pointers to gc-owned memory.
                 Only necessary for old->new pointers,  so don't need to worry about this
                 for initialization.
* finalization - not supported (yet)

- padding - use IAlloc::with_padding(z) for hand-allocated objects.
