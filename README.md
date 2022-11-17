# Entity Component System in C++

This repository contains code for a simple ECS in C++.
This project does not use RTTI; it uses generic containers for components and an archetype-based type of memory structure.

## Table of Contents

* [Introduction](##introduction)
  * [Code Example](###code-example)
* [Architecture Design](##architecture-design)
  * [Archetype Memory Type](###archetype-memory-type)
  * [System Paths](###system-paths)
  * [The Two Commands](###the-two-commands)
  * [Adding/Removing Component](####adding-removing-component)
  * [Reading/Writing Components](####reading-writing-component)

## Introduction

### Code Example

```cpp
#include <ecs/ecs.h>

int main() {
    ecs::scene level_1;

    // create one instance
    ecs::entity player = level_1.make_entity();
    level_1.mutate(player).set_archetype<position>(1, 2);

    // add one component
    level_1.mutate(player).add_component<health>(10);

    // batch create instance
    ecs::entity_batch batch = level_1.make_entity(10000);
    level_1.mutate(batch).set_archetype<position, health>(1, 2, 10);

    // remove one component
    level_1.mutate(batch).remove_component<health>();

    // delete entity
    level_1.free_entity(player);
}
```

## Architecture Design

The following topics explain the problems and design decisions made in creating the ECS.

### Archetype Memory Type

The goal of ECS is to have faster throughput with fewer cache misses. The typical way to solve this is by having each 
archetype (a combination of components) hold arrays for each component. Then, when we want to query a group of 
archetypes, we access each individual container for the component values. This is pretty efficient. If we take the case
where there are $k$ components and each archetype is populated to a full cache line, then there will be $2^{(k-n)} - 1$ 
cache misses where $n$ represents the component count of the smallest archetype in the group. My problem with this 
approach is that we make the assumption that the user of the ECS will use all archetypes and all groups. This is not
always the case, nor is it the likely case.

### System Paths

Take the case where each archetype only has one entity in it. We perform the same amount of cache misses, but now we 
have way less data! This is where the idea of fast and slow paths' comes in. Say we have three components: A, B, and C. 
We will have two systems, one that cares about C and one that cares about AB. The first system has a query or grouping 
that looks like C -> AC -> BC -> ABC and the second system has one that looks like AB -> ABC. What we can do for the 
fast path is say we want to optimise C since it has the most cache misses (4). Instead of treating each archetype 
independently, we will merge their respective containers together. This gives the first system a single cache miss 
(assuming the size of component C is 1/4 or less than the size of the cache line). However, you may notice the second 
system also needs ABC, but now it needs component C. Here, instead of the archetype ABC pointing to its own container, 
it indexes into C's container and allows the second system to work with the same with only two cache misses.

When we create the path is also important. Let's say we have $n$ entities in each. That means when we resize C, we 
have to make it fit at least $4n$, and then we have to copy all the arrays over. That is why it is better to add paths 
before we add entities. This way, we can change the archetypes so that they already point to the containers in C and 
then update the pointers into C as we update the amount of entities per each archetype (in other words, we only pay the 
cost once).

### The Two Commands

An ECS can divide its use cases into two groups: adding/removing components and reading/writing components.

#### Adding/Removing Component

A general issue for archetypes is deleting consistently. The first issue with this is when. If we delete as 
instantly, we will have to index into the archetype's containers and shuffle another instance to fill the gap. This can,
obviously cause at least two cache misses. However, we don't need the deletion to be instant—we only care about it when 
a system is reading the component values. As such, we can delay the update until we read that segment of memory. 
Furthermore, we can split the reading of the component array into two groups: take the count of archetypes as $n$ with 
$m$ deletions, we can read the first $n - m$ instances and when we find a gap, index into the last $m$ instances for the
value instead and write to the gap. This way, we don't have excess reads into the component arrays when deleting. We can
use the method for when adding instances too.

There is, however, another case that we can plan for. Take for example, a particle simulation where we have thousands to
millions of entities created and destroyed. Given the resizing nature of our component containers, we will constantly be
resizing our arrays after each update (when we process the events outlined above). We can solve the resizing problem 
by allowing the user to predefine the size of the archetypes in a form of setters and getters, so we don't have to 
resize the array ever. 

Another aspect of this example, is the idea of batched lifecycles. We already know that in a particle system the user is
likely to set a timer for each particle and then delete it when it hits zero. We also know that the user is going to
create a lot of instances of an archetype at the same time. As such, we can solve this by allowing batch creation and 
deletion. This means we can send a batched event to delete so that all the gaps we will fill are contiguous meaning we
don't even have to split the reading into two groups.

#### Reading/Writing Components