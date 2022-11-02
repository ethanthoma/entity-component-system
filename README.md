# Entity Component System in C++

This repository contains code for a simple ECS in C++.
This project does not use RTTI; it uses generic containers for components and a different type of memory structure.

## ECS Memory Type

### Common Types

From what I've seen, there are two common memory systems:
- First method is to store all components in a single table where entities are treated as row indices and components as column indices (or vice-versa). This has the apparent problem of sparse memory—making the cache benefits of ECS less consistent. 
- The second method I've seen groups entities into archetypes (a unique set of components) and creates tables for each. This solves the sparsity issue of the above method but is less memory efficient as it duplicates subset archetypes.

### New Type

Given that I didn't like these two methods, this system uses a third variation.

1) Group the components by archetype on a single table ordered by larger archetype (i.e., more types of components);
2) Shuffle any gaps in a column and have the archetype keep track of the offset;
3) When a component gets added or removed, move the entities components' data to the new archetype in memory and shuffle the other entities.

This method allows all the data to be stored contiguously and is memory-efficient. 

### Design Issues

This design has two issues: how to shuffle the archetypes efficiently and how to track entities to components.

For the first issue, there is a somewhat simple solution. Say we have two components, A and B. The three possible archetypes (assuming no repeat components are allowed) are AB, A, and B, and let's say that this is the order they are stored in. Take an entity with component A and say we add component B to it. Since AB is ahead of A, we add the entity's components to the end of AB in memory, which now overlaps with the beginning of A in memory. Since we are moving the first component of A and we have a gap in A's memory from moving the previous entity's component, we can move the beginning of A to it. Now we have an overlap in B, but we have no gap. Instead, we move the beginning of B to the end of B. Voila! With three moves, we shuffled all the components we needed to.

For the second issue, the solution is a little more complex. A simple way is to simply store the entity ID with the component values. This, however, adds extra, wasted size to every component and seemingly punishes users for having many small components over large components. The solution presented here is a little different. Since we need to know what components an entity has, we already have a mapping from entity to archetype. Instead of having an unordered list for all the entities of one archetype, it stores an ordered list ordered by where the entity is within the archetype. Since the archetype tracks the offset into the component arrays, we now have a mapping from the entity to its components. Simple!
